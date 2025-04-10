/***********************************************************************************************************************
 * File Name    : mcuboot_logging_user.h
 * Description  : Contains function alias
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
#ifndef MCUBOOT_LOGGING_USER_H_
#define MCUBOOT_LOGGING_USER_H_
#ifndef __MCUBOOT_LOGGING_H__
#define __MCUBOOT_LOGGING_H__
#ifdef __cplusplus
extern "C" {
#endif

#if 0
#include "SEGGER_RTT/SEGGER_RTT.h"

#define MCUBOOT_LOG_ERR(fn_, ...) SEGGER_RTT_printf (0, (fn_), ##__VA_ARGS__); SEGGER_RTT_printf (0, "\r\n")
#define MCUBOOT_LOG_WRN(fn_, ...) SEGGER_RTT_printf (0, (fn_), ##__VA_ARGS__); SEGGER_RTT_printf (0, "\r\n")
#define MCUBOOT_LOG_INF(fn_, ...) SEGGER_RTT_printf (0, (fn_), ##__VA_ARGS__); SEGGER_RTT_printf (0, "\r\n")
#define MCUBOOT_LOG_DBG(fn_, ...) SEGGER_RTT_printf (0, (fn_), ##__VA_ARGS__); SEGGER_RTT_printf (0, "\r\n")
#else
#define MCUBOOT_LOG_ERR(...) IGNORE(__VA_ARGS__)
#define MCUBOOT_LOG_WRN(...) IGNORE(__VA_ARGS__)
#define MCUBOOT_LOG_INF(...) IGNORE(__VA_ARGS__)
#define MCUBOOT_LOG_DBG(...) IGNORE(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MCUBOOT_LOGGING_H__ */
#endif /* MCUBOOT_LOGGING_USER_H_ */
