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
			 else if( jiasu_flag < 10)//1���ڼ��ٵ�600
			{
			jiasu_flag++;
			AimVelocity+=40;
			}
	}
	if(SpeedMode == 5)//�����������Ҳ������ʼ�߾�ͣ��
	{
		if(jiasu_flag < 30)
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
			 if(jiasu_flag<0)jiasu_flag++;//�ȴ���������
			 else if( jiasu_flag < 10)//1���ڼ��ٵ�600
			{
			jiasu_flag++;
			AimVelocity+=60;			
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
			Gray_KP = 10;
			if(jiasu_flag < 40)//1���ڼ��ٵ�0
			{
				jiasu_flag++;
				AimVelocity -= 30;
				
			}
			else if(jiasu_flag < 200)//ͣ��16��
			{
				jiasu_flag++;
//				AimVelocity = 0;
			}
			else if(jiasu_flag < 210)//1���Ӽ��ٵ�200
			{
				jiasu_flag++;
				AimVelocity += 10;
			}
		}
		else if(SpeedMode == 4)//���ٵ�200
		{
//			Gray_KP = 35;
//			arr_bit[0]=70,arr_bit[1]=55,arr_bit[2]=30,arr_bit[3]=10,arr_bit[4]=-10,arr_bit[5]=-30,arr_bit[6]=-55,arr_bit[7]=-70;
			if(jiasu_flag < 220)
			{
				jiasu_flag++;
				AimVelocity += 10;
			}
		}
		else if(SpeedMode == 5)//�ص���ʼ��ͣ��
		{
			if(jiasu_flag < 230)
			{
				jiasu_flag++;
				AimVelocity -= 40;
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
			if(jiasu_flag<0)jiasu_flag++;
			 else if( jiasu_flag < 10)//1���ڼ��ٵ�600
			{
			jiasu_flag++;
			AimVelocity+=60;			
			}
		}
	else if (SpeedMode == 3)//��һȦ��D������ͣ��10��
	{
		 if( jiasu_flag < 30)//2���ڼ��ٵ�0
			{
			jiasu_flag++;
			AimVelocity-=30;
			}
		else if(jiasu_flag < 130)
		{
			jiasu_flag++;
			AimVelocity=0;
		}
		else if(jiasu_flag < 140)
		{
			jiasu_flag++;
			AimVelocity+=60;
		}
	}
	else if(SpeedMode == 9)//�ڶ�Ȧ��E������ͣ��10��
	{
		if( jiasu_flag < 160)//
			{
			jiasu_flag++;
			AimVelocity-=30;
			}
		else if(jiasu_flag < 260)
		{
			jiasu_flag++;
			AimVelocity=0;
		}
		else if(jiasu_flag < 270)
		{
			jiasu_flag++;
			AimVelocity+=60;
		}
	}
	else if(SpeedMode == 10)//�ڶ�Ȧ�ص���ʼ��ͣ��
	{
		if(jiasu_flag < 290)
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

