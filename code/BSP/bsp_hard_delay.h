#ifndef _BSP_HARD_DELAY_H_
#define _BSP_HARD_DELAY_H_

#include "stm32f10x.h"

void BSP_HardDelay_Init(void);
void hard_delay_us(uint16_t us);
void hard_delay_ms(uint16_t ms);

#endif

