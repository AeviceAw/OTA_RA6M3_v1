/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * getCP_YY.c
 *
 *  Created on: 06 Jul 2023
 *      Author: YY
 */

#include "initialisations.h"
#include <Algorithm/DSP/KMDC/getCP_data.h>
#include <Algorithm/DSP/KMDC/extractFea.h>
#include <Algorithm/DSP/KMDC/getGroupScore.h>

/* Function Definitions */

/*
 * [P_cc,P_bc,P_wc]=getConditionalProbability(b_fv)
 *
 *  Generate the conditional probability of [Cough/Breath/Wheeze] given b_fv
 *
 *  Inputs:
 *  b_fv     <- (Samples*[FileIdx,SegIdx, 72features])
 *  edge_cough  <- (72feature * 100 Edges) Edges of Cough  Feature Histogram
 *  edge_breath <- (72feature * 100 Edges) Edges of Breath Feature Histogram
 *  edge_wheeze <- (72feature * 100 Edges) Edges of Wheeze Feature Histogram
 *  density_cough  <- (72feature * 101 Edges) Histogram of Cough  Feature
 *  density_breath <- (72feature * 101 Edges) Histogram of Breath Feature
 *  density_wheeze <- (72feature * 101 Edges) Histogram of Wheeze Feature
 * Arguments    : const float b_fv[NROW*(KFEA+2)]
 *                float P_cc[NROW*(KFEA+2)]
 *                float P_bc[NROW*(KFEA+2)]
 *                float P_wc[NROW*(KFEA+2)]
 * Return Type  : void
 */
void getCP(void)
{
	uint8_t marker = 0;
	volatile float edge_fea = 0.0f;
	volatile float edge_lower=0.0f;
	volatile float edge_upper=0.0f;
	volatile float density_val = 0.0f;
	memset(P_cc,0, NROW*(KFEA+2) * sizeof(float));
	memset(P_wc,0, NROW*(KFEA+2) * sizeof(float));
	memset(P_bc,0, NROW*(KFEA+2) * sizeof(float));
	//------------------------------------------------------
	// P_cc (density features in edge Cough)
    memcpy(P_cc, b_fv, NROW*(KFEA+2) * sizeof(float));
    for(int idxS=0;idxS<NROW;idxS++){
    	for (int idxM=2;idxM<KFEA+2;idxM++){
    		marker = 0;
    		for (int idxK=1;idxK<101;idxK++){
    			edge_fea = P_cc[idxS+(idxM)*NROW];
    			edge_lower=edge_cough[(idxM-2)+(idxK-1)*KFEA];
    			edge_upper=edge_cough[(idxM-2)+(idxK  )*KFEA];
    			if ((edge_fea>=edge_lower) && (edge_fea<=edge_upper)){
    				marker = 1;
    				density_val = density_cough[(idxM-2)+(idxK-1)*KFEA];
    				P_cc[idxS+(idxM)*NROW]=density_val;
    				break;
    			} // end if features inside edge_cough
    		}//end for all the 1:K Edge Column
    		if (marker==0)
    			P_cc[idxS+(idxM)*NROW] = 0.0001f;
    	} // end for idxM=3:K+2 for all the 1:K features
    }// end for idxS=1:NROW Number of Sample

	//------------------------------------------------------
	// P_wc (density features in edge wheeze)
    memcpy(P_wc, b_fv, NROW*(KFEA+2) * sizeof(float));
    for(int idxS=0;idxS<NROW;idxS++){
    	for (int idxM=2;idxM<KFEA+2;idxM++){
    		marker = 0;
    		for (int idxK=1;idxK<101;idxK++){
    			edge_fea = P_wc[idxS+(idxM)*NROW];
    			edge_lower=edge_wheeze[(idxM-2)+(idxK-1)*KFEA];
    			edge_upper=edge_wheeze[(idxM-2)+(idxK  )*KFEA];
    			if ((edge_fea>=edge_lower) && (edge_fea<=edge_upper)){
    				marker = 1;
    				density_val = density_wheeze[(idxM-2)+(idxK-1)*KFEA];
    				P_wc[idxS+(idxM)*NROW]=density_val;
    				break;
    			} // end if features inside edge_wheeze
    		}//end for all the 1:K Edge Column
    		if (marker==0)
    			P_wc[idxS+(idxM)*NROW] = 0.0001f;
    	} // end for idxM=3:K+2 for all the 1:K features
    }// end for idxS=1:NROW Number of Sample

	//------------------------------------------------------
	// P_bc (density features in edge breath)
    memcpy(P_bc, b_fv, NROW*(KFEA+2) * sizeof(float));
    for(int idxS=0;idxS<NROW;idxS++){
    	for (int idxM=2;idxM<KFEA+2;idxM++){
    		marker = 0;
    		for (int idxK=1;idxK<101;idxK++){
    			edge_fea = P_bc[idxS+(idxM)*NROW];
    			edge_lower=edge_breath[(idxM-2)+(idxK-1)*KFEA];
    			edge_upper=edge_breath[(idxM-2)+(idxK  )*KFEA];
    			if ((edge_fea>=edge_lower) && (edge_fea<=edge_upper)){
    				marker = 1;
    				density_val = density_breath[(idxM-2)+(idxK-1)*KFEA];
    				P_bc[idxS+(idxM)*NROW]=density_val;
    				break;
    			} // end if features inside edge_breath
    		}//end for all the 1:K Edge Column
    		if (marker==0)
    			P_bc[idxS+(idxM)*NROW] = 0.0001f;
    	} // end for idxM=3:K+2 for all the 1:K features
    }// end for idxS=1:NROW Number of Sample
} // getCP
