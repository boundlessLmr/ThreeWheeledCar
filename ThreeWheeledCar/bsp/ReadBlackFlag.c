#include "ReadBlackFlag.h"


uint8_t ABCDE_flag=100;
uint8_t Buzzer_flag00=0,Buzzer_flag0=0,Buzzer_flag1=1,Buzzer_flag2=1,Buzzer_flag3=1,Buzzer_flag4=1,Buzzer_flag5=1,Buzzer_flag6=1,Buzzer_flag7=1,Buzzer_flag8=1,Buzzer_flag9=1,Buzzer_flag10=1;
uint8_t coin_flag=0;
extern uint8_t xun[8];
extern uint16_t SpeedMode;
extern double motorA_speed,motorB_speed;
extern double distance;

void ReadBlag(void)
{
	if((xun[2]==0 && xun[3]==0 && xun[4]==0) || (xun[3]==0 && xun[4]==0 && xun[5]==0) || (xun[1]==0 && xun[2]==0 && xun[3]==0) || (xun[4]==0 && xun[5]==0 && xun[6]==0))// && xun[6]==0 && xun[2]==0 && xun[3]==0 && xun[4]==0 && xun[5]==0 && .   xun[1]������
	{
//		if(Buzzer_flag00 == 0)//����
//		{
//			Buzzer_flag00 = 51;
//			Buzzer_On();
//		}
		if(Buzzer_flag0 == 0)//ʶ�𵽵�0��������
		{
			
//			SpeedMode = 0;
			Buzzer_flag0 = 51;
			Buzzer_On();
		}
		if(Buzzer_flag1 == 0)//ʶ���1���̺���
		{
			ABCDE_flag=0;
			SpeedMode = 1;
			Buzzer_flag1 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag2 == 0)//ʶ���2���̺���
		{
			ABCDE_flag=1;
			SpeedMode = 2;
			Buzzer_flag2 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag3 == 0)//ʶ���3���̺��� //D����
		{
			ABCDE_flag=2;
			SpeedMode = 3;
			Buzzer_flag3 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag4 == 0)//ʶ���4���̺���
		{
			ABCDE_flag=3;
			SpeedMode = 4;
			Buzzer_flag4 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag5 == 0)//�������Ҳ���ǻص���ʼ��
		{
			ABCDE_flag=4;
			SpeedMode = 5;
			Buzzer_flag5 = 52;
			Buzzer_On();
		}
		if(Buzzer_flag6 == 0)//6
		{
			SpeedMode = 6;
			Buzzer_flag6 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag7 == 0)//7
		{
			SpeedMode = 7;
			Buzzer_flag7 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag8 == 0)//8
		{
			SpeedMode = 8;
			Buzzer_flag8 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag9 == 0)//��9�������ǵڶ�Ȧ��E����
		{
			SpeedMode = 9;
			Buzzer_flag9 = 21;
			Buzzer_On();
		}
		if(Buzzer_flag10 == 0)//��10������ʼ��
		{
			SpeedMode = 10;
			Buzzer_flag10 = 51;
			Buzzer_On();
		}
		
		
	}
	//�����Ƿ������Ķ�ʱ�����ڸ�flag�Ƕ����ģ��Ҳ���д��else if
	
//	if(Buzzer_flag00 > 1)
//	{
//		Buzzer_flag00--;
//		if(Buzzer_flag00 == 1)
//		{
//			Buzzer_Off();
//			Buzzer_flag0 = 0;
//		}
//	}
	if(Buzzer_flag0 > 1)
	{
		Buzzer_flag0--;
		if(Buzzer_flag0 == 1)
		{
			Buzzer_Off();
			Buzzer_flag1 = 0;
		}
	}
	if(Buzzer_flag1 > 1)
	{
		Buzzer_flag1--;
		if(Buzzer_flag1 == 1)
		{
			Buzzer_Off();
			Buzzer_flag2 = 0;
		}
	}
	if(Buzzer_flag2 > 1)
	{
		Buzzer_flag2--;
		if(Buzzer_flag2 == 1)
		{
			Buzzer_Off();
			Buzzer_flag3 = 0;
		}
	}
	if(Buzzer_flag3 > 1)
	{
		Buzzer_flag3--;
		if(Buzzer_flag3 == 1)
		{
			Buzzer_Off();
			Buzzer_flag4 = 0;
		}
	}
	if(Buzzer_flag4 > 1)
	{
		Buzzer_flag4--;
		if(Buzzer_flag4 == 1)
		{
			Buzzer_Off();
			Buzzer_flag5 = 0;
		}
	}
	if(Buzzer_flag5 > 2)
	{
		Buzzer_flag5--;
		if(Buzzer_flag5 == 2)
		{
			Buzzer_flag6 = 0;
			Buzzer_Off();
			
		}
	}
	if(Buzzer_flag6 > 1)
	{
		Buzzer_flag6--;
		if(Buzzer_flag6 == 1)
		{
			Buzzer_Off();
			Buzzer_flag7 = 0;
		}
	}
	if(Buzzer_flag7 > 1)
	{
		Buzzer_flag7--;
		if(Buzzer_flag7 == 1)
		{
			Buzzer_Off();
			Buzzer_flag8 = 0;
		}
	}
	if(Buzzer_flag8 > 1)
	{
		Buzzer_flag8--;
		if(Buzzer_flag8 == 1)
		{
			Buzzer_flag9 = 0;
			Buzzer_Off();
			
		}
	}
	
	if(Buzzer_flag9 > 1)
	{
		Buzzer_flag9--;
		if(Buzzer_flag9 == 1)
		{
			Buzzer_flag10 = 0;
			Buzzer_Off();
			
		}
	}
	if(Buzzer_flag10 > 1)
	{
		Buzzer_flag10--;
		if(Buzzer_flag10 == 1)
		{
//			Buzzer_flag10 = 0;
			Buzzer_Off();
			
		}
	}
}
void Coin(void)
{
	if(0<=distance &&  distance<0.44)			 		ABCDE_flag = 5;
	else if(0.44<=distance &&  distance<0.88)	ABCDE_flag = 6;	
	else if(0.88<=distance &&  distance<1.58)	ABCDE_flag = 7;	
	else if(1.58<=distance &&  distance<2.28)	ABCDE_flag = 8;		
	else if(2.28<=distance &&  distance<3.04) ABCDE_flag = 9;			
	else if(3.04<=distance &&  distance<3.80) ABCDE_flag = 10;	
	else if(3.80<=distance &&  distance<4.16) ABCDE_flag = 11;	
	else if(4.16<=distance &&  distance<4.53) ABCDE_flag = 12;	
	else if(4.53<=distance &&  distance<5.10) ABCDE_flag = 13;		
	else if(5.10<=distance)                  ABCDE_flag = 14;	
	//���Ӳ�ҵĴ���
}
//	sprintf(tx_char,"va0.val=%d\xff\xff\xff",coin_flag+4);
//	HAL_UART_Transmit(&huart2,(uint8_t *)tx_char,strlen(tx_char),50);
void ReadCoins(void)//��Ӳ��
{
	if(HAL_GPIO_ReadPin(ReadError_GPIO_Port,ReadError_Pin) == 0)
	{
		Coin();
		coin_flag=21;
		Buzzer_On();
	}
	if(coin_flag>0)
	{
		coin_flag--;
		if(coin_flag == 0)
		{
			Buzzer_Off();
		}
		
	}
}
