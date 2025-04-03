/*
 * subscribeHandler.c
 *
 *  Created on: 11 Jun 2024
 *      Author: yyang
 */
#include "initialisations.h"

/* Global Variable */
char g_seqResend[60]={'\0'};
char g_idResend[10]={'\0'};

/**********************************************************************************************************************
 *  @brief       assert if wifi_uart received AP disconnected reply:
 *
 *  @retval      1         AP Disconnected
 *               0         otherwise
 **********************************************************************************************************************/
uint8_t assertDa16200_disconnected(void){
    volatile char *p_rx_dc_wifi = NULL;           // pointer to g_rx_buf
    p_rx_dc_wifi = strstr(wifi_uart_gRxBuffAdd(),"+WFDAP:0"); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
    //    if (p_rx_dc_wifi){
    volatile char *p_rx_dc_mqtt = NULL;           // pointer to g_rx_buf
    p_rx_dc_mqtt = strstr(wifi_uart_gRxBuffAdd(),"+NWMQCL:0");//p_rx_dc_mqtt = strstr(wifi_uart_gRxBuffAdd(),"ERROR:-634"); //634 MQTT client is currently not connecte
    volatile char *p_rx_err_mqtt = NULL;           // pointer to g_rx_buf
    p_rx_err_mqtt = strstr(wifi_uart_gRxBuffAdd(),"ERROR:-63"); //63x MQTT client is currently not connected
    if (p_rx_dc_wifi||p_rx_dc_mqtt || p_rx_err_mqtt){
        wifi_uart_flag_clear();wifi_uart_gRxBuff_clear();

        /* Set Wi-Fi Event Flags: DISCONNECTED */
        //wifi_event_flag_update(MQTT_FLAG_DISCONNECTED);
        xEventGroupSetBits(g_wifi_event, MQTT_FLAG_DISCONNECTED);

        /* Clear True Check State Value */
        setTrueCheckVal(t_AUS_NONE);

        /* Disconnect Sensor */
        BLE_disconnect();

        ////APP_PRINT("WIFI/MQTT DISCONNECTED\n");

        TickType_t xLastWakeTime = xTaskGetTickCount(); // Give wifi_thread_entry time to process
        vTaskDelayUntil(&xLastWakeTime, 10000); // sleep 10000ms
        return 1;
    }// end <<1>> if (WIFI_DISCONNECT)
    else{
        return 0;
    } // <<1>>
} // <<0>> assertDa16200_disconnected()

/**********************************************************************************************************************
 * @brief       assert if wifi_uart received AP connected reply:
 *
 * @retval      1         AP Connected
 *              0         otherwise
 **********************************************************************************************************************/
uint8_t assertDa16200_connected(void){
    volatile char *p_rx_buf = NULL;           // pointer to g_rx_buf
    p_rx_buf = strstr(wifi_uart_gRxBuffAdd(),"+WFJAP:1"); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
    if (p_rx_buf){
        /* Start MQTT Broker Connection */
        Da16200_startMQTTBroker(4,16); // 004 ms for Tx Complete, 160 ms for Rx reply

        internet_state_set();wifi_uart_gRxBuff_clear();

        /* Set Wi-Fi Event Flags: MQTT_CONNECTED */
        xEventGroupSetBits(g_wifi_event, MQTT_FLAG_CONNECTED);

        ////APP_PRINT("WIFI CONNECTED\n");
        return 1;
    }// end <<1>> if (WIFI_CONNECT)
    else{
        return 0;
    } // end <<1>>
} // <<0>> assertDa16200_disconnected()

