// 	@�����ٳ� 2023/03/10
// 	PWM������ģʽʾ������ʾ�����������ģʽ1/2/3��֧�֣�SMCR = 1��ģʽ1��SMCR = 2:ģʽ2��SMCR = 3��ģʽ3
// 	EC11�������������ı����������
//	EC11���Ŷ��壺A��-P1.0��B��-P5.4
//  MAX7219�����ģ�����Ŷ��壺CS = P6^5;DIN = P6^6;CLK = P6^4;������MAX7219.h���޸ģ�
//  ʵ�鿪���壺��������.1 @��Ƶ12MHz

#include "stc32g.h"
#include "config.h"
#include "MAX7219.h"
#include <stdio.h>
#include <stdarg.h>

void SYS_Ini();								// STC32��ʼ������
void EC11_Handle();						// EC11���ݴ���
void SEG_Disp(void);					// �������ʾ
void PWM_Config();						// PWM��ʼ������

u8 cnt_H, cnt_L;							// ����ֵ��8λ����8λ
u16 count,newcount;						// ��ǰ����ֵ���ϴμ���ֵ
u8 number;										// ���ȼ���ֵ
bit numberchange;							// ���ȸı��־

void UartInit(void)		//115200@24.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCC;			//���ö�ʱ��ʼֵ
	T2H = 0xFF;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES=1;
	//	ES=0;//�رմ���0�ж�
	EA=1;
}




