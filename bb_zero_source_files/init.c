#include "inits.h"

void RCC_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // glownie do USARTa
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

void initAll()
{
    RCC_Init();
	TIM3_Init(60,15);
	DAC_PA4_Init();
}

void TIM3_Init(int period,int prescaler)
{
	TIM_TimeBaseInitTypeDef TIM3_Inicjalizacja;
	TIM3_Inicjalizacja.TIM_Period = period;
	TIM3_Inicjalizacja.TIM_Prescaler = prescaler;
	TIM3_Inicjalizacja.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_Inicjalizacja.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM3, &TIM3_Inicjalizacja);
	TIM_Cmd(TIM3, ENABLE);

	//-------------

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

void DAC_PA4_Init()
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