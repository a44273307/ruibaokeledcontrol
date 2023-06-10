/************************************************************************************

程序名称： （晶振频率18.432MHz）
功能说明: 通过串口3发送“Everything is possible!”
          波特率9600，数据位8，奇偶效验无，停止位1，数据流控制无
************************************************************************************/
#include "stc15f2k60s2.h" // 单片机STC15F2K60S2头文件,可以不再加入reg51.h
#include <intrins.h>      // 加入此头文件后,可使用_nop_库函数
#include "delay.h"        // 延时函数头文件
#include "uart.h"         // 串行通信函数头文件
#define uint unsigned int
#define uchar unsigned char
#include <stdio.h>
#include <string.h>
#include "MODBUS.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ledcontrol.h"
uint temp1, temp2, temp3, temp4;

void io_inint()
{
    P0M1 = 0;
    P0M0 = 0;    // 设置P0.0~P0.7为准双向口
    P1M0 = 0x00; // 设置P1.0为ADC口
    P1M1 = 0x80;
    P2M1 = 0;
    P2M0 = 0; // 设置P2.0~P2.7为准双向口
    P3M0 = 0;
    P3M1 = 0;
    P4M1 = 0;
    P4M0 = 0; // 设置P4.0~P4.7为准双向口
    // P5M1 = 0;	P5M0 = 0;	//设置P5.0~P5.7为准双向口

    P5M0 = 0xff;
    P5M1 = 0x00;
}

void Delay1ms() //@24.000MHz
{
    unsigned char i, j;

    i = 24;
    j = 85;
    do
    {
        while (--j)
            ;
    } while (--i);
}

void delay_ms(int m)
{
    int x;
    for (x = m; x > 0; x--)
    {
        Delay1ms();
    }
}



void main()
{
    int dis_data;
    io_inint();
    Uart23Init();

    Timer0Init();

    Uart4Init();

    P_SW2 = 0x80;
    I2CCFG = 0xe0; // 使能I2C主机模式
    I2CMSST = 0x00;
    EA = 1;
  
    delay_ms(10);
    
  
    UartInit();
    delay_ms(10);
    PrintString("system isok");
Modbus_ClearBuff();
    Init_BH1750(); // 初始化BH1750
    delay_ms(10);
    while (1)
    {
        Single_Write_BH1750(0x01); // power on
        Single_Write_BH1750(0x10); // H- resolution mode
        delay_ms(180);
        dis_data=Multiple_Read_BH1750();
        // printf("runing %d\n", dis_data);
        delay_ms(10);
    }
    while (1)
    {

        delay_ms(1);
       
    }
}

uint time, lv_bo;
extern void time1msjisuan();
void Timer0() interrupt 1
{
    time1msjisuan();
}
void UARTInterrupt(void) interrupt 4
{
    unsigned char ans;
    if (RI)
    {
        RI = 0;
        ans = SBUF;
        // IAP_CONTR = 0x60;
        chuankou1jisuuan(ans);
    }
    else
    {
        TI = 0;
    }
    if (TI) // 发送中断..
    {
        TI = 0;
    }
}

void uart2(void) interrupt 8
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;
        temp2 = S2BUF;
    }
    if (S2CON & S2TI)
    {
        // y1=0;
        S2CON &= ~S2TI; // 清除S2TI位
        busy2 = 0;      // 清忙标志
    }
}

void Uart3() interrupt 17 using 1
{
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI; //??S3RI?
        temp3 = S3BUF;
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // 清除S3TI位
        busy3 = 0;      // 清忙标志
    }
}

void Uart4() interrupt 18
{
    if (S4CON & S4RI)
    {
        S4CON &= ~S4RI; //??S4RI?
        temp4 = S4BUF;
    }
    if (TI4)
    {
        CLR_TI4();
        busy4 = 0; // 清忙标志
    }
}