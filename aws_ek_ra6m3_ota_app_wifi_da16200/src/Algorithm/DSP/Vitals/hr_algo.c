/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * hr_algo.c
 *
 *  Created on: 19-June-2021
 *  Modified log: 30-Aug-2021
 *      Author: Vivian
 *      Remark: Added isnan(HRmean) as the final check and did sanity checks with the Matlab version
 *              (YY 22-Dec) Turn Off On-Body Detection, Changed the bracket position of logf(Val+eps), dr = val + eps
 *              19-June-2021 Added the adaptive HRscalelocal size to cater low and high HR
 *      Reference: hr_algo_v2_0.m (30-Aug-2021)
 */

#include <arm_math.h>
#include <Algorithm/DSP/Vitals/hr_algo.h>
#include "initialisations.h"

/********** --------------------------------------------------------------------------------------- **********/
///* Global variables *///
extern float32_t fdata[20480];
extern float32_t hr_display;
extern uint32_t count_hr;

/********** --------------------------------------------------------------------------------------- **********/
///* Function prototypes *///
int32_t float32_t_to_int32_t (float32_t valuei);
float32_t sqroot(float32_t number);
float32_t fround(float32_t number, uint8_t N);

/********** --------------------------------------------------------------------------------------- **********/

float32_t hr_algo(void)
{


//	g_tic();
	vitals_set(g_HR,0.0f); //    float32_t HR = 0.0f;

    /* prepare variables for the 1st decimation */
    arm_fir_decimate_instance_f32 S;
    uint16_t numTaps = (uint32_t)NUMTAPS;
    uint8_t m = (uint32_t)M1;
    uint32_t HRblockSize = (uint32_t)HRBLOCKSIZE; // 400
    uint32_t HRnumBlocks = (uint32_t)HRDATASIZE/(uint32_t)HRBLOCKSIZE; // 20000/400
    float32_t CoefDeciHR[NUMTAPS] = {-0.00889921f, 0.07369634f, 0.50699909f, 0.50699909f, 0.07369634f,-0.00889921f}; //Wn = 0.5 (FIR filter) //HR_validate
    float32_t StateDeciHR[HRBLOCKSIZE + NUMTAPS - 1] = {0.0f};

    /* initialization function for the 1st decimation */
    arm_fir_decimate_init_f32(&S, numTaps,m,(float32_t*)&CoefDeciHR[0],&StateDeciHR[0],HRblockSize);

    /* conduct the 1st decimation */
    float32_t HRslotData1[HRSLOTDATA1_LEN] = {0.0f}; //to simply things, we choose to use a fixed value HR_verify
    for(uint32_t i1=0; i1 < HRnumBlocks; i1++)
    {
        arm_fir_decimate_f32(&S,&fdata[i1*(HRblockSize)],&HRslotData1[i1*(HRblockSize/m)],HRblockSize);
    }

    /* prepare variables for the 2nd decimation */
    arm_fir_decimate_instance_f32 S2;
    uint16_t numTaps2 = (uint32_t)NUMTAPS2;
    uint8_t m2 = (uint32_t)M2;
    uint32_t HRblockSize2 = (uint32_t)HRBLOCKSIZE2;
    uint32_t HRnumBlocks2 = ((uint32_t)(HRDATASIZE/M1))/(uint32_t)HRBLOCKSIZE2;
    float32_t CoefDeciHR2[NUMTAPS2] = {0.01097376f,-0.09119313f, 0.62646727f, 0.62646727f,-0.09119313f, 0.01097376f}; //Wn = 0.99 (FIR filter) //HR_validate
    float32_t StateDeciHR2[HRBLOCKSIZE2 + NUMTAPS2 - 1] = {0.0f};

    /* initialization function for the 2nd decimation */
    arm_fir_decimate_init_f32(&S2, numTaps2,m2,(float32_t*)&CoefDeciHR2[0],&StateDeciHR2[0],HRblockSize2);

    /* conduct 2nd decimation */
    float32_t HRslotData2[HRSLOTDATA2_LEN] = {0.0f};
    for(uint32_t i1=0; i1 < HRnumBlocks2; i1++)
    {
        arm_fir_decimate_f32(&S2,&HRslotData1[i1*(HRblockSize2)],&HRslotData2[i1*(HRblockSize2/m2)],HRblockSize2);
    }

    /* prepare variables for the 3rd decimation */
    arm_fir_instance_f32 S3;
    uint16_t numTaps3 = (uint32_t)NUMTAPS3;
    uint32_t HRblockSize3 = (uint32_t)HRBLOCKSIZE3;
    uint32_t HRnumBlocks3 = ((uint32_t)(HRSLOTDATA2_LEN))/(uint32_t)HRBLOCKSIZE3;
    float32_t CoefBandPassHR[NUMTAPS3] = {0.00122316f, 0.00137704f,-0.00006579f, 0.00132981f, 0.00121070f,-0.00462319f,-0.00465471f, 0.00189788f,-0.00106822f,-0.00027968f, 0.01622474f, 0.01092751f,-0.01337182f,-0.00607038f,-0.00367961f,-0.03697675f,-0.01224084f, 0.05410896f, 0.02799272f, 0.00181884f, 0.06922582f,-0.01498984f,-0.24252724f,-0.12532741f, 0.28033752f, 0.28033752f,-0.12532741f,-0.24252724f,-0.01498984f, 0.06922582f, 0.00181884f, 0.02799272f, 0.05410896f,-0.01224084f,-0.03697675f,-0.00367961f,-0.00607038f,-0.01337182f, 0.01092751f, 0.01622474f,-0.00027968f,-0.00106822f, 0.00189788f,-0.00465471f,-0.00462319f, 0.00121070f, 0.00132981f,-0.00006579f, 0.00137704f, 0.00122316f};//Wn = 60 Hz to 150 Hz (FIR filter) //GT_08
    float32_t StateBandpassHR[HRBLOCKSIZE3 + NUMTAPS3 - 1] = {0.0f};

    /* Call FIR init function to initialize the instance structure. */
    arm_fir_init_f32(&S3, numTaps3, (float32_t *)&CoefBandPassHR[0], &StateBandpassHR[0], HRblockSize3);
    /* ----------------------------------------------------------------------
     ** Call the FIR process function for every blockSize samples
     ** ------------------------------------------------------------------- */
    float32_t HRslotData3[(HRSLOTDATA3_LEN)] = {0.0f};
    for(uint32_t i1=0; i1 < HRnumBlocks3; i1++)
    {
        arm_fir_f32(&S3,&HRslotData2[i1*(HRblockSize3)],&HRslotData3[i1*(HRblockSize3)],HRblockSize3);
    }

    for(uint32_t i1=0; i1 < HRSLOTDATA3_LEN; i1++)
    {
        HRslotData3[i1] = HRslotData3[i1]*2.0f;
    }


    uint32_t idxHRBlock = 0;
    float32_t HRmaxBlock = 0.0f;
    arm_max_f32(&HRslotData3[0],(HRSLOTDATA3_LEN),&HRmaxBlock,&idxHRBlock);

    // Thresholding GT08
    for(uint32_t i1 = 0, i2 = 0; i1<(uint32_t)(HRSLOTDATA3_LEN); i1++)
    {
        if (HRslotData3[i1] < (0.25f*HRmaxBlock))
        {
            HRslotData3[i1] = 0.0f;
        }
        i2++;
    }


    /* prepare variable inputs for the autocorrelation */
    float32_t HRCal1[(HRSLOTDATA3_LEN)] = {0.0f};
    float32_t HRCal2[(HRSLOTDATA3_LEN)] = {0.0f};
    float32_t HRtemp[(HRSLOTDATA3_LEN)] = {0.0f};
    for(uint32_t i1 = 0; i1<(uint32_t)(HRSLOTDATA3_LEN); i1++)
    {
        HRCal1[i1] = HRslotData3[i1];
        HRCal2[i1] = HRslotData3[i1];
        HRtemp[i1] = HRslotData3[i1];
    }

    float32_t HRcorr[(HRSLOTDATA3_LEN-1)] = {0.0f};
    float32_t sum_X = 0.0f, sum_Y = 0.0f, sum_XY = 0.0f;
    float32_t sum_X2 = 0.0f, sum_Y2 = 0.0f;
    float32_t squareSum_X = 0.0f, squareSum_Y = 0.0f;
    volatile float32_t nr = 0.0f, nr_1 = 0.0f, nr_2 = 0.0f;
    volatile float32_t dr = 0.0f, dr_1 = 0.0f, dr_2 = 0.0f, sdr_1 = 0.0f, sdr_2 = 0.0f;// dr_3 = 0.0f;
    for(uint32_t i1 = 0, i2 = 0, i3 = 0; i1<(uint32_t)(HRSLOTDATA3_LEN-1); i1++)
    {
    	// ===========================================================================================================
    	/* Start of the optimized autocorrelation function */
        HRCal1[i1] = 0;
        sum_X = 0.0f; sum_Y = 0.0f; sum_XY = 0.0f; sum_X2 = 0.0f; sum_Y2 = 0.0f;
        squareSum_X = 0.0f; squareSum_Y = 0.0f;
        nr = 0.0f; nr_1 = 0.0f; nr_2 = 0.0f;
        dr = 0.0f; dr_1 = 0.0f; dr_2 = 0.0f; sdr_1 = 0.0f; sdr_2 = 0.0f;
        i2 = 0;
        i3 = 0;
        for (i2 = 1; i2 <(uint32_t)(HRSLOTDATA3_LEN+1); i2++)
        {
        	if (i2<HRSLOTDATA3_LEN)
        		HRCal1[i2] = HRtemp[i3];

            HRtemp[i3] = HRCal1[i3];

            // sum of elements of array X.
            sum_X = sum_X + HRCal2[i3];

            // sum of elements of array Y.
            sum_Y = sum_Y + HRCal1[i3];

            // sum of X[i] * Y[i].
            sum_XY = sum_XY + (HRCal2[i3] * HRCal1[i3]);

            // sum of square of array elements.
            squareSum_X = squareSum_X + (HRCal2[i3] * HRCal2[i3]);
            squareSum_Y = squareSum_Y + (HRCal1[i3] * HRCal1[i3]);

    		i3++;
        }
        /* End of the optimized autocorrelation function */
        // ===========================================================================================================

        nr_1 = (float32_t)(HRSLOTDATA3_LEN) * sum_XY;
        nr_2 = sum_X * sum_Y;
        nr = nr_1 - nr_2;

        sum_X2 = sum_X * sum_X;
        sum_Y2 = sum_Y * sum_Y;

        dr_1 = (float32_t)(HRSLOTDATA3_LEN) * squareSum_X;
        dr_1 = dr_1 - sum_X2;

        dr_2 = (float32_t)(HRSLOTDATA3_LEN) * squareSum_Y;
        dr_2 = dr_2 - sum_Y2;

        if ((fabs(dr_1)>0.000001f) && (fabs(dr_2)>0.000001f) && (fabs(nr)>0.000001f)){
        	arm_sqrt_f32((float32_t)dr_1,(float32_t*)&sdr_1);
        	arm_sqrt_f32((float32_t)dr_2,(float32_t*)&sdr_2);
        	dr = (float32_t) (sdr_1 * sdr_2 + 1e-16);
        	dr = 1.0f/dr;
        	HRcorr[i1] = nr*dr;
        }
    }

//    g_toc();

    uint32_t HRcorrSize = (uint32_t)(HRSLOTDATA3_LEN-1);
    float32_t HRmax = 160.0f;
    float32_t HRmin = 40.0f;

    /* prepare average filter array */
    float32_t HRaverageFilter[HRAVERAGESIZE] = {0.0f};
    float32_t HRaverageValue = (1.0f/(float32_t)HRAVERAGESIZE);
    for(uint32_t i1 = 0; i1<(uint32_t)HRAVERAGESIZE; i1++)
    {
        HRaverageFilter[i1] = HRaverageValue;
    }

    /* apply moving average filter with convolution */
    uint32_t HRaverageSize = (uint32_t)HRAVERAGESIZE;
    float32_t HRcorrSmoothed[(HRSLOTDATA3_LEN)+(HRAVERAGESIZE)] = {0.0f};
    arm_conv_f32(HRcorr,HRcorrSize,HRaverageFilter,HRaverageSize,HRcorrSmoothed);

    /* remove access convolution outputs */
    for(uint32_t i1 = (HRaverageSize/2), i2 = 0; i1<(HRaverageSize/2)+(HRcorrSize); i1++)
    {
        HRcorr[i2] = HRcorrSmoothed[i1]*((float32_t)HRaverageSize);
        HRcorr[i2] = fround(HRcorr[i2],NDECIMAL);
        i2++;
    }

    /* The beginning of the adaptive window function for HR estimation*/
    /* Compute the FFT of the HRcorr */
    /* Reuse the HRslotData3 to store the FFT values */
    float32_t HRslotData3_Buff[(HRSLOTDATA3_LEN*2)] = {0.0f};
    memset(HRslotData3_Buff,0, (size_t) HRSLOTDATA3_LEN*sizeof(float32_t)*2);
    memset(HRslotData3,0, (size_t) HRSLOTDATA3_LEN*sizeof(float32_t));

    /* fill in data according to slots, apply hanning window */
    float32_t HRwin[1024] = {0.00000000f,0.00000943f,0.00003772f,0.00008487f,0.00015089f,0.00023575f,0.00033947f,0.00046204f,0.00060345f,0.00076370f,0.00094278f,0.00114069f,0.00135742f,0.00159296f,0.00184730f,0.00212043f,0.00241234f,0.00272303f,0.00305247f,0.00340066f,0.00376758f,0.00415322f,0.00455757f,0.00498060f,0.00542231f,0.00588268f,0.00636168f,0.00685931f,0.00737554f,0.00791036f,0.00846373f,0.00903565f,0.00962609f,0.01023503f,0.01086244f,0.01150831f,0.01217260f,0.01285530f,0.01355637f,0.01427579f,0.01501354f,0.01576958f,0.01654388f,0.01733643f,0.01814718f,0.01897611f,0.01982318f,0.02068837f,0.02157164f,0.02247296f,0.02339229f,0.02432960f,0.02528485f,0.02625801f,0.02724904f,0.02825791f,0.02928457f,0.03032899f,0.03139113f,0.03247094f,0.03356839f,0.03468344f,0.03581603f,0.03696614f,0.03813372f,0.03931872f,0.04052109f,0.04174080f,0.04297780f,0.04423204f,0.04550347f,0.04679204f,0.04809772f,0.04942044f,0.05076015f,0.05211681f,0.05349037f,0.05488077f,0.05628797f,0.05771190f,0.05915252f,0.06060976f,0.06208358f,0.06357392f,0.06508073f,0.06660394f,0.06814350f,0.06969935f,0.07127143f,0.07285969f,0.07446406f,0.07608448f,0.07772089f,0.07937323f,0.08104144f,0.08272546f,0.08442521f,0.08614064f,0.08787169f,0.08961828f,0.09138035f,0.09315784f,0.09495067f,0.09675878f,0.09858211f,0.10042057f,0.10227412f,0.10414266f,0.10602614f,0.10792448f,0.10983760f,0.11176545f,0.11370794f,0.11566501f,0.11763657f,0.11962256f,0.12162290f,0.12363751f,0.12566631f,0.12770924f,0.12976621f,0.13183715f,0.13392198f,0.13602061f,0.13813298f,0.14025900f,0.14239859f,0.14455166f,0.14671815f,0.14889796f,0.15109102f,0.15329724f,0.15551654f,0.15774883f,0.15999404f,0.16225207f,0.16452284f,0.16680627f,0.16910226f,0.17141074f,0.17373161f,0.17606480f,0.17841020f,0.18076773f,0.18313730f,0.18551883f,0.18791222f,0.19031739f,0.19273423f,0.19516267f,0.19760261f,0.20005395f,0.20251661f,0.20499049f,0.20747550f,0.20997155f,0.21247853f,0.21499636f,0.21752494f,0.22006418f,0.22261398f,0.22517424f,0.22774487f,0.23032577f,0.23291685f,0.23551799f,0.23812912f,0.24075012f,0.24338091f,0.24602137f,0.24867142f,0.25133094f,0.25399985f,0.25667803f,0.25936540f,0.26206184f,0.26476726f,0.26748155f,0.27020461f,0.27293635f,0.27567664f,0.27842540f,0.28118252f,0.28394789f,0.28672142f,0.28950298f,0.29229249f,0.29508984f,0.29789491f,0.30070761f,0.30352783f,0.30635545f,0.30919039f,0.31203252f,0.31488174f,0.31773794f,0.32060102f,0.32347087f,0.32634737f,0.32923043f,0.33211993f,0.33501576f,0.33791782f,0.34082599f,0.34374016f,0.34666023f,0.34958608f,0.35251761f,0.35545470f,0.35839725f,0.36134513f,0.36429825f,0.36725648f,0.37021973f,0.37318786f,0.37616079f,0.37913838f,0.38212053f,0.38510713f,0.38809807f,0.39109322f,0.39409248f,0.39709574f,0.40010288f,0.40311379f,0.40612836f,0.40914646f,0.41216799f,0.41519284f,0.41822088f,0.42125201f,0.42428611f,0.42732307f,0.43036277f,0.43340509f,0.43644993f,0.43949716f,0.44254668f,0.44559836f,0.44865210f,0.45170777f,0.45476527f,0.45782447f,0.46088526f,0.46394753f,0.46701115f,0.47007603f,0.47314203f,0.47620904f,0.47927695f,0.48234565f,0.48541501f,0.48848492f,0.49155526f,0.49462592f,0.49769679f,0.50076774f,0.50383866f,0.50690944f,0.50997996f,0.51305010f,0.51611975f,0.51918879f,0.52225711f,0.52532458f,0.52839111f,0.53145656f,0.53452082f,0.53758378f,0.54064533f,0.54370534f,0.54676370f,0.54982030f,0.55287502f,0.55592774f,0.55897836f,0.56202675f,0.56507280f,0.56811639f,0.57115742f,0.57419576f,0.57723130f,0.58026393f,0.58329353f,0.58631999f,0.58934320f,0.59236303f,0.59537938f,0.59839213f,0.60140117f,0.60440638f,0.60740765f,0.61040488f,0.61339794f,0.61638672f,0.61937111f,0.62235099f,0.62532627f,0.62829681f,0.63126251f,0.63422327f,0.63717896f,0.64012947f,0.64307470f,0.64601453f,0.64894885f,0.65187756f,0.65480053f,0.65771767f,0.66062886f,0.66353398f,0.66643294f,0.66932562f,0.67221191f,0.67509171f,0.67796490f,0.68083137f,0.68369103f,0.68654375f,0.68938944f,0.69222799f,0.69505928f,0.69788322f,0.70069969f,0.70350859f,0.70630981f,0.70910325f,0.71188880f,0.71466636f,0.71743582f,0.72019708f,0.72295003f,0.72569457f,0.72843060f,0.73115801f,0.73387670f,0.73658657f,0.73928751f,0.74197942f,0.74466221f,0.74733577f,0.75000000f,0.75265480f,0.75530006f,0.75793570f,0.76056161f,0.76317768f,0.76578383f,0.76837996f,0.77096595f,0.77354173f,0.77610719f,0.77866223f,0.78120676f,0.78374068f,0.78626390f,0.78877632f,0.79127785f,0.79376839f,0.79624785f,0.79871613f,0.80117314f,0.80361879f,0.80605299f,0.80847564f,0.81088666f,0.81328595f,0.81567342f,0.81804898f,0.82041255f,0.82276403f,0.82510333f,0.82743037f,0.82974505f,0.83204730f,0.83433702f,0.83661413f,0.83887854f,0.84113017f,0.84336893f,0.84559474f,0.84780751f,0.85000716f,0.85219360f,0.85436676f,0.85652656f,0.85867290f,0.86080571f,0.86292491f,0.86503043f,0.86712217f,0.86920006f,0.87126402f,0.87331398f,0.87534986f,0.87737158f,0.87937906f,0.88137223f,0.88335102f,0.88531534f,0.88726513f,0.88920031f,0.89112080f,0.89302655f,0.89491746f,0.89679348f,0.89865453f,0.90050055f,0.90233145f,0.90414718f,0.90594766f,0.90773283f,0.90950262f,0.91125696f,0.91299578f,0.91471903f,0.91642663f,0.91811852f,0.91979464f,0.92145493f,0.92309931f,0.92472774f,0.92634014f,0.92793646f,0.92951663f,0.93108061f,0.93262832f,0.93415971f,0.93567473f,0.93717331f,0.93865540f,0.94012094f,0.94156987f,0.94300216f,0.94441772f,0.94581653f,0.94719852f,0.94856363f,0.94991183f,0.95124305f,0.95255725f,0.95385438f,0.95513439f,0.95639723f,0.95764286f,0.95887121f,0.96008226f,0.96127596f,0.96245225f,0.96361110f,0.96475246f,0.96587628f,0.96698254f,0.96807117f,0.96914215f,0.97019544f,0.97123098f,0.97224875f,0.97324870f,0.97423080f,0.97519502f,0.97614130f,0.97706963f,0.97797996f,0.97887225f,0.97974649f,0.98060262f,0.98144063f,0.98226047f,0.98306212f,0.98384555f,0.98461073f,0.98535763f,0.98608621f,0.98679646f,0.98748835f,0.98816185f,0.98881693f,0.98945357f,0.99007175f,0.99067144f,0.99125262f,0.99181528f,0.99235937f,0.99288490f,0.99339183f,0.99388015f,0.99434984f,0.99480088f,0.99523325f,0.99564694f,0.99604194f,0.99641822f,0.99677578f,0.99711460f,0.99743466f,0.99773596f,0.99801848f,0.99828222f,0.99852716f,0.99875330f,0.99896061f,0.99914911f,0.99931878f,0.99946961f,0.99960160f,0.99971475f,0.99980904f,0.99988448f,0.99994106f,0.99997878f,0.99999764f,0.99999764f,0.99997878f,0.99994106f,0.99988448f,0.99980904f,0.99971475f,0.99960160f,0.99946961f,0.99931878f,0.99914911f,0.99896061f,0.99875330f,0.99852716f,0.99828222f,0.99801848f,0.99773596f,0.99743466f,0.99711460f,0.99677578f,0.99641822f,0.99604194f,0.99564694f,0.99523325f,0.99480088f,0.99434984f,0.99388015f,0.99339183f,0.99288490f,0.99235937f,0.99181528f,0.99125262f,0.99067144f,0.99007175f,0.98945357f,0.98881693f,0.98816185f,0.98748835f,0.98679646f,0.98608621f,0.98535763f,0.98461073f,0.98384555f,0.98306212f,0.98226047f,0.98144063f,0.98060262f,0.97974649f,0.97887225f,0.97797996f,0.97706963f,0.97614130f,0.97519502f,0.97423080f,0.97324870f,0.97224875f,0.97123098f,0.97019544f,0.96914215f,0.96807117f,0.96698254f,0.96587628f,0.96475246f,0.96361110f,0.96245225f,0.96127596f,0.96008226f,0.95887121f,0.95764286f,0.95639723f,0.95513439f,0.95385438f,0.95255725f,0.95124305f,0.94991183f,0.94856363f,0.94719852f,0.94581653f,0.94441772f,0.94300216f,0.94156987f,0.94012094f,0.93865540f,0.93717331f,0.93567473f,0.93415971f,0.93262832f,0.93108061f,0.92951663f,0.92793646f,0.92634014f,0.92472774f,0.92309931f,0.92145493f,0.91979464f,0.91811852f,0.91642663f,0.91471903f,0.91299578f,0.91125696f,0.90950262f,0.90773283f,0.90594766f,0.90414718f,0.90233145f,0.90050055f,0.89865453f,0.89679348f,0.89491746f,0.89302655f,0.89112080f,0.88920031f,0.88726513f,0.88531534f,0.88335102f,0.88137223f,0.87937906f,0.87737158f,0.87534986f,0.87331398f,0.87126402f,0.86920006f,0.86712217f,0.86503043f,0.86292491f,0.86080571f,0.85867290f,0.85652656f,0.85436676f,0.85219360f,0.85000716f,0.84780751f,0.84559474f,0.84336893f,0.84113017f,0.83887854f,0.83661413f,0.83433702f,0.83204730f,0.82974505f,0.82743037f,0.82510333f,0.82276403f,0.82041255f,0.81804898f,0.81567342f,0.81328595f,0.81088666f,0.80847564f,0.80605299f,0.80361879f,0.80117314f,0.79871613f,0.79624785f,0.79376839f,0.79127785f,0.78877632f,0.78626390f,0.78374068f,0.78120676f,0.77866223f,0.77610719f,0.77354173f,0.77096595f,0.76837996f,0.76578383f,0.76317768f,0.76056161f,0.75793570f,0.75530006f,0.75265480f,0.75000000f,0.74733577f,0.74466221f,0.74197942f,0.73928751f,0.73658657f,0.73387670f,0.73115801f,0.72843060f,0.72569457f,0.72295003f,0.72019708f,0.71743582f,0.71466636f,0.71188880f,0.70910325f,0.70630981f,0.70350859f,0.70069969f,0.69788322f,0.69505928f,0.69222799f,0.68938944f,0.68654375f,0.68369103f,0.68083137f,0.67796490f,0.67509171f,0.67221191f,0.66932562f,0.66643294f,0.66353398f,0.66062886f,0.65771767f,0.65480053f,0.65187756f,0.64894885f,0.64601453f,0.64307470f,0.64012947f,0.63717896f,0.63422327f,0.63126251f,0.62829681f,0.62532627f,0.62235099f,0.61937111f,0.61638672f,0.61339794f,0.61040488f,0.60740765f,0.60440638f,0.60140117f,0.59839213f,0.59537938f,0.59236303f,0.58934320f,0.58631999f,0.58329353f,0.58026393f,0.57723130f,0.57419576f,0.57115742f,0.56811639f,0.56507280f,0.56202675f,0.55897836f,0.55592774f,0.55287502f,0.54982030f,0.54676370f,0.54370534f,0.54064533f,0.53758378f,0.53452082f,0.53145656f,0.52839111f,0.52532458f,0.52225711f,0.51918879f,0.51611975f,0.51305010f,0.50997996f,0.50690944f,0.50383866f,0.50076774f,0.49769679f,0.49462592f,0.49155526f,0.48848492f,0.48541501f,0.48234565f,0.47927695f,0.47620904f,0.47314203f,0.47007603f,0.46701115f,0.46394753f,0.46088526f,0.45782447f,0.45476527f,0.45170777f,0.44865210f,0.44559836f,0.44254668f,0.43949716f,0.43644993f,0.43340509f,0.43036277f,0.42732307f,0.42428611f,0.42125201f,0.41822088f,0.41519284f,0.41216799f,0.40914646f,0.40612836f,0.40311379f,0.40010288f,0.39709574f,0.39409248f,0.39109322f,0.38809807f,0.38510713f,0.38212053f,0.37913838f,0.37616079f,0.37318786f,0.37021973f,0.36725648f,0.36429825f,0.36134513f,0.35839725f,0.35545470f,0.35251761f,0.34958608f,0.34666023f,0.34374016f,0.34082599f,0.33791782f,0.33501576f,0.33211993f,0.32923043f,0.32634737f,0.32347087f,0.32060102f,0.31773794f,0.31488174f,0.31203252f,0.30919039f,0.30635545f,0.30352783f,0.30070761f,0.29789491f,0.29508984f,0.29229249f,0.28950298f,0.28672142f,0.28394789f,0.28118252f,0.27842540f,0.27567664f,0.27293635f,0.27020461f,0.26748155f,0.26476726f,0.26206184f,0.25936540f,0.25667803f,0.25399985f,0.25133094f,0.24867142f,0.24602137f,0.24338091f,0.24075012f,0.23812912f,0.23551799f,0.23291685f,0.23032577f,0.22774487f,0.22517424f,0.22261398f,0.22006418f,0.21752494f,0.21499636f,0.21247853f,0.20997155f,0.20747550f,0.20499049f,0.20251661f,0.20005395f,0.19760261f,0.19516267f,0.19273423f,0.19031739f,0.18791222f,0.18551883f,0.18313730f,0.18076773f,0.17841020f,0.17606480f,0.17373161f,0.17141074f,0.16910226f,0.16680627f,0.16452284f,0.16225207f,0.15999404f,0.15774883f,0.15551654f,0.15329724f,0.15109102f,0.14889796f,0.14671815f,0.14455166f,0.14239859f,0.14025900f,0.13813298f,0.13602061f,0.13392198f,0.13183715f,0.12976621f,0.12770924f,0.12566631f,0.12363751f,0.12162290f,0.11962256f,0.11763657f,0.11566501f,0.11370794f,0.11176545f,0.10983760f,0.10792448f,0.10602614f,0.10414266f,0.10227412f,0.10042057f,0.09858211f,0.09675878f,0.09495067f,0.09315784f,0.09138035f,0.08961828f,0.08787169f,0.08614064f,0.08442521f,0.08272546f,0.08104144f,0.07937323f,0.07772089f,0.07608448f,0.07446406f,0.07285969f,0.07127143f,0.06969935f,0.06814350f,0.06660394f,0.06508073f,0.06357392f,0.06208358f,0.06060976f,0.05915252f,0.05771190f,0.05628797f,0.05488077f,0.05349037f,0.05211681f,0.05076015f,0.04942044f,0.04809772f,0.04679204f,0.04550347f,0.04423204f,0.04297780f,0.04174080f,0.04052109f,0.03931872f,0.03813372f,0.03696614f,0.03581603f,0.03468344f,0.03356839f,0.03247094f,0.03139113f,0.03032899f,0.02928457f,0.02825791f,0.02724904f,0.02625801f,0.02528485f,0.02432960f,0.02339229f,0.02247296f,0.02157164f,0.02068837f,0.01982318f,0.01897611f,0.01814718f,0.01733643f,0.01654388f,0.01576958f,0.01501354f,0.01427579f,0.01355637f,0.01285530f,0.01217260f,0.01150831f,0.01086244f,0.01023503f,0.00962609f,0.00903565f,0.00846373f,0.00791036f,0.00737554f,0.00685931f,0.00636168f,0.00588268f,0.00542231f,0.00498060f,0.00455757f,0.00415322f,0.00376758f,0.00340066f,0.00305247f,0.00272303f,0.00241234f,0.00212043f,0.00184730f,0.00159296f,0.00135742f,0.00114069f,0.00094278f,0.00076370f,0.00060345f,0.00046204f,0.00033947f,0.00023575f,0.00015089f,0.00008487f,0.00003772f,0.00000943f,0.00000000f};
    for (uint32_t i1 = 0, i2 = 0, i3 = 0; i1<1024; i1++)
    {
    	HRslotData3_Buff[i2] = HRcorr[i1]*HRwin[i3];
        i2++;
        i3++;
        HRslotData3_Buff[i2] = 0.0f;
        i2++;
    }

    /* process the data through the CFFT/CIFFT module */
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, HRslotData3_Buff, 0, 1);

    /* process the data through the Complex Magnitude Module for
            calculating the magnitude at each bin */
    arm_cmplx_mag_f32(HRslotData3_Buff, HRslotData3, 1024);

    float32_t HRcorrPeaks = 0.0f;
    uint8_t HRcorrPeaksIndex = 0;
    arm_max_f32(HRslotData3,512,&HRcorrPeaks,(uint32_t*) &HRcorrPeaksIndex);

    uint32_t HRscaleLocal = (uint32_t)(150*(FS/M1/M2)/500); //Block size to calculate maximum: 150 is equivalent to 0.3 sec
    /* The beginning of adaptive window */
    if (count_hr > 0)
    {
        if (HRcorrPeaksIndex < 4)
        {
            HRscaleLocal = (uint32_t)(200*(FS/M1/M2)/500);
        }
        else if (HRcorrPeaksIndex < 7)
        {
            HRscaleLocal = (uint32_t)(175*(FS/M1/M2)/500); //Tuned version on HR-V05
        }
        else
        {
            HRscaleLocal = (uint32_t)(90*(FS/M1/M2)/500);
        }
    } //if (count_hr > 0)

    /* End of the adaptive window function for the estimation of HR */
    float32_t local_pks[26] = {0.0f}; //26 is the largest possible HRcorrSize.HRscaleLocal based on the smallest HRscaleLocal
    uint32_t local_pks_idx_tmp[1] = {0}; //26 is the largest possible HRcorrSize.HRscaleLocal based on the smallest HRscaleLocal
    uint32_t local_pks_idx[26] = {0}; //26 is the largest possible HRcorrSize.HRscaleLocal based on the smallest HRscaleLocal
    float32_t global_pks[4] = {0.0f};
    uint32_t global_pks_idx[4] = {0};

    //Find local maximum at each window size of HRminspace (window size is tunable)
    for(uint32_t i1 = 0, i2 = 0; i1<(uint32_t)(HRcorrSize/HRscaleLocal); i1++)
    {
        arm_max_f32(&HRcorr[i1*HRscaleLocal],HRscaleLocal,&local_pks[i1],&local_pks_idx_tmp[0]);
        local_pks_idx[i1] = i1*HRscaleLocal + local_pks_idx_tmp[0];
        local_pks[i1] = fround(local_pks[i1],NDECIMAL);
        FSP_PARAMETER_NOT_USED(i2);
    }


    for(uint32_t i1 = 1, i2 = 0; i1<(uint32_t)(HRcorrSize/HRscaleLocal)-1; i1++)
    {
        if ((local_pks[i1] > local_pks[i1-1]) && (local_pks[i1] > local_pks[i1+1]))
        {
        	global_pks[i2] = local_pks[i1];
        	global_pks_idx[i2] = local_pks_idx[i1];
            i2++;
        }
        if (i2 > 3)
        	break;
    }

    float32_t HRmaxGlobal = {0.1f}; //26 is the largest possible HRcorrSize.HRscaleLocal based on the smallest HRscaleLocal
    uint32_t HRmaxGlobal_idx = {0}; //26 is the largest possible HRcorrSize.HRscaleLocal based on the smallest HRscaleLocal

    arm_max_f32(global_pks,sizeof(global_pks),&HRmaxGlobal,&HRmaxGlobal_idx);
    uint32_t count_global = 0;

    for(uint32_t i1 = 0; i1<(uint32_t)4; i1++)
    {
    	global_pks[i1] = global_pks[i1]/HRmaxGlobal;

    	// Use a threshold to only account the significant global peaks n the HR estimation
    	if (global_pks[i1] > 0.5)
    	{
    		global_pks_idx[count_global] = global_pks_idx[i1];
    		count_global++;
    	}
    }

    // if the number of valid peaks are less than the first allocation peaks, reset the element to 0
    if (count_global < 4)
    	for(uint32_t i1 = count_global; i1<(uint32_t)4; i1++)
    		global_pks_idx[i1] = 0;

    uint32_t numHRreading = 3;
    float32_t HRCaltemp[3] = {0.0f};
    float32_t time_step = (float32_t)HRPERIOD/((float32_t)(HRcorrSize)-1.0f);

    for(uint32_t i1 = 0; i1<(uint32_t)3; i1++)
    {
		if (global_pks_idx[0] < 1)
		{
			HRCaltemp[i1] = 0;
			numHRreading = 1;
			break;
		}
		else if ((global_pks_idx[0] > 0) && (global_pks_idx[1] < 1))
		{
			HRCaltemp[i1] = 60.0f/((float32_t)((global_pks_idx[0])+1)*time_step);
			numHRreading = 1;
			break;
		}
		else
		{
			HRCaltemp[i1] = 60.0f/((float32_t)(global_pks_idx[i1+1]-global_pks_idx[i1])*time_step);
		}

		if ((HRCaltemp[i1] < HRmin) || (HRCaltemp[i1] > HRmax))
        {
            HRCaltemp[i1] = 0.0f;
            numHRreading = numHRreading - 1;
        }
    }

    float32_t HRmean = 0.0f;
    float32_t HR_high = 75.0f;
    uint32_t count_high = 0;
    uint32_t count_low = 0;
    float32_t sum_high = 0.0f;
    float32_t sum_low = 0.0f;

    HRmean = (HRCaltemp[0]+HRCaltemp[1]+HRCaltemp[2])/(float32_t)numHRreading;

    if (count_hr > 3)
    {
    	for(uint32_t i1 = 0; i1<(uint32_t)3; i1++)
    	{
    		if ((HRCaltemp[i1] > HR_high) && (HRCaltemp[i1] > 0))
    		{
    			count_high = count_high + 1;
    			sum_high = sum_high + HRCaltemp[i1];
    		}

    		if ((HRCaltemp[i1] < HR_high) && (HRCaltemp[i1] > 0))
    		{
    			count_low = count_low + 1;
    			sum_low = sum_low + HRCaltemp[i1];
    		}
    	}
    		if (count_high > count_low)
    			HRmean = sum_high/(((float32_t)count_high)*1.0f);
    		if (count_low > count_high)
    			HRmean = sum_low/(((float32_t)count_low)*1.0f);
    		if ((count_low == count_high) && (hr_display < HR_high))
    			HRmean = sum_low;
    		if ((count_low == count_high) && (hr_display > HR_high))
    			HRmean = sum_high;
    }
