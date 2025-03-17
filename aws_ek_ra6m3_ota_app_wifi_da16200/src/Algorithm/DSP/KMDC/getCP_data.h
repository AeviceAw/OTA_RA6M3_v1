/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * getCP_data.h
 *
 *  Created on: 06 Jul 2023
 *      Author: YY
 */

#ifndef GETCP_DATA_H
#define GETCP_DATA_H

/* Include Files */
//#include <stddef.h>
//#include <stdlib.h>
//#include "hal_entry.h"
#include "initialisations.h"
#include "getCP_data.h"
#include "getCP.h"
#include "extractFea.h"

/* Variable Declarations */
extern const float NRow;
extern const float K;
extern float P_cc[NROW*(KFEA+2)];
extern float P_bc[NROW*(KFEA+2)];
extern float P_wc[NROW*(KFEA+2)];
//extern float b_fv[NROW*(KFEA+2)];

extern const float edge_cough[101*KFEA];
extern const float edge_breath[101*KFEA];
extern const float edge_wheeze[101*KFEA];
extern const float density_cough[100*KFEA];
extern const float density_breath[100*KFEA];
extern const float density_wheeze[100*KFEA];

//extern bool isInitialized_getCP;

#endif

/*
 * File trailer for getCP_data.h
 *
 * [EOF]
 */
