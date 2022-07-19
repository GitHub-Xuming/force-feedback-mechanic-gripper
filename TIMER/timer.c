#include "timer.h"
#include "encoder.h"
#include "math.h"
#include "step_motor.h" 


/*系统时间*/
volatile u32 g_sys_time = 0;
u8 controlPeriodFlag = 0;
void (*timerCallBack)();

/**
  * @brief  
  * @param  
  * @param 
  */
void registCallBack(void (*CallBack)())
{
	timerCallBack = CallBack;
}

/**                  
  * @brief 
  * @param  
  * @param   
  */	
void SYS_CLOCK_TIMER_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SYS_CLOCK_TIMER,ENABLE);  ///使能SYS_CLOCK_TIMER时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(SYS_CLOCK_TIMER,&TIM_TimeBaseInitStructure);//初始化SYS_CLOCK_TIMER
	
	TIM_ITConfig(SYS_CLOCK_TIMER,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(SYS_CLOCK_TIMER,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=SYS_CLOCK_TIMER_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/**
  * @brief  get全局系统运行时间
  * @param  
  * @param 
  */
u32 getSysTime()
{
	return g_sys_time;
}

/**
  * @brief  getControlPeriodFlag
  * @param  
  * @param 
  */

u8 getControlPeriodFlag()
{
	return controlPeriodFlag;
}

/**
  * @brief  setControlPeriodFlag
  * @param  
  * @param 
  */
void setControlPeriodFlag(u8 val)
{
	controlPeriodFlag = val;
}

/**
  * @brief  定时器中断服务函数
  * @param  
  * @param 
  */
void SYS_CLOCK_TIMER_IRQHandler(void)
{
	static int count = 0;

	if(TIM_GetITStatus(SYS_CLOCK_TIMER,TIM_IT_Update)==SET) //溢出中断
	{
		g_sys_time ++;
		timerCallBack();
		stepMotorCtrl();
		if(g_sys_time % 200 == 0)
		{
			setControlPeriodFlag(1);
		}
	}
	
	TIM_ClearITPendingBit(SYS_CLOCK_TIMER,TIM_IT_Update);  //清除中断标志位
}
