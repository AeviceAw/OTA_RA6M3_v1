/*
 * vitalsHandler.c
 *
 *  Created on: 13 Aug 2024
 *      Author: yyang
 */

#include "initialisations.h"
#include <arm_math.h>
#include <Algorithm/DSP/ADPCM_Algo.h>
#include <Algorithm/DSP/Vitals/rr_algo.h>
#include <Algorithm/DSP/Vitals/hr_algo.h>
#include <Algorithm/DSP/KMDC/BPF.h>
#include <Algorithm/DSP/KMDC/extractFea.h>
#include <Algorithm/DSP/KMDC/getGroupScore.h>
#include <Algorithm/DSP/KMDC/getCP.h>
#include <Algorithm/dataHandler.h>


//float g_kmdc_threshold = 0.61f;// FIXME: YY set lower 19-Aug-2024 To CWD
float g_kmdc_threshold = 0.41f;//default threshold
//float g_kmdc_threshold = 0.55f;// FIXME: YY set higher 2-Aug-2024

//extern float hr;
//extern float rr;
extern float g_wz;
//extern float batt;
//extern float bp;
//extern uint8_t g_ble; // defined in uart_thread_entry()
//extern uint32_t uart_ok; // defined in uart_thread_entry.c
//extern float WZ_aus;

// ===============================================
// Variables for computation of HR/RR/WZ
// ===============================================
float32_t RRetp5Seconds[RRETP5] ;//= {};
float32_t RRetp15Seconds[RRETP15] ;//= {};
uint8_t count_poor_hr ;//= 0;
uint8_t count_up_hr ;//= 0;
uint8_t count_down_hr ;//= 0;
uint32_t count_hr ;//= 0;
uint32_t count_rr ;//= 0;
float32_t HRtmp[3] ;//= {0.0f};
float32_t RRtmp[3] ;//= {0.0f};
uint8_t count_poor_rr ;//= 0;
uint8_t count_up_rr ;//= 0;
uint8_t count_down_rr ;//= 0;
uint8_t test_audio ;//= 1;
uint8_t data_ok ;//= 0;
uint32_t record_count ;//= 0;

float32_t hr_ave_record[35];
float32_t snr_record[35];
float32_t HRcorrmean_record[35];
float32_t HRstd_record[35];
float32_t HRcorrstd_record[35];
float32_t hr_record[35];
float32_t rr_record[35];

//extern bool record;
int32_t RRetp15Secondsi[RRETP15] ;//= {};

// =======================================================================
// Functions for Computing HR/RR
// =======================================================================



void display_HR(void)
{
    count_hr++; //This is the counter that will progressively count the iteration cycles from the first time a BLE connection is established

    vitals_set(g_HR,hr_algo());////hr = hr_algo();
    vitals_set(HR_DISPLAY,vitals_get(g_HR));//hr_display = hr;

    //// (NEW) Moving average to match Masimo HR estimation
    float32_t sumHRtmp = 0.0f;
    float32_t aveHR = 0.0f;
    uint8_t numHRtmp = 0;

    /* Compute the average of the past three readings */
    if (count_hr > 3)
    {
        for(uint8_t i = 0; i<3; i++)
        {
            if (HRtmp[i] > 0.0f)
            {
                sumHRtmp = sumHRtmp + HRtmp[i];
                numHRtmp = (uint8_t)(numHRtmp+1);
            } //if (HRtmp[i] > 0.0f)
        } //for(uint8_t i = 0; i<3; i++)

        if (sumHRtmp > 0.0f)
            aveHR = sumHRtmp/numHRtmp; //replace the sum value to average value


        if ((aveHR > 0) && (isnan(aveHR) == 0))
        {
            if (vitals_get(HR_DISPLAY) < 0)//if (hr_display < 0)
            {
                //hr_display = (float)aveHR; // If the current hr_reading is not available due to poor readings, gold the value with the previous average value
                vitals_set(HR_DISPLAY,(float)aveHR);
                count_poor_hr = (uint8_t)(count_poor_hr+1);
                count_down_hr = 0;
                count_up_hr = 0;
            }//if (hr_display < 0)
            else if (vitals_get(HR_DISPLAY) > 1.1*aveHR) //moving average upward trend to avoid extreme spike
            {
                //hr_display = (float)1.1f*aveHR;
                vitals_set(HR_DISPLAY,(float)1.1f*aveHR);
                count_up_hr = (uint8_t)(count_up_hr + 1);
                count_poor_hr = 0;
                count_down_hr = 0;
            }//else if (hr_display > 1.1*aveHR)
            else if (vitals_get(HR_DISPLAY) < 0.9*aveHR) //moving average downward trend to avoid extreme spike
            {
                //hr_display = (float)0.9f*aveHR;
                vitals_set(HR_DISPLAY,(float)0.9f*aveHR);
                count_down_hr = (uint8_t)(count_down_hr + 1);
                count_up_hr = 0;
                count_poor_hr = 0;
            }//else if (hr_display < 0.9*aveHR)
            else
            {
                count_down_hr = 0;
                count_up_hr = 0;
                count_poor_hr = 0;
            } //else
        }//if ((sumHRtmp > 0) && (isnan(sumHRtmp) == 0))


        if (count_poor_hr > RESET_HRCOUNT_LIMT) //Output poor reading as '--' in the hr_display and reset the counter
        {
            //hr_display = -2.0f;
            vitals_set(HR_DISPLAY,-2.0f);
            count_poor_hr = 0;
        } //if (count_poor_hr > 6)

        if (count_down_hr > RESET_HRCOUNT_LIMT) //Substitute to a new reading as there was a consecutive down trend for more than 3 times
        {
            //hr_display = hr;
            vitals_set(HR_DISPLAY,vitals_get(g_HR));
            count_down_hr = 0;
        } //if (count_down_hr > 3)

        if (count_up_hr > RESET_HRCOUNT_LIMT) //Substitute to a new reading as there was a consecutive up trend for more than 3 times
        {
            //hr_display = hr;
            vitals_set(HR_DISPLAY,vitals_get(g_HR));
            count_up_hr = 0;
        } //if (count_up_hr > 3)
    }

    for(uint32_t i1 = 0; i1<2; i1++) {
        HRtmp[i1] =  HRtmp[i1+1]; // shift the value one step before
    }

    //HRtmp[2] = hr_display; //update the latest hr_display value into the storage of HRtmp[3] array
    HRtmp[2] = vitals_get(HR_DISPLAY);//update the latest hr_display value into the storage of HRtmp[3] array

} //void display_HR(void)

