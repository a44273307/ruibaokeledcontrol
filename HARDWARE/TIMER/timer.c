#include "timer.h"
#include "led.h"
#include "io.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/6/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 

TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!

void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}
void TIM2_Init(u16 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE 
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,0);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,0);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
}

//定时器3中断服务函数
__weak void time03msjisuan()   //使用前在主函数初始化  
{
	
}
__weak void time02msjisuan()
{
	
}
void TIM3_IRQHandler(void)
{
    if(__HAL_TIM_GET_IT_SOURCE(&TIM3_Handler,TIM_IT_UPDATE)==SET)
	{
    	 shurulvbo();	
		time02msjisuan();
		__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);    //清空定时器的CNT
	}
   __HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_UPDATE);
}
void TIM2_IRQHandler(void)
{
	if(__HAL_TIM_GET_IT_SOURCE(&TIM2_Handler,TIM_IT_UPDATE)==SET)
	{
	
		__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);    //清空定时器的CNT
		__HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,20);//恢复原来的设置
	}
   __HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);
}
////////////////////////////////////////////////////////////////////////////////// 	
TIM_HandleTypeDef 	TIM1_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM1_CH1Handler;	//定时器1通道1句柄

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
    TIM1_Handler.Instance=TIM1;         	//定时器1
    TIM1_Handler.Init.Prescaler=psc;       	//定时器分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM1_Handler.Init.Period=arr;          	//自动重装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM1_Handler);       	//初始化PWM
    
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH1Handler.Pulse=0;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
	
	
	    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_4);//配置TIM1通道1
	
	 
	
	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);//开启PWM通道1
	 	    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_4);//开启PWM通道1 
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();			//使能定时器1
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		
		GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_11;           	//PA8
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure); 	
	}
}

//设置TIM1通道1的占空比
//compare:比较值
void TIM_SetTIM1Compare1(u32 compare)
{
	TIM1->CCR1=compare; 
}
void TIM_SetTIM1Compare4(u32 compare)
{
	TIM1->CCR4=compare; 
}
//定时器1中断服务函数
void TIM1_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}


TIM_HandleTypeDef htim2;
/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 60000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  //htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 5;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
   // _Error_Handler(__FILE__, __LINE__);
  }
}
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(tim_encoderHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    /**TIM2 GPIO Configuration    
    PA0-WKUP     ------> TIM2_CH1
    PA1     ------> TIM2_CH2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    //GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}