/**********************************************************************************************************************
 * @brief       assert if wifi_uart received DURATION MQTT Message From AWS
 * @param[in]   char*   <keyword>, e.g., char keyword[]="\"DURATION\":"
 * @param[in]  uint8_t  <noOfChar>,e.g., 4 for DUR "15" and 6 for KMDC "0.55"
 * @retval      uint8_t <DUR>    ,  Duration Value
 *                        0      ,  otherwise
 *
 * e.g., char g_rx_buf[]="+NWMQMSG:{\"DEVICE_NAME\":\"M815F070\",\"DURATION\":\"15\",\"ERROR\":\"0\"},AeviceMD/M815F070/Status,54";
 *       char keyword[] ="\"DURATION\":";
 *       uint8_t dur = assertDa16200_MqttMsg(keyword)
 *       ////APP_PRINT("Duration: =%d",dur) // 15
 **********************************************************************************************************************/
int8_t assertDa16200_MqttMsg(char* keyword,uint8_t noOfChar){
    volatile char *p_rxBufDname = NULL;      // pointer to g_rx_buf
    volatile char *p_rxBufKword = NULL;      // pointer to g_rx_buf
    volatile char *p_RxBuffAdd = wifi_uart_gRxBuffAdd();
    volatile char xStr[5] = {'\0'};  // Note 5 = 1+4 !!!! VERY IMPORTANT
                                     // Extracted String

    //volatile char[RX_BUF_LEN] rxBuf = {'\0'}; // store g_rx_buf to prevent overwritten
    //strcpy((char*)rxBuf,g_rx_buf);            // cpy global to local;
    //volatile char[] keyword = "\"DURATION\":";

    volatile uint32_t keyword_len = strlen(keyword);
    p_rxBufDname = strstr((char*)p_RxBuffAdd,"+NWMQMSG:"); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");

    if (p_rxBufDname){
        p_rxBufKword = strstr((char*)p_RxBuffAdd,keyword);                     //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
        if( p_rxBufKword){
            /* Start <<Idx>> of <<Keyword>>, e.g., ii1 of "ey" = 1 */
            uint32_t ii1 = (uint32_t) (strlen((char*)wifi_uart_gRxBuffAdd())-strlen((char*)p_rxBufKword));
            /* Stop  <<Idx>> of <<Keyword>>, e.g., ii2 of "ey" = 2 */
            uint32_t ii2 = (uint32_t) (ii1+ keyword_len-1);
            /* Offset ptr g_rx_buf to 1 pos after <<keyword>> */
            p_RxBuffAdd += (ii2+1); // e.g., &"w", "word"
            /* Extract 4 Position After <<keyword>> */
            //strncpy((char*)xStr,(char*)p_RxBuffAdd,4); // Extract Duration: \"15\"
            strncpy((char*)xStr,(char*)p_RxBuffAdd,noOfChar); // Extract Duration: \"15\"
            /* Log Value */
            ////APP_PRINT("%s(%d,%d): (%s,%d)\n",keyword,ii1,ii2,xStr,atoi((char*)&xStr[1]));
            /* Clear g_rx_buf */
            wifi_uart_gRxBuff_clear();
            /* Convert the extracted DUR to an integer */
            return (int8_t) atoi((char*)&xStr[1]);
        }// end <<2>> if (KEYWORD)
        else{
            return -1;
        } // end <<2>> if (!KEYWORD)
    } // end <<1>> if (+NWMQMSG:)
    else{
        return -1;
    } // end <<1>> if (!+NWMQMSG:)
} // <<0>. assertDa16200_MqttMsg()

/**********************************************************************************************************************
 * @brief       assert if wifi_uart received KMDC_THreshold MQTT Message From AWS
 * @param[in]   char*   <keyword>, e.g., char keyword[]="\"DURATION\":"
 * @param[in]  uint8_t  <noOfChar>,e.g., 4 for DUR "15" and 6 for KMDC "0.55"
 * @retval     Float32_t <kmdc_thres>  ,  Duration Value
 *                              -1     ,  otherwise
 *
 * e.g., char g_rx_buf[]="+NWMQMSG:{\"DEVICE_NAME\":\"M815F070\",\"DURATION\":\"15\",\"ERROR\":\"0\"},AeviceMD/M815F070/Status,54";
 *       char keyword[] ="\"DURATION\":";
 *       uint8_t dur = assertDa16200_MqttMsg(keyword)
 *       ////APP_PRINT("Duration: =%d",dur) // 15
 **********************************************************************************************************************/
