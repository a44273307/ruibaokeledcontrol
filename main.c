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


void SYS_Ini();		// STC32初始化设置
void EC11_Handle(); // EC11数据处理
					// 数码管显示
void PWM_Config(); // PWM初始化设置

u8 cnt_H, cnt_L;	 // 计数值高8位、低8位
u16 count, newcount; // 当前计数值、上次计数值
					 // 亮度计数值


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


void dealchuankou();
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
		if(weizhi<MAXgetzhi)
		getzhi[weizhi]=zhi;
		p=myaddstrstr(p,";");  //指向下一个后面
		printf1("get set%d-%d",weizhi,zhi);
	}
}
void setzhione(int dizhi,int zhi)
{
	char out[30]={0};
	sprintf(out,"set:%d-%d;end",dizhi,zhi);
	printf3(out);
}
void jiexi(char* input)
{
	char par[500]={0};
	char *begin,end;
	begin=myaddstrstr(input,"set:");
	// printf1("input begin%s",begin);
	end=myaddstrstr(begin,"end");
	// printf1("input end%s",end);
	if(begin!=NULL && end!=NULL)
	{
		strcpy(par,begin);
		jixi2(par);
	}
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
		setdianliusettime(100);
	}
}
void timechuli()
{
	if(dianliusettime>0)
	{
		dianliusettime--;
		if(dianliusettime==0)
		{
			flagdianliuset=1;
		}
	}
}
void chulidianliu()
{
	if(flagdianliuset==1)
	{
		flagdianliuset=0;
		setzhione(4,gsetzhi);
		printf1("dianliu set%d",gsetzhi);
	}
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
	while (1)
	{
		showpre(gsetzhi);
		dealchuankou();
		changedainliuzhi();
		chulidianliu();
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
		if (timeleft3 == 0) // 数据一次收完了.
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
		// IAP_CONTR=0x60;
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
void addgetsetzhi(int i)
{
	int ans;
	ans=gsetzhi+i;
	if(ans>=0 && ans<=1023)
	{
		gsetzhi=ans;
	}
}
void Timer0() interrupt 1
{
	// gsetzhi++;
	timechuli();
	chuankou1time();
}
void EC11_Handle() // EC11数据处理函数
{
	static unsigned int previous = 0;
	unsigned int nowzhi;
	long ans;
	nowzhi = cnt_H * 256 + cnt_L; // 读取当前计数值
	ans = calculateChange(previous, nowzhi);
	previous = nowzhi;
	if (ans == 4)
	{
		addgetsetzhi(1);
	}
	else
	{
		addgetsetzhi(-1);
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
	if (flag3 == 1)
	{
		flag3 = 0;
		jiexi(buf3);
		memset(buf3, 0, sizeof(buf3));
		weishu3 = 0;
	}
}


void Uart3() interrupt 17 using 1
{
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI; //??S3RI?
        // temp3 = S3BUF;
        // chuankou3put(temp3);
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI; // 清除S3TI位
        busy3 = 0;      // 清忙标志
    }
}
// 写个函数，传入两个非负数，计算是前进还是后退了，变化规律 15,16,0,1,2...15,16