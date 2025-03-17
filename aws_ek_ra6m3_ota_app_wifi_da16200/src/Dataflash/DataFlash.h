/***********************************************************************************************************************
 * File Name    : flash_hp_ep.h
 * Description  : Contains macros, data structures and functions used in flash_hp_ep.h
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef DATAFLASH_H_
#define DATAFLASH_H_
/*******************************************************************************************************************//**
 * @ingroup FLASH_HP_EP
 * @{
 **********************************************************************************************************************/

#include "initialisations.h"
//#include "stdint.h"
//#include "hal_data.h"
//#include "AeviceSystemConfig.h"

#define DF_DEBUG_PRINT                    0
#define TEST_DF                           0

#define FLASH_HP_DF_BLOCK_SIZE            (64)

#define FLASH_HP_DF_BLOCK_0               (0x40100000U) /*   64 B:  0x40100000 - 0x4010003F */
#define FLASH_HP_DF_BLOCK_1               (0x40100040U) /*   64 B:  0x40100040 - 0x4010007F */

#define FLASH_HP_DF_SSID_BLOCK            FLASH_HP_DF_BLOCK_0
#define FLASH_HP_DF_PW_BLOCK              FLASH_HP_DF_BLOCK_1

#define BLOCK_NUM						  (1)//SSID and PW size up to 64 bytes

/* Printing read data */
#define READ_DATA_PRINT(buff)		({for(uint8_t index = RESET_VALUE; BLOCK_SIZE > index; index++)	\
        {													\
    APP_PRINT("%x ",*(read_buffer+index));						\
    if((index + 1) % 8 == 0)							\
    {	APP_PRINT("\r\n");	}						\
        }})
/*flash_hp operating functions */

void Print_DF_SSID_Str(void);

void Print_DF_PW_Str(void);

void Test_DF_SSID_PW(void);

void DF_Init(void);

void DF_Read_SSID_Str(char *out_readStr);

void DF_Read_PW_Str(char *out_readStr);

void DF_Read(uint32_t df_block, char *out_readStr);

fsp_err_t DF_Write_SSID(char *in_ssid_str);

fsp_err_t DF_Write_PW(char *in_pw_Str);

fsp_err_t DF_Write(uint32_t df_block, char *in_writeStr);

/*******************************************************************************************************************//**
 * @brief       This functions de-initializes Flash_HP module.
 **********************************************************************************************************************/
//void flash_hp_deinit(void);

#endif /* DATAFLASH_H_ */
