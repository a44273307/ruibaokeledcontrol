#include "io.h"

char yout[32];
u16 yout_total;
static void inset(void)
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
	
				 GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);


}

static void outset(void)
{
	GPIO_InitTypeDef GPIO_Initure;                                       //GPIO_MODE_OUTPUT_OD      //GPIO_MODE_OUTPUT_PP
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出GPIO_MODE_OUTPUT_PP
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速

			 GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_11;;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				 GPIO_Initure.Pin=GPIO_PIN_15;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
			 GPIO_Initure.Pin=GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}
void initio()
{
	inset();
	outset();
}

char xin[30];

void shurulvbo(void)
{
	static u8 keybuf[40] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		                                   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		                                   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
																			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		                                  }; //矩阵按键扫描缓冲区 8ms
		unsigned char i;
	{		
    keybuf[0] = (keybuf[0] << 1) | X0;
    keybuf[1] = (keybuf[1] << 1) | X1;
	  keybuf[2] = (keybuf[2] << 1) | X2;
	  keybuf[3] = (keybuf[3] << 1) | X3;
    keybuf[4] = (keybuf[4] << 1) | X4;
	  keybuf[5] = (keybuf[5] << 1) | X5;
	  keybuf[6] = (keybuf[6] << 1) | X6;
//    keybuf[7] = (keybuf[7] << 1) |X7;
//    keybuf[8] = (keybuf[8] << 1) | X8;
//    keybuf[9] = (keybuf[9] << 1) |X9;
//		keybuf[10] = (keybuf[10] << 1) | X10;
//    keybuf[11] = (keybuf[11] << 1) |X11;
	//消抖后更新按键状态
    for (i=0; i<12; i++)  //3按键，所以循环3次
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
char yout_set(char weizhi,char zhi)
{
if(weizhi==0)Y0=zhi;
if(weizhi==1)Y1=zhi;
if(weizhi==2)Y2=zhi;
if(weizhi==3)Y3=zhi;
if(weizhi==4)Y4=zhi;
if(weizhi==5)Y5=zhi;
if(weizhi==6)Y6=zhi;
if(weizhi==7)Y7=zhi;
if(weizhi==8)Y8=zhi;
if(weizhi==9)Y9=zhi;
if(weizhi==10)Y10=zhi;
if(weizhi==11)Y11=zhi;
if(weizhi==12)Y12=zhi;
if(weizhi==13)Y13=zhi;
if(weizhi==14)Y14=zhi;
if(weizhi==15)Y15=zhi;
if(weizhi==16)Y16=zhi;
if(weizhi==17)Y17=zhi;
if(weizhi==18)Y18=zhi;
if(weizhi==19)Y19=zhi;
if(weizhi==20)Y20=zhi;
if(weizhi==21)Y21=zhi;
if(weizhi==22)Y22=zhi;
if(weizhi==23)Y23=zhi;
	return(0);
}
char yout_get(char weizhi)
{
	if(weizhi==0)return(Y0);
if(weizhi==1)return(Y1);
if(weizhi==2)return(Y2);
if(weizhi==3)return(Y3);
if(weizhi==4)return(Y4);
if(weizhi==5)return(Y5);
if(weizhi==6)return(Y6);
if(weizhi==7)return(Y7);
if(weizhi==8)return(Y8);
if(weizhi==9)return(Y9);
if(weizhi==10)return(Y10);
if(weizhi==11)return(Y11);
if(weizhi==12)return(Y12);
if(weizhi==13)return(Y13);
if(weizhi==14)return(Y14);
if(weizhi==15)return(Y15);
if(weizhi==16)return(Y16);
if(weizhi==17)return(Y17);
if(weizhi==18)return(Y18);
if(weizhi==19)return(Y19);
if(weizhi==20)return(Y20);
if(weizhi==21)return(Y21);
if(weizhi==22)return(Y22);
if(weizhi==23)return(Y23);
}
void yout_closeall()
{
	char i;
	for(i=0;i<24;i++)
	yout_set(i,1);
}
u16 yout_change(void)//
{
	char i;
	u16 s=0;
	u16 t;
	for(i=0;i<12;i++)
	{
		t=yout[i]<<i;
		s=s|t;
	}
	return(s);
}
void outzhi(u16 s)//收到的数据,转换成y的输出..
{
	char i,j;
	u16 t;
	for(i=0;i<16;i++)
	{
		j=s%2;
		yout_set(i,j);
		s=s>>1;
	}
}
void xfenjie(u16 s)//将收到的数据值,分解成x12_x23
{
	char i,j;
	for(i=0;i<12;i++)
	{
		xin[i+12]=s%2;
		s=s>>1;
	}
}
char in_get(char weizhi)
{
	return(xin[weizhi]);
}
char in_allget()
{
	char i;
	for(i=0;i<23;i++)
	printf("x%d是%d\n",i,xin[i]);
}



//外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
 

    
    //中断线0-PA0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
 
}




