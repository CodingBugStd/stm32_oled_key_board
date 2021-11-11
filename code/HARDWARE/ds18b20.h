#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "self_stm32f10x.h"

#define SKIP_ROM    0xcc
#define CONVERT     0xbe

const static Pin DQ = {GPIO_Pin_5,GPIOB};

void DS18B20_Init(void);

uint8_t DS18B20_Read_Bit(void);

#endif

