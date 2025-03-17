/*
 * uartHandler.c
 *
 *  Created on: 7 Aug 2024
 *      Author: yyang
 */
#include "initialisations.h"

bool g_lyra_tx_complete_flag = 0;
bool g_lyra_rx_complete_flag = 0;
uint32_t lyra_tout_ii =0;
uint8_t g_uart_buf[256]={0};

struct g_uart_event_count {
    uint8_t rx_char;
    uint32_t rx_complete;
    uint32_t tx_complete;
    uint32_t tx_data_empty;
    uint32_t err_parity;
    uint32_t err_framing;
    uint32_t err_overflow;
    uint32_t break_detect;
    uint32_t unknown;
};

/* Global struct initialization: uart callback counter */
struct g_uart_event_count g_uartEventCounter = {0};

/*******************************************************************************************************************//**
 * @brief Read data from communications driver.  This call will return after the number of bytes requested is read or if
 * a timeout occurs while waiting for access to the driver.
 *
 * Global <<bool>>     getLyraRxCompleteFlag();
 * Global <<uint32_t>> lyra_tout_ii;
 *
 * @param[in]   inCtrl    Pointer to device control block initialized in Open call for communications driver.
 * @param[in]   inDst     Destination address to read data out
 * @param[in]   inBytes   Read data length
 * @param[in]   inTout    freeRTOS timeout. 1 tick = 1 ms
 *                        TX_NO_WAIT (0x00000000), portMAX_DELAY (0xFFFFFFFF),
 *                        and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
 **********************************************************************************************************************/
fsp_err_t g_sf_comms0_read(uart_ctrl_t * inCtrl, uint8_t* inDst, uint32_t inBytes, uint32_t inTout){
    volatile fsp_err_t uart_status=FSP_ERR_ASSERTION;

    /* (a) Set Rx Complete Flag to False, (b) clear g_uartEventCounter.rx_char */
    setLyraRxCompleteFlag(false);

    /* Collect Data from Lyra P */
    g_tic();
    uart_status = R_SCI_UART_Read(inCtrl, inDst, inBytes);
    for (lyra_tout_ii =0;lyra_tout_ii<inTout;lyra_tout_ii++){
        //if (g_uartEventCounter.rx_char>=inBytes){            memcpy(inDst,g_uart_buf,inBytes);
        if (getLyraRxCompleteFlag()==true){
            uart_status = FSP_SUCCESS;
            setLyraRxCompleteFlag(false);
            g_toc();
            break;
        } // end if (BUFFER_FULL)
        else{
            uart_status = FSP_ERR_TIMEOUT;
            TickType_t xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil(&xLastWakeTime, 10); // sleep 10ms
        } // end if (TIMEOUT)
    } // end for ii=0:iMax-1

    return uart_status;
} // g_sf_comms0_read()

/*******************************************************************************************************************//**
 * @brief write data to communications driver.  This call will return after all bytes are written or if a timeout occurs
     * while waiting for access to the driver.
 *
 * Global <<bool>>     getLyraRxCompleteFlag();
 * Global <<uint32_t>> lyra_tout_ii;
 *
 * @param[in]   inCtrl    Pointer to device control block initialized in Open call for communications driver.
 * @param[in]   inSrc     Source address to read data out from
 * @param[in]   inBytes   write data length
 * @param[in]   inTout    freeRTOS timeout. 1 tick = 1 ms
 *                        TX_NO_WAIT (0x00000000), portMAX_DELAY (0xFFFFFFFF),
 *                        and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
 **********************************************************************************************************************/
fsp_err_t g_sf_comms0_write(uart_ctrl_t * inCtrl, uint8_t* inSrc, uint32_t inBytes, uint32_t inTout){
    volatile fsp_err_t uart_status=FSP_ERR_ASSERTION;

    /* Write Data to Lyra P */
    uart_status = R_SCI_UART_Write(inCtrl, inSrc, inBytes);
    for (lyra_tout_ii =0;lyra_tout_ii<inTout;lyra_tout_ii++){
        if (getLyraTxCompleteFlag()==true){
            uart_status = FSP_SUCCESS;
            setLyraTxCompleteFlag(false);
            break;
        } // end if (BUFFER_FULL)
        else{
            uart_status = FSP_ERR_TIMEOUT;
            TickType_t xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil(&xLastWakeTime, 10); // sleep 10ms
        } // end if (TIMEOUT)
    } // end for ii=0:iMax-1
    return uart_status;
} // g_sf_comms0_write()

