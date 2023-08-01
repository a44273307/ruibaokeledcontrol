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
#include "tongxin2.h"
#include "tongxin.h"
#include "ntc10k3950.h"
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

void send1(char c)
{
	HAL_UART_Transmit(&UART1_Handler, &c, 1, 0xffffff);
}
void send2(char c)
{
	HAL_UART_Transmit(&UART2_Handler, &c, 1, 0xffffff);
}
void send3(char c)
{
	HAL_UART_Transmit(&UART3_Handler, &c, 1, 0xffffff);
}
void sendshuju(char *p)
{
	while (*p != '\0')
	{
		send2(*p);
		p++;
	}
}


void time03msjisuan() //ʹ��ǰ����������ʼ��  ����Ŀ��ơ���
{

}

//�жϷ�����
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); //�����жϴ����ú���
}
void run()
{
	
}



void init();
void dealchuankou();
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
	// if( weizhi== 4 )
	// {
		push(weizhi,zhi);
		// tmp=zhi;
	// }
}
void print2(char *p)
{
	while (*p != '\0')
	{
		send2(*p);
		p++;
	}
}
void print1(char *p)
{
	while (*p != '\0')
	{
		send1(*p);
		p++;
	}
}
void print3(char *p)
{
	while (*p != '\0')
	{
		send3(*p);
		p++;
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
		// if(get.weizhi==4)
		// {
		// 	get.zhi=get.zhi;
		// }
		sprintf(out,"set:%d-%d;end",get.weizhi,get.zhi);
		print1(out);
		printf("send req2[%s]",out);
	}
}
void jiexi(char* input);
void inputbuf3(const char *s)
{
	char tmpstr[300]={0};
	strcpy(tmpstr,s);
	jiexi(tmpstr);
}

long getdianzu(long dianya)
{
	if(dianya==4095)
	{
		return 4700; 
	}
	return 4700*dianya/(4095-dianya);
	// dianya/1023*
}
int jisuanwendu(int R)
{
	int p,T;
	unsigned long Ac = 0;
        for ( p=1; p<sizeof(NTC10K3950)/sizeof(NTC); p++ ) {
      if ( R >= NTC10K3950[p].R ) 
	  {
        Ac = R - NTC10K3950[p].R; // delta resistance
        Ac *= 50; //multiply by 5.0 degrees celsius step of table
        Ac /= NTC10K3950[p-1].R - NTC10K3950[p].R; // divide by range of resistence 
        T = NTC10K3950[p].t*10 - Ac; // temperature offset
        break;
      }
    }
	return T;
}
int gwendu;
int gguangzhao;
void showendu()
{
	char out[30]={0};
	sprintf(out,"set:06-%d;end",gwendu);
	print3(out);
}
int getwendu()
{
	int dianya;
	long dianzu;
	dianya=Get_Adc_Average(ADC_CHANNEL_10,1);
	dianzu=getdianzu(dianya);
	gwendu=jisuanwendu(dianzu);
	printf("tmp is %d",gwendu);
	showendu();
}

int main(void)
{
	int bushu;
	int times;
	int i, j, k;
	int ts;
	int a[10];
	int key;


	init();
	// inputbuf3("set:6-181;end");
	MY_ADC_Init();
	while (1)
	{
		delay_ms(1);
		dealchuankou();
		getzhiandchange();
		dealorder();
		if(i++>1000)
		{
			i=0;
			getwendu();

			// dianya1=Get_Adc_Average(ADC_CHANNEL_11,1);
			// printf("Get_Adc_Average [%d]-[%d]",dianya,dianya1);
		}
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
	// __HAL_AFIO_REMAP_SWJ_NOJTAG();
	Y0 = 0;
	Y1 = 0;
	initio(); //IO��������ʼ��
	Y0 = 0;
	Y1 = 0;
	TIM3_Init(64-1,1000-1);//1ms һ��

}


// �Ӵ�����ʾ���ģ���Ҳ���Ǳ��˶��������롣��

char flag3 = 0;
int weishu1, weishu2, weishu3, weishu4;
char buf3[300];
char buf1[300]={0};
int timeleft1, timeleft2, timeleft3, timeleft4;
char falgchuankou1 = 0;
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
		if (timeleft3 == 0) // ����һ��������.
		{
			flag3 = 1;
		}
	}
}


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
            return (char*)haystack; // �Ӵ�ƥ��ɹ���������ʼλ��
        }

        haystack++;
    }

    return NULL; // δ�ҵ��Ӵ�������NULL
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
// ���룬�����20������������ص�ֵ��Ĭ���ǵ���ֵ������
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
		p1=myaddstrstr(p,";"); //����û����һ����
		if(p1==NULL)
		{
			break;
		}
		weizhi = atoi(p);
		p=myaddstrstr(p,"-");
		zhi = atoi(p);
		p=myaddstrstr(p,";");  //ָ����һ������
		printf("get set%d-%d",weizhi,zhi);
		push2(weizhi,zhi);
		
	}
}
void jiexi(char* input)
{
	char par[500]={0};
	char *begin,*end;
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
void chuankou3jisuuan()
{
	chuankou1put(USART3->DR);
}

void time02msjisuan()
{
	
	if(!empty())
	{
		timepush++;
	}
	chuankou1time();
}