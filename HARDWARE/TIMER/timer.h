#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F103������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
extern TIM_HandleTypeDef TIM1_Handler;      //��ʱ�����
extern TIM_HandleTypeDef htim2;

void TIM3_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);

 

void TIM1_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM1Compare1(u32 compare);
#endif
