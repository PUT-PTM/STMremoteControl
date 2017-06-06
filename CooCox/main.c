#include "main.h"


int main(void)
{
	SystemInit();
	Led_Init();
	UB_IRMP_Init();
	init_USB();
	TIMER_1HZ_init(9999);
	TIMER_Interrupt_init();
	flag = 0;
	while (1)
	{}
	return 0;
}


