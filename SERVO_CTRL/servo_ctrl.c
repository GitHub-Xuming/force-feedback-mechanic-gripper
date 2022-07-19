#include "timer.h"
#include "encoder.h" 
#include "pid_controller.h"
#include "voice_drive.h"
#include "servo_ctrl.h"

TargetPID pid_pos_loop, pid_vel_loop;
float Kp_pos_loop = 0.0, Ki_pos_loop = 0.0, Kd_pos_loop = 0;
float Kp_vel_loop = 0.2, Ki_vel_loop = 0.0, Kd_vel_loop = 0;
float goal_pos_loop = 330, goal_vel_loop = 0, goal_cur_loop = 0;
float extern_cur = 0;


/**
  * @brief  
  * @param   
  * @return 
  */
void servoCtrlInit()
{
    pid_pos_loop.getFeedbackVal = getMotorPos;
    pid_vel_loop.getFeedbackVal = getMotorVel;

    PID_SetPidPara(&pid_pos_loop, Kp_pos_loop, Ki_pos_loop, Kd_pos_loop);
    PID_SetPidPara(&pid_vel_loop, Kp_vel_loop, Ki_vel_loop, Kd_vel_loop);
}

/**
  * @brief  将外部电流叠加到音圈电机
  * @param   
  * @return 
  */
void servoExternSetCurrent(float cur)
{
    extern_cur = cur;
}

/**
  * @brief  
  * @param   
  * @return 
  */
void servoSetGoalPos(float pos)
{
    goal_pos_loop = pos;
}

/**
  * @brief  
  * @param   
  * @return 
  */
void servoCtrlLoop()
{
    getEncoderCount();
    PID_SetPidPara(&pid_pos_loop, Kp_pos_loop, Ki_pos_loop, Kd_pos_loop);
    PID_SetPidPara(&pid_vel_loop, Kp_vel_loop, Ki_vel_loop, Kd_vel_loop);
    PID_SetGoalVal(&pid_pos_loop, goal_pos_loop);   //设置位置环目标值
    goal_vel_loop = PIDControl(&pid_pos_loop) * 10.0;      //速度环目标 = 位置环输出
    PID_SetGoalVal(&pid_vel_loop, goal_vel_loop);   //设置速度环目标值
    goal_cur_loop = PIDControl(&pid_vel_loop) / 10.0;      //电流环目标 = 速度环输出

    goal_cur_loop = goal_cur_loop + extern_cur;
    
    if(goal_cur_loop > CURRENT_MAX)
        goal_cur_loop = CURRENT_MAX;
    if(goal_cur_loop < -CURRENT_MAX)
        goal_cur_loop = -CURRENT_MAX;

    if(goal_cur_loop > 0)
    {
        MOTOR_DIR_IN1 = 0;
        MOTOR_DIR_IN2 = 1;
    }
    else
    {
        MOTOR_DIR_IN1 = 1;
        MOTOR_DIR_IN2 = 0;
    }
    setMotorCur(fabs(goal_cur_loop));
}


