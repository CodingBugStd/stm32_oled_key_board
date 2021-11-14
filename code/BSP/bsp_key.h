#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "self_stm32f10x.h"

//移植时注意更改中断源配置
//200ms采集比较合适
#define KEY_IRQn            1   //开启中断
#define KEY_DEBOUNCE        1   //软件消抖
#define KEY_TIME_DE         240 //软件消抖时间(ms)

#include "bsp_led.h"

#define KEY0_IRQn() LED_Reversal()
#define KEY1_IRQn() LED_Reversal()
#define KEY2_IRQn() LED_Reversal()

const static Pin Key[3] =
{
    {GPIO_Pin_13,GPIOC},
    {GPIO_Pin_14,GPIOC},
    {GPIO_Pin_15,GPIOC}
};

void BSP_Key_Init(void);
uint8_t Read_key(uint8_t num);
uint8_t Read_List_Key(void);

#endif
