/*
 * crcHandler.c
 *
 *  Created on: 13 Aug 2024
 *      Author: awlir
 */

#ifndef CRCHANDLER_C_
#define CRCHANDLER_C_
#include "initialisations.h"



/*******************************************************************************************************************//**
 * @brief       Compute the crc8 of <<Arr[Byte]>> with <<Byte>> and save output in <<outCrcVal>>
 * @param[in]   (void*)             inArrPtr,  e.g., <<(uint8_t*) str_encoded[2048]>>
 * @param[in]   (uint32_t)          inArrByte, e.g., <<(uint32_t*) 2048>>
 * @param[in]   (uint32_t*)         outCrcVal, e.g., <<(uint32_t*) inCrc32Val_w>>
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open
 ***********************************************************************************************************************/
fsp_err_t crcCompute(void* inArrPtr,uint32_t inArrByte,uint32_t * outCrcVal){
    fsp_err_t status = FSP_SUCCESS;

    crc_input_t p_crc_input ={
        .p_input_buffer = inArrPtr,
        .num_bytes      = inArrByte,
        .crc_seed       = 0,
    };
    status |= R_CRC_Open(&g_crc_ctrl, &g_crc_cfg);
    status |= R_CRC_Calculate(&g_crc_ctrl, &p_crc_input, outCrcVal);
    status |= R_CRC_Close(&g_crc_ctrl);

    //status &= g_crc.p_api->open(g_crc.p_ctrl, g_crc.p_cfg);
    //status &= g_crc.p_api->calculate(g_crc.p_ctrl, (char*)inArrPtr, inArrByte, 0, &outCrcVal);
    //status &= g_crc.p_api->close(g_crc.p_ctrl);

    return status;
}// end crcCompute()


#endif /* CRCHANDLER_C_ */
