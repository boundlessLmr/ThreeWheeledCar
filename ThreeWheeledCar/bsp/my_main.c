#include "my_main.h"

//-----全局变量---
extern struct Bkeys bkey[];
extern struct key_data key_data1[];
extern uint8_t xun[8];
__IO uint32_t uwtick_key=0;
extern int8_t arr_bit[8];
extern uint8_t Buzzer_flag0,Buzzer_flag1,Buzzer_flag2,Buzzer_flag3,Buzzer_flag4,Buzzer_flag5,Buzzer_flag6,Buzzer_flag7,Buzzer_flag8,Buzzer_flag9,Buzzer_flag10;

__IO uint32_t uwTick_UART = 0;
int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量
int Encoder_Left,Encoder_Right,Balance_PWM;
int Moto1,Moto2;
int PWM_out,Turn_Pwm = 0; //闭环输出
		
u8 AIN1,AIN2;//控制电机正反转
u8 BIN1,BIN2;
u8 Moto_Flag=0;//电机控制标志
u8 Start_Flag=0;//系统初始化完成标志

float Pitch,Roll,Yaw;	        // Pitch：俯仰角，Roll：横滚角，Yaw：偏航角
short gyrox,gyroy,gyroz;        // 角速度
short aacx,aacy,aacz;           // 加速度
float Med_Angle=0;//机械中值，能使得小车真正平衡住的角度 



/*UART 相关*/
char Usart2_RX_Buf[50];  //串口接收数据缓存buf
char Usart3_TX_Buf[50];
char temp[50]; 

int8_t Scale=0;


u8 Flag_forward = 0, Flag_retreat = 0, Flag_left = 0, Flag_right = 0;//蓝牙遥控标志





//----调参选择-----// 
#define TIAO_VERp          0
#define TIAO_VERd          1
#define TIAO_VELp          2
#define TIAO_GP            3
#define TIAO_GD            4
#define TIAO_AIM				   5


uint8_t Tiao_Who = TIAO_GD;
float test_num_VERp = 10;
float test_num_VERd = 0.1;
float test_num_VELp = 0.1;
float test_num_GP = 10;
float test_num_GD= 1;
float test_num_AIM= 50;

//----直立环-----// 
//float Bias, Vertical_Kp= 460,Vertical_Kd= -2.1;
//float Bias, Vertical_Kp= 460 * 0.6,Vertical_Kd= -2.1 * 0.6;

//float Bias, Vertical_Kp= 276,Vertical_Kd= -1.76;//1.86
float Bias, Vertical_Kp= 276,Vertical_Kd= -1.4;//速度800 Vertic_kd = -1.8 Velocity_kp = 6.5 // 速度200~600 Vertic_kd = -1.5 Velocity_kp = 4.6
float Med = 10;//14

//float Med = 5.9;
//14.5   15.2
//float Med = 13.75;
//float Med = 10;

//----速度环-----/*0
 float Velocity,Encoder_Err,Encoder,Encoder_last,Movement=0; //速度，误差，编码器
 float Encoder_Integral;	 //编码器数值积分
 #define kp_val 4.6
 float kp=kp_val,ki=kp_val/200;
 int jiasu_flag = -50;//这表示延时50*0.1秒再发车
 int AimVelocity = 0;
 uint8_t SpeedMode = 0;
 
 
//----循迹-----
uint8_t Black_miss_flag = 0;
int last_bias;
//int Gray_KP=0,Gray_KD= 0 ;
//int Gray_KP=0,Gray_KD= -0 ;
int Gray_KP=35,Gray_KD=-12;


//计算距离和时间
double motorA_speed,motorB_speed;
double total_time;
double distance;
 
/*温度*/
float tem;


//----子函数声明-----
void UART_Proc(void);
int Vertical(float Angle,float Gyro_Y);			 				//直立环
int GetVelocity(int Encoder_left,int Encoder_right);				//速度环
u8 Stop(signed int angle);    		//倒下保护
void Limit(int *motoA,int *motoB);  //电机速度限幅
void Set_Pwm(int Moto1,int Moto2);	//控制PWM最终输出

u8 test111 = 0;
uint8_t page = 0;
u8 select_key =0;



