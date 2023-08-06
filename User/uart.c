/************************************************************************************
Copyright,  LCE STUDIO  
�������ƣ�����ͨ�ţ�����Ƶ��18.432MHz��  
����˵��: ����1�ʹ���3��ʼ��,�ַ��������ַ������ͺ���,������9600bps
����汾��1.0 ��2014/06��
����������rob1983
�Ա����̣�http://shop67639654.taobao.com 
************************************************************************************/ 

#include <string.h>
#include "stc32g.h"
#include "config.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>


#include "uart.h"
#include "tongxin.h"
#include "tongxin2.h"
#include "STC32G_Delay.h"
#include "STC32G_UART.h"
//#include "main.h"
 bit busy4;
 bit busy2;
 bit busy3;
//bit busy4;
//void SendData(unsigned char ch);
//void SendString(char *s);
//void UartInit(void);
//void U3SendData(unsigned char ch);
//void U3SendString(char *s);
//void U2SendData(unsigned char ch);
//void U2SendString(char *s);


/*----------------------------
��ʱ��0��ʼ��
----------------------------*/

void Timer0Init(void) // 0.5ms @24.000MHz
{

	AUXR |= 0x80; // ��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0; // ���ö�ʱ��ģʽ
	TL0 = 0x80;	  // ���ö�ʱ��ʼֵ
	TH0 = 0x44;	  // ���ö�ʱ��ʼֵ
	TF0 = 0;	  // ���TF0��־
	TR0 = 1;	  // ��ʱ��0��ʼ��ʱ
	TR0 = 1;	  // ��ʱ��0��ʼ��ʱ
	ET0 = 1;	  // ʹ�ܶ�ʱ��0�ж�
	PT0 = 1;
	EA = 1;
}
/*----------------------------
���ڳ�ʼ��
----------------------------*/

void UartInit(void)		//115200@24.000MHz
{
	SCON = 0x50;  // 8λ����,�ɱ䲨����
	AUXR |= 0x40; // ��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE; // ����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F; // ���ö�ʱ��ģʽ
	TL1 = 0xCC;	  // ���ö�ʱ��ʼֵ
	TH1 = 0xFF;	  // ���ö�ʱ��ʼֵ
	ET1 = 0;	  // ��ֹ��ʱ��%d�ж�
	TR1 = 1;	  // ��ʱ��1��ʼ��ʱ
	ES = 1;
	EA = 1;
	P_SW1 = 0x00; // RXD/P3.0, TXD/P3.1
}
#define  UART3_SW(Pin)				P_SW2 = (P_SW2 & 0xFD) | (Pin << 1)
void Uart23Init(void)		//115200@24.000MHz
{
	S3CON = 0x10;		//8λ����,�ɱ䲨����
	S3CON &= 0xBF;		//����3ѡ��ʱ��2Ϊ�����ʷ�����
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xCC;			//���ö�ʱ��ʼֵ
	T2H = 0xFF;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//������ʱ��2
	
	// S3_S=1;
	// P_SW2 =P_SW2|0x80;
	// P_SW2=P_SW2|0x02;
		UART3_SW(1);		//UART3_SW_P00_P01,UART3_SW_P50_P51
		IE2 |= 0x01;                 //ʹ�ܴ���2�ж�
		IE2 |= 0x08;                 //ʹ�ܴ���3�ж�
		EA=1;
		ES3=1;
	// P_SW2 = (P_SW2 & ~2) | (0 & 0x02);	//�л�IO
	// P_SW2=P_SW2|0x02;
	//	//IE2&=0xFE�رմ���2
//	IE2&=0xF7;	//�رմ���3
}
void Uart4Init(void)		//9600bps@24.000MHz	.����4
{
	S4CON = 0x10;		//8λ����,�ɱ䲨����
	S4CON |= 0x40;		//����4ѡ��ʱ��4Ϊ�����ʷ�����
	T4T3M |= 0x20;		//��ʱ��4ʱ��ΪFosc,��1T
	T4L = 0xCC;		//�趨��ʱ��ֵ
	T4H = 0xFF;		//�趨��ʱ��ֵ
	T4T3M |= 0x80;		//������ʱ��4	
	S4CON &= ~(1<<5);	//��ֹ���ͨѶ��ʽ
	S4CON &= ~(1<<7);	// 8λ����, 1λ��ʼλ, 1λֹͣλ, ��У��
	IE2   |=  (1<<4);	//�����ж�
	//IE2&=0xEF�رմ���4
	S4CON |=  (1<<4);	//�������
//	P_SW2 &= ~4;		//�л��� P0.2 P0.3
}


