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

#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"
#include "uart.h"

void	GPIO_configcom3(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
	GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//��ʼ��
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_configcom3(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer3;			//ѡ�����ʷ�����, BRT_Timer3, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	UART_Configuration(UART3, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART3_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART3_SW(UART3_SW_P50_P51);		//UART3_SW_P00_P01,UART3_SW_P50_P51
}


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

void	GPIO_confibase(void)
{
//	P2_MODE_IO_PU( GPIO_Pin_3 | GPIO_Pin_4 );		//P2 ����Ϊ׼˫���
//	P2_MODE_OUT_PP(GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
    P2_MODE_IO_PU(GPIO_Pin_All);	
	
	P3_MODE_OUT_PP(GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7);
	P3_MODE_IN_HIZ(GPIO_Pin_4|GPIO_Pin_6);
	
	P4_MODE_OUT_PP(GPIO_Pin_2|GPIO_Pin_3);
	P4_MODE_IN_HIZ(GPIO_Pin_0|GPIO_Pin_1);
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
	GPIO_confibase();
	SYS_Init();

	test2();
	sendbytecom1('c');
	GPIO_configcom3();
	sendbytecom1('d');
	Uart23Init();
	sendbytecom1('e');

	while (1)
	{
		
		sendbytecom1('a');
		print3("test3");
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
		printf("rec init now");
		IAP_CONTR = 0x60;
	}

	if(TI)
	{
		TI = 0;
		
      
	}
}
// ��Դ���ӣ�����û��

// lcd���ӣ��������£�������ת����ֵ����������ok�ģ����ٰ��£����档��



void Uart3() interrupt 17
{
	char temp3; 
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI; //??S3RI?
		temp3 = S3BUF;
		// chuankou1put(temp3);
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // ���S3TIλ
        busy3 = 0;      // ��æ��־
    }
}