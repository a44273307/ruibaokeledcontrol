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
#include <string.h>
#include "stc32g.h"
#include "config.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>
#include	"Task.h"
#include	"System_init.h"
#include	"APP.h"

#include	"config.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"
#include "uart.h"
#include "tongxin2.h"
#include "tongxin.h"
#include "STC32G_EEPROM.h"

void showhenxiang();
void ledopen(int weizhi);
void pingmuclear2();
int keyon = 0;
int keylow = 1;
int keyok = 2;
int keyup = 3;

int nowzhi = 0;
int g_dianliu = 0;

#define modekey 0
#define mode232 1
#define modeTCP 2
int g_setmode = modekey;

sbit X0 = P4 ^ 0;
sbit X3 = P3 ^ 4;
sbit X2 = P3 ^ 6;
sbit X1 = P4 ^ 1;

sbit LED1 = P4 ^ 2;
sbit LED2 = P3 ^ 7;

sbit LED3 = P3 ^ 5;
sbit LED0 = P3 ^ 3;


sbit Y1 =  P4 ^ 2;
sbit Y2 =  P3 ^ 7;

sbit Y3 =  P3 ^ 5;
sbit Y0 =  P3 ^ 3;



char flagsystemrun = 0;
#define maxsetzhi 1023


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
			get.zhi=get.zhi;
		}
		sprintf(out,"set:%d-%d;end",get.weizhi,get.zhi);
		print3(out);
	}
}
char flagError=0;
int timeflagError=0;
int timetongxin=0;
void time0() interrupt 1
{
	if(flagsystemrun==1)
		timetongxin++;
	if(flagError==1)
	{
		if(timeflagError++>500)
		{
			timeflagError=0;
			LED0 = ~LED0;
		}
		
	}
			
	if(!empty())
	{
		timepush++;
	}
	chuankou1time();
}

void Uart3() interrupt 17
{
	char temp3; 
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI; //??S3RI?
		temp3 = S3BUF;
		chuankou1put(temp3);
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // ���S3TIλ
        busy3 = 0;      // ��æ��־
    }
}

char flag_show = 0;
// �Ƿ��ܹ�����ֵ��״̬������  ok������ ���ɡ�����
char flagKeyCanSet = 0;
void setdianliu(int zhi)
{
	push(4,zhi);
}



void keySetDianliu(int a)
{
	if (flagKeyCanSet == 0)
		return;
	if (g_dianliu + a < 0)
	{
		g_dianliu = 0;
		// 0�����Ҳ������֤�ܹ����յ�����
		flag_show = 1;
		setdianliu(g_dianliu);
		// printf("setdianliu%d\r\n",g_dianliu);
		return;
	}
	if (g_dianliu + a > maxsetzhi)
	{
		g_dianliu = maxsetzhi;
		return;
	}
	g_dianliu = g_dianliu + a;
	flag_show = 1;
	setdianliu(g_dianliu);
}
int writedizhi(int dizhi, int zhi)
{
	push(dizhi,zhi);
}
void writebuf();
#define showmodeset 1
#define showkeyset 0
// ��ʼ�İ�������״̬������
int g_showset=showkeyset;
// ������ʾ�α괦��ġ���������mode���õĽ���
int g_showyoubiao=0;
void g_showyoubiao_set(int zhi)
{
	int i;
	if(g_showyoubiao<0 && zhi>0)
	{
		g_showyoubiao=0;	
	}
	g_showyoubiao+=zhi;
	if(g_showyoubiao<=0)
	g_showyoubiao=0;
	if(g_showyoubiao>=3)
	g_showyoubiao=3;
	printf("g_showyoubiao-%d",g_showyoubiao);
	for(i=0;i<4;i++)
	LCD_ShowString(160, i*40,"      ", WHITE, BLACK, 32, 0);

	LCD_ShowString(160, g_showyoubiao*40,"<---  ", WHITE, BLACK, 32, 0);
}

