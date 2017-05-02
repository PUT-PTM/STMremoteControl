#include "main.h"

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

	/*Init VCP*/
	init();

	/*Init Timer */
	TIMER_1HZ_init(9999);
	TIMER_Interrupt_init();

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
						
					VCP_send_buffer(&myIRData.command,1);
		      }
		    }
	}
	return 0;
}
void init()
{
	/* Setup USB */
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

	return;
}
void TIMER_1HZ_init(uint16_t a){

 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
 	TIM_TimeBaseInitTypeDef str;
 	str.TIM_Period=8399;
 	str.TIM_Prescaler=a;
 	str.TIM_ClockDivision=TIM_CKD_DIV1;

 	str.TIM_CounterMode=TIM_CounterMode_Up;
 	TIM_TimeBaseInit(TIM2,&str);
 	TIM_Cmd(TIM2, ENABLE);
}
void TIMER_Interrupt_init(void)
 {
 	NVIC_InitTypeDef str;
 	str.NVIC_IRQChannel = TIM2_IRQn ;
 	str.NVIC_IRQChannelPreemptionPriority = 0x00;
 	str.NVIC_IRQChannelSubPriority = 0x00;
 	str.NVIC_IRQChannelCmd = ENABLE ;
 	NVIC_Init(&str);
 	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
 	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );
 }

void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}
void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}

