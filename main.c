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
#include "ntc10k3950.h"	
uchar temp1, temp2, temp3, temp4;
char flagrec4=0;
uchar ans4[100] = {0};
int weizhi4 = 0;
static int rectimes4;

char buf1[100];
char buf2[100];
char buf3[100];
char buf4[100];

char flag3=0;
int weishu3=0;
int timeleft1, timeleft2, timeleft3, timeleft4;


void showoxbuf(char *ans4,int len);
int setmodbus4(int dizhi,int zhi);

void pingmuSetData(char *p);
void pingmuGetData(char *p);


// �Ƚ�ֵ�Ƿ����˱仯����
uint16_t bufcheck[len_HoldingReg]={0};
void setmodbuszhi(int dizhi,int zhi)
{
    if(dizhi<0 || dizhi>=100)
    {
        return ;
    }
    HoldingReg[dizhi]=zhi;
}
void buffchecktongbu()
{
    int i;
    for ( i = 0; i < len_HoldingReg; i++)
    {
        bufcheck[i]=HoldingReg[i];
    }
}
void runbuffcheck()
{
    int i;
    for ( i = 0; i < len_HoldingReg; i++)
    {
       if(bufcheck[i]!=HoldingReg[i])
       {
          setmodbus4(i,HoldingReg[i]);   
       }
    }
    buffchecktongbu();
}



void initadc()
{
	P1M0 = 0x00;                                //����P1.1ΪADC��
	P1M1 = 0x03;
	ADC_CONTR = 0x80;                           //ʹ��ADCģ��
	ADCCFG = 0x0f;                              //����ADCʱ��Ϊϵͳʱ��/2/16/16���Ҷ���
}
int getadczhi(int weizhi)
{
	    int ans,ans1;
		if(weizhi==0)
		{
			ADC_CONTR = 0x80;
		}
		else
		{
			ADC_CONTR = 0x81;
		}
	// ADC_CONTR |=weizhi;
	ADC_CONTR |= 0x40;                      //����ADת��
	
		_nop_();
		_nop_();
		while (!(ADC_CONTR & 0x20));            //��ѯADC��ɱ�־
		ADC_CONTR &= ~ 0X20;            //��־λ��Ҫ�ֶ���0
		
		ans=ADC_RES;
		ans=ans<<2;
		ans1=ADC_RESL;
		ans1=ans1>>6;
		 //( *4)+ ADC_RESL>>6;
		 ADC_RES=0;
		 ADC_RESL=0;
		ans=ans+ans1;
	return ans;
}

long getdianzu(long dianya)
{
	if(dianya==1023)
	{
		return 4700; 
	}
	return 4700*dianya/(1023-dianya);
	// dianya/1023*
}

