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
 ALIENTEK Mini STM32F103������ʵ��7
 ��ʱ���ж�ʵ��-HAL�⺯���� 
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
void nvicset()
{
	HAL_NVIC_SetPriority(USART1_IRQn, 10, 0);
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_SetPriority(TIM2_IRQn, 4, 0); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
	HAL_NVIC_SetPriority(TIM3_IRQn, 4, 0); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
	HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
}
char flag = 0;
void settime(int zhi)
{
	flag = zhi;
}
void sendbyte2(u8 c) //����4����һ���ֽڵĺ���
{
	HAL_UART_Transmit(&UART2_Handler, &c, 1, 0xffffff);
}
//2000��һȦ...
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
void keydown(char i) //�������µĴ�������
{
	//printf("key%d",i);
}
void keyallchuli()
{
	int i;
	static int flag[10] = {0}; //��־��¼
	for (i = 0; i < 10; i++)
	{
		if (xin[i] == 0)
		{
			if (flag[i] == 0) //��������һ�ΰ��¡�����
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
void time03msjisuan() //ʹ��ǰ����������ʼ��  ����Ŀ��ơ���
{
}
int cnt;
int precnt;
//�жϷ�����
void EXTI0_IRQHandler(void)
{
	cnt++;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); //�����жϴ����ú���
}
//�����������������ȡdht22��ʪ��ֵ	������ȡ��ֵ
void pid_init();				//PID������ʼ��
float pid_realise(float speed); //ʵ��PID�㷨
struct
{
	float set_speed;	//�趨�ٶ�
	float actual_speed; //ʵ���ٶ�
	float error;		//ƫ��
	float error_next;	//��һ��ƫ��
	float error_last;	//����һ��ƫ��
	float kp, ki, kd;	//������������֣�΢�ֲ���
} pid;
void pid_init() //PID ������ʼ��
{
	pid.set_speed = 0.0;
	pid.actual_speed = 0.0;
	pid.error = 0.0;
	pid.error_next = 0.0;
	pid.error_last = 0.0;
	//�ɵ���PID ������ʹ�������������ӽ���Ծ����200.0 //
	pid.kp = 0.2;
	pid.ki = 0.8;
	pid.kd = 0.1;
}
float pid_realisexx(float speed, float nowspeed) //ʵ��pid
{
	float increment_speed;						  //����  ��Ҫ�����pwmֵ
	pid.set_speed = speed;						  //����Ŀ��ʪ��
	pid.actual_speed = nowspeed;				  //��ǰ��ʪ��
	pid.error = pid.set_speed - pid.actual_speed; //��ǰ��ʪ�Ȳ�ֵ	��Ӧ��en
	increment_speed = pid.kp * (pid.error - pid.error_next) + pid.ki * pid.error +
					  pid.kd * (pid.error - 2 * pid.error_next + pid.error_last); //�������㹫ʽ
	pid.error_last = pid.error_next;											  //��һ�ε���
	pid.error_next = pid.error;													  //��һ�ε���
	return increment_speed;														  //���ؼ��ȵ����ֵ
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
	// TIM3_Init(64 - 1, 5000 - 1); //10����
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
	Stm32_Clock_Init(RCC_PLL_MUL9); //����ʱ��,72M
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init(64);		 //��ʼ����ʱ����
	uart_init(115200);	 //��ʼ������
	usmart_dev.init(84); //��ʼ��USMART
	__HAL_AFIO_REMAP_SWJ_NOJTAG();
	Y0 = 0;
	Y1 = 0;
	initio(); //IO��������ʼ��
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