/************************************************************************************

程序名称： （晶振频率18.432MHz）
功能说明: 通过串口3发送“Everything is possible!”
          波特率9600，数据位8，奇偶效验无，停止位1，数据流控制无
************************************************************************************/
#include "stc15f2k60s2.h" // 单片机STC15F2K60S2头文件,可以不再加入reg51.h
#include <intrins.h>      // 加入此头文件后,可使用_nop_库函数
#include "delay.h"        // 延时函数头文件
#include "uart.h"         // 串行通信函数头文件
#include "ledcontrol.h"
#include "comhead.h"
#include <stdio.h>

#include "MODBUS.h"
#include "MODBUS2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uchar temp1, temp2, temp3, temp4;
char flagrec4=0;
uchar ans4[100] = {0};
int weizhi4 = 0;
static int rectimes4;
void showoxbuf(char *ans4,int len);
int setmodbus4(int dizhi,int zhi);
void io_inint()
{
    P0M1 = 0;
    P0M0 = 0;    // 设置P0.0~P0.7为准双向口
    P1M0 = 0x00; // 设置P1.0为ADC口
    P1M1 = 0x00;
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

extern int recover;
extern void chulimodbus();
int delay_mszhi;
void dealpingmu();
void delay_ms(int m)
{
    delay_mszhi=m*2;
    while (delay_mszhi>=0)
    {
        if (recover == 1)
        {
            chulimodbus();
            recover = 0;
        }
        dealpingmu();
    }
}
void readdianliu()
{

}
int flagget=0;
int getdizhi=0;
// 显示屏过来的操作。。。
void pingmuGetData(char *p)
{
	char* index;
	int ans;
	index=strstr(p,"getdizhi");
	if(index==0)
	{
		return  ;
	}
	ans=atoi(index+strlen("getdizhi"));
	if(ans==-1)
	{
		return;
	}
	flagget=1;
	getdizhi=ans;
  
}
int flagset=0;
int setdizhi=0;
int setdizhivalue=0;
// 显示屏过来的操作。。。setdizhi02_04
void pingmuSetData(char *p)
{
    
	char* index;
	int ans;
  
	index=strstr(p,"setdizhi");
	if(index==0)
	{
		return  ;
	}
	ans=atoi(index+strlen("setdizhi"));
	if(ans==-1)
	{
		return;
	}
	flagset=1;
	setdizhi=ans;
    ans=atoi(index+strlen("setdizhi")+3);
	if(ans==-1)
	{
		return;
	}
    setdizhivalue=ans;
    
}

void dealpingmu()
{
    char dataxx[100]={0};
    int ans;
    if(flagset==1)
    {
        flagset=0;
        printf("pingmuSetData zhi%d-%d",setdizhi,setdizhivalue);
        setmodbus4(setdizhi,setdizhivalue);
    }
    if(flagget==1)
    {
        flagget=0;
        ans=HoldingReg[getdizhi];
        printf("pingmuGetData getdizhi%d-%d",getdizhi,ans);
    }
}
void dealans4();
int setmodbus4(int dizhi,int zhi)
{
    unsigned char req[10]={0x01,0x06,0x00,0x04,0x00,0x00,0xC8,0x0B};
    unsigned short crc;
    req[3]=dizhi%256;
    req[4]=zhi>> 8;
    req[5]=zhi& 0x00FF;
    crc=Modbus_CRC16(req,6);
    req[6]=crc>> 8;
    req[7]=crc& 0x00FF;
    print4len(req,8);
    delay_mszhi=40;//20毫秒收不到，出问题了
    while (delay_mszhi>0)
    {
        if(flagrec4)
        {
            
            showoxbuf(req,8);
            printf("set dianliu %d__",zhi);
            flagrec4=0;
            dealans4();
            break;
        }   
    }
}

void com4read(int dizhi)
{
    unsigned char req[10]={0x01,0x03,0x00,0x04,0x00,0x01,0xC5,0xCB};
    unsigned short crc;
    unsigned short zhi;
    crc=Modbus_CRC16(req,6);
    req[6]=crc>> 8;
    req[7]=crc& 0x00FF;
    // printf("print req %bx %bx\n",req[6],req[7]);    
    print4len(req,8);
    delay_mszhi=100;
    
    while (delay_mszhi>0)
    {
        if(flagrec4)
        {
            flagrec4=0;
            showoxbuf(req,8);
            zhi=ans4[3] << 8 | ans4[4];
            HoldingReg[4]=zhi; 
            printf("___ dianliu %d--",zhi);
            dealans4();
            break;
        }   
    }
}
void testxxx()
{
    int zhi=2000;
    while (1)
    {
        setmodbus4(4,zhi);
        delay_ms(50);
        com4read(3);
        zhi=zhi*1.1; 
        if(zhi>4095)
        {
            zhi=100;
        }
        delay_ms(500);
    }
}
void getiicguang()
{
    int dis_data;
    Single_Write_BH1750(0x01); // power on
    Single_Write_BH1750(0x10); // H- resolution mode
    delay_ms(180);
    dis_data = Multiple_Read_BH1750();
    delay_ms(2);
}
void main()
{
    int i;
    io_inint();
    UartInit();
    Uart23Init();
    Timer0Init();
    Uart4Init();
    EA = 1;
    delay_ms(2);
    PrintString("system isok");
    print3("system isok");
    delay_ms(2);
    Modbus_ClearBuff();
    delay_ms(2);

    com4read(4);
    delay_ms(10);
    com4read(4);

    while (1)
    {
        if(i++>100)
        {
            com4read(4);
            i=0;
        }
        delay_ms(100);
        dealpingmu();    
    }
}



uchar ans1[100] = {0};
int weizhi1 = 0;
static int rectimes1;
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
void input1(unsigned char c)
{
    ans1[weizhi1++] = c;
    if (weizhi1 > 80)
    {
        weizhi1 = 0;
    }
    rectimes1=5;
}
void timechuli1()
{
    if(rectimes1>0)
    {
        rectimes1--;
        if(rectimes1==0)
        {
            chuliguankji(ans1);
            pingmuSetData(ans1);
            pingmuGetData(ans1);
            memset(ans1,0,sizeof(ans1));
            weizhi1=0;
        }
    }
}
void UARTInterrupt(void) interrupt 4
{
    unsigned char ans;
    if (RI)
    {
        RI = 0;
        ans = SBUF;
        input1(ans);
        // IAP_CONTR=0x60;
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
void input4(uchar c)
{
    ans4[weizhi4++] = c;
    if (weizhi4 > 80)
    {
        weizhi4 = 0;
    }
    rectimes4=2;
}
void Uart4() interrupt 18
{
    unsigned char ans;
    if (S4CON & S4RI)
    {
        S4CON &= ~S4RI; //??S4RI?
        ans = S4BUF;
        input4(ans);
    }
    if (TI4)
    {
        CLR_TI4();
        busy4 = 0; // 清忙标志
    }
}
// printf(“Current count: %bd\n”, counter);//输出8位”十进制有符号整数”
// printf(“Current count: %bx\n”, counter);//输出8位”无符号以十六进制表示的整数”
void clearans4()
{
    memset(ans4,0,sizeof(ans4));
    weizhi4=0;
}
void showoxbuf(char *ans4,int len)
{
       int i;
    for(i=0;i<len;i++)
    {
        printf("%bx ",ans4[i]);
    }
}
void dealans4()
{
    int i;
    if(weizhi4==0)
    {
        printf("not rec");
        return ;
    }
    showoxbuf(ans4,weizhi4);
    memset(ans4,0,sizeof(ans4));
    weizhi4=0;
}

extern void time1msjisuan();

void timechuli4()
{
    if(rectimes4>0)
    {
        rectimes4--;
        if(rectimes4==0)
        {
            flagrec4=1;
        }
    }
}
void Timer0() interrupt 1
{
    delay_mszhi--;
    time1msjisuan();
    timechuli1();
    timechuli4();
}