void keylowdeal()
{
	if(g_showset==showmodeset)
	{
		g_showyoubiao_set(-1);
	}
	else
	{
		keySetDianliu(-1);
	}
	
}
void keyupdeal()
{
	if(g_showset==showmodeset)
	{
		g_showyoubiao_set(1);
	}
	else
	{
		keySetDianliu(1);
	}
	
}
void modeshowandsend();
void showbak();
void keyokdeal()
{
	// mode �������ã�232�ȡ�����
	if(g_showset==showmodeset)
	{
		// ����������ֵ������
		if(g_showyoubiao<=2)
		{
			g_setmode=g_showyoubiao;
			writebuf();
		}
		if(g_showyoubiao>=0)
		{
			showbak();
			modeshowandsend();
			g_showset=showkeyset;
		}
	}
	else
	{
		if(g_setmode!=modekey)
		{
			return;
		}
		if(flagKeyCanSet==1)
		{
			writebuf();
		}
		flagKeyCanSet = 1 - flagKeyCanSet;
		// LED2 = ~LED2;
	}

}
void flagKeyDeal()
{
	if(g_setmode!=modekey)
	{
		flagKeyCanSet=0;
	}
	if(flagKeyCanSet==1)
	{
		LED2=0;
	}
	else
	{
		LED2=1;
	}

}

void keydown(int i) // �������µĴ�������
{
	printf("key down%d",i);
	if (i == keyon)
	{
		LED0 = ~LED0;
		// ����ʱ��صƣ� �������̿��ơ�����
		if (flagsystemrun == 0)
		{
			flagsystemrun = 1;
			LCD_ShowString(0, 0, "Circle TAC", WHITE, BLACK, 32, 0);
			
			showhenxiang();
			writedizhi(2,1);
			delay_ms(30);
			writedizhi(2,1);
			delay_ms(30);
			setdianliu(g_dianliu);
			delay_ms(10);
		}
		else
		{
			writedizhi(2,0);
			delay_ms(100);
			dealorder();
			writedizhi(2,0);
			delay_ms(300);
			dealorder();
			IAP_CONTR = 0x60;
		}
	}
	if (flagKeyCanSet)
	{
		ledopen(i);
	}
	if (i == keylow)
	{
		keylowdeal();
		return;
	}
	if (i == keyup)
	{
		keyupdeal();
		return;
	}
	if (i == keyok)
	{
		keyokdeal();
	}
}
int keyshi = 3;
// �����������¶��ٴεĲ�������
int setbizhi(int times)
{
	if (times < 15 * keyshi)
	{
		return 10;
	}
	if (times < 50 * keyshi)
	{
		return 50;
	}
	return 100;
}
void modeshowandsend()
{
	if(g_setmode==modekey)
		LCD_ShowString(200, 0, "KEY  ", WHITE, BLACK, 32, 0);
	if(g_setmode==mode232)
		LCD_ShowString(200, 0, "232  ", WHITE, BLACK, 32, 0);
	if(g_setmode==modeTCP)
		LCD_ShowString(200, 0, "TCP  ", WHITE, BLACK, 32, 0);
	push(101,g_setmode);
}
void pingmuclear();
void lcdinitshowmode()
{
	pingmuclear2();
    LCD_ShowString(0, 0,"KEY", WHITE, BLACK, 32, 0);
    LCD_ShowString(0, 40,"232", WHITE, BLACK, 32, 0);
    LCD_ShowString(0, 80,"TCP", WHITE, BLACK, 32, 0);
	LCD_ShowString(0, 120,"ESC", WHITE, BLACK, 32, 0);
	LCD_ShowString(160, 0, "<---  ", WHITE, BLACK, 32, 0);
	g_showyoubiao=-1;
}
void showhenxiangbase();
void showbak()
{
	pingmuclear2();
	LCD_ShowString(0, 0, "Circle TAC", WHITE, BLACK, 32, 0);
	showhenxiangbase();
	g_showyoubiao=0;
}

void chulikeyok(int times)
{
	if(times > 10000)
	{
		if(g_showset==showkeyset)
		{
			g_showset=showmodeset;
			printf("chulikeyok over");
			lcdinitshowmode();
		}
	}
}
// 2ms һ�εĻ�����300msһ��ok�İɡ���
void dolongtimes(int i, int times)
{
	int xielv;
	times = times - 150;
	
	if (times < 0)
	{
		return;
	}
	// printf("times %d",times);
	if(i== keyok)
	{
		chulikeyok(times);
		return;
	}
	xielv = setbizhi(times);
	if (times % keyshi != 0)
	{
		return;
	}
	if(g_showset==showmodeset)
	{
		return ;
	}
	if (i == keylow)
	{
		keySetDianliu(-xielv);
		return;
	}
	if (i == keyup)
	{
		keySetDianliu(xielv);
		return;
	}
}
void yout_set(char weizhi, char zhi)
{
	if (weizhi == 0)
		Y0 = zhi;
	if (weizhi == 1)
		Y1 = zhi;
	if (weizhi == 2)
		Y2 = zhi;
	if (weizhi == 3)
		Y3 = zhi;
}
void ledclose(int weizhi)
{
	if (weizhi == keyon || weizhi == keyok)
	{
		return;
	}
	yout_set(weizhi, 1);
}
void ledopen(int weizhi)
{
	if (weizhi == keyon || weizhi == keyok)
	{
		return;
	}
	yout_set(weizhi, 0);
}





