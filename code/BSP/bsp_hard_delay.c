#include "bsp_hard_delay.h"

void BSP_HardDelay_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Prescaler = 72 - 1;      //1MHz
    TIM_InitStruct.TIM_Period = 60000;
    TIM_InitStruct.TIM_RepetitionCounter = 0;   //不循环计数

    TIM_TimeBaseInit(TIM2,&TIM_InitStruct);

    TIM2->CR1 |= 0x01<<3;    //单次更新事件发生后停止计数
}

/**************************************************************************
 * 功能:硬件延时
 * 参数:要延时的时长(单位:us)
 * 返回值:无
 * 备注:
 *  us=1时会进入死循环!
 *  已经修复
 * 2021/6/22   庞碧璋
 ***********************************************************************/
void hard_delay_us(uint16_t us)
{
    if(us==1)
    {
        soft_delay_us(1);
        return;
    }
    TIM2->CR1 &= ~0x01; //关闭定时器
    TIM2->CNT = 0;      //计数器归零
    TIM2->ARR = us - 1; //重装载值设置
    TIM2->EGR |= 0x01;  //重新初始化计数器并生成寄存器的更新
    TIM2->SR = 0x00;    //标志位全部清零
    TIM2->CR1 |= 0x01;  //开启TIM2
    while( (TIM2->SR & 0x01) != 0x01);
}

void hard_delay_ms(uint16_t ms)
{
    while(ms--)
        hard_delay_us(1000 - 1);    //时钟修正1
}
