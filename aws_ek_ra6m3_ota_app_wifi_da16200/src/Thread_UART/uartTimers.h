/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
#ifndef UART_HANDLER_UART_TIMERS_H_
#define UART_HANDLER_UART_TIMERS_H_

#include "initialisations.h"

void g_tic(void);
void g_toc(void);
void g_uart_timer_diff(void);
fsp_err_t g_uart_timer_start(void);
fsp_err_t g_uart_timer_stop(void);

uint8_t get_uart_timer_started_val(void);

#endif /* UART_HANDLER_UART_TIMERS_H_ */
