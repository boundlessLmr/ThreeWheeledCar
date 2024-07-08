/*电机测速测距服务参数*/
#include "motor_speed_process.h"
#define REDUCTION_RATIO 30  /*减速比*/

__IO int8_t MotorA_Direction = 0;/* 电机A转动方向*/
 
__IO int32_t MotorA_Capture_Count = 0;/* 计数值*/
 
__IO int8_t MotorB_Direction = 0;/* 电机B转动方向*/
 
__IO int32_t MotorB_Capture_Count = 0;/* 计数值*/
 
 
__IO uint32_t uwtick_Motor_speedService=0;


//motorA/B_speed







void Motor_speed_process(double *motorA_speed,double *motorB_speed,double *total_time)
{

			MotorA_Capture_Count = Read_Encoder(4);
			*motorA_speed+=(double)MotorA_Capture_Count*0.0325f*3.14*2.0f/0.10f/4.0f/100.0f/30.0f ;
			*total_time +=0.1;
			MotorB_Capture_Count = Read_Encoder(3);
			*motorB_speed+=(double)MotorB_Capture_Count*0.033f*3.14*2.0f/0.10f/4.0f/100.0f/30.0f;
}

