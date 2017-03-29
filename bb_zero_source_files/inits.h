#ifndef _Inits
#define _Inits

#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

void initBluetooth();
void RCC_Init();
void initAll();
void diodyInit();
#endif