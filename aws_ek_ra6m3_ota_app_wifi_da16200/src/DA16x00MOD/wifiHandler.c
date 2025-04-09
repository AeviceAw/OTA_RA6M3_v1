/*
 * wifiHandler.c
 *
 *  Created on: 10 Jun 2024
 *      Author: yyang
 */

#include "initialisations.h"

/* Global Variables */
//rx_state_t g_flag_NewReply;
uint8_t g_err_flag, g_tx_flag,g_rx_flag;
mqtt_flag_t g_aws_flags=MQTT_FLAG_CLEAR;
uint32_t t_flag = 0;

char g_rx_buf[RX_BUF_LEN]={'\0'};
uint32_t g_rx_count=0;
uint32_t rx_complete=0;

char g_tx_buf[TX_BUF_LEN]={'\0'};
uint32_t tx_complete=0;

/*******************************************************************************************************************//**
 * @brief       Initialize  UART.
 * @param[in]   None
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open
 ***********************************************************************************************************************/
fsp_err_t wifi_uart_init(void){
    volatile fsp_err_t err = FSP_SUCCESS;

    /* Initialize UART channel with baud rate 115200 */
    err = R_SCI_UART_Open (&g_uart0_ctrl, &g_uart0_cfg);
    //err = g_uart0.p_api->open(g_uart0.p_ctrl, g_uart0.p_cfg);

    ////if (FSP_SUCCESS != err)
    ////APP_PRINT ("\r\n**  R_SCI_UART_Open API failed  **\r\n");
    return err;
} // end wifi_uart_init()


/*******************************************************************************************************************//**
 *  @brief       Deinitialize SCI UART module
 *  @param[in]   None
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open
 **********************************************************************************************************************/
fsp_err_t wifi_uart_deinit(void){
    fsp_err_t err = FSP_SUCCESS;

    /* Close module */
    err =  R_SCI_UART_Close (&g_uart0_ctrl);
    //err = g_uart0.p_api->close(g_uart0.p_ctrl);

    ////if (FSP_SUCCESS != err)
    ////APP_PRINT ("\r\n**  R_SCI_UART_Close API failed  ** \r\n");
    return err;
} // end wifi_uart_deinit()

//void doTheProcessRxCharThing(rx_state_t* p_state, char data,uint8_t* p_idx);

/**********************************************************************************************************************
 *  Clear all the flags used in UART Rx, TX Buffer
 *
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 **********************************************************************************************************************/
fsp_err_t wifi_uart_flag_clear(void){
    //g_flag_NewReply=FIRST_BYTE;
    g_err_flag = 0;
    g_tx_flag = 0;g_rx_flag = 0;
    internet_state_clear();//g_mqtt = 0;g_wifi = 0;
    return FSP_SUCCESS;
} // end wifi_uart_flag_clear()


/**********************************************************************************************************************
// *  update <<g_aws_flag>> by first (1) Clear All, (2), Set State, (3) update <<g_aws_flag>>
// * @param[in] uint32_t_t  <MQTT_FLAG_STATE> 0: MQTT_FLAG_CLEAR, 1: MQTT_FLAG_DISCONNECTED,
// *                                          2: MQTT_FLAG_CONNECTED, 3: MQTT_FLAG_UNKNOWN
// * @retval    none
// **********************************************************************************************************************/
//volatile int8_t wifi_flag_update_err_count=-1;
//void wifi_event_flag_update(ULONG MQTT_FLAG_STATE){
//    volatile tx_err_t err = 1;
//    wifi_flag_update_err_count = 0;
//    while(err!=0){
//        //ULONG t_flag = (ULONG)g_aws_flags;
//        err = 0;
//        /* Clear Wi-Fi Event Flags */
//        //err |= tx_event_flags_set(&g_wifi_event_flags, MQTT_FLAG_CLEAR, TX_AND);
//
//        /* Any <<Wi-Fi Event Flags>> == <<MQTT_FLAG_All>> are cleared */
//        tx_event_flags_get(&g_wifi_event_flags, MQTT_FLAG_All, TX_OR_CLEAR, &t_flag, TX_NO_WAIT);
//
//        /* Set Wi-Fi Event Flags: MQTT_FLAG_STATE */ //g_wifi_event_flags.tx_event_flags_group_current = MQTT_FLAG_CONNECTED;
//        err |=tx_event_flags_set(&g_wifi_event_flags, MQTT_FLAG_STATE, TX_OR);
//
//        /* Update t_flag */
//        err |=tx_event_flags_get(&g_wifi_event_flags, MQTT_FLAG_STATE, TX_OR,&t_flag,TX_NO_WAIT);
//
//        /* Update Global Wi-Fi Event Flag */
//        g_aws_flags = (mqtt_flag_t) t_flag;
//
//        /* Update Internet State Flag */
//        if (g_aws_flags==DISCONNECTED){
//            /* Clear Internet State Flag */
//            wifi_uart_flag_clear();
//
//            /* Any <<sync_actual_flags>> == <<SYNC_FLAG_ALL>> are cleared */
//            tx_event_flags_get(&g_sync_event_flags, SYNC_FLAG_ALL, TX_OR_CLEAR, &sync_actual_flags, TX_NO_WAIT);
//
//            /* Set Uart Thread to Let it Run */
//            err |= tx_event_flags_set(&g_sync_event_flags, UART_THREAD, TX_OR);
//        }
//        else if (g_aws_flags==CONNECTED){
//            /* Set Internet State Flag */
//            internet_state_set();
//
//            /* Any <<sync_actual_flags>> == <<SYNC_FLAG_ALL>> are cleared */
//            tx_event_flags_get(&g_sync_event_flags, SYNC_FLAG_ALL, TX_OR_CLEAR, &sync_actual_flags, TX_NO_WAIT);
//
//            /* Set Uart Thread to Let it Run */
//            err |= tx_event_flags_set(&g_sync_event_flags, UART_THREAD, TX_OR);
//        }
//        if (err!=0){
//            wifi_flag_update_err_count++;
//        } // end if (err!=0)
//    }// end while(err!=0)
//} // wifi_event_flag_update()
//
///**********************************************************************************************************************
// * @brief For some reasons <<g_sync_event_flags>> != <<UART_THREAD>> when <<g_aws_flags== DISCON/CON>>
// * @retval  Put this inside RGB thread entry and check every 1-sec as a fail safe
// **********************************************************************************************************************/
//void checkWififlagAndUartflag(void){
//    volatile fsp_err_t status=FSP_ERR_ASSERTION;
//    if ((g_aws_flags==CONNECTED)){
//        if (sync_actual_flags!=UART_THREAD){
//            tx_event_flags_set(&g_sync_event_flags, UART_THREAD, TX_OR);
//        }// end if (!sync_actual_flags)
//    } // end if (CONNECTED||DISCONNECTED)
//    FSP_PARAMETER_NOT_USED(status);
//}// end check_flag_wifi_and_uart
//
///**********************************************************************************************************************
// *  Blocking function to wait for <<MQTT_FLAG_STATE>>  and update <<g_aws_flags>>
// * @param[in] uint32_t_t  <MQTT_FLAG_STATE> 0: MQTT_FLAG_CLEAR, 1: MQTT_FLAG_DISCONNECTED,
// *                                          2: MQTT_FLAG_CONNECTED, 3: MQTT_FLAG_UNKNOWN
// * @retval    none
// **********************************************************************************************************************/
//void wifi_event_flag_wait(ULONG MQTT_FLAG_STATE){
//    //ULONG t_flag = (ULONG)g_aws_flags;
//
//    /* Update t_flag */
//    tx_event_flags_get(&g_wifi_event_flags, MQTT_FLAG_STATE, TX_OR,&t_flag,TX_WAIT_FOREVER);
//
//    /* Update Global Wi-Fi Event Flag */
//    g_aws_flags = (mqtt_flag_t) t_flag;
//
//    /* Update Internet State Flag */
//    if (g_aws_flags==DISCONNECTED)
//        /* Clear Internet State Flag */
//        wifi_uart_flag_clear();
//    else if (g_aws_flags==CONNECTED)
//        /* Set Internet State Flag */
//        internet_state_set();
//} // wifi_event_flag_wait()

