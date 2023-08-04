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
#include "stmflash.h"
/************************************************
 ALIENTEK Mini STM32F103������ʵ��7
 ��ʱ���ж�ʵ��-HAL�⺯���� 
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 �������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

// 01 ������������ַ�ģ���Ĭ��55
u16 g_reg[40]={0};
#define indexAdddianliu 20
#define indexTImeuse 21
#define indexTImeAll 22



void EPPROMwrite()
{
	g_reg[0]=0x55;
	STMFLASH_Write(FLASH_SAVE_ADDR,g_reg,30);
}
void showzhi()
{
	int i;
	int len2=sizeof(g_reg[0]);
	int len=sizeof(g_reg);
	len=len/len2;
	for(i=0;i<len;i++)
	{
		if(g_reg[i]!=0)
		{
			printf("[%d-%d]",i,g_reg[i]);
		}
	}
}
void EPPROMinit()
{        
	STMFLASH_Read(FLASH_SAVE_ADDR,g_reg,30);
	if(g_reg[0]!=0x55)
	{
		printf("this is First record");
		memset(g_reg,0,sizeof(g_reg));
		g_reg[indexTImeAll]=25000;
		EPPROMwrite();
	}
	else
	{
		printf("not First record");
	}
	showzhi();
}


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
void send4(char c)
{
	HAL_UART_Transmit(&UART4_Handler, &c, 1, 0xffffff);
}
void sendshuju(char *p)
{
	while (*p != '\0')
	{
		send2(*p);
		p++;
	}
}




//�жϷ�����
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); //�����жϴ������ú���
}
void run()
{
	
}



void init();
void dealchuankou();
// ��Ļ������ָ������·���ȥ������
void getzhiandchange()
{
    int weizhi,zhi;
    VectorInfo get={0};
    VectorGet(Vectorpingmu,&get);
    if(get.weizhi==0)
    {
        return ;
    }
    weizhi=get.weizhi;
    zhi=get.zhi;
	push(weizhi,zhi); //ѹ���������ӷ���ָ�����
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
// push ָ����صļ��㡣��
int selfdeal(int weizhi,int zhi)
{
	if(weizhi==indexAdddianliu)
	{
		g_reg[indexAdddianliu]=zhi;
		EPPROMwrite();
		printf("indexAdddianliu:%d-%d;",weizhi,zhi);
		return 1;
	}
	if(weizhi==indexTImeuse)
	{
		g_reg[indexTImeuse]=zhi;
		EPPROMwrite();
		printf("indexTImeuse:%d-%d;",weizhi,zhi);
		return 1;
	}
	if(weizhi==indexTImeAll)
	{
		g_reg[indexTImeAll]=zhi;
		EPPROMwrite();
		printf("indexTImeAll:%d-%d;",weizhi,zhi);
		return 1;
	}
	return 0;

}
// 25000
// 
int shoumingjisuan(int predianliu)
{
	int bili;
	if(g_reg[indexTImeAll]==0)
	return predianliu;
	bili=g_reg[indexTImeuse]*100/g_reg[indexTImeAll];
	if(bili>100)
	{
		return 0;
	}
	if(bili>90)
	{
		return predianliu*0.3;
	}
	if(bili>80)
	{
		return predianliu*0.5;
	}
	if(bili>70)
	{
		return predianliu*0.8;
	}
	return predianliu;
}
// �����������
int jisuandianliu(int predianliu)
{
	if(predianliu==0)
	{
		return 0;
	}
	predianliu=predianliu+g_reg[indexAdddianliu];
	if(predianliu>2047)
	{
		predianliu=2047;
	}
	return shoumingjisuan(predianliu);
}
int g_dianliu=0;
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
			g_dianliu=get.zhi;
			get.zhi=jisuandianliu(get.zhi);
		}
		sprintf(out,"set:%d-%d;%d-%d;end",get.weizhi,get.zhi,get.weizhi,get.zhi);
		if(selfdeal(get.weizhi,get.zhi))
		{
			
		}
		else
		{
			print1(out);
			// printf("send req2[%s]",out);
		}
		
	}
}
void jiexi(char* input);
void inputbuf3(const char *s)
{
	char tmpstr[1000]={0};
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
	showendu();
}

//*********************************
#define SlaveAddress 0x46 // ����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                          // ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ  0xB8
// #define SlaveAddress 0xB8
void Single_Write_BH1750(u8 REG_Address)
{
    IIC_Start();                // ��ʼ�ź�
    IIC_Send_Byte(SlaveAddress); // �����豸��ַ+д�ź�
	IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);  // �ڲ��Ĵ�����ַ��
    IIC_Wait_Ack();
    IIC_Stop(); // ����ֹͣ�ź�
}


int Multiple_Read_BH1750()
{
    int dis_data; // ����
    u8 BUF[8]={0}; // �������ݻ�����
    u8 i;
    IIC_Start();                    // ��ʼ�ź�
    IIC_Send_Byte(SlaveAddress + 1); // �����豸��ַ+���ź�
	IIC_Wait_Ack();
    for (i = 0; i < 3; i++) // ������ȡ2����ַ���ݣ��洢��BUF
    {
        // BUF[i] = IIC_Read_Byte(); // BUF[0]�洢0x32��ַ�е�����
        if (i == 3)
        {
			BUF[i] =IIC_Read_Byte(0);
            // IIC_NAck(); // ���һ��������Ҫ��NOACK
        }
        else
        {
			BUF[i] =IIC_Read_Byte(1);
            // IIC_Ack(); // ��ӦACK
        }
    }

    IIC_Stop(); // ֹͣ�ź�
    dis_data = BUF[0];
    dis_data = (dis_data << 8) + BUF[1]; // �ϳ����ݣ�����������
    return dis_data;
    // Delay5ms();
}
// ÿ�ν�����1����Ȼ���ú�Ͷ����ݣ�����ȥ������
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
	// printf("ToComputer guang[%d]wendu[%d]end\n",g_guangzhi,g_wendu);
}
int timefengshan=0;

void exitinit()
{
	 GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.Pin = GPIO_PIN_8;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);     
}
int fentmp; //�����ٶ���ʱ����
int g_fengshan=100; //�����ٶ�
void EXTI9_5_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);         /* �����жϴ������ú��� ���KEY1�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);         /* HAL��Ĭ�������ж��ٴ����ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
	fentmp++;
}
void time03msjisuan()
{
	g_fengshan=fentmp;
	fentmp=0;
}
void WriteToPingmu(int weizhi,int zhi)
{
	char out[30]={0};
	delay_ms(100);
	sprintf(out,"set:%d-%d;end",weizhi,zhi);
	print3(out);
	// printf("pingmu[%s]",out);
	delay_ms(100);
}
int iserror()
{
	if(g_fengshan<5)
	{
		return 1;
	}
	if(g_wendu>600)
	{
		return 1;
	}
	return 0;
}
// ����������ص�
void baojincheck()
{
	if(g_fengshan<5)
	{
		push(4,0);  
		WriteToPingmu(11,1);//�����쳣�澯��
	}
	if(g_wendu>600)
	{
		push(4,0);
		WriteToPingmu(12,1);//�¶�
	}
}
// 65536 Сʱ
char flagwrie=0;
void shoumingrun()
{
	static long miao=0;
	miao++;
	if(miao>60*60)
	{
		flagwrie=1;
		miao=0;
	}
}
void shoumingjilu()
{
	if(flagwrie==1)
	{
		flagwrie=0;
		g_reg[indexTImeuse]++;
		EPPROMwrite();
	}
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
	MY_ADC_Init();
	IIC_Init();
	EPPROMinit();
	exitinit();
	while (1)
	{
		// send4('d');
		Y0=0;
		shoumingjilu();
		delay_ms(1);
		dealchuankou();//������Ļ������ָ��
		getzhiandchange();//������Ļ������ָ��
		dealorder();//�Լ�ѹ�����������ȥ����
		getiicguang();//������ֵ����
		if(i++>1000)
		{
			i=0;
			getwendu();
			sendtodiannao();
			baojincheck();
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
	TIM2_Init(64-1,1000-1);//1ms һ��
	TIM3_Init(6400-1,10000-1);//1000ms һ��
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
		VectorPush(Vectorpingmu,weizhi,zhi);
	}
}
void jiexi(char* input)
{
	char par[1000]={0};
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





char rec2[1500]={0};
int weizhi2=0;



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
		p1=myaddstrstr(p,";"); //����û����һ����
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
                // printf("get set%d-%d",weizhi,zhi);
                VectorPush(VectorDiannao,weizhi,zhi);
				flagTrue=1;
            }
            else
            {
				flagTrue=0;
                printf("get failed");
            }
        }
		p=myaddstrstr(p,";");  //ָ����һ������
	}
	return flagTrue;
}



// ���Թ�����ָ���������������
// һ����ѹ�����С���Ȼ�����Ϳ����ˡ��������������ˡ�����
void dealDiannaoOrder()
{
    int weizhi,zhi;
    VectorInfo get={0};
    VectorGet(VectorDiannao,&get);
    if(get.weizhi==0)
    {
        return ;
    }
    weizhi=get.weizhi;
    zhi=get.zhi;
	if(weizhi==4)
	{
		VectorPush(VectorToPingmu,32,zhi);
	}
	{
		push(weizhi,zhi); //ѹ���������ӷ���ָ�����
	}
}
void showrsp()
{
	char out[200]={0};
	sprintf(out,"begin;I:%d;Light:%d;Tmp:%d.%d;",g_dianliu,g_guangzhi,g_wendu/10,g_wendu%10);
	if(iserror())
	{
		strcat(out,"Stat:error;");
	}
	else
	{
		strcat(out,"Stat:ok;");
	}
	strcat(out,"end\n");
	print2(out);
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

void chuankou2jisuuan()
{
	char c=USART2->DR;
	if(c=='\n')
	{
		if(1==com2jiexi(rec2))
		{
			dealDiannaoOrder();
			showrsp();
		}
		else
		{
			print2("oder failed\n");
		}
		memset(rec2, 0, sizeof(rec2));
		weizhi2 = 0;
	}
	else
	{
		rec2[weizhi2++]=USART2->DR;
	}
}