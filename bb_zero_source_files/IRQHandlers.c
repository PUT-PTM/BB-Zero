#include "IRQHandlers.h"
soundIteration=0;
void TIM3_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
   {
		   DAC_SetChannel1Data(DAC_Align_12b_R, rawData[soundIteration]*8);
		   if(soundIteration==85490)
			   {
			   soundIteration=0;
			TIM_Cmd(TIM3,DISABLE);
			   }
		   soundIteration++;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
   }
}


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
