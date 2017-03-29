#include "IRQhandlers.h"

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if( USART3->DR == 'w' )
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
			GPIO_SetBits(GPIOD,GPIO_Pin_13);
		}
		else if( USART3->DR == 's' )
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
			GPIO_ResetBits(GPIOD,GPIO_Pin_13);

		}
		else if (USART3->DR == 'a')
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
			GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		}
		else if(USART3->DR == 'd')
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_13);
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		}
	}
}