#ifndef MY_MAIN_H
#define MY_MAIN_H

#include "math.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "key.h"
#include "stdio.h"
#include "OLED.h"
#include "stdlib.h"
#include "Grayscale.h"
#include "string.h"
#include "BuzzerSet.h"
#include "ReadBlackFlag.h"
#include "motor_speed_process.h"
#include "TiMu.h"


#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


void set_up(void);
void loop(void);
int Gray_PID(int bias);
extern uint8_t xun[];
extern uint8_t ABCDE_flag;

#endif
