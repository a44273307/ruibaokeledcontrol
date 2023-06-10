/************************************************************************************
   
程序名称： （晶振频率18.432MHz） 
功能说明: 通过串口3发送“Everything is possible!”  
          波特率9600，数据位8，奇偶效验无，停止位1，数据流控制无  
************************************************************************************/
#include "stc15f2k60s2.h"	    // 单片机STC15F2K60S2头文件,可以不再加入reg51.h
#include <intrins.h>					// 加入此头文件后,可使用_nop_库函数
#include "delay.h"		        // 延时函数头文件
#include "uart.h"		        	// 串行通信函数头文件
#define  uint unsigned int  
#define  uchar unsigned char	
#include <stdio.h>
#include <string.h>
#include "MODBUS.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bit busy;


uint temp1,temp2,temp3,temp4;




//sbit x0 = P4^3;
//sbit x1 = P4^4; 
//sbit x2 = P2^0; 
//sbit x3 = P2^1;  
//sbit x4 = P2^2;  
//sbit x5 = P2^3;
//sbit x6 = P2^4; 
//sbit x7 = P2^5;
//sbit x10 = P2^6;
//sbit x11 = P2^7; 
//sbit x12 = P4^5;
//sbit x13 = P4^6;

sbit x0 = P2^4; 
sbit x1 = P2^5; 


sbit x2 = P2^6;  
sbit x3 = P2^7;

sbit x4 = P4^5; 
sbit x5 = P4^6;

sbit x6 = P0^2;
sbit x7 = P0^3; 

sbit x8 = P0^4; 
sbit x9 = P5^2;




sbit y0 = P2^3;  
sbit y1 = P2^2;	
sbit y2 = P2^1;
sbit y3 = P2^0;
sbit y4 = P4^4;
sbit y5 = P4^3;



void printf2(char *puts)
{
	 while(*puts) 
	{
		sendbyte2(*puts);
		puts++;
	}
}

void io_inint()
{
	P0M1 = 0;	P0M0 = 0;	//设置P0.0~P0.7为准双向口
	    P1M0 = 0x00;                                //设置P1.0为ADC口
    P1M1 = 0x80;


	// P1M1 = 0;	P1M0 = 0;	//设置P1.0~P1.7为准双向口
	P2M1 = 0;	P2M0 = 0;	//设置P2.0~P2.7为准双向口 
	    P3M0 = 0;
    P3M1 = 0;
	P4M1 = 0;	P4M0 = 0;	//设置P4.0~P4.7为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置P5.0~P5.7为准双向口
}




int xinget(int zhi)
{
	if(zhi==0) return x0;
	if(zhi==1) return x1;
	if(zhi==2) return x2;
	if(zhi==3) return x3;
	if(zhi==4) return x4;
	if(zhi==5) return x5;
	if(zhi==6) return x6;
	if(zhi==7) return x7;
	if(zhi==8) return x8;
	if(zhi==9) return x9;
}
void getxin()
{
	int i;
	char datatmp[10];
	for(i=0;i<10;i++)
	{
		sprintf(datatmp,"x%d %d\n",i,xinget(i));
		PrintString(datatmp);
	}
}
void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

void delay_ms(int m)
{
	int x;
	for(x=m;x>0;x--)
	{
		Delay1ms();
	}
	
}
void Start()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x81;                             //发送START命令
    while (busy&&i<6000)i++;
}

void SendData(char dat)
{uint i=0;
    I2CTXD = dat;                               //写数据到数据缓冲区
    busy = 1;
    I2CMSCR = 0x82;                             //发送SEND命令
    while (busy&&i<6000)i++;
}

void RecvACK()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x83;                             //发送读ACK命令
    while (busy&&i<6000)i++;
}

char RecvData()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x84;                             //发送RECV命令
    while (busy&&i<6000)i++;
    return I2CRXD;
}

void SendACK()
{uint i=0;
    I2CMSST = 0x00;                             //设置ACK信号
    busy = 1;
    I2CMSCR = 0x85;                             //发送ACK命令
    while (busy&&i<6000)i++;
}

void SendNAK()
{uint i=0;
    I2CMSST = 0x01;                             //设置NAK信号
    busy = 1;
    I2CMSCR = 0x85;                             //发送ACK命令
    while (busy&&i<6000)i++;
}

