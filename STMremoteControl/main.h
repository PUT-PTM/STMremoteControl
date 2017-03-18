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
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


void init();
void ColorfulRingOfDeath(void);

/*
 * Define prototypes for interrupt handlers here. The conditional "extern"
 * ensures the weak declarations from startup_stm32f4xx.c are overridden.
 */
#ifdef __cplusplus
 extern "C" {
#endif

void SysTick_Handler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void OTG_FS_IRQHandler(void);
void OTG_FS_WKUP_IRQHandler(void);


void NMI_Handler(void)       {}
void HardFault_Handler(void) { ColorfulRingOfDeath(); }
void MemManage_Handler(void) { ColorfulRingOfDeath(); }
void BusFault_Handler(void)  { ColorfulRingOfDeath(); }
void UsageFault_Handler(void){ ColorfulRingOfDeath(); }
void SVC_Handler(void)       {}
void DebugMon_Handler(void)  {}
void PendSV_Handler(void)    {}

#ifdef __cplusplus
}
#endif
