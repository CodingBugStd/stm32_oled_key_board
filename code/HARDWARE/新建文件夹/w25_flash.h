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
#define W25_Write_Enable    0x06    //写使能
#define W25_Write_Disable   0x04    //写失能
#define W25_EnableVSR       0x50    //易失性状态寄存器的写启用
#define W25_ReadData        0x03    //读数据
#define W25_FastData        0x0B    //快速读数据
#define W25_32Block_Erase   0x52    //32KB块擦除
#define W25_64Block_Erase   0xD8    //64KB块擦除
#define W25_Chip_Erase      0xC7    //全片擦除
#define W25_Read_SR1        0x05    //读状态寄存器1
#define W25_Read_SR2        0x35    //读状态寄存器2
#define W25_Read_SR3        0x15    //读状态寄存器3
#define W25_Page_Write      0x02    //写数据

uint8_t W25_Flash_Init(void);

uint8_t Test(void);

#endif  //_W25_FLASH_H_

