#include "AD5933.h"
#include "iic.h"
#include "usart.h"
#include "free.h"
#include "includes.h"	
/****************************
1v 
//#define AD5933_Correction   9332521.85012f//10k
//#define AD5933_Correction   9315472.05638f//20k
//#define AD5933_Correction   9305268.14938f//30k
//#define AD5933_Correction   9291029.14908f//40k
//#define AD5933_Correction   9267711.0323f//50k
//#define AD5933_Correction   9239111.08778f//60k
#define AD5933_Correction   9217767.7508f//70k
//#define AD5933_Correction   9185985.11768f//80k
//#define AD5933_Correction   9150184.36878f//90k
//#define AD5933_Correction   9137058.0322f//��100k

******************/


//1v 
#define AD5933_Correction   9332521.85012f//10k
//#define AD5933_Correction   9315472.05638f//20k
//#define AD5933_Correction   9305268.14938f//30k
//#define AD5933_Correction   9291029.14908f//40k
//#define AD5933_Correction   9267711.0323f//50k
//#define AD5933_Correction   9239111.08778f//60k
//#define AD5933_Correction   9217767.7508f//70k
//#define AD5933_Correction   9185985.11768f//80k
//#define AD5933_Correction   9150184.36878f//90k
//#define AD5933_Correction   9137058.0322f//��100k


//#define AD5933_Correction 101615461.47044108f//����f
//#define AD5933_Correction 96898667.7215f//����f
//#define AD5933_Correction   9156703.70355f//����f 100khz,1v
#define AD5933_MCLK 16.776  //=536870912/MCLK;

float resistance_mes;
float fuzhi;
long real;
long image;
//����Ϊ��һ��ad��������
void Write_Byte(char nAddr,uint nValue)//nAddr��д���ֽ�nValue
{   
	int nTemp = 0x1A;      // AD5933��Ĭ�ϵ�ַ&д����λ���ͣ�
  IIC_Start();
	 
	IIC_Send_Byte(nTemp);     // ���͵�ַ	
	IIC_Wait_Ack();   
	
	IIC_Send_Byte(nAddr);     // ���͵�ַ�ֽ�
	IIC_Wait_Ack();
	
	IIC_Send_Byte(nValue);	// ���������ֽ�	
	IIC_Wait_Ack();
		
	 IIC_Stop();	// ֹͣ����			
    	return;
}

void SetPointer(char nAddr)  //   ���õ�ַָ��
{          
	int nTemp = 0x1A;      // AD5933��Ĭ�ϵ�ַ&д����λ���ͣ�
	
  IIC_Start(); 
	IIC_Send_Byte(nTemp);     // ���͵�ַ	
	IIC_Wait_Ack();     // �ȴ� ACK		

	IIC_Send_Byte(0xB0);     // ����ָ������1101 0000
	IIC_Wait_Ack();

	IIC_Send_Byte(nAddr);	// ���͵�ַָ��	
	IIC_Wait_Ack();	

	IIC_Stop();	// ֹͣ����			
   	return;
}

int Rece_Byte(char nAddr)//��ȡnAddr�е��ֽڵ�����ֵ
{   
	int nTemp ;    
  SetPointer(nAddr);   //��ַָ��ָ��nAddr
  nTemp=0x1B;      // AD5933��Ĭ�ϵ�ַ&������λ���ߣ�
  IIC_Start(); 

	IIC_Send_Byte(nTemp);     // ���͵�ַ	
	IIC_Wait_Ack();  

	nTemp=IIC_Read_Byte(0);//��һ���ֽ�����		
	IIC_NAck();//����NO_ACK
	
	IIC_Stop();	// ֹͣ����			
	return nTemp;
}


u16 AD5933_Tempter(void)
{
 
// unsigned char Status;  //����״̬
 unsigned int Tm;   //����ʵ�����鲿���¶�
 
 
//   //��λAD5933
//   Write_Byte(0x80,0XB1);
//   Write_Byte(0x81,0X00);
    
   //�����¶Ȳ���
   Write_Byte(0x80,0x93);
    
   //�ȴ�ת�����
//   do
//    {
// 
//     Status=Rece_Byte(0x8F);
//  
//}while(!(Status & 0x01));
   //�����¶ȣ�������Tm��
   Tm=Rece_Byte(0x92);
   Tm<<=8;
   Tm+=Rece_Byte(0x93);
   Tm<<=2;

		return Tm;
}

//float Scale_imp (uint SValue[3],uint IValue[3],uint NValue[2],uint CValue[2],float ki,int Ps);
float resistance[1];
float rads[1];
int 	AD5933_Dat_Re[1];
int 	AD5933_Dat_Im[1];

////ð�ݷ�
//void Maopao_Paixu(float *dat, u16 leng)
//{
//	u16 i,j;
//	float buf;
//	for(j=0;j<leng-1;j++)
//	for(i=0;i<leng-j-1;i++)
//	if(dat[i]>dat[i+1]){
//	buf=dat[i];
//	dat[i]=dat[i+1];
//	dat[i+1]=buf;
//	}
//}