///* Process UART RX byte from DA16200 and extract MSG from: <CR><LF>MSG<CR><LF>
// * Ref: pg 28 https://www.renesas.com/us/en/document/mas/um-wi-003-da16200-da16600-host-interface-and-command-user-manual?language=en
// * @param   uint8_t* <rx_state_t> pointer to state_flag indicating if its a NEW or ONGOING reply
// * @param   char     <data>       data extracted from the p_arg->data from uart callback
// * @param   uint8_t* <p_idx>     pointer to g_rx_buf array
// * @example doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
// * @note    p_idx value is always one larger than strlen!
// */
//void doTheProcessRxCharThing(rx_state_t* p_state, char data,uint8_t* p_idx){
//    uint8_t idx  = *p_idx;                      // Temporary Store p_idx  value here
//    volatile char dataIn=(volatile char) data;  // Temporary Store data   value here
//    char msg[1024]={"\0"};                      // Temporary buff for RX Message
//    volatile rx_state_t state =(volatile rx_state_t) *p_state;                // Temporary Store p_flag value here
//
//    if(state==NEXT_BYTE)
//        if(data=='\r')
//            state=LAST_BYTE;
//
//    if (state==LAST_BYTE){
//        // Extract Full Message from DA16200: extract 0:[\r],1:[\n],<MSG>,idx-1:[\r],idx:[\n]
//        //for (uint8_t ii=2;ii<idx;ii++)
//        //    msg[ii-2]=g_rx_buf[ii];
//        memset(msg,0,sizeof(msg));memcpy(msg,&g_rx_buf[2],idx-2);
//        state = FIRST_BYTE;               // Set New Reply Flag for Next Iter
//        g_rx_flag=1;                    // Stop() Condition met
//        ////APP_PRINT("UART_EVENT_RX_STR: err = %d :%s\n",g_err_flag,msg);      // Print RX Message to RTT
//    } // end if ITS THE LAST BYTE
//    else if ((state==FIRST_BYTE)&&((char)dataIn=='\r')){      // 1st \r,\n
//        idx = 0;g_rx_flag=0;          // Clear pointer
//        memset(g_rx_buf,0,sizeof(g_rx_buf)); // Clear Rx_Buf
//        memset(msg,0,sizeof(msg));           // Clear Msg Buf
//        state = NEXT_BYTE;                   // Next Iter: Change State
//        g_rx_buf[idx]=(char)dataIn;          // Write RX into Buff
//        idx++;                               // Increase RX buff Pointer
//        //////APP_PRINT("UART_EVENT_RX_STR: FIRST BYTE\n")
//    } // end if ITS THE FIRST BYTE
//    else if (state==NEXT_BYTE){
//        g_rx_buf[idx]=(char)data;       // Write RX into Buff
//        idx++;                          // Increase RX buff Pointer
//    } // end if ITS THE NEXT BYTE
//
//    *p_state =(rx_state_t) state;             // Update *p_flag value
//    *p_idx = idx;                 // Update *p_idx value
//} // doTheProcessRxCharThing()

/**********************************************************************************************************************
 *  UART interrupt callback. g_da16200_uart_callback
 *
 *  @param[in]  p_args               Callback parameters.
 **********************************************************************************************************************/
void Da16200_uart_callback(uart_callback_args_t *p_args){
    char rx_char = (char) p_args->data;
    p_args->data = '\0';
    switch (p_args->event){
        case UART_EVENT_RX_CHAR:    {
            //////APP_PRINT("UART_EVENT_RX_CHAR : ascii %c, int %d\n",p_args->data,p_args->data);
            g_rx_buf[g_rx_count++]=rx_char;g_err_flag=0;
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)rx_char,(uint8_t*)&idx);
            break;
        } //  UART_EVENT_RX_CHAR()
        case UART_EVENT_RX_COMPLETE: {
            g_err_flag=0;rx_complete++;tx_complete=0;
            //////APP_PRINT("UART_EVENT_RX_COMPLETE()\n");
            ////APP_PRINT("UART_EVENT_RX_COMPLETE : ascii %c, int %d\n",rx_char,rx_char);
            break;
        } // UART_EVENT_RX_COMPLETE()
        case UART_EVENT_TX_COMPLETE: {
            g_tx_flag = 1;g_err_flag=0;tx_complete++;rx_complete=0;
            //////APP_PRINT("UART_EVENT_TX_STR :%s Completed!\n",g_tx_buf); // Print g_tx_buf value
            wifi_uart_gTxBuff_clear();  // clear g_tx_buf_value
            break;
        } // UART_EVENT_TX_COMPLETE()
        case UART_EVENT_TX_DATA_EMPTY: {
            //////APP_PRINT("UART_EVENT_TX_DATA_EMPTY()\n");
            break;
        } // UART_EVENT_TX_DATA_EMPTY()
        case UART_EVENT_ERR_PARITY: {
            g_err_flag=1;
            ////APP_PRINT("UART_EVENT_ERR_PARITY()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
            break;
        } // UART_EVENT_ERR_PARITY()
        case UART_EVENT_ERR_FRAMING: {
            g_err_flag=1;
            ////APP_PRINT("UART_EVENT_ERR_FRAMING()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
            break;
        } // UART_EVENT_ERR_FRAMING()
        case UART_EVENT_ERR_OVERFLOW: {
            g_err_flag=1;
            ////APP_PRINT("UART_EVENT_ERR_OVERFLOW()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,rx_char,(uint8_t*)&idx);
            break;
        } // UART_EVENT_ERR_OVERFLOW()
        case UART_EVENT_BREAK_DETECT:{
            ////APP_PRINT("UART_EVENT_BREAK_DETECT()\n");
            //doTheProcessRxCharThing((uint8_t*)&g_flag_NewReply,(char)p_args->data,(uint8_t*)&idx);
            break;
        } // UART_EVENT_BREAK_DETECT()
        default:{
            ////APP_PRINT("UART_EVENT_ERR_UNKNOWN()\n");
            break;
        } // default()
    } // end switch
}// Da16200_uart_callback()


/*******************************************************************************************************************//**
 *  @brief       Master API to setup Da16200 by sending all the commands
 *  @param[in]   char* ssid, password
 *  @retval      fsp_err_t <<status>>  Expect: FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t Da16200_masterSetup(char* ssid,char* password){

    /* Variable to store AT+Command Reponse */
    volatile fsp_err_t status=FSP_ERR_ASSERTION;

    const uint32_t txWait=4;
    const uint32_t rxWait=16*2;
    status = Da16200_restart(64,256*2); // 064 ms for Tx Complete, 2560 ms for Rx reply
    vTaskDelay(2000);
    status = Da16200_attention(txWait,rxWait);
    vTaskDelay(1000);
    status = Da16200_init(txWait,rxWait);
    vTaskDelay(1000);
    status = Da16200_setSNTP(txWait,rxWait);
    vTaskDelay(1000);
    status = Da16200_setMqttPingTimeout(10,txWait,rxWait); // 10-sec
