/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * rr_algo.h
 *
 *  Created on: 3 Mar 2020
 *      Author: aiman
 */

#ifndef RR_ALGO_H_
#define RR_ALGO_H_

#include "initialisations.h"
#include "hr_algo.h"

//#define DATA 20480
//#define FS 4000
#define RRNFFT 512
#define RRNWIND 512
#define RRSLOTLENGTH (RRNWIND/2)
#define RRNOVERLAP (RRNWIND-RRSLOTLENGTH)
#define RRPERIOD 5
#define RRSLOTPB ((DATA/RRSLOTLENGTH)-1)
#define RRDATASIZE (RRPERIOD*FS)

#define RRAVERAGESIZE (RRNFFT/4)
//#define RRSMOOTHSIZE 8 %Changed this to a variable
#define RRSMOOTHCLEAN 4

#define RRCUTOFF1 (200*RRNFFT*2)/(FS*2)
#define RRCUTOFF2 (1000*RRNFFT*2)/(FS*2)

#define RRDATANUM 3
#define RRETP5 ((RRDATASIZE/RRSLOTLENGTH)-1)
#define RRETP15 (RRETP5*RRDATANUM)
#define RRETPCLEAN15 (RRETP15-(2*RRSMOOTHCLEAN)) // V 29/4
#define RESET_RRCOUNT_LIMIT 3

void rr_entropy(void);
float32_t rr_algo(void);
#endif /* RR_ALGO_H_ */
