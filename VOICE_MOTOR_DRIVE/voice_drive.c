#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "encoder.h" 
#include "voice_drive.h"

//针对硬盘音圈电机驱动程序

float motor_pos = 0, motor_vel = 0;
int ctrl_period_ms = 0;
float Q_p = 0.01, Q_v = 0.001; 

/**
  * @brief  kalman滤波器
  * @param   
  * @return 
  */
static double KalmanFilterP(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{

    double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;

    static double x_last;
    double x_mid = x_last;
    double x_now;

    static double p_last;
    double p_mid ;
    double p_now;

    double kg;

    x_mid=x_last;                    
    p_mid=p_last+Q;             

    
    kg=p_mid/(p_mid+R);                 
    x_now=x_mid+kg*(ResrcData-x_mid);   
    p_now=(1-kg)*p_mid;                 
    p_last = p_now;                     
    x_last = x_now;                     

    return x_now;

}

/**
  * @brief  kalman滤波器
  * @param   
  * @return 
  */
static double KalmanFilterV(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{

    double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;

    static double x_last;
    double x_mid = x_last;
    double x_now;

    static double p_last;
    double p_mid ;
    double p_now;

    double kg;

    x_mid=x_last;                      
    p_mid=p_last+Q;                     

    
    kg=p_mid/(p_mid+R);                 
    x_now=x_mid+kg*(ResrcData-x_mid);   
    p_now=(1-kg)*p_mid;                 
    p_last = p_now;                     
    x_last = x_now;                     

    return x_now;

}

/**
  * @brief  
  * @param   
  * @return 
  */
void voiceCtrlIO_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIO时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

	GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3);

}


/**
  * @brief  
  * @param   
  * @return 
  */
void setCtrlPeriod(int period_ms)
{
	ctrl_period_ms = period_ms;
}

/**
  * @brief  电位器位置反馈
  * @param   
  * @return 
  */
void voiceBspCB_ADC()
{
	static float last_pos = 0;
	static int count = 0;
	float pos = 0;

	count ++;
	pos = Get_Adc(0) - 1833;
	motor_pos = KalmanFilterP(pos, Q_p, 1.0);
	if(count == 5)
	{
		count = 0;
		motor_vel = KalmanFilterV((motor_pos - last_pos) / ctrl_period_ms * 100.0, Q_v, 1.0);
		last_pos = motor_pos;
	}
}

/**
  * @brief  编码器位置反馈
  * @param   
  * @return 
  */
void voiceBspCB_Encoder()
{
	static float last_pos = 0;
	static int count = 0;
	float pos = 0;

	count ++;
	pos = getEncoderCount();
	motor_pos = pos; //KalmanFilterP(pos, Q_p, 1.0);
	if(count == 2)
	{
		count = 0;
		motor_vel = KalmanFilterV((motor_pos - last_pos) / ctrl_period_ms * 100.0, Q_v, 1.0);
		last_pos = motor_pos;
	}
	
	
}

/**
  * @brief  
  * @param   
  * @return 
  */
void voiceBspInit()
{
	registCallBack(voiceBspCB_Encoder);
	voiceCtrlIO_Init();
	Dac1_Init();
	Adc_Init();
}

/**
  * @brief  
  * @param   
  * @return 
  */
float getMotorPos()
{
	return motor_pos;
}

/**
  * @brief  
  * @param   
  * @return 
  */
float getMotorVel()
{
	return motor_vel;
}

/**
  * @brief  
  * @param   
  * @return 
  */
void setMotorCur(float cur)
{
	Dac_Set_Vol(1, cur);
}







