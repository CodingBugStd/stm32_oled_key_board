#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "self_stm32f10x.h"
#include "bsp_hard_delay.h"

#define DQ_SET      Pin_Set(DQ)
#define DQ_RESET    Pin_Reset(DQ)
#define DQ_Delay_us(us)     hard_delay_us(us)
#define DQ_Delay_ms(ms)     hard_delay_ms(ms)

const static Pin DQ = {GPIO_Pin_5,GPIOB};

uint8_t DS18B20_Init(void);
float DS18B20_GetTemperature(void);

uint8_t DS18B20_ReadByte(void);
uint8_t BS18B20_WriteByte(uint8_t dat);
uint8_t DS18B20_CheckDevice(void);
uint8_t DS18B20_CheckReadySingle(void);
void DS18B20_SendRestSingle(void);

#endif

