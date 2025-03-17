/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * frequency_test.c
 *
 *  Created on: 28 Apr 2020
 *      Author: aiman
 */

#include "initialisations.h"
#include "frequency_test.h"

//#define N1 1024
#define N2 1024

#ifdef N1
	#define FNFFT 1024
	#define FNWIND 1024
	float32_t FfftOutputHalved[(uint32_t) FNWIND/2];
	uint32_t Threshold = 50;
	uint32_t FfftOutputi[(uint32_t) FNWIND/2];
	float32_t FslotData[(uint32_t) FNWIND*2];
#elif N2
	#define FNFFT 1024
	#define FNWIND 1024
	uint32_t FfftOutputi[(uint32_t) FNWIND/2];
	uint32_t Threshold = 50;
#endif

extern float32_t fdata[DATA_LEN];

uint8_t f60 = 0;
uint8_t f150 = 0;
uint8_t f500 = 0;
uint8_t f900 = 0;
uint8_t f1800 = 0;

float32_t FfftOutput[(uint32_t) FNWIND];

void frequency_test_init(void){
	f60 = 0;
	f150 = 0;
	f500 = 0;
	f900 = 0;
	f1800 = 0;
}

/*
 * ********************************************************************
 * @brief handler to call for the FFT of <<fdata>> and determine
 *        if the power spectrum density corresponds to [60,150,500,900,1800] Hz
 *
 *  @param  (N.A.) Calls frequency_test_results();
 *
 *  @return uint8_t Ftest  signifying completion of PSD checks
 *
 *  @example (uint8_t) Ftest=frequency_test_results();
 *  *******************************************************************
 */
uint8_t frequency_test(void)
{
    uint8_t Ftest = 0.0f;

    Ftest=frequency_test_results();

    return Ftest;
}

#ifdef N1
uint8_t frequency_test_results(void)
{
    uint8_t Ftest = 0.0f;

    //    float32_t FslotData[(uint32_t) FNWIND*2];

    for(uint32_t num = 0; num <(uint32_t)FNWIND*2; num++)
    {
        FslotData[num] = 0.0f;
    }

    for (uint32_t i1 = 0, i2 = 0; i1<FNWIND; i1++)
    {
        FslotData[i2] = fdata[i1];
        i2++;
        FslotData[i2] = 0.0f;
        i2++;
    }

    /* process the data through the CFFT/CIFFT module */
    uint8_t FifftFlag = 0;
    uint8_t FdoBitReverse = 1;
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, FslotData, FifftFlag, FdoBitReverse);

    /* clear WZfftOutput variable */
    for(uint32_t num = 0; num <(uint32_t)FNWIND; num++)
    {
        FfftOutput[num] = 0.0f;
    }

    /* Process the data through the Complex Magnitude Module for
    calculating the magnitude at each bin */
    uint32_t FfftSize = (uint32_t)FNFFT;
    arm_cmplx_mag_f32(FslotData, FfftOutput, FfftSize);

    for(uint32_t i1 = 0; i1<((uint32_t)FNWIND)/2; i1++)
    {
        FfftOutputHalved[i1] = FfftOutput[i1];
    }

    // Convert to Magnitude to Integer
    for(uint32_t i1 = 0; i1<(uint32_t)(FNWIND/2); i1++)
    {
        //FfftOutputi[i1] = (uint32_t) floor(fabs(FfftOutputHalved[i1]*10000.0f));
        FfftOutputi[i1] = (uint32_t) floor(fabs(FfftOutputHalved[i1]));
    }

    // fBin=Fs/NFFT=4e3/1024 = 3.9063 Hz
    // 15 (60Hz)   38 (150Hz)   230 (900Hz)  460 (1800Hz)
    for (uint32_t i = 0;i<5;i++)
    {
        if (FfftOutputi[12+i] > 30)
        {
            f60   = 1; // ~ [0046-0067] Hz
        }
        if (FfftOutputi[35+i] > Threshold)
        {
            f150  = 1; // ~ [0136-0157]  Hz
        }
        if (FfftOutputi[125+i] > Threshold)
        {
            f500  = 1; // ~ [0488-0507]  Hz
        }
        if (FfftOutputi[227+i] > Threshold)
        {
        	f900  = 1; // ~ [0886-0907]  Hz
        }
        if (FfftOutputi[463+i] > Threshold)
        {
            f1800 = 1; // ~ [1789-1809]  Hz
        }
    }


    Ftest = 1;

    return Ftest;
}
#elif N2