/*----------------------------
ͨ������1���ʹ�������
----------------------------*/
//void SendData(unsigned char ch)
//{
//    TI     =   0;  //���㴮�ڷ�������ж������־
//    SBUF   =   ch;
//    while(TI ==0); //�ȴ��������
//}

//void sendbyte1(unsigned char ch)
//{
//    TI     =   0;  //���㴮�ڷ�������ж������־
//    SBUF   =   ch;
//    while(TI ==0); //�ȴ��������
//}


void sendbyte1(unsigned char ch)
{
	int i;
	// EA=0;
    TI     =   0;  //���㴮�ڷ�������ж������־
    SBUF   =   ch;
    while(TI ==0) //�ȴ��������
	{
		for(i=0;i<2000; i++){
			if(	TI) break;
		}
		break;
	}
	EA=1;
}

/*----------------------------
ͨ������2���ʹ�������
----------------------------*/
void sendbyte2(unsigned char ch)
{
   while (busy2);               //�ȴ�ǰ������ݷ������                      //��ȡУ��λP (PSW.
   	busy2 = 1;
    S2BUF = ch;                //д���ݵ�UART2���ݼĴ���
}




/*----------------------------
ͨ������3���ʹ�������
----------------------------*/
void sendbyte3(unsigned char ch)
{
    while (busy3);               //�ȴ�ǰ������ݷ������
	  busy3 = 1;
    S3BUF = ch;                //д���ݵ�UART2���ݼĴ���
}
void print3(char *p)
{
	while (*p != '\0')
	{
		sendbyte3(*p);
		p++;
	}
}

/*----------------------------
ͨ������4���ʹ�������
----------------------------*/
void sendbyte4(unsigned char ch)
{
    while (busy4);               //�ȴ�ǰ������ݷ������
	busy4 = 1;
    S4BUF = ch;                //д���ݵ�UART2���ݼĴ���
}
void print4(char *p)
{
	while (*p != '\0')
	{
		sendbyte4(*p);
		p++;
	}
}
void print4len(unsigned char *p,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		sendbyte4(*p);
		p++;
	}
}

void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 57;
	while (--i);
}
char putchar(char dat)
{
	// Delay1us();
	sendbyte1(dat);
	return (dat);
}


char flag3 = 0;
int weishu1, weishu2, weishu3, weishu4;
char buf3[300];
char buf1[300]={0};
int timeleft1, timeleft2, timeleft3, timeleft4;
char falgchuankou1 = 0;
void chuankou1put(char c)
{
	buf3[weishu3++] = c;
	if (weishu3 > sizeof(buf3) - 3)
		weishu3 = 0;
	timeleft3 = 3;
}
void chuankou1time()
{
	if (timeleft3 > 0)
	{
		timeleft3--;
		if (timeleft3 == 0) // ����һ��������.
		{
			flag3 = 1;
		}
	}
}
void jiexi(char* input);

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
            return (char*)haystack; // �Ӵ�ƥ��ɹ���������ʼλ��
        }

        haystack++;
    }

    return NULL; // δ�ҵ��Ӵ�������NULL
}
void dealchuankou()
{
	if (flag3 == 1)
	{
		flag3 = 0;
		jiexi(buf3);
		memset(buf3, 0, sizeof(buf3));
		weishu3 = 0;
	}
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
// ���룬�����20������������ص�ֵ��Ĭ���ǵ���ֵ������
int bakweizhi;
int bakzhi;
void jixi2(char* input)
{
	char *p=input;
	char *p1;
	int i;
	unsigned int weizhi;
	unsigned int zhi;
	//554-2234;333-4;end
    // printf("input %s",input);
	for( i=0;i<100;i++)
	{
		p1=myaddstrstr(p,";"); //����û����һ����
		if(p1==NULL)
		{
			break;
		}
		weizhi = atoi(p);
		p=myaddstrstr(p,"-");
		zhi = atoi(p);
        if(i%2==0)
        {
            bakweizhi=weizhi;
            bakzhi=zhi;
        }
        if(i%2==1)
        {
            if(weizhi==bakweizhi && bakzhi==zhi)
            {
                printf("get set%d-%d",weizhi,zhi);
                push2(weizhi,zhi);
            }
            else
            {
                printf("get failed");
            }
        }
		p=myaddstrstr(p,";");  //ָ����һ������
	}
}
void jiexi(char* input)
{
	char par[500]={0};
	char *begin,end;
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

 void chuliguankji(char *ans1)
{
	char *index;
	index = mystrstr(ans1, "@STCISP#");
	if (index == 0)
	{
		return;
	}
	printf("rec @STCISP#,researt now");
	IAP_CONTR = 0x60;
}



void showbuf()
{
	// if(weishu1>0)
	// {
	// 	delay_ms(2);
	// 	printf(buf1);
	// }

	// weishu1 = 0;
	// buf1[0]=0;
}