void key_process(void)
{

	if(key_data1[0].key_return==1)//按键KEY1 
	{													

		switch(Tiao_Who)
		{
			case TIAO_VERp:Vertical_Kp-= test_num_VERp;break;	
			case TIAO_VERd:Vertical_Kd-= test_num_VERd;break;
			case TIAO_VELp:kp					-= test_num_VELp;ki = kp/200;break;
			case TIAO_GP  :  Gray_KP+=test_num_GP;  break;
//			case TIAO_GP  :  Gray_KD-=test_num_GD;  break;	
			case TIAO_GD  :  AimVelocity+=20;  break;	
		}		
		
		key_data1[0].key_return = 0;		
	}
	
	if(key_data1[1].key_return==1)
	{

		switch(Tiao_Who)
		{
			case TIAO_VERp:Vertical_Kp+= test_num_VERp;break;	
			case TIAO_VERd:Vertical_Kd+= test_num_VERd;break;
			case TIAO_VELp:kp					+= test_num_VELp;ki = kp/200;break;
			case TIAO_GP  :Gray_KP		+=test_num_GP;  break;
			case TIAO_GD  :Gray_KD		+=test_num_GD;  break;
		}		

		key_data1[1].key_return = 0;
	}	
	
	if(key_data1[2].key_return==1)//这是第四个按键
	{
		Tiao_Who++;if(Tiao_Who== 5)Tiao_Who = 0;	
		OLED_Clear();				
		
		//这个按键是第四个按键，按下复位所有的题目标志位并切换模式
		SpeedMode=0;
		jiasu_flag = -30;
		Buzzer_flag0=0,Buzzer_flag1=1,Buzzer_flag2=1,Buzzer_flag3=1,Buzzer_flag4=1,Buzzer_flag5=1,Buzzer_flag6=1,Buzzer_flag7=1,Buzzer_flag8=1,Buzzer_flag9=1,Buzzer_flag10=1;
		


		key_data1[2].key_return = 0;
	}
	
	if(key_data1[3].key_return==1)
	{
		
		switch(Tiao_Who)
		{
			case TIAO_VERp:if(test_num_VERp == 100)test_num_VERp = 10;else test_num_VERp = 100;break;
			case TIAO_VERd:if(test_num_VERd == 1) test_num_VERd = 0.1;else test_num_VERd = 1;break;
			case TIAO_VELp:if(test_num_VELp == 0.01)test_num_VELp = 0.1;else test_num_VELp = 0.01;break;	
			case TIAO_GP  :    break;
			case TIAO_GD  :    break;
			
		}
		
	key_data1[3].key_return = 0;
	}	
}
/**************************************************************************
函数功能：通过串口测试数据
**************************************************************************/
void UART_Proc()
{
  if((uwTick - uwTick_UART) < 200) return;//return;结束函数
	uwTick_UART = uwTick;
	//ReadBlag();
//		sprintf(Usart3_TX_Buf,"%lf,%lf,%d,%d,%d,%d,%d,%d,%d,%d\r\n",motorA_speed,motorB_speed,Moto1,Moto2,Buzzer_flag0,Buzzer_flag1,Buzzer_flag2,Buzzer_flag3,Buzzer_flag4,Buzzer_flag5);
//  	HAL_UART_Transmit(&huart2,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
	
	
		if((Moto1>=7000 || Moto1<=-7000) && (Moto2 >= 7000 || Moto2<=-7000))
		{
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_1);
		}
		
	sprintf(Usart3_TX_Buf,"va0.val=%d\xff\xff\xff",ABCDE_flag);
	HAL_UART_Transmit(&huart2,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//	if(coin_flag
//	sprintf(tx_char,"va0.val=%d\xff\xff\xff",coin_flag+4);
//  HAL_UART_Transmit(&huart2,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
	
//		
//		sprintf(Usart3_TX_Buf,"Tiao_Who %d",Tiao_Who);
//  	HAL_UART_Transmit(&huart2,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);	
	


//	if ((mpu_dmp_get_data(&Pitch, &Roll, &Yaw) == 0) && (MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz) == 0)) //DMP读取欧拉角，digital motion processor数字运动处理器
//   {
//		//俯仰角，横滚角，偏航角
//		 sprintf(Usart3_TX_Buf,"pitch=%.3f roll=%.3f yaw=%.3f\r\n", Pitch, Roll, Yaw);   //串口打印欧拉角
//     HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//  	 sprintf(Usart3_TX_Buf,"gyrox=%d   gyroy=%d  gyroz=%d \r\n", gyrox, gyroy, gyroz);	// 角速度
//		 HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//		 sprintf(Usart3_TX_Buf,"-------------------------------------------\r\n");
//     HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//			
//	 }

//		 sprintf(Usart3_TX_Buf,"E_Left %d E_right %d\r\n",Encoder_Left,Encoder_Right);
//     HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);


}

