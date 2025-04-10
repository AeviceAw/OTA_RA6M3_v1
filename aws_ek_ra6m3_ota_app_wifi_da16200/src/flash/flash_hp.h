/***********************************************************************************************************************
 * File Name    : flash_hp.h
 * Description  : Contains macros, data structures and functions used in flash_hp.h
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
#include "common_utils.h"

#ifndef FLASH_HP_EP_H_
#define FLASH_HP_EP_H_

typedef enum e_credentials
{
    CERTIFICATE = 0,
    PRIVATE_KEY,
	MQTT_ENDPOINT,
	IOT_THING_NAME,
	CODE_SIGN_PKEY,
}credentials_t;

/* Declare stored status string and its length to write into the data flash when the credential stored */
#define STRING_SAVE                    "SAVED"
#define LENGTH_SAVE                    (5)

typedef struct s_credentials_mem_map
{
	uint32_t num_bytes;
	uint32_t addr;
	uint8_t num_block;
	char stored_in_flash[LENGTH_SAVE];
	uint32_t length;
}credentials_mem_map_t;

typedef struct s_credentials_stored_info
{
    char stored_in_flash[LENGTH_SAVE];
	uint32_t num_bytes;
}credentials_stored_info_t;

#define FLASH_HP_DF_BLOCK_SIZE            (64)

/* Data Flash */
#define DATA_FLASH_BASE_ADDR              (BSP_FEATURE_FLASH_DATA_FLASH_START)
#define FLASH_HP_DF_BLOCK_0               (DATA_FLASH_BASE_ADDR + 0x1000U) /*   64 B:    0x1000 - 0x103F */
#define FLASH_HP_DF_BLOCK_CERTIFICATE     (DATA_FLASH_BASE_ADDR + 0x1040U) /*   1536 B:  0x1040 - 0x163F */
#define FLASH_HP_DF_BLOCK_KEY             (DATA_FLASH_BASE_ADDR + 0x1640U) /*   1792 B:  0x1640 - 0x1D3F */
#define FLASH_HP_DF_BLOCK_CODE_SIGN_PKEY  (DATA_FLASH_BASE_ADDR + 0x1D40U) /*   256 B:   0x1D40 - 0x1E3F */
#define FLASH_HP_DF_MQTT_END_POINT        (DATA_FLASH_BASE_ADDR + 0x1E40U) /*   128 B:   0x1E40 - 0x1EBF */
#define FLASH_HP_DF_IOT_THING_NAME        (DATA_FLASH_BASE_ADDR + 0x1EC0U) /*   128 B:   0x1EC0 - 0x1F3F */
#define FLASH_HP_DF_NETWORK_IF_INFO       (DATA_FLASH_BASE_ADDR + 0x1F40U) /*   64 B:    0x1F40 - 0x1F7F */
#define FLASH_HP_DF_DATA_INFO             (DATA_FLASH_BASE_ADDR + 0x1F80U) /*   128 B:   0x1F80 - 0x1FFF */


#define TOTAL_BLOCK_SIZE                  (4032)
#define TOTAL_BLOCK_NUM					  (63)

#define BLOCK_SIZE_CERT                   (1536)
#define BLOCK_NUM_CERT			          (24)

#define BLOCK_SIZE_KEY                    (1792)
#define BLOCK_NUM_KEY			          (28)

#define BLOCK_SIZE_CODE_SIGN_PKEY         (256)
#define BLOCK_NUM_CODE_SIGN_PKEY		  (4)

#define BLOCK_SIZE_MQTT_ENDPOINT          (128)
#define BLOCK_NUM_MQTT_ENDPOINT	          (2)

#define BLOCK_SIZE_IOT_THING              (128)
#define BLOCK_NUM_IOT_THING		          (2)

#define BLOCK_SIZE_NETWORK_IF_INFO        (64)
#define BLOCK_NUM_NETWORK_IF_INFO         (1)

#define BLOCK_SIZE_DATA_INFO              (128)
#define BLOCK_NUM_DATA_INFO		          (2)
//
#define BUFFER_SIZE                       (2048)

#define CREDENTIAL_COUNT                  (6)

