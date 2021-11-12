#include "bsp_key.h"

#if KEY_DEBOUNCE == 1

static uint32_t key_time;

#endif //KEY_DEBOUNCE

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

    #if KEY_DEBOUNCE == 1

    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 1000 - 1;
    TIM_InitStruct.TIM_Prescaler = 72 - 1;
    TIM_InitStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM3,&TIM_InitStruct);

    NVIC_InitTypeDef    NVIC_InitStruct_2;

    NVIC_InitStruct_2.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct_2.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct_2.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct_2.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStruct_2);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3,ENABLE);

    #endif  //KEY_DEBOUNCE
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

extern uint8_t count;

#if KEY_IRQn == 1

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line13))
    {
        #if KEY_DEBOUNCE == 1
            static uint32_t last_time = 0;
            if( (last_time + KEY_TIME_DE) < key_time )
            {
                KEY0_IRQn();
                last_time = key_time;
            }
        #else
            KEY0_IRQn();
        #endif  //KEY0_IRQn();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if(EXTI_GetITStatus(EXTI_Line14))
    {
        #if KEY_DEBOUNCE == 1
            static uint32_t last_time = 0;
            if( (last_time + KEY_TIME_DE) < key_time )
            {
                KEY1_IRQn();
                last_time = key_time;
            }
        #else
            KEY1_IRQn();
        #endif  //KEY0_IRQn();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if(EXTI_GetITStatus(EXTI_Line15))
    {
        #if KEY_DEBOUNCE == 1
            static uint32_t last_time = 0;
            if( (last_time + KEY_TIME_DE) < key_time )
            {
                KEY2_IRQn();
                last_time = key_time;
            }
        #else
            KEY2_IRQn();
        #endif  //KEY0_IRQn();
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

#endif  //KEY_IRQn

#if KEY_DEBOUNCE == 1

void TIM3_IRQHandler(void)
{
    if( TIM_GetITStatus(TIM3,TIM_IT_Update) == SET )
    {
        key_time++;
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    }
}

#endif //KEY_DEBOUNCE
