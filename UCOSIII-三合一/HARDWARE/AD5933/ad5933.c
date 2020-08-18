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
//#define AD5933_Correction   9137058.0322f//！100k

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
//#define AD5933_Correction   9137058.0322f//！100k


//#define AD5933_Correction 101615461.47044108f//加了f
//#define AD5933_Correction 96898667.7215f//加了f
//#define AD5933_Correction   9156703.70355f//加了f 100khz,1v
#define AD5933_MCLK 16.776  //=536870912/MCLK;

float resistance_mes;
float fuzhi;
long real;
long image;
//以下为第一代ad交流程序
void Write_Byte(char nAddr,uint nValue)//nAddr中写入字节nValue
{   
	int nTemp = 0x1A;      // AD5933的默认地址&写控制位（低）
  IIC_Start();
	 
	IIC_Send_Byte(nTemp);     // 发送地址	
	IIC_Wait_Ack();   
	
	IIC_Send_Byte(nAddr);     // 发送地址字节
	IIC_Wait_Ack();
	
	IIC_Send_Byte(nValue);	// 发送数据字节	
	IIC_Wait_Ack();
		
	 IIC_Stop();	// 停止总线			
    	return;
}

void SetPointer(char nAddr)  //   设置地址指针
{          
	int nTemp = 0x1A;      // AD5933的默认地址&写控制位（低）
	
  IIC_Start(); 
	IIC_Send_Byte(nTemp);     // 发送地址	
	IIC_Wait_Ack();     // 等待 ACK		

	IIC_Send_Byte(0xB0);     // 发送指针命令1101 0000
	IIC_Wait_Ack();

	IIC_Send_Byte(nAddr);	// 发送地址指针	
	IIC_Wait_Ack();	

	IIC_Stop();	// 停止总线			
   	return;
}

int Rece_Byte(char nAddr)//读取nAddr中的字节到返回值
{   
	int nTemp ;    
  SetPointer(nAddr);   //地址指针指向nAddr
  nTemp=0x1B;      // AD5933的默认地址&读控制位（高）
  IIC_Start(); 

	IIC_Send_Byte(nTemp);     // 发送地址	
	IIC_Wait_Ack();  

	nTemp=IIC_Read_Byte(0);//读一个字节数据		
	IIC_NAck();//发送NO_ACK
	
	IIC_Stop();	// 停止总线			
	return nTemp;
}


u16 AD5933_Tempter(void)
{
 
// unsigned char Status;  //保存状态
 unsigned int Tm;   //保存实部，虚部，温度
 
 
//   //复位AD5933
//   Write_Byte(0x80,0XB1);
//   Write_Byte(0x81,0X00);
    
   //启动温度测量
   Write_Byte(0x80,0x93);
    
   //等待转换完成
//   do
//    {
// 
//     Status=Rece_Byte(0x8F);
//  
//}while(!(Status & 0x01));
   //读出温度，保存在Tm中
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

////冒泡法
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

/*Fre_Begin起始频率，Fre_UP频率增量，UP_Num增量数，OUTPUT_Vatage输出电压，Gain增益系数，SWeep_Rep扫频为1重复为0*/

/*
Fre_Begin 		开始频率 （HZ）
Fre_UP				步进频率（HZ）
UP_Num				步进次数
OUTPUT_Vatage	输出电压
								AD5933_OUTPUT_2V
								AD5933_OUTPUT_1V
								AD5933_OUTPUT_400mV
								AD5933_OUTPUT_200mV
								
Gain					PGA增益			
							AD5933_Gain_1
							AD5933_Gain_5
SWeep_Rep			扫描模式
							AD5933_Fre_UP 	递增频率
							AD5933_Fre_Rep	重复频率
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

/*SValue[3]起始频率，IValue[3]频率增量，NValue[2]增量数，CValue[2]控制字，ki增益系数，Ps扫频为1重复为0*/


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
								
                IIC_Init();    //初始化I2C,为什么
   
                AddrTemp=0X82; //初始化起始频率寄存器
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,SValue[i]);
                AddrTemp++;
                }               
                AddrTemp=0X85; //初始化频率增量寄存器
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,IValue[i]);
                AddrTemp++;
                } 
                AddrTemp=0X88; //初始化频率点数寄存器
                for(i = 0;i <2;i++)
                {
    	Write_Byte(AddrTemp,NValue[i]);
                AddrTemp++;
                } 
    //初始化控制寄存器，1011 0001 0000 0000待机模式，2V，一倍放大，内部时钟                                  
                AddrTemp=0X80; 
