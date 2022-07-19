#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H
#include "sys.h"

//motor ctrl端口定义
 
#define   DrY  PDout(1)
#define   StY  PDout(3)

void stepMotorIO_Init(void);//初始化
void stepMotorSetGoalPosition(int pos);
void stepMotorCtrl();

#endif
