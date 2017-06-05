#include "IRQHandlers.h"
#include "sound.h"
#include "math.h"
int soundIteration=0;
float kp=16.5;
float ki=0.7;
float kd=40;
float target_position = -1/12;
int k =0;
int w,g,o;
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


void interpreter()
{

	if(p[1] >= 48 && p[1]<=57)
		w = p[1]-48;
	else return;

	if(p[2] >= 48 && p[2]<=57)
			g = p[2]-48;
	else return;

	if(p[4] >= 48 && p[4]<=57)
			o = p[4]-48;
		else return;
	if(p[3] == 'm')
		o=-o;

	if(p[0] == 'w')
	{
		target_position = (w*10+g) * pow(10,o);
	}
	if(p[0] == 's')
		{
		target_position = -((w*10+g) * pow(10,o));
		}
	else if(p[0] == 'p')
	{
		kp = (w*10+g) * pow(10,o);
	}
	else if(p[0] == 'i')
		{
			ki = (w*10+g) * pow(10,o);
		}
	else if(p[0] == 'd')
		{
			kd = (w*10+g) * pow(10,o);
		}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{

		p[k] = USART3->DR;
		k++;
		if(k==5)
		{
		interpreter();
		k=0;
		}
	}
}
