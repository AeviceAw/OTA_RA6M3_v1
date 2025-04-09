#include "initialisations.h"

#define LYRA_TIMEOUT_THRES 800 // 8-sec
#define BLE_TIMEOUT_MIN 1      // 1-min
uint32_t g_uartOK=0;           // Count UART OK
uint32_t g_uartNOK=0;          // Count UART NOK
uint32_t uart_awsNOK = 0;         // Count buttonpressed
extern TaskHandle_t uart_thread;

/* Uart Thread entry function */
/* pvParameters contains TaskHandle_t */
void uart_thread_entry(void *pvParameters){
    FSP_PARAMETER_NOT_USED (pvParameters);
    fsp_err_t   status = FSP_ERR_ASSERTION;
    EventBits_t uxBits;
    uint8_t     euart = 0;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority %d: uart_thread_entry()",RTT_TIMESTAMP(),uxTaskPriorityGet(uart_thread));

    /* Open the Lyra-P UART Port */
    status = R_SCI_UART_Open(&g_uart1_ctrl, &g_uart1_cfg);

    /* Enable access to the PFS registers. If using r_ioport module then
     * register protection is automatically handled.
     *  This code uses BSP IO functions to show how it is used.
     */
    R_BSP_PinAccessEnable();

    /* Disconnect BLE*/
    BLE_disconnect();

    vTaskDelay(5000); // Delay Uart

    /* Clear <<SYNC_FLAG_ALL>> Bits in <<g_sync_event>>. */
    uxBits = xEventGroupClearBits( g_sync_event,    /* The event group being updated. */
                                   SYNC_FLAG_ALL ); /* The bits being cleared. */

    /* Set Only <<UART_THREAD>> Flag */
    xEventGroupSetBits(g_sync_event, UART_THREAD);

    /* FIXME: ENABLE to BY PASSING WIFI THREAD Setup*/
    //internet_state_set();


    vTaskDelay(5000); // Delay Uart to let RGB Finish running first

    while (1)    {
        // Wait for UART_THREAD Bit to be set
        uxBits = xEventGroupWaitBits(  g_sync_event,           // The event group handle
                                       UART_THREAD,            // Bits to wait for
                                       pdFALSE,                // !Clear bits on exit
                                       pdTRUE,                 // Wait for both bits (logical AND)
                                       portMAX_DELAY           // Wait indefinitely
        );
        APP_PRINT("\r\n%s Inside Uart_thread",RTT_TIMESTAMP());

        // ==================================================================================================
        // UART Recovery Logic: (A) buttonpressed if !G_MQTT, (B) Connect BLE if G_MQTT and not charging
        // ==================================================================================================
        checkButtonpressOrWaitUntilBLEconnected();

        if(!internet_state_get(G_MQTT) || getButtonPressed()){
            uart_awsNOK++;

            /* Abort any Read and Write */
            status = R_SCI_UART_Abort(&g_uart1_ctrl, UART_DIR_RX_TX);

            // ======================================================================
            /* Feedback SSIDPW Setup Progress */
            if(checkIfSkipLoopAndTxAnyFeedback())
                continue; // (A) skip if feedback "Y", (B) "R" and go SSID,
            // (C) "W" and go SSID,      (D) Go Wifi only
            // ======================================================================
        } // end if(buttonPressed)
        else if(internet_state_get(G_MQTT)&&BLEC_state_get()!=BLEC_OFF){
            memset(getUartDataAddr(),0, EUART_DATA_SIZE);

            if (BLEC_state_get()!=BLEC_VITALS_OFF){
                /* FIXME: YY Anyway to optimize this without losing the battery readout at first connect? */
                while(!BLE_check()){
                    BLE_connect();
                    vTaskDelay(10);
                } // end while(!g_ble)
                vTaskDelay(1000);
                /* Collect Data from Lyra P */
                status=g_sf_comms0_read(&g_uart1_ctrl, getUartDataAddr(), EUART_DATA_SIZE, LYRA_TIMEOUT_THRES);
                if (status==FSP_SUCCESS){
                    /* Update BLEC State because UART SUCESS */
                    //blec_state_update(LED_color_get(),imu_onbody_get(),BLEC_state_get(), getTrueCheckVal(),&g_uartOK);
                    g_uartOK++; // counts number of 5.12 sec

                    if(euart<80)
                    {
                        /* (A) Uart Data to Comm Data for Dataprocess to use */
                        memcpy((getComDataAddr()+(euart*768)), getUartDataAddr(), EUART_DATA_SIZE);
                        if (euart>=79){
                            g_uartOK++; // because 5.12 sec collecte

                            // TODO given semaphore to dataprocess
                            // TODO visual inspection of comData
                            APP_PRINT("\r\n5.12 sec ready");

                            // (B) Put data_processing semaphore
                            xSemaphoreGive(g_data_semaphore);
                        }

                    }
                    else
                    {
                        // REset comData Pointer
                        // Reset comData counter
                    }

                    //getData();

                } // end (uart_ok)
                else{
                    /* Increase NOK Counter*/
                    g_uartNOK++;  g_uartOK=0;

                    /* TODO: Error Handling */
                    if (g_uartNOK%(10*BLE_TIMEOUT_MIN)==0){ // x10 NOK ~ 60sec
                        status=g_sf_comms0_write(&g_uart1_ctrl, (uint8_t*)"Connection-Timeouted!\r\n", 23, 100);
                        BLE_disconnect();
                    } // end if (>uartNOK Count)
                }  // end if (uart_nok)
            } // end if (!BLEC_VITALS_OFF)

//            if (BLE_check()){
//                /* Collect Data from Lyra P */
//                status=g_sf_comms0_read(&g_uart1_ctrl, getUartDataAddr(), UART_DATA_SIZE, LYRA_TIMEOUT_THRES);
//                if (status==FSP_SUCCESS){
//                    /* Update BLEC State because UART SUCESS */
//                    blec_state_update(LED_color_get(),imu_onbody_get(),BLEC_state_get(), getTrueCheckVal(),&g_uartOK);
//                    //g_uartOK++;
//                } // end (uart_ok)
//                else{
//                    /* Increase NOK Counter*/
//                    g_uartNOK++;  g_uartOK=0;
//
//                    /* TODO: Error Handling */
//                    if (g_uartNOK%(10*BLE_TIMEOUT_MIN)==0){ // x10 NOK ~ 60sec
//                        status=g_sf_comms0_write(&g_uart1_ctrl, (uint8_t*)"Connection-Timeouted!\r\n", 23, 100);
//                        BLE_disconnect();
//                    } // end if (>uartNOK Count)
//                }  // end if (uart_nok)
//
//            /* (A) Uart Data to Comm Data for Dataprocess to use */
//            memcpy(getComDataAddr(), getUartDataAddr(), UART_DATA_SIZE);
//
//            // (B) Put data_processing semaphore
//            xSemaphoreGive(g_data_semaphore);
//
//            } // if (BLE)
            else if (BLEC_state_get()==BLEC_VITALS_OFF){
            //if (BLEC_state_get()==BLEC_VITALS_OFF){
                for (uint32_t blec_tout_ii =0;blec_tout_ii<LYRA_TIMEOUT_THRES*2;blec_tout_ii++){
                    TickType_t xLastWakeTime = xTaskGetTickCount();
                    vTaskDelayUntil(&xLastWakeTime, 10); // sleep 10ms
                } // end for ii=0:iMax-1
                //blec_state_update(LED_color_get(),imu_onbody_get(),BLEC_state_get(), getTrueCheckVal(),&g_uartOK);
            } // end if (BLEC_VITALS_OFF)
        } // end if(!buttonPressed && G_MQTT)

        /* Update BLEC State because UART SUCESS */
        blec_state_update(LED_color_get(),imu_onbody_get(),BLEC_state_get(), getTrueCheckVal(),&g_uartOK);

    } // end while(1)

    FSP_PARAMETER_NOT_USED(uxBits);
    FSP_PARAMETER_NOT_USED(status);
} // end uart_thread_entry()
