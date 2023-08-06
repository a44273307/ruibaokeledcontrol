// 	@�����ٳ� 2023/03/10
// 	PWM������ģʽʾ������ʾ�����������ģʽ1/2/3��֧�֣�SMCR = 1��ģʽ1��SMCR = 2:ģʽ2��SMCR = 3��ģʽ3
// 	EC11�������������ı����������
//	EC11���Ŷ��壺A��-P1.0��B��-P5.4
//  MAX7219�����ģ�����Ŷ��壺CS = P6^5;DIN = P6^6;CLK = P6^4;������MAX7219.h���޸ģ�
//  ʵ�鿪���壺��������.1 @��Ƶ12MHz








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
#include "STC32G_EEPROM.h"

void SYS_Ini();		// STC32��ʼ������
void EC11_Handle(); // EC11���ݴ���
					// �������ʾ
void PWM_Config(); // PWM��ʼ������

u8 cnt_H, cnt_L;	 // ����ֵ��8λ����8λ
u16 count, newcount; // ��ǰ����ֵ���ϴμ���ֵ
					 // ���ȼ���ֵ



char rec2[500];
int weizhi2=0;


// �����ȥ�����ӵ�Ҫ����2�������ļ���ֵ����

int bili=2;
void SYS_Ini() // STC32��ʼ������
{
	EAXFR = 1;	  // ʹ�ܷ��� XFR
	CKCON = 0x00; // �����ⲿ���������ٶ�Ϊ���
	WTST = 0x00;  // ���ó������ȴ���������ֵΪ 0 �ɽ� CPU ִ�г�����ٶ�����Ϊ���
	P0M1 = 0x00;
	P0M0 = 0x00; // ����P0��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P1M1 = 0x00;
	P1M0 = 0x00; // ����P1��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P2M1 = 0x00;
	P2M0 = 0x00; // ����P2��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P3M1 = 0x00;
	P3M0 = 0x00; // ����P3��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P4M1 = 0x00;
	P4M0 = 0x00; // ����P4��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P5M1 = 0x00;
	P5M0 = 0x00; // ����P5��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P6M1 = 0x00;
	P6M0 = 0x00; // ����P6��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P7M1 = 0x00;
	P7M0 = 0x00; // ����P7��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
}



void Delay100ms() //@24.000MHz
{
	unsigned long i;

	_nop_();
	_nop_();
	i = 599998UL;
	while (i)
		i--;
}
// ����printf����
void printf1(const char *fmt, ...)
{
	char *p;
	char buf[128]; // ����һ�����������㹻�洢������ַ���
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args); // ����ʽ�����ַ���д�뻺����
	va_end(args);

	p = (unsigned char *)buf;
	while (*p != '\0')
	{
		sendbyte1(*p);
		p++;
	}
}

// ����printf����
void print2(const char *fmt, ...)
{
	char *p;
	char buf[400]={0}; // ����һ�����������㹻�洢������ַ���
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args); // ����ʽ�����ַ���д�뻺����
	va_end(args);
	p = (unsigned char *)buf;
	while (*p != '\0')
	{
		sendbyte1(*p);
		p++;
	}
}


// ����printf����
void printf3(const char *fmt, ...)
{
	char *p;
	char buf[128]; // ����һ�����������㹻�洢������ַ���
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args); // ����ʽ�����ַ���д�뻺����
	va_end(args);

	p = (unsigned char *)buf;
	while (*p != '\0')
	{
		sendbyte3(*p);
		p++;
	}
}
char code duanzhi[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x76, 0x00, 0x40, 0xff};

sbit led4 = P4 ^ 3;
sbit led3 = P4 ^ 4;
sbit led2 = P2 ^ 0;
sbit led1 = P2 ^ 1;
#define Y0 led1
#define Y1 led2
#define Y2 led3
#define Y3 led4

