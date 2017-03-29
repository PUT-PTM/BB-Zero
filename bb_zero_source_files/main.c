#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dac.h"
#include "misc.h"
#include "sound.h"

#include "inits.h"
#include "IRQHandlers.h"




int main(void)
{
	SystemInit();
	initAll();

	while(1)
	{
	}
}
