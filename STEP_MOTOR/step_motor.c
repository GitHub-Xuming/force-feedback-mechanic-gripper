#include "step_motor.h" 

int goal_position_ = 0, pre_position = 0, pos_bias = 0;
u8 dir = 0;

/**
  * @brief  
  * @param   
  * @return 
  */
void stepMotorIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIO时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO

	GPIO_SetBits(GPIOD, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7);
}

/**
  * @brief  
  * @param   
  * @return 
  */
void stepMotorSetGoalPosition(int pos)
{
	goal_position_ = pos;
}

/**
  * @brief  
  * @param   
  * @return 
  */
void stepMotorCtrl()
{
static int count = 0;
static u8 status = 1;

	switch(status)
	{
		case 1:
		if(goal_position_ != pre_position)  //扫描
		{
			pos_bias = goal_position_ - pre_position; //得到偏差
			status = 2;
		}			
		break;
		
		case 2:
		if(pos_bias > 0)
		{
			count ++;
			DrY = 1;  //正方向
			dir = DrY;
			StY = !StY;
			if(count == (pos_bias * 2))
			{
				pre_position += pos_bias;
				count = 0;
				status = 1;
				break;
			}
		}
		else if(pos_bias < 0)
		{
			count ++;
			DrY = 0;  //负方向
			dir = DrY;
			StY = !StY;
			if(count == ((-pos_bias) * 2))
			{
				pre_position += pos_bias;
				count = 0;
				status = 1;
				break;
			}					
		}
				
		default:
		break;			
	}
}


