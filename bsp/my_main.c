#include "my_main.h"

//-----ȫ�ֱ���---
extern struct Bkeys bkey[];
extern struct key_data key_data1[];
extern uint8_t xun[8];
__IO uint32_t uwtick_key=0;
extern int8_t arr_bit[8];
extern uint8_t Buzzer_flag0,Buzzer_flag1,Buzzer_flag2,Buzzer_flag3,Buzzer_flag4,Buzzer_flag5,Buzzer_flag6,Buzzer_flag7,Buzzer_flag8,Buzzer_flag9,Buzzer_flag10;

__IO uint32_t uwTick_UART = 0;
int Vertical_out,Velocity_out,Turn_out; // ֱ����&�ٶȻ�&ת�򻷵��������
int Encoder_Left,Encoder_Right,Balance_PWM;
int Moto1,Moto2;
int PWM_out,Turn_Pwm = 0; //�ջ����
		
u8 AIN1,AIN2;//���Ƶ������ת
u8 BIN1,BIN2;
u8 Moto_Flag=0;//������Ʊ�־
u8 Start_Flag=0;//ϵͳ��ʼ����ɱ�־

float Pitch,Roll,Yaw;	        // Pitch�������ǣ�Roll������ǣ�Yaw��ƫ����
short gyrox,gyroy,gyroz;        // ���ٶ�
short aacx,aacy,aacz;           // ���ٶ�
float Med_Angle=0;//��е��ֵ����ʹ��С������ƽ��ס�ĽǶ� 



/*UART ���*/
char Usart2_RX_Buf[50];  //���ڽ������ݻ���buf
char Usart3_TX_Buf[50];
char temp[50]; 

int8_t Scale=0;


u8 Flag_forward = 0, Flag_retreat = 0, Flag_left = 0, Flag_right = 0;//����ң�ر�־





//----����ѡ��-----// 
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

//----ֱ����-----// 
//float Bias, Vertical_Kp= 460,Vertical_Kd= -2.1;
//float Bias, Vertical_Kp= 460 * 0.6,Vertical_Kd= -2.1 * 0.6;

//float Bias, Vertical_Kp= 276,Vertical_Kd= -1.76;//1.86
float Bias, Vertical_Kp= 276,Vertical_Kd= -1.4;//�ٶ�800 Vertic_kd = -1.8 Velocity_kp = 6.5 // �ٶ�200~600 Vertic_kd = -1.5 Velocity_kp = 4.6
float Med = 10;//14

//float Med = 5.9;
//14.5   15.2
//float Med = 13.75;
//float Med = 10;

//----�ٶȻ�-----/*0
 float Velocity,Encoder_Err,Encoder,Encoder_last,Movement=0; //�ٶȣ���������
 float Encoder_Integral;	 //��������ֵ����
 #define kp_val 4.6
 float kp=kp_val,ki=kp_val/200;
 int jiasu_flag = -50;//���ʾ��ʱ50*0.1���ٷ���
 int AimVelocity = 0;
 uint8_t SpeedMode = 0;
 
 
//----ѭ��-----
uint8_t Black_miss_flag = 0;
int last_bias;
//int Gray_KP=0,Gray_KD= 0 ;
//int Gray_KP=0,Gray_KD= -0 ;
int Gray_KP=35,Gray_KD=-12;


//��������ʱ��
double motorA_speed,motorB_speed;
double total_time;
double distance;
 
/*�¶�*/
float tem;


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



