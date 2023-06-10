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
void Start()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x81;                             //发送START命令
    while (busy && i < 6000)i++;
}

void SendData(char dat)
{uint i=0;
    I2CTXD = dat;                               //写数据到数据缓冲区
    busy = 1;
    I2CMSCR = 0x82;                             //发送SEND命令
    while (busy && i < 6000)i++;
}

void RecvACK()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x83;                             //发送读ACK命令
    while (busy && i < 6000)i++;
}

char RecvData()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x84;                             //发送RECV命令
    while (busy && i < 6000)i++;
    return I2CRXD;
}

void SendACK()
{uint i=0;
    I2CMSST = 0x00;                             //设置ACK信号
    busy = 1;
    I2CMSCR = 0x85;                             //发送ACK命令
    while (busy && i < 6000)i++;
}

void SendNAK()
{uint i=0;
    I2CMSST = 0x01;                             //设置NAK信号
    busy = 1;
    I2CMSCR = 0x85;                             //发送ACK命令
    while (busy && i < 6000)i++;
}

void Stop()
{uint i=0;
    busy = 1;
    I2CMSCR = 0x86;                             //发送STOP命令
    while (busy && i < 6000)i++;
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

void deanyanpre()
 {  
  uint sw1=2047;
  uint sw2=3000;
  sw1=HoldingReg[3];
  sw2=HoldingReg[4];
  printf("***%d-%d***\n",sw2/256,sw2%256);
  Start();        // 发送起始命令
  SendData(0xc2); // 发送设备地址+写命令
  RecvACK();
  SendData(0x60); // 发送存储地址高字节
  RecvACK();
  SendData(sw1 / 16); // 发送存储地址低字节
  RecvACK();
  SendData((sw1 % 16) << 4); // 写测试数据1
  RecvACK();
  Stop(); // 发送停止命令

  Start();        // 发送起始命令
  SendData(0xc0); // 发送设备地址+写命令
  RecvACK();
  SendData(0x60); // 发送存储地址高字节
  RecvACK();
  SendData(sw2 / 16); // 发送存储地址低字节
  RecvACK();
  SendData((sw2 % 16) << 4); // 写测试数据1
  RecvACK();
  Stop();

  Start();        // 发送起始命令
  SendData(0xc2); // 发送设备地址+写命令
  RecvACK();
  SendData(0x60); // 发送存储地址高字节
  RecvACK();
  SendData(sw1 / 16); // 发送存储地址低字节
  RecvACK();
  SendData((sw1 % 16) << 4); // 写测试数据1
  RecvACK();
  Stop(); // 发送停止命令

  Start();        // 发送起始命令
  SendData(0xc0); // 发送设备地址+写命令
  RecvACK();
  SendData(0x60); // 发送存储地址高字节
  RecvACK();
  SendData(sw2 / 16); // 发送存储地址低字节
  RecvACK();
  SendData((sw2 % 16) << 4); // 写测试数据1
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