char yout_set(char weizhi, char zhi)
{
	if (weizhi == 0)
		Y0 = zhi;
	if (weizhi == 1)
		Y1 = zhi;
	if (weizhi == 2)
		Y2 = zhi;
	if (weizhi == 3)
		Y3 = zhi;

	return (0);
}
void yout_closeall()
{
	char i;
	for (i = 0; i < 4; i++)
	{
		yout_set(i, 1);
	}
}
void Delay10us() //@24.000MHz
{
	unsigned long i;

	_nop_();
	_nop_();
	_nop_();
	i = 58UL;
	while (i)
		i--;
}
void delay10x(int i)
{
	while (i-- > 0)
	{
		Delay10us();
	}
}
void showpre(int num)
{
	int i;
	int a[4];
	a[0] = num % 10000 / 1000;
	a[1] = num % 1000 / 100;

	a[2] = num % 100 / 10;
	a[3] = num % 10;

	for (i = 0; i < 4; i++)
	{
		yout_closeall();
		yout_set(i, 0);
		P0 = ~duanzhi[a[i]];
		Delay10us();
		yout_closeall();
		P0 = 0xff;
		Delay10us();
	}
}

void testmain()
{
	int a, b;
	int times = 0;
}



int gsetzhi = 600;







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

#define MAX_REGISTERS 10 // �������Ĵ�������

struct Register
{
	unsigned int num;
	unsigned int value;
};
struct Register registers[MAX_REGISTERS];
int numRegisters = 0;
void showgetzhi()
{
	unsigned char i;
	if (i == 0)
	{
		printf1("Register not get");
	}
	// ��������ӡ���мĴ�����ֵ
	for (i = 0; i < numRegisters; i++)
	{
		printf1("Register %d: %d\n", registers[i].num, registers[i].value);
	}
}

#define MAXgetzhi 100
int getzhi[MAXgetzhi]={0};
void jixi3(char* input)
{
	char *p=input;
	char *p1;
	int i;
	numRegisters=0;
	//1234-2234;333-4;end
	for( i=0;i<100;i++)
	{
		p1=myaddstrstr(p,";"); //����û����һ����
		if(p1==NULL)
		{
			break;
		}
		registers[numRegisters].num = atoi(p);
		p=myaddstrstr(p,"-");
		registers[numRegisters].value = atoi(p);
		numRegisters++;
		p=myaddstrstr(p,";");  //ָ����һ������
	}
	showgetzhi();
}

int flaginit=0;
void setzhione(int dizhi,int zhi)
{
	if(flaginit==0)
	{
		return ;
	}
	push(dizhi,zhi);
}




int setprezhi;

int dianliusettime=0;
char flagdianliuset=0;
void setdianliusettime(int zhi)
{
	if(dianliusettime!=0)
	{
		return ;
	}
	dianliusettime=zhi;
}
void changedainliuzhi()
{
	if(setprezhi!=gsetzhi)
	{
		setprezhi=gsetzhi;
		setdianliusettime(250);
	}
}



char xin[30] = {0};
sbit X0=P1^3;
sbit X1=P1^1;
void shurulvbo(void)
{
	static u8 keybuf[40] = {
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
	}; // ���󰴼�ɨ�軺���� 8ms
	unsigned char i;
	i = X0;
	keybuf[0] = (keybuf[0] << 1) | i;
	i = X1;
	keybuf[1] = (keybuf[1] << 1) | i;
	// i = X2;
	// keybuf[2] = (keybuf[2] << 1) | i;
	// i = X3;
	// keybuf[3] = (keybuf[3] << 1) | i;
	for (i = 0; i < 2; i++) // 3����������ѭ��3��
	{
		if ((keybuf[i] & 0xFF) == 0x00)
		{
			xin[i] = 0;
		}
		else if ((keybuf[i] & 0xFF) == 0xFF)
		{ // ����3��ɨ��ֵΪ1����1*8ms�ڶ��ǵ���״̬ʱ������Ϊ�������ȶ��ĵ���
			xin[i] = 1;
		}
	}
}
char flagsetliangdu=0;
void writebuf();
void keydown(int i) // �������µĴ�������
{
	printf1("keydown %d",i);
	if(i==0)
		setzhione(4,gsetzhi);
	if(i==1)
	{
		// if(flagsetliangdu==0)//��������ֵ�����������
		// {

		// }
		if(flagsetliangdu==1)//�����¼��ֵ
		{
			printf1("flagsetliangdusetzhi %d",gsetzhi);
			writebuf();		
		}
		flagsetliangdu=1-flagsetliangdu;
	}
		

}
void keyup(int i) // �������µĴ�������
{
	printf1("keyup %d",i);
	if(i==0)
		setzhione(4,0);
}
void keyallchuli()
{
	int i;
	static char flag[10] = {0};		// ��־��¼
	static int dowmtimes[10] = {0}; // ��־��¼
	for (i = 0; i < 6; i++)
	{
		if (xin[i] == 0)
		{
			if (flag[i] == 0) // ��������һ�ΰ��¡�����
			{
				flag[i] = 1;
				keydown(i);
			}
		}
		if (xin[i] == 1)
		{
			if (flag[i] == 1) // ������ȥ��������
			{
				keyup(i);
			}
			flag[i] = 0;
			
		}
	}
}
void dealorder();
void getdianliupre()
{

}
int timereport=0;