#define MODULE_NAME_FLASH_INFO         "\r\nDATA FLASH INFO\r\n"
#define FLASH_WRITE_CERT_MENU          "\r\nDATA FLASH WRITE CERTIFICATE\r\n"
#define FLASH_WRITE_KEY_MENU           "\r\nDATA FLASH WRITE PRIVATE KEY\r\n"
#define FLASH_WRITE_THING_NAME          "\r\nDATA FLASH WRITE THING NAME\r\n"
#define FLASH_WRITE_MQTT_END_POINT     "\r\nDATA FLASH WRITE MQTT END POINT\r\n"
#define FLASH_WRITE_CODE_SIGN_PKEY     "\r\nDATA FLASH WRITE CODE SIGN PUBLIC KEY\r\n"
#define FLASH_WRITE_NETWORK_IF_INFO    "\r\nNETWORK INTERFACE INFO\r\n"
#define FLASH_CHECK_CREDENTIALS        "\r\nCHECK CREDENTIALS STORED IN DATA FLASH\r\n"
#define OPTIONS_SELECT                 "\r\n> Select from the options in the menu below:\r\n"
#define SELECT_FILE                    "\r\n Select the file to write data in data flash \r\n"
#define TYPE_CREDENTIALS               "\r\n Type or Paste credential and press enter to save credentials in flash\r\n"
#define TYPE_NETWORK_IF_INFO           "\r\n Type or Paste network interface info and press enter to save it in flash\r\n"
#define TYPE_NETWORK_IF_INFO_ETHER     " For Ethernet, type MAC address. Format: XX:XX:XX:XX:XX:XX (XX is hex value (00~FF))\r\n"
#define TYPE_NETWORK_IF_INFO_WIFI      " For Wi-Fi, type SSID and password. Format: <SSID> + <a space> + <password>\r\n"
#define MODULE_NAME_FLASH_READ         "\r\nDATA FLASH READ\r\n"
#define MODULE_NAME_FLASH_HELP         "\r\nHELP\r\n\r\n"
#define FLASH_HELP_TAG1                "  Please type a for flash memory information\r\n"
#define FLASH_HELP_TAG2                "  Please type b to save AWS certificate\r\n"
#define FLASH_HELP_TAG3                "  Please type c to save AWS private key\r\n"
#define FLASH_HELP_TAG4                "  Please type d to save MQTT broker end point\r\n"
#define FLASH_HELP_TAG5                "  Please type e to save IOT thing name\r\n"
#define FLASH_HELP_TAG6                "  Please type f to save Code signing public key (for OTA)\r\n"
#define FLASH_HELP_TAG10               "  Please type x to save Network information\r\n"
#define FLASH_HELP_TAG7                "  Please type g to read data saved in flash\r\n"
#define FLASH_HELP_TAG8                "  Please type h to check and validate credentials saved\r\n"
#define END_OF_AWS_CERTIFICATE         "-----END CERTIFICATE-----"
#define END_OF_AWS_PRIVATE_KEY         "-----END RSA PRIVATE KEY-----"
#define END_OF_AWS_CODE_SIGN_PKEY	   "-----END PUBLIC KEY-----"
#define CERTIFICATE                    (0)
#define PRIVATE_KEY                    (1)
#define MQTT_ENDPOINT                  (2)
#define IOT_THING_NAME                 (3)
#define CODE_SIGN_PKEY                 (4)
#define NETWORK_IF_INFO                (5)
#define FLASH_DATA_INFO                (6)

/* Switch Commands */
#define CODE_FLASH                     (1U)
#define DATA_FLASH                     (2U)
#define EXIT						   (3U)
#define BUFF_SIZE					   (0x0F)
#define BUFF_INDEX					   (0x00)

/*flash_hp operating functions */
fsp_err_t aws_certficate_write(uint8_t cert_type);
fsp_err_t flash_data_write(void);
fsp_err_t store_flashed_data_info (uint8_t cert_type);
fsp_err_t flash_hp_data_read(bool print_data);
void flash_hp_deinit(void);
void flash_display_menu(uint8_t credential_type);
void flash_info(void);
void flash_memory_mapping(void);
fsp_err_t flash_mem_init(void);
fsp_err_t flash_stored_data_info (void);
fsp_err_t check_credentials_stored (void);
void help_menu(void);
/*******************************************************************************************************************//**
 * @} (end defgroup FLASH_HP_EP)
 **********************************************************************************************************************/

#endif /* FLASH_HP_H_ */