void display_RR(void)
{
    count_rr++; //This is the counter that will progressively count the iteration cycles from the first time a BLE connection is established

    vitals_set(g_RR,rr_algo());//rr = rr_algo();
    vitals_set(RR_DISPLAY,vitals_get(g_RR));//rr_display = rr;

    //// Predictive moving average (described in the ICASSP 2022) - Adapted for RR. Implemented in the FDA validation study.
    float32_t sumRRtmp = 0.0f;
    float32_t aveRR = 0.0f;
    uint8_t numRRtmp = 0;

    /* Compute the average of the past three readings */
    if (count_rr > 5) // count_rr < 3, rr_display = -2, so ideally you will want to get three first reading and
        //compare the average, so start from 3rd, 4th, 5th. "5th" onward, compare the next reading
        // with the average.
    {
        for(uint8_t i = 0; i<3; i++)
        {
            if (RRtmp[i] > 0.0f)
            {
                sumRRtmp = sumRRtmp + RRtmp[i];
                numRRtmp = (uint8_t)(numRRtmp+1);
            } //if (RRtmp[i] > 0.0f)
        } //for(uint8_t i = 0; i<3; i++)

        if (sumRRtmp > 0.0f)
            aveRR = sumRRtmp/numRRtmp; //replace the sum value to average value


        if ((aveRR > 0) && (isnan(aveRR) == 0))
        {
            if (vitals_get(RR_DISPLAY) < 0)//if (rr_display < 0)
            {
                // average value of the past three readings
                //rr_display = (float)aveRR; // If the current rr_reading is not available due to poor readings, display the
                vitals_set(RR_DISPLAY,(float)aveRR);
                count_poor_rr = (uint8_t)(count_poor_rr+1);
                count_down_rr = 0;
                count_up_rr = 0;
            }//if (rr_display < 0)
            else if (vitals_get(RR_DISPLAY) > 1.1*aveRR) //moving average upward trend to avoid extreme spike
            {
                //rr_display = (float)1.1f*aveRR;
                vitals_set(RR_DISPLAY,(float)1.1f*aveRR);
                count_up_rr = (uint8_t)(count_up_rr + 1);
                count_poor_rr = 0;
                count_down_rr = 0;
            }//else if (rr_display > 1.1*aveRR)
            else if (vitals_get(RR_DISPLAY) < 0.9*aveRR) //moving average downward trend to avoid extreme spike
            {
                //rr_display = (float)0.9f*aveRR;
                vitals_set(RR_DISPLAY,(float)0.9f*aveRR);
                count_down_rr = (uint8_t)(count_down_rr + 1);
                count_up_rr = 0;
                count_poor_rr = 0;
            }//else if (rr_display < 0.9*aveRR)
            else
            {
                count_down_rr = 0;
                count_up_rr = 0;
                count_poor_rr = 0;
            } //else
        }//if if ((aveRR > 0) && (isnan(aveRR) == 0))


        if (count_poor_rr > RESET_RRCOUNT_LIMIT) //Output poor reading as '--' in the rr_display and reset the count_poor_rr
        {
            //rr_display = -2.0f;
            vitals_set(RR_DISPLAY,-2.0f);
            count_poor_rr = 0;
        } //if (count_poor_rr > 6)

        if (count_down_rr > RESET_RRCOUNT_LIMIT) //Substitute to a new reading as there was a consecutive down trend for more than 3 times
        {
            //rr_display = rr;
            vitals_set(RR_DISPLAY,vitals_get(g_RR));
            count_down_rr = 0;
        } //if (count_down_rr > 3)

        if (count_up_rr > RESET_RRCOUNT_LIMIT) //Substitute to a new reading as there was a consecutive up trend for more than 3 times
        {
            //rr_display = rr;
            vitals_set(RR_DISPLAY,vitals_get(g_RR));
            count_up_rr = 0;
        } //if (count_up_rr > 3)
    } //if (count_rr > 5)

    for(uint32_t i1 = 0; i1<2; i1++)
        RRtmp[i1] =  RRtmp[i1+1]; // shift the value one step before

    //RRtmp[2] = rr_display; //update the latest rr_display value into the storage of RRtmp[3] array
    RRtmp[2] = vitals_get(RR_DISPLAY); //update the latest rr_display value into the storage of RRtmp[3] array

} //void display_RR(void)

