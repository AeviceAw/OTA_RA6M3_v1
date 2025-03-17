/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * frequency_test.h
 *
 *  Created on: 29 Nov 2022
 *      Author: YY
 */

#ifndef ALGO_FREQUENCY_TEST_H_
#define ALGO_FREQUENCY_TEST_H_


extern uint8_t f60 ;
extern uint8_t f150 ;
extern uint8_t f500 ;
extern uint8_t f900 ;
extern uint8_t f1800;

extern float batt;
extern float bp;
uint8_t frequency_test(void);
uint8_t frequency_test_results(void);
uint8_t frequency_test_1024(void);
void gFFT_hamming_NFFT(float *fin, float *Xm);

void frequency_test_init(void);

#endif /* ALGO_FREQUENCY_TEST_H_ */
