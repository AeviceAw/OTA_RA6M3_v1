/*
 * crcHandler.h
 *
 *  Created on: 13 Aug 2024
 *      Author: awlir
 */

#ifndef CRCHANDLER_H_
#define CRCHANDLER_H_
#include "initialisations.h"

fsp_err_t crcCompute(void* inArrPtr,uint32_t inArrByte,uint32_t * outCrcVal);

#endif /* CRCHANDLER_H_ */
