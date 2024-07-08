#include "Grayscale.h"

uint8_t xun[8];
//int8_t arr_bit[8]={70,55,30,10,-10,-30,-55,-70};
int8_t arr_bit[8]={50,40,20,10,-10,-20,-40,-50};
int a[30];


int8_t Read_8PIN(void)
{
	xun[0] = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
	xun[1] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);	
	xun[2] = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11);
	xun[3] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4);		
	xun[4] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8);
	xun[5] = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14);		
	xun[6] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	xun[7] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);		
	return ERR();
}

int8_t ERR(void)
{
	int8_t Err = arr_bit[0]*xun[0]+arr_bit[1]*xun[1]+arr_bit[2]*xun[2]+arr_bit[3]*xun[3]+arr_bit[4]*xun[4]+arr_bit[5]*xun[5]+arr_bit[6]*xun[6]+arr_bit[7]*xun[7];
	return Err;
}
int Paixu(uint8_t* times,int Everytimes_Gray)//这个函数记录正好前30次灰度的输出值并返回其平均值
{
    a[*times]=Everytimes_Gray;
    
    (*times)+=1;
    if((*times)>=30)(*times)=0;
    
    return (a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9]+a[10]+a[11]+a[12]+a[13]+a[14]+a[15]+a[16]+a[17]+a[18]+a[19]
        +a[20]+a[21]+a[22]+a[23]+a[24]+a[25]+a[26]+a[27]+a[28]+a[29])/30;
}
