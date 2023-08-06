// 	@布丁橘长 2023/03/10
// 	PWM编码器模式示例，本示例程序编码器模式1/2/3均支持，SMCR = 1：模式1，SMCR = 2:模式2，SMCR = 3：模式3
// 	EC11左旋或右旋，改变数码管亮度
//	EC11引脚定义：A相-P1.0，B相-P5.4
//  MAX7219数码管模块引脚定义：CS = P6^5;DIN = P6^6;CLK = P6^4;（可在MAX7219.h中修改）
//  实验开发板：屠龙刀三.1 @主频12MHz








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

void SYS_Ini();		// STC32初始化设置
void EC11_Handle(); // EC11数据处理
					// 数码管显示
void PWM_Config(); // PWM初始化设置

u8 cnt_H, cnt_L;	 // 计数值高8位、低8位
u16 count, newcount; // 当前计数值、上次计数值
					 // 亮度计数值



char rec2[500];
int weizhi2=0;


// 最后下去到板子的要乘以2，，最后的计算值。。

int bili=2;
void SYS_Ini() // STC32初始化设置
{
	EAXFR = 1;	  // 使能访问 XFR
	CKCON = 0x00; // 设置外部数据总线速度为最快
	WTST = 0x00;  // 设置程序代码等待参数，赋值为 0 可将 CPU 执行程序的速度设置为最快
	P0M1 = 0x00;
	P0M0 = 0x00; // 设置P0口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P1M1 = 0x00;
	P1M0 = 0x00; // 设置P1口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P2M1 = 0x00;
	P2M0 = 0x00; // 设置P2口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P3M1 = 0x00;
	P3M0 = 0x00; // 设置P3口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P4M1 = 0x00;
	P4M0 = 0x00; // 设置P4口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P5M1 = 0x00;
	P5M0 = 0x00; // 设置P5口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P6M1 = 0x00;
	P6M0 = 0x00; // 设置P6口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P7M1 = 0x00;
	P7M0 = 0x00; // 设置P7口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
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
// 定义printf函数
void printf1(const char *fmt, ...)
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
		sendbyte1(*p);
		p++;
	}
}

// 定义printf函数
void print2(const char *fmt, ...)
{
	char *p;
	char buf[400]={0}; // 定义一个缓冲区，足够存储输出的字符串
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args); // 将格式化的字符串写入缓冲区
	va_end(args);
	p = (unsigned char *)buf;
	while (*p != '\0')
	{
		sendbyte1(*p);
		p++;
	}
}


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

#define MAX_REGISTERS 10 // 设置最大寄存器数量

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
	// 遍历并打印所有寄存器的值
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
		p1=myaddstrstr(p,";"); //找有没有下一个的
		if(p1==NULL)
		{
			break;
		}
		registers[numRegisters].num = atoi(p);
		p=myaddstrstr(p,"-");
		registers[numRegisters].value = atoi(p);
		numRegisters++;
		p=myaddstrstr(p,";");  //指向下一个后面
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
	}; // 矩阵按键扫描缓冲区 8ms
	unsigned char i;
	i = X0;
	keybuf[0] = (keybuf[0] << 1) | i;
	i = X1;
	keybuf[1] = (keybuf[1] << 1) | i;
	// i = X2;
	// keybuf[2] = (keybuf[2] << 1) | i;
	// i = X3;
	// keybuf[3] = (keybuf[3] << 1) | i;
	for (i = 0; i < 2; i++) // 3按键，所以循环3次
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
char flagsetliangdu=0;
void writebuf();
void keydown(int i) // 按键按下的处理、、、
{
	printf1("keydown %d",i);
	if(i==0)
		setzhione(4,gsetzhi);
	if(i==1)
	{
		// if(flagsetliangdu==0)//不能设置值。。变成设置
		// {

		// }
		if(flagsetliangdu==1)//保存记录的值
		{
			printf1("flagsetliangdusetzhi %d",gsetzhi);
			writebuf();		
		}
		flagsetliangdu=1-flagsetliangdu;
	}
		

}
void keyup(int i) // 按键按下的处理、、、
{
	printf1("keyup %d",i);
	if(i==0)
		setzhione(4,0);
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
		}
		if (xin[i] == 1)
		{
			if (flag[i] == 1) // 代表下去了又上来
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
		p1=myaddstrstr(p,";"); //找有没有下一个的
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
		p=myaddstrstr(p,";");  //指向下一个后面
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
	p=mystrstr(input,"ruibaokesettingmimaflag"); //找有没有下一个的)
	if(p!=NULL)
	{
		print2("passwd checkpass,you can set now\n");
		print2("\n");
		print2("初始电流值 20\n");
		print2("已使用寿命 21\n");
		print2("总寿命 22\n");
		print2("最大电流值 23\n");
		print2("使用方式 set:20-601;20-601;end\n");
		flag_canset=1;
		return 1;
	}
	p=mystrstr(input,"fuwei()"); //找有没有下一个的)
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
		dealDiannaoOrder();	//com过来的电脑命令解析。。。
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
	SYS_Ini();	  // STC32初始化设置
	PWM_Config(); // PWM初始化设置
	EA = 1;		  // 使能EA总中断

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
	// 没有什么时序时序要求，，指点函数里面做算了。。
	// 算了，之前都写成这样了。。不改了。。原有逻辑上面改改就行吧，，
	while (1)
	{
		showled();
		keyallchuli();
		showpre(gsetzhi);
		dealorder();//取缓冲区里面的命令进行发送
		
	}
}

void PWM_ISR() interrupt 26
{
	if (PWMA_SR1 & 0X02) // 当捕获中断标志置1
	{
		cnt_H = PWMA_CCR1H; // 读取计数值高8位
		cnt_L = PWMA_CCR1L; // 读取计数值低8位
		PWMA_SR1 &= ~0X02;	// 清零捕获中断标志
		EC11_Handle();		// 处理EC11数据
	}
}
void PWM_Config() // PWM初始化设置
{
	PWMA_CCER1 = 0x00; // 关闭通道
	PWMA_CCMR1 = 0xA1; // 通道模式配置为输入，接编码器 , 滤波器 80 时钟
	PWMA_CCMR2 = 0xA1; // 通道模式配置为输入，接编码器 , 滤波器 80 时钟
	PWMA_CCER1 = 0x55; // 使能捕获/比较通道1、通道2

	//	PWMA_SMCR = 0x01; 				// 编码器模式 1
	//	PWMA_SMCR = 0x02; 				// 编码器模式 2
	PWMA_SMCR = 0x03; // 编码器模式 3

	PWMA_IER = 0x02;  // 使能中断
	PWMA_CR1 |= 0x01; // 使能计数器
	PWMA_PS |= 0x04;  // 选择 PWM2_2 通道
}

// 返回变化的步数
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
		if (timeleft3 == 0) // 数据一次收完了.
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
void EC11_Handle() // EC11数据处理函数
{
	static unsigned int previous = 0;
	unsigned int nowzhi;
	long ans;
	nowzhi = cnt_H * 256 + cnt_L; // 读取当前计数值
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
		// 设定电流值。。。
		setzhione(4,gsetzhi);
		// setzhione(2,1);//开灯。。
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
	if (TI) // 发送中断..
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
        S3CON &= ~S3TI; // 清除S3TI位
        busy3 = 0;      // 清忙标志
    }
}

// 写个函数，传入两个非负数，计算是前进还是后退了，变化规律 15,16,0,1,2...15,16