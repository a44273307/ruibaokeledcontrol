#include <string.h>
#include "stc32g.h"
#include "config.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>


#include "lcd.h"
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tongxin.h"
#include "tongxin2.h"
#include "uart.h"

#define u8 unsigned char
#define FOSC 24000000UL
#define BRT (65536 - FOSC / 9600 / 4)
int writedizhi(int dizhi, int zhi);
int keyon = 0;
int keylow = 1;
int keyok = 2;
int keyup = 3;

int nowzhi = 0;
int setzhi = 0;

char flagsystemrun = 0;
#define maxsetzhi 2047


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

void showhenxiang();
void ledopen(int weizhi);




sbit X0 = P2 ^ 3;
sbit X1 = P2 ^ 1;
sbit X2 = P3 ^ 7;
sbit X3 = P3 ^ 5;

sbit LED3 = P3 ^ 4;
sbit LED2 = P3 ^ 6;

sbit LED1 = P2 ^ 0;
sbit LED0 = P2 ^ 2;
sbit Y3 = P3 ^ 4;
sbit Y2 = P3 ^ 6;

sbit Y1 = P2 ^ 0;
sbit Y0 = P2 ^ 2;

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
	}; // 矩阵按键扫描缓冲区 8ms
	unsigned char i;
	i = X0;
	keybuf[0] = (keybuf[0] << 1) | i;
	i = X1;
	keybuf[1] = (keybuf[1] << 1) | i;
	i = X2;
	keybuf[2] = (keybuf[2] << 1) | i;
	i = X3;
	keybuf[3] = (keybuf[3] << 1) | i;
	for (i = 0; i < 10; i++) // 3按键，所以循环3次
	{
		if ((keybuf[i] & 0xFF) == 0x00)
		{
			xin[i] = 0;
		}
		else if ((keybuf[i] & 0xFF) == 0xFF)
		{ // 连续3次扫描值为1，即1*8ms内都是弹起状态时，可认为按键已稳定的弹起
			xin[i] = 1;
		}
	}
}
char flagsetzhichange = 0;
// 是否能够设置值的状态。。。  ok键按下 即可。。。
char flagcanset = 0;
void setdianliu(int zhi)
{
	push(4,zhi);
}



void setzhichange(int a)
{
	if (flagcanset == 0)
		return;
	if (setzhi + a < 0)
	{
		setzhi = 0;
		// 0的情况也发，保证能够被收到。。
		flagsetzhichange = 1;
		setdianliu(setzhi);
		// printf("setdianliu%d\r\n",setzhi);
		return;
	}
	if (setzhi + a > maxsetzhi)
	{
		setzhi = maxsetzhi;
		return;
	}
	setzhi = setzhi + a;
	flagsetzhichange = 1;
	setdianliu(setzhi);
}

