
#ifndef		__PINGMU_H
#define		__PINGMU_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
 char readonepage(int step);
 void changepage(char i);//?????1.?
 void pingmu_bee(char i);
void pingmu_changepage(int value);//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
 void pingmu_write(int weizhi,int value);//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
  char pingmu_readlong(int dizhi,int length,int *s);//a是地址,length长度,s保存值..
 void pingmu_writelong(int dizhi,int length,int *s);//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
 char getpingmustate(void);
 void sendbyte4(u8 c);//串口4发送一个字节的函数
 int pingmu_read(int a);//a是地址,下载某个地址的值,并且返回..


  //显示汉字的程序.
 void pm_hanzhi(int dizhi,char *s);
 #define  S4BUF USART2->DR //串口2和屏幕通信
#endif