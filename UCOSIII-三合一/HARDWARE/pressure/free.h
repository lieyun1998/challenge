#ifndef __FREE_H
#define __FREE_H			 
								  
#include "sys.h"
#include "delay.h"	

#define USART2_MAX_RECV_LEN		12
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 
extern u16 USART2_RX_STA;   

void free_Init(u32 bound);

u16 deal_digital (void);
#endif	   