//float Get_resistance(u16 num)
//{
//	u16 i;
//	float navle;
////	Maopao_Paixu(resistance,num);
//	navle=resistance[0];
////	for(i=num/2-num/4;i<num/2+num/4;i++)
////	{
////		navle=(navle+resistance[i])/2;
////	}
//	return navle*AD5933_Correction;
//}


void Fre_To_Hex(float fre,u8 *buf)
{
	u32 dat;
	dat=(536870912/(double)(AD5933_MCLK*1000000))*fre;
	buf[0]=dat>>16;
	buf[1]=dat>>8;
	buf[2]=dat;

}

/*Fre_Begin��ʼƵ�ʣ�Fre_UPƵ��������UP_Num��������OUTPUT_Vatage�����ѹ��Gain����ϵ����SWeep_RepɨƵΪ1�ظ�Ϊ0*/

/*
Fre_Begin 		��ʼƵ�� ��HZ��
Fre_UP				����Ƶ�ʣ�HZ��
UP_Num				��������
OUTPUT_Vatage	�����ѹ
								AD5933_OUTPUT_2V
								AD5933_OUTPUT_1V
								AD5933_OUTPUT_400mV
								AD5933_OUTPUT_200mV
								
Gain					PGA����			
							AD5933_Gain_1
							AD5933_Gain_5
SWeep_Rep			ɨ��ģʽ
							AD5933_Fre_UP 	����Ƶ��
							AD5933_Fre_Rep	�ظ�Ƶ��
*/

//AD5933_Sweep(30000,200,200,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);

float AD5933_Sweep (float Fre_Begin,float Fre_UP,u16 UP_Num,u16 OUTPUT_Vatage,u16 Gain,u16 SWeep_Rep,int type)
{
	int time=0;
	u8 SValue[3], IValue[3], NValue[2], CValue[2];
	u16 buf=0;
	float messure;
	if (time==0)
	{
	Fre_To_Hex(Fre_Begin,SValue);
	Fre_To_Hex(Fre_UP,IValue);
	NValue[0]=UP_Num>>8;
	NValue[1]=UP_Num;
		time=1;
	}
#ifdef AD5933_MCLK_USE_OUT
	buf=OUTPUT_Vatage|Gain|SWeep_Rep|AD5933_OUT_MCLK;
#else
		buf=OUTPUT_Vatage|Gain|SWeep_Rep|AD5933_IN_MCLK;
#endif
	CValue[0]=buf>>8;
	CValue[1]=buf;
	
	messure =Scale_imp(SValue,IValue,NValue,CValue,type);
	return messure;
}

/*SValue[3]��ʼƵ�ʣ�IValue[3]Ƶ��������NValue[2]��������CValue[2]�����֣�ki����ϵ����PsɨƵΪ1�ظ�Ϊ0*/


float Scale_imp (u8 *SValue,u8 *IValue,u8 *NValue,u8 *CValue,int type)
{
	
	int i,AddrTemp,time=0;
OS_ERR err;
	float Rs;
	float messure;
	u8 Gain=((~CValue[0])&0x01)?5:1;
	u8 SWeep_Rep=((CValue[0]&0xF0)==(AD5933_Fre_UP>>8))?1:0;
	u8 Mode=CValue[0]&0x0f;
	long ReadTemp,realArr[3],imageArr[3];
                float magnitude;    
	
  if (time==0)
	{
								
                IIC_Init();    //��ʼ��I2C,Ϊʲô
   
                AddrTemp=0X82; //��ʼ����ʼƵ�ʼĴ���
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,SValue[i]);
                AddrTemp++;
                }               
                AddrTemp=0X85; //��ʼ��Ƶ�������Ĵ���
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,IValue[i]);
                AddrTemp++;
                } 
                AddrTemp=0X88; //��ʼ��Ƶ�ʵ����Ĵ���
                for(i = 0;i <2;i++)
                {
    	Write_Byte(AddrTemp,NValue[i]);
                AddrTemp++;
                } 
    //��ʼ�����ƼĴ�����1011 0001 0000 0000����ģʽ��2V��һ���Ŵ��ڲ�ʱ��                                  
                AddrTemp=0X80; 
