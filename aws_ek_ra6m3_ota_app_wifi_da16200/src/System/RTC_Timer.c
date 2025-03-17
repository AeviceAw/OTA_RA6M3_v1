/***********************************************************************************************************************
 * File Name    : rtc_ep.c
 * Description  : Contains data structures and functions used in rtc_ep.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *
 * This file is part of Renesas RA Flex Software Package (FSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas FSP license agreement. Unless otherwise agreed in an FSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

//#include "common_utils.h"
//#include "hal_data.h"
//#include "RTC_Timer.h"
//#include "RTT_Logger.h"
#include "initialisations.h"

static uint16_t sysTimer_count = 0;

/*******************************************************************************************************************//**
 * @addtogroup rtc_ep
 * @{
 **********************************************************************************************************************/

/* Variable to store time */
static rtc_time_t g_set_time =  {
    .tm_hour    =  RTC_RESET_VALUE,
    .tm_isdst   =  RTC_RESET_VALUE,
    .tm_mday    =  RTC_RESET_VALUE,
    .tm_min     =  RTC_RESET_VALUE,
    .tm_mon     =  RTC_RESET_VALUE,
    .tm_sec     =  RTC_RESET_VALUE,
    .tm_wday    =  RTC_RESET_VALUE,
    .tm_yday    =  RTC_RESET_VALUE,
    .tm_year    =  RTC_RESET_VALUE,
   };

static rtc_time_t g_present_time =  {
 .tm_hour    =  RTC_RESET_VALUE,
 .tm_isdst   =  RTC_RESET_VALUE,
 .tm_mday    =  RTC_RESET_VALUE,
 .tm_min     =  RTC_RESET_VALUE,
 .tm_mon     =  RTC_RESET_VALUE,
 .tm_sec     =  RTC_RESET_VALUE,
 .tm_wday    =  RTC_RESET_VALUE,
 .tm_yday    =  RTC_RESET_VALUE,
 .tm_year    =  RTC_RESET_VALUE,
                                    };
static uint32_t set_time_flag = RTC_RESET_FLAG;  //Flag to be set on successful RTC Calendar setting
static void rtc_date_time_conversion(rtc_time_t * time, unsigned char read_buffer[]);

void rtc_callback(rtc_callback_args_t *p_args) {
    if(RTC_EVENT_PERIODIC_IRQ == p_args->event) {
        sysTimer_count++;
//        char debugStr[128];
//        snprintf(debugStr, sizeof(debugStr) - 1, "rtc_callback: RTC count: %d\n", rtc_count);
//        RTT_LOG(debugStr);
    }
}

uint16_t RTC_Get_Time(void) {
    return sysTimer_count;
}

void RTC_Init(void) {
    fsp_err_t err = FSP_SUCCESS;
    err = R_RTC_Open(&g_rtc_ctrl, &g_rtc_cfg);
    if (FSP_SUCCESS != err) {
        RTT_LOG("RTC module open failed.");
        RTT_LOG("Restart the Application");
        APP_ERR_TRAP(err);
    }
    //Set RTC Calendar Time
    RTC_set_rtc_calendar_time();

    err = RTC_set_rtc_periodic_rate();
    if (err != FSP_SUCCESS) {
        RTC_Deinit();
        RTT_LOG("Periodic interrupt rate setting failed.");
        RTT_LOG("Closing the driver. Restart the Application");
        APP_ERR_TRAP(err);
    }
    //else RTT_LOG("[RTC]: RTC mode: Periodic INT");
    RTT_LOG("\r\n[RTC]: RTC init: SUCCESS");
}

//==========================================================================
fsp_err_t RTC_set_rtc_calendar_time(void) {
    fsp_err_t err = FSP_SUCCESS;

    //Does not matter time, only using counter
    uint8_t read_time[RTC_BUFFER_SIZE_DOWN] = {};
    snprintf((char*)read_time, sizeof(read_time), "01:01:2000 00:00:00");

    /* Modify user provided date and time to standard format */
    rtc_date_time_conversion(&g_set_time, &read_time[0]);

    /* Set the time provided by user */
    err = R_RTC_CalendarTimeSet(&g_rtc_ctrl, &g_set_time);
    if(err != FSP_SUCCESS) {
        RTT_LOG("CalendarTime Set failed.");
        return err;
    }

    /* Get the current Calendar time */
    err = R_RTC_CalendarTimeGet(&g_rtc_ctrl, &g_present_time);
    if(err != FSP_SUCCESS) {
        RTT_LOG("CalendarTime Get failed.");
        return err;
    }

    set_time_flag = RTC_SET_FLAG;
    return err;
}

//==========================================================================
fsp_err_t RTC_set_rtc_periodic_rate(void) {
    fsp_err_t err = FSP_SUCCESS;
    //RTT_LOG("[RTC]: Setting Periodic IRQ rate for RTC");

    /* Enable periodic interrupt rate */
    err = R_RTC_PeriodicIrqRateSet(&g_rtc_ctrl, RTC_PERIODIC_IRQ_SELECT_1_SECOND);

    if(err != FSP_SUCCESS) {
        RTT_LOG("Periodic interrupt rate setting failed.");
    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This functions modifies the user input to rtc date and time.
 * @param[IN]   time                    date and time to be modified
 * @param[IN]   read_buffer             Buffer that contains user input time values
 * @retval      None
 **********************************************************************************************************************/
static void rtc_date_time_conversion(rtc_time_t * time, unsigned char read_buffer[])
{
    /* User is expected to enter Date and Time in DD:MM:YYYY HH:MM:SS format. This User input in the buffer is processed
     * using the following conversion, where ASCII value is converted to integer value and stored in the RTC time structure.
     * According to DD:MM:YYYY HH:MM:SS format, different array index values are used.  */
    time->tm_mday = (((read_buffer[0] - ASCII_ZERO) * PLACE_VALUE_TEN) + (read_buffer[1] - ASCII_ZERO));
    time->tm_mon = ((((read_buffer[3] - ASCII_ZERO) * PLACE_VALUE_TEN) + (read_buffer[4] - ASCII_ZERO))) - MON_ADJUST_VALUE;
    time->tm_year = (((read_buffer[6] - ASCII_ZERO) * PLACE_VALUE_THOUSAND) +
            ((read_buffer[7] - ASCII_ZERO )* PLACE_VALUE_HUNDRED) +
            ((read_buffer[8] - ASCII_ZERO) * PLACE_VALUE_TEN) + (read_buffer[9] - ASCII_ZERO)) - YEAR_ADJUST_VALUE;
    time->tm_hour = (((read_buffer[11] - ASCII_ZERO) * PLACE_VALUE_TEN) + (read_buffer[12] - ASCII_ZERO));
    time->tm_min = (((read_buffer[14] - ASCII_ZERO) * PLACE_VALUE_TEN) + (read_buffer[15] - ASCII_ZERO));
    time->tm_sec = (((read_buffer[17] - ASCII_ZERO) * PLACE_VALUE_TEN )+ (read_buffer[18] - ASCII_ZERO));
}

void RTC_Deinit(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Close RTC module */
    err = R_RTC_Close(&g_rtc_ctrl);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        /* RTC Close failure message */
        RTT_LOG("** RTC module Close failed **");
    }
}

/*******************************************************************************************************************//**
 * @} (end addtogroup rtc_ep)
 **********************************************************************************************************************/

