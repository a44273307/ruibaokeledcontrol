/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"Task.h"
#include	"System_init.h"
#include	"APP.h"
//========================================================================
// ����: void	main(void)
// ����: ����������.
// ����: None.
// ����: None.
// �汾: V1.0, 2012-10-22
//========================================================================
void test2()
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
void Delay1ms()		//@11.0592MHz
{
	unsigned long i;

	_nop_();
	_nop_();
	_nop_();
	i = 5998UL;
	while (i) i--;
}


void delayx_ms(unsigned int ms)
{
	while(ms--)
	{
		Delay1ms();
	}
}
extern void Sample_Lamp(void);
void sendbytecom1(unsigned char ch)
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
#define maxjindu 16
#define qidian 0
void showhenxiang()
{
	int i;
	char dataxx[60] = {0};
	static int runflag = 0;


	for (i = 0; i < maxjindu; i++)
	{
		dataxx[i] = '-';
	}

	LCD_ShowString(qidian, 140, dataxx, RED, WHITE, 32, 0);
	LCD_ShowString(qidian, 140 + 40, dataxx, RED, WHITE, 32, 0);
	LCD_ShowString(qidian, 140 + 40 + 30, "0            12bit          2047", RED, WHITE, 16, 0);
}


void main(void)
{
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�
	
	XOSCCR = 0xc0;        //�����ⲿ����
	while(!(XOSCCR&1));   //�ȴ�ʱ���ȶ�
	CLKDIV = 0x00;        //ʱ�Ӳ���Ƶ
	CLKSEL = 0x01;        //ѡ���ⲿʱ��		

	SYS_Init();
	test2();
		sendbytecom1('c');
	while (1)
	{
		sendbytecom1('a');
		delayx_ms(100);
		sendbytecom1('b');
		Sample_Lamp();
		showhenxiang();
	}
}

void UART1_ISR_Handler (void) interrupt UART1_VECTOR
{


	if(RI)
	{
		RI = 0;
		IAP_CONTR = 0x60;
	}

	if(TI)
	{
		TI = 0;
		
      
	}
}
// ��Դ���ӣ�����û��

// lcd���ӣ��������£�������ת����ֵ����������ok�ģ����ٰ��£����档��



