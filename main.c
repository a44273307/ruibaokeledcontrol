/************************************************************************************

�������ƣ� ������Ƶ��18.432MHz��
����˵��: ͨ������3���͡�Everything is possible!��
          ������9600������λ8����żЧ���ޣ�ֹͣλ1��������������
************************************************************************************/
#include "stc15f2k60s2.h" // ��Ƭ��STC15F2K60S2ͷ�ļ�,���Բ��ټ���reg51.h
#include <intrins.h>      // �����ͷ�ļ���,��ʹ��_nop_�⺯��
#include "delay.h"        // ��ʱ����ͷ�ļ�
#include "uart.h"         // ����ͨ�ź���ͷ�ļ�
#include "ledcontrol.h"
#include "comhead.h"
#include <stdio.h>

#include "MODBUS.h"
#include "MODBUS2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uchar temp1, temp2, temp3, temp4;

void io_inint()
{
    P0M1 = 0;
    P0M0 = 0;    // ����P0.0~P0.7Ϊ׼˫���
    P1M0 = 0x00; // ����P1.0ΪADC��
    P1M1 = 0x80;
    P2M1 = 0;
    P2M0 = 0; // ����P2.0~P2.7Ϊ׼˫���
    P3M0 = 0;
    P3M1 = 0;
    P4M1 = 0;
    P4M0 = 0; // ����P4.0~P4.7Ϊ׼˫���
    // P5M1 = 0;	P5M0 = 0;	//����P5.0~P5.7Ϊ׼˫���

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

extern int recover;
extern void chulimodbus();
void main()
{
    int dis_data;
    io_inint();
    Uart23Init();
    Timer0Init();
    Uart4Init();
    EA = 1;

    delay_ms(2);
    UartInit();
    PrintString("system isok");
    print4("system isok");
    delay_ms(2);
    Modbus_ClearBuff();
    delay_ms(2);
    Init_BH1750(); // ��ʼ��BH1750

    while (1)
    {
        if (recover == 1)
        {
            chulimodbus();
            recover = 0;
        }
        delay_ms(2);
    }
    while (1)
    {
        Single_Write_BH1750(0x01); // power on
        Single_Write_BH1750(0x10); // H- resolution mode
        delay_ms(180);
        dis_data = Multiple_Read_BH1750();
        delay_ms(2);
    }
}

extern void time1msjisuan();
void Timer0() interrupt 1
{
    time1msjisuan();
}

char ans1[100] = {0};
int weizhi1 = 0;
static void chuliguankji(char *ans1)
{
    char *index;
    index = strstr(ans1, "@STCISP#");
    if (index == 0)
    {
        return;
    }
    printf("rec @STCISP#,researt now");
    IAP_CONTR = 0x60;
}
void input(char c)
{
    ans1[weizhi1++] = c;
    if (weizhi1 > 80)
    {
        weizhi1 = 0;
    }
    chuliguankji(ans1);
}
void UARTInterrupt(void) interrupt 4
{
    unsigned char ans;
    if (RI)
    {
        RI = 0;
        ans = SBUF;
        input(ans);
        // IAP_CONTR=0x60;
    }
    else
    {
        TI = 0;
    }
    if (TI) // �����ж�..
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
        S2CON &= ~S2TI; // ���S2TIλ
        busy2 = 0;      // ��æ��־
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
        S3CON &= ~S3TI; // ���S3TIλ
        busy3 = 0;      // ��æ��־
    }
}

void Uart4() interrupt 18
{
    unsigned char ans;
    if (S4CON & S4RI)
    {
        S4CON &= ~S4RI; //??S4RI?
        ans = S4BUF;
        chuankou1jisuuan(ans);
    }
    if (TI4)
    {
        CLR_TI4();
        busy4 = 0; // ��æ��־
    }
}