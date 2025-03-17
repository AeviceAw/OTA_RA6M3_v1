/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * extractFea.h
 *
 *  Created on: 06 Jul 2023
 *      Author: YY
 */

#ifndef EXTRACTFEA_H_
#define EXTRACTFEA_H_

#include "initialisations.h"

#define KFEA 5
#define NROW 79                       // Breath
#define SAMP_L 20480                  // Recording Sample Length(1:20480) (x47.wav)
                            // (~5.12 period of fmin Hz): 5.12 x (numOfSample/numOfCycleInFmin) = (numOfSample/Sec) / (numOfCycleInFmin/Sec)
#define WIN_L 512           // NFFT for extractFea ~  WIN_L = floor(5.12*fs/fmin) = 512 ; fs = 4e3 ; fmin = 80 hz
#define OVERLAPSIZE 0.5f   // initialize moving window size and overlap size
#define WIN_N floor(SAMP_L/WIN_L)*2-1 // Number of Segment
#define NUMOFBAND 9// Number of Band
#define I_B 12     // (bins/Band) = NumOfBins Per Band
#define NMEL 26    // Number of MFCC Coefficients (MUST BE EVEN!)

#ifndef PI
  #define PI               3.14159265358979f
#endif

	// Variables for Testing - C001_WZ_001_Sunit.wav (RAW) -------------------------------------------------------
	//const float32_t f_wz[20480];

	// Variables for Classification
    //extern float fdata[SAMP_L];
    //extern const float fdata0[SAMP_L];

	// Variables for Classifications -----------------------------------------------------------------------------
	extern float b_fv[NROW*(KFEA+2)];
	extern float d_results[3];          //d_results[0] = cough; d_results[1] = breath; d_results[2] = wheeze
	//float M_test[NROW][KFEA];  //NROW = floor(SAMP_L/winL)*2-1
	//uint8_t WZ_ready;

    // Function Prototypes
    void max_abs(float* fin, int Len, float* fMax);
    void extractCoef(float* fin, float M_test[NROW*(KFEA+2)]);  // Extract Feature into M_test= (nRow x (KFEA+2))
    void dct(float* a,float* b);                                // DCT for 26 Coeffcients
    void gFFT_hamming_256(float *fin, float *Xm);    //fft for 256 nfft point
    void gFFT_hamming_512(float *fin, float *Xm);    //fft for 256 nfft point

//    void mergeSort(int* x_rank,float* x,float* y, int len, char TOP, char UD); //Descending sort of the energy band
//    void arrCpy(float* dst, float* src, int len);                             // Copy array from src to dst
//    void bRank(int* y_rank, float* y,float* y_sort,int y_len);              // Find the rank of the energy band
    void bRank(int* y_rank, const float* y, const float* y_sort, int len) ;
    void mergeSort(int* x_rank, float* x, float* y, int len, char TOP, char UD);
    void merge(float* y, const float* y1, int y1_len, const float* y2, int y2_len, char UD);
    void arrCpy(float* dst, const float* src, int len);
#endif /* EXTRACTFEA_H_ */