int errpromdizhi=0x000040;
void writebuf()
{
	u8 get[10];
	get[0]=55;
	get[1]=gsetzhi/100;
	get[2]=gsetzhi%100;
	EEPROM_SectorErase(errpromdizhi);
	EEPROM_write_n(errpromdizhi,get,3);
}

void readbuf()
{
	u8 get[10];

    EEPROM_read_n(errpromdizhi,get,3);
    if(get[0]==55)
    {
        printf("has init\n");
		gsetzhi=get[1]*100+get[2];
		if(gsetzhi>=1023)
		{
			gsetzhi=1023;
		}
		if(gsetzhi<=0)
		{
			gsetzhi=0;
		}
		printf("has init %d-%d\n",get[1],get[2]);
    }
    else
    {
        printf("not init\n");
		get[0]=55;
		gsetzhi=550;
    	EEPROM_write_n(errpromdizhi,get,1);
    }
}
sbit keyled1=P3^6;
sbit keyled2=P3^5;
#define ledon 0
#define ledclose 1
void showled()
{
	keyled1=ledclose;
	keyled2=ledclose;
	keyled2=ledon;
}
void delay_ms(int weizhi)
{

}
void fuwei()
{
	IAP_CONTR=0x60;
}

int com2jixi2(char* input)
{
	char flagTrue=0;
	char *p=input;
	char *p1;
	int i;
	int weizhi;
	int zhi;
	int bakweizhi;
	int bakzhi;
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
                VectorPush(VectorDiannao,weizhi,zhi);
				flagTrue=1;
            }
            else
            {
				flagTrue=0;
                printf("get failed");
            }
        }
		p=myaddstrstr(p,";");  //ָ����һ������
	}
	return flagTrue;
}

int  com2jiexi(char* input)
{
	char par[1000]={0};
	char *begin,*end;
	begin=myaddstrstr(input,"set:");
	end=myaddstrstr(begin,"end");
	if(begin!=NULL && end!=NULL)
	{
		strcpy(par,begin);
		return com2jixi2(par);
	}
	return 0;
}


int flag_canset=0;
int precom2check(char *input)
{
	char *p;
	p=mystrstr(input,"ruibaokesettingmimaflag"); //����û����һ����)
	if(p!=NULL)
	{
		print2("passwd checkpass,you can set now\n");
		print2("\n");
		print2("��ʼ����ֵ 20\n");
		print2("��ʹ������ 21\n");
		print2("������ 22\n");
		print2("������ֵ 23\n");
		print2("ʹ�÷�ʽ set:20-601;20-601;end\n");
		flag_canset=1;
		return 1;
	}
	p=mystrstr(input,"fuwei()"); //����û����һ����)
	if(p!=NULL)
	{
		fuwei();
	}
	return 0;
}

void dealDiannaoOrder()
{

}
void diannaoinputset()
{
	int i;
	if(1==precom2check(rec2))
	{
		return ;
	}
	if(VectorIsEmpty(VectorDiannao))
	{
		print2("oder format error,pleas check\n");
		return ;
	}
	for(i=0;i<10;i++)
	{
		dealDiannaoOrder();	//com�����ĵ����������������
	}
	// ShowInfoToDiannan(1);
}
void diannaocheck()
{
	if(weizhi2==0)
	{
		return ;
	}
	delay_ms(2);
	memset(rec2, 0, sizeof(rec2));
	weizhi2 = 0;
}
void main(void)
{
	SYS_Ini();	  // STC32��ʼ������
	PWM_Config(); // PWM��ʼ������
	EA = 1;		  // ʹ��EA���ж�

	UartInit();
	Uart23Init();
	printf1("system is ok");
	Timer0Init();
	printf1("system is overall");
	
	keyallchuli();
	flaginit=1;
	setzhione(4,0);
	// test3();
	readbuf();
	// û��ʲôʱ��ʱ��Ҫ�󣬣�ָ�㺯�����������ˡ���
	// ���ˣ�֮ǰ��д�������ˡ��������ˡ���ԭ���߼�����ĸľ��аɣ���
	while (1)
	{
		showled();
		keyallchuli();
		showpre(gsetzhi);
		dealorder();//ȡ�����������������з���
		
	}
}