float assertDa16200_MqttMsgF(char* keyword,uint8_t noOfChar){
    volatile char *p_rxBufDname = NULL;      // pointer to g_rx_buf
    volatile char *p_rxBufKword = NULL;      // pointer to g_rx_buf
    volatile char *p_RxBuffAdd = wifi_uart_gRxBuffAdd();
    volatile char xStr[7] = {'\0'};  // Note 5 = 1+6 !!!! VERY IMPORTANT
                                     // Extracted String

    //volatile char[RX_BUF_LEN] rxBuf = {'\0'}; // store g_rx_buf to prevent overwritten
    //strcpy((char*)rxBuf,g_rx_buf);            // cpy global to local;
    //volatile char[] keyword = "\"DURATION\":";

    volatile uint32_t keyword_len = strlen(keyword);
    p_rxBufDname = strstr((char*)p_RxBuffAdd,"+NWMQMSG:"); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");

    if (p_rxBufDname){
        p_rxBufKword = strstr((char*)p_RxBuffAdd,keyword);                     //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
        if( p_rxBufKword){
            /* Start <<Idx>> of <<Keyword>>, e.g., ii1 of "ey" = 1 */
            uint32_t ii1 = (uint32_t) (strlen((char*)wifi_uart_gRxBuffAdd())-strlen((char*)p_rxBufKword));
            /* Stop  <<Idx>> of <<Keyword>>, e.g., ii2 of "ey" = 2 */
            uint32_t ii2 = (uint32_t) (ii1+ keyword_len-1);
            /* Offset ptr g_rx_buf to 1 pos after <<keyword>> */
            p_RxBuffAdd += (ii2+1); // e.g., &"w", "word"
            /* Extract 4 Position After <<keyword>> */
            //strncpy((char*)xStr,(char*)p_RxBuffAdd,4); // Extract Duration: \"15\"
            strncpy((char*)xStr,(char*)p_RxBuffAdd,noOfChar); // Extract Duration: \"15\"
            /* Log Value */
            ////APP_PRINT("%s(%d,%d): (%s,%d)\n",keyword,ii1,ii2,xStr,atoi((char*)&xStr[1]));
            /* Clear g_rx_buf */
            wifi_uart_gRxBuff_clear();

            /* Convert the extracted DUR to an float */
            // Remove the surrounding quotes (i.e., char at index 0 and last index)
            char *num_str = xStr + 1;  // Skip the first quote
            num_str[strlen(num_str) - 1] = '\0';  // Remove the last quote

            // Convert to float
            float value = atof(num_str);  // Or use strtof for better error handling

            return (float) value;
        }// end <<2>> if (KEYWORD)
        else{
            return -1.0f;
        } // end <<2>> if (!KEYWORD)
    } // end <<1>> if (+NWMQMSG:)
    else{
        return -1.0f;
    } // end <<1>> if (!+NWMQMSG:)
} // <<0>. assertDa16200_MqttMsgF()

/**********************************************************************************************************************
 * @brief       assert if wifi_uart received ID MQTT Message From AWS
 * @param[in]   char*   <keyword>, e.g., char keyword[]="\"ERROR\":"
 * @param[in]   char*   <dst>    , e.g., char dst[60] = {'\0'};
 * @param[in]   size_t  <dst_len>, e.g, dst_len = 60
 * @param[in]   bool  <clearAfterRead>, clear RX buffer (IRQ) after reading? e.g, 1 or 0
 * @retval      uint8_t <DUR>    ,  Duration Value
 *                        0      ,  otherwise
 *
 * e.g., char g_rx_buf[]="+NWMQMSG:{\"DEVICE_NAME\":\"M815F070\",\"ID\":\"1258426693\",\"ERROR\":\"000000000000111111110000000000000000000000000000000000000000\"},AeviceMD/M815F070/Status,54";
 *       char keyword[] ="\"ERROR\":";
 *       uint8_t dur = assertDa16200_AudioResend(keyword)
 *       ////APP_PRINT("Duration: =%d",dur) // 15
 **********************************************************************************************************************/
