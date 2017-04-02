
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
#include "rc5.h"
#include "main.h"

int ktoraDioda;
int decode;

void init()
{
	/*GPIO init*/
	GPIOdiody_init();

	/*USART init*/
	init_USART();

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

uint16_t dr;
uint16_t brr;
uint16_t x;

void USART3_IRQHandler(){
	int a;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){//enter interrupt when STM32 receice data.
			dr = USART3->DR;
			brr = USART3->BRR;
			x = USART3->SR;

			for (a = 0; a < 4000000; a++);
			if(ktoraDioda == 0){
				LED_GREEN_ON;
				ktoraDioda = 1;
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)==1)
					for (a = 0; a < 100; a++);
			}
			else if(ktoraDioda == 1){
				LED_ORANGE_ON;
				ktoraDioda = 2;
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)==1)
					for (a = 0; a < 100; a++);
			}
			else if(ktoraDioda == 2){
				LED_RED_ON;
				ktoraDioda = 3;
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)==1)
					for (a = 0; a < 100; a++);
			}
			else if(ktoraDioda == 3){
				LED_BLUE_ON;
				ktoraDioda = 4;
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)==1)
					for (a = 0; a < 100; a++);
			}
			else{
				ALL_OFF;
				ktoraDioda = 0;
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)==1)
					for (a = 0; a < 100; a++);
			}
			brr = USART3->BRR;
			x = USART3->SR;
		}

}

int main(void)
{
	/* Set up the system clocks */
	SystemInit();

	/* Initialize USB, GPIO, Timer, IO, SysTick, and all those other things you do in the morning */
	init();
	ktoraDioda = 0;

	while (1)
	{

	}
	return 0;
}
