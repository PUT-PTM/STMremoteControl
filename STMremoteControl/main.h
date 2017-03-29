volatile uint32_t ticker, downTicker;

#define LED_BLUE_ON GPIOD->BSRRL = GPIO_Pin_15;
#define LED_BLUE_OFF GPIOD->BSRRH = GPIO_Pin_15;

#define LED_RED_ON GPIOD->BSRRL = GPIO_Pin_14;
#define LED_RED_OFF GPIOD->BSRRH = GPIO_Pin_14;

#define LED_ORANGE_ON GPIOD->BSRRL = GPIO_Pin_13;
#define LED_ORANGE_OFF GPIOD->BSRRH = GPIO_Pin_13;

#define LED_GREEN_ON GPIOD->BSRRL = GPIO_Pin_12;
#define LED_GREEN_OFF GPIOD->BSRRH = GPIO_Pin_12;

#define ALL_ON GPIOD->BSRRL = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
#define ALL_OFF GPIOD->BSRRH = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

/*
 * The USB data must be 4 byte aligned if DMA is enabled. This macro handles
 * the alignment, if necessary (it's actually magic, but don't tell anyone).
 */

