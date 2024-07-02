#include "BuzzerSet.h"

//void Buzzerset(uint8_t flag)
//{
//	if(flag==1)
//	{
//		Buzzer_On();
//		HAL_Delay(300);
//		Buzzer_Off();
//		flag=0;
//		
//	}
//}

void Buzzer_On()
{
	HAL_GPIO_WritePin(buzzer_GPIO_Port,buzzer_Pin,GPIO_PIN_RESET);
	
}
void Buzzer_Off()
{
	HAL_GPIO_WritePin(buzzer_GPIO_Port,buzzer_Pin,GPIO_PIN_SET);
	
}
