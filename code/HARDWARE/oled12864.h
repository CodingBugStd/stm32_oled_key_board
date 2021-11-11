#ifndef _OLED12864_H_
#define _OLED12864_H_

#include "stm32f10x.h"
#include "self_stm32f10x.h"

/**************************************************************************************
 * 基于stm32f103zet6的OLED12864 4Pin_SPI
 * OLED12864驱动芯片:SSD1306
 * 使用了self_stm32f10x.h
 * 方便引脚初始化 和 输出
 * 
 * 2021/10/16: 将spi分离了出去,留出了spi接口OLED12864_SPI_Send_Byte()
 * 
 * Creat by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/6/22
 * last date: 2021/10/16
 * 
 * 备注:移植时主要更改OLED12864_GPIO_Init()函数和宏定义接口,注意OLED12864的SPI时序!
**************************************************************************************/

//外部接口
#include "bsp_spi.h"
#define OLED12864_delay_ms(ms)                  soft_delay_ms(ms)
#define OLED12864_SPI_Send_Byte(dat)            SPI_Send_Byte(2,dat)
#define OLED12864_Set_Bit(pin_Num)              Pin_Set(OLED_Pin[pin_Num])
#define OLED12864_Reset_Bit(pin_Num)            Pin_Reset(OLED_Pin[pin_Num])
//硬件相关宏定义,定义物理引脚名字
#define OLED_RES_Pin    GPIO_Pin_15
#define OLED_RES_GPIO   GPIOA
#define OLED_DC_Pin     GPIO_Pin_4
#define OLED_DC_GPIO    GPIOA
#define OLED_CS_Pin     GPIO_Pin_11
#define OLED_CS_GPIO    GPIOA
//引脚编号
#define OLED_RES    0
#define OLED_DC     1
#define OLED_CS     2
static const Pin OLED_Pin[3] = {
    {OLED_RES_Pin,OLED_RES_GPIO},
    {OLED_DC_Pin,OLED_DC_GPIO},
    {OLED_CS_Pin,OLED_CS_GPIO}
};


#define x_MAX           128
#define y_MAX           64
#define page_MAX        8
#define USE_POINT_CRT   1   //使用像素点级别操作
#define USE_C_STDLIB    0   //使用C的标准库

#define OLED_CMD    0
#define OLED_DATA   1

/************************************************
 * OLED12864缓存
 * [paeg][x]
 * 一个元素保函8像素点信息
 * 在屏幕上,Bit0~Bit7自上向下排列 高位在下
************************************************/
static uint8_t OLED12864_Sbuffer[8][128];
//OLED_初始化指令
static const uint8_t OLED12864_InitCmd[28] = {
    0xae,0x00,0x10,0x40,0x81,0xcf,
    0xa1,0xc8,0xa6,0xa8,0x3f,0xd3,
    0x00,0xd5,0x80,0xd9,0xf1,0xda,
    0x12,0xdb,0x40,0x20,0x02,0x8d,
    0x14,0xa4,0xa6,0xaf
};

//初始化
void OLED12864_Init(void);
void OLED12864_GPIO_Init(void);
void OLED12864_Hard_Reset(void);

void OLED12864_Send_Byte(uint8_t dat,uint8_t cmd);
void OLED12864_Send_NumByte(const uint8_t*dat,uint8_t len,uint8_t cmd);

void OLED12864_Refresh(void);
void OLED12864_Set_Position(uint8_t page,uint8_t x);
void OLED12864_Clear_Sbuffer(void);   
void OLED12864_Clear(void);

//y坐标位置和高度都以page为单位的图形操作
void OLED12864_Clear_PageBlock(uint8_t page,uint8_t x,uint8_t len);
void OLED12864_Clear_Page(uint8_t page);
void OLED12864_Show_Char(uint8_t page,uint8_t x,uint8_t chr,uint8_t size);
void OLED12864_Show_String(uint8_t page,uint8_t x,char*str,uint8_t size);
uint8_t OLED12864_Show_Num(uint8_t page,uint8_t x,int num,uint8_t size);    //返回num的长度

    //以单个像素点为单位的图形操作
    #if USE_POINT_CRT == 1
    void OLED12864_Draw_Point(uint8_t x,uint8_t y,uint8_t bit);             //<--像素点操作底层函数
    void OLED12864_Draw_Line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
    void OLED12864_Draw_Rect(uint8_t x,uint8_t y,uint8_t len,uint8_t hight);
    //*img 以字节竖直,高位在上,数据水平
    void OLED12864_Draw_Img(uint8_t x,uint8_t y,uint8_t len,uint8_t hight,uint8_t*img);
    void OLED12864_Draw_aImg(uint8_t x,uint8_t y,uint8_t*img);  //自带图形大小识别
    #endif  //USE_POINT_CRT

#endif

