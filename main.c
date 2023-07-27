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
#include "tongxin2.h"
#include "EEPROM.h"
char buf3[60];
char flag3 = 0;
int weishu3;
int timeleft1, timeleft2, timeleft3, timeleft4;
void com1clearbuf()
{
    memset(buf3, 0, sizeof(buf3));
	weishu3 = 0;
}
void chuankou1put(char c)
{
	buf3[weishu3++] = c;
	if (weishu3 > sizeof(buf3) - 3)
		weishu3 = 0;
	timeleft3 = 3;
}
void dealchuankou();	
void chuliguankji();
void chuankou1time()
{
	if (timeleft3 > 0)
	{
		timeleft3--;
		if (timeleft3 == 0) // 数据一次收完了.
		{
			flag3 = 1;
            dealchuankou();	
		}
	}
}




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
void jixi2(char* input)
{
	char *p=input;
	char *p1;
	int i;
	unsigned int weizhi;
	unsigned int zhi;
	//554-2234;333-4;end
    printf("input %s",input);
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
        printf("get set%d-%d",weizhi,zhi);
        push2(weizhi,zhi);
		p=myaddstrstr(p,";");  //指向下一个后面
		
	}
}
void jiexi(char* input)
{
	char par[100]={0};
	char *begin;
	char *end;
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

char* my_strstr(const char* haystack, const char* needle) {
    if (*needle == '\0') {
        return (char*) haystack;
    }

    while (*haystack != '\0') {
        const char* h = haystack;
        const char* n = needle;

        while (*h == *n && *n != '\0') {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char*) haystack;
        }

        haystack++;
    }

    return NULL;
}

// 串口的处理，，开机，解析等等。
void dealchuankou()
{
    chuliguankji();
	jiexi(buf3);
	memset(buf3, 0, sizeof(buf3));
	weishu3 = 0;
}


int delay_mszhi;
uint temp1,temp2;


void io_inint()
{
	P0M1 = 0;	P0M0 = 0;	//设置P0.0~P0.7为准双向口
  P1M0 = 0x00; // 设置P1.0为ADC口
  P1M1 = 0x80;
	P2M1 = 0;	P2M0 = 0;	//设置P2.0~P2.7为准双向口 
	    P3M0 = 0;
    P3M1 = 0;
	P4M1 = 0;	P4M0 = 0;	//设置P4.0~P4.7为准双向口
	// P5M1 = 0;	P5M0 = 0;	//设置P5.0~P5.7为准双向口

	    P5M0 = 0xff; P5M1 = 0x00; 

}


void delay_ms(int m);



sbit out2=P3^2;

extern void deanyan();
sbit fen=P5^4;


#define Iapid 0x0000
void IapErase(int addr);
void Exxwrite(int addr, uint dat);
void writebuf()
{
    int i;
    int dizhi=Iapid;
    IapErase(Iapid); // ????????
    Exxwrite(dizhi,55);
    for ( i = 0; i < len_HoldingReg; i++)
    {
        dizhi=dizhi+1;
        Exxwrite(dizhi,HoldingReg[i]);
    }
}
// 比较值是否发生了变化。。
int bufcheck[len_HoldingReg]={0};
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

int timereport=0;
void sendzhi(int weizhi,int zhi)
{
    char out[30]={0};
    sprintf(out,"set:%d-%d;end",weizhi,zhi);
	printf(out);
}
void runreport()
{
    if(timereport>300)
    {
        timereport=0;
        sendzhi(4,HoldingReg[4]);
    }
}
void chuliguankji()
{
    char* index;
    index=strstr(buf3,"@STCISP#");
	if(index==NULL)
	{
	    return  ;
	}
    IAP_CONTR=0x60;
}