float fBin = (float) (4000.0f/FNFFT);
#define BW 20
#define nWin (uint32_t) floor(DATASIZE/FNFFT)
#define nBin  (uint32_t) floor( ((float) BW*1.0f)/fBin )
#define n0060 (uint32_t) floor(50.0f/fBin)
#define n0150 (uint32_t) floor(140.0f/fBin)
#define n0500 (uint32_t) floor(490.0f/fBin)
#define n0900 (uint32_t) floor(890.0f/fBin)
#define n1800 (uint32_t) floor(1790.0f/fBin)

/*
 * ********************************************************************
 * @brief Function to compute the NFFT of <<fdata>> and determine
 *        if the power spectrum density corresponds to [60,150,500,900,1800] Hz
 *
 *  @param  (global) fdata
 *
 *  @return uint8_t Ftest  signifying completion of PSD checks
 *
 *  @example (uint8_t) Ftest=frequency_test_results();
 *  *******************************************************************
 */
uint8_t frequency_test_results(void){

    uint8_t Ftest = 0.0f;

    // Loop Through each NFFT windows in wIdx, where wIdx=[1,nWin]
    for (uint32_t wIdx=0;wIdx<nWin;wIdx++){
    	gFFT_hamming_NFFT(&fdata[(uint32_t) floor(wIdx*FNFFT)],FfftOutput);

    	// Convert to Magnitude to Integer (for visualizing in render)
    	for(uint32_t i1 = 0; i1<(uint32_t)(FNWIND/2); i1++)
    		FfftOutputi[i1] = (uint32_t) floor(fabs(FfftOutput[i1]));
    	// end for i1=1:NFFT/2

    	// Check all the Frequency Bin at [60,150,500,900,1800] Hz
    	for (uint32_t i = 0;i<nBin;i++)  {
    		if (FfftOutputi[n0060+i] > 30)
    			f60   = 1; // ~ [0050-0069] Hz
    		if (FfftOutputi[n0150+i] > Threshold)
    			f150  = 1; // ~ [0140-0159]  Hz
    		if (FfftOutputi[n0500+i] > Threshold)
    			f500  = 1; // ~ [0490-0509]  Hz
    		if (FfftOutputi[n0900+i] > Threshold)
    			f900  = 1; // ~ [0890-0909]  Hz
    		if (FfftOutputi[n1800+i] > Threshold)
    			f1800 = 1; // ~ [1790-1809]  Hz
    	} // end for i=1:nBin
    }// end for wIdx=1:w

    Ftest = (uint8_t)1;
    return Ftest;

} // end frequency_test_results()
/*
 * COmpute the N-FFT Point FFT of float x[NFFT], i.e., NFFT \in [512,4096]
 * Arguments    : float* fin  pointer to input  vector  float x[4096]
 *                float* Xm   pointer to output vector  abs(fft(x,4096)))
 *                float* Xa   pointer to output vector  angle(fft(x,4096)))
 * Return Type  : null
 */
float fout[FNWIND*2] ={0.0f};
void gFFT_hamming_NFFT(float *fin, float *Xm)
{

	/* (1) Prepare the Vector to perform fft */
	for (uint32_t i1 = 0, i2 = 0, i3 = 0; i1 < (uint32_t) FNWIND; i1++)
	{
		fout[i2] = fin[i1]; // Apply Hamming window
		i2++;
		i3++;
		fout[i2] = 0.0f;
		i2++;
	} // end for ( i1 = 1:nwind + offset ; i2 = 0, i3 = 1:nwind )

	/* (2) process the data through the CFFT/CIFFT module */
	//int8_T WZifftFlag = 0 (forward transform);  int8_T WZdoBitReverse = 1 (enables bit reversal of output);
	if (FNFFT==512)
		arm_cfft_f32(&arm_cfft_sR_f32_len512, fout, 0, 1);
	else if (FNFFT==1024)
		arm_cfft_f32(&arm_cfft_sR_f32_len1024, fout, 0, 1);
	else if (FNFFT==4096)
		arm_cfft_f32(&arm_cfft_sR_f32_len4096, fout, 0, 1);

	// (2) clear Mag for the mth-window FFT
	//for (uint32_t ii = 0; ii < (uint32_t) WIN_L ; ii ++)
	//	Xm[ii] = 0.0f;
	memset(Xm,0,(size_t)sizeof(float32_t)*FNWIND);

	/* (3) Process the data through the Complex Magnitude Module for
                calculating the magnitude at each bin */
	arm_cmplx_mag_f32(fout, Xm, (uint32_t)FNWIND);

	// (4) Get the Angle of the FFT
	/*
	for (uint32_t i1 = 0, i2 = 0; i1 < (uint32_t) WIN_L ; i1 ++)
	{
		Xa[i1] = approxAtan2(fout[i2+1],fout[i2]);
		i2 = i2+2;
	}  // end for Get Angle of the FFT
	*/
} // gFFT_hamming_NFFT()
#endif
