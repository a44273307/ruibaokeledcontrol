#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/6/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
  
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOA时钟
	    __HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOA时钟
	    __HAL_RCC_GPIOD_CLK_ENABLE();           	//开启GPIOA时钟
	    __HAL_RCC_GPIOE_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOF_CLK_ENABLE();           	//开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5; 				//PB5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_6; 				//PE5
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
  
}
