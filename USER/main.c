#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "io.h"
#include "pingmu.h"
#include "DS1302.h"
#include "24cxx.h"
#include "bujindianji1.h"
#include "bujindianji2.h"
#include <string.h>
/************************************************
 ALIENTEK Mini STM32F103开发板实验7
 定时器中断实验-HAL库函数版 
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
void nvicset()
{
	HAL_NVIC_SetPriority(USART1_IRQn, 10, 0);
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_SetPriority(TIM2_IRQn, 4, 0); //设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_SetPriority(TIM3_IRQn, 4, 0); //设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
}
char flag = 0;
void settime(int zhi)
{
	flag = zhi;
}
void sendbyte2(u8 c) //串口4发送一个字节的函数
{
	HAL_UART_Transmit(&UART2_Handler, &c, 1, 0xffffff);
}
//2000步一圈...
#define zheng 0
#define fan 1
void send2(char c)
{
	HAL_UART_Transmit(&UART2_Handler, &c, 1, 0xffffff);
}
void sendshuju(char *p)
{
	while (*p != '\0')
	{
		sendbyte2(*p);
		p++;
	}
	sendbyte2('\r');
	sendbyte2('\n');
}
void keydown(char i);
char shuju[15];
int shujuweizhi = 0;
int timeleft = 0;
void chuankou2jisuuan()
{
	shuju[shujuweizhi++] = USART2->DR;
	if (shujuweizhi > 8)
		shujuweizhi = 0;
	timeleft = 4;
}
void shujuchuli()
{
	int c;
	char *p = 0;
	char flag = 0;
	if (shujuweizhi == 3)
	{
		if (strstr(shuju, "do") != p)
		{
			c = shuju[2] - '0';
			keydown(c);
			flag = 1;
		}
	}
	if (flag == 1)
	{
		sendshuju("success");
	}
	else
		sendshuju("error");
}
void timechulichuankou()
{
	if (timeleft > 0)
	{
		timeleft--;
		if (timeleft == 0)
		{
			shujuchuli();
			shujuweizhi = 0;
		}
	}
}
#define zidong 1
#define true 1
int guangset = 50;
int flagzidong = 0;
int flagopen = 0;
int flagopenkey = 0;
int flagdongzuo = 0;
void keydown(char i) //按键按下的处理、、、
{
	//printf("key%d",i);
}
void keyallchuli()
{
	int i;
	static int flag[10] = {0}; //标志记录
	for (i = 0; i < 10; i++)
	{
		if (xin[i] == 0)
		{
			if (flag[i] == 0) //代表按键第一次按下。。。
			{
				flag[i] = 1;
				keydown(i);
			}
		}
		else
			flag[i] = 0;
	}
}
void time02msjisuan()
{
	keyallchuli();
	timechulichuankou();
}
void time03msjisuan() //使用前在主函数初始化  电机的控制。。
{
}
int cnt;
int precnt;
//中断服务函数
void EXTI0_IRQHandler(void)
{
	cnt++;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); //调用中断处理公用函数
}
//从这里结束，，，读取dht22温湿度值	。。读取的值
void pid_init();				//PID参数初始化
float pid_realise(float speed); //实现PID算法
struct
{
	float set_speed;	//设定速度
	float actual_speed; //实际速度
	float error;		//偏差
	float error_next;	//上一个偏差
	float error_last;	//上上一个偏差
	float kp, ki, kd;	//定义比例，积分，微分参数
} pid;
void pid_init() //PID 参数初始化
{
	pid.set_speed = 0.0;
	pid.actual_speed = 0.0;
	pid.error = 0.0;
	pid.error_next = 0.0;
	pid.error_last = 0.0;
	//可调节PID 参数。使跟踪曲线慢慢接近阶跃函数200.0 //
	pid.kp = 0.2;
	pid.ki = 0.8;
	pid.kd = 0.1;
}
float pid_realisexx(float speed, float nowspeed) //实现pid
{
	float increment_speed;						  //增量  需要输出的pwm值
	pid.set_speed = speed;						  //设置目标湿度
	pid.actual_speed = nowspeed;				  //当前的湿度
	pid.error = pid.set_speed - pid.actual_speed; //当前的湿度差值	对应的en
	increment_speed = pid.kp * (pid.error - pid.error_next) + pid.ki * pid.error +
					  pid.kd * (pid.error - 2 * pid.error_next + pid.error_last); //增量计算公式
	pid.error_last = pid.error_next;											  //下一次迭代
	pid.error_next = pid.error;													  //下一次迭代
	return increment_speed;														  //返回加热的输出值
}
int pwm;
int times = 0;
void showdianjiweizhi();
void run()
{
	
}

void zhuanhuan()
{
	
}
void showdianjiweizhi()
{
	
}
void zhongduanclose()
{
	
}
void zhongduanopen()
{
	
}
__weak void getdata()
{
	// zhongduanclose();
	// DHT11_Read_Data(&wendu, &shidu);
	// zhongduanopen();
	//printf("wendu%dshidu%d",wendu,shidu);
	//Get_Adc_Average(ADC_CHANNEL_6,5)/20;
	//guang = 100 - Get_Adc_Average(ADC_CHANNEL_4, 5) / 41;
}

void init();
int main(void)
{
	int bushu;
	int times;
	int i, j, k;
	int ts;
	int a[10];
	int key;
	
	// HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	// TIM_SetTIM1Compare1(0);
	// TIM1_PWM_Init(1000 - 1, 6400 - 1);
	// TIM_SetTIM1Compare1(0);
	// TIM_SetTIM1Compare4(0);
	// TIM3_Init(64 - 1, 5000 - 1); //10毫秒
	// TIM2->CNT = 30000;
	init();
	while (1)
	{
		delay_ms(2000);
		printf("test run");
	}
}

void init()
{
	HAL_Init();
	Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init(64);		 //初始化延时函数
	uart_init(115200);	 //初始化串口
	usmart_dev.init(84); //初始化USMART
	__HAL_AFIO_REMAP_SWJ_NOJTAG();
	Y0 = 0;
	Y1 = 0;
	initio(); //IO输出输出初始化
	Y0 = 0;
	Y1 = 0;
	delay_ms(50);
	nvicset();
	DHT11_Init();
	delay_ms(100);
	OLED_Init();
	delay_ms(100);
	OLED_Clear();
	MY_ADC_Init();
	delay_ms(50);
	MX_TIM2_Init();
	delay_ms(50);
}