//    g_toc();
    if ((HRmean < HRmin) || isnan(HRmean) ||(HRmean > HRmax))
        vitals_set(g_HR,-2.0f);//HR = -2.0f;
    else
        vitals_set(g_HR,HRmean);//HR = HRmean;

    return vitals_get(g_HR);//HR;

}


/*
 * Square Root Computation via Newton Methods
 * Arguments    : float32_t number  		Input for computation
 * Return Type  : float32_t sqrt(number)	Output Square root of "number" via Newton Method
 * Reference    : https://math.mit.edu/~stevenj/18.335/newton-sqrt.pdf
 * Complexity   : O(log(N))
 */
float32_t sqroot(float32_t number){

	float32_t x1,x0=0.0f;

	// Define Initial Guess
	x1 = 0.25f*number;

	// Perform Newton Iteration
	while (fabs(x1-x0)>0.000001f){
		// Update Guess for current iteration
		x0 = x1;

		// Perform Update
		x1 = 0.5f*(x0+number/x0);
	}

	return x1;
} // sqroot()


/*
 * Arguments    : float32_t number  		Input value for computation
 * 				: uint8_t 	N				N decimal point
 * Return Type  : float32_t number			number with N decimal point
 * Reference    : https://stackoverflow.com/questions/30376890/math-round-function-with-variable-number-of-digits
 */
float32_t fround(float32_t number, uint8_t N){

	int32_t tmp_int = 0.0f;
	float32_t tmp_float = 0.0f;

	tmp_int = (int32_t)(number*(pow(10,N)));
	tmp_float = (float32_t)(pow(10,-1*N));

	number = (float32_t)tmp_int*tmp_float;

	return number;
}// fround()

