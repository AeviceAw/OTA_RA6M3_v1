/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * getCP_data.c
 *
 *  Created on: 06 Jul 2023
 *      Author: YY
 *
 */


/* Include Files */
#include "initialisations.h"
#include "getCP_data.h"
#include "getCP.h"
#include "extractFea.h"

/* Variable Definitions */
const float NRow = NROW;
const float K = KFEA;

float P_cc[NROW*(KFEA+2)]={0.0f};
float P_bc[NROW*(KFEA+2)]={0.0f};
float P_wc[NROW*(KFEA+2)]={0.0f};

// NUH Phase 2 - Data Augmentation 7
//Normal Breath (MLE)
//[ C , B , W ]
//[003,355,039]
//[0.01,0.90,0.10]
//Wheeze (MLE)
//[ C , B , W ]
//[010,137,1915]
//[0.00,0.07,0.93]
//
//Thres: 0.500, NB: 0.914, WZ: 0.892
//
// SS=[3    24    47    69    70]
const float weig[KFEA]={0.029268f,0.036426f,0.044180f,0.043885f,0.037791f};
const float mean_cough[KFEA]={-1.056303f,-0.059814f,2.399379f,1.803704f,1.579659f};
const float mean_breath[KFEA]={5.457512f,0.110478f,0.039388f,1.142405f,1.024837f};
const float mean_wheeze[KFEA]={5.295285f,0.047418f,0.037659f,1.321989f,1.075450f};
const float std_cough[KFEA]={4.563965f,0.383011f,4.280878f,0.665543f,0.647610f};
const float std_breath[KFEA]={1.958202f,0.227346f,0.252853f,0.350019f,0.157062f};
const float std_wheeze[KFEA]={1.855414f,0.361845f,0.237037f,0.468152f,0.264769f};

/*
// NUH Phase 2 - Data Augmentation 6
//Normal Breath (MLE)
//[ C , B , W ]
//[007,326,066]
//[0.02,0.82,0.17]
//Wheeze (MLE)
//[ C , B , W ]
//[020,047,1992]
//[0.01,0.02,0.97]
//
//Thres: 0.500, NB: 0.902, WZ: 0.916
//
// SS=[ 3     6    24    69    70]
const float weig[KFEA]={0.062762f,0.046333f,0.037842f,0.057910f,0.042226f};
const float mean_cough[KFEA]={-1.056303f,0.797782f,-0.059815f,1.803704f,1.579659f};
const float mean_breath[KFEA]={5.457512f,-1.505960f,0.110478f,1.142405f,1.024837f};
const float mean_wheeze[KFEA]={5.295285f,-1.298744f,0.047418f,1.321989f,1.075450f};
const float std_cough[KFEA]={4.563965f,2.063912f,0.383011f,0.665543f,0.647610f};
const float std_breath[KFEA]={1.958202f,1.132955f,0.227346f,0.350019f,0.157062f};
const float std_wheeze[KFEA]={1.855414f,1.303424f,0.361845f,0.468152f,0.264769f};
*/

// A&D Demo Weights
//(1) sK  = [2     6    34    57    70] - v1_6_7 ADPCM de-mean,normalize by Max; window - mean
//(2) Perf= [0.98496240	0.81818181	0.81034482	0.87116307] ; // Cough, Breath, Wheeze + Xue Bin's, Accuracy (Windows de-meaned and normalized)
//const float weig[KFEA]={0.001556f,0.000710f,0.000640f,0.001076f,0.002160f};
//const float mean_cough[KFEA]={-3.005328f,0.742154f,-0.167062f,4.613894f,2.239603f};
//const float mean_breath[KFEA]={12.529761f,-1.820798f,-0.091215f,7.123346f,1.021319f};
//const float mean_wheeze[KFEA]={13.841747f,1.319126f,0.077250f,6.268958f,1.331156f};
//const float std_cough[KFEA]={4.559873f,2.282691f,0.284395f,2.463504f,0.751208f};
//const float std_breath[KFEA]={1.679918f,1.351968f,0.128079f,0.503002f,0.145110f};
//const float std_wheeze[KFEA]={3.956314f,2.558054f,0.229744f,1.654235f,0.543820f};


/*
 * File trailer for getCP_data.c
 *
 * [EOF]
 */
