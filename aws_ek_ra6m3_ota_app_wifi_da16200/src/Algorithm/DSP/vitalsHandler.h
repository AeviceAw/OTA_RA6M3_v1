/*
 * vitalsHandler.h
 *
 *  Created on: 13 Aug 2024
 *      Author: awlir
 */

#ifndef ALGORITHM_DSP_VITALS_VITALSHANDLER_H_
#define ALGORITHM_DSP_VITALS_VITALSHANDLER_H_

#include "initialisations.h"

/* Handlers for HR Computations */
void display_HR(void);

/* Handlers for RR Computations */
void display_RR(void);
void RR_entropy_15(void);


float Get_KMDC_Threshold(void);
void Set_KMDC_Threshold(float inKMDC_Threshold);

/* Handlers for WZ Computations */
float getKMDC(void);
float d_diff(float* d_hat, const float* d_tru, float* d_err, uint32_t d_len);
#endif /* ALGORITHM_DSP_VITALS_VITALSHANDLER_H_ */
