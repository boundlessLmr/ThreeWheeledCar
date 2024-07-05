#include "my_main.h"

//-----全局变量---
extern struct Bkeys bkey[];
extern struct key_data key_data1[];
extern uint8_t xun[8];
__IO uint32_t uwtick_key=0;
extern int8_t arr_bit[8];

__IO uint32_t uwTick_UART = 0;
int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量
int Encoder_Left,Encoder_Right,Balance_PWM;
int Moto1,Moto2;
int PWM_out,Turn_Pwm = 0; //闭环输出
		
u8 AIN1,AIN2;//控制电机正反转
u8 BIN1,BIN2;
u8 Moto_Flag=0;//电机控制标志
u8 Start_Flag=0;//系统初始化完成标志

//----调参选择-----// 


#define TIAO_VELp          1
#define TIAO_GP            2
#define TIAO_GD            3


uint8_t Tiao_Who = TIAO_GD;
float test_num_VELp = 0.1;
float test_num_GP = 1;
float test_num_GD= 1;



//----速度环-----//
 float Velocity,Encoder_Err,Encoder,Encoder_last,Movement=0; //速度，误差，编码器
 float Encoder_Integral;	 //编码器数值积分
 #define kp_val 4.6
 float kp=kp_val,ki=kp_val/200;
 int AimVelocity = 600;
 uint8_t SpeedMode = 0;
 
 
//----循迹-----
uint8_t Black_miss_flag = 0;
int last_bias;
int8_t Scale=0;
int Gray_KP=60,Gray_KD=-16;

//计算距离和时间
double motorA_speed,motorB_speed;
double total_time;
double distance;
 
//-----------具体功能控制-----------

char Usart2_RRRX_Buf[50];  //串口接收数据缓存buf
char Usart2_TTTX_Buf[50];  //串口发送数据缓存buf

#define RXIDLE              20
#define LONELINE						10
#define SHORTLINE						11
#define GREENCIRCLE					12
#define REDCIRCLE						13
#define REDTRI						  14
#define BLUECIRCLE					15
#define YELLOWTRI						16
uint8_t RRRXnum = RXIDLE;
uint8_t countLongline = 0;
uint8_t countShortline = 0;
uint8_t stopEN = 0;
uint8_t CarVelocity = 0;

#define IDLE              0
#define BASE_1            1
#define BASE_2            2
#define BASE_3            3
#define IMPROVE_1         4
#define IMPROVE_2         5
#define IMPROVE_3         6
uint8_t whichFuction = IDLE;
//基础1
//基础2
uint8_t blueEN = 0;
uint8_t yellowEN = 0;
uint8_t VelocitySel = 0;
uint8_t FindBlueYellowEN = 0;
//基础3
uint8_t stop_10sEN= 0;
//发挥1
uint8_t tx_lcd = 0;

//发挥2
uint8_t BaseCarVelocity = 0;
uint8_t RatioCarVelocity = 0;
//发挥3
uint8_t turnEN = 0;


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
void FuctionBase2(void)
{
		switch(RRRXnum)
		{
			case LONELINE:
										countLongline++;if(countLongline == 3) {/*stop,*/ countLongline = 0;FindBlueYellowEN = 0;}
										countShortline = 0;
										if(countLongline == 2)
										{
											if(!blueEN && !yellowEN) VelocitySel = 1;//A<10s,B>20s
											else if(blueEN && !yellowEN) VelocitySel = 2;//A+B<20s,C>30s	
											else if(!blueEN && yellowEN) VelocitySel = 3;//A+B>30s,C<20s		
											
											switch(VelocitySel){//速度3挡
												case 1:										
													break;
												case 2:
													break;
												case 3:
													break;
												default:
													break;
											}												
										}
										blueEN = 0;yellowEN = 0;
				break;
			case SHORTLINE:
										countShortline++;if(countShortline == 4) FindBlueYellowEN = 1;
										if(countShortline == 1 && VelocitySel != 0){//B
														switch(VelocitySel){//速度3挡
															case 1:										
																break;
															case 2:
																break;
															case 3:
																break;											
															default:
																break;					
															}
														}
										if(countShortline == 2 && VelocitySel != 0){//C
														switch(VelocitySel){//速度3挡
															case 1:										
																break;
															case 2:
																break;
															case 3:
																break;											
															default:
																break;					
															}
														}
				break;
			case BLUECIRCLE:if(FindBlueYellowEN) blueEN = 1;break;
			case YELLOWTRI:if(FindBlueYellowEN) yellowEN = 1;break;																				
		}
		RRRXnum = RXIDLE;					
}
void FuctionSelect(void)
{/****基础1********/
	switch (whichFuction){
		case IDLE:
					break;
		case BASE_1:/*********基础1********/
								switch(RRRXnum)
								{
									case LONELINE://stop 
										break;
									default:
										break;
								}
								RRRXnum = RXIDLE;
								break;
		case BASE_2:FuctionBase2();break;
		case IMPROVE_1:/**************基础3***********/
				
					break;		
		case IMPROVE_2:
					break;		
		case IMPROVE_3:
					break;	
		default:
					break;
	}
}

