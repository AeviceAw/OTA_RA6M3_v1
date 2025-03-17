/*
 * ssidpwHandler.c
 *
 *  Created on: 8 Aug 2024
 *      Author: yyang
 */

#include <Thread_SSIDPW/ssidpwHandler.h>
#include "initialisations.h"

//Global variables
uint8_t g_wifi_ssid_str[MAX_STR_SSID__PW]={0};  //holds ssid for loading into wifi_setup  //WIFISSID
uint8_t g_wifi_pw_str[MAX_STR_SSID__PW]={0};    //holds pw   for loading into wifi_setup  //WIFISSID

bool g_userssidpwstate = false;//true: Process SSID Callback
bool g_wifidownstate = false;    //true: Triggering wifi reconnection protocol
//ULONG sync_actual_flags = 0;    //Shared with uart_thread_entry.c, wifi_thread_entry.c
//static uint8_t ssidpw = 0;

/* Callback function */
void external_irq1_callback(external_irq_callback_args_t *p_args){
    FSP_PARAMETER_NOT_USED(p_args);
    setButtonPressed(true); // unnecessary - G_MQTT does the work too
    setProcessingCallback(true);
    internet_state_clear();

    // Remove this to prevent setting of UART_FLAG too early
    //wifi_event_flag_update(MQTT_FLAG_DISCONNECTED);
} // end external_irq1_callback()


/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(uint8_t)  g_wifi_ssid_str[64]>> or <<(uint8_t)  g_wifi_pw_str[64]>>
 *  @param       (wifi_param_t)  inParam
 *  @retval      &g_wifi_ssid_str[64] or &g_wifi_pw_str[64]
 **********************************************************************************************************************/
uint8_t* getWifiParamAddr(wifi_param_t inParam){
    switch (inParam){
        case WIFI_SSID:
            return (uint8_t*) g_wifi_ssid_str;
            break;
        case WIFI_PASSWORD:
            return (uint8_t*) g_wifi_pw_str;
            break;
        default:
            return 0;
            break;
    } // end switch
} // end getWifiParamAddr()

/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(uint8_t)  g_wifi_ssid_str[64]>>
 *  @param       (wifi_param_t)  inParam
 *  @retval      &g_wifi_ssid_str[64]
 **********************************************************************************************************************/
uint8_t* getWifiSSIDAddr(void){
    return (uint8_t*) g_wifi_ssid_str;
} // end getWifiSSIDAddr()

/******************************************************************************************************************//**
 *  @brief       Get the address location of  <<(uint8_t)  g_wifi_pw_str[64]>>
 *  @param       (wifi_param_t)  inParam
 *  @retval       &g_wifi_pw_str[64]
 **********************************************************************************************************************/
uint8_t* getWifiPWAddr(void){
    return (uint8_t*) g_wifi_pw_str;
} // end getWifiPWAddr()

/*******************************************************************************************************************//**
 *  @brief       get the value of buttonpressed (i.e., IRQ/Lyra GPIO 1
 * @inputs   None
 * @retval       bool         True/False button_pressed
 **********************************************************************************************************************/
bool getButtonPressed(void){
    return g_wifidownstate;
} // end getButtonPressed()

/*******************************************************************************************************************//**
 *  @brief       set the value of buttonpressed (i.e., IRQ/Lyra GPIO 1
 *  @param[in]   bool        inVal True/False button_pressed
 * @retval       None
 **********************************************************************************************************************/
void setButtonPressed(bool inVal){
    g_wifidownstate = inVal;
} // end getButtonPressed()

/*******************************************************************************************************************//**
 *  @brief       get the value of processing_callback
 * @inputs   None
 * @retval       bool         True/False button_pressed
 **********************************************************************************************************************/
bool getProcessingCallback(void){
    return g_userssidpwstate;
} // end getProcessingCallback()

/*******************************************************************************************************************//**
 *  @brief       set the value of processing_callback
 *  @param[in]   bool        inVal True/False button_pressed
 * @retval       None
 **********************************************************************************************************************/
void setProcessingCallback(bool inVal){
    g_userssidpwstate = inVal;
} // end setProcessingCallback()