void PWM_ISR() interrupt 26
{
	if (PWMA_SR1 & 0X02) // �������жϱ�־��1
	{
		cnt_H = PWMA_CCR1H; // ��ȡ����ֵ��8λ
		cnt_L = PWMA_CCR1L; // ��ȡ����ֵ��8λ
		PWMA_SR1 &= ~0X02;	// ���㲶���жϱ�־
		EC11_Handle();		// ����EC11����
	}
}
void PWM_Config() // PWM��ʼ������
{
	PWMA_CCER1 = 0x00; // �ر�ͨ��
	PWMA_CCMR1 = 0xA1; // ͨ��ģʽ����Ϊ���룬�ӱ����� , �˲��� 80 ʱ��
	PWMA_CCMR2 = 0xA1; // ͨ��ģʽ����Ϊ���룬�ӱ����� , �˲��� 80 ʱ��
	PWMA_CCER1 = 0x55; // ʹ�ܲ���/�Ƚ�ͨ��1��ͨ��2

	//	PWMA_SMCR = 0x01; 				// ������ģʽ 1
	//	PWMA_SMCR = 0x02; 				// ������ģʽ 2
	PWMA_SMCR = 0x03; // ������ģʽ 3

	PWMA_IER = 0x02;  // ʹ���ж�
	PWMA_CR1 |= 0x01; // ʹ�ܼ�����
	PWMA_PS |= 0x04;  // ѡ�� PWM2_2 ͨ��
}

// ���ر仯�Ĳ���
long calculateChange(unsigned int previous, unsigned int current)
{
	long diff = (current - previous + 65536) % 65536;
	return diff;
}


char flag3 = 0;
int timeleft1, timeleft2, timeleft3, timeleft4;
void chuankou1put(char c)
{
	rec2[weizhi2++] = c;
	if (weizhi2 > sizeof(rec2) - 3)
		weizhi2 = 0;
	// timeleft3 = 3;
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
char get1[100];
int weizhi1=0;
void chuliguankji(char *get1)
{
    char* index;
    index=mystrstr(get1,"@STCISP#");
	if(index==0)
	{
	    return  ;
	}
    IAP_CONTR=0x60;
}

void addgetsetzhi(int i)
{
	int ans;
	ans=gsetzhi+i;
	if(ans>=0 && ans<=1023)
	{
		gsetzhi=ans;
	}
}
static int timepush=0;
void dealorder()
{
	char out[30]={0};
	Alltongxininfo get;
	if(timepush>45)
	{
		timepush=0;
		pop2(&get);
		if(get.weizhi==4)
		{
			get.zhi=get.zhi*bili;
		}
		sprintf(out,"set:%d-%d;end",get.weizhi,get.zhi);
		printf3(out);
		printf(out);
	}
}
void Timer0() interrupt 1
{
	timereport++;
	shurulvbo();
	chuankou1time();
	if(!empty())
	{
		timepush++;
	}
}
void EC11_Handle() // EC11���ݴ�����
{
	static unsigned int previous = 0;
	unsigned int nowzhi;
	long ans;
	nowzhi = cnt_H * 256 + cnt_L; // ��ȡ��ǰ����ֵ
	ans = calculateChange(previous, nowzhi);
	previous = nowzhi;
	if(flagsetliangdu==1)
	{
		if (ans == 4)
		{
			addgetsetzhi(1);
		}
		else
		{
			addgetsetzhi(-1);
		}
		// �趨����ֵ������
		setzhione(4,gsetzhi);
		// setzhione(2,1);//���ơ���
	}
	
}

int step=0;
void showstep(const char *s)
{
	printf1("%s\n",s);
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

void dealchuankou()
{

}

void UARTInterrupt(void) interrupt 4
{
	unsigned char ans;
	if (RI)
	{
		RI = 0;
		ans = SBUF;
		get1[weizhi1++]=ans;
		if(weizhi1>80)
		{
			weizhi1=0;
		}
		chuliguankji(get1);
		chuankou1put(ans);
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
void Uart3() interrupt 17 using 1
{
	char temp3; 
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

// д�����������������Ǹ�����������ǰ�����Ǻ����ˣ��仯���� 15,16,0,1,2...15,16