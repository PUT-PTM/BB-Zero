#include "inits.h"

void RCC_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // glownie do USARTa
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
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

void initAll()
{
	RCC_Init();
	initBluetooth();
	diodyInit();
}

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