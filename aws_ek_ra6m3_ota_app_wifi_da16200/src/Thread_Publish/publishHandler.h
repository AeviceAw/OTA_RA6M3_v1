/*
 * publishHandler.h
 *
 *  Created on: 15 Jun 2024
 *      Author: awlir
 */

#ifndef PUBLISHHANDLER_H_
#define PUBLISHHANDLER_H_
#include "initialisations.h"

extern float wz12[BUF_LEN];
extern const char c_aus_val[60][2049]; // FIXME: YY For Testing ONLY

int sumArray(char *array, int size);
void publish_seq_clear(void);
void publish_str_encoded(char* inDname,trueCheck_t inTrueCheck);
void g_generate_aur_ID(char* inBuf, uint8_t inSel);
void publish_state_actuator(trueCheck_t inTrueCheck, dataprocess_state_t inDataprocessState);
void publish_state_actuator_tester(trueCheck_t inTrueCheck, dataprocess_state_t inDataprocessState);

#endif /* PUBLISHHANDLER_H_ */