void key_process(void)
{

	if(key_data1[0].key_return==1)//����KEY1 
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
	
	if(key_data1[2].key_return==1)//���ǵ��ĸ�����
	{
		Tiao_Who++;if(Tiao_Who== 5)Tiao_Who = 0;	
		OLED_Clear();				
		
		//��������ǵ��ĸ����������¸�λ���е���Ŀ��־λ���л�ģʽ
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
�������ܣ�ͨ�����ڲ�������
**************************************************************************/
void UART_Proc()
{
  if((uwTick - uwTick_UART) < 200) return;//return;��������
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
	


//	if ((mpu_dmp_get_data(&Pitch, &Roll, &Yaw) == 0) && (MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz) == 0)) //DMP��ȡŷ���ǣ�digital motion processor�����˶�������
//   {
//		//�����ǣ�����ǣ�ƫ����
//		 sprintf(Usart3_TX_Buf,"pitch=%.3f roll=%.3f yaw=%.3f\r\n", Pitch, Roll, Yaw);   //���ڴ�ӡŷ����
//     HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//  	 sprintf(Usart3_TX_Buf,"gyrox=%d   gyroy=%d  gyroz=%d \r\n", gyrox, gyroy, gyroz);	// ���ٶ�
//		 HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//		 sprintf(Usart3_TX_Buf,"-------------------------------------------\r\n");
//     HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);
//			
//	 }

//		 sprintf(Usart3_TX_Buf,"E_Left %d E_right %d\r\n",Encoder_Left,Encoder_Right);
//     HAL_UART_Transmit(&huart1,(uint8_t *)Usart3_TX_Buf,strlen(Usart3_TX_Buf),50);


}

void oled_proc()
{//��x:0~127    ��y:0~63
		switch(Tiao_Who)
		{

			case TIAO_VERp:
											//============= ��һ�� ��ֱ����=======================//	
												OLED_ShowString(00,00,"P:",12);                    OLED_ShowFNum(96,00,motorA_speed*0.6*0.85 ,12);//
												OLED_ShowFNum(24,00,Vertical_Kp,12);
											//=============  �ڶ���=======================//	
												OLED_ShowString(00,12,"D:",12);                   
												OLED_ShowFNum(24,12,Vertical_Kd,12);               OLED_ShowFNum(96,12,total_time,12);
											//=============  ������=======================//	
												OLED_ShowString(00,24,"Pi:",12);
												OLED_ShowFNum(36,24,Pitch,12);   							 		 OLED_ShowFNum(96,24,motorB_speed*0.6*0.85,12);
											//=============  ������=======================//	
												OLED_ShowString(00,36,"gy:",12);
												OLED_ShowFNum(36,36,AimVelocity,12);	
											//=============  ��5��=======================//	
												OLED_ShowString(00,48,"vp:",12);
												OLED_ShowFNum(36,48,Vertical_out,12);			
											
												OLED_Refresh();	break;					
			
			case TIAO_VERd:
											//============= ��һ�� ��ֱ����=======================//	
												OLED_ShowString(00,00,"P:",12);                   
												OLED_ShowFNum(24,00,Vertical_Kp,12);
											//=============  �ڶ���=======================//	
												OLED_ShowString(00,12,"D:",12);                   
												OLED_ShowFNum(24,12,Vertical_Kd,12);
											//=============  ������=======================//	
												OLED_ShowString(00,24,"pi:",12);
												OLED_ShowFNum(36,24,Pitch,12);
											//=============  ������=======================//	
												OLED_ShowString(00,36,"gy:",12);
												OLED_ShowFNum(36,36,gyroy,12);	
											//=============  ��5��=======================//	
												OLED_ShowString(00,48,"vp:",12);
												OLED_ShowFNum(36,48,Vertical_out,12);			
											
												OLED_Refresh();	break;					
						
			case TIAO_VELp:
											//============= ��һ�� ���ٶȻ�=======================//	
												OLED_ShowString(00,00,"P:",12);                   
												OLED_ShowFNum(24,00,kp,12);         
											//=============  �ڶ���=======================//	
												OLED_ShowString(00,12,"i:",12);                   
												OLED_ShowFNum(24,12,ki,12);					
											//=============  ������=======================//	
												OLED_ShowString(00,24,"le:",12);
												OLED_ShowSignedNum(24,24,Encoder_Left,4,12);   
												
									//			OLED_ShowString(00,24,"er:",12);
									//		  OLED_ShowSignedNum(24,24,Encoder_Err,4,12);			
											//=============  ������=======================//	
//												OLED_ShowString(00,36,"ri:",12);
//												OLED_ShowSignedNum(24,36,Encoder_Right,4,12);
												OLED_ShowString(00,36,"tt:",12);
											  OLED_ShowSignedNum(24,36,test111,4,12);			
											//=============  ��5��=======================//		
												OLED_ShowFNum(00,48,Pitch,12);	  OLED_ShowFNum(60,48,PWM_out,12);	 
											// û�е�6��
									
												OLED_Refresh();break;			
			case TIAO_GP:
											//============= ��һ�� ���ٶȻ�=======================//	
												OLED_ShowString(00,00,"GP",12);                   
												OLED_ShowFNum(24,00,Gray_KP,12);
											//=============  �ڶ���=======================//	
												OLED_ShowString(00,12,"GD",12);                   
												OLED_ShowFNum(24,12,Gray_KD,12);	
												OLED_Refresh();break;			

			case TIAO_GD:
											//============= ��һ�� ���ٶȻ�=======================//	
												OLED_ShowString(00,00,"GD",12);                   
												OLED_ShowFNum(24,00,Gray_KD,12);
											//=============  �ڶ���=======================//	
												OLED_ShowString(00,12,"GP:",12);                   
												OLED_ShowFNum(24,12,AimVelocity,12);
												OLED_Refresh();break;			


		}

}

void set_up(void)
{
  //����TIM��ʼ��

	
	
   HAL_TIM_Base_Start_IT(&htim2);//ÿ10ms����һ���ж� ���ȼ���ߵ��ж�
	 HAL_Delay(200);
	 HAL_TIM_Base_Start_IT(&htim6);//ÿ10ms����һ���ж� ���ȼ���֮
	
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//PWM
	 HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);	
  
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);//������
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
  //-----DMP��ʼ��----
	
  while(mpu_dmp_init())//�ɹ�����0�����򷵻�1
  {
		uint8_t res;
		res = mpu_dmp_init();
		HAL_Delay(300);
		OLED_ShowNum(00,12,res,1,12);
		OLED_Refresh();	
  }
  	OLED_Clear();	
		OLED_ShowString(103,00,"on",12);  
	Start_Flag = 1; //��־ϵͳ��ʼ���ɹ�
	OLED_Refresh();	
//	OLED_Refresh();	
	
  
	
	
	HAL_TIM_Base_Start_IT(&htim5);//���ȼ���͵��жϣ���Ϊ���ںܳ�0.1�룬��Ҫ�����Ƕ�ʱ
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
�������ܣ�10ms��ʱ���жϣ������ƺ���
**************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == TIM2)//TIM2����Ϊ10ms�ж�
  {
		// 1.�ɼ�����������&MPU6050�Ƕ���Ϣ
   // �������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��
	  mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	    // ��ȡ�Ƕ�
    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);  // ��ȡ���ٶ�

	  Encoder_Left  =  Read_Encoder(4);		//��ȡ�����������ĵ��ת��
		Encoder_Right =   Read_Encoder(3);		

  // 2.������ѹ��ջ������У���������������
		Velocity_out = GetVelocity(Encoder_Left,Encoder_Right);// �ٶȻ�������
	  Vertical_out = Vertical(Pitch,gyroy);// ֱ�������PWM
		
		Scale = Read_8PIN();//���Ҷȵ���pid����pwm
		//------------�������----------------
	  PWM_out= Vertical_out+Velocity_out;
		
		// 3.�ѿ�����������ص�����ϣ�������տ���
			Moto1 = PWM_out+Gray_PID(Scale); // ����
      Moto2 = PWM_out-Gray_PID(Scale); // �ҵ��
      Limit(&Moto1,&Moto2);     // PWM�޷� 
		  Set_Pwm(Moto1,Moto2);        // ���ص������
  }
	if(htim->Instance == TIM5)//0.1��
	{
		if(Buzzer_flag0!= 0 && Buzzer_flag5<=1) 
		{
			Motor_speed_process(&motorA_speed,&motorB_speed,&total_time);
			distance = (motorA_speed+motorB_speed)*0.5f*0.85f*0.6f;
		}//���ǵ�һ����������˵�һ���߲���δ����������߾͵��ú�����ʱ�Ͳ��
		if(Tiao_Who == TIAO_VERp) Timu_1(SpeedMode);//������������Ŀ���л�
		else if(Tiao_Who == TIAO_VERd) Timu_2(SpeedMode);
		else if(Tiao_Who == TIAO_VELp) Timu_3(SpeedMode);
	}
	if(htim->Instance == TIM6)//10ms
	{
		ReadBlag();//��ÿ����ɫ��־��
		ReadCoins();//��Ӳ��
		ReadKey();//������
	}
}

