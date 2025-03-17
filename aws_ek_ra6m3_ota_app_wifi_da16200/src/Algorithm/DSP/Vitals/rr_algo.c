/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * rr_algo.c
 *
 *  Created on: 04-Jan-2021
 *      Author: Vivian
 *    Revision: The following code is consistent with MATLAB (get)
 *              The following code is consistent with GT_Dock_v8 (Aiman 19-dec-2020)
 *              Did sanity checks with the Matlab version
 *    Revision: Checked with VV (14-June-2021)
 *   Reference: RR_e2Studio.m
 *
 */

#include <Algorithm/DSP/Vitals/rr_algo.h>
#include "initialisations.h"
#include <arm_math.h>

/********** --------------------------------------------------------------------------------------- **********/
///* Global variables *///
extern float32_t fdata[20480];
extern float32_t RRetp5Seconds[RRETP5];
extern float32_t RRetp15Seconds[RRETP15];

/********** --------------------------------------------------------------------------------------- **********/
///* Test variables *///
//int32_t float32_t_to_int32_t (float32_t valuei);

/********** --------------------------------------------------------------------------------------- **********/
//Function prototype

extern float32_t fround(float32_t number, uint8_t N);

void rr_entropy(void)
{
	/* clear RRetpRaw variable */
	float32_t RRetpRaw[RRETP5] = {};
	for(uint32_t i1 = 0; i1<(uint32_t)RRETP5; i1++)
	{
		RRetpRaw[i1] = 0.0f;
	}

	/* RRetpRaw calculations, done in slots*/
	for (uint32_t RRslotIndex = 0, RRcount = 0; RRslotIndex<RRETP5; RRslotIndex++)
	{
		/* clear RRetpRaw variable */
		float32_t RRslotData[RRNWIND*2] = {};
		for(uint32_t num = 0; num <(uint32_t)RRNWIND*2; num++)
		{
			RRslotData[num] = 0.0f;
		}

		/* fill in data according to slots, apply hanning window */
		float32_t RRwin[512] = {0.0000000f,0.0000378f,0.0001512f,0.0003401f,0.0006046f,0.0009446f,0.0013601f,0.0018509f,0.0024171f,0.0030584f,0.0037749f,0.0045665f,0.0054329f,0.0063741f,0.0073899f,0.0084803f,0.0096449f,0.0108836f,0.0121963f,0.0135828f,0.0150428f,0.0165761f,0.0181825f,0.0198617f,0.0216136f,0.0234377f,0.0253339f,0.0273019f,0.0293413f,0.0314519f,0.0336334f,0.0358853f,0.0382074f,0.0405993f,0.0430607f,0.0455912f,0.0481903f,0.0508578f,0.0535932f,0.0563961f,0.0592660f,0.0622026f,0.0652053f,0.0682738f,0.0714076f,0.0746061f,0.0778690f,0.0811957f,0.0845857f,0.0880385f,0.0915536f,0.0951304f,0.0987685f,0.1024672f,0.1062260f,0.1100444f,0.1139217f,0.1178574f,0.1218508f,0.1259015f,0.1300087f,0.1341718f,0.1383902f,0.1426633f,0.1469904f,0.1513709f,0.1558041f,0.1602894f,0.1648260f,0.1694132f,0.1740505f,0.1787370f,0.1834721f,0.1882551f,0.1930852f,0.1979617f,0.2028838f,0.2078509f,0.2128622f,0.2179168f,0.2230141f,0.2281533f,0.2333336f,0.2385542f,0.2438143f,0.2491131f,0.2544499f,0.2598238f,0.2652341f,0.2706798f,0.2761602f,0.2816744f,0.2872216f,0.2928010f,0.2984117f,0.3040530f,0.3097238f,0.3154234f,0.3211509f,0.3269054f,0.3326861f,0.3384921f,0.3443226f,0.3501765f,0.3560531f,0.3619515f,0.3678708f,0.3738100f,0.3797683f,0.3857448f,0.3917385f,0.3977486f,0.4037742f,0.4098143f,0.4158681f,0.4219346f,0.4280129f,0.4341020f,0.4402012f,0.4463093f,0.4524256f,0.4585491f,0.4646788f,0.4708139f,0.4769534f,0.4830964f,0.4892420f,0.4953891f,0.5015370f,0.5076846f,0.5138311f,0.5199755f,0.5261168f,0.5322542f,0.5383868f,0.5445135f,0.5506335f,0.5567458f,0.5628496f,0.5689439f,0.5750277f,0.5811002f,0.5871604f,0.5932075f,0.5992405f,0.6052584f,0.6112605f,0.6172457f,0.6232132f,0.6291621f,0.6350914f,0.6410003f,0.6468879f,0.6527533f,0.6585956f,0.6644140f,0.6702074f,0.6759752f,0.6817163f,0.6874300f,0.6931153f,0.6987714f,0.7043975f,0.7099927f,0.7155561f,0.7210869f,0.7265843f,0.7320475f,0.7374755f,0.7428677f,0.7482232f,0.7535411f,0.7588207f,0.7640611f,0.7692617f,0.7744215f,0.7795398f,0.7846159f,0.7896489f,0.7946382f,0.7995829f,0.8044823f,0.8093357f,0.8141423f,0.8189014f,0.8236123f,0.8282743f,0.8328867f,0.8374487f,0.8419597f,0.8464190f,0.8508260f,0.8551798f,0.8594800f,0.8637259f,0.8679167f,0.8720520f,0.8761310f,0.8801531f,0.8841177f,0.8880243f,0.8918722f,0.8956608f,0.8993897f,0.9030581f,0.9066657f,0.9102117f,0.9136957f,0.9171172f,0.9204756f,0.9237705f,0.9270012f,0.9301675f,0.9332686f,0.9363043f,0.9392740f,0.9421773f,0.9450138f,0.9477830f,0.9504844f,0.9531178f,0.9556827f,0.9581786f,0.9606053f,0.9629624f,0.9652495f,0.9674662f,0.9696122f,0.9716873f,0.9736910f,0.9756232f,0.9774834f,0.9792714f,0.9809870f,0.9826298f,0.9841997f,0.9856964f,0.9871196f,0.9884692f,0.9897450f,0.9909467f,0.9920742f,0.9931273f,0.9941058f,0.9950097f,0.9958387f,0.9965927f,0.9972716f,0.9978754f,0.9984039f,0.9988571f,0.9992348f,0.9995371f,0.9997638f,0.9999150f,0.9999906f,0.9999906f,0.9999150f,0.9997638f,0.9995371f,0.9992348f,0.9988571f,0.9984039f,0.9978754f,0.9972716f,0.9965927f,0.9958387f,0.9950097f,0.9941058f,0.9931273f,0.9920742f,0.9909467f,0.9897450f,0.9884692f,0.9871196f,0.9856964f,0.9841997f,0.9826298f,0.9809870f,0.9792714f,0.9774834f,0.9756232f,0.9736910f,0.9716873f,0.9696122f,0.9674662f,0.9652495f,0.9629624f,0.9606053f,0.9581786f,0.9556827f,0.9531178f,0.9504844f,0.9477830f,0.9450138f,0.9421773f,0.9392740f,0.9363043f,0.9332686f,0.9301675f,0.9270012f,0.9237705f,0.9204756f,0.9171172f,0.9136957f,0.9102117f,0.9066657f,0.9030581f,0.8993897f,0.8956608f,0.8918722f,0.8880243f,0.8841177f,0.8801531f,0.8761310f,0.8720520f,0.8679167f,0.8637259f,0.8594800f,0.8551798f,0.8508260f,0.8464190f,0.8419597f,0.8374487f,0.8328867f,0.8282743f,0.8236123f,0.8189014f,0.8141423f,0.8093357f,0.8044823f,0.7995829f,0.7946382f,0.7896489f,0.7846159f,0.7795398f,0.7744215f,0.7692617f,0.7640611f,0.7588207f,0.7535411f,0.7482232f,0.7428677f,0.7374755f,0.7320475f,0.7265843f,0.7210869f,0.7155561f,0.7099927f,0.7043975f,0.6987714f,0.6931153f,0.6874300f,0.6817163f,0.6759752f,0.6702074f,0.6644140f,0.6585956f,0.6527533f,0.6468879f,0.6410003f,0.6350914f,0.6291621f,0.6232132f,0.6172457f,0.6112605f,0.6052584f,0.5992405f,0.5932075f,0.5871604f,0.5811002f,0.5750277f,0.5689439f,0.5628496f,0.5567458f,0.5506335f,0.5445135f,0.5383868f,0.5322542f,0.5261168f,0.5199755f,0.5138311f,0.5076846f,0.5015370f,0.4953891f,0.4892420f,0.4830964f,0.4769534f,0.4708139f,0.4646788f,0.4585491f,0.4524256f,0.4463093f,0.4402012f,0.4341020f,0.4280129f,0.4219346f,0.4158681f,0.4098143f,0.4037742f,0.3977486f,0.3917385f,0.3857448f,0.3797683f,0.3738100f,0.3678708f,0.3619515f,0.3560531f,0.3501765f,0.3443226f,0.3384921f,0.3326861f,0.3269054f,0.3211509f,0.3154234f,0.3097238f,0.3040530f,0.2984117f,0.2928010f,0.2872216f,0.2816744f,0.2761602f,0.2706798f,0.2652341f,0.2598238f,0.2544499f,0.2491131f,0.2438143f,0.2385542f,0.2333336f,0.2281533f,0.2230141f,0.2179168f,0.2128622f,0.2078509f,0.2028838f,0.1979617f,0.1930852f,0.1882551f,0.1834721f,0.1787370f,0.1740505f,0.1694132f,0.1648260f,0.1602894f,0.1558041f,0.1513709f,0.1469904f,0.1426633f,0.1383902f,0.1341718f,0.1300087f,0.1259015f,0.1218508f,0.1178574f,0.1139217f,0.1100444f,0.1062260f,0.1024672f,0.0987685f,0.0951304f,0.0915536f,0.0880385f,0.0845857f,0.0811957f,0.0778690f,0.0746061f,0.0714076f,0.0682738f,0.0652053f,0.0622026f,0.0592660f,0.0563961f,0.0535932f,0.0508578f,0.0481903f,0.0455912f,0.0430607f,0.0405993f,0.0382074f,0.0358853f,0.0336334f,0.0314519f,0.0293413f,0.0273019f,0.0253339f,0.0234377f,0.0216136f,0.0198617f,0.0181825f,0.0165761f,0.0150428f,0.0135828f,0.0121963f,0.0108836f,0.0096449f,0.0084803f,0.0073899f,0.0063741f,0.0054329f,0.0045665f,0.0037749f,0.0030584f,0.0024171f,0.0018509f,0.0013601f,0.0009446f,0.0006046f,0.0003401f,0.0001512f,0.0000378f,0.0000000f};
		for (uint32_t i1 = (RRslotIndex)*(uint32_t)RRSLOTLENGTH, i2 = 0, i3 = 0; i1<((RRslotIndex)*(uint32_t)RRSLOTLENGTH)+(uint32_t)RRNWIND; i1++)
		{
			RRslotData[i2] = fdata[i1]*RRwin[i3];
			i2++;
			i3++;
			RRslotData[i2] = 0.0f;
			i2++;
		}

		/* process the data through the CFFT/CIFFT module */
		uint8_t RRifftFlag = 0;
		uint8_t RRdoBitReverse = 1;
		arm_cfft_f32(&arm_cfft_sR_f32_len512, RRslotData, RRifftFlag, RRdoBitReverse);

		/* clear RRfftOutput variable */
		float32_t RRfftOutput[RRNWIND] = {};
		for(uint32_t num = 0; num <(uint32_t)RRNWIND; num++)
		{
			RRfftOutput[num] = 0.0f;
		}

		/* process the data through the Complex Magnitude Module for
        calculating the magnitude at each bin */
		uint32_t RRfftSize = (uint32_t)RRNFFT;
		arm_cmplx_mag_f32(RRslotData, RRfftOutput, RRfftSize);

		/* clear RRfftOutputCutoff variable */
		float32_t RRfftOutputCutoff[RRCUTOFF2-RRCUTOFF1] = {0.0f}; // V 28/4

		/* isolate RRfftOutput values which are above cutoff, the squared */ //RRCUTOFF1: 25 array starts at 0 (i.e. 25-1) and RRCUTOFF:128 (i.e. 128-1)
		for(uint32_t i1 = (uint32_t)RRCUTOFF1, i2 = 0; i1<((uint32_t)RRCUTOFF2); i1++)
		{
			float32_t temp = 0.0f;
			RRfftOutputCutoff[i2] = RRfftOutput[i1];
			temp = powf(RRfftOutputCutoff[i2],2);  // V 28/4
			RRfftOutputCutoff[i2] = temp;  // V 28/4
			i2++;
		}

		/* prepare average filter array */
		float32_t RRaverageFilter[RRAVERAGESIZE] = {};
		float32_t RRaverageValue = 1.0f/(float32_t)RRAVERAGESIZE;
		for(uint32_t i1 = 0; i1<(uint32_t)RRAVERAGESIZE; i1++)
		{
			RRaverageFilter[i1] = RRaverageValue;
		}

		/* apply moving average filter with convolution */
		uint32_t RRfftCutoffSize = ((uint32_t)(RRCUTOFF2-RRCUTOFF1));;
		uint32_t RRaverageSize = (uint32_t)RRAVERAGESIZE;
		float32_t RRconv[(RRCUTOFF2-RRCUTOFF1)+RRAVERAGESIZE] = {};
		arm_conv_f32(RRfftOutputCutoff,RRfftCutoffSize,RRaverageFilter,RRaverageSize,RRconv);

		/* remove access convolution outputs */
		float32_t RRave[(RRCUTOFF2-RRCUTOFF1)] = {};
		for(uint32_t i1 = RRaverageSize/2, i2 = 0; i1<RRfftCutoffSize + (RRaverageSize/2); i1++)
		{
			RRave[i2] = RRconv[i1];
			i2++;
		}

		/* isolate dominant/peak values + calculating sum of peak values */
		float32_t RRpower = 0.0f;
		float32_t wbnLevel = 0.05f;
		float32_t RRpeak[RRCUTOFF2-RRCUTOFF1] = {0.0f};

		for(uint32_t i1 = 0; i1<(uint32_t)(RRCUTOFF2-RRCUTOFF1); i1++)
		{
			RRpeak[i1] = RRfftOutputCutoff[i1]-RRave[i1];
		}

		float32_t RRpeakMax = 0.0f;
		uint32_t RRpeakMaxIndex = 0;
		arm_max_f32(RRpeak,(RRCUTOFF2-RRCUTOFF1),&RRpeakMax,&RRpeakMaxIndex);

		for(uint32_t i1 = 0; i1<(uint32_t)(RRCUTOFF2-RRCUTOFF1); i1++)
		{
			if (RRpeak[i1]<(float32_t)0)
			{
				RRpeak[i1] = wbnLevel; // V 6/5
			}
			else
			{
				RRpeak[i1] = (float32_t)(RRpeak[i1]);
			}
			RRpower = RRpower + RRpeak[i1];
		}

		/* entropy calculation */
		float32_t RRp[(RRCUTOFF2-RRCUTOFF1)] = {};
		for(uint32_t i1 = 0; i1<(uint32_t)(RRCUTOFF2-RRCUTOFF1); i1++)
		{
			float32_t temp = 0.0f;
			RRp[i1] = RRpeak[i1]/(RRpower+2.22044604925031e-16f);
			temp = RRetpRaw[RRcount] + (RRp[i1]*(logf(RRp[i1]+2.22044604925031e-16f))*(-1.0f));
			RRetpRaw[RRcount] = temp;
		}
		RRcount++;
	}

	for(uint32_t i1 = 0; i1<(uint32_t)RRETP5; i1++)
	{
		RRetp5Seconds[i1] = RRetpRaw[i1];
	}

}

