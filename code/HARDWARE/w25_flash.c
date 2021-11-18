#include "w25_flash.h"

#define W25_WaitBusy()  while(Read_Status1() & 0x01 == 0x01)    

static void Write_Enable(void);     //写使能
static void Write_Disable(void);    //写失能
static uint8_t Read_Status1(void);  //读状态寄存器1

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
    if(id == W25_FLASH_ID)  //匹配芯片型号0xEF4017 W25Q64  
        return 0;
    else
       return 1;
}

void Write_Enable(void)
{
    //产生下降沿
    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Write_Enable);
    W25_CS_Set;
}

void Write_Disable(void)
{
    //产生下降沿
    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Write_Disable);
    W25_CS_Set;
}

uint8_t Read_Status1(void)
{
    uint8_t temp;
    //产生下降沿
    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Read_SR1);
    temp = W25_Send(0xff);
    W25_CS_Set;
    return temp;
}

void W25_WakeUp(void)
{
    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Release);
    W25_CS_Set;
    soft_delay_ms(1);
}

#include "oled12864.h"
#include <stdio.h>

uint8_t Test(void)
{
    W25_WakeUp();
    Write_Enable();
    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Chip_Erase);
    W25_CS_Set;
    W25_WaitBusy();

    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_ReadData);
    W25_Send(0x01);
    W25_Send(0x12);
    W25_Send(0x12);
    OLED12864_Show_Num(0,0,W25_Send(0xff),1);
    OLED12864_Show_Num(1,0,W25_Send(0xff),1);
    OLED12864_Show_Num(2,0,W25_Send(0xff),1);
    OLED12864_Show_Num(3,0,W25_Send(0xff),1);
    W25_CS_Set;
    W25_WaitBusy();

    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Page_Write);
    W25_Send(0x01);
    W25_Send(0x12);
    W25_Send(0x12);
    W25_Send(0xab);
    W25_Send(0xab);
    W25_Send(0xab);
    W25_Send(0xab);
    W25_CS_Set;
    W25_WaitBusy();

    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_ReadData);
    W25_Send(0x01);
    W25_Send(0x12);
    W25_Send(0x12);
    OLED12864_Show_Num(0,0,W25_Send(0xff),1);
    OLED12864_Show_Num(1,0,W25_Send(0xff),1);
    OLED12864_Show_Num(2,0,W25_Send(0xff),1);
    OLED12864_Show_Num(3,0,W25_Send(0xff),1);
    W25_CS_Set;
    W25_WaitBusy();

    OLED12864_Refresh();

    return 0;
}
