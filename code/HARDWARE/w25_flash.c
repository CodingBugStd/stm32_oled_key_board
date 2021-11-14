#include "w25_flash.h"

uint8_t W25_Flash_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    for(uint8_t temp=0;temp<3;temp++)
    {
        GPIO_InitStruct.GPIO_Pin = W25Q16_Pin[temp].bit;
        GPIO_Init(W25Q16_Pin[temp].port,&GPIO_InitStruct);
        Pin_Set(W25Q16_Pin[temp]);
    }
    W25_CS_Reset;   //片选
    W25_Send(0x9F); //读芯片ID 验证是否挂载W25Q64
    int id;
    id = W25_Send(0xff);
    id = id<<8;
    id |= W25_Send(0xff);
    id = id<<8;
    id |= W25_Send(0xff);
    W25_CS_Set;     //关闭片选
    if(id == W25_FLASH_ID)
        return 0;
    else
        return 1;
}

