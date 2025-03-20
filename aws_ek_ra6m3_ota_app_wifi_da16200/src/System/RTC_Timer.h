/***********************************************************************************************************************
 * File Name    : rtc_ep.h
 * Description  : Contains declarations of data structures and functions used in rtc_ep.c.
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

#ifndef RTC_EP_H_
#define RTC_EP_H_

#include "stdint.h"
#include "hal_data.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* MACRO to flag the status */
#define RTC_BUFFER_SIZE_DOWN                 (32)
#define RTC_RESET_VALUE                      (0x00)

#define RTC_SET_FLAG                         (0x01)
#define RTC_RESET_FLAG                       (0x00)

/* MACRO for ASCII value of zero */
#define ASCII_ZERO       (48)
/* MACRO for null character */
#define NULL_CHAR   ('\0')

/* MACRO for checking if no byte is received */
#define BYTES_RECEIVED_ZERO  (0U)

/*MACROs to adjust month and year values */
#define MON_ADJUST_VALUE      (1)
#define YEAR_ADJUST_VALUE     (1900)

/* MACROs for RTT input processing */
#define PLACE_VALUE_TEN           (10)
#define PLACE_VALUE_HUNDRED       (100)
#define PLACE_VALUE_THOUSAND      (1000)

/***********************************************************************************************************************
 * User-defined APIs
 **********************************************************************************************************************/

/*
 * function declarations
 */

uint16_t RTC_Get_Time(void);

/*******************************************************************************************************************//**
 * @brief       This functions initializes RTC module.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
void RTC_Init(void);


/*******************************************************************************************************************//**
 * @brief This function closes opened RTC module before the project ends up in an Error Trap.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
void RTC_Deinit(void);

/*******************************************************************************************************************//**
 * @brief       This functions sets the Calendar time provided by user.
 * @param[IN]   None
 * @retval      FSP_SUCCESS                  Upon successful Calendar time set
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful Calendar time set
 **********************************************************************************************************************/
fsp_err_t RTC_set_rtc_calendar_time(void);

/*******************************************************************************************************************//**
 * @brief       This functions sets the periodic interrupt rate provided by user.
 * @param[IN]   None
 * @retval      FSP_SUCCESS                  Upon successful periodic interrupt rate set
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful periodic interrupt rate set
 **********************************************************************************************************************/
fsp_err_t RTC_set_rtc_periodic_rate(void);



#endif /* RTC_EP_H_ */