char xin[30] = {0};
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
	i = X2;
	keybuf[2] = (keybuf[2] << 1) | i;
	i = X3;
	keybuf[3] = (keybuf[3] << 1) | i;
	for (i = 0; i < 10; i++) // 3����������ѭ��3��
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

			dowmtimes[i]++;
			dolongtimes(i, dowmtimes[i]);
		}
		else
		{
			flag[i] = 0;
			dowmtimes[i] = 0;
			ledclose(i);
		}
	}
}

int tmp = 203; // �¶�ֵ
void shownwendu()
{
	char dataxx[40];
	// �ǵø�λ
	if (flagsystemrun == 0 || g_showset!= showkeyset)
	{
		return;
	}
	sprintf(dataxx, "TMP:%3d.%01d  ", tmp / 10, tmp % 10);
	LCD_ShowString(0, 80, dataxx, WHITE, BLACK, 32, 0);
}

void shownow()
{
	// char dataxx[40];
	// sprintf(dataxx,"NOW:%05d",nowzhi);
	// LCD_ShowString(0,40,dataxx,WHITE,BLACK,32,0);
}

#define maxjindu 16
#define qidian 07
void pingmuclear2()
{
	char dataxx[60] = {0};

	int i;
	// LCD_Clear(BLACK);

	sprintf(dataxx, "                     ", 1);
	for (i = 0; i < 10; i++)
	{
		LCD_ShowString(0, i * 30, dataxx, WHITE, BLACK, 32, 0);
		// delay_ms(1);
	}
}
void pingmuclear()
{
	static int runflag2 = 0;
	if (runflag2 == 1)
	{
		return;
	}
	runflag2 = 1;
	pingmuclear2();
}

void showhenxiangbase()
{
	int i;
	char dataxx[60] = {0};
	for (i = 0; i < maxjindu; i++)
	{
		dataxx[i] = '-';
	}
	LCD_ShowString(qidian, 140, dataxx, WHITE, BLACK, 32, 0);
	LCD_ShowString(qidian, 140 + 40, dataxx, WHITE, BLACK, 32, 0);
	LCD_ShowString(qidian, 140 + 40 + 30, "0            12bit          1023", WHITE, BLACK, 16, 0);
}
void showhenxiang()
{
	int i;
	char dataxx[60] = {0};
	static int runflag = 0;
	if (runflag == 1)
	{
		return;
	}
	runflag = 1;

	for (i = 0; i < maxjindu; i++)
	{
		dataxx[i] = '-';
	}
	LCD_ShowString(qidian, 140, dataxx, WHITE, BLACK, 32, 0);
	LCD_ShowString(qidian, 140 + 40, dataxx, WHITE, BLACK, 32, 0);
	LCD_ShowString(qidian, 140 + 40 + 30, "0            12bit          1023", WHITE, BLACK, 16, 0);
}

void showjindtiao()
{
	int i;
	char dataxx[60] = {0};
	int jindu;
	jindu = g_dianliu * maxjindu / maxsetzhi;
	for (i = 0; i < maxjindu; i++)
	{
		if (i < jindu)
			dataxx[i] = '>';
		else
			dataxx[i] = ' ';
	}
	showhenxiang();
	LCD_ShowString(qidian, 160, dataxx, WHITE, BLACK, 32, 0);
}
void showsetzhi()
{
	char dataxx[40];
	sprintf(dataxx, "SET:        %04d    ", g_dianliu);
	LCD_ShowString(0, 120, dataxx, WHITE, BLACK, 32, 0);
	showjindtiao();
}
void showdata()
{
	// �ǵø�λ
	if (flagsystemrun == 0 || g_showset!= showkeyset)
	{
		return;
	}
	shownow();
	showsetzhi();
}

