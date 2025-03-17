/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * mMfccDiff.c
 *
 *  Created on: 9 Jun 2022
 *      Author: YY
 */

#include "initialisations.h"

/* mfcc_diff Tester
 *
 void main(void)
{
    float mfout_n[26]={-46.234188f,10.605685f,8.020667f,6.104793f,1.684195f,.492015f,1.347760f,2.851517f,.956456f,-0.397115f,.781607f,-0.240248f,1.185285f,-0.817205f,-0.867948f,-0.055032f,.049764f,.500716f,.466704f,.723854f,.431450f,.869236f,-0.084531f,.038574f,.144784f,.371141f};
    float mfout_d[26]={0.0f};
    float mfout_dd[26]={0.0f};
    mfcc_diff(mfout_n,mfout_d, mfout_dd);

    for (int mm=0;mm<26;mm++)
        printf("[%.6f,%.6f,%.6f]\n",mfout_n[mm],mfout_d[mm],mfout_dd[mm]);

    return 0;
}
 *
 */

/* mfcc_diff MATLAB
 function [mfout_d,mfout_dd]=mfccDiff(mfout_n)
%
% [mfout_d,mfout_dd]=mfccDiff(mfout_n)
%
% Calcuate 1st and 2nd Order MFCC Difference
%
% Inputs:
% mfout_n <- (26x1) MFCC
%
% Outputs:
% mfout_d -> (26x1) 1st Order MFCC Difference
%mfout_dd -> (26x1) 2nd Order MFCC Difference
%
% Example
if nargin == 0
    % Band Energy of 'C001_WZ_001.wav' Sunit annotation from NUH Clinical Trial
    mfout_n = [-46.234188;10.605685;8.0206671;6.1047926;1.6841949;0.49201488;1.3477600;2.8515172;0.95645612;-0.39711457;0.78160739;-0.24024782;1.1852854;-0.81720543;-0.86794829;-0.055031739;0.049764019;0.50071579;0.46670401;0.72385383;0.43145037;0.86923581;-0.084530838;0.038573768;0.14478409;0.37114093];
    [mfout_d,mfout_dd]=mfccDiff(mfout_n);
    [mfout_n,mfout_d,mfout_dd]
    return
end % if nargin == 0
%% Main
    d1 = [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,26];
    d2 = [1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25];
    d3 = [3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,26,26];
    d4 = [1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24];
    % mfout_n = 26 x 1
    mfout_d = zeros(26,1); % 1st Order Difference
    mfout_dd = zeros(26,1);% 2nd Order Difference
    for mIdx=1:26
        % M_14:M_26
        mfout_d(mIdx) = (mfout_n(d1(mIdx),:)-mfout_n(d2(mIdx),:)+2.*(mfout_n(d3(mIdx),:)-mfout_n(d4(mIdx),:)))/10;
        % M_27:M_39
        mfout_dd(mIdx)= (mfout_d(d1(mIdx),:)-mfout_d(d2(mIdx),:)+2.*(mfout_d(d3(mIdx),:)-mfout_d(d4(mIdx),:)))/10;
    end % for mIdx=1:26
end % mfccDiff()
 */

/*
* Compute M_14:M_26 and M_27:M:39 Features of KMDC
* Corresponding to 1st Order Difference and 2nd Order Difference of MFCC
* Arguments : float* mfout_n  pointer to input vector of 26 MFCC results
*             float* mfout_d  pointer to output vector of 26 1st Order Difference of MFCC
*             float* mfout_dd pointer to output vector of 26 2nd Order Difference of MFCC
*
* Return Type: null
*/
void mfcc_diff(float* mfout_n,float* mfout_d,float* mfout_dd);
void mfcc_diff(float* mfout_n,float* mfout_d,float* mfout_dd){
    const int d1[26]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,25};
    const int d2[26]={0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    const int d3[26]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,25,25};
    const int d4[26]={0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

    memset(mfout_d,0, (size_t) sizeof(mfout_d));
    // -----------------------------------------------------------
    // M_14:M_26
    for (int mIdx=0;mIdx<26;mIdx++){
        //M_14:M_26
        mfout_d[mIdx]= (mfout_n[d1[mIdx]]-mfout_n[d2[mIdx]]+2*(mfout_n[d3[mIdx]]-mfout_n[d4[mIdx]]))/10;
    }// end for mIdx=1:26
    memset(mfout_dd,0,(size_t)sizeof(mfout_dd));
    // -----------------------------------------------------------
    // M_27:M_39
    for (int mIdx=0;mIdx<26;mIdx++){
        //M_27:M_39
        mfout_dd[mIdx]=(mfout_d[d1[mIdx]]-mfout_d[d2[mIdx]]+2*(mfout_d[d3[mIdx]]-mfout_d[d4[mIdx]]))/10;
    }// end for mIdx=1:26
} // end mfcc_diff()
