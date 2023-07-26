#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "zhuanhuan.h"
#include "config.H"
void initio()
{
	GPIO_InitTypeDef GPIO_Initure;
	   __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOD时钟
		__HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOD时钟
		   __HAL_RCC_GPIOD_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();           	//开启GPIOD时钟
		__HAL_RCC_GPIOF_CLK_ENABLE();           	//开启GPIOD时钟
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  	//推挽输出GPIO_MODE_OUTPUT_PP
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
	 GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
                    
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出GPIO_MODE_OUTPUT_PP
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速

	 GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
}

char xin[40];

void shurulvbo(void)
{
	static u8 keybuf[40] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		                                   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		                                   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
																			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		                                  }; //矩阵按键扫描缓冲区 8ms
		unsigned char i;
    
	//按键值移入缓冲区
	
	{		
    keybuf[0] = (keybuf[0] << 1) | X0;
    keybuf[1] = (keybuf[1] << 1) | X1;
	  keybuf[2] = (keybuf[2] << 1) | X2;
	  keybuf[3] = (keybuf[3] << 1) | X3;
    keybuf[4] = (keybuf[4] << 1) | X4;
	  keybuf[5] = (keybuf[5] << 1) | X5;
	  keybuf[6] = (keybuf[6] << 1) | X6;
    keybuf[7] = (keybuf[7] << 1) | X7;
    keybuf[8] = (keybuf[8] << 1) | X6;
    keybuf[9] = (keybuf[9] << 1) |X7;

		


		
	//消抖后更新按键状态
    for (i=0; i<10; i++)  //3按键，所以循环3次
    {
        if ((keybuf[i] & 0xFF) == 0x00)
        {   
            xin[i] = 0;
        }
        else if ((keybuf[i] & 0xFF) == 0xFF)
        {   //连续3次扫描值为1，即1*8ms内都是弹起状态时，可认为按键已稳定的弹起
            xin[i] = 1;
        }
    }
	}  
}
void delayms(int i)
{
	 delay_ms(i);
}