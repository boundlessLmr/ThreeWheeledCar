#include "my_main.h"

//-----ȫ�ֱ���---
extern struct Bkeys bkey[];
extern struct key_data key_data1[];
extern uint8_t xun[8];
__IO uint32_t uwtick_key=0;
extern int8_t arr_bit[8];

__IO uint32_t uwTick_UART = 0;
int Vertical_out,Velocity_out,Turn_out; // ֱ����&�ٶȻ�&ת�򻷵��������
int Encoder_Left,Encoder_Right,Balance_PWM;
int Moto1,Moto2;
int PWM_out,Turn_Pwm = 0; //�ջ����
		
u8 AIN1,AIN2;//���Ƶ������ת
u8 BIN1,BIN2;
u8 Moto_Flag=0;//������Ʊ�־
u8 Start_Flag=0;//ϵͳ��ʼ����ɱ�־

//----����ѡ��-----// 


#define TIAO_VELp          1
#define TIAO_GP            2
#define TIAO_GD            3


uint8_t Tiao_Who = TIAO_GD;
float test_num_VELp = 0.1;
float test_num_GP = 1;
float test_num_GD= 1;



//----�ٶȻ�-----//
 float Velocity,Encoder_Err,Encoder,Encoder_last,Movement=0; //�ٶȣ���������
 float Encoder_Integral;	 //��������ֵ����
 #define kp_val 4.6
 float kp=kp_val,ki=kp_val/200;
 int AimVelocity = 600;
 uint8_t SpeedMode = 0;
 
 
//----ѭ��-----
uint8_t Black_miss_flag = 0;
int last_bias;
int8_t Scale=0;
int Gray_KP=60,Gray_KD=-16;

//��������ʱ��
double motorA_speed,motorB_speed;
double total_time;
double distance;
 
//-----------���幦�ܿ���-----------

char Usart2_RRRX_Buf[50];  //���ڽ������ݻ���buf
char Usart2_TTTX_Buf[50];  //���ڷ������ݻ���buf

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
//����1
//����2
uint8_t blueEN = 0;
uint8_t yellowEN = 0;
uint8_t VelocitySel = 0;
uint8_t FindBlueYellowEN = 0;
//����3
uint8_t stop_10sEN= 0;
//����1
uint8_t tx_lcd = 0;

//����2
uint8_t BaseCarVelocity = 0;
uint8_t RatioCarVelocity = 0;
//����3
uint8_t turnEN = 0;


