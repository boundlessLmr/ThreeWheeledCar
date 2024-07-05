#include "TiMu.h"

extern int jiasu_flag;
extern int AimVelocity;
extern int8_t arr_bit[8];
extern int Gray_KP;
extern int Gray_KD;


void Timu_1(uint8_t SpeedMode)//第一个题目
{
	if(SpeedMode == 0)//从0开始加速
	{
		if(jiasu_flag<0)jiasu_flag++;//等待几秒
			 else if( jiasu_flag < 20)//1秒内加速到600
			{
			jiasu_flag++;
			AimVelocity+=30;
			}
	}
	if(SpeedMode == 5)//读到第五根线也就是起始线就停下
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

void Timu_2(uint8_t SpeedMode)//第二小题
{
	if(SpeedMode == 0)//从0开始加速
		{
	//			if(jiasu_flag < 50)//
	//			{
	//				jiasu_flag++;
	//			}
	//		
			 if(jiasu_flag<-10)jiasu_flag++;//等待几秒启动
			 else if( jiasu_flag < 10)//1秒内加速到600
			{
			jiasu_flag++;
			AimVelocity+=30;			
			}
		}
		
//		else if(SpeedMode == 1)//模式1
//		{
////			arr_bit[0]=50,arr_bit[1]=40,arr_bit[2]=20,arr_bit[3]=10,arr_bit[4]=-10,arr_bit[5]=-20,arr_bit[6]=-40,arr_bit[7]=-50;
////			
////			if(jiasu_flag < 20)//1秒内减速到600
////			{
////				jiasu_flag++;
////				AimVelocity -= 10;
////			}
//		}
		else if(SpeedMode == 2)//第二根线弯道减速一点
		{
//			Gray_KP = 30,Gray_KD = -10;
			if(jiasu_flag < 30)//1秒内减速到400
			{
				jiasu_flag++;
				AimVelocity -= 10;
			}
		}
		else if(SpeedMode == 3)//第三根线也就是D区域，减速20秒然后起步
		{
			Gray_KP = 15,Gray_KD = 5;;
			if(jiasu_flag < 40)//1秒内减速到10
			{
				jiasu_flag++;
				AimVelocity -= 38;
				
			}
			else if(jiasu_flag < 200)//停留16秒
			{
				jiasu_flag++;
//				AimVelocity = 0;
			}
			else if(jiasu_flag < 210)//1秒钟加速到150
			{
				jiasu_flag++;
				AimVelocity += 13;
			}
		}
		else if(SpeedMode == 4)//加速到300
		{
			Gray_KP = 25,Gray_KD = 8;
//			arr_bit[0]=70,arr_bit[1]=55,arr_bit[2]=30,arr_bit[3]=10,arr_bit[4]=-10,arr_bit[5]=-30,arr_bit[6]=-55,arr_bit[7]=-70;
			if(jiasu_flag < 220)
			{
				jiasu_flag++;
				AimVelocity += 15;
			}
		}
		else if(SpeedMode == 5)//回到起始点停下
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
	if(SpeedMode == 0)//初始模式
		{
	//			if(jiasu_flag < 50)//5秒后开始启动
	//			{
	//				jiasu_flag++;
	//			}
	//		
			if(jiasu_flag<-10)jiasu_flag++;
			 else if( jiasu_flag < 10)//2秒内加速到600
			{
			jiasu_flag++;
			AimVelocity+=30;			
			}
		}
	else if (SpeedMode == 2)//稍微减速，300
	{
		if(jiasu_flag < 40)
		{
			jiasu_flag++;
			AimVelocity-=10;
		}
	}
	else if (SpeedMode == 3)//第一圈的D区域内停下10秒
	{
		 if( jiasu_flag < 70)//3秒内减速到0
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
			AimVelocity+=30;//初始速度600
		}
	}
	else if(SpeedMode == 8)//稍微减速，300
	{
		if(jiasu_flag < 220)
		{
			AimVelocity-=10;
		}
	}
	else if(SpeedMode == 9)//第二圈的E区域内停下10秒
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
		else if(jiasu_flag < 370)//再次启动400
		{
			jiasu_flag++;
			AimVelocity+=30;
		}
	}
	else if(SpeedMode == 10)//第二圈回到起始点停车
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

