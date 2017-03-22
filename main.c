#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "sound.h"

double ADC_Result;
int i=0;
void RCC_Inicjalizacja()
	{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //glownie do DAC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // glownie do USARTa
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //PWM
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//do robienia w okresie
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //PWM
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//byc moze nie potrzebny
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//do wydawania dzwiekow
	}
void UART_Inicjalizacja()
	{
	// konfiguracja linii Rx(PC11) i Tx(PC10)
	GPIO_InitTypeDef Piny_do_UART;
	Piny_do_UART.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	Piny_do_UART.GPIO_Mode = GPIO_Mode_AF;
	Piny_do_UART.GPIO_OType = GPIO_OType_PP;
	Piny_do_UART.GPIO_PuPd = GPIO_PuPd_UP;
	Piny_do_UART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Piny_do_UART);
	// ustawienie funkcji alternatywnej dla pinów
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	//parametry UART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
	}
void UART_Przerwanie()
	{
	NVIC_InitTypeDef UART_Przerwanie;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	UART_Przerwanie.NVIC_IRQChannel = USART3_IRQn;
	UART_Przerwanie.NVIC_IRQChannelPreemptionPriority = 0;
	UART_Przerwanie.NVIC_IRQChannelSubPriority = 0;
	UART_Przerwanie.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&UART_Przerwanie);
	NVIC_EnableIRQ(USART3_IRQn);
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

			TIM4->CCR1=19;//PWMA

			GPIO_SetBits(GPIOB,GPIO_Pin_5);//BIN1
			GPIO_ResetBits(GPIOB,GPIO_Pin_4);//BIN2

			TIM4->CCR2=21;//PWMB
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


		}

	}
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
void TIM4_Inicjalizacja(int period,int prescaler)
{
	TIM_TimeBaseInitTypeDef TIM4_Inicjalizacja;
	TIM4_Inicjalizacja.TIM_Period = period;
	TIM4_Inicjalizacja.TIM_Prescaler = prescaler;
	TIM4_Inicjalizacja.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM4_Inicjalizacja.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM4, &TIM4_Inicjalizacja);
	TIM_Cmd(TIM4, ENABLE);
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
void TIM4_Przerwanie()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef TIM4_Przerwanie;
	TIM4_Przerwanie.NVIC_IRQChannel = TIM4_IRQn;
	TIM4_Przerwanie.NVIC_IRQChannelPreemptionPriority = 0x00;
	TIM4_Przerwanie.NVIC_IRQChannelSubPriority = 0x00;
	TIM4_Przerwanie.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&TIM4_Przerwanie);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}
void TIM3_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
   {
		   DAC_SetChannel1Data(DAC_Align_12b_R, rawData[i]*ADC_Result/4095);
		   if(i==85490)i=0;i++;

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
   }
}

void TIM4_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
   {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
   }
}
void ADC_PA1_Inicjalizacja()
	{
		GPIO_InitTypeDef ADC_PA1_Inicjalizacja;
		ADC_PA1_Inicjalizacja.GPIO_Pin = GPIO_Pin_1;
		ADC_PA1_Inicjalizacja.GPIO_Mode = GPIO_Mode_AN;
		ADC_PA1_Inicjalizacja.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &ADC_PA1_Inicjalizacja);

			ADC_CommonInitTypeDef ADC_PA1_Inicjalizacja_Struktury;
			ADC_PA1_Inicjalizacja_Struktury.ADC_Mode = ADC_Mode_Independent;
			ADC_PA1_Inicjalizacja_Struktury.ADC_Prescaler = ADC_Prescaler_Div2;
			ADC_PA1_Inicjalizacja_Struktury.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
			ADC_PA1_Inicjalizacja_Struktury.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
			ADC_CommonInit(&ADC_PA1_Inicjalizacja_Struktury);

			ADC_InitTypeDef ADC_PA1_Inicjalizacja_Struktury_Parametry;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_Resolution = ADC_Resolution_12b;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_ScanConvMode = DISABLE;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_ContinuousConvMode = ENABLE;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_DataAlign = ADC_DataAlign_Right;
			ADC_PA1_Inicjalizacja_Struktury_Parametry.ADC_NbrOfConversion = 1;
			ADC_Init(ADC1, &ADC_PA1_Inicjalizacja_Struktury_Parametry);

			ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);
			ADC_Cmd(ADC1, ENABLE);
	}
