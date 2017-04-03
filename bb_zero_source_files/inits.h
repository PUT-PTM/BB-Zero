#ifndef _Inits
#define _Inits
#include "stm32f4xx_dac.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

#include "tests.h"
#include "IRQhandlers.h"


void initBluetooth();
void RCC_Init(); 
void initAll();
void diodyInit();
void TIM3_Init(int period,int prescaler); //Inicjalizacja timera 3 i przerwań
void DAC_PA4_Init(); //Inicjalizacja DAC na pinie A4
void initWheels();
void PWM_TIM4_Init();

#endif
