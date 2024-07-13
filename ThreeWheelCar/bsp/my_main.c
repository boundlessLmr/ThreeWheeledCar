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
#define NO_TIAO						 4

uint8_t Tiao_Who = NO_TIAO;
float test_num_VELp = 0.1;
float test_num_GP = 1;
float test_num_GD= 0.02;
  

//----�ٶȻ�-----//
 float Velocity,Encoder_Err,Encoder,Encoder_last,Movement=0; //�ٶȣ���������
 float Encoder_Integral;	 //��������ֵ����
 #define kp_val 4.6
 float kp=kp_val,ki=kp_val/200;
 int AimVelocity = 1000;
 uint8_t SpeedMode = 0;
 
 
//----ѭ��-----
uint8_t Black_miss_flag = 0;
int last_bias;
int last_Gray;
int Gray_Out;
uint8_t times=0;
int8_t Scale=0;
uint8_t _if_black=0;
int Gray_KP=0;
float Gray_KD=0;
// 400,GP 25 , GD -0.14
//1000,GP 30,  GD -0.22
//1600 GP 38,  GD -0.20


//-----------��������ʱ��-----------
double motorA_speed,motorB_speed;
double total_time;
double distance;
//-----------���幦�ܿ���-----------

char Usart2_RRRX_Buf[50];  //���ڽ������ݻ���buf
char Usart2_TTTX_Buf[50];  //���ڷ������ݻ���buf

char Usart1_RRRX_Buf[50];  //���ڽ������ݻ���buf
char Usart1_TTTX_Buf[50];  //���ڷ������ݻ���buf


#define RXIDLE              20
#define LONELINE						10
#define SHORTLINE						11
#define GREENCIRCLE					12
#define REDCIRCLE						13
#define REDTRI						  14
#define BLUECIRCLE					15
#define YELLOWTRI						16
uint8_t RRRXnum = RXIDLE;
uint8_t RRRXnum_reg = RXIDLE;
uint8_t countLongline = 0;
uint8_t countShortline = 0;
uint8_t stopEN = 0;



#define IDLE              0
#define BASE_1            1
#define BASE_2            2
#define BASE_3            3
#define IMPROVE_1         4
#define IMPROVE_2         5//////notice! notice!notice!û�а�����ά��Ϊ0�����
#define IMPROVE_3         6
uint8_t whichFuction = IDLE;
//����1
//����2
uint8_t blueCircleEN = 0;
uint8_t yellowTriEN = 0;
uint8_t VelocitySel = 0;
uint8_t FindBlueyellowTriEN = 0;
//����3
uint8_t stop_10sEN= 0;
//����1
uint8_t tx_lcd = 0;

//����2
uint8_t BaseCarVelocity = 0;
uint8_t RatioCarVelocity = 0;
//����3
uint8_t turnEN = 0;

u8 test111 = 0;

