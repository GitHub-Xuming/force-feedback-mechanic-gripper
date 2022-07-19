#ifndef SERVO_BSP_H
#define SERVO_BSP_H
#include <math.h>
#include <stdint.h> 
#include <sys.h>

#define     MOTOR_DIR_IN1    PAout(2)
#define     MOTOR_DIR_IN2    PAout(3)


void voiceBspInit();
float getMotorPos();
void setMotorPos(float pos);
float getMotorVel();
void setMotorCur(float cur);
void setCtrlPeriod(int period_ms);

#endif 



