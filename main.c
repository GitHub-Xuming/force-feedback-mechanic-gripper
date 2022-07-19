#include "encoder.h" 
#include "timer.h" 
#include "math.h" 
#include "dac.h"
#include "adc.h"
#include "servo_ctrl.h"
#include "voice_drive.h"
#include "step_motor.h"
#include "force_claw.h" 

int ADC_pos = 0;

//kalman位置滤波器
static double KalmanFilterPos(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
    double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;
    static double x_last=0;
    double x_mid = x_last;
    double x_now=0;
    static double p_last=0;
    double p_mid=0 ;
    double p_now=0;
    double kg=0;

    x_mid=x_last;
    p_mid=p_last+Q;

    kg=p_mid/(p_mid+R);                 
    x_now=x_mid+kg*(ResrcData-x_mid);   
    p_now=(1-kg)*p_mid;                 
    p_last = p_now;                     
    x_last = x_now;                     

    return x_now;
}

int main(void)
{ 
	stepMotorIO_Init();
	voiceBspInit();
	setCtrlPeriod(5); //5ms
	SYS_CLOCK_TIMER_Init(100,84-1);
	ENCODER_TIMER_Init();
	servoCtrlInit();
	Adc_Init();

	while(1)
	{	
		ADC_pos = Get_Adc(5);
		servoSetGoalPos(KalmanFilterPos(ADC_pos / 4.0, 0.01, 100.0));
		forceCtrl(); 
		servoCtrlLoop();
	}
}