//                for(i = 0;i <2;i++)
                {
								Write_Byte(AddrTemp,Mode|(AD5933_Standby>>8));
                AddrTemp++;
								Write_Byte(AddrTemp,CValue[1]);
                AddrTemp++;
                }
                 
                Write_Byte(0x80,Mode|(AD5933_SYS_Init>>8));//���ƼĴ���д���ʼ��Ƶ��ɨ������
								delay_ms(10);
                Write_Byte(0X80,Mode|(AD5933_Begin_Fre_Scan>>8));//���ƼĴ���д�뿪ʼƵ��ɨ������
								time=1;
		}
 while (1)
{
                while(1)
                {
                ReadTemp=Rece_Byte(0x8F);  //��ȡ״̬�Ĵ������DFT�Ƿ����
               //OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); 
                if (ReadTemp&0x02)
                break;
                }                  
                realArr[0]=Rece_Byte(0x94);
                realArr[1]=Rece_Byte(0x95);
                realArr[2]=realArr[0]*0x100+realArr[1];
                
                imageArr[0]=Rece_Byte(0x96);
                imageArr[1]=Rece_Byte(0x97);
                imageArr[2]=imageArr[0]*0x100+imageArr[1];      
								
								rads[0]=atan2(imageArr[2],realArr[2])-0.00143485062;

                
	if (realArr[2]>=0x8000)  //����ʵ����ԭ��(������λ�⣬ȡ����һ)
	{
                realArr[2]^=0xFFFF; 
                realArr[2]^=0x8000; 
								realArr[2]+=1;
                realArr[2]^=0x8000;
	}
	if (imageArr[2]>=0x8000)  //�����鲿��ԭ��(������λ�⣬ȡ����һ)
	{
                imageArr[2]^=0xFFFF; 
                imageArr[2]^=0x8000; 
								imageArr[2]+=1;
                imageArr[2]^=0x8000;
	}
								AD5933_Dat_Re[0]=realArr[2];
								AD5933_Dat_Im[0]=imageArr[2];
                magnitude=sqrt(realArr[2]*realArr[2]+imageArr[2]*imageArr[2]);  //ģֵ����              
                resistance[0]=1/(magnitude*Gain);		 //�迹����
								//��ֵ
//	              fuzhi=magnitude;
//	              real=realArr[2];
//	              image=imageArr[2];
	     
	              switch(type)
								{
									case 1:
									{

										Rs=resistance[0]*AD5933_Correction;
										resistance_mes=Rs;
										
//����Ϊ�ֱ��ʹ���										
//										if(Rs>15000000)
//										{

//										printf("��%03.03fL��,%d)",Rs/1000000,get_ostime());
//											resistance_mes=Rs/1000000;

//										}else if(Rs>1000000)
//										{
//										printf("(%03.03fM��,%d)",Rs/1000000,get_ostime());

//										}else	if(Rs>1000)
//										{
//											printf("(%03.03fK��,%d)",Rs/1000,get_ostime());
//											resistance_mes=Rs;

//										}else if(Rs<1000)
//										{
//											//printf("(%03.03f�� ,%d)",Rs,get_ostime());
//											resistance_mes=Rs;

//										}
										
										
										break;
									}
									default: break;
									
	              }
                ReadTemp=Rece_Byte(0x8F);  //��ȡ״̬�Ĵ������Ƶ��ɨ���Ƿ����
                if (ReadTemp&0x04)
                break;//��
                if (SWeep_Rep==1)
                Write_Byte(0X80,CValue[0]);	//���ƼĴ���д������Ƶ�ʣ�������һ��Ƶ�ʵ�)������
								else
								Write_Byte(0X80,CValue[0]);	//���ƼĴ���д���ظ���ǰƵ�ʵ�ɨ��	
}  


                //Write_Byte(0X80,0XA1);	//�������ģʽ
return messure;
}


//�ײ�������ɣ�������ʹ��

float DA5933_Get_Rs(void)
{
	OS_ERR err;
	float Rs2,re,im;
	CPU_SR_ALLOC();
	//OS_CRITICAL_ENTER();	//�����ٽ���
	
	//Rs=AD5933_Sweep(70000,10000,3,AD5933_OUTPUT_1V,AD5933_Gain_1,AD5933_Fre_UP,res_mes);
	Rs2=AD5933_Sweep(10000,10000,9,AD5933_OUTPUT_1V,AD5933_Gain_1,AD5933_Fre_UP,res_mes);
	OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); 
	//AD5933_Sweep(10000,1,40,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);
	//AD5933_Sweep(10000,1,40,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_Rep);
	//AD5933_Sweep(10000,200,2,AD5933_OUTPUT_200mV,AD5933_Gain_1,AD5933_Fre_Rep);//10kHZ,200mv,�ظ�Ƶ��
	
	//Rs=Get_resistance(0);//ȡ��
//	re=Rs*cos(rads[0]);
//	im=Rs*sin(rads[0]);
	//OS_CRITICAL_EXIT();
	return Rs2;
}


//�õ��迹
//��ȷ���Ƿ�׼ȷ
float DA5933_Get_Cap(float Fre)
{
//	float Cap,dat;
//	
//	AD5933_Sweep(30000,1,40,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);
//	Cap=DA5933_Get_Rs();
//	dat=1/(Cap*30000*2*3.1415926/100000000000000);
//	DA5933_Dat_Cap(30000);
//	
//	return Cap;
	
	float Rs,re,im,cap;
	
//	AD5933_Sweep(30000,200,2,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_Rep);
	AD5933_Sweep(90000,10000,1,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP,2);
	//Rs=Get_resistance(40);
//	re=Rs*cos(rads[0]);
	im=Rs*sin(rads[0]);
	cap=0.9442/(2*3.1415926*im/10000000);
	return cap;
	
	
}

float DA5933_Get_L(float Fre)
{
	float L;
	float Rs,re,im;
	
	AD5933_Sweep(100000,1,20,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP,3);
	//Rs=Get_resistance(20);
//	re=Rs*cos(rads[0]);
	im=Rs*sin(rads[0]);
	L=im*1000/(2*3.1415926);	
	return L;
}


