#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define SYS_CLOCK_TIMER                 	TIM2
#define RCC_APB1Periph_SYS_CLOCK_TIMER   	RCC_APB1Periph_TIM2
#define SYS_CLOCK_TIMER_IRQn   			 	TIM2_IRQn
#define SYS_CLOCK_TIMER_IRQHandler 	    	TIM2_IRQHandler

void registCallBack(void (*CallBack)());
u32 getSysTime();
void SYS_CLOCK_TIMER_Init(u16 arr,u16 psc);
void setControlPeriodFlag(u8 val);
u8 getControlPeriodFlag();

#endif


