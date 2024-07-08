#include "key.h"

struct Bkeys bkey[5]={0,0,0,0};

struct key_data key_data1[4]={0,0,0};



#define KEY_num 5

uint8_t key_read()
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)== 1	)       return 1;
	else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==1)  return 2;
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)==1) return 3;	
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)==1) return 4;
	
	else return 0;	
}
void key_serv()
{
	uint8_t key_sta=key_read();
	if(key_sta!=0)
	{
		bkey[key_sta].age++;
		if(bkey[key_sta].age==2)bkey[key_sta].short_flag=1;

	}
	else
	{
		for(int i=0;i<KEY_num;i++) bkey[i].age=0;
	}
}

void key_serv_long()
{
	uint8_t key_sta=key_read();
	if(key_sta!=0)
	{
		bkey[key_sta].age++;
		if(bkey[key_sta].age==2) bkey[key_sta].press=1;
	}
	else
	{
		for(int i=0;i<KEY_num;i++) 
		{
			if(bkey[i].age>69) bkey[i].long_flag=1;
			if(bkey[i].press==1&&bkey[i].long_flag==0)
				bkey[i].short_flag=1;
			bkey[i].age=0;
			bkey[i].press=0;
		}
	}
}

void ReadKey(void)
{
				key_data1[0].key_flag = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);
				key_data1[1].key_flag = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2);
				key_data1[2].key_flag = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10);
				key_data1[3].key_flag = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12);
			
			for(uint8_t i=0;i<4;i++)
			{
				switch(key_data1[i].key_std)
				{
					case 0:
					{
						if(key_data1[i].key_flag==1)
						{
							key_data1[i].key_std	=1;
							
						}
						else
							key_data1[i].key_std=0;
					
					
					
					}
					break;
					case 1:
					{
						if(key_data1[i].key_flag==1)
						{
							key_data1[i].key_std	=2;
							
						}
						else
							key_data1[i].key_std=0;
					
					
					
					}
					break;
					case 2:
					{
						if(key_data1[i].key_flag==0)
						{
							key_data1[i].key_std	=0;
							key_data1[i].key_return=1;
						}
					
					
					}
					break;
				
				
				
				}
		
		
		
		
		
			}
	
}

//void key_proc(void)
//{

///*短按*/	
//	if(bkey[2].short_flag)
//	{
//		test111++;
//		
////		switch(Tiao_Who)
////		{
////			case TIAO_VERp:Vertical_Kp-= test_num_VERp;break;	
////			case TIAO_VERd:Vertical_Kd-= test_num_VERd;break;
////			case TIAO_VELp:kp					-= test_num_VELp;ki = kp/200;break;
////		}
////		
//		bkey[2].short_flag = 0;
//	}
//	if(bkey[1].short_flag)
//	{	
//		test111++;
//		
////		switch(Tiao_Who)
////		{
////			case TIAO_VERp:Vertical_Kp+= test_num_VERp;break;	
////			case TIAO_VERd:Vertical_Kd+= test_num_VERd;break;
////			case TIAO_VELp:kp					+= test_num_VELp;ki = kp/200;break;
////		}
////		
//		bkey[1].short_flag = 0;
//	}		
///*长按*/	
//	if(bkey[2].long_flag)
//	{
//		test111++;
////		Set_Pwm(0,0);
////		Start_Flag = !Start_Flag;
////		if(Start_Flag)
////		{
////			OLED_ShowString(103,00,"on",12);     
////		}
////		else
////		{
////			OLED_ShowString(103,00,"of",12);    		
////		}
////		Tiao_Who++;if(Tiao_Who== 3)Tiao_Who = 0;	
////		OLED_Clear();		
//		
//		bkey[2].long_flag = 0;
//	}
//	if(bkey[1].long_flag)
//	{
//		test111++;
//		
////		switch(Tiao_Who)
////		{
////			case TIAO_VERp:if(test_num_VERp == 100)test_num_VERp = 10;else test_num_VERp = 100;break;
////			case TIAO_VERd:if(test_num_VERd == 1) test_num_VERd = 0.1;else test_num_VERd = 1;break;
////			case TIAO_VELp:if(test_num_VELp == 0.01)test_num_VELp = 0.1;else test_num_VELp = 0.01;break;	
////		}		
////		
//		bkey[1].long_flag = 0;		
//	}	
//	
//}