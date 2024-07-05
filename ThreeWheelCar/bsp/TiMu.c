#include "TiMu.h"

extern int jiasu_flag;
extern int AimVelocity;
extern int8_t arr_bit[8];
extern int Gray_KP;
extern int Gray_KD;


void Timu_1(uint8_t SpeedMode)//��һ����Ŀ
{
	if(SpeedMode == 0)//��0��ʼ����
	{
		if(jiasu_flag<0)jiasu_flag++;//�ȴ�����
			 else if( jiasu_flag < 20)//1���ڼ��ٵ�600
			{
			jiasu_flag++;
			AimVelocity+=30;
			}
	}
	if(SpeedMode == 5)//�����������Ҳ������ʼ�߾�ͣ��
	{
		if(jiasu_flag < 40)
		{
			jiasu_flag++;
			AimVelocity -= 20;
		}
		else
		{
			AimVelocity = 0;
		}
	}
}

void Timu_2(uint8_t SpeedMode)//�ڶ�С��
{
	if(SpeedMode == 0)//��0��ʼ����
		{
	//			if(jiasu_flag < 50)//
	//			{
	//				jiasu_flag++;
	//			}
	//		
			 if(jiasu_flag<-10)jiasu_flag++;//�ȴ���������
			 else if( jiasu_flag < 10)//1���ڼ��ٵ�600
			{
			jiasu_flag++;
			AimVelocity+=30;			
			}
		}
		
//		else if(SpeedMode == 1)//ģʽ1
//		{
////			arr_bit[0]=50,arr_bit[1]=40,arr_bit[2]=20,arr_bit[3]=10,arr_bit[4]=-10,arr_bit[5]=-20,arr_bit[6]=-40,arr_bit[7]=-50;
////			
////			if(jiasu_flag < 20)//1���ڼ��ٵ�600
////			{
////				jiasu_flag++;
////				AimVelocity -= 10;
////			}
//		}
		else if(SpeedMode == 2)//�ڶ������������һ��
		{
//			Gray_KP = 30,Gray_KD = -10;
			if(jiasu_flag < 30)//1���ڼ��ٵ�400
			{
				jiasu_flag++;
				AimVelocity -= 10;
			}
		}
		else if(SpeedMode == 3)//��������Ҳ����D���򣬼���20��Ȼ����
		{
			Gray_KP = 15,Gray_KD = 5;;
			if(jiasu_flag < 40)//1���ڼ��ٵ�10
			{
				jiasu_flag++;
				AimVelocity -= 38;
				
			}
			else if(jiasu_flag < 200)//ͣ��16��
			{
				jiasu_flag++;
//				AimVelocity = 0;
			}
			else if(jiasu_flag < 210)//1���Ӽ��ٵ�150
			{
				jiasu_flag++;
				AimVelocity += 13;
			}
		}
		else if(SpeedMode == 4)//���ٵ�300
		{
			Gray_KP = 25,Gray_KD = 8;
//			arr_bit[0]=70,arr_bit[1]=55,arr_bit[2]=30,arr_bit[3]=10,arr_bit[4]=-10,arr_bit[5]=-30,arr_bit[6]=-55,arr_bit[7]=-70;
			if(jiasu_flag < 220)
			{
				jiasu_flag++;
				AimVelocity += 15;
			}
		}
		else if(SpeedMode == 5)//�ص���ʼ��ͣ��
		{
			if(jiasu_flag < 240)
			{
				jiasu_flag++;
				AimVelocity -= 15;
			}
			else
			{
				AimVelocity = 0;
			}
		}
}

void Timu_3(uint8_t SpeedMode)
{
	if(SpeedMode == 0)//��ʼģʽ
		{
	//			if(jiasu_flag < 50)//5���ʼ����
	//			{
	//				jiasu_flag++;
	//			}
	//		
			if(jiasu_flag<-10)jiasu_flag++;
			 else if( jiasu_flag < 10)//2���ڼ��ٵ�600
			{
			jiasu_flag++;
			AimVelocity+=30;			
			}
		}
	else if (SpeedMode == 2)//��΢���٣�300
	{
		if(jiasu_flag < 40)
		{
			jiasu_flag++;
			AimVelocity-=10;
		}
	}
	else if (SpeedMode == 3)//��һȦ��D������ͣ��10��
	{
		 if( jiasu_flag < 70)//3���ڼ��ٵ�0
			{
			jiasu_flag++;
			AimVelocity-=10;
			}
		else if(jiasu_flag < 170)
		{
			jiasu_flag++;
			AimVelocity=0;
		}
		else if(jiasu_flag < 190)
		{
			jiasu_flag++;
			AimVelocity+=30;//��ʼ�ٶ�600
		}
	}
	else if(SpeedMode == 8)//��΢���٣�300
	{
		if(jiasu_flag < 220)
		{
			AimVelocity-=10;
		}
	}
	else if(SpeedMode == 9)//�ڶ�Ȧ��E������ͣ��10��
	{
		if( jiasu_flag < 250)//
			{
			jiasu_flag++;
			AimVelocity-=10;
			}
		else if(jiasu_flag < 350)
		{
			jiasu_flag++;
			AimVelocity=0;
		}
		else if(jiasu_flag < 370)//�ٴ�����400
		{
			jiasu_flag++;
			AimVelocity+=30;
		}
	}
	else if(SpeedMode == 10)//�ڶ�Ȧ�ص���ʼ��ͣ��
	{
		if(jiasu_flag < 390)
		{
			jiasu_flag++;
			AimVelocity-=30;
		}
		else
		{
			AimVelocity = 0;
		}
	}
	
}