/*******************************************************************************************************************//**
 *  @brief       Read UART via interrupt for SSID and PWD extraction
 *  @param[in]   uint8_t*   inSrc     SRC location of array collecting values from UART RX_CHAR interrupt
 *  @param[in]   uint8_t*   inSSID    Destination to store the extracted SSID
 *  @param[in]   uint8_t*   inPWD     Destination to store the extracted PASSWORD
 *  @param[in]   UINT       inTout    Timeout value (default: TX_WAIT_FOREVER)
 * @retval       None
 **********************************************************************************************************************/
uint32_t ssid_tout_ii=0;
fsp_err_t g_sf_ssidpw_read( uint8_t* inSrc, uint8_t* inSSID,uint8_t* inPWD,uint32_t inTout){
    volatile fsp_err_t uart_status=FSP_ERR_ASSERTION;

    char *start;
    char *end;
    char ssid_t[30];
    char pwd_t[30];

    /* Collect Data from Lyra P Via Interrupt */
    //uart_status = R_SCI_UART_Read(inCtrl, inDst, inBytes);
    for (ssid_tout_ii =0;ssid_tout_ii<inTout;ssid_tout_ii++){
        // Find the first comma
        start = strstr((char*)inSrc, ",");
        if (start != NULL) {
            // Move to the first character after the first comma
            start++;

            // Find the next comma
            end = strstr(start, ",");
            if (end != NULL) {
                // Extract the first substring
                strncpy(ssid_t, start, (size_t) (end - start) );
                ssid_t[end - start] = '\0';  // Null-terminate the string

                // Move to the character after the next comma
                start = end + 1;

                // Find the third comma (which should be the end of the second substring)
                end = strstr(start, ",");
                if (end != NULL) {
                    // Extract the second substring
                    strncpy(pwd_t, start, (size_t) (end - start) );
                    pwd_t[end - start] = '\0';  // Null-terminate the string

                    /* Copy Extracted values into destination */
                    strcpy((char*)inSSID,ssid_t);
                    strcpy((char*)inPWD,pwd_t);
                    break;
                } // end if 3rd Comma (end != null)
            } // end if 2nd Comma (end != null)
        } // end if 1st Comma (start != null)
        else{
            uart_status = FSP_ERR_TIMEOUT;
            TickType_t xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil(&xLastWakeTime, 10); // sleep 10ms
        } // end if (TIMEOUT)
    } // end for ii=0:iMax-1

    /* Abort any Read and Write */
    uart_status = R_SCI_UART_Abort(&g_uart0_ctrl, UART_DIR_RX_TX);

    FSP_PARAMETER_NOT_USED(uart_status);

    return uart_status;
} // g_sf_ssidpw_read()

/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(uint8_t)  g_uart_buf[256]>>
 *  @param       none
 *  @retval      &g_uart_buf[256]
 **********************************************************************************************************************/
uint8_t* getGuartBuffAddr(void){
    return (uint8_t*) g_uart_buf;
} // end getGuartBuffAddr()

/*******************************************************************************************************************//**
 *  @brief       get the Flag value of UART_EVENT_RX_COMPLETE
 *  @param[in]   None
 * @retval       bool         True/False g_uart_event_rx_complete_flag
 **********************************************************************************************************************/
bool getLyraRxCompleteFlag(void){
    return g_lyra_rx_complete_flag;
} // getLyraRxCompleteFlag()

/*******************************************************************************************************************//**
 *  @brief       set the Flag value of UART_EVENT_RX_COMPLETE
 *  @param[in]   bool         True/False
 * @retval       Global       g_uartDataLen of current uart extraction
 **********************************************************************************************************************/
void setLyraRxCompleteFlag(bool inBool){
    memset(g_uart_buf,0,sizeof(g_uart_buf));
    g_lyra_rx_complete_flag = inBool;
    g_uartEventCounter.rx_char=0;
} // setLyraRxCompleteFlag()

/*******************************************************************************************************************//**
 *  @brief       get the Flag value of UART_EVENT_TX_COMPLETE
 *  @param[in]   None
 * @retval       bool         True/False g_uart_event_tx_complete_flag
 **********************************************************************************************************************/
bool getLyraTxCompleteFlag(void){
    return g_lyra_tx_complete_flag;
} // getLyratxCompleteFlag()

