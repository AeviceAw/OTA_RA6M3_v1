/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * BPF.h
 *
 *  Created on: 28 Feb 2022
 *      Author: YY
 */

#ifndef BPF_H_
#define BPF_H_

//    #ifndef ARM_MATH_CM4
//        #define ARM_MATH_CM4
//        #include <arm_math.h>
//        #include "arm_const_structs.h"
//    #endif

#include "initialisations.h"
//extern void BPF(void);
void BPF(float* fIn, float* fFilt);
extern float fFilt_max[1];  // max of all values inside fdata

#endif /* BPF_H_ */
