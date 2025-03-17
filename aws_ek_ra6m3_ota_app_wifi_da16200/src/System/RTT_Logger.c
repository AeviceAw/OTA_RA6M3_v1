/*
 * RTT_Logger.c
 *
 *  Created on: 30 Apr 2024
 *      Author: WilsonQuah
 */

//#include <SystemFunctions/RTC_Timer/RTC_Timer.h>
//#include <SystemFunctions/RTT_Logger/RTT_Logger.h>
#include "initialisations.h"

char debugStr[128];

void Init_RTT(void) {
    RTT_LOG("RTT LOGGER Initialised");
}

char* RTT_Str_float(char* textStr, float inFloat) {
    snprintf(debugStr, sizeof(debugStr), "%s: %f", textStr, inFloat);
    return debugStr;
}

char* RTT_Str_bool(char* textStr, bool inBool) {
    snprintf(debugStr, sizeof(debugStr), "%s: %d", textStr, inBool);
    return debugStr;
}

char* RTT_Str_signedVal(char* textStr, int32_t inSignedVal) {
    snprintf(debugStr, sizeof(debugStr), "%s: %ld", textStr, inSignedVal);
    return debugStr;
}

char* RTT_Str_unsignedVal(char* textStr, uint32_t inSignedVal) {
    snprintf(debugStr, sizeof(debugStr), "%s: %lu", textStr, inSignedVal);
    return debugStr;
}


char* RTT_Str_unsignedHex(char* textStr, uint32_t inSignedVal) {
    snprintf(debugStr, sizeof(debugStr), "%s: 0x%lX", textStr, inSignedVal);
    return debugStr;
}

void RTT_LOG(char *inStr) {
    char logStr[256];
    uint16_t hours   = 0;
    uint16_t minutes = 0;
    uint16_t seconds = 0;
    hours = RTC_Get_Time() / 3600;
    uint16_t total_seconds = RTC_Get_Time() % 3600;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;

    snprintf(logStr, sizeof(logStr) - 1 - 4, "%02d:%02d:%02d: %s\r\n", hours, minutes, seconds, inStr);
    //snprintf(logStr, sizeof(logStr) - 1 - 4, "%06d: %s\r\n", RTC_Get_Time(), inStr);
    APP_PRINT(logStr);
}

void RTT_ERROR_LOG(char *inStr) {
    char logStr[128];
    uint16_t hours   = 0;
    uint16_t minutes = 0;
    uint16_t seconds = 0;
    hours = RTC_Get_Time() / 3600;
    uint16_t total_seconds = RTC_Get_Time() % 3600;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;

    snprintf(logStr, sizeof(logStr) - 1 - 4, "%02d:%02d:%02d: %s\r\n", hours, minutes, seconds, inStr);
    //snprintf(logStr, sizeof(logStr) - 1 - 4, "%06d: %s\r\n", RTC_Get_Time(), inStr);
    APP_ERR_PRINT(logStr);
}

char g_rtc_timestamp[256];
char* RTT_TIMESTAMP(void){
    memset(g_rtc_timestamp,0,sizeof(g_rtc_timestamp));
    uint16_t hours   = 0;
    uint16_t minutes = 0;
    uint16_t seconds = 0;
    hours = RTC_Get_Time() / 3600;
    uint16_t total_seconds = RTC_Get_Time() % 3600;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;
    snprintf(g_rtc_timestamp, sizeof(g_rtc_timestamp) - 1 - 4, "%02d:%02d:%02d:", hours, minutes, seconds);

    return (char*) &g_rtc_timestamp;
} // end RTT_TIMESTAMP()

