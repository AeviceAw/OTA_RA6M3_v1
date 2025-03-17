/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
#include "initialisations.h"
// =================================================================
// Timers
// =================================================================
uint8_t g_uart_timer_started = 0;
uint32_t g_uart_timer_count = 0;
uint32_t g_uart_timer_prev = 0;
uint32_t g_uart_timer_elasped = 0;

uint8_t get_uart_timer_started_val(void){
    return g_uart_timer_started;
} // end get_uart_timer_started_val()

void g_tic(void){
    g_uart_timer_prev=g_uart_timer_count;
} // end g_tic()

void g_toc(void){
    g_uart_timer_diff();
} // end g_toc()

void g_uart_timer_diff(void){
    g_uart_timer_elasped = (uint32_t) (g_uart_timer_count-g_uart_timer_prev); // Find the difference
    g_uart_timer_prev = g_uart_timer_count;
} // end g_uart_timer_elasped()

fsp_err_t g_uart_timer_start(void){
    volatile fsp_err_t val;
    g_uart_timer_count = 0;
    g_uart_timer_started = 1;

    val = R_GPT_Open(&g_uart_timer_ctrl, &g_uart_timer_cfg);
    APP_ERR_TRAP(val);
    val = R_GPT_Start(&g_uart_timer_ctrl);
    APP_ERR_TRAP(val);

    return val;
} // end g_uart_timer_start()

fsp_err_t g_uart_timer_stop(void){
    fsp_err_t val;
    g_uart_timer_elasped = (uint32_t) (g_uart_timer_count-g_uart_timer_prev);

    val = R_GPT_Stop(&g_uart_timer_ctrl);
    APP_ERR_TRAP(val);
    val = R_GPT_Close(&g_uart_timer_ctrl);
    APP_ERR_TRAP(val);
    g_uart_timer_started = 0;
    return val;
} // end g_uart_timer_stop()


void g_uart_timer_callback(timer_callback_args_t * p_args){
    FSP_PARAMETER_NOT_USED (p_args);
    g_uart_timer_count++;
} // end g_uart_timer_callback()

// =================================================================
// End Timers
// =================================================================