void oled_proc()
{//行x:0~127    列y:0~63
		switch(Tiao_Who)
		{

			case TIAO_VERp:
											//============= 第一行 调直立环=======================//	
												OLED_ShowString(00,00,"P:",12);                    OLED_ShowFNum(96,00,motorA_speed*0.6*0.85 ,12);//
												OLED_ShowFNum(24,00,Vertical_Kp,12);
											//=============  第二行=======================//	
												OLED_ShowString(00,12,"D:",12);                   
												OLED_ShowFNum(24,12,Vertical_Kd,12);               OLED_ShowFNum(96,12,total_time,12);
											//=============  第三行=======================//	
												OLED_ShowString(00,24,"Pi:",12);
												OLED_ShowFNum(36,24,Pitch,12);   							 		 OLED_ShowFNum(96,24,motorB_speed*0.6*0.85,12);
											//=============  第四行=======================//	
												OLED_ShowString(00,36,"gy:",12);
												OLED_ShowFNum(36,36,AimVelocity,12);	
											//=============  第5行=======================//	
												OLED_ShowString(00,48,"vp:",12);
												OLED_ShowFNum(36,48,Vertical_out,12);			
											
												OLED_Refresh();	break;					
			
			case TIAO_VERd:
											//============= 第一行 调直立环=======================//	
												OLED_ShowString(00,00,"P:",12);                   
												OLED_ShowFNum(24,00,Vertical_Kp,12);
											//=============  第二行=======================//	
												OLED_ShowString(00,12,"D:",12);                   
												OLED_ShowFNum(24,12,Vertical_Kd,12);
											//=============  第三行=======================//	
												OLED_ShowString(00,24,"pi:",12);
												OLED_ShowFNum(36,24,Pitch,12);
											//=============  第四行=======================//	
												OLED_ShowString(00,36,"gy:",12);
												OLED_ShowFNum(36,36,gyroy,12);	
											//=============  第5行=======================//	
												OLED_ShowString(00,48,"vp:",12);
												OLED_ShowFNum(36,48,Vertical_out,12);			
											
												OLED_Refresh();	break;					
						
			case TIAO_VELp:
											//============= 第一行 调速度环=======================//	
												OLED_ShowString(00,00,"P:",12);                   
												OLED_ShowFNum(24,00,kp,12);         
											//=============  第二行=======================//	
												OLED_ShowString(00,12,"i:",12);                   
												OLED_ShowFNum(24,12,ki,12);					
											//=============  第三行=======================//	
												OLED_ShowString(00,24,"le:",12);
												OLED_ShowSignedNum(24,24,Encoder_Left,4,12);   
												
									//			OLED_ShowString(00,24,"er:",12);
									//		  OLED_ShowSignedNum(24,24,Encoder_Err,4,12);			
											//=============  第四行=======================//	
//												OLED_ShowString(00,36,"ri:",12);
//												OLED_ShowSignedNum(24,36,Encoder_Right,4,12);
												OLED_ShowString(00,36,"tt:",12);
											  OLED_ShowSignedNum(24,36,test111,4,12);			
											//=============  第5行=======================//		
												OLED_ShowFNum(00,48,Pitch,12);	  OLED_ShowFNum(60,48,PWM_out,12);	 
											// 没有第6行
									
												OLED_Refresh();break;			
			case TIAO_GP:
											//============= 第一行 调速度环=======================//	
												OLED_ShowString(00,00,"GP",12);                   
												OLED_ShowFNum(24,00,Gray_KP,12);
											//=============  第二行=======================//	
												OLED_ShowString(00,12,"GD",12);                   
												OLED_ShowFNum(24,12,Gray_KD,12);	
												OLED_Refresh();break;			

			case TIAO_GD:
											//============= 第一行 调速度环=======================//	
												OLED_ShowString(00,00,"GD",12);                   
												OLED_ShowFNum(24,00,Gray_KD,12);
											//=============  第二行=======================//	
												OLED_ShowString(00,12,"GP:",12);                   
												OLED_ShowFNum(24,12,AimVelocity,12);
												OLED_Refresh();break;			


		}

}