void keydown(int i) // 按键按下的处理、、、
{
	if (i == keyon)
	{
		LED0 = ~LED0;
		// 结束时候关灯， 开机键盘开灯。。。
		if (flagsystemrun == 0)
		{
			flagsystemrun = 1;
			LCD_ShowString(0, 0, "Circle TAC", RED, WHITE, 32, 0);
			showhenxiang();
			writedizhi(2,1);
			delay_ms(30);
			writedizhi(2,1);
			delay_ms(30);
			setdianliu(setzhi);
			delay_ms(10);
		}
		else
		{
			writedizhi(2,0);
			delay_ms(100);
			writedizhi(2,0);
			IAP_CONTR = 0x60;
		}
	}
	if (flagcanset)
	{
		ledopen(i);
	}
	if (i == keylow)
	{
		setzhichange(-1);
		return;
	}
	if (i == keyup)
	{
		setzhichange(1);
		return;
	}
	if (i == keyok)
	{
		flagcanset = 1 - flagcanset;
		LED2 = ~LED2;
	}
}
int keyshi = 3;
// 按键连续按下多少次的操作。。
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
// 2ms 一次的话，那300ms一次ok的吧。。
void dolongtimes(int i, int times)
{
	int xielv;
	times = times - 300;
	if (times < 0)
	{
		return;
	}
	xielv = setbizhi(times);
	if (times % keyshi != 0)
	{
		return;
	}
	if (i == keylow)
	{
		setzhichange(-xielv);
		return;
	}
	if (i == keyup)
	{
		setzhichange(xielv);
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
void keyallchuli()
{
	int i;
	static char flag[10] = {0};		// 标志记录
	static int dowmtimes[10] = {0}; // 标志记录
	for (i = 0; i < 6; i++)
	{
		if (xin[i] == 0)
		{
			if (flag[i] == 0) // 代表按键第一次按下。。。
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

int tmp = 203; // 温度值
void shownwendu()
{
	char dataxx[40];
	sprintf(dataxx, "TMP:%3d.%01d  ", tmp / 10, tmp % 10);
	LCD_ShowString(0, 80, dataxx, RED, WHITE, 32, 0);
}

void shownow()
{
	// char dataxx[40];
	// sprintf(dataxx,"NOW:%05d",nowzhi);
	// LCD_ShowString(0,40,dataxx,RED,WHITE,32,0);
}

#define maxjindu 16
#define qidian 0

void pingmuclear()
{
	char dataxx[60] = {0};
	static int runflag2 = 0;
	int i;
	if (runflag2 == 1)
	{
		return;
	}
	runflag2 = 1;
	// LCD_Clear(WHITE);

	sprintf(dataxx, "                     ", 1);
	for (i = 0; i < 10; i++)
	{
		LCD_ShowString(0, i * 30, dataxx, RED, WHITE, 32, 0);
		delay_ms(2);
	}
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

	LCD_ShowString(qidian, 140, dataxx, RED, WHITE, 32, 0);
	LCD_ShowString(qidian, 140 + 40, dataxx, RED, WHITE, 32, 0);
	LCD_ShowString(qidian, 140 + 40 + 30, "0            12bit          2047", RED, WHITE, 16, 0);
}

void showjindtiao()
{
	int i;
	char dataxx[60] = {0};
	int jindu;
	jindu = setzhi * maxjindu / maxsetzhi;
	for (i = 0; i < maxjindu; i++)
	{
		if (i < jindu)
			dataxx[i] = '>';
		else
			dataxx[i] = ' ';
	}
	showhenxiang();
	LCD_ShowString(qidian, 160, dataxx, RED, WHITE, 32, 0);
}
void showsetzhi()
{
	char dataxx[40];
	sprintf(dataxx, "SET:        %04d    ", setzhi);
	LCD_ShowString(0, 120, dataxx, RED, WHITE, 32, 0);
	showjindtiao();
}
void showdata()
{
	// 记得复位
	// if (flagsystemrun == 0)
	// {
	// 	return;
	// }
	shownow();
	showsetzhi();
	shownwendu();
}
int getwendu();
void init();
int readzhi(int dizhi);
int readdianliuzhi()
{
	int ans;
	ans= readzhi(4);
	if(ans>=0 && ans<=2047)
	{
		return ans;
	}
	return 0;
}
void dealorder();
void main()
{
	int rumtimes = 0;
	init();
	// setzhi = readdianliuzhi();
	while (1)
	{
		shurulvbo();
		keyallchuli();
		delay_ms(1);
		dealorder();
		// if (flagsetzhichange == 1)
		// {
		// 	flagsetzhichange = 0;
		// 	showdata();
		// 	rumtimes = 0;
		// }
		// else
		// {
		// 	rumtimes++;
		// }
		// if (rumtimes++ > 500)
		// {
		// 	rumtimes = 0;
		// 	// tmp = getwendu();
		// 	showdata();
		// }
	}
}
int weishu1, weishu2, weishu3, weishu4;
char buf1[300];
char buf3[300];

int timeleft1, timeleft2, timeleft3, timeleft4;
char falgchuankou1 = 0;
static void chuliguankji(char *ans1)
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
void clearbuff1()
{
	memset(buf1, 0, sizeof(buf1));
	weishu1 = 0;
	falgchuankou1 = 0;
}

int readzhi(int dizhi)
{
	int times = 0;
	int ans;
	char dataxx[40];
	sprintf(dataxx, "pingmuGetData getdizhi%d-", dizhi);
	clearbuff1();
	printf(dataxx);
	// while (1)
	// {
	// 	// delay_ms(1);
	// 	// if (times++ > 100)
	// 	// {
	// 	// 	return 0;
	// 	// }
	// 	// if (falgchuankou1)
	// 	// {
	// 	// 	char *index;
	// 	// 	index = strstr(buf1, dataxx);
	// 	// 	if (index != 0)
	// 	// 	{
	// 	// 		ans = atoi(index + strlen(dataxx));
	// 	// 		printf("read ans%d\n", ans);
	// 	// 		return ans;
	// 	// 	}
	// 	// 	clearbuff1();
	// 	// }
	// }
}
// 上到下  灰白黑紫
int writedizhi(int dizhi, int zhi)
{
	int times = 0;
	int ans;
	char dataxx[40];
	sprintf(dataxx, "pingmuSetData setdizhi0%d-%d", dizhi, zhi);
	clearbuff1();
	printf(dataxx);
	// while (1)
	// {
	// 	// delay_ms(1);
	// 	// if (times++ > 100)
	// 	// {
	// 	// 	return 0;
	// 	// }
	// 	// if (falgchuankou1)
	// 	// {
	// 	// 	char *index;
	// 	// 	index = strstr(buf1, "pingmuSetData zhi");
	// 	// 	if (index != 0)
	// 	// 	{
	// 	// 		return 0;
	// 	// 	}
	// 	// 	clearbuff1();
	// 	// }
	// }
}


// 采用中断和处理分开的方式吧，方便调试。。
int getwendu()
{
	return readzhi(6);
}
void UartIsr() interrupt 4
{
	if (RI)
	{
		RI = 0;
				buf1[weishu1++] =SBUF;
	if (weishu1 >= sizeof(buf1))
		weishu1 = 0;
	chuliguankji(buf1);
	}
} 
static int timepush=0;
// 定义printf函数
void printf3(const char *fmt, ...)
{
	char *p;
	char buf[128]; // 定义一个缓冲区，足够存储输出的字符串
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args); // 将格式化的字符串写入缓冲区
	va_end(args);

	p = (unsigned char *)buf;
	while (*p != '\0')
	{
		sendbyte3(*p);
		p++;
	}
}
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
		printf3(out);
	}
}
char flag3 = 0;
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
		if (timeleft3 == 0) // 数据一次收完了.
		{
			flag3 = 1;
		}
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
// 分离，发命令，20发读的命令，返回的值，默认是电流值。。。
void jixi2(char* input)
{
	char *p=input;
	char *p1;
	int i;
	unsigned int weizhi;
	unsigned int zhi;
	//1234-2234;333-4;end
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
		p=myaddstrstr(p,";");  //指向下一个后面
		printf("get set%d-%d",weizhi,zhi);
		push2(weizhi,zhi);
		
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
void tm0_isr() interrupt 1
{
	chuankou1time();
	if(!empty())
	{
		timepush++;
	}
}


void Uart3() interrupt 17 using 1
{
	char temp3; 
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI; //??S3RI?
		temp3 = S3BUF;
		chuankou1put(temp3);
        // chuankou3put(temp3);
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // 清除S3TI位
        busy3 = 0;      // 清忙标志
    }
}
void init()
{
	WTST = 0;	
	EAXFR = 1;	  // 使能访问 XFR
	CKCON = 0x00; // 设置外部数据总线速度为最快
	// delay_ms(10);
	P0M0 = 0x00;
	P0M1 = 0x02;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	LCD_LED = 0;
	LED0 = ~LED0;
	UartInit();
	Uart23Init();
	Timer0Init();
	delay_ms(10);
	LCD_Init();
	delay_ms(10);
	printf("system begin\n");
	// 屏幕相关的，记得复位。。。
	// LCD_Fill(0, 0, 320, 240, WHITE);
	// printf("system begin\r");
	// delay_ms(10);
	// pingmuclear();
	// printf("system begin over\r");
	// LCD_Fill(0, 0, 320, 240, WHITE);
	
}
