#include "irmp.h"
#include "irmpconfig.h"
#include "irmpprotocols.h"
#include "irmpsystem.h"
#include "stm32_ub_led.h"
#include "stm32_ub_irmp.h"

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include "usb_conf/usb_bsp.h"
#include "usb_conf/usb_conf.h"
#include "usb_conf/usbd_conf.h"
#include "usb_conf/usbd_desc.h"

#include "usb_lib/cdc/usbd_cdc_core.h"
#include "usb_lib/cdc/usbd_cdc_vcp.h"
#include "usb_lib/core/usbd_core.h"
#include "usb_lib/core/usbd_def.h"
#include "usb_lib/core/usbd_ioreq.h"
#include "usb_lib/core/usbd_req.h"
#include "usb_lib/core/usbd_usr.h"
#include "usb_lib/otg/usb_core.h"
#include "usb_lib/otg/usb_dcd.h"
#include "usb_lib/otg/usb_dcd_int.h"
#include "usb_lib/otg/usb_defines.h"
#include "usb_lib/otg/usb_regs.h"


int lastCommand;
uint8_t flag;
IRMP_DATA  myIRData;

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

#define HSE_VALUE ((uint32_t)8000000) 

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