/*******************************************************************************************************************//**
 *  @brief       set the Flag value of UART_EVENT_tx_COMPLETE
 *  @param[in]   bool         True/False
 * @retval       None
 **********************************************************************************************************************/
void setLyraTxCompleteFlag(bool inBool){
    g_lyra_tx_complete_flag = inBool;
} // setLyratxCompleteFlag()

/**********************************************************************************************************************
 *  UART interrupt callback. g_g_uart0_callback
 *
 *  @param[in]  p_args               Callback parameters.
 **********************************************************************************************************************/
void g_uart0_callback(uart_callback_args_t *p_args){
    char rx_char = (char) p_args->data;
    p_args->data = '\0';
    switch (p_args->event){
        case UART_EVENT_RX_CHAR:    {
            g_uart_buf[g_uartEventCounter.rx_char++]=(uint8_t) rx_char;
            //g_uartEventCounter.rx_char++;
            //g_lyra_rx_complete_flag = (g_uart_event_count[0]==UART_DATA_SIZE)? true:false;
            break;
        } //  UART_EVENT_RX_CHAR()
        case UART_EVENT_RX_COMPLETE: {
            g_lyra_rx_complete_flag = true;
            g_uartEventCounter.rx_complete++;
            break;
        } // UART_EVENT_RX_COMPLETE()
        case UART_EVENT_TX_COMPLETE: {
            g_lyra_tx_complete_flag = true;
            g_uartEventCounter.tx_complete++;
            break;
        } // UART_EVENT_TX_COMPLETE()
        case UART_EVENT_TX_DATA_EMPTY: {
            //////APP_PRINT("UART_EVENT_TX_DATA_EMPTY()\n");
            g_uartEventCounter.tx_data_empty++;
            break;
        } // UART_EVENT_TX_DATA_EMPTY()
        case UART_EVENT_ERR_PARITY: {
            g_uartEventCounter.err_parity++;
            //g_err_flag=1;
            ////APP_PRINT("UART_EVENT_ERR_PARITY()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
            break;
        } // UART_EVENT_ERR_PARITY()
        case UART_EVENT_ERR_FRAMING: {
            g_uartEventCounter.err_framing++;
            //g_err_flag=1;
            ////APP_PRINT("UART_EVENT_ERR_FRAMING()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
            break;
        } // UART_EVENT_ERR_FRAMING()
        case UART_EVENT_ERR_OVERFLOW: {
            g_uartEventCounter.err_overflow++;
            //g_err_flag=1;
            ////APP_PRINT("UART_EVENT_ERR_OVERFLOW()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,rx_char,(uint8_t*)&idx);
            break;
        } // UART_EVENT_ERR_OVERFLOW()
        case UART_EVENT_BREAK_DETECT:{
            g_uartEventCounter.break_detect++;
            ////APP_PRINT("UART_EVENT_BREAK_DETECT()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
            break;
        } // UART_EVENT_BREAK_DETECT()
        default:{
            g_uartEventCounter.unknown++;
            ////APP_PRINT("UART_EVENT_ERR_UNKNOWN()\n");
            break;
        } // default()
    } // end switch
    FSP_PARAMETER_NOT_USED(rx_char);
}// g_uart0_callback()

// =============================================================================================================
// UART Thread API
// =============================================================================================================

/**********************************************************************************************************************
 * @brief     Check processingCallback condition are met and (A) Set Flags, (B) Tx response accordingly
 * @param[in] None <<(bool) processing_callback>>       NEW SSID?
 *                  <<(mqtt_flag_t) g_aws_flag>>        WIFI Down?
 *                  <<(char) g_feedback_char[3]>>       Uart Tx Value
 * @retval    bool  true/false
 *                  <<(char) g_feedback_char[3]>>       modify
 *                  <<(bool) processing_callback>>      clear
 *                  <<(bool) button_pressed>>           clear
 *                  <<(uint8_t) feedbackState>>         Set
 *                  <<(ULONG) sync_actual_flags>>       KEEP or (Remove and SET SSID)
 *                  <<g_wifi_semaphore>>                put semaphore
 *                  call function BLE_disconnect()
 **********************************************************************************************************************/
