/***********************************************************************************************************************
 * File Name    : flash_hp_ep.c
 * Description  : Contains macros, data structures and functions used in flash_hp_ep.c
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

#include "initialisations.h"
#include "Dataflash/DataFlash.h"
#define DATAFLASH_MODULE 1


#if DATAFLASH_MODULE

/* Flags, set from Callback function */
static volatile _Bool g_b_flash_event_not_blank = false;
static volatile _Bool g_b_flash_event_blank = false;
static volatile _Bool g_b_flash_event_erase_complete = false;
static volatile _Bool g_b_flash_event_write_complete = false;

static char df_ssid_buffer[FLASH_HP_DF_BLOCK_SIZE] = {0};
static char df_pw_buffer[FLASH_HP_DF_BLOCK_SIZE]   = {0};

/*******************************************************************************************************************//**
 * @brief This function is called to set the blank check event flags
 * @param[IN]   None
 * @retval      FSP_SUCCESS             Upon successful Flash_HP is blank
 * @retval      Any Other Error code    Upon unsuccessful Flash_HP is not blank
 **********************************************************************************************************************/
static fsp_err_t blankcheck_event_flag(void);

void Print_DF_SSID_Str(void) {
#if DF_DEBUG_PRINT
	char debugStr[128];
    char tempStr[FLASH_HP_DF_BLOCK_SIZE + 1];
    memset(tempStr, 0x00, sizeof(tempStr));
    memcpy(tempStr, df_ssid_buffer, sizeof(df_ssid_buffer));
    snprintf(debugStr, sizeof(debugStr) - 1, "[DF]: SSID: 0x%x : %s", FLASH_HP_DF_SSID_BLOCK, tempStr);
    ////RTT_LOG(debugStr);
#endif
}

void Print_DF_PW_Str(void) {
#if DF_DEBUG_PRINT
    char debugStr[128];
    char tempStr[FLASH_HP_DF_BLOCK_SIZE + 1];
    memset(tempStr, 0x00, sizeof(tempStr));
    memcpy(tempStr, df_pw_buffer, sizeof(df_pw_buffer));
    snprintf(debugStr, sizeof(debugStr) - 1, "[DF]:   PW: 0x%x : %s", FLASH_HP_DF_PW_BLOCK, tempStr);
    ////RTT_LOG(debugStr);
#endif
}

void Test_DF_SSID_PW(void) {
    ////RTT_LOG("========================================");
    for(uint8_t i = 0; i < 3; i++) {
        char test_id_str[FLASH_HP_DF_BLOCK_SIZE];
        for (int ii = 0; ii < FLASH_HP_DF_BLOCK_SIZE; ii++) {
            test_id_str[ii] = (char)(i + 'A' + (ii % 26));  // Incrementing
        }

        ////RTT_LOG("-----------------------------");
        fsp_err_t err = FSP_ERR_ASSERTION;
        err = DF_Write_SSID(test_id_str);
        if (FSP_SUCCESS != err) {
        	////RTT_LOG("[DF]: FAILED\n");
            flash_hp_deinit();
            APP_ERR_TRAP(err);
        }
        //else ////RTT_LOG("DF: Write SUCCESS\n");
        memset(df_ssid_buffer, 0x00, sizeof(df_ssid_buffer));
        DF_Read_SSID_Str(df_ssid_buffer);
        Print_DF_SSID_Str();


        //======================================================================================
        char test_pw_str[FLASH_HP_DF_BLOCK_SIZE];
        for (int ii = 0; ii < FLASH_HP_DF_BLOCK_SIZE; ii++) {
            test_pw_str[ii] = (char)(i + '0' + (ii % 10));  // Incrementing
        }
        ////RTT_LOG("");
        err = DF_Write_PW(test_pw_str);
        if (FSP_SUCCESS != err) {
            ////RTT_LOG("DF: FAILED\n");
            flash_hp_deinit();
            APP_ERR_TRAP(err);
        }
        //else ////RTT_LOG("DF: Write SUCCESS\n");
        memset(df_pw_buffer, 0x00, sizeof(df_pw_buffer));
        DF_Read_PW_Str(df_pw_buffer);
        Print_DF_PW_Str();

        vTaskDelay(1000); // Sleep for 1-sec
        ////RTT_LOG("-----------------------------");
    }
    ////RTT_LOG("========================================");
}

void DF_Init(void) {
    fsp_err_t err = R_FLASH_HP_Open(&g_flash_ctrl, &g_flash_cfg);
    if (FSP_SUCCESS != err) {
        ////RTT_LOG("[DF]: ERR: R_FLASH_HP_Open(): FAILED");
        APP_ERR_TRAP(err);
    }
    else {
        ////RTT_LOG("[DF]: Data Flash Module init: SUCCESS");
    }
}

void DF_Read_SSID_Str(char *out_readStr) {
    DF_Read(FLASH_HP_DF_BLOCK_0, out_readStr);
}

void DF_Read_PW_Str(char *out_readStr) {
    DF_Read(FLASH_HP_DF_BLOCK_1, out_readStr);
}

void DF_Read(uint32_t df_block, char *out_readStr) {
    memcpy(out_readStr, (uint8_t *) df_block, FLASH_HP_DF_BLOCK_SIZE);
}

fsp_err_t DF_Write_SSID(char *in_ssid_str) {
    fsp_err_t err = DF_Write(FLASH_HP_DF_BLOCK_0, in_ssid_str);
    return err;
}