void key_process(void)
{

	if(key_data1[0].key_return==1)//按键KEY1 
	{													
		switch(Tiao_Who)
		{
			case TIAO_GP  :  Gray_KP+=test_num_GP;  break;
			case TIAO_GD  :  Gray_KD+=test_num_GD; break;	
			case TIAO_VELp:   kp+= test_num_VELp;ki = kp/200;break;
		}		
	
		key_data1[0].key_return = 0;		
	}
	
	else if(key_data1[1].key_return==1)//按键KEY2 坏了
	{

	
		key_data1[1].key_return = 0;
	}	
	

	
	else if(key_data1[3].key_return==1)//按键KEY3
	{
		switch(Tiao_Who)
		{
			case TIAO_GP  :  Gray_KP-=test_num_GP;  break;
			case TIAO_GD  :  Gray_KD-= test_num_GD;  break;	
			case TIAO_VELp:   kp-= test_num_VELp;ki = kp/200;break;
		}				
		
		key_data1[3].key_return = 0;
	}	
	else if(key_data1[2].key_return==1)//按键KEY3 第4个
	{
		switch(Tiao_Who)
		{
			case TIAO_VELp:if(test_num_VELp == 0.01)test_num_VELp = 0.1;else test_num_VELp = 0.01;break;	
			case TIAO_GP  :if(test_num_GP == 1)test_num_GP = 10;else test_num_GP = 1;break;	
			case TIAO_GD  :break;				
		}
		
		key_data1[2].key_return = 0;
	}	
}
/**************************************************************************
函数功能：通过串口测试数据
**************************************************************************/
void UART_Proc()
{
  if((uwTick - uwTick_UART) < 200) return;//return;结束函数
	uwTick_UART = uwTick;
	
		if((Moto1>=7000 || Moto1<=-7000) && (Moto2 >= 7000 || Moto2<=-7000))
		{
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_1);
		}
		
		
		


}

void oled_proc()
{//列x:0~127    行y:0~63

		switch(Tiao_Who)
		{
				case TIAO_VELp:
												OLED_ShowString(00,00,"V:",12);               OLED_ShowString(84,00,"VP",12);        
												OLED_ShowSignedNum(24,00,AimVelocity,4,12);
											//=============  =======================//	
												OLED_ShowString(00,12,"EL:",12);                   
												OLED_ShowSignedNum(24,12,Encoder_Left,5,12);               
											//=============  =======================//	
												OLED_ShowString(00,24,"ER:",12);
												OLED_ShowSignedNum(24,24,Encoder_Right,5,12);   							 		
											//=============  =======================//	
												OLED_ShowString(00,36,"VP:",12);
												OLED_ShowSignedNum(36,36,kp,3,12);	
												OLED_Refresh();		
												break;
				
				case TIAO_GP:
												OLED_ShowString(00,00,"V:",12);                   OLED_ShowString(84,00,"GP",12);     
												OLED_ShowSignedNum(24,00,AimVelocity,4,12);
											//=============  =======================//	
												OLED_ShowString(00,12,"EL:",12);                   
												OLED_ShowSignedNum(24,12,Encoder_Left,5,12);               
											//=============  =======================//	
												OLED_ShowString(00,24,"ER:",12);
												OLED_ShowSignedNum(24,24,Encoder_Right,5,12);   							 		
											//=============  =======================//	
												OLED_ShowString(00,36,"GP:",12);
												OLED_ShowSignedNum(36,36,Gray_KP,3,12);	
											//=============  =======================//	
												OLED_ShowString(00,48,"GD:",12);
												OLED_ShowSignedNum(36,48,Gray_KD,3,12);				
												OLED_Refresh();					
												break;
				case TIAO_GD:											
												OLED_ShowString(00,00,"V:",12);                  OLED_ShowString(84,00,"GD",12);  			    
												OLED_ShowSignedNum(24,00,AimVelocity,4,12);
											//=============  =======================//	
												OLED_ShowString(00,12,"EL:",12);                   
												OLED_ShowSignedNum(24,12,Encoder_Left,5,12);               
											//=============  =======================//	
												OLED_ShowString(00,24,"ER:",12);
												OLED_ShowSignedNum(24,24,Encoder_Right,5,12);   							 		
											//=============  =======================//	
												OLED_ShowString(00,36,"GP:",12);
												OLED_ShowSignedNum(36,36,Gray_KP,3,12);	
											//=============  =======================//	
												OLED_ShowString(00,48,"GD:",12);
												OLED_ShowSignedNum(36,48,Gray_KD,3,12);				
												OLED_Refresh();	
												break;
			}
}