void RR_entropy_15(void)
{
    /* Calculate RR_Entropy at every 5 second */
    rr_entropy();

    float32_t etptemp[RRETP15] = {};
    //Clear the entropy array it is at the beginning of the recording
    if (count_rr == 0)
    {
        for(uint32_t i= 0;i<RRETP15;i++)
            RRetp15Seconds[i] =  0.0f; // empty the 15 sec entropy if count_rr = 0
    }

    // RR 05 sec
    for(uint32_t i= 0;i<RRETP15;i++)
    {
        etptemp[i] =  RRetp15Seconds[i];
    }
    // RR 10 sec - (i: 79-236, ii = 0:157)
    for(uint32_t i= RRETP5, ii = 0;i<RRETP15;i++)
    {
        RRetp15Seconds[ii] =  etptemp[i]; // slide the latest 10 sec entropy data backward
        ii++;
    }
    // RR 15 sec - (i: 158-236, ii = 0:78)
    for(uint32_t i= (RRETP15-RRETP5), ii = 0;i<RRETP15;i++)
    {
        RRetp15Seconds[i] =  RRetp5Seconds[ii]; //Update the last 5 sec entropy with new 5 sec entropy calculated from r_entropy()
        ii++;
    }

}// end void RR_entropy_15()



void Set_KMDC_Threshold(float inKMDC_Threshold) {
    g_kmdc_threshold = inKMDC_Threshold;

//  char debugStr[128];
//  snprintf(debugStr, sizeof(debugStr) - 1, "Updated KMDC Threshold: %0.2f", g_kmdc_threshold);
//  DEBUG_LOG(0, debugStr);
}

float Get_KMDC_Threshold(void) {
//  char debugStr[128];
//  snprintf(debugStr, sizeof(debugStr) - 1, "g_kmdc_threshold: %0.2f", g_kmdc_threshold);
//  DEBUG_LOG(0, debugStr);

    return g_kmdc_threshold;
}

/*
 * ********************************************************************
 * @brief Handler function to Perform KMDC Computation on each 5-sec
 *        sensor recording, i.e., "fdata"
 *
 *  @param float32_t* fdata   Src Pointer to input  5-sec "fdata" array
 *         float32_t   b_fv   Coefficient Matrix of the features
 *         float32_t   wz     KMDC results of WZ classification
 *
 *  @return wz
 *
 *  @example getKMDC();
 *  *******************************************************************
 */

