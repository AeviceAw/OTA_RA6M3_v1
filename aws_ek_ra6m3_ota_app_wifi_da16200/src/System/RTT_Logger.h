/*
 * RTT_Logger.h
 *
 *  Created on: 30 Apr 2024
 *      Author: WilsonQuah
 */

#ifndef RTT_LOGGER_RTT_LOGGER_H_
#define RTT_LOGGER_RTT_LOGGER_H_

#include "common_utils.h"

void Init_RTT(void);

char* RTT_Str_float(char* textStr, float inFloat);

char* RTT_Str_bool(char* textStr, bool inBool);

char* RTT_Str_signedVal(char* textStr, int32_t inSignedVal);

char* RTT_Str_unsignedVal(char* textStr, uint32_t inSignedVal);

char* RTT_Str_unsignedHex(char* textStr, uint32_t inSignedVal);

void RTT_LOG(char *inStr);

void RTT_ERROR_LOG(char *inStr);

char* RTT_TIMESTAMP(void);

#endif /* RTT_LOGGER_RTT_LOGGER_H_ */
