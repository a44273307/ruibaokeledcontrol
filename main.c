/************************************************************************************
   
�������ƣ� ������Ƶ��18.432MHz�� 
����˵��: ͨ������3���͡�Everything is possible!��  
          ������9600������λ8����żЧ���ޣ�ֹͣλ1��������������  
************************************************************************************/
#include "stc15f2k60s2.h"	    // ��Ƭ��STC15F2K60S2ͷ�ļ�,���Բ��ټ���reg51.h
#include <intrins.h>					// �����ͷ�ļ���,��ʹ��_nop_�⺯��
#include "delay.h"		        // ��ʱ����ͷ�ļ�
#include "uart.h"		        	// ����ͨ�ź���ͷ�ļ�
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
	P0M1 = 0;	P0M0 = 0;	//����P0.0~P0.7Ϊ׼˫���
	    P1M0 = 0x00;                                //����P1.0ΪADC��
    P1M1 = 0x80;


	// P1M1 = 0;	P1M0 = 0;	//����P1.0~P1.7Ϊ׼˫���
	P2M1 = 0;	P2M0 = 0;	//����P2.0~P2.7Ϊ׼˫��� 
	    P3M0 = 0;
    P3M1 = 0;
	P4M1 = 0;	P4M0 = 0;	//����P4.0~P4.7Ϊ׼˫���
	P5M1 = 0;	P5M0 = 0;	//����P5.0~P5.7Ϊ׼˫���
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
    I2CMSCR = 0x81;                             //����START����
    while (busy&&i<6000)i++;
}

void SendData(char dat)
{uint i=0;
    I2CTXD = dat;                               //д���ݵ����ݻ�����
    busy = 1;
    I2CMSCR = 0x82;                             //����SEND����
    while (busy&&i<6000)i++;
}

void RecvACK()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x83;                             //���Ͷ�ACK����
    while (busy&&i<6000)i++;
}

char RecvData()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x84;                             //����RECV����
    while (busy&&i<6000)i++;
    return I2CRXD;
}

void SendACK()
{uint i=0;
    I2CMSST = 0x00;                             //����ACK�ź�
    busy = 1;
    I2CMSCR = 0x85;                             //����ACK����
    while (busy&&i<6000)i++;
}

void SendNAK()
{uint i=0;
    I2CMSST = 0x01;                             //����NAK�ź�
    busy = 1;
    I2CMSCR = 0x85;                             //����ACK����
    while (busy&&i<6000)i++;
}

void Stop()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x86;                             //����STOP����
    while (busy&&i<6000)i++;
}
void I2C_Isr() interrupt 24
{
    _push_(P_SW2);
    P_SW2 |= 0x80;
    if (I2CMSST & 0x40)
    {
        I2CMSST &= ~0x40;                       //���жϱ�־
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
	Start();                                    //������ʼ����
    SendData(0xc2);                             //�����豸��ַ+д����
    RecvACK();
    SendData(0x60);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData(sw1/16);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData((sw1%16)<<4);                             //д��������1
    RecvACK();
    Stop();                                     //����ֹͣ����

    Start();                                    //������ʼ����
    SendData(0xc0);                             //�����豸��ַ+д����
    RecvACK();
    SendData(0x60);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData(sw2/16);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData((sw2%16)<<4);                             //д��������1
    RecvACK();
    Stop();
		
		Start();                                    //������ʼ����
    SendData(0xc2);                             //�����豸��ַ+д����
    RecvACK();
    SendData(0x60);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData(sw1/16);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData((sw1%16)<<4);                             //д��������1
    RecvACK();
    Stop();                                     //����ֹͣ����

    Start();                                    //������ʼ����
    SendData(0xc0);                             //�����豸��ַ+д����
    RecvACK();
    SendData(0x60);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData(sw2/16);                             //���ʹ洢��ַ���ֽ�
    RecvACK();
    SendData((sw2%16)<<4);                             //д��������1
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
    I2CCFG = 0xe0;                              //ʹ��I2C����ģʽ
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
	if (TI) //�����ж�..
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
        S2CON &= ~S2TI;         //���S2TIλ
        busy2 = 0;               //��æ��־
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
        S3CON &= ~S3TI;         //���S3TIλ
        busy3 = 0;               //��æ��־
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
		busy4 = 0;               //��æ��־
	}
}



