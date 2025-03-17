/*
 * iwdtHandler.c
 *
 *  Created on: 12 Nov 2024
 *      Author: yyang
 */
#include <Thread_RGB/iwdtHandler.h>

/*******************************************************************************************************************//**
 *  @brief       Initialize independent Watch Dog Timer
 *  @param[in]   Global          <<g_iwdt_ctrl>>, <<g_iwdt_cfg>>
 *  @retval      fsp_err_t        FSP_SUCCESS OR other ERROR_CODE
 *  @remarks
 *  The IWDT operates from IWDTCLK. With a IWDTCLK of 15000 Hz, the maximum time from the last refresh to device reset or NMI generation will be just below 35 seconds as detailed below.
 *
 *  IWDTCLK = 15000 Hz
 *  Clock division ratio = IWDTCLK / 256
 *  Timeout period = 512 cycles
 *  WDT clock frequency = 15000 Hz / 256 = 58.5938 Hz
 *  Cycle time = 1 / 58.5938 Hz = 0.0171 s
 *  Timeout = 0.0171 s x 512 cycles = 8.7381 seconds
 **********************************************************************************************************************/
fsp_err_t iwdt_init(void){
    fsp_err_t status = FSP_ERR_ASSERTION;

    /* iWatchdog init */
    status = R_IWDT_Open(&g_iwdt_ctrl, &g_iwdt_cfg);

    /* (Optional) Enable the IWDT to count and generate NMI or reset when the
     * debugger is connected. */
    R_DEBUG->DBGSTOPCR_b.DBGSTOP_IWDT = 0;

    return status;
} // end init_iwdt()

/*******************************************************************************************************************//**
 *  @brief       Get the pointer address of the <<iwdt_counter>>
 *  @param[in]   Global          <<iwdt_counter>
 *  @retval      uint32_t*        address to global variable <<(uint32_t) iwdt_counter>>
 **********************************************************************************************************************/
uint32_t iwdt_counter = 0;
uint32_t* iwdt_counter_addr(void){
    return &iwdt_counter;
} // end iwdt_counter_addr()


/*******************************************************************************************************************//**
 *  @brief      refresh the independent Watch Dog Timer
 *  @param[in]   Global       <<g_iwdt_ctrl>>, <<iwdt_counter>>
 *  @retval      fsp_err_t    FSP_SUCCESS OR other ERROR_CODE
 **********************************************************************************************************************/
fsp_err_t iwdt_refresh(void){
    fsp_err_t status = FSP_ERR_ASSERTION;

    status = R_IWDT_Refresh(&g_iwdt_ctrl);
    status = R_IWDT_CounterGet(&g_iwdt_ctrl, iwdt_counter_addr());

    return status;
} // end iwdt_refresh()