//    vTaskDelay(1000);
    status = Da16200_subTopic(txWait,rxWait);
//    vTaskDelay(1000);
    status = Da16200_setMQTTBroker(txWait,rxWait);
//    vTaskDelay(1000);
    status = Da16200_setAWSCerts(txWait,128*2);
//    vTaskDelay(1000);

    /* Connect to SSID and Password */
    status = Da16200_setWifi(ssid,password,4,512); // 004 ms for Tx Complete, 1280 ms for Rx reply
    if (status)
        return status; // NO AP Found
    /* Ping to a known DNS */
    //status = Da16200_pingAddr("8.8.8.8",4,512); // 004 ms for Tx Complete, 2560 ms for Rx reply
    vTaskDelay(10000); // sleep 10-sec
    if (status)
        return status; // Cannot PING to Address;
    /* Connect to AWS MQTT Broker */
    status = Da16200_startMQTTBroker(4,1024*4);
    if (status)
        return status; // Cannot Connect to MQTT Broker

    /* Return Success */
    return status;
} // Da16200_masterSetup()

/* **********************************************************************************************************************
 * @brief Function to Write Array Values into UART
 * @param char*     g_buf     Array containing Values to Send to UART
 * @param uint32_t  buf_len   Number of elements to send to UART
 * @param uint32_t  txWait    in 10 ms
 * @param char*     wReply    Wait for Reply From UART, e.g., "\r\nOK\r\n"
 * @param uint32_t  rxWait    in 10 ms
 * @param uint8_t   ON_LOG    whether to use ////APP_PRINT for Segger RTT printout
 *
 *  sendBuffToWifiUart(g_rx_buf,TX_BUF_LEN,txWait,"\r\nOK\r\n",rxWait,1);
 **********************************************************************************************************************/
fsp_err_t sendBuffToWifiUart(char*g_buf,uint32_t buf_len,uint32_t txWait,char* wReply,uint32_t rxWait,uint8_t ON_LOG){
    /* Initialize Variables used in sendBuffToWifiUart() */
    fsp_err_t status = FSP_ERR_ASSERTION;// for tracking SUCCESS?

    const uint32_t rxWaitMax = 2048;
    if (rxWait>(uint32_t)rxWaitMax)
        rxWait=(uint32_t)rxWaitMax;

    /* Replace with calloc: Dun use so much memory if we dun need */
    //volatile char t_buf[TX_BUF_LEN]={'\0'}; //NOTE: g_buf = NULL after R_SCI_UART_Write()
    // Dynamically allocate memory using calloc() & check for sucessful allocation
    char* t_buf;
    t_buf = (char*)calloc(buf_len, sizeof(char));
    if (t_buf == NULL)
        return status;

    char *p_rx_buf = NULL;   // ptr of <<g_rx_buf>> for extracting RX_reply
    volatile uint32_t iTx = 0;   // (EA: 010 ms)for counting Wait Limit to Tx_complete
    volatile uint32_t iRx = 0;   // (EA: 010 ms)for counting Wait Limiit to Rx_reply

    /* Copy g_tx_buf (from g_buf) to local t_buf for Safekeeping*/
    memset((char*)t_buf,0,buf_len);
    strcpy((char*)t_buf,g_buf);

    /* Perform AT Command TX and Wait until SUCCESS: Only do x1 times only */
    while (status!=FSP_SUCCESS && iTx<1){

        /* obtains a mutex not owned by another thread */
        //tx_mutex_get(&g_wifi_mutex,0xffff);

        /* Initialize ALl the BUF needed for TX */
        wifi_uart_gRxBuff_clear(); // Clear g_rx_buf
        // Clear g_rx_buf_count, i.e., g_rx_buf[g_rx_count]
        p_rx_buf = NULL;           // Clear g_rx_buf_cointer
        iRx = 0;                   // Initialize  WaitCounter Value for Rx_reply
        g_tx_flag = 0;             // State Flag: set when Tx_Completed
        g_err_flag = 0;            // Unused: Initialized for consistency
        g_rx_flag = 0;             // Unused: Initialized for consistency

        /* Send Buf to UART TX*/
        //status = g_uart0.p_api->write(g_uart0.p_ctrl, (uint8_t*)t_buf, (uint32_t) buf_len);
        status = R_SCI_UART_Write (&g_uart0_ctrl, (uint8_t*)t_buf,(uint32_t) buf_len);

        /* Error Handling */
        if (status==FSP_SUCCESS){
            while(g_tx_flag==0 && iTx<txWait){
                vTaskDelay(10); // sleep 10ms
                iTx++;
            } // end wait for <<tx_complete>> state
            while (p_rx_buf==NULL && iRx<rxWait){
                // Search for the substring within the main string
                p_rx_buf = strstr(g_rx_buf,wReply); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
                iRx++;
                vTaskDelay(10); // sleep 10ms
            } // end wait for <<rx_reply>> state

            /* Assert if Timeout has occured? */
            if (p_rx_buf==NULL && iRx>=rxWait){
                status = FSP_ERR_ASSERTION;
            } // end if (NO_REPLY && TIMEOUT)
        } // if (UART_SUCCESS)
        else{
            wifi_uart_deinit();vTaskDelay(100); // sleep 100ms
            wifi_uart_init();vTaskDelay(100); // sleep 100ms
        }// if (UART_NOT_SUCCESS)

        /* releases a previously obtained mutex */
        //tx_mutex_put(&g_wifi_mutex);

        if (ON_LOG){
            ////APP_PRINT("(Tx%03d,Rx%03d) status = %d,strlen(msg)=%d,msg=%s",iTx,iRx,status, buf_len,(char*) t_buf);
        } // end (ON_LOG)
    } // while(RETRY)

    free(t_buf);
    return status;
} // sendBuffToWifiUart()