float32_t rr_algo(void)
{
	float32_t RR = 0;

	/* prepare smoothing array */
	uint32_t RRetp15 = (uint32_t)RRETP15;
	uint32_t RRsmoothsize = 8;
	float32_t RRsmoothValue;
	RRsmoothValue = (1.0f/(float32_t)RRsmoothsize);
	float32_t RRsmoothFilter[30] = {};
	for(uint32_t i1 = 0; i1<(uint32_t)RRsmoothsize; i1++)
	{
		RRsmoothFilter[i1] = RRsmoothValue;
	}

	/* apply moving average filter (smoothing) with convolution */
	float32_t RRetpConv[RRETP15 + 30] = {};
	arm_conv_f32(RRetp15Seconds,RRetp15,RRsmoothFilter,RRsmoothsize,RRetpConv);

	float32_t RRetp15Convfilt[RRETPCLEAN15] = {0.0f};
	/* remove access convolution outputs, then the values are inversed */
	for(uint32_t i1 = ((uint32_t)RRsmoothsize/2) + (uint32_t)RRSMOOTHCLEAN, i2 = 0; i1<(uint32_t)RRETP15 + ((uint32_t)RRsmoothsize/2) - (uint32_t)RRSMOOTHCLEAN; i1++)
	{
		RRetp15Convfilt[i2] = RRetpConv[i1] * (-1.0f);
		i2++;
	}

	/* calculate the maximum of RRetp */
	uint32_t RRetpSize = (uint32_t)RRETPCLEAN15;
	float32_t RRetpMax = 0.0f;
	uint32_t RRetpMaxindex = 0;
	arm_max_f32(RRetp15Convfilt,RRetpSize,&RRetpMax,&RRetpMaxindex);

	/* calculate the minimum of RRetp */
	float32_t RRetpMin = 0.0f;
	uint32_t RRetpMinindex = 0;
	arm_min_f32(RRetp15Convfilt,RRetpSize,&RRetpMin,&RRetpMinindex);

	/* calculate the absolute of RRetpMin */
	float32_t etpMinC = 0.0f;
	etpMinC = fabsf(RRetpMin);

	/* move the RRetp15Seconds above zero */
	for(uint32_t i1 = 0; i1<(uint32_t)RRETPCLEAN15; i1++)
	{
		float32_t temp = 0.0f;
		temp = RRetp15Convfilt[i1];
		RRetp15Convfilt[i1] = temp + etpMinC + 2.2e-16f;
	}

	/* prepare input variables for autocorrelation */
	float32_t RRetpCal1[RRETPCLEAN15] = {};
	float32_t RRetpCal2[RRETPCLEAN15] = {};
	float32_t RRtemp[RRETPCLEAN15] = {};

	for(uint32_t i1 = 0; i1<(uint32_t)RRETPCLEAN15; i1++)
	{
		RRetpCal1 [i1] = RRetp15Convfilt[i1];
		RRetpCal2 [i1] = RRetp15Convfilt[i1];
		RRtemp[i1] = RRetp15Convfilt[i1];
	}

	/* apply autocorrelation */
	float32_t RRcorr[RRETPCLEAN15-1] = {};
	float32_t sum_X = 0.0f, sum_Y = 0.0f, sum_XY = 0.0f;
	float32_t sum_X2 = 0.0f, sum_Y2 = 0.0f;
	float32_t squareSum_X = 0.0f, squareSum_Y = 0.0f;
	float32_t nr = 0.0f, nr_1 = 0.0f, nr_2 = 0.0f;
	float32_t dr = 0.0f, dr_1 = 0.0f, dr_2 = 0.0f, sdr_1 = 0.0f, sdr_2 = 0.0f;// dr_3 = 0.0f;
	for(uint32_t i1 = 0, i2 = 0, i3 = 0; i1<(uint32_t)RRETPCLEAN15-1; i1++)
	{
		/* Start of the optimized autocorrelation function */
		sum_X = 0; sum_Y = 0; sum_XY = 0; sum_X2 = 0; sum_Y2 = 0;
		squareSum_X = 0; squareSum_Y = 0;
		nr = 0; nr_1 = 0; nr_2 = 0;
		dr = 0; dr_1 = 0; dr_2 = 0; sdr_1 = 0; sdr_2 = 0;
		i2 = 0;
		i3 = 0;
		RRetpCal1[i1] = 0;
		for (i2 = 1; i2 <(uint32_t)RRETPCLEAN15+1; i2++)
		{
			if (i2<RRETPCLEAN15)
				RRetpCal1[i2] = RRtemp[i3];

			RRtemp[i3] = RRetpCal1[i3];

			// sum of elements of array X.
			sum_X = sum_X + RRetpCal2[i3];

			// sum of elements of array Y.
			sum_Y = sum_Y + RRetpCal1[i3];

			// sum of X[i] * Y[i].
			sum_XY = sum_XY + (RRetpCal2[i3] * RRetpCal1[i3]);

			// sum of square of array elements.
			squareSum_X = squareSum_X + (RRetpCal2[i3] * RRetpCal2[i3]);
			squareSum_Y = squareSum_Y + (RRetpCal1[i3] * RRetpCal1[i3]);

			i3++;
		}

		/* End of the optimized autocorrelation function */
		nr_1 = (float32_t)RRETPCLEAN15 * sum_XY;
		nr_2 = sum_X * sum_Y;
		nr = nr_1 - nr_2;

		sum_X2 = sum_X * sum_X;
		sum_Y2 = sum_Y * sum_Y;

		dr_1 = (float32_t)RRETPCLEAN15 * squareSum_X;
		dr_1 = dr_1 - sum_X2;
		dr_2 = (float32_t)RRETPCLEAN15 * squareSum_Y;
		dr_2 = dr_2 - sum_Y2;
		if( (fabs(dr_1)>0.000001f) && (fabs(dr_2)>0.000001f) && (fabs(nr)>0.000001f) ){
			arm_sqrt_f32(dr_1,&sdr_1);
			arm_sqrt_f32(dr_2,&sdr_2);
			dr = sdr_1 * sdr_2 + 1e-16f;
			dr = 1.0f/dr;
			RRcorr[i1] = nr*dr;
		}
	}

	/* Start of adaptive window (RRscaleLocal) and RRsmoothsize correlation adjustment */
	float32_t RR15tmpMean = 0.0f;
	float32_t RRcorrtmp[128];

	arm_mean_f32(RRcorr,128,&RR15tmpMean);

	for (uint32_t i1 = 0; i1<128; i1++)
		RRcorrtmp[i1] = RRcorr[i1]-RR15tmpMean;

	float32_t RR15tmp[256] = {0.0f};

	/* fill in data according to slots, apply hanning window */
	float32_t RRwin[128] = {0.00000000f,0.00061179f,0.00244567f,0.00549715f,0.00975876f,0.01522007f,0.02186772f,0.02968544f,0.03865409f,0.04875174f,0.05995367f,0.07223246f,0.08555808f,0.09989790f,0.11521684f,0.13147741f,0.14863981f,0.16666206f,0.18550003f,0.20510764f,0.22543690f,0.24643807f,0.26805974f,0.29024901f,0.31295157f,0.33611188f,0.35967324f,0.38357801f,0.40776768f,0.43218306f,0.45676440f,0.48145154f,0.50618408f,0.53090148f,0.55554326f,0.58004912f,0.60435908f,0.62841366f,0.65215399f,0.67552198f,0.69846043f,0.72091321f,0.74282539f,0.76414333f,0.78481486f,0.80478941f,0.82401809f,0.84245384f,0.86005154f,0.87676814f,0.89256273f,0.90739665f,0.92123360f,0.93403973f,0.94578368f,0.95643673f,0.96597280f,0.97436855f,0.98160345f,0.98765978f,0.99252273f,0.99618039f,0.99862382f,0.99984703f,0.99984703f,0.99862382f,0.99618039f,0.99252273f,0.98765978f,0.98160345f,0.97436855f,0.96597280f,0.95643673f,0.94578368f,0.93403973f,0.92123360f,0.90739665f,0.89256273f,0.87676814f,0.86005154f,0.84245384f,0.82401809f,0.80478941f,0.78481486f,0.76414333f,0.74282539f,0.72091321f,0.69846043f,0.67552198f,0.65215399f,0.62841366f,0.60435908f,0.58004912f,0.55554326f,0.53090148f,0.50618408f,0.48145154f,0.45676440f,0.43218306f,0.40776768f,0.38357801f,0.35967324f,0.33611188f,0.31295157f,0.29024901f,0.26805974f,0.24643807f,0.22543690f,0.20510764f,0.18550003f,0.16666206f,0.14863981f,0.13147741f,0.11521684f,0.09989790f,0.08555808f,0.07223246f,0.05995367f,0.04875174f,0.03865409f,0.02968544f,0.02186772f,0.01522007f,0.00975876f,0.00549715f,0.00244567f,0.00061179f,0.00000000f};
	for (uint32_t i1 = 0, i2 = 0, i3 = 0; i1<128; i1++)
	{
		RR15tmp[i2] = RRcorrtmp[i1]*RRwin[i3];
		i2++;
		i3++;
		RR15tmp[i2] = 0.0f;
		i2++;
	}

	/* process the data through the CFFT/CIFFT module */
	uint8_t RRifftFlag = 0;
	uint8_t RRdoBitReverse = 1;
	arm_cfft_f32(&arm_cfft_sR_f32_len128, RR15tmp, RRifftFlag, RRdoBitReverse);

	/* process the data through the Complex Magnitude Module for calculating the magnitude at each bin */
	arm_cmplx_mag_f32(RR15tmp, RR15tmp, 128);

	float32_t RR15tmpPeaks = 0.0f;
	uint8_t RR15tmpPeaksIndex = 0;
	arm_max_f32(RR15tmp,10,&RR15tmpPeaks,(uint32_t*) &RR15tmpPeaksIndex);
	uint32_t RRscaleLocal = 14;

	/* The beginning of adaptive window */
	if (RR15tmpPeaksIndex < 3)
	{
		RRscaleLocal = 18;
		RRsmoothsize = 30;
	}
	else if (RR15tmpPeaksIndex < 4)
	{
		RRscaleLocal = 14;
		RRsmoothsize = 18;
	}
	else
	{
		RRscaleLocal = 10;
		RRsmoothsize = 6;
	}

	RRsmoothValue = (1.0f/(float32_t)RRsmoothsize);
	for(uint32_t i1 = 0; i1<(uint32_t)RRsmoothsize; i1++)
	{
		RRsmoothFilter[i1] = RRsmoothValue;
	}
	/* End of adaptive window (RRscaleLocal) and RRsmoothsize correlation adjustment */

	/* Smooth correlation coefficient plot */
	float32_t RRcorrConv[RRETPCLEAN15+30-1];
	arm_conv_f32(RRcorr,RRETPCLEAN15-1,RRsmoothFilter,RRsmoothsize,RRcorrConv);

	//Clean the front and back of the smoothing
	float32_t RRcorrConvNew[RRETPCLEAN15-1] = {0.0f};
	for(uint32_t i1 = RRsmoothsize/2, i2 = 0; i1<(uint32_t)RRETPCLEAN15-1+RRsmoothsize/2; i1++)
	{
		RRcorrConvNew[i2] = RRcorrConv[i1];
		RRcorrConvNew[i2] = fround(RRcorrConvNew[i2],NDECIMAL);
		i2++;
	}

	float32_t RRcorrMaxLocal[RRETPCLEAN15/10] = {0.0f}; //Denominator is selected at the smallest adaptive window size
	uint32_t RRcorrMaxLocalindex[RRETPCLEAN15/10] = {0}; //Denominator is selected at the smallest adaptive window size
	uint32_t RRcorrNewMaxLocalindex[RRETPCLEAN15/10] = {0}; //Denominator is selected at the smallest adaptive window size
	uint32_t RRcorrMaxLocalPeakindex[4] = {0}; //Size is capped at 4 to consider the first four global peaks

	/* Find local maximum at each window size of HRminspace (window size is tunable) */ // V 28/4
	for(uint32_t i1 = 0; i1<(uint32_t)(RRETPCLEAN15/RRscaleLocal); i1++)
	{
		arm_max_f32(&RRcorrConvNew[i1*RRscaleLocal],RRscaleLocal,&RRcorrMaxLocal[i1],&RRcorrMaxLocalindex[i1]);
		RRcorrMaxLocal[i1] = fround(RRcorrMaxLocal[i1],NDECIMAL);
	}


	for(uint32_t i1 = 0; i1<(uint32_t)(RRETPCLEAN15/RRscaleLocal); i1++)
	{
		RRcorrNewMaxLocalindex[i1] = i1*RRscaleLocal + RRcorrMaxLocalindex[i1];
	}

	/* Find peaks among the local maxima */ // V 29/4
	for(uint32_t i1 = 0; i1<(uint32_t)4; i1++)
	{
		RRcorrMaxLocalPeakindex[i1] = 0;
	}

	for(uint32_t i1 = 1, i2 = 0; i1<(uint32_t)(RRETPCLEAN15/RRscaleLocal)-1; i1++)
	{
		if ((RRcorrMaxLocal[i1] > RRcorrMaxLocal[i1-1]) && (RRcorrMaxLocal[i1] > RRcorrMaxLocal[i1+1]))
		{
			RRcorrMaxLocalPeakindex[i2] = RRcorrNewMaxLocalindex[i1];
			i2++;
		}
	}

	/* Calculate RR and  */
	float32_t RRCaltemp[3] = {0.0f};
	uint32_t numRRreading = 3;
	float32_t time_step = (float32_t)(RRDATASIZE/FS*3)/(float32_t)(RRETPCLEAN15-2);
	float32_t RRmax = 40.0f;
	float32_t RRmin = 4.0f;

	for(uint32_t i1 = 0; i1<(uint32_t)3; i1++)
	{
		if (RRcorrMaxLocalPeakindex[0] < 1)
		{
			RRCaltemp[i1] = 0;
			numRRreading = 1;
			break;
		}
		else if ((RRcorrMaxLocalPeakindex[0] > 0) && (RRcorrMaxLocalPeakindex[1] < 1))
		{
			RRCaltemp[i1] = 60.0f/((float32_t)((RRcorrMaxLocalPeakindex[0])+1)*time_step);
			numRRreading = 1;
			break;
		}
		else
		{
			RRCaltemp[i1] = 60.0f/((float32_t)(RRcorrMaxLocalPeakindex[i1+1]-RRcorrMaxLocalPeakindex[i1])*time_step); // 0.064 sec corresponding to 256 data samples
		}

		if (isnan(RRCaltemp[i1]) || (RRCaltemp[i1] < RRmin) || (RRCaltemp[i1] > RRmax)) //excluding erroneous RR, RR < 3 bpm and RR > 30 bpm
		{
			RRCaltemp[i1] = 0.0f;
			numRRreading = numRRreading - 1;
		}
	}

	float32_t RRmean = 0.0f;
	RRmean = (RRCaltemp[0]+RRCaltemp[1]+RRCaltemp[2])/(float32_t)numRRreading;

	if ((RRmean < RRmin) || isnan(RRmean) || (RRmean > RRmax))
	{
		RR = -2.0f; //To avoid confusion with the IMU, -2 indicates poor reading
	}
	else
	{
		RR = RRmean;
	}

	return RR;
}
