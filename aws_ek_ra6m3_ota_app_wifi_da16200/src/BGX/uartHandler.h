/*
 * uartHandler.h
 *
 *  Created on: 7 Aug 2024
 *      Author: yyang
 */

#ifndef UARTHANDLER_H_
#define UARTHANDLER_H_

#include "initialisations.h"

fsp_err_t g_sf_comms0_write(uart_ctrl_t * inCtrl, uint8_t* inSrc, uint32_t inBytes, uint32_t inTout);
fsp_err_t g_sf_comms0_read(uart_ctrl_t * inCtrl, uint8_t* inDst, uint32_t inBytes, uint32_t inTout);
fsp_err_t g_sf_ssidpw_read( uint8_t* inSrc, uint8_t* inSSID,uint8_t* inPWD,uint32_t inTout);
uint8_t* getGuartBuffAddr(void);
bool getLyraRxCompleteFlag(void);
void setLyraRxCompleteFlag(bool inBool);
bool getLyraTxCompleteFlag(void);
void setLyraTxCompleteFlag(bool inBool);

// UART THREAD API
bool checkIfSkipLoopAndTxAnyFeedback(void);
bool checkButtonpressOrWaitUntilBLEconnected(void);

#endif /* UARTHANDLER_H_ */