void set_up(void)
{
  //启动TIM初始化

	
	
   HAL_TIM_Base_Start_IT(&htim2);//每10ms触发一次中断 优先级最高的中断
	 HAL_TIM_Base_Start_IT(&htim6);//每10ms触发一次中断 优先级次之
	 HAL_TIM_Base_Start_IT(&htim5);//优先级最低的中断，因为周期很长0.1秒，主要作用是定时	
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//PWM
	 HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);	
  
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);//编码器
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);	

	 HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t *)Usart2_RRRX_Buf,50);
	
		OLED_Init();	
  	OLED_Clear();	
	  Buzzer_Off();


}

void loop(void)
{
	UART_Proc();
	key_process();
	oled_proc();

}

/**************************************************************************
函数功能：10ms定时器中断，主控制函数
**************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == TIM2)//TIM2设置为10ms中断
  {
   // 电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反
	  Encoder_Left  =  Read_Encoder(4);		//读取编码器测量的电机转速
		Encoder_Right =   Read_Encoder(3);		
  // 2.将数据压入闭环控制中，计算出控制输出量
		Velocity_out = GetVelocity(Encoder_Left,Encoder_Right);// 速度环输出误差
		Scale = Read_8PIN();//读灰度调用pid生成pwm
		//------------最终输出----------------
	  PWM_out= Velocity_out;
		
		// 3.把控制输出量加载到电机上，完成最终控制
//			Moto1 = PWM_out+Gray_PID(Scale); // 左电机
//      Moto2 = PWM_out-Gray_PID(Scale); // 右电机 
		
			Moto1 = PWM_out; // 左电机
      Moto2 = PWM_out; // 右电机
		
      Limit(&Moto1,&Moto2);     // PWM限幅 
		  Set_Pwm(Moto1,Moto2);        // 加载到电机上
  }
	if(htim->Instance == TIM5)//0.1秒
	{
//			Motor_speed_process(&motorA_speed,&motorB_speed,&total_time);

	}
	if(htim->Instance == TIM6)//10ms
	{
		ReadKey();//读按键
	}
}

/*************************************************************************** 
直立环PD控制器：Kp*Ek+Kd*Ek_D
入口：Med:机械中值(期望角度)，Angle:真实角度，gyro_Y:真实角速度
出口：PMW数值
******************************************************************************/
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
	
	// 1.计算速度偏差 	//
	Encoder_Err = (AimVelocity-(Encoder_Left+Encoder_Right) );	//速度目标值//Encoder_Left+Encoder_Right 最大是 160  7200 /（160 * 50%） = 90
 
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
	
//	 Moto_Flag=Stop(Roll);	//获取是否倒下的标志
	if(1)		//一级判断系统是否正常初始化
	{
		if(1)	//二级判断//
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
	if(strcmp(Usart2_RRRX_Buf,"1")==0)
	{

	}
	else if(strcmp(Usart2_RRRX_Buf,"2")==0)
	{

	
		
	}
	else if(strcmp(Usart2_RRRX_Buf,"3")==0)
	{


		
	}	
	else if(strcmp(Usart2_RRRX_Buf,"4")==0)
	{


		
	}	
	else if(strcmp(Usart2_RRRX_Buf,"5")==0)
	{

		
		
	}		
	else if(strcmp(Usart2_RRRX_Buf,"6")==0)
	{


	}	
	else if(strcmp(Usart2_RRRX_Buf,"7")==0)
	{
		
		

	}			
			
}
