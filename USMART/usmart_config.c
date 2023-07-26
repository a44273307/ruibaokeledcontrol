#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"	 	
#include "sys.h"
#include "io.h"

#include "stepkong.h"
#include "ds18b20.h"
#include "ds18b201.h"
#include "main.h"
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
//	(void*)read_addr,"u32 read_addr(u32 addr)",
//	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	
//
//char changewait(char weizhi,char zhi);//0设置watset;
//char deng(char *s,char zhi,long time);	
//			void upshuju(int weizhi,int value); 
//		void changepage(int value);
//		void bee(char i);
#endif	   
	//char in_get(char weizhi)
	(void*)delay_ms,"void delay_ms(u16 nms)",
(void*)delay_us,"void delay_us(u32 nus)",
(void*)fuwei,"void fuwei(void)",
(void*)settime,"void settime(int zhi)",
(void*)yout_set,"char yout_set(char weizhi,char zhi)",
	(void*)yout_get,"char yout_get(char weizhi)",
						//				char yout_set(char weizhi,char zhi);
			//	char yout_get(char weizhi);						
	//void settime(int zhi);
//void pwmchange(int i);
//void TIM_SetTIM1Compare1(u32 compare);
//u8 DS18B20_Init(void);			//初始化DS18B20
//short DS18B20_Get_Temp(void);	//获取温度			
// void pingmu_write(int weizhi,int value);//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
//char yout_get(char weizhi);
//void pingmu_changepage(int value);			
//void upchengxu(char yeshu);			
//从0x0b开始,页码数量是10..			
//char jisuan_step(char i)			
					//void pingmu_bee(char i)
						//char in_allget()
			//	void outzhi(u16 s);void xfenjie(u16 s);//将收到的数据值,分解成x12_x23
//		(void*)delay_xms,"void delay_xms(u16 nms)",
//			(void*)changewait,"char changewait(char weizhi,char zhi)",
//	 (void*)fuwei,"void fuwei(void)",	
//		 (void*)setout,"void setout(char weizhi,char zhi)",	
//			  (void*)changepage,"void changepage(int value)",
		
};						  
//void setout(char weizhi,char zhi)
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















