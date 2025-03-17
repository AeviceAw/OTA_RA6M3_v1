/*
 * SystemVariables.c
 *
 *  Created on: 5 Jun 2024
 *      Author: WilsonQuah
 */

//#include "stdint.h"
//#include "SystemVariables.h"
#include "initialisations.h"

#define BUF_LEN 12


//mqtt_status_enum mqtt = MQTT_OFF;


// SSID Variable
uint8_t feedbackready = 0;
fbState_t feedbackState = 0;	//Set by T_WIFI and T_UART

/**********************************************************************************************************************
 * @brief       Set the status of the feedbackready <<g_feedbackready = inVal>>
 * @param       uint8_t         inVal
 * @retval      None
 **********************************************************************************************************************/
void g_feedbackready_set(uint8_t inVal){
    feedbackready = (uint8_t) inVal;
} // end g_feedbackready_set()

/**********************************************************************************************************************
 * @brief       Get the status of the feedbackready
 * @param       None
 * @retval      fbState_flag g_feedbackready value
 **********************************************************************************************************************/
uint8_t g_feedbackready_get(void){
    return feedbackready ;
} // end g_feedbackready_get()

/**********************************************************************************************************************
 * @brief       Set the status of the feedbackState <<g_feedbackState =  inVal>>
 * @param       (fbState_t)         inVal
 * @retval      None
 **********************************************************************************************************************/
void g_feedbackState_set(fbState_t inVal){
    feedbackState = (fbState_t) inVal;
} // end g_feedbackState_set()

/**********************************************************************************************************************
 * @brief       Get the status of the  feedbackState
 * @param       None
 * @retval      (fbState_t) g_feedbackState value
 **********************************************************************************************************************/
fbState_t g_feedbackState_get(void){
    return feedbackState ;
} // end g_feedbackState_get()


/* Global State Variable */
wifi_status_t g_wifi = 0;
mqtt_status_t g_mqtt = 0;

/* SRAM */
bool g_sram_ok = false;

// AUSCULTATION VARIABLEs
trueCheck_t g_trueCheck = t_AUS_NONE;
uint32_t rTime = 0;
uint8_t last_pub_seq = 0;
float wz12[BUF_LEN]={0.0f};
uint8_t seq=0;

/**********************************************************************************************************************
 * @brief		Set the status of the SRAM Check g_sram = inBool
 * @param       bool 		inBool
 * @retval      None
 **********************************************************************************************************************/
void g_sram_set(bool inBool){
	g_sram_ok = (bool) inBool;
} // end g_sram_set()

/**********************************************************************************************************************
 * @brief		Get the status of the SRAM Check g_sram
 * @param       None
 * @retval      g_sram value
 **********************************************************************************************************************/
bool g_sram_get(void){
	return g_sram_ok ;
} // end g_sram_get()


/**********************************************************************************************************************
 * @brief		Clear  the MQTT & WIFI flags:
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 **********************************************************************************************************************/
fsp_err_t internet_state_clear(void){
    if (g_wifi)
        g_wifi = WIFI_OFF;
    if (g_mqtt)
        g_mqtt = MQTT_OFF;
    return FSP_SUCCESS;
} // end internet_state_clear()


/**********************************************************************************************************************
 *  Set  the MQTT & WIFI flags:
 *
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 **********************************************************************************************************************/
fsp_err_t internet_state_set(void){
    if (!g_wifi)
        g_wifi = WIFI_ON;
    if (!g_mqtt)
        g_mqtt = MQTT_ON;
    return FSP_SUCCESS;
} // end internet_state_set()

/**********************************************************************************************************************
 *  Set  the WIFI flags:
 *
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 **********************************************************************************************************************/
fsp_err_t wifi_state_set(void){
    if (!g_wifi)
        g_wifi = WIFI_ON;
    return FSP_SUCCESS;
} // end wifi_state_set()

/**********************************************************************************************************************
 *  Set  the MQTT lags:
 *
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 **********************************************************************************************************************/
fsp_err_t mqtt_state_set(void){
    if (!g_mqtt)
        g_mqtt = MQTT_ON;
    return FSP_SUCCESS;
} // end mqtt_state_set()


