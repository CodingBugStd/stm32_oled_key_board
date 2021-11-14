#ifndef _SELF_STM32F10X_H_
#define _SELF_STM32F10X_H_

#include "stm32f10x.h"

/**************************************************************************************
 * stm32f10x的相关类型定义
 * 方便快速连续初始化引脚 和 直接操作引脚 
 * 软件延时
 * 
 * Creat by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/8/22
 * last date: 2021/10/16
**************************************************************************************/

//用于快速初始化IO口以及操作IO口
typedef struct
{
    uint16_t bit;           //Pin的标号
    GPIO_TypeDef*port;      //所属的GPIO端口
}Pin;

void Pin_Set(Pin pin);
void Pin_Reset(Pin pin);
void Pin_Reversal(Pin pin);
uint8_t Pin_Read(Pin pin);
void soft_delay_ms(uint16_t ms);
void soft_delay_us(uint16_t us);

void startDebugTiming(void);         //开始计时
uint16_t endDebugTiming(void);       //结束计时 返回计时时长 us

#endif