/*******************************************************************************************************************//**
// * @brief Function to Write Array Values into UART
// * @param char*     g_buf     Array containing Values to Send to UART
// * @param uint32_t  buf_len   Number of elements to send to UART
// * @param uint32_t  txWait    in 10 ms
// * @param char*     wReply    Wait for Reply From UART, e.g., "\r\nOK\r\n"
// * @param uint32_t  rxWait    in 10 ms
// * @param uint8_t   ON_LOG    whether to use ////APP_PRINT for Segger RTT printout
// *
// *  sendBuffToWifiUart(g_rx_buf,TX_BUF_LEN,txWait,"\r\nOK\r\n",rxWait,1);
// **********************************************************************************************************************/
//fsp_err_t sendBuffToWifiUart(char*g_buf,uint32_t buf_len,uint32_t txWait,char* wReply,uint32_t rxWait,uint8_t ON_LOG){
//    /* Initialize Variables used in sendBuffToWifiUart() */
//    fsp_err_t status = FSP_ERR_ASSERTION;// for tracking SUCCESS?
//
//    const uint32_t rxWaitMax = 2048;
//    if (rxWait>(uint32_t)rxWaitMax)
//        rxWait=(uint32_t)rxWaitMax;
//    volatile char t_buf[TX_BUF_LEN]={'\0'}; //NOTE: g_buf = NULL after R_SCI_UART_Write()
//    char *p_rx_buf = NULL;   // ptr of <<g_rx_buf>> for extracting RX_reply
//
//    volatile uint32_t iTx = 0;   // (EA: 010 ms)for counting Wait Limit to Tx_complete
//    volatile uint32_t iRx = 0;   // (EA: 010 ms)for counting Wait Limiit to Rx_reply
//
//    /* Copy g_tx_buf (from g_buf) to local t_buf for Safekeeping*/
//    memset((char*)t_buf,0,TX_BUF_LEN);
//    strcpy((char*)t_buf,g_buf);
//
//    /* Perform AT Command TX and Wait until SUCCESS: Only do x1 times only */
//    while (status!=FSP_SUCCESS && iTx<1){
//
//        /* obtains a mutex not owned by another thread */
//        //tx_mutex_get(&g_wifi_mutex,0xffff);
//
//        /* Initialize ALl the BUF needed for TX */
//        wifi_uart_gRxBuff_clear(); // Clear g_rx_buf
//        // Clear g_rx_buf_count, i.e., g_rx_buf[g_rx_count]
//        p_rx_buf = NULL;           // Clear g_rx_buf_cointer
//        iRx = 0;                   // Initialize  WaitCounter Value for Rx_reply
//        g_tx_flag = 0;             // State Flag: set when Tx_Completed
//        g_err_flag = 0;            // Unused: Initialized for consistency
//        g_rx_flag = 0;             // Unused: Initialized for consistency
//
//        /* Send Buf to UART TX*/
//        //status = g_uart0.p_api->write(g_uart0.p_ctrl, (uint8_t*)t_buf, (uint32_t) buf_len);
//        status = R_SCI_UART_Write (&g_uart0_ctrl, (uint8_t*)t_buf,(uint32_t) buf_len);
//
//        /* Error Handling */
//        if (status==FSP_SUCCESS){
//            while(g_tx_flag==0 && iTx<txWait){
//                vTaskDelay(10); // sleep 10ms
//                iTx++;
//            } // end wait for <<tx_complete>> state
//            while (p_rx_buf==NULL && iRx<rxWait){
//                // Search for the substring within the main string
//                p_rx_buf = strstr(g_rx_buf,wReply); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
//                iRx++;
//                vTaskDelay(10); // sleep 10ms
//            } // end wait for <<rx_reply>> state
//
//            /* Assert if Timeout has occured? */
//            if (p_rx_buf==NULL && iRx>=rxWait){
//                status = FSP_ERR_ASSERTION;
//            } // end if (NO_REPLY && TIMEOUT)
//        } // if (UART_SUCCESS)
//        else{
//            wifi_uart_deinit();vTaskDelay(100); // sleep 100ms
//            wifi_uart_init();vTaskDelay(100); // sleep 100ms
//        }// if (UART_NOT_SUCCESS)
//
//        /* releases a previously obtained mutex */
//        //tx_mutex_put(&g_wifi_mutex);
//
//        if (ON_LOG){
//            ////APP_PRINT("(Tx%03d,Rx%03d) status = %d,strlen(msg)=%d,msg=%s",iTx,iRx,status, buf_len,(char*) t_buf);
//        } // end (ON_LOG)
//    } // while(RETRY)
//
//    return status;
//} // sendBuffToWifiUart()


