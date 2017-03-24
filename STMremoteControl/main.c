
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

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"
#include "main.h"

int ktoraDioda;
void init()
{
	/*GPIO init*/
	GPIOdiody_init();

	/*USART init*/
	init_USART();

	/* USB init*/
	USBD_Init(&USB_OTG_dev,	USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);

	return;
}

void GPIOdiody_init(){
	/* STM32F4 discovery LEDs */
	GPIO_InitTypeDef LED_Config;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	LED_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	LED_Config.GPIO_Mode = GPIO_Mode_OUT;
	LED_Config.GPIO_OType = GPIO_OType_PP;
	LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
	LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &LED_Config);
}
void init_USART(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	//TX line
	GPIO_InitTypeDef pb1011;
	pb1011.GPIO_OType = GPIO_OType_PP;
	pb1011.GPIO_PuPd = GPIO_PuPd_UP;
	pb1011.GPIO_Pin = GPIO_Pin_10;
	pb1011.GPIO_Mode = GPIO_Mode_AF;
	pb1011.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &pb1011);

	//RX line
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	pb1011.GPIO_Mode = GPIO_Mode_AF;
	pb1011.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &pb1011);

	//USART
	USART_InitTypeDef usa;
	usa.USART_BaudRate = 115200;
	usa.USART_WordLength = USART_WordLength_8b;
	usa.USART_StopBits = USART_StopBits_1;
	usa.USART_Parity = USART_Parity_No;
	usa.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usa.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &usa);
	USART_Cmd(USART3, ENABLE);

	//USART interrupt
	NVIC_InitTypeDef usart;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	usart.NVIC_IRQChannel = USART3_IRQn;
	usart.NVIC_IRQChannelPreemptionPriority = 0;
	usart.NVIC_IRQChannelSubPriority = 0;
	usart.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&usart);
	NVIC_EnableIRQ(USART3_IRQn);
}

uint16_t decode;
void USART3_IRQHandler(){
	int a;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		for (a = 0; a < 400000; a++);
		switch (ktoraDioda){
			case 0:{
				LED_GREEN_ON;
				ktoraDioda = 1;
				for (a = 0; a < 400000; a++);
				break;
			}
			case 1:{
				LED_ORANGE_ON;
				ktoraDioda = 2;
				for (a = 0; a < 400000; a++);
				break;
			}
			case 2:{
				LED_RED_ON;
				ktoraDioda = 3;
				for (a = 0; a < 400000; a++);
				break;
			}
			case 3:{
				LED_BLUE_ON;
				ktoraDioda = 4;
				for (a = 0; a < 400000; a++);
				break;
			}
			default:{
				ALL_OFF;
				ktoraDioda = 0;
				for (a = 0; a < 400000; a++);
				break;
			}
		}

	}
	decode = USART_ReceiveData(USART3);
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


int main(void)
{
	/* Set up the system clocks */
	SystemInit();

	/* Initialize USB, GPIO, Timer, IO, SysTick, and all those other things you do in the morning */
	init();
	ktoraDioda = 5;

	while (1)
	{

	}
	return 0;
}
