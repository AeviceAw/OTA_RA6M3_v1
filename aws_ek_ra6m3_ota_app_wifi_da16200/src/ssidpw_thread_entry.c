#include "initialisations.h"

#ifdef DO_THE_THING
/* SSIDPW thread entry function */
/* pvParameters contains TaskHandle_t */
void ssidpw_thread_entry(void *pvParameters){
    FSP_PARAMETER_NOT_USED (pvParameters);
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    EventBits_t uxBits;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 4: ssidpw_thread_entry()",RTT_TIMESTAMP());

    /* Open IRQ */
    status = R_ICU_ExternalIrqOpen(&g_external_irq1_ctrl, &g_external_irq1_cfg);

    /* Enable IRQ */
    status = R_ICU_ExternalIrqEnable(&g_external_irq1_ctrl);

    /* Open DataFlash */
    status = R_FLASH_HP_Open(&g_flash_ctrl, &g_flash_cfg);

    /* Disconnect BLE*/
    //BLE_disconnect();

    /* TODO: add your own code here */
    while (1)   {

        // Wait for UART_THREAD Bit to be set
        uxBits = xEventGroupWaitBits(  g_sync_event,           // The event group handle
                                       SSIDPW_THREAD,          // Bits to wait for
                                       pdFALSE,                // !Clear bits on exit
                                       pdTRUE,                 // Wait for both bits (logical AND)
                                       portMAX_DELAY           // Wait indefinitely
                                       );

        APP_PRINT("\r\n%s Inside ssidpw_thread",RTT_TIMESTAMP());

        /* Clear UART Callback's Buffer Values */
        setLyraRxCompleteFlag(false);

        /* Wait and sleep forever until got SSID and PASSWORD*/
        status=g_sf_ssidpw_read( getGuartBuffAddr(),getWifiParamAddr(WIFI_SSID),getWifiParamAddr(WIFI_PASSWORD), portMAX_DELAY);

        status=g_sf_comms0_write(&g_uart0_ctrl, (uint8_t*)"Success\r\n", 9, 100);
        g_feedbackState_set(fb_CONNECTING);

//        /* Write to dataFlash - Reading will be done at Wifi_thread Entry */
//        status = DF_Write_SSID((char*)getWifiParamAddr(WIFI_SSID));
//        status = DF_Write_PW((char*)getWifiParamAddr(WIFI_PASSWORD));

        /* Clear <<SYNC_FLAG_ALL>> Bits in <<g_sync_event>>. */
        uxBits = xEventGroupClearBits( g_sync_event,    /* The event group being updated. */
                                       SYNC_FLAG_ALL ); /* The bits being cleared. */

        /* Let Wifi Thread to Run First */
        xSemaphoreGive(g_wifi_semaphore);
    } // end while(1)

    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (uxBits);

} // end ssidpw_thread_entry()
#else CHECKPOINT1
/* SSIDPW thread entry function */
/* pvParameters contains TaskHandle_t */
void ssidpw_thread_entry(void *pvParameters){
    FSP_PARAMETER_NOT_USED (pvParameters);
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    EventBits_t uxBits;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 4: ssidpw_thread_entry()",RTT_TIMESTAMP());

    /* Open IRQ */
    status = R_ICU_ExternalIrqOpen(&g_external_irq1_ctrl, &g_external_irq1_cfg);

    /* Enable IRQ */
    status = R_ICU_ExternalIrqEnable(&g_external_irq1_ctrl);

    /* Open DataFlash */
//    status = R_FLASH_HP_Open(&g_flash_ctrl, &g_flash_cfg);

    /* Disconnect BLE*/
    //BLE_disconnect();

    /* TODO: add your own code here */
    while (1)   {

        // Wait for UART_THREAD Bit to be set
        uxBits = xEventGroupWaitBits(  g_sync_event,           // The event group handle
                                       SSIDPW_THREAD,          // Bits to wait for
                                       pdFALSE,                // !Clear bits on exit
                                       pdTRUE,                 // Wait for both bits (logical AND)
                                       portMAX_DELAY           // Wait indefinitely
                                       );

        APP_PRINT("\r\n%s Inside ssidpw_thread",RTT_TIMESTAMP());

//        flash_cert_init();
//        APP_PRINT("Certs hardcoded");

        status=g_sf_comms0_write(&g_uart0_ctrl, (uint8_t*)"Success\r\n", 9, 100);
        g_feedbackState_set(fb_CONNECTING);

//        /* Write to dataFlash - Reading will be done at Wifi_thread Entry */
//        status = DF_Write_SSID((char*)getWifiParamAddr(WIFI_SSID));
//        status = DF_Write_PW((char*)getWifiParamAddr(WIFI_PASSWORD));

        /* Clear <<SYNC_FLAG_ALL>> Bits in <<g_sync_event>>. */
        uxBits = xEventGroupClearBits( g_sync_event,    /* The event group being updated. */
                                       SYNC_FLAG_ALL ); /* The bits being cleared. */

        /* Let Wifi Thread to Run First */
        xSemaphoreGive(g_wifi_semaphore);
    } // end while(1)

    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (uxBits);

} // end ssidpw_thread_entry()
//#else
//void ssidpw_thread_entry(void *pvParameters){
//    volatile fsp_err_t status=FSP_ERR_ASSERTION;
//    while (1)
//    {
//        vTaskDelay (1);
//    }
//    FSP_PARAMETER_NOT_USED (status);
//    FSP_PARAMETER_NOT_USED (pvParameters);
//
//} // end subscribe_thread_entry()
#endif
