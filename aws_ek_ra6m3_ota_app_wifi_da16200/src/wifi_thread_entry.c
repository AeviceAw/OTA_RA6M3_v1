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

#else CHECKPOINT1
#include "usr_hal.h"
#include "mqtt_agent_task.h"
#include "flash/flash_hp.h"
#include <usr_config.h>

#define APP_MAIN_MQTT_AGENT_TASK_STACK_SIZE         ( 6144 )
#define APP_MAIN_MQTT_AGENT_TASK_PRIORITY           ( tskIDLE_PRIORITY + 4 )

/*************************************************************************************
 * global variables
 ************************************************************************************/
ProvisioningParams_t params;

/*************************************************************************************
 * Private variables
 ************************************************************************************/
char CLIENT_CERTIFICATE_PEM[2048];
char CLIENT_KEY_PEM[2048];
static char USER_MQTT_ENDPOINT[128];

extern char g_certificate[2048];
extern char g_private_key[2048];
extern char g_mqtt_endpoint[128];

extern char g_write_buffer[2048];

/*************************************************************************************
 * Private functions
 ************************************************************************************/
static int config_littlFs_flash(void);

void wifi_thread_entry(void *pvParameters){
    FSP_PARAMETER_NOT_USED (pvParameters);
    fsp_err_t err = FSP_SUCCESS;
    fsp_err_t          fsp_status  = FSP_SUCCESS;
    BaseType_t         bt_status   = pdFALSE;
    int                lfs_err     = LFS_ERR_OK;
    int                ierr        = FSP_SUCCESS;
    CK_RV              str_status  = CKR_OK;

    EventBits_t uxBits;
    volatile fsp_err_t status = FSP_ERR_ASSERTION;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 0: wifi_thread_entry()",RTT_TIMESTAMP());

    /* Open RA6M3 WIFI UART Port */
    err = R_FLASH_HP_Open(&user_flash_ctrl, &user_flash_cfg);

    flash_cert_init();
    APP_PRINT("Certs hardcoded\r\n");

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

//        /* Synchronize with UART/SSIDPW Thread */
//        xSemaphoreTake(g_wifi_semaphore,portMAX_DELAY);

        /* Start of Wi-Fi Thread Task */
        APP_PRINT("\r\n%s Inside Wifi_thread",RTT_TIMESTAMP());

        FSP_PARAMETER_NOT_USED (pvParameters);

        /* Copy AWS client certificate, private key and MQTT end point */
        memcpy (CLIENT_CERTIFICATE_PEM, g_certificate, strlen(g_certificate));
        memcpy (CLIENT_KEY_PEM, g_private_key, strlen(g_private_key));
        memcpy (USER_MQTT_ENDPOINT, g_mqtt_endpoint, strlen(g_mqtt_endpoint));

        params.pucClientPrivateKey       = (uint8_t *) CLIENT_KEY_PEM;
        params.pucClientCertificate      = (uint8_t *) CLIENT_CERTIFICATE_PEM;
        params.ulClientPrivateKeyLength  = 1 + strlen((const char *) params.pucClientPrivateKey);
        params.ulClientCertificateLength = 1 + strlen((const char *) params.pucClientCertificate);
        params.pucJITPCertificate        = NULL;
        params.ulJITPCertificateLength   = 0;

        lfs_err = config_littlFs_flash();
        if (LFS_ERR_OK != lfs_err)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** littleFs flash config failed **\r\n");
            APP_ERR_TRAP(lfs_err);
        }

        /* Initialize the crypto hardware acceleration. */
        ierr = mbedtls_platform_setup(NULL);
        if (FSP_SUCCESS != ierr)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** HW SCE Init failed **\r\n");
            APP_ERR_TRAP(ierr);
        }

        fsp_status = network_interface_setup();
        if (FSP_SUCCESS != fsp_status)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** network interface setup failed **\r\n");
            APP_ERR_TRAP(fsp_status);
        }

        str_status = vAlternateKeyProvisioning (&params);
        if (CKR_OK != str_status)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** Alternate Key Provisioning failed **\r\n");
            APP_ERR_TRAP(str_status);
        }

    #if ENABLE_OTA_UPDATE_DEMO
        /* Store public key */
        str_status = storePublicKey();
        if (CKR_OK != str_status)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** Store Code Signing Public Key key failed **\r\n");
            APP_ERR_TRAP(str_status);
        }
    #endif

        bt_status = xMQTTAgentInit();
        if (pdTRUE != bt_status)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("xMQTTAgentInit Failed");
            APP_ERR_TRAP(bt_status);
        }

        vTaskDelay(10000); // sleep 10-sec

        xSetMQTTAgentState( MQTT_AGENT_STATE_INITIALIZED );
        vStartMQTTAgent (APP_MAIN_MQTT_AGENT_TASK_STACK_SIZE, APP_MAIN_MQTT_AGENT_TASK_PRIORITY);

        /******** Attempt to establish TLS session with MQTT broker. **********/
        if( xGetMQTTAgentState() != MQTT_AGENT_STATE_CONNECTED )
        {
            ( void ) xWaitForMQTTAgentState( MQTT_AGENT_STATE_CONNECTED, portMAX_DELAY );
        }

        /* Clear <<g_wifi_event>> Bits in <<MQTT_FLAG_ALL>>. */
        uxBits = xEventGroupClearBits( g_wifi_event,    /* The event group being updated. */
                                       MQTT_FLAG_ALL ); /* The bits being cleared. */

        /* Publish SSID Information to Cloud: NOTE must - sizeof(diagStr)<90 Bytes */
        char diagStr[64]; snprintf(diagStr, sizeof(diagStr) - 1, "Wifi: %s Ch: %i", (char*)getWifiParamAddr(WIFI_SSID), 99);
        publishSysDiagnostic(DNAME,diagStr,8,4);

        /* Publish Firmware Version to Cloud */
        publishSysVersion(DNAME, MMP_MAJOR_VERSION, MMP_MINOR_VERSION, MMP_PATCH,8,4);

        /* Publish Request to Get KMDC Threshold Value */
        publishKMDCThresRequest(DNAME,8,4);

