/*
 * sramHandler.h
 *
 *  Created on: 2 Aug 2024
 *      Author: yyang
 */

#ifndef SRAMHANDLER_H_
#define SRAMHANDLER_H_

#include "initialisations.h"
extern uint8_t seqAdd[60][3];
extern const uint8_t RDMR;   // Read the Mode Register
extern const uint8_t WRMR;   // Write to the Mode Register
extern const uint8_t READ;   // Byte Read command
extern const uint8_t WRITE;   // Byte Write command
extern const uint8_t RSTIO;   // Reset memory to SPI mode
extern const uint8_t ByteMode;   // Byte mode (read/write one byte at a time)
extern const uint8_t Sequential;   // Sequential mode (read/write blocks of memory)
extern const uint8_t Page;   // Page mode (read/write blocks of memory)
extern  const uint8_t READING;   // Sequential mode (read/write blocks of memory)

// Function Prototype
fsp_err_t g_sram_init(void);
fsp_err_t g_sram_deinit(void);
bool g_sram_test(void);
void payLoad( uint8_t MODE, uint8_t* ADDR,size_t numByte_ADDR,uint8_t* MSG,size_t numByte_MSG);
void prepSEQ(uint8_t seqIdx,uint8_t* MSG); // load data to payloadArr
fsp_err_t writeToSRAM(void); // Write payloadArr to sram
fsp_err_t readFromSRAM(uint8_t seqIdx, uint8_t* dst); //read SRAM to dst


void SRAM_Handle_TransferComplete(uint16_t transferSize);
void SRAM_ChipSelect_Low(void);

#endif /* SRAMHANDLER_H_ */
