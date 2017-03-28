#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

void RCC_Inicjalizacja()
	{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//sterowanie kierunkiem obrotu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //PWM
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
void TIM4_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
   {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
   }
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
		Sterowanie_Inicjalizacja();
		TIM4_Inicjalizacja(39,34);
		//TIM4_Przerwanie();
		PWM_TIM4_Inicjalizacja();

    while(1)
    {
    	GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY

    	GPIO_SetBits(GPIOB,GPIO_Pin_12);//AIN1
    	GPIO_ResetBits(GPIOB,GPIO_Pin_11);//AIN2
    	TIM4->CCR1=19;//PWMA

    	GPIO_SetBits(GPIOB,GPIO_Pin_5);//BIN1
    	GPIO_ResetBits(GPIOB,GPIO_Pin_4);//BIN2
    	TIM4->CCR2=19;//PWMB

    }
}
