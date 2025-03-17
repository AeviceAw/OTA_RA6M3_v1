/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * mBandEtp.c
 *
 *  Created on: 9 Jun 2022
 *      Author: YY
 *
 */


#include "initialisations.h"

/* bandEtp Tester
 int main()
{
    float BandEnergy[9]={1.265276f,0.057720f,0.001885f,0.000224f,0.000125f,0.000017f,0.000005f,0.000050f,0.000057f};
    float Etp = {0.0f};

    Etp = bandEtp(BandEnergy);
    printf("Etp = %f",Etp);
}

 */

/* bandEtp MATLAB
 function Etp=bandEtp(BandEnergy)
%
% Etp=bandEtp(BandEnergy)
%
% Calcuate Band Entropy of the 9 Bands, i.e., M_51 Features of KMDC
%
% Inputs:
% BandEnergy <- (9x1) Energy content of the 9 Bands
%
% Outputs:
% Etp -> (1x1) Entropy Content
%
% Example
if nargin == 0
    % Band Energy of 'C001_WZ_001.wav' Sunit annotation from NUH Clinical Trial
    BandEnergy = [1.26527643203735;0.0577199347317219;0.00188509747385979;0.000223954644752666;0.000124693906400353;1.67081834661076e-05;5.35364870302146e-06;4.97051587444730e-05;5.73193792661186e-05];
    Etp=bandEtp(BandEnergy)
    return
end % if nargin == 0
%% Main
% (a) Find Sum of BandEnergy
bE_sum = 0; %
bT = length(BandEnergy); % BandTotal = 9
for bIdx = 1:bT
    bE_sum = bE_sum + BandEnergy(bIdx);
end % sum(BandEnergy)

% (b) Normalized-BandEnergy
d = zeros(bT,1);
for bIdx=1:bT
    d(bIdx) = BandEnergy(bIdx)/(eps+bE_sum);
end % BandEnergy./sum(BandEnergy)

% (c) Find the Log of  Normalzied-BandEnergy
logd = zeros(bT,1);
for bIdx=1:bT
    logd(bIdx) = log2(d(bIdx)+eps);
end % log_bE_sum = log2(bE_sum +eps);

% (d) Compute entropy
Etp = 0;
for bIdx=1:bT
    Etp = Etp + d(bIdx)*logd(bIdx);
end % -sum()
Etp = Etp*-1;
end % bandEtp()
 */

/*
* Calculate Band Entropy of the 9 Bands, i.e., M_51 Features of KMDC
*
* Arguments : float* BandEnergy  pointer to input vector of 9 Band's Energy Content
*
* Return Float: Entropy of the 9 Band's Energy
*/
float bandEtp(float* BandEnergy);
float bandEtp(float* BandEnergy){
    int bb = 0;           // counter for EnergyBand Index
    const int bE_num = 9; // Total Number of Energy Band
    float bE_sum = 0.0;  // Total Energy in all Band
    float etp = {0.0f};  // Entropy of Band Energy

    float* d ;     // d = 9x1 :  Normalized-BandEnergy
    float* logd ; // Logd = 9x1 : Log of the Normalized-BandEnergy

    d = (float*)calloc((size_t)bE_num, sizeof(float));
    logd = (float*)calloc((size_t)bE_num, sizeof(float));

    // (a) Find Sum of BandWidth
    for(bb=0;bb<bE_num;bb++)
        bE_sum = bE_sum + BandEnergy[bb];

    // (b) Normalized-BandWidth
    for (bb=0;bb<bE_num;bb++){
        d[bb] = BandEnergy[bb]/(bE_sum+0.000000001f);
        //printf("d[%d] = %f\n",bb,d[bb]);
    }
    // (c) Find the Log of the Normalized-BandEnergy
    for (bb=0;bb<bE_num;bb++){
        logd[bb] = (float) log2(d[bb]+0.000000001f);
        //printf("logd[%d] = %f\n",bb,logd[bb]);
    }

    // (d) Compute Entropy
    etp = 0.0f;
    for (bb=0;bb<bE_sum;bb++)
        etp=etp+d[bb]*logd[bb];
    etp=etp*-1;

    return etp;
} // end bandEtp()