void set_up(void)
{
  //启动TIM初始化

	
	
   HAL_TIM_Base_Start_IT(&htim2);//每10ms触发一次中断 优先级最高的中断
	 HAL_Delay(200);
	 HAL_TIM_Base_Start_IT(&htim6);//每10ms触发一次中断 优先级次之
	
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//PWM
	 HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);	
  
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);//编码器
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);	

	 HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t *)Usart2_RX_Buf,50);
	
		MPU_Init();
	  mpu_init();
		mpu_dmp_init();
		OLED_Init();	
  	OLED_Clear();	
		
	OLED_ShowString(00,00,"mpu_dmp_init",12);  
	OLED_Refresh();	
	Buzzer_Off();
  //-----DMP初始化----
	
  while(mpu_dmp_init())//成功返回0，否则返回1
  {
		uint8_t res;
		res = mpu_dmp_init();
		HAL_Delay(300);
		OLED_ShowNum(00,12,res,1,12);
		OLED_Refresh();	
  }
  	OLED_Clear();	
		OLED_ShowString(103,00,"on",12);  
	Start_Flag = 1; //标志系统初始化成功
	OLED_Refresh();	
//	OLED_Refresh();	
	
  
	
	
	HAL_TIM_Base_Start_IT(&htim5);//优先级最低的中断，因为周期很长0.1秒，主要作用是定时
}

void loop(void)
{
	UART_Proc();
	key_process();
	oled_proc();
//	if(Buzzer_flag1!= 0 && Buzzer_flag5<=1) Motor_speed_process(&motorA_speed,&motorB_speed,&total_time);
//	Buzzerset(Buzzer_flag0);
}

/**************************************************************************
函数功能：10ms定时器中断，主控制函数
**************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == TIM2)//TIM2设置为10ms中断
  {
		// 1.采集编码器数据&MPU6050角度信息
   // 电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反
	  mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	    // 读取角度
    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);  // 读取角速度

	  Encoder_Left  =  Read_Encoder(4);		//读取编码器测量的电机转速
		Encoder_Right =   Read_Encoder(3);		

  // 2.将数据压入闭环控制中，计算出控制输出量
		Velocity_out = GetVelocity(Encoder_Left,Encoder_Right);// 速度环输出误差
	  Vertical_out = Vertical(Pitch,gyroy);// 直立环输出PWM
		
		Scale = Read_8PIN();//读灰度调用pid生成pwm
		//------------最终输出----------------
	  PWM_out= Vertical_out+Velocity_out;
		
		// 3.把控制输出量加载到电机上，完成最终控制
			Moto1 = PWM_out+Gray_PID(Scale); // 左电机
      Moto2 = PWM_out-Gray_PID(Scale); // 右电机
      Limit(&Moto1,&Moto2);     // PWM限幅 
		  Set_Pwm(Moto1,Moto2);        // 加载到电机上
  }
	if(htim->Instance == TIM5)//0.1秒
	{
		if(Buzzer_flag0!= 0 && Buzzer_flag5<=1) 
		{
			Motor_speed_process(&motorA_speed,&motorB_speed,&total_time);
			distance = (motorA_speed+motorB_speed)*0.5f*0.85f*0.6f;
		}//这是第一题如果触发了第一根线并且未触发第五根线就调用函数计时和测距
		if(Tiao_Who == TIAO_VERp) Timu_1(SpeedMode);//以下是三个题目的切换
		else if(Tiao_Who == TIAO_VERd) Timu_2(SpeedMode);
		else if(Tiao_Who == TIAO_VELp) Timu_3(SpeedMode);
	}
	if(htim->Instance == TIM6)//10ms
	{
		ReadBlag();//读每根黑色标志线
		ReadCoins();//读硬币
		ReadKey();//读按键
	}
}

/*************************************************************************** 
直立环PD控制器：Kp*Ek+Kd*Ek_D
入口：Med:机械中值(期望角度)，Angle:真实角度，gyro_Y:真实角速度
出口：PMW数值
******************************************************************************/
int Vertical(float Angle,float Gyro_Y) 
{

	Bias = Med-Angle;//直立偏差 //angle = -0.23就平衡了
	PWM_out = Vertical_Kp*Bias+Vertical_Kd*(Gyro_Y-0);	
	return PWM_out;	
} 

int Gray_PID(int bias)
{
	int res = Gray_KP*bias+Gray_KD*(bias-last_bias);
	last_bias = bias;
	return res;
}