void FuctionIDLE(void)
{
	MediumVelocity();
}
void FuctionBase1(void)
{
		switch(RRRXnum)
		{
			case GREENCIRCLE:MediumVelocity();
			case LONELINE:countLongline++;if(countLongline == 2) {VelocityStop();countLongline = 0;}//stop 
				break;
			default:
				break;
		}
		RRRXnum = RXIDLE;
}
void FuctionBase2(void)
{
	
		switch(RRRXnum)
		{
			case GREENCIRCLE:MediumVelocity();
			case LONELINE:
										countLongline++;if(countLongline == 3) {VelocityStop(); countLongline = 0;VelocitySel =0;}
										FindBlueyellowTriEN = 0;
										countShortline = 0;
										if(countLongline == 2)
										{
											if(!blueCircleEN && !yellowTriEN) VelocitySel = 1;//A<10s,B>20s
											else if(blueCircleEN && !yellowTriEN) VelocitySel = 2;//A+B<20s,C>30s	
											else if(!blueCircleEN && yellowTriEN) VelocitySel = 3;//A+B>30s,C<20s		
											
											switch(VelocitySel){//�ٶ�3�� ///����A��
												case 1:MediumVelocity();break;
												case 2:MediumVelocity();break;
												case 3:SlowVelocity();break;
												default:break;
											}
											blueCircleEN = 0;yellowTriEN = 0;											
										}

				break;
			case SHORTLINE:
										countShortline++;if(countShortline == 4) FindBlueyellowTriEN = 1;
										if(countShortline == 1 ){//����B��
														switch(VelocitySel){//�ٶ�3��
															case 1:	SlowVelocity();break;
															case 2:MediumVelocity();break;
															case 3:SlowVelocity();break;											
															default:break;					
															}
														}
										if(countShortline == 2 ){//����C��
														switch(VelocitySel){//�ٶ�3��
															case 1:MediumVelocity();break;
															case 2:SlowVelocity();break;
															case 3:MediumVelocity();break;											
															default:break;					
															}
														}
										if(countShortline == 3 ) MediumVelocity();//����D��						
				break;
			case BLUECIRCLE:if(FindBlueyellowTriEN) blueCircleEN = 1;break;
			case YELLOWTRI:if(FindBlueyellowTriEN) yellowTriEN = 1;break;			
			default: break;
		}
		RRRXnum = RXIDLE;					
}
void FuctionBase3(void)
{
		switch(RRRXnum)
		{
			case GREENCIRCLE:MediumVelocity();			
			case LONELINE:
										countLongline++;if(countLongline == 3) {VelocityStop(); countLongline = 0;}
										countShortline = 0;
										if(countLongline == 2)stop_10sEN = 1;//�ܸ�0.5s��ͣ10s���ڶ�ȦA����
										break;
			case SHORTLINE:countShortline++;
										if((countLongline == 1) && (countShortline == 1))stop_10sEN = 1;	//�ܸ�0.5s��ͣ10s����һȦB����								
										break;	
			default: break;
		}
		RRRXnum = RXIDLE;	
}
void FuctionImprove1(void)//һȦ
{
		switch(RRRXnum)
		{
			case GREENCIRCLE:MediumVelocity();			
			case LONELINE:
										countShortline = 0;
										tx_lcd = 5;//tx_lcd ����������0��ֻ������
										sprintf(Usart1_TTTX_Buf,"va0.val=%d\xff\xff\xff",tx_lcd);
										HAL_UART_Transmit(&huart1,(uint8_t *)Usart1_TTTX_Buf,strlen(Usart1_TTTX_Buf),50);		
										tx_lcd = 0;			
										break;
			case SHORTLINE:
										if     ( distance>=55 && distance <205)  tx_lcd = 1;
										else if( distance>=205 && distance <366) tx_lcd = 2;
										else if( distance>=366 && distance <482) tx_lcd = 3;			
										else if( distance>=482 && distance <585) tx_lcd = 4;	
										else ;
										sprintf(Usart1_TTTX_Buf,"va0.val=%d\xff\xff\xff",tx_lcd);
										HAL_UART_Transmit(&huart1,(uint8_t *)Usart1_TTTX_Buf,strlen(Usart1_TTTX_Buf),50);	
										tx_lcd = 0;
										break;	
			case REDCIRCLE:VelocityStop();break;
			default: break;
		}
		RRRXnum = RXIDLE;	
}
void FuctionImprove2(void)
{
		switch(RRRXnum)
		{
			case GREENCIRCLE:MediumVelocity();			
			case REDCIRCLE:VelocityStop(); break;
			case 				 1:SlowVelocity(); break;
			case 				 2:SlowVelocity(); break;			
			case 				 3:SlowVelocity(); break;
			case 				 4:MediumVelocity(); break;
			case 				 5:MediumVelocity(); break;
			case 				 6:MediumVelocity(); break;			
			case 				 7:FastVelocity(); break;
			case 				 8:FastVelocity(); break;
			case 				 9:FastVelocity(); break;	/////notice! notice!notice!û�а�����ά��Ϊ0�����
			default				:break;
		}
		RRRXnum = RXIDLE;	
}
void FuctionImprove3(void)
{
		switch(RRRXnum)
		{
			case GREENCIRCLE:MediumVelocity();			
			case LONELINE:countLongline++;if(countLongline == 2) {VelocityStop(); countLongline = 0;}//�ȼ�����һȦֹͣ
										break;

			case REDTRI:ChangeDirection();turnEN = 1;  break;
			default: break;
		}
		RRRXnum = RXIDLE;	
}
void FuctionSelect(void)//�Ǳ𵽶����Ż�ı�GP��GD���߲Ż�++--
{
	switch (whichFuction){
		case IDLE:FuctionIDLE();break;//���ٶ��˶�
		case BASE_1:FuctionBase1();break;
		case BASE_2:FuctionBase2();break;
		case BASE_3:FuctionBase3();break;
		case IMPROVE_1:FuctionImprove1();break;		
		case IMPROVE_2:FuctionImprove2();break;		///notice! notice!notice!û�а�����ά��Ϊ0�����
		case IMPROVE_3:FuctionImprove3();break;	
		default:break;
	}
}

