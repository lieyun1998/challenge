/*
 * EBI.h
 *
 * Created: 29/09/2019 22:23:16
 *  Author: ZCheng
 */ 


#ifndef EBI_H_
#define EBI_H_

// include the IIC for AD5933
//#include <Wire.h>
//#include "EBI.h"
//#include "SVEI3.h"

// address of AD5933
#define SLAVE_ADDR 0x0D
#define ADDR_PTR 0xB0
// address of registers
#define START_FREQ_R1 0x82
#define START_FREQ_R2 0x83
#define START_FREQ_R3 0x84
#define FREG_INCRE_R1 0x85
#define FREG_INCRE_R2 0x86
#define FREG_INCRE_R3 0x87
#define NUM_INCRE_R1 0x88
#define NUM_INCRE_R2 0x89
#define NUM_SCYCLES_R1 0x8A
#define NUM_SCYCLES_R2 0x8B
#define RE_DATA_R1 0x94
#define RE_DATA_R2 0x95
#define IMG_DATA_R1 0x96
#define IMG_DATA_R2 0x97
#define TEMP_R1 0x92
#define TEMP_R2 0x93
#define CTRL_REG 0x80
#define CTRL_REG2 0x81
#define STATUS_REG 0x8F


void ebiInitial();
void ebiDetectFast(long, long);
void ebiOnce(short *real, short *image);
void ebiStop();
void ebiSendContinue(long, long);
float ebiMulSamp();
// functions for EBI
void programReg(float, float, int);
void writeData(int addr, int data) ;
int readData(int addr);
float getFrequency(float freq, int n);
char measureTemperature();


#endif /* EBI_H_ */