void IapIdle()
{
    IAP_CONTR = 0;                              //关闭IAP功能
    IAP_CMD = 0;                                //清除命令寄存器
    IAP_TRIG = 0;                               //清除触发寄存器
    IAP_ADDRH = 0x80;                           //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

uchar IapRead(int addr)
{
    uchar dat;

    IAP_CONTR = 0x80;                           //使能IAP
    IAP_TPS = 40;                               //设置等待参数12MHz
    IAP_CMD = 1;                                //设置IAP读命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();
    dat = IAP_DATA;                             //读IAP数据
    IapIdle();                                  //关闭IAP功能

    return dat;
}

void IapProgram(int addr, char dat)
{
    IAP_CONTR = 0x80;                           //使能IAP
    IAP_TPS = 40;                               //设置等待参数12MHz
    IAP_CMD = 2;                                //设置IAP写命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_DATA = dat;                             //写IAP数据
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();
    IapIdle();                                  //关闭IAP功能
}

void IapErase(int addr)
{
    IAP_CONTR = 0x80;                           //使能IAP
    IAP_TPS = 40;                               //设置等待参数12MHz
    IAP_CMD = 3;                                //设置IAP擦除命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();                                    //
    IapIdle();                                  //关闭IAP功能
}



void Exxwrite(int addr, uint dat)
{
    addr=addr*2;
    IapProgram(addr,dat/256);
    IapProgram(addr+1,dat%256);
}

int ExxRead(int addr)
{
    uint dat1,dat2;
    int dat3;
    addr=addr*2;
    dat1=IapRead(addr);
    // printf("ExxRead[%d]\n",dat1);

    
    dat2=IapRead(addr+1);
    // printf("ExxRead[%d]\n",dat2);
    dat3=dat1*256+dat2;
    // printf("ExxRead[%d]\n",dat3);
    return dat3; 
}

void readbuf()
{
    int i;
    int zhi;
    int dizhi=Iapid;
    zhi=ExxRead(dizhi);
    if(zhi==55)
    {
        printf("has init\n");
    }
    else
    {
        printf("not init\n");
        HoldingReg[4]=600;
        writebuf();
    }
    for ( i = 0; i < len_HoldingReg; i++)
    {
        dizhi=dizhi+1;
        HoldingReg[i]=ExxRead(dizhi);
        // delay_ms(4);
        zhi=HoldingReg[i];
        printf("HoldingReg[%d]-[%d]\n",i,zhi);
    }
}
void initbuf()
{
    int i;
    for ( i = 0; i < len_HoldingReg; i++)
    {
        HoldingReg[i]=0;
    }
    readbuf();
}

void getzhiandchange()
{
    int weizhi,zhi;
    Alltongxininfo2 get={0};
    pop22(&get);
    if(get.weizhi==0)
    {
        return ;
    }
    weizhi=get.weizhi;
    zhi=get.zhi;
    printf("getzhiandchange weizhi[%d] zhi[%d]\n",weizhi,zhi);
    if(weizhi<len_HoldingReg)
    {
        HoldingReg[weizhi]=zhi;
    }
    if(weizhi==4 || weizhi==2)
	{
		deanyan();
	}
    if(IsbuffcheckFailed())
    {
        // printf("xiugaidata begin\n");
        delay_mszhi=0;
        buffchecktongbu();
        writebuf();
        // printf("HoldingReg[4] %d",HoldingReg[4]);
        // printf("xiugaidata end %d\n",delay_mszhi);
        readbuf();
    }
}
void main()		                                       
{
	io_inint();
    UartInit();
	Uart23Init();
	Timer0Init();
	Uart4Init();
	P_SW2 = 0x80;
    I2CCFG = 0xe0;                              //使能I2C主机模式
    I2CMSST = 0x00;
	EA = 1;
	out2=1;
	Modbus_ClearBuff();
    deanyan();
	delay_ms(100);
    printf("system init ok\n");
    initbuf();
    buffchecktongbu();
    printf("system init ok1");
    HoldingReg[2]=1;
    com1clearbuf();
    while (1)
	{
        runreport();
        getzhiandchange();
	}
} 
int delay_mszhi;	 
uint time,lv_bo;
void delay_ms(int m)
{
    delay_mszhi=0;
    while (delay_mszhi<=m*2)
    {
        
    }
}
void Timer0() interrupt 1
{
    chuankou1time();
	delay_mszhi++;
    timereport++;
}
void UARTInterrupt(void) interrupt 4
{
	unsigned char ans;
	if (RI)
	{
		RI = 0;
		ans=SBUF;
        chuankou1put(ans);
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

  