/*******************************************************************************************************************//**
 *  @brief       Send AT Command <<attention>> to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_attention(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] ="AT\r\n";
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1); //g_tx_buf='\n'
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_init()

/*******************************************************************************************************************//**
 *  @brief       Send AT Command <<Initialize>> to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_init(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] ="ATZ\r\n";
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);//g_tx_buf='\n'
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_init()

/*******************************************************************************************************************//**
 *  @brief       Send AT Command <<Factory Default>> to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_factory(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] ="ATF\r\n";
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    uint32_t tx_wait = txWait;//UART_TX_RETRY;  // x 10 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait;//UART_RX_RETRY;  // x 10 ms Wait for Rx Reply;
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"+INIT:DONE",rx_wait,1);//g_tx_buf='\n'
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_factory()

/*******************************************************************************************************************//**
 *  @brief       Send AT Command toggle <<echo>> to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_echo(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    char cmd0[]="ATE\r\n";
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_echo()

/*******************************************************************************************************************//**
 *  @brief       Send AT Command <<RESTART>> to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_restart(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] ="AT+RESTART\r\n";
    uint32_t tx_wait = txWait;  // 0640 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 2560 ms Wait for Rx Reply
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"+INIT:DONE",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_restart()

/*******************************************************************************************************************//**
 *  @brief       Send AT Command connect to <<ssid,password>> to UART
 *               set <<g_wifi>> if SUCCESS, clear <<g_wifi>> otherwise
 *  @param[in]   char*    ssid      Wi-Fi's SSID String
 *  @param[in]   char*    password  Wi-Fi's password String
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_setWifi(char* ssid,char* password,uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[1024]={'\0'};
    uint32_t tx_wait = txWait;   // 0040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 1280 ms Wait for Rx Reply
    snprintf(cmd0,sizeof(cmd0),"AT+WFJAP=%s,3,2,%s\r\n",ssid,password);
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"+WFJAP:1",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    if (status==FSP_SUCCESS)
        wifi_state_set();
    //g_wifi =status==FSP_SUCCESS? 1:0; // set wifi flag
    return status;
} // Da16200_setWifi()

/*******************************************************************************************************************//**
 *  @brief       Send AT Command connect to ping to DNS with <<IP ADDRESS>> to UART
 *               set <<g_wifi>> if SUCCESS, clear <<g_wifi>> otherwise
 *  @param[in]   char*    ip_addr  Domain Name Server IP Address, e.g., "8.8.8.8"
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_pingAddr(char* ip_addr_str,uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[64]={'\0'}; // WLAN0, ip_addr, Ping 1 time
    uint32_t tx_wait = txWait;   // 0040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 2560 ms Wait for Rx Reply
    snprintf(cmd0,sizeof(cmd0),"AT+NWPING=0,%s,1\r\n",ip_addr_str);
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"+NWPING:1,1",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    if (status==FSP_SUCCESS)
        wifi_state_set();
    else
        internet_state_clear();
    //g_wifi =status==FSP_SUCCESS? 1:0; // set wifi flag
    return status;
} // Da16200_pingAddress()


/*******************************************************************************************************************//**
 *  @brief       Send AT Command <<start DHCP>> to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_startDHCP(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[]={"AT+NWDHC=1\r\n"}; // Start DHCP
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_pingAddress()

/*******************************************************************************************************************//**
 *  @brief       Send AT Commands to setup Simple Network Time Protocol (SNTP) to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_setSNTP(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] = "AT+NWSNS=8.8.8.8\r\n";
    char cmd1[] = "AT+NWSNTP=1\r\n";
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd1);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd1),cmd1);

    return status;
} // Da16200_setSNTP()

/*******************************************************************************************************************//**
 *  @brief       Send AT Commands to define MQTT Ping Timeout to UART (Default: 300 sec)
 *  @param[in]   uint32_t timeout,( sec ) Interval to PING MQTT Broker and wait for responds
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_setMqttPingTimeout(uint32_t period_in_sec, uint32_t txWait, uint32_t rxWait ){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[1024]={'\0'};
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    snprintf(cmd0,sizeof(cmd0),"AT+NWMQPING=%d\r\n",(int)period_in_sec);
    //snprintf(cmd0,sizeof(cmd0),"AT+NWMQPING=?\r\n",period_in_sec); // default: 600 sec
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    //status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"+NWMQPING:",rx_wait,1);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);
    return status;
} // Da16200_setMqttPingTimeout()

/*******************************************************************************************************************//**
 *  @brief       Send AT Commands for setting up MQTT Broker Address to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_setMQTTBroker(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] ="AT+NWMQBR=a13pmlz832iuc9-ats.iot.ap-southeast-1.amazonaws.com,8883\r\n";
    char cmd1[] ="AT+NWMQSNI=a13pmlz832iuc9-ats.iot.ap-southeast-1.amazonaws.com\r\n";
    char cmd2[] ="AT+NWMQQOS=0\r\n";
    char cmd3[128] ={'\0'};
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    snprintf(cmd3,sizeof(cmd3),"AT+NWMQCID=%s\r\n",DNAME);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd1);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd1),cmd1);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd2);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd2),cmd2);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd3);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd3),cmd3);

    return status;
} // Da16200_setMQTTBroker() FRDRH

/*******************************************************************************************************************//**
 *  @brief       Send AT Commands for setting up MQTT TLS Certificates to UART, e.g., rootCA.pem, Device.crt, Device.key
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_setAWSCerts(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    //    char cmd0[]="AT+NWDCRT\r\n"; // Delete All Certs
    //    char cmd1[]="AT+NWCCRT\r\n"; // Check the Number of Certs

    //    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0); // Delete
    //    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);;

    wifi_uart_gTxBuff_clear();snprintf(g_tx_buf,sizeof(g_tx_buf),"%sC0,%s%s","\x1b",ROOT_CA,"\x03"); // page 88 https://www.renesas.com/us/en/document/qsg/da16200-da16600-getting-started-aws-iot-core?r=1599971
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,0); // ROOT CA
    ////APP_PRINT("\nstatus = %d,strlen(msg)=x,msg=C0\n",status);

    //    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd1);
    //    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1); // CHECK
    //    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd1),cmd1);

    wifi_uart_gTxBuff_clear();snprintf(g_tx_buf,sizeof(g_tx_buf),"%sC1,%s%s","\x1b",DEVICE_CERT,"\x03"); // page 88 https://www.renesas.com/us/en/document/qsg/da16200-da16600-getting-started-aws-iot-core?r=1599971
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,0); // DEVICE CERT
    ////APP_PRINT("status = %d,strlen(msg)=x,msg=C1\n",status);

    //    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd1);
    //    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);  // CHECK
    //    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd1),cmd1);

    wifi_uart_gTxBuff_clear();snprintf(g_tx_buf,sizeof(g_tx_buf),"%sC2,%s%s","\x1b",DEVICE_KEY,"\x03"); // page 88 https://www.renesas.com/us/en/document/qsg/da16200-da16600-getting-started-aws-iot-core?r=1599971
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,0); // DEVICE KEY
    ////APP_PRINT("status = %d,strlen(msg)=x,msg=C2\n\n",status);

    //    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd1);
    //    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1); // CHECK:7
    //    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd1),cmd1);

    return status;
} // Da16200_setAWSCerts()

/*******************************************************************************************************************//**
 *  @brief       Send AT Commands for setting up MQTT Subscribe Topics to UART
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_subTopic(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    //char cmd0[] ="AT+NWMQTS=3,AeviceMD/M815F070/Status,AeviceMD/M815F070/KMDCThreshold,AeviceMD/M815F070/AudioResend/Request\r\n";
    char cmd0[128]={'\0'};
    snprintf(cmd0,sizeof(cmd0),
            "AT+NWMQTS=3,AeviceMD/%s/Status,AeviceMD/%s/KMDCThreshold,AeviceMD/%s/AudioResend/Request\r\n",DNAME,DNAME,DNAME);
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    return status;
}// Da16200_subTopic()

/*******************************************************************************************************************//**
 *  @brief       Send AT Commands for starting MQTT Connection to UART
 *               Only run this after completing all setup and Wi-Fi is available
 *               set <<g_wifi>> if SUCCESS, clear <<g_wifi>> otherwise
 *               set <<g_mqtt>> if SUCCESS, clear <<g_wifi>> otherwise
 *  @param[in]   uint32_t txWait, (*10ms) wait for Wifi_Uart's Tx_Complete Flag
 *  @param[in]   uint32_t rxWait, (*10ms) wait for Wifi_Uart's Rx_reply (expected)
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t Da16200_startMQTTBroker(uint32_t txWait, uint32_t rxWait){
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[] ="AT+NWMQTLS=1\r\n";
    char cmd1[] ="AT+NWMQCS=1\r\n";
    char cmd2[] ="AT+NWMQCL=1\r\n";
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd0);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd0),cmd0);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd1);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait,"\r\nOK\r\n",rx_wait,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd1),cmd1);

    wifi_uart_gTxBuff_clear();strcpy(g_tx_buf,cmd2);
    status=sendBuffToWifiUart(g_tx_buf,strlen(g_tx_buf),tx_wait*4,"+NWMQCL:1",rx_wait*16,1);
    //////APP_PRINT("R_SCI_UART_Write() status = %d,strlen(msg)=%d,msg=%s\n",status, strlen(cmd2),cmd2);

    if (status==FSP_SUCCESS)
        internet_state_set();
    else
        internet_state_clear();
    //g_wifi =status==FSP_SUCCESS? 1:0; // set wifi flag
    //g_mqtt =status==FSP_SUCCESS? 1:0; // set mqtt flag
    return status;
} // Da16200_startMQTTBroker()


/*******************************************************************************************************************//**
 *  @brief       Get Address of <<g_rx_buff>>
 *  @param[in]   None
 *  @retval      char* Address to global wifi uart buffer <<g_rx_buff>>
 **********************************************************************************************************************/
char* wifi_uart_gRxBuffAdd(void){
    return (char*)&g_rx_buf;
}// wifi_uart_gRxBuffAdd()

/*******************************************************************************************************************//**
 *  @brief       Get Address of <<g_tx_buff>>
 *  @param[in]   None
 *  @retval      char* Address to global wifi uart buffer <<g_tx_buff>>
 **********************************************************************************************************************/
char* wifi_uart_gTxBuffAdd(void){
    return (char*)&g_tx_buf;
}// wifi_uart_gTxBuffAdd()

/*******************************************************************************************************************//**
 *  @brief       clear <<g_rx_buff>> values
 *  @param[in]   None
 *  @retval      fsp_err_t FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t wifi_uart_gRxBuff_clear(void){
    memset(g_rx_buf,0,sizeof(g_rx_buf));
    memset(&g_rx_count,0,sizeof(g_rx_count));
    return FSP_SUCCESS;
}// wifi_uart_gRxBuff_clear()

/*******************************************************************************************************************//**
 *  @brief       clear <<g_Tx_buff>> values
 *  @param[in]   None
 *  @retval      fsp_err_t FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t wifi_uart_gTxBuff_clear(void){
    memset(g_tx_buf,0,sizeof(g_tx_buf));
    return FSP_SUCCESS;
}// wifi_uart_gTxBuff_clear()

/*******************************************************************************************************************//**
 *  @brief       Publish System diagnostic message (<90 Bytes of data) to Cloud
 *  @param[in]   char*      inDNAME     <Device Name>
 *               char*      inMsg      Diagnostic Message
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 *  @remark      NOTE ONLY 256 BYTES of TX MESSAGE!!!!!!!!!!!!!!!!
 *  @remark      Max Significant for inDname 8 s.f., i.e., M1234567
 *  @remark      Max Significant for inMsg  90 Bytes,
 *  @remark      Max Significant for inSysTimer_cout  2^32,
 *  @remark      Max Significant for inSysStatusCount 2^32,
 *  @remark      Max Significant for inRetry          2^32f,
 *  @remark      Total utilization is 166 Bytes  < 256 bytes
 *  @remark      ENSURE sizeof(inMsg) <90 Bytes  !!!!!!!!!!!!!!!!
 **********************************************************************************************************************/
