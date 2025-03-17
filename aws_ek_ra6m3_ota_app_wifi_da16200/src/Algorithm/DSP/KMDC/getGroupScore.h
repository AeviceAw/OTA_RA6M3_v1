/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * getGroupScore.h
 *
 *  Created on: 9 Feb 2022
 *      Author: YY
 */

#ifndef GETGROUPSCORE_H_
#define GETGROUPSCORE_H_

/* Include Files */
#include "initialisations.h"

extern const float weig[KFEA];
extern const float mean_cough[KFEA];
extern const float mean_breath[KFEA];
extern const float mean_wheeze[KFEA];
extern const float std_cough[KFEA];
extern const float std_breath[KFEA];
extern const float std_wheeze[KFEA];

/* Function Declarations */
extern float getGroupScore(char Grp);
//void getGroupScoreIntialization(void);


#endif /* GETGROUPSCORE_H_ */
