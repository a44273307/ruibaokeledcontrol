// 	@布丁橘长 2023/03/10
// 	PWM编码器模式示例，本示例程序编码器模式1/2/3均支持，SMCR = 1：模式1，SMCR = 2:模式2，SMCR = 3：模式3
// 	EC11左旋或右旋，改变数码管亮度
//	EC11引脚定义：A相-P1.0，B相-P5.4
//  MAX7219数码管模块引脚定义：CS = P6^5;DIN = P6^6;CLK = P6^4;（可在MAX7219.h中修改）
//  实验开发板：屠龙刀三.1 @主频12MHz

#include "stc32g.h"
#include "config.h"
#include "MAX7219.h"
#include <stdio.h>
#include <stdarg.h>

void SYS_Ini();								// STC32初始化设置
void EC11_Handle();						// EC11数据处理
void SEG_Disp(void);					// 数码管显示
void PWM_Config();						// PWM初始化设置

u8 cnt_H, cnt_L;							// 计数值高8位、低8位
u16 count,newcount;						// 当前计数值、上次计数值
u8 number;										// 亮度计数值
bit numberchange;							// 亮度改变标志

void UartInit(void)		//115200@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCC;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES=1;
	//	ES=0;//关闭串口0中断
	EA=1;
}




void SYS_Ini()								// STC32初始化设置
{
	EAXFR = 1; 									// 使能访问 XFR
	CKCON = 0x00; 							// 设置外部数据总线速度为最快
	WTST = 0x00; 								// 设置程序代码等待参数，赋值为 0 可将 CPU 执行程序的速度设置为最快
	P0M1 = 0x00;P0M0 = 0x00;		// 设置P0口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P1M1 = 0x00;P1M0 = 0x00;		// 设置P1口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P2M1 = 0x00;P2M0 = 0x00;		// 设置P2口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P3M1 = 0x00;P3M0 = 0x00;		// 设置P3口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P4M1 = 0x00;P4M0 = 0x00;		// 设置P4口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P5M1 = 0x00;P5M0 = 0x00;		// 设置P5口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P6M1 = 0x00;P6M0 = 0x00;		// 设置P6口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
	P7M1 = 0x00;P7M0 = 0x00;		// 设置P7口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
}

void sendbyte1(unsigned char ch)
{
	int i;
	// EA=0;
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   ch;
    while(TI ==0) //等待发送完成
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
    if (TI) // 发送中断..
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
// 定义printf函数
void printf1(const char *fmt, ...) {
	 char *p;
    char buf[128];  // 定义一个缓冲区，足够存储输出的字符串
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);  // 将格式化的字符串写入缓冲区
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
	
	SYS_Ini();									// STC32初始化设置
	PWM_Config();							  // PWM初始化设置
	EA = 1;											// 使能EA总中断
	MAX7219_Ini();							// MAX7219初始化
	number = 10;								// 初始亮度10
	SEG_Disp();									// 数码管显示
	testmain();
	while (1)
	{
		if(numberchange == 1)			// 当改变亮度标志置1
		{
			Write7219(INTENSITY,number);  // 设置数码管亮度
			SEG_Disp();							// 数码管刷新显示
			numberchange = 0;				// 清零改变亮度标志
		}
	}
}
void PWM_ISR() interrupt 26
{
	if (PWMA_SR1 & 0X02)				// 当捕获中断标志置1
	{
		cnt_H = PWMA_CCR1H;				// 读取计数值高8位
		cnt_L = PWMA_CCR1L;				// 读取计数值低8位
		PWMA_SR1 &= ~0X02;				// 清零捕获中断标志
		EC11_Handle();							// 处理EC11数据
	}
}
void PWM_Config()							// PWM初始化设置
{
	PWMA_CCER1 = 0x00;					// 关闭通道
	PWMA_CCMR1 = 0xA1; 					// 通道模式配置为输入，接编码器 , 滤波器 80 时钟
	PWMA_CCMR2 = 0xA1; 					// 通道模式配置为输入，接编码器 , 滤波器 80 时钟
	PWMA_CCER1 = 0x55;					// 使能捕获/比较通道1、通道2
	
//	PWMA_SMCR = 0x01; 				// 编码器模式 1
//	PWMA_SMCR = 0x02; 				// 编码器模式 2	
	PWMA_SMCR = 0x03; 					// 编码器模式 3
	
	PWMA_IER = 0x02; 						// 使能中断
	PWMA_CR1 |= 0x01; 					// 使能计数器
	PWMA_PS |= 0x04; //选择 PWM2_2 通道
}



// 返回变化的步数
long  calculateChange(unsigned int previous, unsigned int current) {
  long  diff = (current - previous + 65536) % 65536;
  return diff;
}



void showled()
{

}
void EC11_Handle()						// EC11数据处理函数
{
	static unsigned int previous=0; 
	unsigned int  nowzhi;
	long ans;
	nowzhi = cnt_H * 256 + cnt_L;	// 读取当前计数值
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
	// if(newcount < count) 				// 当前计数值小于上次计数值
	// {
	// 	if(number > 0)	number--;	// 数字减
	// 	numberchange = 1;					// 亮度改变标志置1
	// 	count = newcount;					// 更新计数值
	// }
	// else if(newcount > count)		// 当前计数值大于上次计数值
	// {
	// 	if(number < 15) number++;	// 数字加
	// 	numberchange = 1;					// 亮度改变标志置1
	// 	count = newcount;					// 更新计数值
	// }
}
void SEG_Disp(void)											
{							
	Write7219(8,15); 						// 左起第1位熄灭
	Write7219(7,15); 						// 左起第2位熄灭
	Write7219(6,15); 						// 左起第3位熄灭
	Write7219(5,15); 						// 左起第4位熄灭
	Write7219(4,15); 						// 左起第5位熄灭
	Write7219(3,15); 						// 左起第6位熄灭
	Write7219(2,(u8)(number / 10)); 	// 左起第7位显示数字十位
	Write7219(1,(u8)(number % 10)); 	// 左起第8位显示数字个位
}

// 写个函数，传入两个非负数，计算是前进还是后退了，变化规律 15,16,0,1,2...15,16    