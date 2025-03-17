/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
#ifndef ALGO_ADPCM_ALGO_H_
#define ALGO_ADPCM_ALGO_H_

#include "initialisations.h"

// 100-Hz Data for ADPCM test
extern const float32_t fdata_adpcm[20480];

// Handler for encoding
void doEncodingInit(void);
void doEncoding(float32_t* fIn, uint8_t* eOut, uint32_t eLen);
void doDecoding(uint8_t* eIn,float32_t* fOut, uint32_t eLen);

uint8_t ADPCMEncoder(int16_t sample);
int16_t ADPCMDecoder(uint8_t code);

#endif /* ALGO_ADPCM_ALGO_H_ */
