/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include	"Task.h"
#include	"System_init.h"
#include	"APP.h"

#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"
#include "uart.h"

void	GPIO_configcom3(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//初始化
}

/***************  串口初始化函数 *****************/
void	UART_configcom3(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer3;			//选择波特率发生器, BRT_Timer3, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	UART_Configuration(UART3, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
	NVIC_UART3_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	UART3_SW(UART3_SW_P50_P51);		//UART3_SW_P00_P01,UART3_SW_P50_P51
}


//========================================================================
// 函数: void	main(void)
// 描述: 主函数程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2012-10-22
//========================================================================
void test2()
{
		SCON = 0x50;  // 8位数据,可变波特率
	AUXR |= 0x40; // 定时器时钟1T模式
	AUXR &= 0xFE; // 串口1选择定时器1为波特率发生器
	TMOD &= 0x0F; // 设置定时器模式
	TL1 = 0xCC;	  // 设置定时初始值
	TH1 = 0xFF;	  // 设置定时初始值
	ET1 = 0;	  // 禁止定时器%d中断
	TR1 = 1;	  // 定时器1开始计时
	ES = 1;
	EA = 1;
	P_SW1 = 0x00; // RXD/P3.0, TXD/P3.1

}
void Delay1ms()		//@11.0592MHz
{
	unsigned long i;

	_nop_();
	_nop_();
	_nop_();
	i = 5998UL;
	while (i) i--;
}


void delayx_ms(unsigned int ms)
{
	while(ms--)
	{
		Delay1ms();
	}
}
extern void Sample_Lamp(void);
void sendbytecom1(unsigned char ch)
{
	int i;
	// EA=0;
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   ch;
    while(TI ==0) //等待发送完成
	{
		for(i=0;i<2000; i++){
			if(	TI) break;
		}
		break;
	}
	EA=1;
}
#define maxjindu 16
#define qidian 0
void showhenxiang()
{
	int i;
	char dataxx[60] = {0};
	static int runflag = 0;


	for (i = 0; i < maxjindu; i++)
	{
		dataxx[i] = '-';
	}

	LCD_ShowString(qidian, 140, dataxx, RED, WHITE, 32, 0);
	LCD_ShowString(qidian, 140 + 40, dataxx, RED, WHITE, 32, 0);
	LCD_ShowString(qidian, 140 + 40 + 30, "0            12bit          2047", RED, WHITE, 16, 0);
}

void	GPIO_confibase(void)
{
//	P2_MODE_IO_PU( GPIO_Pin_3 | GPIO_Pin_4 );		//P2 设置为准双向口
//	P2_MODE_OUT_PP(GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
    P2_MODE_IO_PU(GPIO_Pin_All);	
	
	P3_MODE_OUT_PP(GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7);
	P3_MODE_IN_HIZ(GPIO_Pin_4|GPIO_Pin_6);
	
	P4_MODE_OUT_PP(GPIO_Pin_2|GPIO_Pin_3);
	P4_MODE_IN_HIZ(GPIO_Pin_0|GPIO_Pin_1);
}
void main(void)
{
	WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR();		//扩展SFR(XFR)访问使能 
	CKCON = 0;      //提高访问XRAM速度
	
	XOSCCR = 0xc0;        //启动外部晶振
	while(!(XOSCCR&1));   //等待时钟稳定
	CLKDIV = 0x00;        //时钟不分频
	CLKSEL = 0x01;        //选择外部时钟		
	GPIO_confibase();
	SYS_Init();

	test2();
	sendbytecom1('c');
	GPIO_configcom3();
	sendbytecom1('d');
	Uart23Init();
	sendbytecom1('e');

	while (1)
	{
		
		sendbytecom1('a');
		print3("test3");
		delayx_ms(100);
		sendbytecom1('b');
		Sample_Lamp();
		showhenxiang();
		
	}
}

void UART1_ISR_Handler (void) interrupt UART1_VECTOR
{


	if(RI)
	{
		RI = 0;
		printf("rec init now");
		IAP_CONTR = 0x60;
	}

	if(TI)
	{
		TI = 0;
		
      
	}
}
// 电源板子，风扇没开

// lcd板子，按键按下，才能旋转设置值。。其他是ok的，，再按下，保存。。



void Uart3() interrupt 17
{
	char temp3; 
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI; //??S3RI?
		temp3 = S3BUF;
		// chuankou1put(temp3);
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // 清除S3TI位
        busy3 = 0;      // 清忙标志
    }
}