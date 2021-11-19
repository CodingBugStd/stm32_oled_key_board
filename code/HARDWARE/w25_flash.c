#include "w25_flash.h"

static void Wait_Busy(void);
static void Write_Enable(void);         //写使能
static void Write_Disable(void);        //写失能
static void Send_Addr(uint32_t Addr);   //发送24位地址
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

void Wait_Busy(void)
{
    while(Read_Status1() & 0x01 == 0x01);
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

void Send_Addr(uint32_t Addr)
{
    W25_Send((uint8_t)((Addr)>>16));
    W25_Send((uint8_t)((Addr)>>8));
    W25_Send((uint8_t)Addr);
}

void W25_WakeUp(void)
{
    W25_CS_Set;
    W25_CS_Reset;
    W25_Send(W25_Release);
    W25_CS_Set;
    soft_delay_ms(1);
}

void W25_Erase_Sector(uint32_t Addr)
{
    Wait_Busy();
    Write_Enable();
    W25_CS_Reset;
    W25_Send(W25_Sector_Erase);     //扇区擦除
    Send_Addr(Addr&0xFFF000);       //取扇区头地址
    W25_CS_Set;
}

void W25_Read_Data(uint8_t*buf,uint32_t Addr,uint16_t len)
{
    Wait_Busy();
    W25_CS_Reset;
    W25_Send(W25_ReadData);
    Send_Addr(Addr);
    for(uint16_t temp=0;temp<len;temp++)
    {
        *buf = W25_Send(0xFF);
        buf++;
    }
    W25_CS_Set;
}

void W25_Write_Page(uint8_t*buf,uint32_t Addr,uint16_t len)
{
    Wait_Busy();
    Write_Enable();
    W25_CS_Reset;
    W25_Send(W25_PageProgram);
    Send_Addr(Addr&0xFFFF00);     //取页首地址
    for(uint16_t temp=0;temp<len;temp++)
    {
        W25_Send(*buf);
        buf++;
    }
    W25_CS_Set;
}

#include "oled12864.h"
#include "bsp_usart.h"

void Test(void)
{
    uint8_t buff[64];
    uint8_t a[64];
    for(uint8_t temp=0;temp<64;temp++)
        a[temp] = temp;
    W25_Write_Page(a,0x7F0000,64);
    W25_Read_Data(buff,0x7F0000,64);
    Usart_Send(1,buff,64);
}
