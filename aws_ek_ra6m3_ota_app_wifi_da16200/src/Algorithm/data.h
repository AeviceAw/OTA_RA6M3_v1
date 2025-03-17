/*
 * data.h
 *
 *  Created on: 14 Jun 2024
 *      Author: yyang
 */

#ifndef DATA_H_
#define DATA_H_

#include "initialisations.h"

#define UART_DATA_SIZE      61440           //5sec UART Buffer
#define EUART_DATA_SIZE     768             // 1/8 of 5sec UART Buffer
#define DATA_LEN            20480           //5sec Sample Data
#define DATAPACKET          DATA*3
#define DATASIZE            DATA_LEN
#define AUDIOSIZE           1024
#define BYTEARRAYSIZE       AUDIOSIZE
#define SIZE                BYTEARRAYSIZE*3
#define MSGSIZE             (BYTEARRAYSIZE*3)+50

#define BUF_LEN             12// HR/RR/Wz Publish-Buffer Length dataprocessing_thread_entry()

#endif /* DATA_H_ */


