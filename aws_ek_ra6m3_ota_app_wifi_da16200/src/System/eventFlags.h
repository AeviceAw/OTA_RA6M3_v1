/*
 * eventFlags.h
 *
 *  Created on: 13 Nov 2024
 *      Author: yyang
 */

#ifndef EVENTFLAGS_H_
#define EVENTFLAGS_H_

#include "initialisations.h"

/* Sync Flag Bits */
#define UART_THREAD         0x001
#define SSIDPW_THREAD       0x010
#define RGB_THREAD          0x100
#define SSIDPW_RGB_THREAD   0x110
#define SYNC_FLAG_ALL       0x111

#endif /* EVENTFLAGS_H_ */
