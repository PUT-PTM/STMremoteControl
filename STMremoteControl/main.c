
#define HSE_VALUE ((uint32_t)8000000) /* STM32 discovery uses a 8Mhz external crystal */

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"
#include "main.h"

int main(void)
{
	/* Set up the system clocks */
	SystemInit();

	/* Initialize USB, GPIO, Timer, IO, SysTick, and all those other things you do in the morning */
	init();


	while (1)
	{
		/* Blink the orange LED at 1Hz */
		if (500 == ticker)
		{
			GPIOD->BSRRH = GPIO_Pin_13;
		}
		else if (1000 == ticker)
		{
			ticker = 0;
			GPIOD->BSRRL = GPIO_Pin_13;
		}


		/* If there's data on the virtual serial port:
		 *  - Echo it back
		 *  - Turn the green LED on for 10ms
		 */
		uint8_t theByte;
		if (VCP_get_char(&theByte))
		{
			VCP_put_char(theByte);


			GPIOD->BSRRL = GPIO_Pin_12;
			downTicker = 10;
		}
		if (0 == downTicker)
		{
			GPIOD->BSRRH = GPIO_Pin_12;
		}
	}
	return 0;
}


void init()
{
	/*GPIO init*/
	GPIO_init();

	/*Interrupt init*/
	NVIC_init();

	/*Timer init*/
	TIMER_1HZ_init(2999);
	TIMER_Interrupt_init();

	/* Setup SysTick or CROD! */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		ColorfulRingOfDeath();
	}


	/* Setup USB */
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

	return;
}

void NVIC_init(){
	//dla kana³u 1
	NVIC_InitTypeDef interrupt;
	interrupt.NVIC_IRQChannel = TIM2_IRQn; 				//w intrukcji z przerwan jest TIM3_IRQn
	interrupt.NVIC_IRQChannelPreemptionPriority = 0x00;
	interrupt.NVIC_IRQChannelSubPriority = 0x00;
	interrupt.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&interrupt);
}
void EXTI_init(){
	EXTI_InitTypeDef exti;
	exti.EXTI_Line = EXTI_Line1;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	//podlaczenie pinu 1 z portu A (PA1) do modulu przerwan zewnetrznych
	SYSCFG_EXTILineConfig(GPIOA, EXTI_PinSource1);

}
void GPIO_init(){
	/* STM32F4 discovery LEDs */
	GPIO_InitTypeDef LED_Config;

	/* Always remember to turn on the peripheral clock...  If not, you may be up till 3am debugging... */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	LED_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	LED_Config.GPIO_Mode = GPIO_Mode_OUT;
	LED_Config.GPIO_OType = GPIO_OType_PP;
	LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
	LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &LED_Config);
}
void TIMER_1HZ_init(uint16_t a){

 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
 	TIM_TimeBaseInitTypeDef str;
 	str.TIM_Period=27999;
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
 /*function to sample RC5 data*/
 void TIM2_IRQHandler(void)
 {
 	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
 	{
	 	GPIO_ToggleBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15 | GPIO_Pin_12|GPIO_Pin_13);
 		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
 	}
 }


/*
 * Call this to indicate a failure.  Blinks the STM32F4 discovery LEDs
 * in sequence.  At 168Mhz, the blinking will be very fast - about 5 Hz.
 * Keep that in mind when debugging, knowing the clock speed might help
 * with debugging.
 */
void ColorfulRingOfDeath(void)
{
	uint16_t ring = 1;
	while (1)
	{
		uint32_t count = 0;
		while (count++ < 500000);

		GPIOD->BSRRH = (ring << 12);
		ring = ring << 1;
		if (ring >= 1<<4)
		{
			ring = 1;
		}
		GPIOD->BSRRL = (ring << 12);
	}
}

/*
 * Interrupt Handlers
 */

void SysTick_Handler(void)
{
	ticker++;
	if (downTicker > 0)
	{
		downTicker--;
	}
}

void NMI_Handler(void)       {}
void HardFault_Handler(void) { ColorfulRingOfDeath(); }
void MemManage_Handler(void) { ColorfulRingOfDeath(); }
void BusFault_Handler(void)  { ColorfulRingOfDeath(); }
void UsageFault_Handler(void){ ColorfulRingOfDeath(); }
void SVC_Handler(void)       {}
void DebugMon_Handler(void)  {}
void PendSV_Handler(void)    {}

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