/*************************************************************************** 
ֱ����PD��������Kp*Ek+Kd*Ek_D
��ڣ�Med:��е��ֵ(�����Ƕ�)��Angle:��ʵ�Ƕȣ�gyro_Y:��ʵ���ٶ�
���ڣ�PMW��ֵ
******************************************************************************/
int Vertical(float Angle,float Gyro_Y) 
{

	Bias = Med-Angle;//ֱ��ƫ�� //angle = -0.23��ƽ����
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
�ٶȻ�PI��������Kp*Ek+Ki*Ek_S(Ek_S��ƫ��Ļ���)
��ڣ����ұ������⵽����ֵ
���ڣ�                   
**********************************************************************/
int GetVelocity(int Encoder_left,int Encoder_right)	
{
	
	// 1.�����ٶ�ƫ�� 	
	Encoder_Err = ((Encoder_Left+Encoder_Right) - AimVelocity);	//�ٶ�Ŀ��ֵ//Encoder_Left+Encoder_Right ����� 160  7200 /��160 * 50%�� = 90
 
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
	
	 Moto_Flag=Stop(Roll);	//��ȡ�Ƿ��µı�־
	if(Start_Flag == 1)		//һ���ж�ϵͳ�Ƿ�������ʼ��
	{
		if(Moto_Flag==0)	//�����ж�//
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