void showfen(int error)
{
	// �ǵø�λ
	if (flagsystemrun == 0 || g_showset!= showkeyset)
	{
		return;
	}
	if(error==11)
    LCD_ShowString(0, 40,"ERROR FOR FAN", WHITE, BLACK, 32, 0);
	if(error==12)
    LCD_ShowString(0, 40,"ERROR FOR TMP", WHITE, BLACK, 32, 0);
	if(error==13)
    LCD_ShowString(0, 40,"ERROR FOR REC CENTER", WHITE, BLACK, 32, 0);
	if(error==14)
    LCD_ShowString(0, 40,"ERROR FOR REC LIGHT", WHITE, BLACK, 32, 0);
	if(error>=11 && error<=14)
	{
		flagError=1;
	}
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
	timetongxin=0;
	if( weizhi== 6 )
	{
		tmp=zhi;
	}
	if( weizhi== 32 )//�������ù����ĵ�������
	{
		g_dianliu=zhi;
		flag_show=1;
		writebuf();
	}
	showfen(weizhi);
}
void readbuf();

void mainrun()
{
	int rumtimes = 0;
	LED0 = ~LED0;
	readbuf();

	writedizhi(4,0);
	modeshowandsend();
	
	
	while (1)
	{
		delay_ms2(1);
		flagKeyDeal();
		shurulvbo();
		keyallchuli(); 
		dealorder();
		dealchuankou();//�����пذ�����ĵ�����
		getzhiandchange();//�����ݽ��д�������
		if (flag_show == 1)
		{
			flag_show = 0;
			showdata();
			rumtimes = 0;
		}
		if(flagsystemrun==1)
		{
			if(timetongxin>6000)
			{
				timetongxin=0;
				push2(13,1);
			}
		}
		
		rumtimes++;
		if (rumtimes == 15000)
			showdata();
		if (rumtimes == 20000)
		{
			shownwendu();
			rumtimes = 0;
		}
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
	TL1 = 0xB8;	  // ���ö�ʱ��ʼֵ
	TH1 = 0xFF;	  // ���ö�ʱ��ʼֵ
	ET1 = 0;	  // ��ֹ��ʱ��%d�ж�
	TR1 = 1;	  // ��ʱ��1��ʼ��ʱ
	ES = 1;
	EA = 1;
	P_SW1 = 0x00; // RXD/P3.0, TXD/P3.1

}
int errpromdizhi=0x000040;
#define u8 unsigned char
void writebuf()
{
	u8 get[10];
	get[0]=55;
	get[1]=g_dianliu/100;
	get[2]=g_dianliu%100;
	get[3]=g_setmode%100;
	EEPROM_SectorErase(errpromdizhi);
	EEPROM_write_n(errpromdizhi,get,4);
}

void readbuf()
{
	u8 get[10]={0};
    EEPROM_read_n(errpromdizhi,get,4);
    if(get[0]==55)
    {
        printf("has init\n");
		g_dianliu=get[1]*100+get[2];
		if(g_dianliu>=1023)
		{
			g_dianliu=1023;
		}
		if(g_dianliu<=0)
		{
			g_dianliu=0;
		}
		g_setmode=get[3];
		if(g_setmode<0 || g_setmode>=3)
		{
			g_setmode=0;
		}
		printf("has init %d-%d-%d\n",get[1],get[2],get[3]);
    }
    else
    {
        printf("not init\n");
		get[0]=55;
		g_dianliu=550;
    	EEPROM_write_n(errpromdizhi,get,1);
    }
}
void main(void)
{
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�
	
	XOSCCR = 0xc0;        //�����ⲿ����
	while(!(XOSCCR&1));   //�ȴ�ʱ���ȶ�
	CLKDIV = 0x00;        //ʱ�Ӳ���Ƶ
	CLKSEL = 0x0;        //ѡ���ⲿʱ��		
	GPIO_confibase();
	SYS_Init();
	test2();
	GPIO_configcom3();
	Uart23Init();
	P4_MODE_IO_PU(GPIO_Pin_0|GPIO_Pin_1);
	P3_MODE_IO_PU(GPIO_Pin_6|GPIO_Pin_4);
	Timer0Init();
	delay_ms(50);

	LCD_Init();
	delay_ms(50);
	LCD_Fill(0, 0, 320, 240, BLACK);
	delay_ms(50);
	pingmuclear();
	delay_ms(50);
	mainrun();
}
