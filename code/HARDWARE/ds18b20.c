#include "ds18b20.h"
#include "bsp_hard_delay.h"

static void PinToFloat(void);
static void PinToOutOD(void);

void DS18B20_Init(void)
{
    //引脚初始化
    GPIO_InitTypeDef    GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Pin = DQ.bit;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(DQ.port,&GPIO_InitStruct);

}

void DS18B20_Reset(void)
{
    PinToOutOD();
    Pin_Reset(DQ);
    //hard_delay_us(500);
    Pin_Set(DQ);
    //hard_delay_us(15);
}

void PinToFloat(void)
{
    //清空 MODE5[1:0] 和 CNF5[1:0]
    DQ.port->CRL &= ~((uint32_t)0x0f<<20);
    //CNF5[1:0] = 01
    //MODE5[1:0] = 00
    //即 GPIOB的CRL[23:22:21:20] = 0100
    DQ.port->CRL |= (uint32_t)0x04<<20;
}

void PinToOutOD(void)
{
    //清空 MODE5[1:0] 和 CNF5[1:0]
    DQ.port->CRL &= ~((uint32_t)0x0f<<20);
    //CNF5[1:0] = 10
    //MODE5[1:0] = 01
    //即 GPIOB的CRL[23:22:21:20] = 1001
    DQ.port->CRL |= (uint32_t)0x09<<20;
}

uint8_t DS18B20_Read_Bit(void)
{
    PinToFloat();
    return Pin_Read(DQ);
}
