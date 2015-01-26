#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"
#include "My_Config.h"

extern bool 		TimeUp;


void StartTimeOut( uint16_t delay );
void StopTimeOut( void );
void Timer_Config( void );
void Timer_Interrupts_Config(void);
void delayHighPriority_ms(uint16_t delay);
#endif
