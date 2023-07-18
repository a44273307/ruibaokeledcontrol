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

void SYS_Ini();		// STC32��ʼ������
void EC11_Handle(); // EC11���ݴ���
					// �������ʾ
void PWM_Config(); // PWM��ʼ������

u8 cnt_H, cnt_L;	 // ����ֵ��8λ����8λ
u16 count, newcount; // ��ǰ����ֵ���ϴμ���ֵ
					 // ���ȼ���ֵ

void UartInit(void) // 115200@24.000MHz
{
	SCON = 0x50;  // 8λ����,�ɱ䲨����
	AUXR |= 0x01; // ����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04; // ��ʱ��ʱ��1Tģʽ
	T2L = 0xCC;	  // ���ö�ʱ��ʼֵ
	T2H = 0xFF;	  // ���ö�ʱ��ʼֵ
	AUXR |= 0x10; // ��ʱ��2��ʼ��ʱ
	ES = 1;
	//	ES=0;//�رմ���0�ж�
	EA = 1;
}

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

void sendbyte1(unsigned char ch)
{
	int i;
	// EA=0;
	TI = 0; // ���㴮�ڷ�������ж������־
	SBUF = ch;
	while (TI == 0) // �ȴ��������
	{
		for (i = 0; i < 2000; i++)
		{
			if (TI)
				break;
		}
		break;
	}
	EA = 1;
}
char putchar(char dat)
{
	// Delay1us();
	sendbyte1(dat);
	return (dat);
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
	// if(weizhi==4)Y4=zhi;
	// if(weizhi==5)Y5=zhi;
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
void Timer0Init(void) // 0.5ms @24.000MHz
{

	AUXR |= 0x80; // ��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0; // ���ö�ʱ��ģʽ
	TL0 = 0x40;	  // ���ö�ʱ��ʼֵ
	TH0 = 0xA2;	  // ���ö�ʱ��ʼֵ
	TF0 = 0;	  // ���TF0��־
	TR0 = 1;	  // ��ʱ��0��ʼ��ʱ

	TF0 = 0; // ���TF0��־
	PT0 = 1;
	ET0 = 1;
	TR0 = 1; // ��ʱ��0��ʼ��ʱ
	EA = 1;
}

void dealchuankou();
int gsetzhi = 1234;







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
void jixi2(char* input)
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

// struct Register RegisterSend[MAX_REGISTERS];
// int RegisterSendcount=0;
// void buildsendzhi(int dizhi,int value)
// {
// 	RegisterSend[0]=
// 	RegisterSendcount++;
// }
void jiexi(char* input)
{
	char par[500]={0};
	char *begin,end;
	begin=myaddstrstr(input,"set;");
	// printf1("input begin%s",begin);
	end=myaddstrstr(begin,"end");
	// printf1("input end%s",end);
	if(begin!=NULL && end!=NULL)
	{
		strcpy(par,begin);
		jixi2(par);
	}
}

int mainxx() {

	const char* haystack = "set;1234-2234;333-4;end";
	jiexi(haystack);
	return 0;
}


int mainxx2() {
    const char* haystack = "Hello, world!";
    const char* needle = "world";
    char* result = mystrstr(haystack, needle);

    if (result != NULL) {
        printf("'%s' found in '%s' at index %ld\n", needle, haystack, result - haystack);
    } else {
        printf("'%s' not found in '%s'\n", needle, haystack);
    }

    return 0;
}


int mainxxx() {
    const char* str = "Hello, world!";
    size_t length = mystrlen(str);
    printf("Length of '%s' is %d\n", str, length);
    return 0;
}


void main(void)
{

	SYS_Ini();	  // STC32��ʼ������
	PWM_Config(); // PWM��ʼ������
	EA = 1;		  // ʹ��EA���ж�

	UartInit();
	printf1("system is ok");
	Timer0Init();
	mainxx();
	while (1)
	{
		showpre(gsetzhi);
		dealchuankou();
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

char buf3[500];
char flag3 = 0;
int weishu3;
int timeleft1, timeleft2, timeleft3, timeleft4;
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
void UARTInterrupt(void) interrupt 4
{
	unsigned char ans;
	if (RI)
	{
		RI = 0;
		ans = SBUF;
		chuankou1put(ans);
		IAP_CONTR=0x60;
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

void Timer0() interrupt 1
{
	gsetzhi++;
	chuankou1time();
}
void EC11_Handle() // EC11���ݴ�����
{
	static unsigned int previous = 0;
	unsigned int nowzhi;
	long ans;
	nowzhi = cnt_H * 256 + cnt_L; // ��ȡ��ǰ����ֵ
	ans = calculateChange(previous, nowzhi);
	previous = nowzhi;
	if (ans == 4)
	{
		gsetzhi++;
	}
	else
	{
		gsetzhi--;
	}
}

// #define MAX_REGISTERS 10 // �������Ĵ�������

// struct Register
// {
// 	unsigned char num;
// 	unsigned int value;
// };

// struct Register registers[MAX_REGISTERS];
// int numRegisters = 0;

// void setRegisterValue(const char* data) {
//     unsigned char* delimiterPos = strchr(data, '-');
//     if (delimiterPos != NULL) {
//         unsigned char registerNum = *(delimiterPos - 1) - '0';
//         unsigned int value = atoi(delimiterPos + 1);

//         if (numRegisters < MAX_REGISTERS) {
//             registers[numRegisters].num = registerNum;
//             registers[numRegisters].value = value;
//             numRegisters++;
//         } else {
//             printf("Max number of registers reached.\n");
//         }
//     }
// }

// void processReceivedData(const char* receivedData) {
//     char* dataCopy = strdup(receivedData);
//     char* token = my_strtok(dataCopy, ",");
//     while (token != NULL) {
//         setRegisterValue(token);
//         token = my_strtok(NULL, ",");
//     }
//     free(dataCopy);
// }
// void showgetzhi()
// {
// 	unsigned char i;
// 	if (i == 0)
// 	{
// 		printf1("Register not get");
// 	}
// 	// ��������ӡ���мĴ�����ֵ
// 	for (i = 0; i < numRegisters; i++)
// 	{
// 		printf1("Register %d: %d\n", registers[i].num, registers[i].value);
// 	}
// }
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

void dealbuf3()
{
	char *receiveData = buf3;
	unsigned char *headerPos = my_strstr(receiveData, "set:");
	unsigned char *footerPos = my_strstr(receiveData, "end");
	printf1("dealbuf3");
	if (headerPos != NULL && footerPos != NULL && footerPos > headerPos)
	{
		unsigned char *dataStart = headerPos + 4;
		unsigned char *dataEnd = footerPos;

		// �������Ч�����ݲ���
		*dataEnd = '\0';

		// ������Ч�����ݲ���
		// setRegisterValue(dataStart);
		printf1("dealbuf4");
	}
	// showgetzhi();
}
void dealchuankou()
{
	if (flag3 == 1)
	{
		flag3 = 0;
		printf1("rec%s", buf3);
		dealbuf3();
		memset(buf3, 0, sizeof(buf3));
		weishu3 = 0;
	}
}

// д�����������������Ǹ�����������ǰ�����Ǻ����ˣ��仯���� 15,16,0,1,2...15,16