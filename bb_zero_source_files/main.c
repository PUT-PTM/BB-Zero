int main(void)
{
	SystemInit();
	initWheels();

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
