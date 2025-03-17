#include "initialisations.h"

#define NUM_WIFI_RETRY 1

#ifdef DO_THE_THING
/* Wifi Thread entry function */
/* pvParameters contains TaskHandle_t */
void wifi_thread_entry(void *pvParameters){
    FSP_PARAMETER_NOT_USED (pvParameters);
    EventBits_t uxBits;
    volatile fsp_err_t status = FSP_ERR_ASSERTION;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 0: wifi_thread_entry()",RTT_TIMESTAMP());

    /* Open RA6M3 WIFI UART Port */
    status = wifi_uart_init();

    /* Clear <<g_wifi_event>> Bits in <<MQTT_FLAG_ALL>>. */
    uxBits = xEventGroupClearBits( g_wifi_event,    /* The event group being updated. */
                                   MQTT_FLAG_ALL ); /* The bits being cleared. */

    /* Set Wi-Fi Event Flags: DISCONNECTED */
    xEventGroupSetBits(g_wifi_event, MQTT_FLAG_DISCONNECTED);

    /* TODO: add your own code here */
    while (1)  {

        /* Wait for <<g_wifi_even>> == <<MQTT_FLAG_DISCONNECTED>> */
        uxBits = xEventGroupWaitBits(  g_wifi_event,           // The event group handle
                                       MQTT_FLAG_DISCONNECTED, // Bits to wait for
                                       pdTRUE,                 // Clear bits on exit
                                       pdTRUE,                 // Wait for both bits (logical AND)
                                       portMAX_DELAY           // Wait indefinitely
                                       );

        /* Synchronize with UART/SSIDPW Thread */
        xSemaphoreTake(g_wifi_semaphore,portMAX_DELAY);

        /* Start of Wi-Fi Thread Task */
        APP_PRINT("\r\n%s Inside Wifi_thread",RTT_TIMESTAMP());

        /* (1) Read SSIDPW From Flash */
        memset(getWifiParamAddr(WIFI_SSID),0,MAX_STR_SSID__PW);
        memset(getWifiParamAddr(WIFI_PASSWORD),0,MAX_STR_SSID__PW);
        DF_Read_SSID_Str((char*)getWifiParamAddr(WIFI_SSID));
        DF_Read_PW_Str((char*)getWifiParamAddr(WIFI_PASSWORD));

        /* (2) Setup Wi-Fi: Send Command to DA16200: with <<SSID>> and <<PWD>> */
        //status= Da16200_masterSetup(SSID,PWD);
        for (uint8_t ii =0; ii<NUM_WIFI_RETRY ; ii++){
            status= Da16200_masterSetup((char*)getWifiParamAddr(WIFI_SSID),(char*)getWifiParamAddr(WIFI_PASSWORD));
            if (status == FSP_SUCCESS)
                break;
        } // end for ii=0:NUM_WIFI_RETRY-1

        /* Clear <<g_wifi_event>> Bits in <<MQTT_FLAG_ALL>>. */
        uxBits = xEventGroupClearBits( g_wifi_event,    /* The event group being updated. */
                                       MQTT_FLAG_ALL ); /* The bits being cleared. */

        /* (3) Update <<g_wifi_event>> Accordingly */
        if (status == FSP_SUCCESS){
            xEventGroupSetBits(g_wifi_event, MQTT_FLAG_CONNECTED);

            /* Write To RTT Segger for Print Out */
            APP_PRINT("\r\n%s Wi-Fi & MQTT Setup Successful (%s,%s)!!!",RTT_TIMESTAMP(),(char*)getWifiParamAddr(WIFI_SSID),(char*)getWifiParamAddr(WIFI_PASSWORD));

            /* Publish SSID Information to Cloud: NOTE must - sizeof(diagStr)<90 Bytes */
            char diagStr[64]; snprintf(diagStr, sizeof(diagStr) - 1, "Wifi: %s Ch: %i", (char*)getWifiParamAddr(WIFI_SSID), 99);
            publishSysDiagnostic(DNAME,diagStr,8,4);

            /* Publish Firmware Version to Cloud */
            publishSysVersion(DNAME, MMP_MAJOR_VERSION, MMP_MINOR_VERSION, MMP_PATCH,8,4);

            /* Publish Request to Get KMDC Threshold Value */
            publishKMDCThresRequest(DNAME,8,4);

        } // end if (FSP_SUCCESS)
        else{
            xEventGroupSetBits(g_wifi_event, MQTT_FLAG_DISCONNECTED);
            APP_PRINT("\r\n%s Wi-Fi & MQTT Setup Not Successful (%s,%s)...",RTT_TIMESTAMP(),(char*)getWifiParamAddr(WIFI_SSID),(char*)getWifiParamAddr(WIFI_PASSWORD));
        } // end if (!FSP_SUCCESS)

        /* (4) Pass back to UART_THREAD */
        xEventGroupSetBits(g_sync_event, UART_THREAD);

        //vTaskDelay (1);
    } // end while(1);

    FSP_PARAMETER_NOT_USED (uxBits);
    FSP_PARAMETER_NOT_USED (status);

} // end wifi_thread_entry()

#else
void wifi_thread_entry(void *pvParameters){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    while (1)
    {
        vTaskDelay (1);
    }
    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (pvParameters);

}
#endif