fsp_err_t publishSysDiagnostic(char* inDname,char* inMsg,uint32_t txWait, uint32_t rxWait){
    char cmd0[256]={'\0'};
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    uint32_t inSysTimer_cout= 0; // TODO: To be replaced with system timer val
    static uint32_t inSysStatusCount=0;
    uint32_t inRetry = 0; // TODO: To be replaced with pub retry count
    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\",\"Timer\":\"%d\",\"Entry\":\"%d\",\"Msg\":\"%s\",\"Retry\":\"%d\"}',AeviceMD/%s/SysDiagnostics\r\n",
            inDname,(int)inSysTimer_cout,(int)inSysStatusCount,inMsg,(int)inRetry,inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"\r\nOK\r\n",rx_wait,0);
    else
        inRetry++;
    vTaskDelay(80); // sleep 80ms
    inSysStatusCount++;
    return status;
}   // publishSysDiagnostic()

/*******************************************************************************************************************//**
 *  @brief       Publish Firmware Version to Cloud
 *  @param[in]   char*      inDNAME     <Device Name>
 *               uint8_t    inMajor    MMP_MAJOR_VERSION
 *               uint8_t    inMinor    MMP_MINOR_VERSION
 *               uint8_t    inPatch    MMP_PATCH
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 *  @remark      NOTE <<cmd0>> ONLY 128 BYTES of TX MESSAGE!!!!!!!!!!!!!!!!
 *  @remark      Max Significant for inDname 8 s.f., i.e., M1234567
 *  @remark      Max Significant for inMajor 3 s.f., i.e., 999
 *  @remark      Max Significant for inMinor 3 s.f., i.e., 999
 *  @remark      Max Significant for inPatch 3 s.f., i.e., 999
 *  @remark      Total utilization is 121 Bytes  < 128 bytes
 **********************************************************************************************************************/
fsp_err_t publishSysVersion(char* inDname,uint32_t inMajor,uint32_t inMinor,uint32_t inPatch,uint32_t txWait, uint32_t rxWait){
    char cmd0[128]={'\0'};
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    snprintf(cmd0, sizeof(cmd0),"AT+NWMQMSG='{\"state\": {\"desired\": {\"version\": \"%02d.%02d.%02d\"}}}',$aws/things/%s/shadow/name/Version/update\r\n", (int) inMajor,(int) inMinor,(int) inPatch,inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"\r\nOK\r\n",rx_wait,0);
    //APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    vTaskDelay(80); // sleep 80ms
    return status;
}   // publishSysVersion()


/*******************************************************************************************************************//**
 *  @brief       Request for the current KMDC Threshold From Cloud
 *  @param[in]   char*      inDNAME     <Device Name>
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 *  @remark      NOTE <<cmd0>> ONLY 128 BYTES of TX MESSAGE!!!!!!!!!!!!!!!!
 *  @remark      Max Significant for inDname 8 s.f., i.e., M1234567
 *  @remark      Total utilization is 8 Bytes  < 128 bytes
 **********************************************************************************************************************/
fsp_err_t publishKMDCThresRequest(char* inDname,uint32_t txWait, uint32_t rxWait){
    char cmd0[128]={'\0'};
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    snprintf(cmd0, sizeof(cmd0),"AT+NWMQMSG='{}',$aws/things/%s/shadow/name/KMDCThreshold/get\r\n", inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"\r\nOK\r\n",rx_wait,0);
    //APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    vTaskDelay(80); // sleep 80ms
    return status;
}   // publishKMDCThresRequest()


/*******************************************************************************************************************//**
 *  @brief       Send the RGB value to $aws/things/+DNAME+/shadow/name/DeviceStatus/update
 *  @param[in]   char* inDNAME <Device Nname>
 *               RGB_Colour inColor   <heart rate value>
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 *  @remark      NOTE <<cmd0>> ONLY 128 BYTES of TX MESSAGE!!!!!!!!!!!!!!!!
 *  @remark      Max Significant for inDname 8 s.f., i.e., M1234567
 *  @remark      Max Significant for inColor 3 s.f., i.e., 999
 *  @remark      Total utilization is 114 Bytes < 128 bytes
 **********************************************************************************************************************/
fsp_err_t publishSysLed(char* inDname,RGB_Colour inColor,uint32_t txWait, uint32_t rxWait){
    //status= sendVital(inDname, 60.0, 12.0, 0.0,4.12, 99.9);
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[128]={'\0'};
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    memset(cmd0,0,sizeof(cmd0));// Set <paylod>,<topic>

    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"state\": {\"desired\": {\"led\": \"%i\"}}}',$aws/things/%s/shadow/name/DeviceStatus/update\r\n",inColor,inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
    vTaskDelay(200); // sleep 200ms
    return status;
} // publishSysLed()


/*******************************************************************************************************************//**
 *  @brief       Send A Dummy Audio Sequence to AeviceMD/+DNAME+/Waveform
 *  @param[in]   char*    inDname     <Device Nname>
 *               uint8_t  inSeq       <SEQ Number>
 *               char*    inAudioVal <2048 bytes>
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 **********************************************************************************************************************/
fsp_err_t publishAudioSeq(char* inDname,char* inTopic,uint8_t inSeq,char* inAudioVal,uint32_t txWait, uint32_t rxWait){
    char cmd0[TX_BUF_LEN]={'\0'};
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    static uint32_t pub_err=0; // count strlen <2000

    /* Caculate crc-16: https://crccalc.com/ */
    uint32_t inCrcVal=0;
    crc_input_t p_crc_input ={
                              .p_input_buffer = inAudioVal,
                              .num_bytes      = 2048,
                              .crc_seed       = 0,
    };
    status = R_CRC_Open(&g_crc_ctrl, &g_crc_cfg);
    status = R_CRC_Calculate(&g_crc_ctrl, &p_crc_input, &inCrcVal);
    status = R_CRC_Close(&g_crc_ctrl);


    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\",\"SEQ\":\"%d\", \"AUDIO\":\"%s\", \"CRC\":\"%x\"}',AeviceMD/%s/%s\r\n",inDname,inSeq,inAudioVal,(unsigned int)inCrcVal,inDname,inTopic);
    //**********************************************************************************************************
    //    if (internet_state_get(G_MQTT)){// if (g_mqtt==1)
    //        char* cmd0_ptr= &cmd0[0];
    //        int size = strlen(cmd0);
    //        int nCHR = (int) 32;
    //        char CHUCK[1024]={'\0'};
    //        int nBLK = (int) size/nCHR;
    //        int nREM = (int) size%nCHR;
    //
    //        /* Print Str Chuck-by-Chuck: nCHR * nBLK */
    //        for (int ii=0;ii<nBLK;ii++){
    //            snprintf(CHUCK,sizeof(CHUCK),"%.*s",nCHR,cmd0_ptr+(ii*nCHR)); // PRINT 512 Sample each iteration
    //            //////APP_PRINT("%s",CHUCK);//printf("%s",CHUCK);
    //            status=sendBuffToWifiUart(CHUCK,strlen(CHUCK),tx_wait,"",rx_wait,0);
    //        }
    //        /* Print the remaining Chuck */
    //        snprintf(CHUCK,sizeof(CHUCK),"%.*s",nREM,cmd0_ptr+(nBLK*nCHR));
    //        //////APP_PRINT("%s",CHUCK);//printf("%s",CHUCK);
    //        status=sendBuffToWifiUart(CHUCK,strlen(CHUCK),tx_wait,"+NWMQMSGSND:1",rx_wait,0);}
    //**********************************************************************************************************
    if (strlen(cmd0)<2000)
        pub_err++;

    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if(internet_state_get(G_MQTT)){
        //status=sendBuffToWifiUart(cmd0,strlen(cmd0),tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
        status=sendBuffToWifiUart(cmd0,TX_BUF_LEN,tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
        vTaskDelay(80); // sleep 80ms
    }
    //    while (status!=FSP_SUCCESS && (internet_state_get(G_MQTT))){ // if (g_mqtt==1)
    //        status=sendBuffToWifiUart(cmd0,strlen(cmd0),tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
    //        vTaskDelay(500); // sleep 500ms
    //    }

    return status;
} // publishAudioSeq()