void key_process(void)
{
	if(bkey[1].short_flag == 1)
	{
		switch(Tiao_Who)
		{
			case TIAO_GP  :  Gray_KP+=test_num_GP;  break;
			case TIAO_GD  :  Gray_KD+=test_num_GD; break;	
			case TIAO_VELp:   kp+= test_num_VELp;ki = kp/200;break;
			case NO_TIAO  :   AimVelocity = AimVelocity+200;break;			
			default:break;
		}		
		 bkey[1].short_flag = 0;
	}
	else if(bkey[2].short_flag == 1)
	{
		switch(Tiao_Who)
		{
			case TIAO_GP  :  Gray_KP-=test_num_GP;  break;
			case TIAO_GD  :  Gray_KD-= test_num_GD;  break;	
			case TIAO_VELp:   kp-= test_num_VELp;ki = kp/200;break;
			case NO_TIAO  :   AimVelocity = AimVelocity-200;break;
		  default:break;
		}					
		
		 bkey[2].short_flag = 0;
	}	
	else if(bkey[4].short_flag == 1)
	{
		 whichFuction++; if(whichFuction == 7) whichFuction = 0;

		 bkey[4].short_flag = 0;
	}		
	else if(bkey[3].short_flag == 1)
	{
		 FuctionInit();//��ʼ�����¿�ʼ
		 sprintf(Usart2_TTTX_Buf,"0");
		 HAL_UART_Transmit(&huart2,(uint8_t *)Usart2_TTTX_Buf,strlen(Usart2_TTTX_Buf),50);			
		 bkey[3].short_flag = 0;
	}		
	else if(bkey[1].long_flag == 1)
	{
		Tiao_Who++;if(Tiao_Who == 5)Tiao_Who = 1;
		OLED_Clear();
		
		bkey[1].long_flag = 0;
	}
}
/**************************************************************************
�������ܣ�ͨ�����ڲ�������
**************************************************************************/
void UART_Proc()
{
  if((uwTick - uwTick_UART) < 200) return;//return;��������
	uwTick_UART = uwTick;
//	sprintf(Usart2_TTTX_Buf,"1");
//  HAL_UART_Transmit(&huart2,(uint8_t *)Usart2_TTTX_Buf,strlen(Usart2_TTTX_Buf),50);	
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
				case   NO_TIAO:
												OLED_ShowString(00,00,"Time",12);                    
												OLED_ShowFNum(60,00,total_time,12);		
											//=============  =======================//	
												OLED_ShowString(00,12,"route",12);                   
//												OLED_ShowSignedNum(60,12,Encoder_Left,5,12);               
											//=============  =======================//	
												OLED_ShowString(00,24,"Fuc",12);
												OLED_ShowNum(60,24,whichFuction,1,12);  	OLED_ShowString(72,24,"T",12);     OLED_ShowNum(84,24,turnEN,3,12);					 		
											//=============  =======================//	
												OLED_ShowString(00,36,"RXreg",12);
												OLED_ShowNum(60,36,RRRXnum_reg,3,12);	
												OLED_ShowString(00,48,"aim_V",12);
												OLED_ShowSignedNum(60,48,AimVelocity,4,12);							
												OLED_Refresh();		
												break;					
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
												OLED_ShowFNum(36,48,Gray_KD,12);				
												OLED_Refresh();					
												break;
				
//												OLED_ShowNum(00,24,xun[0],1,12);               OLED_ShowString(84,00,"08",12);     
//												OLED_ShowNum(12,24,xun[1],1,12); 
//											//=============  =======================//	
//												OLED_ShowNum(24,24,xun[2],1,12);                 
//												OLED_ShowNum(36,24,xun[3],1,12);               
//											//=============  =======================//	
//												OLED_ShowNum(48,24,xun[4],1,12);                 
//												OLED_ShowNum(60,24,xun[5],1,12);      							 		
//											//=============  =======================//	
//												OLED_ShowNum(72,24,xun[6],1,12);                 
//												OLED_ShowNum(84,24,xun[7],1,12);     		
//												OLED_Refresh();					
//												break;
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
												OLED_ShowFNum(36,48,Gray_KD,12);	
												
												OLED_ShowNum(80,36,test111,3,12);					
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
	key_process();
	oled_proc();
	FuctionSelect();
}