int8_t assertDa16200_AudioResend(char* keyword,char* dst,size_t dst_len,bool clearAfterRead){
    volatile char *p_rxBufDname = NULL;      // pointer to g_rx_buf
    volatile char *p_rxBufKword = NULL;      // pointer to g_rx_buf
    volatile char *p_RxBuffAdd = wifi_uart_gRxBuffAdd();
    volatile char xStr[60+2+1] = {'\0'};  // Note 5 = 1+4
                                     // Extracted String

    //volatile char[RX_BUF_LEN] rxBuf = {'\0'}; // store g_rx_buf to prevent overwritten
    //strcpy((char*)rxBuf,g_rx_buf);            // cpy global to local;
    //volatile char[] keyword = "\"DURATION\":";

    volatile uint32_t keyword_len = strlen(keyword);
    p_rxBufDname = strstr((char*)p_RxBuffAdd,"+NWMQMSG:"); //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");

    if (p_rxBufDname){
        p_rxBufKword = strstr((char*)p_RxBuffAdd,keyword);                     //p_rx_buf = strstr(g_rx_buf, "\r\nOK\r\n");
        if( p_rxBufKword){
            /* Start <<Idx>> of <<Keyword>>, e.g., ii1 of "ey" = 1 */
            uint32_t ii1 = (uint32_t) (strlen((char*)wifi_uart_gRxBuffAdd())-strlen((char*)p_rxBufKword));
            /* Stop  <<Idx>> of <<Keyword>>, e.g., ii2 of "ey" = 2 */
            uint32_t ii2 = (uint32_t) (ii1+ keyword_len-1);
            /* Offset ptr g_rx_buf to 1 pos after <<keyword>> */
            p_RxBuffAdd += (ii2+1); // e.g., &"w", "word"
            /* Extract 4 Position After <<keyword>> */
            strncpy((char*)xStr,(char*)p_RxBuffAdd,62); // Extract Duration: \"15\"
            /* Log Value */
            ////APP_PRINT("%s(%d,%d): (%s,%d)\n",keyword,ii1,ii2,xStr,atoi((char*)&xStr[1]));
            /* Clear g_rx_buf */
            if (clearAfterRead)
                wifi_uart_gRxBuff_clear();
            /* strCpy <<xStr>> to <<dst>> */
            /* xStr[63] = \"000000000000000000000000000000000000000001000000000000000000\0\" */
            memset(dst,0,dst_len); memcpy(dst,(char*)xStr+1,dst_len);
            /* Set True_Check == AudioResend AUS_RESEND:*/
            if (getTrueCheckVal()==t_AUS_STOP)
            	setTrueCheckVal(t_AUS_NONE);  // Stop AUDIO_RESEND, Stop AUS
            else if (getTrueCheckVal() == t_AUS_MAN_15)
            	setTrueCheckVal(t_AUS_MAN_RESEND);
            else if (getTrueCheckVal() == t_AUS_CWD)
            	setTrueCheckVal(t_AUS_CWD_RESEND);
            else
                setTrueCheckVal(t_AUS_MAN_RESEND);
            /* Return FSP_SUCCESS */
            return 0;
        }// end <<2>> if (KEYWORD)
        else{
            return -1;
        } // end <<2>> if (!KEYWORD)
    } // end <<1>> if (+NWMQMSG:)
    else{
        return -1;
    } // end <<1>> if (!+NWMQMSG:)
} // <<0>. assertDa16200_AudioResend()

