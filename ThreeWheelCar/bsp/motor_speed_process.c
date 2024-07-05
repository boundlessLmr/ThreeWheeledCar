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

	
		//motorA
//		if (uwTick - uwtick_Motor_speedService <100)
//		{
//			return;
//		}
//		else
//		{
//			uwTick = uwtick_Motor_speedService;
//			MotorA_Direction=__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
			MotorA_Capture_Count = Read_Encoder(4);
//			if(MotorA_Direction)  //如果向下计数，真实的脉冲计数值＝65535-CaptureNumber
//			MotorA_Capture_Count=65535-MotorA_Capture_Count;
			*motorA_speed+=(double)MotorA_Capture_Count*0.0325f*3.14*2.0f/0.10f/4.0f/100.0f/30.0f ;
//			*motorA_speed=MotorA_Capture_Count;
			
			*total_time +=0.1;
			
//			MotorB_Direction=__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
			MotorB_Capture_Count = Read_Encoder(3);
//			if(MotorB_Direction)  //如果向下计数，真实的脉冲计数值＝65535-CaptureNumber
//			MotorB_Capture_Count=65535-MotorB_Capture_Count;
			*motorB_speed+=(double)MotorB_Capture_Count*0.033f*3.14*2.0f/0.10f/4.0f/100.0f/30.0f;
//			*motorB_speed=MotorB_Capture_Count;
			// 脉冲数量/4倍频/单圈脉冲数/减速比例/定时时间   单位 m/s    
			
//		}
		
		
		
		


}

