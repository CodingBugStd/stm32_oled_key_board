#include "stm32f10x.h"
#include <stdio.h>

#include "bsp_spi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_hard_delay.h"
#include "bsp_key.h"

#include "oled12864.h"
#include "w25_flash.h"
#include "ds18b20.h"

#include "self_stm32f10x.h"

int main(void)
{
	//设置中断分组 4位主优先级 0位子优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	//关闭JTAG调试接口,只保留SWD调试接口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	BSP_HardDelay_Init();	//硬件延时初始化
	BSP_Usart_Init();		//串口初始化
	BSP_SPI_Init();			//硬件SPI初始化
	BSP_Key_Init();			//按键初始化
	BSP_LED_Init();			//LED指示灯初始化

	char tmp[16];
	OLED12864_Init();				//OLED初始化			需要SPI支持
	tmp[0] = W25_Flash_Init();		//W25Q64外扩FLASH初始化	需要SPI支持
	tmp[1] = DS18B20_Init();		//温度传感器初始化		需要硬件延时支持

	Test();

	while(1)
	{
		sprintf(tmp,"Tmp:%.2f",DS18B20_GetTemperature());
		OLED12864_Show_String(7,74,tmp,1);
		OLED12864_Refresh();
		LED_Reversal();
		hard_delay_ms(50);
	}
}
