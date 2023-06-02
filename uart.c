/************************************************************************************
Copyright,  LCE STUDIO  
程序名称：串口通信（晶振频率18.432MHz）  
功能说明: 串口1和串口3初始化,字符发送与字符串发送函数,波特率9600bps
程序版本：1.0 （2014/06）
阿里旺旺：rob1983
淘宝店铺：http://shop67639654.taobao.com 
************************************************************************************/ 

#include "stc15f2k60s2.h"
#include "intrins.h"
#include "uart.h"
//#include "main.h"
 bit busy4;
 bit busy2;
 bit busy3;
//bit busy4;
//void SendData(unsigned char ch);
//void SendString(char *s);
//void UartInit(void);
//void U3SendData(unsigned char ch);
//void U3SendString(char *s);
//void U2SendData(unsigned char ch);
//void U2SendString(char *s);


/*----------------------------
定时器0初始化
----------------------------*/

void Timer0Init(void)		//2毫秒@24.000MHz
{


	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0X9A;		//设置定时初值
	TH0 = 0XA9;		//设置定时初值
//	TL0 = 0x20;		//设置定时初值
//	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	PT0 = 1;
	ET0 = 1;
	TR0 = 1;		//定时器0开始计时
	EA=1;
}
/*----------------------------
串口初始化
----------------------------*/

void UartInit(void)		//115200@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES=1;
	//	ES=0;//关闭串口0中断
	EA=1;
}
void Uart23Init(void)		//115200@24.000MHz
{
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON &= 0xBF;		//串口3选择定时器2为波特率发生器
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	IE2 |= 0x01;                 //使能串口2中断
	IE2 |= 0x08;                 //使能串口3中断
	EA=1;
	P_SW2 = (P_SW2 & ~2) | (0 & 0x02);	//切换IO
	//	//IE2&=0xFE关闭串口2
//	IE2&=0xF7;	//关闭串口3
}
void Uart4Init(void)		//9600bps@24.000MHz	.串口4
{
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器4时钟为Fosc,即1T
	T4L = 0x8F;		//设定定时初值
	T4H = 0xFD;		//设定定时初值
	T4T3M |= 0x80;		//启动定时器4	
	S4CON &= ~(1<<5);	//禁止多机通讯方式
	S4CON &= ~(1<<7);	// 8位数据, 1位起始位, 1位停止位, 无校验
	IE2   |=  (1<<4);	//允许中断
	//IE2&=0xEF关闭串口4
	S4CON |=  (1<<4);	//允许接收
//	P_SW2 &= ~4;		//切换到 P0.2 P0.3
}


/*----------------------------
通过串口1发送串口数据
----------------------------*/
//void SendData(unsigned char ch)
//{
//    TI     =   0;  //清零串口发送完成中断请求标志
//    SBUF   =   ch;
//    while(TI ==0); //等待发送完成
//}

//void sendbyte1(unsigned char ch)
//{
//    TI     =   0;  //清零串口发送完成中断请求标志
//    SBUF   =   ch;
//    while(TI ==0); //等待发送完成
//}


void sendbyte1(unsigned char ch)
{
	int i;
	// EA=0;
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   ch;
    while(TI ==0) //等待发送完成
	{
		// for(i=0;i<2000; i++){
		// 	if(	TI) break;
		// }
		// break;



	}
	EA=1;
}

/*----------------------------
通过串口2发送串口数据
----------------------------*/
void sendbyte2(unsigned char ch)
{
   while (busy2);               //等待前面的数据发送完成                      //获取校验位P (PSW.
   	busy2 = 1;
    S2BUF = ch;                //写数据到UART2数据寄存器
}




/*----------------------------
通过串口3发送串口数据
----------------------------*/
void sendbyte3(unsigned char ch)
{
    while (busy3);               //等待前面的数据发送完成
	  busy3 = 1;
    S3BUF = ch;                //写数据到UART2数据寄存器
}

/*----------------------------
通过串口4发送串口数据
----------------------------*/
void sendbyte4(unsigned char ch)
{
    while (busy4);               //等待前面的数据发送完成
	  busy4 = 1;
    S4BUF = ch;                //写数据到UART2数据寄存器
}
int debug=1;
void PrintString(const char *puts)
{
	if (debug==0)
	{
		return ;
	}
	
    for (; *puts != 0;	puts++)  sendbyte1(*puts); 	//遇到停止符0结束
}


