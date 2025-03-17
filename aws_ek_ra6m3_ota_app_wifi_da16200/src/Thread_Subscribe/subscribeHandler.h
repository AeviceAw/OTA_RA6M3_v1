/*
 * subscribeHandler.h
 *
 *  Created on: 11 Jun 2024
 *      Author: awlir
 */

#ifndef SUBSCRIBEHANDLER_H_
#define SUBSCRIBEHANDLER_H_

#include "initialisations.h"


/* Prototype Function */
uint8_t assertDa16200_disconnected(void); // check if Rx==MQTT Disconnected
uint8_t assertDa16200_connected(void);    // check if Rx==MQTT D/C
int8_t assertDa16200_MqttMsg(char* keyword,uint8_t noOfChar); // check if MQTT Rx==<<STATUS>>>
float assertDa16200_MqttMsgF(char* keyword,uint8_t noOfChar); // check if MQTT Rx==<<STATUS>>>
int8_t assertDa16200_AudioResend(char* keyword,char* dst,size_t dst_len,bool clearAfterRead); // check if AudioResend Requests
char* AudioResendID_ptr(void);        // API to get g_idResend[11]'s pointer
void AudioResendID_set(uint8_t inVal);// API to set g_idResend[11]'s to Val
char* AudioResendSelector_ptr(void);        // API to get g_seqResend[60]'s pointer
void AudioResendSelector_set(uint8_t inVal);// API to set g_seqResend[60]'s to Val
void decodeDurState(int8_t dur);    // change Dock State based on <<STATUS>>


//typedef enum trueCheck_state{
//    AUS_NONE = 0,
//    AUS_MAN = 1,
//    AUS_CWD = 2,
//    AUS_MAN_05 = 3,
//    AUS_MAN_15 = 4,
//    AUS_RESEND = 5,
//}trueCheck_t;
//
//trueCheck_t getTrueCheckVal(void);   // get TrueCheck State
//trueCheck_t setTrueCheckVal(trueCheck_t val);// set TrueCheck State

#endif /* SUBSCRIBEHANDLER_H_ */
