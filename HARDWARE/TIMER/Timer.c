#include "stm32f10x.h"
#include "Timer.h"

static __IO uint16_t					counter_delay_ms;

void StartTimeOut( uint16_t delay )
{
	/* Set the TimeOut flag to false */
	TimeUp 	 = FALSE;
	/* Clear the IT flag */
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	/* Set the timer counter */
	TIM_SetCounter(TIM3, delay);
  /* Enable the Time out timer */
	TIM_Cmd(TIM3, ENABLE);
}

void StopTimeOut( void )
{	
  /* Disable the Time out timer */
	TIM_Cmd(TIM3, DISABLE);	
}

void Timer_Config( void )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(
								RCC_APB1Periph_TIM3 	|				//溢出时钟
								RCC_APB1Periph_TIM4 	, 			//延时时钟
								ENABLE);
	/* -------------------------------------------------------------------------- 
	 * 72 MHz / 7200 = 10KHz (100祍)
	 * 100祍 * 300 + 100祍 ~= 30ms	
	 * -------------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period 					= 300;     
	TIM_TimeBaseStructure.TIM_Prescaler 			= 7200;       
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;      
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Down;				//向下计数  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Global);			//设置TIM3更新请求源(生成重复脉冲--在更新事件时 计数器不停止)
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);									//清除TIM3的TIM中断源标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);									//允许TIM3的TIM中断源
	
	/* -------------------------------------------------------------------------- 
	 * 72 MHz / 72 = 1MHz (1祍)
	 * 1祍 * 1000 + 1祍 ~= 1ms	
	 * -------------------------------------------------------------------------- */ 
	TIM_TimeBaseStructure.TIM_Period 					= 1000;      
	TIM_TimeBaseStructure.TIM_Prescaler 			= 72;       
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;				//向上计数 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Global);			//设置TIM4更新请求源(生成重复脉冲--在更新事件时 计数器不停止)
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);									//清除TIM4的TIM中断源标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);									//允许TIM4的TIM中断源
	
	TIM_Cmd(TIM3, DISABLE);																			//TIM3 停止计数
	TIM_Cmd(TIM4, DISABLE);																			//TIM4 停止计数
}

void Timer_Interrupts_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable and set TIMER IRQ used for timeout */
	NVIC_InitStructure.NVIC_IRQChannel 											= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set TIMER IRQ used for delays */ 
	NVIC_InitStructure.NVIC_IRQChannel 					 						= TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
	NVIC_Init(&NVIC_InitStructure);						 	
}

void delayHighPriority_ms(uint16_t delay)
{
NVIC_InitTypeDef NVIC_InitStructure;

	counter_delay_ms = delay;

	/* Enable and set TIMER IRQ used for delays. High priority*/ 
	NVIC_InitStructure.NVIC_IRQChannel 					 						= TIMER_DELAY_IRQ_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);
	TIM_SetCounter(TIMER_DELAY, 0);
	/* TIM4 enable counter */
  	TIM_Cmd(TIMER_DELAY, ENABLE);
	/* Wait for 'delay' milliseconds */
	while(counter_delay_ms != 0);
	/* TIM4 disable counter */
	TIM_Cmd(TIMER_DELAY, DISABLE);

	/* Enable and set TIMER IRQ used for delays. Default priority */ 
	NVIC_InitStructure.NVIC_IRQChannel 					 						= TIMER_DELAY_IRQ_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= TIMER_DELAY_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


}

