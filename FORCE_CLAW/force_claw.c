#include "force_claw.h" 
#include "voice_drive.h"
#include "servo_ctrl.h"
#include "step_motor.h" 
#include "adc.h"
#include "timer.h"


int voice_motor_pos = 0;

/**
  * @brief  
  * @param   
  * @return 
  */
void forceCtrl()
{
	voice_motor_pos = getMotorPos();  //获取控制器（音圈电机）位置
	stepMotorSetGoalPosition(voice_motor_pos / 2.0);  //step motor
	
  float force = Get_Adc(0) / (4096.0 / 1.5);
	servoExternSetCurrent(force); //将电流叠加到音圈电机
}





