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


void main(void)
{
	WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR();		//扩展SFR(XFR)访问使能 
	CKCON = 0;      //提高访问XRAM速度
	
	XOSCCR = 0xc0;        //启动外部晶振
	while(!(XOSCCR&1));   //等待时钟稳定
	CLKDIV = 0x00;        //时钟不分频
	CLKSEL = 0x01;        //选择外部时钟		

	SYS_Init();
	test2();
		sendbytecom1('c');
	while (1)
	{
		sendbytecom1('a');
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
		IAP_CONTR = 0x60;
	}

	if(TI)
	{
		TI = 0;
		
      
	}
}
// 电源板子，风扇没开

// lcd板子，按键按下，才能旋转设置值。。其他是ok的，，再按下，保存。。