/*******************************************************************************************************************//**
 *  @brief       Send A Dummy Resend Sequence to AeviceMD/+DNAME+/AudioResend/x
 *  @param[in]   char*    inDname     <Device Nname>
 *               uint8_t  inSeq       <SEQ Number>
 *               char*    inAudioVal <2048 bytes>
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 **********************************************************************************************************************/
fsp_err_t publishResendSeq(char* inDname, char* inTopic, uint8_t inSeq,char* inAudioVal,uint32_t txWait, uint32_t rxWait){
    char cmd0[TX_BUF_LEN]={'\0'};
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply

    /* Caculate crc-16: https://crccalc.com/ */
    uint32_t inCrcVal=0;
    crc_input_t p_crc_input ={
                              .p_input_buffer = inAudioVal,
                              .num_bytes      = 2048,
                              .crc_seed       = 0,
    };
    status = R_CRC_Open(&g_crc_ctrl, &g_crc_cfg);
    status = R_CRC_Calculate(&g_crc_ctrl, &p_crc_input, &inCrcVal);
    status = R_CRC_Close(&g_crc_ctrl);

    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\",\"SEQ\":\"%d\", \"AUDIO\":\"%s\", \"CRC\":\"%x\"}',AeviceMD/%s/AudioResend/%s\r\n",inDname,inSeq,inAudioVal,(unsigned int)inCrcVal,inDname,inTopic);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if(internet_state_get(G_MQTT)){
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
        vTaskDelay(80); // sleep 80ms
    }

    return status;
} // publishResendSeq()

/*******************************************************************************************************************//**
 *  @brief       Send A Dummy Vitals to AeviceMD/+DNAME+/Records
 *  @param[in]   char* inDNAME <Device Nname>
 *               float s_hr   <heart rate value>
 *               float s_rr   <respiratory rate Value>
 *               float s_wz   <wz Classification>
 *               float s_bt   <battery Voltage>
 *               float s_bp   <battery Percentage>
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 **********************************************************************************************************************/
fsp_err_t publishVital(char* inDname, float inHr, float inRr, float inWz,float inBatt,float inBp,uint32_t txWait, uint32_t rxWait){
    //status= sendVital(inDname, 60.0, 12.0, 0.0,4.12, 99.9);
    fsp_err_t status = FSP_ERR_ASSERTION;
    char cmd0[TX_BUF_LEN]={'\0'};
    char s_Hr[10]={'\0'};snprintf(s_Hr,sizeof(s_Hr), "%.2f", inHr);//gcvt(inHr,4,s_Hr);
    char s_Rr[10]={'\0'};snprintf(s_Rr,sizeof(s_Rr), "%.2f", inRr);//gcvt(inRr,4,s_Rr);
    char s_Wz[10]={'\0'};snprintf(s_Wz,sizeof(s_Wz), "%.2f", inWz);//gcvt(inWz,4,s_Wz);
    char s_Batt[10]={'\0'};snprintf(s_Batt,sizeof(s_Batt), "%.2f", inBatt);//gcvt(inBatt,4,s_Batt);
    char s_Bp[10]={'\0'};snprintf(s_Bp,sizeof(s_Bp),  "%.2f", inBp);//gcvt(inBp,4,s_Bp);
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    memset(cmd0,0,sizeof(cmd0));// Set <paylod>,<topic>
    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\", \"HR\":\"%s\", \"RR\":\"%s\", \"WZ\":\"%s\", \"BATTERY\":\"%s\", \"BP\":\"%s\"}',AeviceMD/%s/Records\r\n",inDname, s_Hr, s_Rr, s_Wz, s_Batt, s_Bp,inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
    vTaskDelay(200); // sleep 200ms
    return status;
} // publishVital()

/*******************************************************************************************************************//**
 *  @brief       Send A Dummy Classification to AeviceMD/+DNAME+/Classification
 *  @param[in]   char*      inDNAME     <Device Nname>
 *               uint8_t    inDUR       <05 or 15> Recording
 *               float*     wz12[12]    Ptr to Classification Result Array
 *               uint8_t    inCwd       AUS_MAN or AUS_CWD
 *               char*      inID        SRAM CHECK SOME ID
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 **********************************************************************************************************************/
fsp_err_t publishClassification(char* inDname, uint8_t inDur,float* inClass,uint8_t inCwd,char* inID,uint32_t txWait, uint32_t rxWait){
    char cmd0[TX_BUF_LEN]={'\0'};
    // Convert the Wheeze Classification vlaue into string
    char cWZ05[6];snprintf(cWZ05,6, "%.0f", inClass[0]);//gcvt(inClass[0],4,cWZ05);
    char cWZ10[6];snprintf(cWZ10,6, "%.0f", inClass[1]);//gcvt(inClass[1],4,cWZ10);
    char cWZ15[6];snprintf(cWZ15,6, "%.0f", inClass[2]);//gcvt(inClass[2],4,cWZ15);
    char cWZ20[6];snprintf(cWZ20,6, "%.0f", inClass[3]);//gcvt(inClass[3],4,cWZ20);
    char cWZ25[6];snprintf(cWZ25,6, "%.0f", inClass[4]);//gcvt(inClass[4],4,cWZ25);
    char cWZ30[6];snprintf(cWZ30,6, "%.0f", inClass[5]);//gcvt(inClass[5],4,cWZ30);
    char cWZ35[6];snprintf(cWZ35,6, "%.0f", inClass[6]);//gcvt(inClass[6],4,cWZ35);
    char cWZ40[6];snprintf(cWZ40,6, "%.0f", inClass[7]);//gcvt(inClass[7],4,cWZ40);
    char cWZ45[6];snprintf(cWZ45,6, "%.0f", inClass[8]);//gcvt(inClass[8],4,cWZ45);
    char cWZ50[6];snprintf(cWZ50,6, "%.0f", inClass[9]);//gcvt(inClass[9],4,cWZ50);
    char cWZ55[6];snprintf(cWZ55,6, "%.0f", inClass[10]);//gcvt(inClass[10],4,cWZ55);
    char cWZ60[6];snprintf(cWZ60,6, "%.0f", inClass[11]);//gcvt(inClass[11],4,cWZ60);
    cWZ05[1]='\0';cWZ10[1]='\0';cWZ15[1]='\0';cWZ20[1]='\0';cWZ25[1]='\0';cWZ30[1]='\0';
    cWZ35[1]='\0';cWZ40[1]='\0';cWZ45[1]='\0';cWZ50[1]='\0';cWZ55[1]='\0';cWZ60[1]='\0';
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\",\"DURATION\":\"%d\", \"WHEEZE\":\"%s%s%s%s%s%s%s%s%s%s%s%s\", \"CWD\":\"%d\", \"ID\":\"%s\"}',AeviceMD/%s/Classification\r\n",
            inDname,inDur,
            cWZ60,cWZ55,cWZ50,cWZ45,cWZ40,cWZ35,cWZ30,cWZ25,cWZ20,cWZ15,cWZ10,cWZ05,
            inCwd,inID,inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"\r\nOK\r\n",rx_wait,0);
    vTaskDelay(80); // sleep 80ms
    return status;
}   // publishClassification()


