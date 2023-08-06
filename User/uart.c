/************************************************************************************
Copyright,  LCE STUDIO  
程序名称：串口通信（晶振频率18.432MHz）  
功能说明: 串口1和串口3初始化,字符发送与字符串发送函数,波特率9600bps
程序版本：1.0 （2014/06）
阿里旺旺：rob1983
淘宝店铺：http://shop67639654.taobao.com 
************************************************************************************/ 

#include <string.h>
#include "stc32g.h"
#include "config.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>


#include "uart.h"
#include "tongxin.h"
#include "tongxin2.h"
#include "STC32G_Delay.h"
#include "STC32G_UART.h"
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

void Timer0Init(void) // 0.5ms @24.000MHz
{

	AUXR |= 0x80; // 定时器时钟1T模式
	TMOD &= 0xF0; // 设置定时器模式
	TL0 = 0x80;	  // 设置定时初始值
	TH0 = 0x44;	  // 设置定时初始值
	TF0 = 0;	  // 清除TF0标志
	TR0 = 1;	  // 定时器0开始计时
	TR0 = 1;	  // 定时器0开始计时
	ET0 = 1;	  // 使能定时器0中断
	PT0 = 1;
	EA = 1;
}
/*----------------------------
串口初始化
----------------------------*/

void UartInit(void)		//115200@24.000MHz
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
#define  UART3_SW(Pin)				P_SW2 = (P_SW2 & 0xFD) | (Pin << 1)
void Uart23Init(void)		//115200@24.000MHz
{
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON &= 0xBF;		//串口3选择定时器2为波特率发生器
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xCC;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//启动定时器2
	
	// S3_S=1;
	// P_SW2 =P_SW2|0x80;
	// P_SW2=P_SW2|0x02;
		UART3_SW(1);		//UART3_SW_P00_P01,UART3_SW_P50_P51
		IE2 |= 0x01;                 //使能串口2中断
		IE2 |= 0x08;                 //使能串口3中断
		EA=1;
		ES3=1;
	// P_SW2 = (P_SW2 & ~2) | (0 & 0x02);	//切换IO
	// P_SW2=P_SW2|0x02;
	//	//IE2&=0xFE关闭串口2
//	IE2&=0xF7;	//关闭串口3
}
void Uart4Init(void)		//9600bps@24.000MHz	.串口4
{
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器4时钟为Fosc,即1T
	T4L = 0xCC;		//设定定时初值
	T4H = 0xFF;		//设定定时初值
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
		for(i=0;i<2000; i++){
			if(	TI) break;
		}
		break;
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
void print3(char *p)
{
	while (*p != '\0')
	{
		sendbyte3(*p);
		p++;
	}
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
void print4(char *p)
{
	while (*p != '\0')
	{
		sendbyte4(*p);
		p++;
	}
}
void print4len(unsigned char *p,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		sendbyte4(*p);
		p++;
	}
}

void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 57;
	while (--i);
}
char putchar(char dat)
{
	// Delay1us();
	sendbyte1(dat);
	return (dat);
}


char flag3 = 0;
int weishu1, weishu2, weishu3, weishu4;
char buf3[300];
char buf1[300]={0};
int timeleft1, timeleft2, timeleft3, timeleft4;
char falgchuankou1 = 0;
void chuankou1put(char c)
{
	buf3[weishu3++] = c;
	if (weishu3 > sizeof(buf3) - 3)
		weishu3 = 0;
	timeleft3 = 3;
}
void chuankou1time()
{
	if (timeleft3 > 0)
	{
		timeleft3--;
		if (timeleft3 == 0) // 数据一次收完了.
		{
			flag3 = 1;
		}
	}
}
void jiexi(char* input);

char* mystrstr(const char* haystack, const char* needle) {
    if (*needle == '\0') {
        return (char*)haystack;
    }

    while (*haystack != '\0') {
        const char* h = haystack;
        const char* n = needle;

        while (*n != '\0' && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char*)haystack; // 子串匹配成功，返回起始位置
        }

        haystack++;
    }

    return NULL; // 未找到子串，返回NULL
}
void dealchuankou()
{
	if (flag3 == 1)
	{
		flag3 = 0;
		jiexi(buf3);
		memset(buf3, 0, sizeof(buf3));
		weishu3 = 0;
	}
}
size_t mystrlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

char* myaddstrstr(const char* haystack, const char* needle)
{	
	char* result = mystrstr(haystack, needle);
	 if (result != NULL)
	 {
		result=result+mystrlen(needle);
	 }
	 return result;
}
// 分离，发命令，20发读的命令，返回的值，默认是电流值。。。
int bakweizhi;
int bakzhi;
void jixi2(char* input)
{
	char *p=input;
	char *p1;
	int i;
	unsigned int weizhi;
	unsigned int zhi;
	//554-2234;333-4;end
    // printf("input %s",input);
	for( i=0;i<100;i++)
	{
		p1=myaddstrstr(p,";"); //找有没有下一个的
		if(p1==NULL)
		{
			break;
		}
		weizhi = atoi(p);
		p=myaddstrstr(p,"-");
		zhi = atoi(p);
        if(i%2==0)
        {
            bakweizhi=weizhi;
            bakzhi=zhi;
        }
        if(i%2==1)
        {
            if(weizhi==bakweizhi && bakzhi==zhi)
            {
                printf("get set%d-%d",weizhi,zhi);
                push2(weizhi,zhi);
            }
            else
            {
                printf("get failed");
            }
        }
		p=myaddstrstr(p,";");  //指向下一个后面
	}
}
void jiexi(char* input)
{
	char par[500]={0};
	char *begin,end;
	begin=myaddstrstr(input,"set:");
	// printf("input begin%s",begin);
	end=myaddstrstr(begin,"end");
	// printf("input end%s",end);
	if(begin!=NULL && end!=NULL)
	{
		strcpy(par,begin);
		jixi2(par);
	}
}

 void chuliguankji(char *ans1)
{
	char *index;
	index = mystrstr(ans1, "@STCISP#");
	if (index == 0)
	{
		return;
	}
	printf("rec @STCISP#,researt now");
	IAP_CONTR = 0x60;
}



void showbuf()
{
	// if(weishu1>0)
	// {
	// 	delay_ms(2);
	// 	printf(buf1);
	// }

	// weishu1 = 0;
	// buf1[0]=0;
}