/**********************************************************************************************************************
 *  Get  the g_MQTT/g_WIFI Value:
 * @param[in] uint8_t  <mode> 1: g_wifi value, 2: g_mqtt_value
 * @retval    uint8_t  val    1 or 0
 **********************************************************************************************************************/
uint8_t internet_state_get(internet_state_t mode){
    switch (mode){
        case G_WIFI:{
            return (uint8_t) g_wifi;
            break;
        }
        case G_MQTT:{
            return (uint8_t) g_mqtt;
            break;
        }
        default: {
            return 0;
            break;
        }
    } // end switch (mode)
} // end wifi_uart_flag_get()

/**********************************************************************************************************************
 *  @brief      Get global Auscultation <<g_trueCheck>> value
 *
 *  @retval      trueCheck_t        AUS_MAN/AUS_NONE
 **********************************************************************************************************************/
trueCheck_t getTrueCheckVal(void){
    return (trueCheck_t) g_trueCheck;
} // end getTrueCheckVal()

/**********************************************************************************************************************
 *  @brief      Set global Auscultation <<g_trueCheck>> value
 *
 *  @retval      trueCheck_t        AUS_MAN/AUS_NONE
 **********************************************************************************************************************/
trueCheck_t setTrueCheckVal(trueCheck_t val){
    g_trueCheck = (trueCheck_t) val;
    return (trueCheck_t) g_trueCheck;
} // end setTrueCheckVal()


/**********************************************************************************************************************
 *  @brief      write the WZ value into the wz12 buffer based on AUDIO_SET1/AUDIO_SET2/AUDIO_SET3
 *  @param[in]  (dataprocess_state_t) in_dataprocess_state (AUDIO_SET1/AUDIO_SET2/AUDIO_SET3)
 *              (float)               inWZ                  Values to be written to wz12
 *  @retval     (float)               wz12[12]
 *  @Remark		wz12_set(AUDIO_SET1,1.0f);wz12_set(AUDIO_SET2,0.0f);wz12_set(AUDIO_SET3,0.0f);
 *              wz12[0] = 1.0f;  wz12[1] = 0.0f;  wz12[2] = 0.0f
 **********************************************************************************************************************/
void wz12_set(dataprocess_state_t in_dataprocess_state,float inWZ){
	switch (in_dataprocess_state){
	case AUDIO_SET1:
		wz12[0]= inWZ;//1;//
		break;
	case AUDIO_SET2:
		wz12[1]= inWZ;//2;//
		break;
	case AUDIO_SET3:
		wz12[2]= inWZ;//3;//
		break;
	default:
		break;
	}
} // end wz12_set()

/**********************************************************************************************************************
 *  @brief      clear the WZ value into the wz12 by setting all values to "0"
 *  @param[in]  (float)               wz12[12] = [x,...,x]
 *  @retval     (float)               wz12[12] = [0,...,0]
 **********************************************************************************************************************/
void wz12_clear(void){
	memset(wz12,0,sizeof(wz12));
} // end wz12_clear()

/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(float)  wz12[12]>>
 *  @param       none     <<(float)  wz12[12]>>
 *  @retval      none     <<(float)  wz12[12]>>
 **********************************************************************************************************************/
float* getWz12dAddr(void){
	return (float*) wz12;
} // end getWz12dAddr()

// AWS COMMAND VARIABLES for System Testing
aws_cmd_t g_aws_cmd = CMD_NONE;
/**********************************************************************************************************************
 * @brief       Set the current received AWS CMD
 * @param       aws_cmd_t        inCmd
 * @retval      None
 **********************************************************************************************************************/
void g_awscmd_set(aws_cmd_t inCmd){
    g_aws_cmd =  inCmd;
} // end g_awscmd_set()

/**********************************************************************************************************************
 * @brief       Get the current received AWS CMD
 * @param       None
 * @retval      aws_cmd_curr value
 **********************************************************************************************************************/
aws_cmd_t g_awscmd_get(void){
    return g_aws_cmd;
} // end aws_cmd_curr()

