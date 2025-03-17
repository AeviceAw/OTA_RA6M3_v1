/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * BPF.c
 *
 *  Created on: 28 Feb 2022
 *      Author: YY
 */


#include "initialisations.h"
#include <Algorithm/DSP/KMDC/extractFea.h>
#include <Algorithm/DSP/KMDC/BPF.h>
#include <arm_math.h>

// FILTER Coefficients
#define IIR_ORDER 20
#define IIR_NUMSTAGES (IIR_ORDER / 2)
static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5 * IIR_NUMSTAGES] = {0.770485f,-1.540970f,0.770485f,1.873839f,-0.877912f,1.000000f,-2.000000f,1.000000f,1.885124f,-0.889220f,1.000000f,-2.000000f,1.000000f,1.906951f,-0.911094f,1.000000f,-2.000000f,1.000000f,1.937804f,-0.942013f,1.000000f,2.000000f,1.000000f,-1.969188f,-0.969431f,1.000000f,2.000000f,1.000000f,-1.972144f,-0.972387f,1.000000f,2.000000f,1.000000f,-1.977786f,-0.978031f,1.000000f,-2.000000f,1.000000f,1.975347f,-0.979636f,1.000000f,2.000000f,1.000000f,-1.985598f,-0.985843f,1.000000f,2.000000f,1.000000f,-1.994854f,-0.995100f};
arm_biquad_cascade_df2T_instance_f32 const filtInst = {IIR_ORDER / 2, m_biquad_state, m_biquad_coeffs };

/*
 * Perform Biquad Cascade IIR Filter Direct Form 2
 * Arguments    : float* fdata[SAMP_L] pointer to input  vector of recording
 *                float* fFilt[SAMP_L] pointer to output  vector of BPF(Recording)
 *                SAMP_L = number of samples
 *
 * Remarks: m_biquad_coeffs is designed from MATLAB based on
 *     odr=10;fmin=41.914;fmax=1990;fdata_fs=4e3;
 *     [zlo,plo,klo] = butter(odr,[fmin fmax]/floor(fdata_fs/2));
 *     soslo = zp2sos(zlo,plo,klo);
 *     soslo(:,4)=[]
 *     soslo(:,end-1:end)=-soslo(:,end-1:end);
 *     m_biquad_coeffs = vec(soslo')'
 *
 *     Note: if MATLAB's     odr = 10;
 *              ARM's  IIR_ORDER = 20;
 *
 * Return Type  : null
 */
float fFilt_max[1]= {1.0f};  // max of all values inside fdata
void BPF(float* fIn, float* fFilt) {

    // http://gaidi.ca/weblog/implementing-a-biquad-cascade-iir-filter-on-a-cortex-m4

    // Perform Band Pass Filter using Butter (Biquad Cascade IIR Filter Direct Form 2)
    // BPF(fmin=80,fmax=1990,fs=4e3,odr=10)
    //arm_biquad_cascade_df2T_f32(&filtInst, fdata, fFilt, SAMP_L);
    arm_biquad_cascade_df2T_f32(&filtInst, fIn, fFilt, SAMP_L);

    // Find the Max element in fFilt
    //float fFilt_max[1]= {0.0f};  // max of all values inside fdata
    float fFilt_min[1]= {0.0f};  // min of all values inside fdata
    arm_max_f32(fFilt,SAMP_L,fFilt_max,0);
    arm_min_f32(fFilt,SAMP_L,fFilt_min,0);

    // This doesnt work; maybe cause of complicated computation needed
    // fFilt_max[0] = fFilt_max[0]<-fFilt_min[0]? -fFilt_min[0]:fFilt_max[0];
    if (fFilt_max[0]<0.0f)
        fFilt_max[0]=-1.0f*fFilt_max[0];
    if (fFilt_min[0]<0.0f)
        fFilt_min[0]=-1.0f*fFilt_min[0];
    if  (fFilt_max[0]<fFilt_min[0])
        fFilt_max[0] = fFilt_min[0];

    // Normalize fFilt to +-ONE // Normalize during feaExtraction
//    if (fFilt_max[0]>0.000001f)
//        for(int ell = 0; ell< (int)SAMP_L;ell++)
//            fFilt[ell] = fFilt[ell]/fFilt_max[0];
} // end BPF()