/*
*  Training Results from MATLAB Data Augment 6
*  Thres: 0.250, NB: 0.323, WZ: 0.998
*  Thres: 0.300, NB: 0.455, WZ: 0.996
*  Thres: 0.350, NB: 0.593, WZ: 0.990
*  Thres: 0.400, NB: 0.727, WZ: 0.981
*  Thres: 0.440, NB: 0.803, WZ: 0.965 <---
*  Thres: 0.450, NB: 0.828, WZ: 0.958
*  Thres: 0.460, NB: 0.846, WZ: 0.949
*  Thres: 0.470, NB: 0.861, WZ: 0.942
*  Thres: 0.480, NB: 0.861, WZ: 0.942
*  Thres: 0.490, NB: 0.876, WZ: 0.928
*  Thres: 0.500, NB: 0.902, WZ: 0.916
*  Thres: 0.510, NB: 0.914, WZ: 0.898
*  Thres: 0.520, NB: 0.927, WZ: 0.877
*  Thres: 0.530, NB: 0.927, WZ: 0.877
*  Thres: 0.540, NB: 0.934, WZ: 0.856
*/
/*
*  Training Results from MATLAB Data Augment 7
*  Thres: 0.250, NB: 0.371, WZ: 0.999
*  Thres: 0.300, NB: 0.548, WZ: 0.994
*  Thres: 0.350, NB: 0.682, WZ: 0.982
*  Thres: 0.400, NB: 0.785, WZ: 0.962
*  Thres: 0.410, NB: 0.806, WZ: 0.957 <---
*  Thres: 0.420, NB: 0.826, WZ: 0.953
*  Thres: 0.430, NB: 0.826, WZ: 0.953
*  Thres: 0.440, NB: 0.833, WZ: 0.946
*  Thres: 0.450, NB: 0.866, WZ: 0.937
*  Thres: 0.460, NB: 0.881, WZ: 0.927
*  Thres: 0.470, NB: 0.907, WZ: 0.918
*  Thres: 0.480, NB: 0.907, WZ: 0.918
*  Thres: 0.490, NB: 0.912, WZ: 0.908
*  Thres: 0.500, NB: 0.914, WZ: 0.892
*  Thres: 0.550, NB: 0.960, WZ: 0.818
*/
extern float32_t fdata[DATASIZE];
float getKMDC(void){
    float32_t fmu[1] = {0.0f};
    // ---------------------------------------------------------------------------------------------------------------------
    // Demean and Normalize Decoded ADPCM
    // Normalized by mean
    fmu[0] = 0.0f;
    arm_mean_f32((float32_t*)fdata,20480,fmu);
    for (uint32_t idx = 0; idx<DATA_LEN;idx++) {
        fdata[idx] = ((float32_t)fdata[idx])-((float32_t)fmu[0]);
    }

    // Normalized by max
    max_abs((float32_t*)fdata, 20480, fmu);
    for (uint32_t idx = 0; idx<DATA_LEN;idx++) {
        fdata[idx] = ((float32_t)fdata[idx])/((float32_t)fmu[0]+0.000000000001f);
    }
    // ---------------------------------------------------------------------------------------------------------------------

    BPF(fdata,fdata);        //memset(b_fv,0,(size_t)sizeof(float)*NROW*(KFEA+2));
    extractCoef(fdata, b_fv); // Results is poor for "with BPF"
    getCP(); // Conditional Probability of b_fvi.e., P(Cough|b_fv) ; P(Breath|b_fv) ; P(Wheeze|b_fv)
    // Check if b_fv belongs to Grp 1/2/3
    d_results[0]=getGroupScore(1); // Cough
    d_results[1]=getGroupScore(2); // Breath
    d_results[2]=getGroupScore(3); // Wheeze
    g_wz = d_results[2]> Get_KMDC_Threshold() ? 1.0f:0.0f;
    return g_wz;
}

/*
 * ********************************************************************
 * @brief Handler function to compute err difference between
 *        <<d_hat>> & <<d_tru>> and store in <<d_err>>
 *         It also return a sum(|d_err|) to the user
 *  @param float32_t* d_hat   Pointer computed d_results[3] array
 *         float32_t* d_tru   Pointer to TRUE d_results[3] array
 *         float32_t* d_err   Pointer to Error d_results[3] array
 *          uint32_t  len     NumberOfElements in array
 *
 *  @return float32_t err
 *
 *  @example uint32_t d_result_err = d_diff(d_results,d_results_expected,d_results_err,3);
 *  *******************************************************************
 */
float d_diff(float* d_hat, const float* d_tru, float* d_err, uint32_t d_len){
float d_err_sum_loc = 0.0f;

for (uint32_t d_idx=0;d_idx<d_len;d_idx++){
    d_err[d_idx] =((float)fabs( ((float)d_hat[d_idx]) - ((float) d_tru[d_idx])));
    d_err_sum_loc = d_err_sum_loc + d_err[d_idx] ;
} // for d_idx=0:d_len-1

return d_err_sum_loc;

} // d_diff()
