#include "tests.h"

int main(void)
{
	SystemInit();
	initWheels();

    while(1)
    {
    	runWheels();
    }
}