char g_feedback_char[] ="R\r\n"; // for feedback Y/W/R
bool checkIfSkipLoopAndTxAnyFeedback(void){
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    EventBits_t uxBits;

    /* Clear <<g_wifi_event>> Bits in <<MQTT_FLAG_ALL>>. */
    uxBits = xEventGroupClearBits( g_wifi_event,    /* The event group being updated. */
                                   MQTT_FLAG_ALL ); /* The bits being cleared. */

    /* Set Wi-Fi Event Flags: DISCONNECTED */
    xEventGroupSetBits(g_wifi_event, MQTT_FLAG_DISCONNECTED);

    /* If User GSE 11/GSE 10-> ProcessingCallback */
    if (getProcessingCallback()){
        if(internet_state_get(G_MQTT)){
            status=g_sf_comms0_write(&g_uart0_ctrl, (uint8_t*)"Y\r\n", 3, 100);
            g_feedback_char[0]='R';
            vTaskDelay(1000);
            setProcessingCallback(false);
            setButtonPressed(false);
            g_feedbackState_set(fb_YES);
            BLE_disconnect();
            return true; //continue
        }// end if (G_MQTT)
        else{
            vTaskDelay(2000); // Slow down: BGX need time to switch from <<CMD>> to <<STREAM>>

            /* Clear <<SYNC_FLAG_ALL>> Bits in <<g_sync_event>>. */
            uxBits = xEventGroupClearBits( g_sync_event,    /* The event group being updated. */
                                           SYNC_FLAG_ALL ); /* The bits being cleared. */

            /* Write Feedback to Lyra P*/
            status=g_sf_comms0_write(&g_uart0_ctrl, (uint8_t*)g_feedback_char, 3, 100);
            g_feedback_char[0]='W'; // First Time "R" Subsequent Time "W"
            g_feedbackState_set(fb_READY);

            /* Set Only <<SSIDPW_THREAD>> Flag */
            xEventGroupSetBits(g_sync_event, SSIDPW_THREAD);
        } // end if (processing_callback && !G_MQTT)
    } // end if (processing_callback)
    else if(internet_state_get(G_MQTT)){
        /* Do Nothing if its only Wi-Fi Reconnect */
        setButtonPressed(false);
        return true;
    } // end if (G_MQTT)
    else {
        /* Clear <<SYNC_FLAG_ALL>> Bits in <<g_sync_event>>. */
        uxBits = xEventGroupClearBits( g_sync_event,    /* The event group being updated. */
                                       SYNC_FLAG_ALL ); /* The bits being cleared. */

        /* Likely disconnect due to poor wifi connect, try to reconnect. */
        xSemaphoreGive(g_wifi_semaphore);
    }  // end if (!processing_callback && !G_MQTT)
    return false;
    FSP_PARAMETER_NOT_USED(status);
    FSP_PARAMETER_NOT_USED(uxBits);
} // end checkIfSkipLoopAndTxAnyFeedback()


/**********************************************************************************************************************
 * @brief     This is a blocking function to check for !G_MQTT and return else WAIT until g_ble=1
 * @param[in] None  <<(RGB_Colour) LED_CURR>>           Check sensor ON dock?
 *                  <<(mqtt_flag_t) g_aws_flag>>        WIFI Down?
 *                  <<(blec_state_t) g_blec_state>>     !BLEC_VITALS_OFF?
 *                  <<(uint8_t)      BLE_check>>        check
 * @retval    bool  true/false
 *                  <<(bool) button_pressed>>           true/false (set)
 *                  Sleep until g_ble==1
 *                  call function BLE_connect()         for <<G_MQTT>> and <<LED!=CHARGIN>>
 **********************************************************************************************************************/
bool checkButtonpressOrWaitUntilBLEconnected(void){
    /* Check if BLE is Connected */
    if(!internet_state_get(G_MQTT)){
        setButtonPressed(true);
        return true;
    }  // end if (!G_MQTT)
    else {
        setButtonPressed(false);
        if ((BLEC_state_get()!=BLEC_VITALS_OFF)){
            if((!BLE_check()) && (internet_state_get(G_MQTT)) &&
                    ((LED_color_get()!=GREEN_PULSING)||(LED_color_get()!=GREEN_GLOWING))){
                BLE_connect();
            } // end while(!g_ble && G_MQTT && SENSOR_OFF_DOCK)
        } // end if (!BLEC_VITAL_OFF)
        vTaskDelay(10); // sleep 1-sec
        return false;
    } // end if (G_MQTT)
} // checkButtonpressOrWaitUntilBLEconnected()