//----�Ӻ�������-----
void UART_Proc(void);
int Vertical(float Angle,float Gyro_Y);			 				//ֱ����
int GetVelocity(int Encoder_left,int Encoder_right);				//�ٶȻ�
u8 Stop(signed int angle);    		//���±���
void Limit(int *motoA,int *motoB);  //����ٶ��޷�
void Set_Pwm(int Moto1,int Moto2);	//����PWM�������

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
											
											switch(VelocitySel){//�ٶ�3��
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
														switch(VelocitySel){//�ٶ�3��
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
														switch(VelocitySel){//�ٶ�3��
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
{/****����1********/
	switch (whichFuction){
		case IDLE:
					break;
		case BASE_1:/*********����1********/
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
		case IMPROVE_1:/**************����3***********/
				
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

	if(key_data1[0].key_return==1)//����KEY1 
	{													
		switch(Tiao_Who)
		{
			case TIAO_GP  :  Gray_KP+=test_num_GP;  break;
			case TIAO_GD  :  Gray_KD+=test_num_GD; break;	
			case TIAO_VELp:   kp+= test_num_VELp;ki = kp/200;break;
		}		
	
		key_data1[0].key_return = 0;		
	}
	
	else if(key_data1[1].key_return==1)//����KEY2 ����
	{

	
		key_data1[1].key_return = 0;
	}	
	

	
	else if(key_data1[3].key_return==1)//����KEY3
	{
		switch(Tiao_Who)
		{
			case TIAO_GP  :  Gray_KP-=test_num_GP;  break;
			case TIAO_GD  :  Gray_KD-= test_num_GD;  break;	
			case TIAO_VELp:   kp-= test_num_VELp;ki = kp/200;break;
		}				
		
		key_data1[3].key_return = 0;
	}	
	else if(key_data1[2].key_return==1)//����KEY3 ��4��
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
�������ܣ�ͨ�����ڲ�������
**************************************************************************/
void UART_Proc()
{
  if((uwTick - uwTick_UART) < 200) return;//return;��������
	uwTick_UART = uwTick;
	
		if((Moto1>=7000 || Moto1<=-7000) && (Moto2 >= 7000 || Moto2<=-7000))
		{
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_1);
		}
		
		
		


}

void oled_proc()
{//��x:0~127    ��y:0~63

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
  //����TIM��ʼ��

	
	
   HAL_TIM_Base_Start_IT(&htim2);//ÿ10ms����һ���ж� ���ȼ���ߵ��ж�
	 HAL_TIM_Base_Start_IT(&htim6);//ÿ10ms����һ���ж� ���ȼ���֮
	 HAL_TIM_Base_Start_IT(&htim5);//���ȼ���͵��жϣ���Ϊ���ںܳ�0.1�룬��Ҫ�����Ƕ�ʱ	
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//PWM
	 HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);	
  
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);//������
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
�������ܣ�10ms��ʱ���жϣ������ƺ���
**************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == TIM2)//TIM2����Ϊ10ms�ж�
  {
   // �������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��
	  Encoder_Left  =  Read_Encoder(4);		//��ȡ�����������ĵ��ת��
		Encoder_Right =   Read_Encoder(3);		
  // 2.������ѹ��ջ������У���������������
		Velocity_out = GetVelocity(Encoder_Left,Encoder_Right);// �ٶȻ�������
		Scale = Read_8PIN();//���Ҷȵ���pid����pwm
		//------------�������----------------
	  PWM_out= Velocity_out;
		
		// 3.�ѿ�����������ص�����ϣ�������տ���
//			Moto1 = PWM_out+Gray_PID(Scale); // ����
//      Moto2 = PWM_out-Gray_PID(Scale); // �ҵ�� 
		
			Moto1 = PWM_out; // ����
      Moto2 = PWM_out; // �ҵ��
		
      Limit(&Moto1,&Moto2);     // PWM�޷� 
		  Set_Pwm(Moto1,Moto2);        // ���ص������
  }
	if(htim->Instance == TIM5)//0.1��
	{
//			Motor_speed_process(&motorA_speed,&motorB_speed,&total_time);

	}
	if(htim->Instance == TIM6)//10ms
	{
		ReadKey();//������
	}
}

/*************************************************************************** 
ֱ����PD��������Kp*Ek+Kd*Ek_D
��ڣ�Med:��е��ֵ(�����Ƕ�)��Angle:��ʵ�Ƕȣ�gyro_Y:��ʵ���ٶ�
���ڣ�PMW��ֵ
******************************************************************************/
int Gray_PID(int bias)
{
	int res = Gray_KP*bias+Gray_KD*(bias-last_bias);
	last_bias = bias;
	return res;
}
/********************************************************************* 
�ٶȻ�PI��������Kp*Ek+Ki*Ek_S(Ek_S��ƫ��Ļ���)
��ڣ����ұ������⵽����ֵ
���ڣ�                   
**********************************************************************/
int GetVelocity(int Encoder_left,int Encoder_right)	
{ 
	
	// 1.�����ٶ�ƫ�� 	//
	Encoder_Err = (AimVelocity-(Encoder_Left+Encoder_Right) );	//�ٶ�Ŀ��ֵ//Encoder_Left+Encoder_Right ����� 160  7200 /��160 * 50%�� = 90
 
	// 2.���ٶ�ƫ�����--��ͨ�˲�--
  // low_out = (1-a)*Ek+a*low_out_last	
	Encoder = Encoder_Err*0.3 + Encoder_last*0.7;// ʹ�ò��θ���ƽ��
	Encoder_last = Encoder; 							// ��ֹ�ٶȹ���Ӱ��ֱ��������������

	// 3.���ٶ�ƫ����ֳ�λ��,ң�ص��ٶ�ͨ�����������ٶȿ������������ٶ�ͻ���ֱ�����Ƶ�Ӱ��	
  Encoder_Integral += Encoder-Movement;	



	// 4.�����޷�	
	if(Encoder_Integral>3000)  	Encoder_Integral=3000;   
	if(Encoder_Integral<-3000)	   Encoder_Integral=-3000;           	

	if(Moto_Flag == 1||Start_Flag ==0) 			Encoder_Integral=0;     		//===����رպ���߸�λ�������
    //5.�ٶȻ��������	
  Velocity=Encoder*kp+Encoder_Integral*ki;

	return Velocity;



}



/*************************************************************************** 
�������ܣ����Ƶ��
******************************************************************************/
void Contrl(void)
{

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, BIN1);	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, BIN2);
	
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, AIN1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, AIN2);

}




/**************************************************************************
�������ܣ�����쳣�رպ���
��ڲ������Ƕ�
����  ֵ��1���رգ�0�����ر�
**************************************************************************/	 		
u8 Stop(signed int angle)
{
	    u8 temp=0;
			if(angle<-50||angle>50)
			{	                                //===��Ǵ���40�ȹرյ��
				temp=1;                   		   //===Flag_Stop��1�رյ��
				Moto1 = 0;
				Moto2 = 0;
      }
	
		return temp;
}



/**************************************************************************
�������ܣ����ת�����ƺ���
��ڲ������ջ������������ֵ
**************************************************************************/	
void Set_Pwm(int Moto1,int Moto2)
{
	
//	 Moto_Flag=Stop(Roll);	//��ȡ�Ƿ��µı�־
	if(1)		//һ���ж�ϵͳ�Ƿ�������ʼ��
	{
		if(1)	//�����ж�//
		{
			if(Moto1>0)  AIN1 = 0,AIN2 = 1;
			else			 AIN1 = 1,AIN2 = 0;				
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,abs(Moto1));		 
			
			if(Moto2>0)  BIN1 = 0,BIN2 = 1;		
			else		    BIN1 = 1 ,BIN2 = 0;
    		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,abs(Moto2)); //0-7200
		}
			
		else//���¾͹رյ��
		{
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);	//4500-6000
		   __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,0);	//4500-6000
		}
		 Contrl();
	}
}


/**************************************************************************
�������ܣ����Ƶ���ٶ�
��ڲ������ջ������������ֵ
**************************************************************************/	
void Limit(int *motoA,int *motoB)
{
	if(*motoA>7000)*motoA=7000;//���7200
	if(*motoA<-7000)*motoA=-7000;

	if(*motoB>7000)*motoB=7000;
	if(*motoB<-7000)*motoB=-7000;
} 


/**************************************************************************
�������ܣ�����ң������USART3�����жϻص�
���ţ�		 
**************************************************************************/
//���ڿ����жϻص�����
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
