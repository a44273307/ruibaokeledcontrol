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

uint temp1,temp2,temp3,temp4;


void io_inint()
{
	P0M1 = 0;	P0M0 = 0;	//����P0.0~P0.7Ϊ׼˫���
  P1M0 = 0x00; // ����P1.0ΪADC��
  P1M1 = 0x80;
	P2M1 = 0;	P2M0 = 0;	//����P2.0~P2.7Ϊ׼˫��� 
	    P3M0 = 0;
    P3M1 = 0;
	P4M1 = 0;	P4M0 = 0;	//����P4.0~P4.7Ϊ׼˫���
	// P5M1 = 0;	P5M0 = 0;	//����P5.0~P5.7Ϊ׼˫���

	    P5M0 = 0xff; P5M1 = 0x00; 

}


void delay_ms(int m);



sbit out2=P3^2;

extern void deanyan();
sbit fen=P5^4;

#define Iapid 0x0000
uchar p1, a_a;
void IapIdle()
{
  IAP_CONTR = 0;    // ??��?IAP????
  IAP_CMD = 0;      // ?????��???????��
  IAP_TRIG = 0;     // ??????��??????��
  IAP_ADDRH = 0x80; // ?????��?��????��?IAP???��
  IAP_ADDRL = 0;
}

uint8_t IapRead(int addr)
{
  uint8_t dat;

  IAP_CONTR = 0x80;      // ????IAP
  IAP_TPS = 40;          // ?��??????????12MHz
  IAP_CMD = 1;           // ?��??IAP???��??
  IAP_ADDRL = addr;      // ?��??IAP?????��
  IAP_ADDRH = addr >> 8; // ?��??IAP?????��
  IAP_TRIG = 0x5a;       // ????��??��??(0x5a)
  IAP_TRIG = 0xa5;       // ????��??��??(0xa5)
  _nop_();
  dat = IAP_DATA; // ??IAP????
  IapIdle();      // ??��?IAP????

  return dat;
}

void IapProgram(int addr, char dat)
{
  IAP_CONTR = 0x80;      // ????IAP
  IAP_TPS = 40;          // ?��??????????12MHz
  IAP_CMD = 2;           // ?��??IAP???��??
  IAP_ADDRL = addr;      // ?��??IAP?????��
  IAP_ADDRH = addr >> 8; // ?��??IAP?????��
  IAP_DATA = dat;        // ??IAP????
  IAP_TRIG = 0x5a;       // ????��??��??(0x5a)
  IAP_TRIG = 0xa5;       // ????��??��??(0xa5)
  _nop_();
  IapIdle(); // ??��?IAP????
}

void IapErase(int addr)
{
  IAP_CONTR = 0x80;      // ????IAP
  IAP_TPS = 40;          // ?��??????????12MHz
  IAP_CMD = 3;           // ?��??IAP?????��??
  IAP_ADDRL = addr;      // ?��??IAP?????��
  IAP_ADDRH = addr >> 8; // ?��??IAP?????��
  IAP_TRIG = 0x5a;       // ????��??��??(0x5a)
  IAP_TRIG = 0xa5;       // ????��??��??(0xa5)
  _nop_();               //
  IapIdle();             // ??��?IAP????
}

//
void write_eeprom()
{
  IapErase(Iapid); // ????????
  IapProgram(Iapid + 1, p1);

  IapProgram(Iapid + 60, a_a);
}
/******************��????????????��????eeprom????????*****************/
void read_eeprom()
{
  uchar t, t1;
  p1 = IapRead(Iapid + 1);
  a_a = IapRead(Iapid + 60);
}
/**************???����??��eeprom??????*****************/
void init_eeprom()
{
  read_eeprom();
  if (a_a != 6) // ?????????��?????????��????eeprom
  {

    p1 = 0;
    a_a = 6;

    write_eeprom(); // ��???????
  }
}
void Exxwrite(int addr, uint dat)
{
    addr=addr*2;
    IapProgram(addr,dat/256);
    IapProgram(addr+1,dat%256);
}
uint ExxRead(int addr)
{
    uint dat;
    addr=addr*2;
    dat=IapRead(addr);
    dat=dat*256;
    dat=dat+IapRead(addr+1);
    return dat;
}
void writebuf();
void initbuf()
{
    int i;
    int dizhi=Iapid+1;
    for ( i = 0; i < len_HoldingReg; i++)
    {
        HoldingReg[i]=0;
    }
    if(123!=ExxRead(dizhi))//û�г�ʼ������ʼ��һ�Ρ���
    {
        Exxwrite(dizhi,123);
        writebuf();
    }
    for ( i = 0; i < len_HoldingReg; i++)
    {
        dizhi=dizhi+1;
        HoldingReg[i]=ExxRead(dizhi);
        if(0!=HoldingReg[i] && -1 !=HoldingReg[i])
        {
            printf("HoldingReg[%d]-[%d]",i,HoldingReg[i]);
        }
    }
}
void writebuf()
{
    int i;
    int dizhi=Iapid+1;
    for ( i = 0; i < len_HoldingReg; i++)
    {
        dizhi=dizhi+1;
        Exxwrite(dizhi,HoldingReg[i]);
    }
}
// �Ƚ�ֵ�Ƿ����˱仯����
uint16_t bufcheck[len_HoldingReg]={0};
void buffchecktongbu()
{
    int i;
    for ( i = 0; i < len_HoldingReg; i++)
    {
        bufcheck[i]=HoldingReg[i];
    }
}
int IsbuffcheckFailed()
{
    int i;
    for ( i = 0; i < len_HoldingReg; i++)
    {
       if(bufcheck[i]!=HoldingReg[i])
       {
        return 1;
       }
    }
    return 0;
}
void main()		                                       
{
	io_inint();
    UartInit();
	Uart23Init();
	Timer0Init();
	Uart4Init();
	P_SW2 = 0x80;
    I2CCFG = 0xe0;                              //ʹ��I2C����ģʽ
    I2CMSST = 0x00;
	EA = 1;
	out2=1;
	Modbus_ClearBuff();
    deanyan();
	delay_ms(10);
    printf("system init ok");
    initbuf();
    buffchecktongbu();
    printf("system init ok1");
	while (1)
	{
		if (recover == 1)
        {
			chuliguankji();
            jishouokjisuan();
            if(IsbuffcheckFailed())
            {
                buffchecktongbu();
                deanyan();
                writebuf();
            }
            recover = 0;
        }
	}
} 
int delay_mszhi;	 
uint time,lv_bo;
void delay_ms(int m)
{
    delay_mszhi=m*2;
    while (delay_mszhi>=0)
    {
        
    }
}
void Timer0() interrupt 1
{
	time1msjisuan();
	delay_mszhi--;
}
void UARTInterrupt(void) interrupt 4
{
	unsigned char ans;
	if (RI)
	{
		RI = 0;
		ans=SBUF;
 		chuankou1jisuuan(ans);
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