/**************************************************************************
�������ܣ�10ms��ʱ���жϣ������ƺ���
**************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM2)//TIM2����Ϊ10ms�ж�
		{
		 // �������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��
				Encoder_Left  =  Read_Encoder(4);        //��ȡ�����������ĵ��ת��
					Encoder_Right =   Read_Encoder(3);        
		// 2.������ѹ��ջ������У���������������
					Velocity_out = GetVelocity(Encoder_Left,Encoder_Right);// �ٶȻ�������
					Scale = Read_8PIN();//���Ҷȵ���pid����pwm
					//------------�������----------------
				PWM_out= Velocity_out;
					
					Gray_Out=Gray_PID(Scale);//�����ҶȻ�
					// 3.�ѿ�����������ص�����ϣ�������տ���
					if(turnEN != 0)
					{
						Moto1 = 800;Moto2 = -800;
					}
					else{						
						if((xun[2]==0&&xun[3]==0&&xun[4]==0) || (xun[3]==0&&xun[4]==0&&xun[5]==0))//��������м����ڣ���ֻ����ǰһ��ʱ���ƽ���ҶȻ�
						{
								_if_black=1;
								Moto1 = PWM_out+last_Gray;
								Moto2 = PWM_out-last_Gray;
						}
						else//�������ѭ��������������ҶȻ�
						{
								Moto1 = PWM_out+Gray_Out; // ����
								Moto2 = PWM_out-Gray_Out; // �ҵ�� 
						}						
							Limit(&Moto1,&Moto2);     // PWM�޷� 
							Set_Pwm(Moto1,Moto2);        // ���ص������
						
						if(_if_black==0)//������Ǻڣ�Ҳ����������ѭ��������ʱ�̼�¼֮ǰ�ĻҶ����ֵ
						{
								last_Gray=Paixu(&times,Gray_Out);
						}
						else//������ڶ��ڣ���ֹͣ��¼����λ
						{
								_if_black=0;
						}
					}
							Limit(&Moto1,&Moto2);     // PWM�޷� 
							Set_Pwm(Moto1,Moto2);        // ���ص������					
		}
		if(htim->Instance == TIM5)//0.1��
		{
				Motor_speed_process(&motorA_speed,&motorB_speed,&total_time);
				if(stop_10sEN != 0){
					stop_10sEN++;
					if(stop_10sEN ==   6)  {VelocityStop();}
					if(stop_10sEN == 116) {MediumVelocity();stop_10sEN =0;}//10.5s
				}
				if(turnEN != 0){
					turnEN++;
					if(turnEN == 20){turnEN = 0; kp=kp_val,ki=kp_val/200;MediumVelocity();}
				}
		}
		if(htim->Instance == TIM6)//10ms
		{
				key_serv_long();
		}
}

/*************************************************************************** 
ֱ����PD��������Kp*Ek+Kd*Ek_D
��ڣ�Med:��е��ֵ(�����Ƕ�)��Angle:��ʵ�Ƕȣ�gyro_Y:��ʵ���ٶ�
���ڣ�PMW��ֵ
******************************************************************************/
float Gray_PID(int bias)
{
	float res = Gray_KP*bias+Gray_KD*(bias-last_bias);
	last_bias = bias;
	return res;
}
void FuctionInit(void)
{
	FindBlueyellowTriEN =0;
	countLongline = 0;
	countShortline =0;
	VelocitySel = 0;
	stop_10sEN = 0;
	tx_lcd = 0;
	MediumVelocity();
	if(whichFuction == 4) FastVelocity();
}
void ChangeDirection(void)
{
  kp=0,ki=0;
	Gray_KP = 0;
	Gray_KD = 0;	
	Moto1 = 800;
	Moto2 = -800;
}
void VelocityStop(void)
{
		AimVelocity	 = 0;
		Gray_KP = 0;
		Gray_KD = 0;
}
void SlowVelocity(void)
{
		AimVelocity	 = 400;
		Gray_KP = 25;
		Gray_KD = 0.14;
}
void MediumVelocity(void)
{
		AimVelocity	 = 1000;
		Gray_KP = 30;
		Gray_KD = 0.22;
}
void FastVelocity(void)
{
		AimVelocity	 = 1600;
		Gray_KP = 38;
		Gray_KD = -0.20;	
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
	if(huart->Instance==USART2)
	{
		RRRXnum = Usart2_RRRX_Buf[0] - 48;			
	}
}
