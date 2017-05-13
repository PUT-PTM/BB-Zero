#include "IRQHandlers.h"
#include "sound.h"
int soundIteration=0;
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
					GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY

					GPIO_SetBits(GPIOB,GPIO_Pin_12);//AIN1
					GPIO_ResetBits(GPIOB,GPIO_Pin_11);//AIN2

					GPIO_SetBits(GPIOB,GPIO_Pin_4);//BIN2
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);//BIN1

					TIM4->CCR1=19;//PWMA
					TIM4->CCR2=19;//PWMB
				}
				else if( USART3->DR == 's' )
				{
					GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY
					TIM4->CCR1=0;//PWMA
					TIM4->CCR2=0;//PWMB

				}
				else if (USART3->DR == 'a')
				{
					GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY

						GPIO_SetBits(GPIOB,GPIO_Pin_12);//AIN1
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);//AIN2

						TIM4->CCR1=19;//PWMA

						TIM4->CCR2=0;//PWMB
				}
				else if(USART3->DR == 'd')
				{
					GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY

						TIM4->CCR1=0;//PWMA

						GPIO_SetBits(GPIOB,GPIO_Pin_5);//BIN1
						GPIO_ResetBits(GPIOB,GPIO_Pin_4);//BIN2

						TIM4->CCR2=19;//PWMB
				}
				else if(USART3->DR == 'e')
				{
					TIM_Cmd(TIM3,ENABLE);

				}
	}
}
