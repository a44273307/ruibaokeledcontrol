#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/6/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
  
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//����GPIOAʱ��
	    __HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOAʱ��
	    __HAL_RCC_GPIOC_CLK_ENABLE();           	//����GPIOAʱ��
	    __HAL_RCC_GPIOD_CLK_ENABLE();           	//����GPIOAʱ��
	    __HAL_RCC_GPIOE_CLK_ENABLE();           	//����GPIOAʱ��
	__HAL_RCC_GPIOF_CLK_ENABLE();           	//����GPIODʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_5; 				//PB5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_6; 				//PE5
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
  
}
