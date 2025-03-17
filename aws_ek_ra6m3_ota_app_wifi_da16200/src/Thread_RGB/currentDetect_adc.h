#ifndef CURRENTDETECT_ADC_H_
#define CURRENTDETECT_ADC_H_

//#include "AeviceSystemConfig.h"
//#include "fsp_common_api.h"
//#include "stdbool.h"

#include "initialisations.h"


//#if CURRENT_DETECT_ADC_MODULE

#define DEBUG_CURRENT_DETECT_ADC		1//Default: 0 Disabled
#define TEST_ADC						0//Default: 0 Disabled

void Test_ADC_Current_Detect(void);

void CD_Read_ADC(void);

uint16_t g_adcData_get(void);
float g_adcVolt_get(void);
float g_adc_mA_get(void);

#endif//#if ADC_CURRENT_DETECT_MODULE

//#endif //CURRENTDETECT_ADC_H_
//