void Stop()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x86;                             //发送STOP命令
    while (busy&&i<6000)i++;
}
void I2C_Isr() interrupt 24
{
    _push_(P_SW2);
    P_SW2 |= 0x80;
    if (I2CMSST & 0x40)
    {
        I2CMSST &= ~0x40;                       //清中断标志
        busy = 0;
    }
    _pop_(P_SW2);
}
sbit out1=P1^6;  //
sbit out2=P3^2;
void deanyan()
 {  
	int sw1=2000;
	int sw2=3000;
	Start();                                    //发送起始命令
    SendData(0xc2);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x60);                             //发送存储地址高字节
    RecvACK();
    SendData(sw1/16);                             //发送存储地址低字节
    RecvACK();
    SendData((sw1%16)<<4);                             //写测试数据1
    RecvACK();
    Stop();                                     //发送停止命令

    Start();                                    //发送起始命令
    SendData(0xc0);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x60);                             //发送存储地址高字节
    RecvACK();
    SendData(sw2/16);                             //发送存储地址低字节
    RecvACK();
    SendData((sw2%16)<<4);                             //写测试数据1
    RecvACK();
    Stop();
		
		Start();                                    //发送起始命令
    SendData(0xc2);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x60);                             //发送存储地址高字节
    RecvACK();
    SendData(sw1/16);                             //发送存储地址低字节
    RecvACK();
    SendData((sw1%16)<<4);                             //写测试数据1
    RecvACK();
    Stop();                                     //发送停止命令

    Start();                                    //发送起始命令
    SendData(0xc0);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x60);                             //发送存储地址高字节
    RecvACK();
    SendData(sw2/16);                             //发送存储地址低字节
    RecvACK();
    SendData((sw2%16)<<4);                             //写测试数据1
    RecvACK();
    Stop();
	
		  
 }
void main()		                                       
{
	
	io_inint();
	Uart23Init();
	
	
	Timer0Init();
	UartInit();
	
	Uart4Init();
	PrintString("system is ok\n");
	P_SW2 = 0x80;
    I2CCFG = 0xe0;                              //使能I2C主机模式
    I2CMSST = 0x00;
	 EA = 1;
	out2=1;
	delay_ms(10);
	Modbus_ClearBuff();
delay_ms(200);
deanyan();
	while (1)
	{
		delay_ms(2);
		// printf("0000000000");
		if(recover==1)
		{
			// deanyan();
			jishouokjisuan();
			recover=0;
		}
	}
}
	 
	 



void Timer0() interrupt 1
{
	time1msjisuan();
}
char buf1[100]={0};
int weizhi1=0;
void putchuankou1(char c)
{
    
    buf1[weizhi1++]=c;
    if(weizhi1>80)
    weizhi1=0;
}
void chuliguankji()
{
    char* index;
    index=strstr(buf1,"@STCISP#");
	if(index==0)
	{
		return  ;
	}
    IAP_CONTR=0x60;
    
}
void UARTInterrupt(void) interrupt 4
{
	unsigned char ans;
	if (RI)
	{
		RI = 0;
		ans=SBUF;
        IAP_CONTR=0x60;
 		// putchuankou1(ans);
        // chuliguankji();
	}
	else
	{
		TI = 0;
	}
	if (TI) //发送中断..
	{
		TI = 0;
	}
}

void uart2(void ) interrupt 8 
{
  if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;         
		temp2 = S2BUF;
	}
  if (S2CON & S2TI)
    {
			//y1=0;
        S2CON &= ~S2TI;         //清除S2TI位
        busy2 = 0;               //清忙标志
    }  
}

  


void Uart3() interrupt 17 using 1
{
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI;         //??S3RI?
        temp3 = S3BUF;
		chuankou1jisuuan(temp3);
   }
if (S3CON & S3TI)
    {
        S3CON &= ~S3TI;         //清除S3TI位
        busy3 = 0;               //清忙标志
    }
}

void Uart4() interrupt 18 
{
    if (S4CON & S4RI)
    {
        S4CON &= ~S4RI;         //??S4RI?
        temp4=S4BUF;
   }
if(TI4)
	{
		CLR_TI4();
		busy4 = 0;               //清忙标志
	}
}



