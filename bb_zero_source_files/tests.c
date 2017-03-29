#include "tests.h"

void diodyInit()
{
	GPIO_InitTypeDef Diody_testowe;
	Diody_testowe.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13;
	Diody_testowe.GPIO_Mode = GPIO_Mode_OUT;
	Diody_testowe.GPIO_OType = GPIO_OType_PP;
	Diody_testowe.GPIO_Speed = GPIO_Speed_100MHz;
	Diody_testowe.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &Diody_testowe);
}