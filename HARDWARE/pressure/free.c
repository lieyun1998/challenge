#include "free.h"
#include "usart.h"
#include "ad5933.h"
#include "sys.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
u16 USART2_RX_STA=0;   	 

u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]={0}; 

extern long current_pos;//λ����Ϣ
extern float resistance_mes;//����ֵ


void USART2_IRQHandler(void)
{
	u8 res;	  
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif  
	    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        USART2_RX_STA |= 0x8000;
                        
        USART_ReceiveData(USART2); 
			//  OSIntExit(); 
    }

                if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
                {
										res =USART_ReceiveData(USART2);      
									
								if((USART2_RX_STA&0x8000)==0)
												USART2_RX_BUF[USART2_RX_STA++]=res;								
								       
											if(USART2_RX_STA>14)USART2_RX_STA |= 0x8000;
											USART_ClearITPendingBit(USART2, USART_IT_RXNE);                        
                }		
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��ж�
#endif								
} 


//#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void free_Init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	USART_DeInit(USART2);  //��λ����2
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART3ʱ��
	
 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOB11��GPIOB10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOB10����ΪUSART3	  
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������3
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�  
		
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	
 // USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);   //DMAʹ��
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	

  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);  
}


u16 deal_digital (void)
{
	u8 t;
	u8 i;
	u8 flag;
	float real_move;
  float gross_weight;
  //CPU_SR_ALLOC();

		//OS_CRITICAL_ENTER();
	if( USART2_RX_STA&0x8000)
	{   

		for (i=0;i<13;i++)
		{	
		  if (USART2_RX_BUF[i]==0xfe)
			{
			
			   t=i;
				 i=15;
				 flag=1;
			}
		
		}
		if (flag==1)
		{
			if (t+6<14)
			{
						gross_weight=USART2_RX_BUF[t+6]*256+USART2_RX_BUF[t+7];
				     real_move=current_pos/400;
					 	//printf("(%0.2lfN,%d)",gross_weight/100,get_ostime());
				printf("%0.2lf,%0.3f,%ld\r\n",gross_weight/100,resistance_mes,current_pos);
				//if (gross_weight/100>4.2f)
				//{
				//printf("STOP!!!!-don't move any more \r\n");
				
				//}
				  // OS_CRITICAL_EXIT();
				    //printf("%d",1);
			}
						
		}
					
//	if( USART2_RX_STA&0x8000)
//	{   


//		  if (USART2_RX_BUF[0]==0xfe)
//			{
//			


//						gross_weight=USART2_RX_BUF[6]*256+USART2_RX_BUF[7];
//				     real_move=current_pos/400;
//					 	//printf("(%0.2lfN,%d)",gross_weight/100,get_ostime());
//				printf("%0.2lf,%0.3f,%ld\r\n",gross_weight/100,resistance_mes,current_pos);
//				//if (gross_weight/100>4.2f)
//				//{
//				//printf("STOP!!!!-don't move any more \r\n");
//				
//				//}
//				  // OS_CRITICAL_EXIT();
//				    //printf("%d",1);
//			}
						
		
		                                               
		USART2_RX_STA=0;
	}
	else
	{
//	OS_CRITICAL_EXIT();
	}

return gross_weight;

}