fsp_err_t DF_Write_PW(char *in_pw_Str) {
    fsp_err_t err = DF_Write(FLASH_HP_DF_BLOCK_1, in_pw_Str);
    return err;
}

fsp_err_t DF_Write(uint32_t df_block, char *in_writeStr) {
    fsp_err_t err = FSP_ERR_ASSERTION;
    flash_result_t blank_check_result = FLASH_RESULT_BLANK;
    char write_buffer[FLASH_HP_DF_BLOCK_SIZE] = {0};
    char read_buffer[FLASH_HP_DF_BLOCK_SIZE]  = {0};

    memset(write_buffer, 0x00, sizeof(write_buffer));
    memcpy(write_buffer, in_writeStr, sizeof(write_buffer));

    err = R_FLASH_HP_Erase(&g_flash_ctrl, df_block, BLOCK_NUM);
    if (FSP_SUCCESS != err) {
        ////RTT_LOG("[DF]: ERR: R_FLASH_HP_Erase(): FAILED");
        return err;
    }

    /* Wait for the erase complete event flag, if BGO is SET  */
    if (true == g_flash_cfg.data_flash_bgo) {
        //////RTT_LOG("\r\n BGO has enabled");
        while (!g_b_flash_event_erase_complete);
        g_b_flash_event_erase_complete = false;
    }
    //////RTT_LOG("\r\nErase successful");

    err = R_FLASH_HP_BlankCheck(&g_flash_ctrl, df_block,FLASH_HP_DF_BLOCK_SIZE, &blank_check_result);
    if (FSP_SUCCESS != err) {
        ////RTT_LOG("[DF]: ERR: R_FLASH_HP_BlankCheck(): FAILED");
        return err;
    }

    /* Validate the blank check result */
    if (FLASH_RESULT_BLANK == blank_check_result) {
        //////RTT_LOG("\r\n BlankCheck is successful");
    }
    else if (FLASH_RESULT_NOT_BLANK == blank_check_result) {
        ////RTT_LOG("[DF]: ERR: R_FLASH_HP_BlankCheck(): is NOT blank, unable to write data");
        return (fsp_err_t)FLASH_RESULT_NOT_BLANK;
    }
    else if (FLASH_RESULT_BGO_ACTIVE == blank_check_result) {
        /* BlankCheck will update in Callback */
        /* Event flag will be updated in the blank check function when BGO is enabled */
        err = blankcheck_event_flag();
        if(FSP_SUCCESS != err)
        {
            return err;
        }
    }

    err = R_FLASH_HP_Write(&g_flash_ctrl, (uint32_t) write_buffer,df_block, FLASH_HP_DF_BLOCK_SIZE);
    if (FSP_SUCCESS != err) {
        ////RTT_LOG("[DF]: ERR: R_FLASH_HP_Write(): FAILED");
        return err;
    }

    if (true == g_flash_cfg.data_flash_bgo) {
        while (!g_b_flash_event_write_complete);
        g_b_flash_event_write_complete = false;
    }

#if DF_DEBUG_PRINT
    char debugStr[128];
    snprintf(debugStr, sizeof(debugStr) - 1, "[DF]: write 0x%lX : %s", df_block, write_buffer);
    ////RTT_LOG(debugStr);
#endif

    //Read from buffer and validate
    DF_Read(df_block, read_buffer);

    if (0 != memcmp(read_buffer, write_buffer, FLASH_HP_DF_BLOCK_SIZE)) {
        ////RTT_LOG("[DF]: ERR: Data Flash Read-Write data verified but NOT SUCCESSFUL");
        return FSP_ERR_WRITE_FAILED;
    }

    return err;
}

//void flash_hp_deinit(void) {
//    fsp_err_t err = FSP_ERR_ASSERTION;
//    err = R_FLASH_HP_Close(&g_flash_ctrl);
//	if (FSP_SUCCESS != err) {
//        ////RTT_LOG("[DF]: ERR: R_FLASH_HP_Close(): FAILED");
//	}
//}

/*******************************************************************************************************************//**
 * @brief Callback function for FLASH HP HAL
 **********************************************************************************************************************/
void bgo_callback(flash_callback_args_t *p_args) {
	if (FLASH_EVENT_NOT_BLANK == p_args->event) {
		g_b_flash_event_not_blank = true;
	}

	else if (FLASH_EVENT_BLANK == p_args->event) {
		g_b_flash_event_blank = true;
	}

	else if (FLASH_EVENT_ERASE_COMPLETE == p_args->event) {
		g_b_flash_event_erase_complete = true;
	}

	else if (FLASH_EVENT_WRITE_COMPLETE == p_args->event) {
		g_b_flash_event_write_complete = true;
	}

	else {
		/*No operation */
	}

}

static fsp_err_t blankcheck_event_flag(void) {
    fsp_err_t err = FSP_SUCCESS;
    /* Wait for callback function to set flag */
    while (!(g_b_flash_event_not_blank || g_b_flash_event_blank)) {
    	vTaskDelay(10); // Sleep for 10-ms
    }

	if (g_b_flash_event_not_blank) {
        ////RTT_LOG("[DF]: ERR: Data Flash is not blank, unable to write the data");
        /* Reset Flag */
        g_b_flash_event_not_blank = false;
        return (fsp_err_t)FLASH_EVENT_NOT_BLANK;
	}

	else {
        g_b_flash_event_blank = false;
    }
    return err;
}

#endif//DATAFLASH_MODULE
