#include "bsp_key.h"


void BSP_Key_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    for(uint8_t temp=0;temp<3;temp++)
    {
        GPIO_InitStruct.GPIO_Pin = Key[temp].bit;
        GPIO_Init(Key[temp].port,&GPIO_InitStruct);
    }

    #if KEY_IRQn == 1
    //配置
    EXTI_InitTypeDef    EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    EXTI_InitStruct.EXTI_Line = EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;

    EXTI_Init(&EXTI_InitStruct);
    
    NVIC_InitTypeDef    NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);

    #endif
}

uint8_t Read_key(uint8_t num)
{
    if(num<3)
    {
        if(Pin_Read(Key[num]))
            return 0;
        else
            return 1;
    }
    return 0;
}

//只能读取单个按键的状态
//多个按下时取最序号最低值
uint8_t Read_List_Key(void)
{
    for(uint8_t temp=0;temp<3;temp++)
    {
        if(Pin_Read(Key[temp]) == 0)
            return temp+1;
    }
    return 0;
}

#if KEY_IRQn == 1

void EXTI15_10_IRQHandler(void)
{

    if(EXTI_GetITStatus(EXTI_Line13))
    {
            KEY0_IRQn();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if(EXTI_GetITStatus(EXTI_Line14))
    {
            KEY1_IRQn();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if(EXTI_GetITStatus(EXTI_Line15))
    {
            KEY2_IRQn();
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
    #if USE_SOFT_DEBOUNCE == 1
        if(flag == 1)
        last_time = key_time;
    #endif
}

#endif  //KEY_IRQn
