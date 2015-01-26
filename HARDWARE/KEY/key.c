#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


void KEY_Init(void) //IO��ʼ��
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//init GPIOC.6,GPIOC.7 ,GPIOC.8  ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//��������GPIO_Mode_IPU;														//��������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//�������ɿ���־	
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		delay_ms(10);
		if(KEY1==0)							//���� ���� ��
		{
			return 1;
		}//���� ���� ��
		else if(KEY2==0)				//���� ENTER ��
		{
			return 2;
		}//���� ENTER ��
		else if(KEY3==0)				//����BACK��
		{
			return 3;
		}//����BACK��
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1)
		key_up=1; 
	delay_ms(50);
	return 0;// �ް�������
}
