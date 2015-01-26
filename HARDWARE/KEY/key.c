#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


void KEY_Init(void) //IO初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//init GPIOC.6,GPIOC.7 ,GPIOC.8  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//浮空输入GPIO_Mode_IPU;														//上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		delay_ms(10);
		if(KEY1==0)							//按下 方向 键
		{
			return 1;
		}//按下 方向 键
		else if(KEY2==0)				//按下 ENTER 键
		{
			return 2;
		}//按下 ENTER 键
		else if(KEY3==0)				//按下BACK键
		{
			return 3;
		}//按下BACK键
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1)
		key_up=1; 
	delay_ms(50);
	return 0;// 无按键按下
}
