#include "tests.h"

int main(void)
{
	SystemInit();
	RCC_Init();
	initWheels();

    while(1)
    {
    	runWheels();
    }
}
