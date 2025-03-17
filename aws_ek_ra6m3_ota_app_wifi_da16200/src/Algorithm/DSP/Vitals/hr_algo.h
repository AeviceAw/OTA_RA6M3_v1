/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * hr_algo.h
 *
 *  Created on: 25 Feb 2020
 *      Author: aiman
 */

#ifndef HR_ALGO_H_
#define HR_ALGO_H_

#include "initialisations.h"

#define FS 4000
#define HRPERIOD 5
#define HRDATASIZE (FS*HRPERIOD)

#define HRBLOCKSIZE 400
#define NUMTAPS 6
#define M1 4

#define HRBLOCKSIZE2 100
#define NUMTAPS2 6
#define M2 4
#define M3 1

#define HRBLOCKSIZE3 100

#define NUMTAPS3 50  //Follow FIR Array Length

#define HRAVERAGESIZE (85*(FS/M1/M2)/500)

#define HRSLOTDATA1_LEN ((HRDATASIZE/HRBLOCKSIZE)*(HRBLOCKSIZE/M1))
#define HRSLOTDATA2_LEN ((HRSLOTDATA1_LEN/HRBLOCKSIZE2)*(HRBLOCKSIZE2/M2))
#define HRSLOTDATA3_LEN ((HRSLOTDATA2_LEN/HRBLOCKSIZE3)*(HRBLOCKSIZE3/M3))

#define RESET_HRCOUNT_LIMT 3
#define NDECIMAL 4

float32_t hr_algo(void);

#endif /* HR_ALGO_H_ */
