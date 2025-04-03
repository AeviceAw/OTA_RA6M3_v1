/*
 * ssidpwHandler.h
 *
 *  Created on: 8 Aug 2024
 *      Author: awlir
 */

#ifndef SSIDPWHANDLER_H_
#define SSIDPWHANDLER_H_

#include "initialisations.h"


#define MAX_STR_SSID__PW                64

/*************************************************************
 * State Variables for SSID/PW
 ************************************************************/
typedef enum wifi_param{
    WIFI_SSID=0,
    WIFI_PASSWORD=1,
    WIFI_CHANNEL=3,
//    WIFI_SECURITY=4
}wifi_param_t;

uint8_t* getWifiSSIDAddr(void);
uint8_t* getWifiPWAddr(void);
uint8_t* getWifiParamAddr(wifi_param_t inParam);
bool getButtonPressed(void);
void setButtonPressed(bool inVal);
void setProcessingCallback(bool inVal);
bool getProcessingCallback(void);

#endif /* SSIDPWHANDLER_H_ */
