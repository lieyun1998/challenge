#ifndef __RS485_H
#define __RS485_H			 
								  
#include "system.h"
#include "SysTick.h"	

extern u8 RS485_RX_BUF[2048]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

//模式控制
#define RS485_TX_EN		PDout(7)	//485模式控制.0,接收;1,发送.

u16 CRC_Compute(u8 *puchMsg, u16 usDataLen) ;
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(void);
void RS485_Sending_And_Receiving_Time_Control(void);
void RS485_Send_Weather_Station_Instructions(void);

#endif	   