/********************************************************************* 
速度环PI控制器：Kp*Ek+Ki*Ek_S(Ek_S：偏差的积分)
入口：左右编码器测到的数值
出口：                   
**********************************************************************/
int GetVelocity(int Encoder_left,int Encoder_right)	
{
	
	// 1.计算速度偏差 	
	Encoder_Err = ((Encoder_Left+Encoder_Right) - AimVelocity);	//速度目标值//Encoder_Left+Encoder_Right 最大是 160  7200 /（160 * 50%） = 90
 
	// 2.对速度偏差进行--低通滤波--
  // low_out = (1-a)*Ek+a*low_out_last	
	Encoder = Encoder_Err*0.3 + Encoder_last*0.7;// 使得波形更加平滑
	Encoder_last = Encoder; 							// 防止速度过大影响直立环的正常工作

	// 3.对速度偏差积分出位移,遥控的速度通过积分融入速度控制器，减缓速度突变对直立控制的影响	
  Encoder_Integral += Encoder-Movement;	



	// 4.积分限幅	
	if(Encoder_Integral>3000)  	Encoder_Integral=3000;   
	if(Encoder_Integral<-3000)	   Encoder_Integral=-3000;           	

	if(Moto_Flag == 1||Start_Flag ==0) 			Encoder_Integral=0;     		//===电机关闭后或者复位清除积分
    //5.速度环控制输出	
  Velocity=Encoder*kp+Encoder_Integral*ki;

	return Velocity;



}



/*************************************************************************** 
函数功能：控制电机
******************************************************************************/
void Contrl(void)
{

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, BIN1);	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, BIN2);
	
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, AIN1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, AIN2);

}




/**************************************************************************
函数功能：电机异常关闭函数
入口参数：角度
返回  值：1：关闭，0：不关闭
**************************************************************************/	 		
u8 Stop(signed int angle)
{
	    u8 temp=0;
			if(angle<-50||angle>50)
			{	                                //===倾角大于40度关闭电机
				temp=1;                   		   //===Flag_Stop置1关闭电机
				Moto1 = 0;
				Moto2 = 0;
      }
	
		return temp;
}



/**************************************************************************
函数功能：电机转动控制函数
入口参数：闭环控制最终输出值
**************************************************************************/	
void Set_Pwm(int Moto1,int Moto2)
{
	
	 Moto_Flag=Stop(Roll);	//获取是否倒下的标志
	if(Start_Flag == 1)		//一级判断系统是否正常初始化
	{
		if(Moto_Flag==0)	//二级判断//
		{
			if(Moto1>0)  AIN1 = 0,AIN2 = 1;
			else			 AIN1 = 1,AIN2 = 0;				
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,abs(Moto1));		 
			
			if(Moto2>0)  BIN1 = 0,BIN2 = 1;		
			else		    BIN1 = 1 ,BIN2 = 0;
    		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,abs(Moto2)); //0-7200
		}
			
		else//倒下就关闭电机
		{
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);	//4500-6000
		   __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,0);	//4500-6000
		}
		 Contrl();
	}
}


/**************************************************************************
函数功能：限制电机速度
入口参数：闭环控制最终输出值
**************************************************************************/	
void Limit(int *motoA,int *motoB)
{
	if(*motoA>7000)*motoA=7000;//最大7200
	if(*motoA<-7000)*motoA=-7000;

	if(*motoB>7000)*motoB=7000;
	if(*motoB<-7000)*motoB=-7000;
} 


/**************************************************************************
函数功能：蓝牙遥控设置USART3接收中断回调
引脚：		 
**************************************************************************/
//串口空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)	
{
	if(strcmp(Usart2_RX_Buf,"1")==0)
	{
//			jizhi();
		
		
		//float Med = 2;
	}
	else if(strcmp(Usart2_RX_Buf,"2")==0)
	{
//			Flag_forward = 1,Flag_retreat = 0;
//			test111++;
	
		
	}
	else if(strcmp(Usart2_RX_Buf,"3")==0)
	{
//		right();
//		test111++;

		
	}	
	else if(strcmp(Usart2_RX_Buf,"4")==0)
	{
//			left();
//		test111++;

		
	}	
	else if(strcmp(Usart2_RX_Buf,"5")==0)
	{
//		  Flag_retreat = 1, Flag_forward = 0;
//		test111++;	
		
		
	}		
	else if(strcmp(Usart2_RX_Buf,"6")==0)
	{

////			BigRight();
//		test111++;		
	}	
	else if(strcmp(Usart2_RX_Buf,"7")==0)
	{
		
		
//		  BigLeft();
//		test111++;	
	}			
			
}
