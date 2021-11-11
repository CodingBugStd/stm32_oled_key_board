#include "self_stm32f10x.h"

void Pin_Set(Pin pin)
{
    pin.port->ODR |= pin.bit;
}

void Pin_Reset(Pin pin)  
{
    pin.port->ODR &= ~pin.bit;
}

uint8_t Pin_Read(Pin pin)
{
    if( pin.port->IDR & pin.bit)
        return 1;
    else
        return 0;
}

void Pin_Reversal(Pin pin)
{
    if(pin.port->ODR & pin.bit )
        pin.port->ODR &= ~pin.bit;
    else
        pin.port->ODR |= pin.bit;
}

//注意:软件延时可能会被编译器优化掉!!
void soft_delay_ms(uint16_t ms)
{
    while(ms!=0)
    {
        soft_delay_us(1000);
        ms--;
    }
}

void soft_delay_us(uint16_t us)
{
  	volatile uint16_t i = 0;
	while(us--)
	{
	    i = 2;
	    while(i--);
	}
}

//freeRTOS运行后禁止硬件Debug计时
void startDebugTiming(void)
{
    #ifdef INC_FREERTOS_H
    //freeRTOS运行后禁止硬件Debug计时
    if(xTaskGetSchedulerState()==taskSCHEDULER_NOT_STARTED)
    {
    #endif //#ifdef INC_FREERTOS_H

    SysTick->LOAD  = (uint32_t)(0xfff - 1UL);                         /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk; 
                     
    #ifdef INC_FREERTOS_H
    }
    #endif //#ifdef INC_FREERTOS_H
}

//freeRTOS运行后禁止硬件Debug计时
uint16_t endDebugTiming(void)
{
    #ifdef INC_FREERTOS_H
    //freeRTOS运行后禁止硬件Debug计时
    if(xTaskGetSchedulerState()==taskSCHEDULER_NOT_STARTED)
    {
    #endif //#ifdef INC_FREERTOS_H

    uint32_t temp;

    //关闭SysTick定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    temp = SysTick->VAL;
    temp = temp/(SystemCoreClock/1000000);  //us
    return (uint16_t)temp;

    #ifdef INC_FREERTOS_H
    }
    return 0;
    #endif //#ifdef INC_FREERTOS_H
}

#if 0
void SysTick_Handler(void)
{
    //判断是否装载了freeRTOS系统
    #ifdef INC_FREERTOS_H
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED) //如果freeRTOS已经运行
    {
        xPortSysTickHandler();
    }
    #endif  //_freeRTOS_
}
#endif