//        /* (3) Update <<g_wifi_event>> Accordingly */
//        if (status == FSP_SUCCESS){
//            xEventGroupSetBits(g_wifi_event, MQTT_FLAG_CONNECTED);
//
//            /* Write To RTT Segger for Print Out */
//            APP_PRINT("\r\n%s Wi-Fi & MQTT Setup Successful (%s,%s)!!!",RTT_TIMESTAMP(),(char*)getWifiParamAddr(WIFI_SSID),(char*)getWifiParamAddr(WIFI_PASSWORD));
//
//            /* Publish SSID Information to Cloud: NOTE must - sizeof(diagStr)<90 Bytes */
//            char diagStr[64]; snprintf(diagStr, sizeof(diagStr) - 1, "Wifi: %s Ch: %i", (char*)getWifiParamAddr(WIFI_SSID), 99);
//            publishSysDiagnostic(DNAME,diagStr,8,4);
//
//            /* Publish Firmware Version to Cloud */
//            publishSysVersion(DNAME, MMP_MAJOR_VERSION, MMP_MINOR_VERSION, MMP_PATCH,8,4);
//
//            /* Publish Request to Get KMDC Threshold Value */
//            publishKMDCThresRequest(DNAME,8,4);
//
//        } // end if (FSP_SUCCESS)
//        else{
//            xEventGroupSetBits(g_wifi_event, MQTT_FLAG_DISCONNECTED);
//            APP_PRINT("\r\n%s Wi-Fi & MQTT Setup Not Successful (%s,%s)...",RTT_TIMESTAMP(),(char*)getWifiParamAddr(WIFI_SSID),(char*)getWifiParamAddr(WIFI_PASSWORD));
//        } // end if (!FSP_SUCCESS)

        /* (4) Pass back to UART_THREAD */
        xEventGroupSetBits(g_sync_event, UART_THREAD);

        //vTaskDelay (1);
    } // end while(1);

    FSP_PARAMETER_NOT_USED (uxBits);
    FSP_PARAMETER_NOT_USED (status);

} // end wifi_thread_entry()

/*********************************************************************************************************************//**
 * @brief   configures the littleFS Flash.
 *
 * This function sets up the littleFS Flash for Data storage.
 * @param[in]   None
 * @retval      LFS_ERR_OK              If both the connectivity checks are success.
 * @retval      Any other error         If one of the connectivity check fails.
 *********************************************************************************************************************/
int config_littlFs_flash(void)
{
    int lfs_err = LFS_ERR_OK;
    fsp_err_t err = FSP_SUCCESS;

    err = RM_LITTLEFS_FLASH_Open(&g_rm_littlefs0_ctrl, &g_rm_littlefs0_cfg);
    if(FSP_SUCCESS != err)
    {
        FAILURE_INDICATION;
        APP_ERR_PRINT("** littleFs Initialization failed **\r\n");
    }

    /* mount the file system */
    lfs_err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
    if(LFS_ERR_OK != lfs_err)
    {
        /* reformat if we can't mount the file system
         * this should only happen on the first boot
         */
        lfs_err = lfs_format(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
        if(LFS_ERR_OK != lfs_err)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** littleFs Flash Format failed **\r\n");
        }
        lfs_err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
        if(LFS_ERR_OK != lfs_err)
        {
            FAILURE_INDICATION;
            APP_ERR_PRINT("** littleFs Mount failed **\r\n");
        }
    }

    return lfs_err;
}
//#else
//void wifi_thread_entry(void *pvParameters){
//    volatile fsp_err_t status=FSP_ERR_ASSERTION;
//    while (1)
//    {
//        vTaskDelay (1);
//    }
//    FSP_PARAMETER_NOT_USED (status);
//    FSP_PARAMETER_NOT_USED (pvParameters);
//
//} // end wifi_thread_entry()
#endif