/**********************************************************************************************************************
 * @brief       Return the pointer to g_idResend[11] :
 *
 * @retval      char*    ptr
 **********************************************************************************************************************/
char* AudioResendID_ptr(void){
    return (char*) g_idResend;
} //AudioResendID_ptr();


/**********************************************************************************************************************
 * @brief       Set All the Values of g_idResend[11] :
 * @param[in]   uint8_t inVal to set
 * @retval      N.A
 **********************************************************************************************************************/
void AudioResendID_set(uint8_t inVal){
    memset(g_idResend,inVal,sizeof(g_idResend));
} //AudioResendID_get();


/**********************************************************************************************************************
 * @brief       Return the pointer to g_seqResend[60] :
 *
 * @retval      char*    ptr
 **********************************************************************************************************************/
char* AudioResendSelector_ptr(void){
    return (char*) g_seqResend;
} //AudioResendSelector_ptr();


/**********************************************************************************************************************
 * @brief       Set All the Values of g_seqResend[60] :
 * @param[in]   uint8_t inVal to set
 * @retval      N.A
 **********************************************************************************************************************/
void AudioResendSelector_set(uint8_t inVal){
    memset(g_seqResend,inVal,sizeof(g_seqResend));
} //AudioResendSelector_get();

/**********************************************************************************************************************
 * @brief       initiate a Dock State change based on received <<dur>> value
 * @ param[in]  uint8_t <dur>
 * @retval      NONE
 **********************************************************************************************************************/
