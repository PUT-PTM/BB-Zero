#include "inits.h"

void RCC_Init()
{	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//sterowanie kierunkiem obrotu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //PWM
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // glownie do USARTa
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

void initAll()
{
    RCC_Init();
	initWheels();
	initBluetooth();
}

void initWheels()
{
	PWM_TIM4_Init();

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

void PWM_TIM4_Init()
{
	TIM_TimeBaseInitTypeDef TIM4_Inicjalizacja;
	TIM4_Inicjalizacja.TIM_Period = 99;
	TIM4_Inicjalizacja.TIM_Prescaler = 20;
	TIM4_Inicjalizacja.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM4_Inicjalizacja.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM4, &TIM4_Inicjalizacja);
	TIM_Cmd(TIM4, ENABLE);

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

void initBluetooth()
{
	// konfiguracja linii Rx(PC11) i Tx(PC10)
	GPIO_InitTypeDef Piny_do_UART;
	Piny_do_UART.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	Piny_do_UART.GPIO_Mode = GPIO_Mode_AF;
	Piny_do_UART.GPIO_OType = GPIO_OType_PP;
	Piny_do_UART.GPIO_PuPd = GPIO_PuPd_UP;
	Piny_do_UART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Piny_do_UART);
	// ustawienie funkcji alternatywnej dla pin�w
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

	///-----------------

	NVIC_InitTypeDef UART_Przerwanie;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	UART_Przerwanie.NVIC_IRQChannel = USART3_IRQn;
	UART_Przerwanie.NVIC_IRQChannelPreemptionPriority = 0;
	UART_Przerwanie.NVIC_IRQChannelSubPriority = 0;
	UART_Przerwanie.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&UART_Przerwanie);
	NVIC_EnableIRQ(USART3_IRQn);
}
