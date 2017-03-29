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
