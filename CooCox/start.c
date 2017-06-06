#include "main.h"


void init_USB()
{
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

	return;
}
void TIMER_1HZ_init(uint16_t a){

 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
 	TIM_TimeBaseInitTypeDef str;
 	str.TIM_Period = 2099;
 	str.TIM_Prescaler = a;
 	str.TIM_ClockDivision = TIM_CKD_DIV1;

 	str.TIM_CounterMode=TIM_CounterMode_Up;
 	TIM_TimeBaseInit(TIM3,&str);
 	TIM_Cmd(TIM3, ENABLE);
}
void TIMER_Interrupt_init(void)
 {
 	NVIC_InitTypeDef str;
 	str.NVIC_IRQChannel = TIM3_IRQn ;
 	str.NVIC_IRQChannelPreemptionPriority = 0x00;
 	str.NVIC_IRQChannelSubPriority = 0x00;
 	str.NVIC_IRQChannelCmd = ENABLE ;
 	NVIC_Init(&str);
 	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
 	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );
 }
void TIM3_IRQHandler(void)
 {
 	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
 				    if(UB_IRMP_Read(&myIRData)==TRUE) {
 				      if(myIRData.address==65280) {
 							if(myIRData.command==8)
 									LED_GREEN_ON;
 							if(myIRData.command==24)
 									LED_ORANGE_ON;
 							if(myIRData.command==90)
 									LED_RED_ON;
 							if(myIRData.command==82)
 									LED_BLUE_ON;
 							if(myIRData.command==28)
 									ALL_OFF;

 							if(flag = 1){
 								VCP_send_buffer(&myIRData.command,1);
 								lastCommand = myIRData.command;
 							}
 							else{
 								VCP_send_buffer(flag,1);
 								flag = 1;
 							}


 				      }
 				    }
 		if(lastCommand == myIRData.command)
 			flag = 0;
 		else{
 			flag = 1;
 			myIRData.command = 0;
 		}
 		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
 	}
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

