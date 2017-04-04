#define HSE_VALUE ((uint32_t)8000000) /* STM32 discovery uses a 8Mhz external crystal */

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

#include "main.h"
#include "irmp.h"
#include "irmpconfig.h"
#include "irmpprotocols.h"
#include "irmpsystem.h"
#include "stm32_ub_led.h"
#include "stm32_ub_irmp.h"


int main(void)
{
	/* Set up the system clocks */
	SystemInit();

	/*Led init*/
	Led_Init();

	/*IR Init*/
	UB_IRMP_Init();


	/*Struct for IR-Data*/
	IRMP_DATA  myIRData;

	while (1)
	{
		 // IR Data pollen
		    if(UB_IRMP_Read(&myIRData)==TRUE) {
		      // If IR data has been received
		      if(myIRData.address==65280) {
		        // If address from IR device is correct
					// Button "4"
					if(myIRData.command==8)
							LED_GREEN_ON;

					// Button "2"
					if(myIRData.command==24)
							LED_ORANGE_ON;

					// Button "6"
					if(myIRData.command==90)
							LED_RED_ON;

					// Button "8"
					if(myIRData.command==82)
							LED_BLUE_ON;

					//Button "5"
					if(myIRData.command==28)
							ALL_OFF;
		      }
		    }
	}
	return 0;
}
