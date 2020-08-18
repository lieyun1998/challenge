#include "task_manage.h"
#include "iic.h"
//#include "key.h"
#include "usart.h"
#include "ad5933.h"
#include "includes.h"	
char str[30];
extern float rads[200];
extern float fuzhi;
extern long real;
extern long image;
//电容还没测
void Get_Cap(void)
{
	float Cap;

	Cap=DA5933_Get_Cap(10000);
	if(Cap>1000000)
	{
		printf("%03.03fuF",Cap/1000000);//这里需要进行实验
	}else	if(Cap>1000)
	{
		sprintf(str,"%03.03fnF  ",Cap/1000);
	}else if(Cap<1000)
	{
		sprintf(str,"%03.03fpF  ",Cap);
		printf("loading");
		delay_ms(50);
	}
	printf("loading2");
	delay_ms(50);
}


//测电阻
void Get_Rs(void)
{
	float Rs;

	Rs=DA5933_Get_Rs();
	if(Rs>15000000)
	{
	sprintf(str,"%03.03fLΩ  ",Rs/1000000);
	}else if(Rs>1000000)
	{
		sprintf(str,"%03.03fMΩ  ",Rs/1000000);
	}else	if(Rs>1000)
	{
		printf("%03.03fKΩ  ",Rs/1000);
	}else if(Rs<1000)
	{
		printf("%03.03fΩ  ",Rs);
	}
   //printf("loading ");
//	 printf("rads:%f ",rads[0]);
//	 printf("幅值：%f\r\n",fuzhi);
//	printf("real:%ld, ",real);  
//	printf("image:%ld \r\n",image);        

}
