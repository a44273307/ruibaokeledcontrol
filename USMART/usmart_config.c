#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"	 	
#include "sys.h"
#include "io.h"

#include "stepkong.h"
#include "ds18b20.h"
#include "ds18b201.h"
#include "main.h"
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
//	(void*)read_addr,"u32 read_addr(u32 addr)",
//	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	
//
//char changewait(char weizhi,char zhi);//0����watset;
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
//u8 DS18B20_Init(void);			//��ʼ��DS18B20
//short DS18B20_Get_Temp(void);	//��ȡ�¶�			
// void pingmu_write(int weizhi,int value);//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
//char yout_get(char weizhi);
//void pingmu_changepage(int value);			
//void upchengxu(char yeshu);			
//��0x0b��ʼ,ҳ��������10..			
//char jisuan_step(char i)			
					//void pingmu_bee(char i)
						//char in_allget()
			//	void outzhi(u16 s);void xfenjie(u16 s);//���յ�������ֵ,�ֽ��x12_x23
//		(void*)delay_xms,"void delay_xms(u16 nms)",
//			(void*)changewait,"char changewait(char weizhi,char zhi)",
//	 (void*)fuwei,"void fuwei(void)",	
//		 (void*)setout,"void setout(char weizhi,char zhi)",	
//			  (void*)changepage,"void changepage(int value)",
		
};						  
//void setout(char weizhi,char zhi)
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