//                for(i = 0;i <2;i++)
                {
								Write_Byte(AddrTemp,Mode|(AD5933_Standby>>8));
                AddrTemp++;
								Write_Byte(AddrTemp,CValue[1]);
                AddrTemp++;
                }
                 
                Write_Byte(0x80,Mode|(AD5933_SYS_Init>>8));//控制寄存器写入初始化频率扫描命令
								delay_ms(10);
                Write_Byte(0X80,Mode|(AD5933_Begin_Fre_Scan>>8));//控制寄存器写入开始频率扫描命令
								time=1;
		}
 while (1)
{
                while(1)
                {
                ReadTemp=Rece_Byte(0x8F);  //读取状态寄存器检查DFT是否完成
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

                
	if (realArr[2]>=0x8000)  //计算实部的原码(除符号位外，取反加一)
	{
                realArr[2]^=0xFFFF; 
                realArr[2]^=0x8000; 
								realArr[2]+=1;
                realArr[2]^=0x8000;
	}
	if (imageArr[2]>=0x8000)  //计算虚部的原码(除符号位外，取反加一)
	{
                imageArr[2]^=0xFFFF; 
                imageArr[2]^=0x8000; 
								imageArr[2]+=1;
                imageArr[2]^=0x8000;
	}
								AD5933_Dat_Re[0]=realArr[2];
								AD5933_Dat_Im[0]=imageArr[2];
                magnitude=sqrt(realArr[2]*realArr[2]+imageArr[2]*imageArr[2]);  //模值计算              
                resistance[0]=1/(magnitude*Gain);		 //阻抗计算
								//辐值
//	              fuzhi=magnitude;
//	              real=realArr[2];
//	              image=imageArr[2];
	     
	              switch(type)
								{
									case 1:
									{

										Rs=resistance[0]*AD5933_Correction;
										resistance_mes=Rs;
										
//以下为分别发送代码										
//										if(Rs>15000000)
//										{

//										printf("（%03.03fLΩ,%d)",Rs/1000000,get_ostime());
//											resistance_mes=Rs/1000000;

//										}else if(Rs>1000000)
//										{
//										printf("(%03.03fMΩ,%d)",Rs/1000000,get_ostime());

//										}else	if(Rs>1000)
//										{
//											printf("(%03.03fKΩ,%d)",Rs/1000,get_ostime());
//											resistance_mes=Rs;

//										}else if(Rs<1000)
//										{
//											//printf("(%03.03fΩ ,%d)",Rs,get_ostime());
//											resistance_mes=Rs;

//										}
										
										
										break;
									}
									default: break;
									
	              }
                ReadTemp=Rece_Byte(0x8F);  //读取状态寄存器检查频率扫描是否完成
                if (ReadTemp&0x04)
                break;//！
                if (SWeep_Rep==1)
                Write_Byte(0X80,CValue[0]);	//控制寄存器写入增加频率（跳到下一个频率点)的命令
								else
								Write_Byte(0X80,CValue[0]);	//控制寄存器写入重复当前频率点扫描	
}  


                //Write_Byte(0X80,0XA1);	//进入掉电模式
return messure;
}


//底层配置完成，以下是使用

float DA5933_Get_Rs(void)
{
	OS_ERR err;
	float Rs2,re,im;
	CPU_SR_ALLOC();
	//OS_CRITICAL_ENTER();	//进入临界区
	
	//Rs=AD5933_Sweep(70000,10000,3,AD5933_OUTPUT_1V,AD5933_Gain_1,AD5933_Fre_UP,res_mes);
	Rs2=AD5933_Sweep(10000,10000,9,AD5933_OUTPUT_1V,AD5933_Gain_1,AD5933_Fre_UP,res_mes);
	OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); 
	//AD5933_Sweep(10000,1,40,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);
	//AD5933_Sweep(10000,1,40,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_Rep);
	//AD5933_Sweep(10000,200,2,AD5933_OUTPUT_200mV,AD5933_Gain_1,AD5933_Fre_Rep);//10kHZ,200mv,重复频率
	
	//Rs=Get_resistance(0);//取了
//	re=Rs*cos(rads[0]);
//	im=Rs*sin(rads[0]);
	//OS_CRITICAL_EXIT();
	return Rs2;
}


//得到阻抗
//不确定是否准确
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


