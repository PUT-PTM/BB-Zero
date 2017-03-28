#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dac.h"
#include "misc.h"
#include "sound.h"
int i=0;
void RCC_Inicjalizacja()
	{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //glownie do DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//do robienia w okresie
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//do wydawania dzwiekow
	}
void TIM3_Inicjalizacja(int period,int prescaler)
{
	TIM_TimeBaseInitTypeDef TIM3_Inicjalizacja;
	TIM3_Inicjalizacja.TIM_Period = period;
	TIM3_Inicjalizacja.TIM_Prescaler = prescaler;
	TIM3_Inicjalizacja.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_Inicjalizacja.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM3, &TIM3_Inicjalizacja);
	TIM_Cmd(TIM3, ENABLE);
}
void TIM3_Przerwanie()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef TIM3_Przerwanie;
	TIM3_Przerwanie.NVIC_IRQChannel = TIM3_IRQn;
	TIM3_Przerwanie.NVIC_IRQChannelPreemptionPriority = 0x00;
	TIM3_Przerwanie.NVIC_IRQChannelSubPriority = 0x00;
	TIM3_Przerwanie.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&TIM3_Przerwanie);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}
void TIM3_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
   {
		   DAC_SetChannel1Data(DAC_Align_12b_R, rawData[i]*4);
		   if(i==85490)
			   {
			   i=0;
			   TIM_Cmd(TIM3,DISABLE);
			   }
		   i++;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
   }
}
void DAC_PA4_Inicjalizacja()
{
	GPIO_InitTypeDef DAC_PA4_Inicjalizacja;
	DAC_PA4_Inicjalizacja.GPIO_Pin = GPIO_Pin_4;
	DAC_PA4_Inicjalizacja.GPIO_Mode = GPIO_Mode_AN;
	DAC_PA4_Inicjalizacja.GPIO_Speed = GPIO_Speed_100MHz;
	DAC_PA4_Inicjalizacja.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &DAC_PA4_Inicjalizacja);

		DAC_InitTypeDef DAC_PA4_Inicjalizacja_Parametry;
		DAC_PA4_Inicjalizacja_Parametry.DAC_Trigger = DAC_Trigger_None;
		DAC_PA4_Inicjalizacja_Parametry.DAC_WaveGeneration = DAC_WaveGeneration_None;
		DAC_PA4_Inicjalizacja_Parametry.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
		DAC_PA4_Inicjalizacja_Parametry.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
		DAC_Init(DAC_Channel_1, &DAC_PA4_Inicjalizacja_Parametry);
		DAC_Cmd(DAC_Channel_1, ENABLE);
}
int main(void)
{
		SystemInit();
		RCC_Inicjalizacja();
		TIM3_Inicjalizacja(60,15);
		TIM3_Przerwanie();
		DAC_PA4_Inicjalizacja();

    while(1)
    {
    }
}