void decodeDurState(int8_t dur){
    switch (dur){
        case -1:{
            /* No DURATION FOUND */
            break;
        } // end (AUS_CANCEL)
        case 0:{
            /* Cancel CWD */
            setTrueCheckVal(t_AUS_STOP);
            //tx_semaphore_ceiling_put(&g_data_semaphore,1);
            //tx_semaphore_ceiling_put(&g_publish_semaphore,1);
            ////APP_PRINT("DUR(%02d):Cancel CWD\n",dur);
            break;
        } // end (00:AUS_CANCEL)
        case 1:{
            /* Publish System Version */
            publishSysVersion(DNAME, MMP_MAJOR_VERSION, MMP_MINOR_VERSION, MMP_PATCH,8,4);
            ////APP_PRINT("DUR(%02d): Publish system version\n",dur);
            break;
        } // end (01:AUS_CANCEL)
        case 2:{
            /* System Rset  */
            NVIC_SystemReset();
            break;
        } // end (02: SystemReset)
        case 5:{
            /* 5-sec Auscultation */
            ////APP_PRINT("DUR(%02d):05-sec Auscultation\n",dur);
        	publish_seq_clear();
        	setTrueCheckVal(t_AUS_MAN_05);
            //tx_semaphore_ceiling_put(&g_publish_semaphore,1);
            //tx_semaphore_ceiling_put(&g_publish_semaphore,1);
            break;
        } // end (05: AUS_05sec)
        case 11:{
                /* DEPRICIATED : End Ausculcation Mode */
                ////APP_PRINT("AUS_DONE!\n");
        		publish_seq_clear();
                setTrueCheckVal(t_AUS_NONE);
                break;
        } // end (11: End Auscultation)
        case 13:{
            /* Trigger Run-time Exception to test Watchdog */
            g_awscmd_set(CMD_WATCHDOG_TEST);
            break;
        } // end (13: Test WatchDog Timer - Create Failure)
        case 15:{
            /* 15-sec Auscultation */
            ////APP_PRINT("DUR(%02d):15-sec Auscultation\n",dur);
        	publish_seq_clear();
        	setTrueCheckVal(t_AUS_MAN_15);
            //tx_semaphore_ceiling_put(&g_publish_semaphore,1);
            //tx_semaphore_ceiling_put(&g_publish_semaphore,1);
            break;
        } // end (15: AUS_15sec)
        case 33: {
            // TODO: DUR 33 - Add KDMC Threshold Get message
            g_awscmd_set(CMD_KMDC_THRES_GET);
            break;
        } // end (33: KMDC Thresholding Message)
        case 81:{
            /* BLE Always ON */
            // TODO: DUR 81 - BLE=always_on
            ////APP_PRINT("DUR(%02d):BLE always ON\n",dur);
            g_awscmd_set(CMD_BLE_ALWAYS_ON);
            break;
        } // end (81: ble=1)
        case 82:{
            /* BLE == GPIO Value ON */
            // TODO: DUR 82 - BLE=GPIO Value
            ////APP_PRINT("DUR(%02d):BLE always ON\n",dur);
            g_awscmd_set(CMD_BLE_NORMAL);
            break;
        } // end (82: g_ble=BLE_check())
        case 83:{
            /* Revert WZ to Algorithm Dependent */
            // TODO: DUR 83 - WZ= getKMDC()
            ////APP_PRINT("DUR(%02d): BLE depends on GPIO Value\n",dur);
            g_awscmd_set(CMD_WZ_NORMAL);
            break;
        } // end (83: WZ=getKMDC())
        case 84:{
            /* Force WZ to be always ON */
            // TODO: DUR 84 - WZ=always_on
            ////APP_PRINT("DUR(%02d):Force WZ to be always ON\n",dur);
            g_awscmd_set(CMD_WZ_ALWAYS_ON);
            break;
        } // end (84: WZ=1)
        case 85:{
            /* Force WZ to be always OFF */
            // TODO: DUR 85 - WZ=always_off
            ////APP_PRINT("DUR(%02d):Force WZ to be always OFF\n",dur);
            g_awscmd_set(CMD_WZ_ALWAYS_OFF);
            break;
        } // end (85: WZ=0)
        case 86:{
            /* Revert IMU to be Sensor Dependent */
            imu_cmd_set(false);
            ////APP_PRINT("DUR(%02d):Revert IMU to be Sensor Dependent\n",dur);
            break;
        } // end (86: IMU=getOnBody())
        case 87:{
            /* FORCE IMU to be Always ON */
            imu_cmd_set(true);
            ////APP_PRINT("DUR(%02d): IMU to be awlays on\n",dur);
            break;
        } // end (87: IMU=1)
        case 88:{
            /* Start OTA */
            vStartOtaDemo();
            ////APP_PRINT("DUR(%02d): IMU to be awlays on\n",dur);
            break;
        } // end (88: OTA init)
        case 99:{
            /* Reinitialize all the variables */
            // TODO: DUR 99 - Variables Reinitialie
            ////APP_PRINT("DUR(%02d): Variables Reinitialize\n",dur);
            g_awscmd_set(CMD_INIT_VAR);
            break;
        } // end (99: reinitailize variables)
        default:{
            /* Unknown/Reserved State */
            ////APP_PRINT("DUR(%02d):Unknown/Reserved State\n",dur);
            break;
        }
    } // end switch(dur)
}// decodeDurState()

/**********************************************************************************************************************
// *  @brief      Get global <<g_trueCheck>> value
// *
// *  @retval      trueCheck_t        AUS_MAN/AUS_NONE
// **********************************************************************************************************************/
//trueCheck_t getTrueCheckVal(void){
//    return (trueCheck_t) g_trueCheck;
//} // end getTrueCheckVal()
//
///**********************************************************************************************************************
// *  @brief      Set global <<g_trueCheck>> value
// *
// *  @retval      trueCheck_t        AUS_MAN/AUS_NONE
// **********************************************************************************************************************/
//trueCheck_t setTrueCheckVal(trueCheck_t val){
//    g_trueCheck = (trueCheck_t) val;
//    return (trueCheck_t) g_trueCheck;
//} // end setTrueCheckVal()
