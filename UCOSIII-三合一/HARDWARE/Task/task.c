#include "task.h"
#include "tim2.h"
#include "free.h"
#include "led.h"
#include "usart.h"
#include "ad5933.h"
int id;
int id1;
// 用于使任务按一定频率运行的循环
void task_frequency_control_main_loop(void)
{
	while(1)
	{
		if(TIM2_TASK_COUNTER_DATA.COUNTER_1MS >= 1)
		{
			task_1000Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_1MS = 0;
		}
		
		if(TIM2_TASK_COUNTER_DATA.COUNTER_2MS >= 2)
		{
			task_500Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_2MS = 0;
		}	
		
		if(TIM2_TASK_COUNTER_DATA.COUNTER_5MS >= 5)
		{
			task_200Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_5MS = 0;
		}	

		if(TIM2_TASK_COUNTER_DATA.COUNTER_10MS >= 10)
		{
			task_100Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_10MS = 0;
		}

		if(TIM2_TASK_COUNTER_DATA.COUNTER_20MS >= 20)
		{
			task_50Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_20MS = 0;
		}

		if(TIM2_TASK_COUNTER_DATA.COUNTER_50MS >= 50)
		{
			task_20Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_50MS = 0;
		}

		if(TIM2_TASK_COUNTER_DATA.COUNTER_100MS >= 100)
		{
			task_10Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_100MS = 0;
		}

		if(TIM2_TASK_COUNTER_DATA.COUNTER_200MS >= 200)
		{
			task_5Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_200MS = 0;
		}

		if(TIM2_TASK_COUNTER_DATA.COUNTER_500MS >= 500)
		{
			task_2Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_500MS = 0;
		}		
		
		if(TIM2_TASK_COUNTER_DATA.COUNTER_1000MS >= 1000)
		{
			task_1Hz();
			TIM2_TASK_COUNTER_DATA.COUNTER_1000MS = 0;
		}	
	}
}


void task_1000Hz(void)
{
	  DA5933_Get_Rs();
		//deal_digital();
 

}


void task_500Hz(void)
{
	deal_digital();
}


void task_200Hz(void)
{
 
}


void task_100Hz(void)
{

}


void task_50Hz(void)
{
	
}


void task_20Hz(void)
{
	
}


void task_10Hz(void)
{
	
}


void task_5Hz(void)
{
	
}


void task_2Hz(void)
{
	
}


void task_1Hz(void)
{
	
}

