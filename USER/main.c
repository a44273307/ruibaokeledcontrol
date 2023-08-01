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
#include "myiic.h"
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


void time03msjisuan() //使用前在主函数初始化  电机的控制。。
{

}

//中断服务函数
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); //调用中断处理公用函数
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
    // printf("getzhiandchange weizhi[%d] zhi[%d]\n",weizhi,zhi);
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
		sprintf(out,"set:%d-%d;%d-%d;end",get.weizhi,get.zhi,get.weizhi,get.zhi);
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
int g_wendu;
int gguangzhao;
void showendu()
{
	char out[30]={0};
	sprintf(out,"set:06-%d;end",g_wendu);
	print3(out);
}
int getwendu()
{
	int dianya;
	long dianzu;
	dianya=Get_Adc_Average(ADC_CHANNEL_10,1);
	dianzu=getdianzu(dianya);
	g_wendu=jisuanwendu(dianzu);
	// printf("tmp is %d",g_wendu);
	showendu();
}

//*********************************
#define SlaveAddress 0x46 // 定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                          // ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为  0xB8
// #define SlaveAddress 0xB8
void Single_Write_BH1750(u8 REG_Address)
{
    IIC_Start();                // 起始信号
    IIC_Send_Byte(SlaveAddress); // 发送设备地址+写信号
	IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);  // 内部寄存器地址，
    IIC_Wait_Ack();
    IIC_Stop(); // 发送停止信号
}


int Multiple_Read_BH1750()
{
    int dis_data; // 变量
    u8 BUF[8]={0}; // 接收数据缓存区
    u8 i;
    IIC_Start();                    // 起始信号
    IIC_Send_Byte(SlaveAddress + 1); // 发送设备地址+读信号
	IIC_Wait_Ack();
    for (i = 0; i < 3; i++) // 连续读取2个地址数据，存储中BUF
    {
        // BUF[i] = IIC_Read_Byte(); // BUF[0]存储0x32地址中的数据
        if (i == 3)
        {
			BUF[i] =IIC_Read_Byte(0);
            // IIC_NAck(); // 最后一个数据需要回NOACK
        }
        else
        {
			BUF[i] =IIC_Read_Byte(1);
            // IIC_Ack(); // 回应ACK
        }
    }

    IIC_Stop(); // 停止信号
    dis_data = BUF[0];
    dis_data = (dis_data << 8) + BUF[1]; // 合成数据，即光照数据
    return dis_data;
    // Delay5ms();
}
// 每次进来加1，，然后多久后就读数据，发出去。。。
int runningtimes=0; 
int g_guangzhi;
void getiicguang()
{
    int dis_data;
	runningtimes++;
	if(runningtimes==1)
	{
		Single_Write_BH1750(0x01); // power on
    	Single_Write_BH1750(0x10); // H- resolution mode
		return ;
	}
    // delay_ms(180);
	if(runningtimes==200)
	{
		g_guangzhi = Multiple_Read_BH1750();
		return ;
	}
	if(runningtimes==800)
	{
		runningtimes=0;
	}
}
void sendtodiannao()
{
	printf("ToComputer guang[%d]wendu[%d]end\n",g_guangzhi,g_wendu);
}
int timefengshan=0;
void checksudu()
{
	int i;
	int count=0;
	timefengshan=0;
	while (timefengshan<50)
	{
		while (X0==0 && timefengshan<50);
		while (X0==1 && timefengshan<50);
		count++;
	}
	if(count==1)
	{
		printf("speed is low close");
		push(4,0);
	}
	// printf("count is %d",count);
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
	IIC_Init();
	while (1)
	{
		Y0=0;
		delay_ms(1);
		
		dealchuankou();
		getzhiandchange();
		dealorder();
		getiicguang();
		if(i==800)
		{
			checksudu();
		}
		if(i++>1000)
		{
			i=0;
			getwendu();
			sendtodiannao();
			
		}
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
	// __HAL_AFIO_REMAP_SWJ_NOJTAG();
	Y0 = 0;
	Y1 = 0;
	initio(); //IO输出输出初始化
	Y0 = 0;
	Y1 = 0;
	TIM3_Init(64-1,1000-1);//1ms 一次

}


// 接触到显示屏的，，也就是别人对我是输入。。

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
		if (timeleft3 == 0) // 数据一次收完了.
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
            return (char*)haystack; // 子串匹配成功，返回起始位置
        }

        haystack++;
    }

    return NULL; // 未找到子串，返回NULL
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
void showcom1();
void time02msjisuan()
{
	if(!empty())
	{
		timepush++;
	}
	chuankou1time();
	showcom1();
	timefengshan++;
}

char rec1[1000]={0};
int weizhi1=0;
int timeleft1=0;
void chuankou1jisuuan()
{
	rec1[weizhi1++]=USART1->DR;
	if (weizhi1 > sizeof(rec1) - 3)
		weizhi1 = 0;
	timeleft1 = 3;
}
void showcom1()
{
	if(timeleft1>0)
	{
		timeleft1--;
		if(timeleft1==0)
		{
			printf("rec[%s]",rec1);
			memset(rec1,0,sizeof(rec1));
			weizhi1=0;
		}
	}
}