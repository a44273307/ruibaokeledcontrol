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
void Start()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x81;                             //����START����
    while (busy && i < 6000)i++;
}

void SendData(char dat)
{uint i=0;
    I2CTXD = dat;                               //д���ݵ����ݻ�����
    busy = 1;
    I2CMSCR = 0x82;                             //����SEND����
    while (busy && i < 6000)i++;
}

void RecvACK()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x83;                             //���Ͷ�ACK����
    while (busy && i < 6000)i++;
}

char RecvData()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x84;                             //����RECV����
    while (busy && i < 6000)i++;
    return I2CRXD;
}

void SendACK()
{uint i=0;
    I2CMSST = 0x00;                             //����ACK�ź�
    busy = 1;
    I2CMSCR = 0x85;                             //����ACK����
    while (busy && i < 6000)i++;
}

void SendNAK()
{uint i=0;
    I2CMSST = 0x01;                             //����NAK�ź�
    busy = 1;
    I2CMSCR = 0x85;                             //����ACK����
    while (busy && i < 6000)i++;
}

void Stop()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x86;                             //����STOP����
    while (busy && i < 6000)i++;
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

void deanyanpre()
 {  
  uint sw1=2047;
  uint sw2=3000;
  sw1=HoldingReg[3];
  sw2=HoldingReg[4];
  printf("***%d-%d***\n",sw2/256,sw2%256);
  Start();        // ������ʼ����
  SendData(0xc2); // �����豸��ַ+д����
  RecvACK();
  SendData(0x60); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData(sw1 / 16); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData((sw1 % 16) << 4); // д��������1
  RecvACK();
  Stop(); // ����ֹͣ����

  Start();        // ������ʼ����
  SendData(0xc0); // �����豸��ַ+д����
  RecvACK();
  SendData(0x60); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData(sw2 / 16); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData((sw2 % 16) << 4); // д��������1
  RecvACK();
  Stop();

  Start();        // ������ʼ����
  SendData(0xc2); // �����豸��ַ+д����
  RecvACK();
  SendData(0x60); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData(sw1 / 16); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData((sw1 % 16) << 4); // д��������1
  RecvACK();
  Stop(); // ����ֹͣ����

  Start();        // ������ʼ����
  SendData(0xc0); // �����豸��ַ+д����
  RecvACK();
  SendData(0x60); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData(sw2 / 16); // ���ʹ洢��ַ���ֽ�
  RecvACK();
  SendData((sw2 % 16) << 4); // д��������1
  RecvACK();
  Stop();
		  
 }
 
sbit led1 = P3 ^ 7;
sbit led2 = P3 ^ 6;
 void deanyan()
 {
      led1 = 0;
      led2 = 1;
      deanyanpre();

      led1 = 1;
      led2 = 0;
 }