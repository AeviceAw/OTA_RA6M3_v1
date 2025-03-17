/*
 * wifiHandler.h
 *
 *  Created on: 10 Jun 2024
 *      Author: yyang
 */

#ifndef WIFIHANDLER_H_
#define WIFIHANDLER_H_

#include "initialisations.h"

/* Synchronization Flags */
#define MQTT_FLAG_CLEAR 0x00
#define MQTT_FLAG_DISCONNECTED 0x001
#define MQTT_FLAG_CONNECTED    0x010
#define MQTT_FLAG_CONNECTING   0x100
#define MQTT_FLAG_UNKNOWN     0x1000
#define MQTT_FLAG_ALL         0x1111

/* Defines */
#define UART_TX_RETRY (64)  // retries for R_SCI_UART_WRITE()
#define UART_RX_RETRY (256) // retries for Rx wait after R_SCI_UART_WRITE()
#define TX_BUF_LEN (1024*3) // No of Elements for Global Wi-fi Uart TX Buffer
#define RX_BUF_LEN (1024*3) // No of Elements for Global Wi-fi Uart RX Buffer

/* Output for RTOS's <<g_wifi_event_flags>> and Firmware's <<g_aws_flags>>*/
typedef enum mqtt_event_flag{
    CLEAR = (uint32_t)MQTT_FLAG_CLEAR,
    DISCONNECTED = (uint32_t)MQTT_FLAG_DISCONNECTED,
    CONNECTED = (uint32_t)MQTT_FLAG_CONNECTED,
    CONNECTING = (uint32_t)MQTT_FLAG_CONNECTING,
    UNKNOWN = (uint32_t)MQTT_FLAG_UNKNOWN
}mqtt_flag_t;
extern mqtt_flag_t g_aws_flags;

///* Inputs for wifiHandler's internet_state_get */
//typedef enum internet_state_flag{
//    // Inputs option for internet_state_get()
//    G_WIFI=(uint8_t)1,
//    G_MQTT=(uint8_t)2,
//    G_UNKNOWN
//}internet_state_t;
//
///* Output for wifiHandler g_variable g_mqtt */
//typedef enum mqtt_status {
//    // Output for g_mqtt
//    MQTT_ON  = 1,
//    MQTT_OFF = 0
//} mqtt_status_t;
//
///* Output for wifiHandler g_variable g_wifi */
//typedef enum wifi_status {
//    // Output for w_mqtt
//    WIFI_ON  = 1,
//    WIFI_OFF = 0
//} wifi_status_t;


/* (1a) Prototype functions to Open/Close UART Port to comms with DA16200*/
fsp_err_t wifi_uart_init(void);
fsp_err_t wifi_uart_deinit(void);
fsp_err_t wifi_uart_flag_clear(void);

///* (1b) For setting/clearing g_variables: g_wifi/g_mqtt */
//fsp_err_t internet_state_set(void);
//uint8_t internet_state_get(internet_state_t mode);

/* (1c) For updating/waiting RTOS's <<g_wifi_event_flags>> */
//void wifi_event_flag_update(uint32_t MQTT_FLAG_STATE);
//void wifi_event_flag_wait(uint32_t MQTT_FLAG_STATE);
//void checkWififlagAndUartflag(void);

/* (1d) For preparing variables inside Da16200_uart_callback() */
char* wifi_uart_gTxBuffAdd(void); // Get the address of g_tx_buff
char* wifi_uart_gRxBuffAdd(void); // Get the address of g_rx_buff
fsp_err_t wifi_uart_gTxBuff_clear(void);// Clear elements of g_tx_buff
fsp_err_t wifi_uart_gRxBuff_clear(void);// Clear elements of g_rx_buff

/* (2) Prototype Function to Configure and use DA16200MOD */
fsp_err_t Da16200_masterSetup(char* ssid,char* password);
fsp_err_t sendBuffToWifiUart(char*g_buf,uint32_t buf_len,uint32_t noTxRetry,char* wReply,uint32_t noRxRetry,uint8_t ON_LOG);
fsp_err_t Da16200_attention(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_init(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_factory(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_echo(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_restart(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_setWifi(char* ssid,char* password,uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_startDHCP(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_pingAddr(char* ip_addr_str,uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_setSNTP(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_setMqttPingTimeout(uint32_t period_in_sec, uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_setMQTTBroker(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_setAWSCerts(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_subTopic(uint32_t txWait, uint32_t rxWait);
fsp_err_t Da16200_startMQTTBroker(uint32_t txWait, uint32_t rxWait);

/* (3) Prototype Function publish to mqtt, after <<MQTT_ON>> */
fsp_err_t publishSysDiagnostic(char* inDname,char* inMsg,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishSysVersion(char* inDname,uint32_t inMajor,uint32_t inMinor,uint32_t inPatch,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishKMDCThresRequest(char* inDname,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishSysLed(char* inDname,RGB_Colour inColor,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishVital(char* inDname, float inHr, float inRr, float inWz,float inBatt,float inBp,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishAudioSeq(char* dname,char* inTopic,uint8_t idx,char* audio_val,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishClassification(char* inDname, uint8_t inDur,float* inClass,uint8_t inCwd,char* inID,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishResendSeq(char* dname, char* inTopic, uint8_t idx,char* audio_val,uint32_t txWait, uint32_t rxWait);
fsp_err_t publishResendStatus(char* inDname,char* inID, char* inStatus,uint32_t txWait, uint32_t rxWait);

//char *gcvt(double number, int ndigit, char *buf);
#endif /* WIFIHANDLER_H_ */