void ADC_PA5_Inicjalizacja()
	{
		GPIO_InitTypeDef ADC_PA5_Inicjalizacja;
		ADC_PA5_Inicjalizacja.GPIO_Pin = GPIO_Pin_5;
		ADC_PA5_Inicjalizacja.GPIO_Mode = GPIO_Mode_AN;
		ADC_PA5_Inicjalizacja.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &ADC_PA5_Inicjalizacja);

			ADC_CommonInitTypeDef ADC_PA5_Inicjalizacja_Struktury;
			ADC_PA5_Inicjalizacja_Struktury.ADC_Mode = ADC_Mode_Independent;
			ADC_PA5_Inicjalizacja_Struktury.ADC_Prescaler = ADC_Prescaler_Div2;
			ADC_PA5_Inicjalizacja_Struktury.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
			ADC_PA5_Inicjalizacja_Struktury.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
			ADC_CommonInit(&ADC_PA5_Inicjalizacja_Struktury);

			ADC_InitTypeDef ADC_PA5_Inicjalizacja_Struktury_Parametry;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_Resolution = ADC_Resolution_12b;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_ScanConvMode = DISABLE;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_ContinuousConvMode = ENABLE;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_DataAlign = ADC_DataAlign_Right;
			ADC_PA5_Inicjalizacja_Struktury_Parametry.ADC_NbrOfConversion = 1;
			ADC_Init(ADC2, &ADC_PA5_Inicjalizacja_Struktury_Parametry);
			ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_84Cycles);
			ADC_Cmd(ADC2, ENABLE);
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
void PWM_TIM4_Inicjalizacja()
{
		GPIO_InitTypeDef PWM_TIM4_Inicjalizacja;
		PWM_TIM4_Inicjalizacja.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
		PWM_TIM4_Inicjalizacja.GPIO_OType = GPIO_OType_PP;
		PWM_TIM4_Inicjalizacja.GPIO_Speed = GPIO_Speed_100MHz;
		PWM_TIM4_Inicjalizacja.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

		PWM_TIM4_Inicjalizacja.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOD, &PWM_TIM4_Inicjalizacja);

		TIM_OCInitTypeDef TIM_OCInitStructure;
		/* PWM1 Mode configuration: */
		TIM_OCInitStructure. TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure. TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure. TIM_Pulse = 0;
		TIM_OCInitStructure. TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
void Sterowanie_Inicjalizacja()
	{
		GPIO_InitTypeDef Sterowanie_Inicjalizacja;
		Sterowanie_Inicjalizacja.GPIO_Pin =GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_4|GPIO_Pin_5 ;
		Sterowanie_Inicjalizacja.GPIO_Mode = GPIO_Mode_OUT;
		Sterowanie_Inicjalizacja.GPIO_OType = GPIO_OType_PP;
		Sterowanie_Inicjalizacja.GPIO_Speed = GPIO_Speed_100MHz;
		Sterowanie_Inicjalizacja.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &Sterowanie_Inicjalizacja);
		//PB12 - AIN1
		//PB11 - AIN2
		//PB5 - BIN1
		//PB4 - BIN2
		//PB13 - STBY
	}
int main(void)
{
	SystemInit();
	RCC_Inicjalizacja();
	UART_Inicjalizacja();
	UART_Przerwanie();
	Sterowanie_Inicjalizacja();
	TIM3_Inicjalizacja(84,21);
	TIM3_Przerwanie();
	TIM4_Inicjalizacja(39,34);
	//TIM4_Przerwanie();
	ADC_PA1_Inicjalizacja();
	//ADC_PA5_Inicjalizacja();
	DAC_PA4_Inicjalizacja();
	PWM_TIM4_Inicjalizacja();

	while(1)
    {

    }
}
