
#ifndef		__PINGMU_H
#define		__PINGMU_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
 char readonepage(int step);
 void changepage(char i);//?????1.?
 void pingmu_bee(char i);
void pingmu_changepage(int value);//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
 void pingmu_write(int weizhi,int value);//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
  char pingmu_readlong(int dizhi,int length,int *s);//a�ǵ�ַ,length����,s����ֵ..
 void pingmu_writelong(int dizhi,int length,int *s);//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
 char getpingmustate(void);
 void sendbyte4(u8 c);//����4����һ���ֽڵĺ���
 int pingmu_read(int a);//a�ǵ�ַ,����ĳ����ַ��ֵ,���ҷ���..


  //��ʾ���ֵĳ���.
 void pm_hanzhi(int dizhi,char *s);
 #define  S4BUF USART2->DR //����2����Ļͨ��
#endif