int jisuanwendu(int R)
{
	int p,T;
	unsigned long Ac = 0;
        for ( p=1; p<sizeof(NTC10K3950)/sizeof(NTC); p++ ) {
      if ( R >= NTC10K3950[p].R ) 
	  {
        Ac = R - NTC10K3950[p].R; // delta resistance
        Ac *= 50; //multiply by 5.0 degrees celsius step of table
        Ac /= NTC10K3950[p-1].R - NTC10K3950[p].R; // divide by range of resistence 
        T = NTC10K3950[p].t*10 - Ac; // temperature offset
        break;
      }
    }
	return T;
}
void io_inint()
{
    P0M1 = 0;
    P0M0 = 0;    // ����P0.0~P0.7Ϊ׼˫���
    P1M0 = 0x00; // ����P1.0ΪADC��
    P1M1 = 0x00;
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

extern int recover;
extern void chulimodbus();
int delay_mszhi;
void dealpingmu();
void dealpingmuall()
{
    if(flag3==1)
    {
        flag3=0;
        pingmuSetData(buf3);
        pingmuGetData(buf3);
        dealpingmu();
        memset(buf3, 0, sizeof(buf3));
	    weishu3 = 0;
    }
}
void delay_ms(int m)
{
    delay_mszhi=m*2;
    while (delay_mszhi>=0)
    {
        if (recover == 1)
        {
            jishouokjisuan();
            runbuffcheck();//modbus �Ĳ��ԡ�����
            recover = 0;
        }
        dealpingmuall();
    }
}

int flagget=0;
int getdizhi=0;
// ��ʾ�������Ĳ���������
void pingmuGetData(char *p)
{
	char* index;
	int ans;
	index=strstr(p,"pingmuGetData getdizhi");
	if(index==0)
	{
		return  ;
	}
	ans=atoi(index+strlen("pingmuGetData getdizhi"));
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
// ��ʾ�������Ĳ���������setdizhi02_04
// pingmuSetData setdizhi04-74
void pingmuSetData(char *p)
{
	char* index;
	int ans;
  
	index=strstr(p,"pingmuSetData setdizhi");
	if(index==0)
	{
		return  ;
	}
	ans=atoi(index+strlen("pingmuSetData setdizhi"));
	if(ans==-1)
	{
		return;
	}
	flagset=1;
	setdizhi=ans;
    ans=atoi(index+strlen("pingmuSetData setdizhi")+3);
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
        memset(dataxx,0,sizeof(dataxx));
        sprintf(dataxx,"pingmuSetData zhi%d-%d",setdizhi,setdizhivalue);
        print3(dataxx);
        setmodbus4(setdizhi,setdizhivalue);
    }
    if(flagget==1)
    {
        memset(dataxx,0,sizeof(dataxx));
        flagget=0;
        ans=HoldingReg[getdizhi];
        sprintf(dataxx,"pingmuGetData getdizhi%d-%d",getdizhi,ans);
        print3(dataxx);
    }
}
void dealans4();

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
    delay_mszhi=40;//20�����ղ�������������
    while (delay_mszhi>0)
    {
        if(flagrec4)
        {
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
            // delay_ms(5);
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
int getiicguang()
{
    int dis_data;
    Single_Write_BH1750(0x01); // power on
    Single_Write_BH1750(0x10); // H- resolution mode
    delay_ms(180);
    dis_data = Multiple_Read_BH1750();
    delay_ms(2);
    return dis_data;
}
void main()
{
    int i;
    int ans,ans1;
    int dianzu;
    int wendu;
    io_inint();
    P_SW1=P_SW1|0x80;
    UartInit();
    
    Uart23Init();
    Timer0Init();
    Uart4Init();
    EA = 1;
    delay_ms(2);
    PrintString("system start now");
    delay_ms(2);
    Modbus_ClearBuff();
    delay_ms(2);

    com4read(4);
    delay_ms(10);
    com4read(4);
    initadc();
    while (1)
    {
        if(i%10==0)
        {
            ans1=getadczhi(1);
            dianzu=getdianzu(ans1);
			wendu=jisuanwendu(dianzu);
            HoldingReg[6]=wendu; 
            HoldingReg[5]=getiicguang();
        }
        if(i++>100)
        {
            com4read(4);
            i=0;
        }
        delay_ms(100);
        dealpingmuall();    
    }
}
uchar ans1[100] = {0};
int weizhi1 = 0;
static int rectimes1;
void UARTInterrupt(void) interrupt 4
{
    unsigned char ans;
    if (RI)
    {
        RI = 0;
        ans = SBUF;
        chuankou1jisuuan(ans);
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

void chuankou3put(char c)
{
	buf3[weishu3++] = c;
	if (weishu3 > 80)
		weishu3 = 0;
	timeleft3 = 3;
}
void chuankou3time()
{
	if (timeleft3 > 0)
	{
		timeleft3--;
		if (timeleft3 == 0) // ����һ��������.
		{
			flag3=1;
		}
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
        chuankou3put(temp3);
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // ���S3TIλ
        busy3 = 0;      // ��æ��־
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
        busy4 = 0; // ��æ��־
    }
}
// printf(��Current count: %bd\n��, counter);//���8λ��ʮ�����з���������
// printf(��Current count: %bx\n��, counter);//���8λ���޷�����ʮ�����Ʊ�ʾ��������
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
    timechuli4();
    chuankou3time();
}