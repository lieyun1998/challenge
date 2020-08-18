#ifndef _AD5933_H
#define _AD5933_H

#include "sys.h"
#include <math.h>
#include "delay.h"
#define uint unsigned int
#define res_mes 1

//以下为新增

void Write_Byte(char nAddr,uint nValue);
void SetPointer(char nAddr);
int Rece_Byte(char nAddr);
float Scale_imp (u8 *SValue,u8 *IValue,u8 *NValue,u8 *CValue,int type);
u16 AD5933_Tempter(void);
float Get_resistance(u16 num);//？
float AD5933_Sweep (float Fre_Begin,float Fre_UP,u16 UP_Num,u16 OUTPUT_Vatage,u16 Gain,u16 SWeep_Rep,int type);

float DA5933_Get_Rs(void);
float DA5933_Get_Cap(float Fre);
float DA5933_Get_L(float Fre);

#define AD5933_SYS_Init					(1)<<12
#define AD5933_Begin_Fre_Scan		(2)<<12
#define AD5933_Fre_UP						(3)<<12
#define AD5933_Fre_Rep					(4)<<12
#define AD5933_Get_Temp					(9)<<12
#define AD5933_Sleep						(10)<<12
#define AD5933_Standby					(11)<<12

#define AD5933_OUTPUT_2V				(0)<<9
#define AD5933_OUTPUT_1V				(3)<<9
#define AD5933_OUTPUT_400mV			(2)<<9
#define AD5933_OUTPUT_200mV			(1)<<9

#define AD5933_Gain_1						(1)<<8//256
#define AD5933_Gain_5						(0)<<8

#define AD5933_IN_MCLK					(0)<<3
#define AD5933_OUT_MCLK					(1)<<3

#define AD5933_Reset						(1)<<4
#endif

