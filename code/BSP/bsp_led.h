#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "bsp_led.h"
#include "self_stm32f10x.h"

static Pin LED = {GPIO_Pin_12,GPIOB};

#define LED_ON()    Pin_Set(LED)
#define LED_OFF()   Pin_Reset(LED)
#define LED_Reversal()  Pin_Reversal(LED)

void BSP_LED_Init(void);

#endif