/*******************************************************************************************************************//**
 *  @brief       Publish AudioResend 200/404
 *  @param[in]   char* inDNAME  <Device Nname>
 *               char* inID     <AUS ID>
 *               char* inStatus <200/404>
 *               uint32_t txWait  Wait for txComplete in *10ms
 *               uint32_t rxWait  Wait for rxReply in *10ms
 *  @retval      fsp_err_t status <Expect: FSP_SUCCESS)
 **********************************************************************************************************************/
fsp_err_t publishResendStatus(char* inDname,char* inID, char* inStatus,uint32_t txWait, uint32_t rxWait){
    char cmd0[TX_BUF_LEN]={'\0'};
    fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t tx_wait = txWait;  // 040 ms Wait for Tx Complete
    uint32_t rx_wait = rxWait; // 160 ms Wait for Rx Reply
    memset(cmd0,0,sizeof(cmd0));// Set <paylod>,<topic>
    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\", \"ID\":\"%s\", \"STATUS\":\"%s\"}',AeviceMD/%s/AudioResend/Response\r\n",inDname, inID,inStatus,inDname);
    APP_PRINT("\r\n%s %s",RTT_TIMESTAMP(),cmd0);
    if (internet_state_get(G_MQTT)) // if (g_mqtt==1)
        status=sendBuffToWifiUart(cmd0,sizeof(cmd0),tx_wait,"+NWMQMSGSND:1\r\n",rx_wait,0);
    vTaskDelay(200); // sleep 200ms
    return status;
} // publishResendStatus200()

fsp_err_t testPublishingInChucks(void);
fsp_err_t testPublishingInChucks(void){
    char inDname[] = "EXAMPLE";
    int inSeq = 20;
    char inAudioVal[2049] = "0705080B0304000A0A080A0C080A0900000802080A010908080B000508000A0A030F0B080B0B00090207090000000102010505030004020002030002010E00090A040300070200000009020C0A000A0E090D090808010800030801080905080A090B0F0A090A0A010900030A01070800000A0A010C08030103040202040000010009020101070502050008010209010B0007080C02090A010B08030C00030A02090E000A0C0A0D090008090202090C0A010B0F090008090C01020A020302050A0B010A0C00080104020905010905050003040003090902010A09010508090B000402080203040A0F0C0901080C010301000C09010C0B090D090000090A0106000B0A00010C0A0801010109030208030B0F08030B0200040703090008020B0D020104040804020003090C050109020101050809010A00000D0A0A0909090D080A0A090A0D090A02090D0A0B090F0B0A0809080808010802090A010702020B030503020305070200000000080B09050003080204020B000102030309050009090C0F09080A0B0D0808090A00050500090008090D0D090000000802020008090A000A0D0B0B090101080A050702000003050209090008090B0C000908050301020406020808000202000A0B0808090D0B0A00000B0F000009090902020B0906000C0E090304010C0F0D09000402090B0A000108010706010A0E090801010102020809080102000B0E0C0B0A010405040101000800080A0D0C0D0A0A0802060304020100090A0B090A0B0D0B0B0A0A0802060403040302000008090B0C0B0C0A0B0C0A0908000102040503030304020101080A090B0D0B0B0B0C0B0A0808000104040402030303010000080B0D0A0A0C0B0B0908090801030203050200000104080D0A08090D0D090402010000000C0D0B0901030705020109090A0B0E0A09090000030603030402000A0D0C0B0C09080102050303030100090A0C0B0D0A0B0B0A0800030406030402010108080A0B0C0C0B0A0C0A0908080202050403030303020009090B0B0D0C0A0B0B0C0A090808010304040403030303030100090A0B0C0C0B0B0B0B0C0A090808000002020203040504020303020108090A0A0F0C0B0B0B0A0B09000808010402020205040100020101000908090B0B01000100010601090B0003000B0B0404010002060301000303010100010A0E09080B0F0C0B0B0B0C0C00010002020403010A080C0D0B0B0A090B090108010201070301020B0B0C0F090809020305060201020001080A0B0B0B0A0903020404060401010100080A0A0A0A0B0C09010808010102010A0C0B0E0B0A0B090B0C0C0B080009020303090B0D0F0B090C09010A01030205020202000A090A000B08070404050103020002080A020300000004050100020300010009060009080D090B0F0B0908010900010A09\0";

    char cmd0[1024*3]={'\0'};
    snprintf(cmd0,sizeof(cmd0),"AT+NWMQMSG='{\"DEVICE_NAME\":\"%s\",\"SEQ\":\"%d\", \"AUDIO\":\"%s\"}',AeviceMD/M815F070/Waveform\r\n",inDname,inSeq,inAudioVal);

    char* cmd0_ptr= &cmd0[0];
    int size = (int) strlen(cmd0);
    int nCHR = (int) 128;
    char CHUCK[1024]={'\0'};
    int nBLK = (int) size/nCHR;
    int nREM = (int) size%nCHR;

    /* Print Str Chuck-by-Chuck: nCHR * nBLK */
    for (int ii=0;ii<nBLK;ii++){
        //printf("%.*s\n", size, substring_start);//printf("%.*s",nCHR,cmd0_ptr+(ii*nCHR));
        snprintf(CHUCK,sizeof(CHUCK),"%.*s",nCHR,cmd0_ptr+(ii*nCHR)); // PRINT 512 Sample each iteration
        ////APP_PRINT("%s",CHUCK);//printf("%s",CHUCK);
    }
    /* Print the remaining Chuck */
    snprintf(CHUCK,sizeof(CHUCK),"%.*s",nREM,cmd0_ptr+(nBLK*nCHR));
    ////APP_PRINT("%s",CHUCK);//printf("%s",CHUCK);

    /* Total Number of Variables involved*/
    ////APP_PRINT("\n%d: %d*%d+%d=%d",size,nBLK,nCHR,nREM,nBLK*nCHR+nREM);//printf("\n%d: %d*%d+%d=%d",size,nBLK,nCHR,nREM,nBLK*nCHR+nREM);

    /* Sanity Check*/
    ////APP_PRINT("\n%s",cmd0);//printf("\n%s",cmd0);

    return 0;
}// testPublishingInChucks()


