/* generated thread header file - do not edit */
#ifndef UART_THREAD_H_
#define UART_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void uart_thread_entry(void * pvParameters);
                #else
extern void uart_thread_entry(void *pvParameters);
#endif
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_uart_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_uart_timer_ctrl;
extern const timer_cfg_t g_uart_timer_cfg;

#ifndef g_uart_timer_callback
void g_uart_timer_callback(timer_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart1;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart1_ctrl;
extern const uart_cfg_t g_uart1_cfg;
extern const sci_uart_extended_cfg_t g_uart1_cfg_extend;

#ifndef NULL
void NULL(uart_callback_args_t *p_args);
#endif
FSP_FOOTER
#endif /* UART_THREAD_H_ */
