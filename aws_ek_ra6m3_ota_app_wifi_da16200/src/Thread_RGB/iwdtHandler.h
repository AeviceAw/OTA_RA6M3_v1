/*
 * iwdtHandler.h
 *
 *  Created on: 12 Nov 2024
 *      Author: yyang
 */

#ifndef THREAD_RGB_IWDTHANDLER_H_
#define THREAD_RGB_IWDTHANDLER_H_

#include "initialisations.h"


fsp_err_t iwdt_init(void);
uint32_t* iwdt_counter_addr(void);
fsp_err_t iwdt_refresh(void);

#endif /* THREAD_RGB_IWDTHANDLER_H_ */
