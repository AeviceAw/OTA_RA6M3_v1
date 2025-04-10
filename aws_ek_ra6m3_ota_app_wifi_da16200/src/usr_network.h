/***********************************************************************************************************************
 * File Name    : usr_network.h
 * Description  : Contains macros, data structures and functions used in the Application
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
#ifndef USR_NETWORK_H_
#define USR_NETWORK_H_

#include "hal_data.h"

//################### USER CONFIGURATION ###################
#define AWSIOT_NETWORK_TYPE_NONE        0x00000000
#define AWSIOT_NETWORK_TYPE_WIFI        0x00000001
#define AWSIOT_NETWORK_TYPE_ETH         0x00000004
#define AWSIOT_ENTWORK_TYPE_USB_CDC_ECM 0x00000008
#define configENABLED_NETWORKS          (AWSIOT_NETWORK_TYPE_WIFI)

#define NETWORK_SETUP_USE_FIXED_INFO    (0U) // 0: Not used, 1: Used

#if (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_ETH)
#if (NETWORK_SETUP_USE_FIXED_INFO == 1U)
#define ETHERNET_MAC_ADDRESS            "00:11:22:33:44:54"
#endif
#elif (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_WIFI)
#if (NETWORK_SETUP_USE_FIXED_INFO == 1U)
#define WIFI_SSID                       "testtest"
#define WIFI_PASSWORD                   "testtest"
#endif
#define WIFI_SECURITY                   eWiFiSecurityWPA2
#endif

#include "common_utils.h"
#define NETWORK_SETUP_APP_PRINT(fn_, ...)   APP_PRINT((fn_), ##__VA_ARGS__)
//############### END OF USER CONFIGURATION ################

#if (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_WIFI)
#define NETWORK_WIFI_MODULE_TYPE_DA16XXX  0x00000001
#define NETWORK_WIFI_MODULE_TYPE_SILEX    0x00000004
#if defined(RM_WIFI_DA16XXX_CFG_H_)
#define NETWORK_WIFI_MODULE_TYPE          (NETWORK_WIFI_MODULE_TYPE_DA16XXX)
#elif defined(RM_WIFI_ONCHIP_SILEX_CFG_H_)
#define NETWORK_WIFI_MODULE_TYPE          (NETWORK_WIFI_MODULE_TYPE_SILEX)
#endif
#endif

#if (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_ETH)
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_Sockets.h"
#if( ipconfigUSE_DHCP != 0 )
#include "FreeRTOS_DHCP.h"
#endif

#define USR_TEST_PING_IP "8.8.8.8"
#define USR_PING_COUNT (100)

#define ETHERNET_LINK_DOWN (0x01)
#define ETHERNET_LINK_UP (0x00)
#define IP_LINK_DOWN (0x02)
#define IP_LINK_UP (0x00)

#if( ipconfigUSE_DHCP != 0 )
void updateDhcpResponseToUsr(void);
#endif
#elif (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_WIFI)
#define BUFF_LEN                    (32U)
#define IP_V4                       (4U)

typedef struct
{
    char ssid[BUFF_LEN];
    char pwd[BUFF_LEN];
    WIFISecurity_t security;
    uint8_t device_ip_addr[IP_V4];
    fsp_err_t status;
} wifi_param_t;
#endif

typedef struct st_ping_data
{
    uint32_t sent;     // Ping Request
    uint32_t received; // Ping Response
    uint32_t lost;     // Ping failure
} ping_data_t;

fsp_err_t network_interface_setup(void);

#endif /* USR_NETWORK_H_ */
