#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H
#include <math.h>
#include <stdint.h> 
#include <sys.h>

#define    CURRENT_MAX   1.5

void servoCtrlInit();
void servoCtrlLoop();
void servoExternSetCurrent(float cur);
void servoSetGoalPos(float pos);

#endif 



