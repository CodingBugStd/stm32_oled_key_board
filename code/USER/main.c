#include "stm32f10x.h"
#include <stdio.h>

#include "bsp_spi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_hard_delay.h"
#include "bsp_key.h"

#include "oled12864.h"
#include "ds18b20.h"

#include "self_stm32f10x.h"

uint8_t count = 0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	//关闭JTAG,只保留SWD调试接口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	BSP_SPI_Init();
	BSP_LED_Init();
	BSP_Usart_Init();
	BSP_HardDelay_Init();
	BSP_Key_Init();

	OLED12864_Init();
	DS18B20_Init();

	while(1)
	{
		char temp[60];
		hard_delay_ms(200);
		OLED12864_Clear_PageBlock(0,0,64);
		sprintf(temp,"KeyInput:%d",count);
		OLED12864_Show_String(0,0,temp,1);
		OLED12864_Refresh();
		#if 0
		OLED12864_Show_Num(0,0,num,1);
		OLED12864_Refresh();
		hard_delay_ms(1000);
		LED_Reversal();
		num++;
		#endif
	}
}
