/*
 * dataHandler.h
 *
 *  Created on: 13 Aug 2024
 *      Author: yyang
 */

#ifndef ALGORITHM_DATAHANDLER_H_
#define ALGORITHM_DATAHANDLER_H_

#include "initialisations.h"


/* Enum to Set/Get vaules from <<HR/RR/WZ_DISPLAY>> or <<HR/RR/WZ_PUBLISH>> */
typedef enum vital_enum{
    g_HR = 0,
    g_RR = 1,
    g_WZ = 2,
    HR_DISPLAY=3,
    RR_DISPLAY=4,
    WZ_DISPLAY=5,
    HR_PUBLISH=6,
    RR_PUBLISH=7,
    WZ_PUBLISH=8
}vital_t;

char* getEncodedAddr(void);

void vitals_compute(void);
float32_t vitals_get(vital_t inType);
void vitals_set(vital_t inType,float32_t inVal);


float testKMDC(void);

void datapreprocess_compute(void);
uint8_t* getComDataAddr(void);
void getData(void);
uint32_t getDataSuccessCount(void);
void getMed(float32_t *fin, uint32_t odr_val);

#endif /* ALGORITHM_DATAHANDLER_H_ */