void SYS_Ini()								// STC32��ʼ������
{
	EAXFR = 1; 									// ʹ�ܷ��� XFR
	CKCON = 0x00; 							// �����ⲿ���������ٶ�Ϊ���
	WTST = 0x00; 								// ���ó������ȴ���������ֵΪ 0 �ɽ� CPU ִ�г�����ٶ�����Ϊ���
	P0M1 = 0x00;P0M0 = 0x00;		// ����P0��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P1M1 = 0x00;P1M0 = 0x00;		// ����P1��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P2M1 = 0x00;P2M0 = 0x00;		// ����P2��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P3M1 = 0x00;P3M0 = 0x00;		// ����P3��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P4M1 = 0x00;P4M0 = 0x00;		// ����P4��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P5M1 = 0x00;P5M0 = 0x00;		// ����P5��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P6M1 = 0x00;P6M0 = 0x00;		// ����P6��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
	P7M1 = 0x00;P7M0 = 0x00;		// ����P7��Ϊ׼˫���ģʽ //00��׼˫��� 01��������� 10���������� 11����©���
}

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
char putchar (char dat)
{
	// Delay1us();
	sendbyte1(dat);
	return (dat);
}
void UARTInterrupt(void) interrupt 4
{
    unsigned char ans;
    if (RI)
    {
        RI = 0;
        ans = SBUF;
        // chuankou1jisuuan(ans);
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
void Delay100ms()		//@24.000MHz
{
	unsigned long i;

	_nop_();
	_nop_();
	i = 599998UL;
	while (i) i--;
}
// ����printf����
void printf1(const char *fmt, ...) {
	 char *p;
    char buf[128];  // ����һ�����������㹻�洢������ַ���
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);  // ����ʽ�����ַ���д�뻺����
    va_end(args);
    
     p = (unsigned char *)buf;
    while (*p != '\0')
	{
		sendbyte1(*p);
		p++;
	}
}
void testmain()
{
	int a,b;
	int times=0;
   UartInit();
   printf1("test run");
   while (1)
   {
	a=P2^2;
	b=P1^0;
//    printf1("zhi%d-%d-%d",times++,a,b);
	   Delay100ms();
	//    sendbyte1('a');
   }
}
void main(void)
{
	
	SYS_Ini();									// STC32��ʼ������
	PWM_Config();							  // PWM��ʼ������
	EA = 1;											// ʹ��EA���ж�
	MAX7219_Ini();							// MAX7219��ʼ��
	number = 10;								// ��ʼ����10
	SEG_Disp();									// �������ʾ
	testmain();
	while (1)
	{
		if(numberchange == 1)			// ���ı����ȱ�־��1
		{
			Write7219(INTENSITY,number);  // �������������
			SEG_Disp();							// �����ˢ����ʾ
			numberchange = 0;				// ����ı����ȱ�־
		}
	}
}
void PWM_ISR() interrupt 26
{
	if (PWMA_SR1 & 0X02)				// �������жϱ�־��1
	{
		cnt_H = PWMA_CCR1H;				// ��ȡ����ֵ��8λ
		cnt_L = PWMA_CCR1L;				// ��ȡ����ֵ��8λ
		PWMA_SR1 &= ~0X02;				// ���㲶���жϱ�־
		EC11_Handle();							// ����EC11����
	}
}
void PWM_Config()							// PWM��ʼ������
{
	PWMA_CCER1 = 0x00;					// �ر�ͨ��
	PWMA_CCMR1 = 0xA1; 					// ͨ��ģʽ����Ϊ���룬�ӱ����� , �˲��� 80 ʱ��
	PWMA_CCMR2 = 0xA1; 					// ͨ��ģʽ����Ϊ���룬�ӱ����� , �˲��� 80 ʱ��
	PWMA_CCER1 = 0x55;					// ʹ�ܲ���/�Ƚ�ͨ��1��ͨ��2
	
//	PWMA_SMCR = 0x01; 				// ������ģʽ 1
//	PWMA_SMCR = 0x02; 				// ������ģʽ 2	
	PWMA_SMCR = 0x03; 					// ������ģʽ 3
	
	PWMA_IER = 0x02; 						// ʹ���ж�
	PWMA_CR1 |= 0x01; 					// ʹ�ܼ�����
	PWMA_PS |= 0x04; //ѡ�� PWM2_2 ͨ��
}



// ���ر仯�Ĳ���
long  calculateChange(unsigned int previous, unsigned int current) {
  long  diff = (current - previous + 65536) % 65536;
  return diff;
}



void showled()
{

}
void EC11_Handle()						// EC11���ݴ�����
{
	static unsigned int previous=0; 
	unsigned int  nowzhi;
	long ans;
	nowzhi = cnt_H * 256 + cnt_L;	// ��ȡ��ǰ����ֵ
	ans=calculateChange(previous,nowzhi);
	previous=nowzhi;
	if(ans==4)
	{
		printf1("zheng");
	}
	else
	{
		printf1("fan");
	}
	// printf1("newcount %ld",ans);
	// if(newcount < count) 				// ��ǰ����ֵС���ϴμ���ֵ
	// {
	// 	if(number > 0)	number--;	// ���ּ�
	// 	numberchange = 1;					// ���ȸı��־��1
	// 	count = newcount;					// ���¼���ֵ
	// }
	// else if(newcount > count)		// ��ǰ����ֵ�����ϴμ���ֵ
	// {
	// 	if(number < 15) number++;	// ���ּ�
	// 	numberchange = 1;					// ���ȸı��־��1
	// 	count = newcount;					// ���¼���ֵ
	// }
}
void SEG_Disp(void)											
{							
	Write7219(8,15); 						// �����1λϨ��
	Write7219(7,15); 						// �����2λϨ��
	Write7219(6,15); 						// �����3λϨ��
	Write7219(5,15); 						// �����4λϨ��
	Write7219(4,15); 						// �����5λϨ��
	Write7219(3,15); 						// �����6λϨ��
	Write7219(2,(u8)(number / 10)); 	// �����7λ��ʾ����ʮλ
	Write7219(1,(u8)(number % 10)); 	// �����8λ��ʾ���ָ�λ
}

// д�����������������Ǹ�����������ǰ�����Ǻ����ˣ��仯���� 15,16,0,1,2...15,16    