#ifndef _W25_FLASH_H_
#define _W25_FLASH_H_

#include "self_stm32f10x.h"
#include "bsp_spi.h"

#define W25_Send(dat)   SPI_Replace_Byte(1,dat)
#define W25_CS_Reset    Pin_Reset(W25Q16_Pin[W25_CS])
#define W25_CS_Set      Pin_Set(W25Q16_Pin[W25_CS])

#define W25_HOLD_Port   GPIOA
#define W25_HOLD_Pin    GPIO_Pin_1
#define W25_WP_Port     GPIOA
#define W25_WP_Pin      GPIO_Pin_0
#define W25_CS_Port     GPIOA
#define W25_CS_Pin      GPIO_Pin_12

#define W25_HOLD    0
#define W25_WP      1
#define W25_CS      2
const static Pin W25Q16_Pin[3] = 
{
    {W25_HOLD_Pin,W25_HOLD_Port},
    {W25_WP_Pin,W25_WP_Port},
    {W25_CS_Pin,W25_CS_Port}
};

#define W25_FLASH_ID    0xEF4017    //w25q64的ID号

uint8_t W25_Flash_Init(void);

#endif  //_W25_FLASH_H_

