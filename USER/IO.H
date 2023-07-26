#ifndef _IO_H
#define _IO_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define X1 PBin(14)
#define X2 PBin(15)
#define X3 PBin(7)
#define X4 PBin(6)
#define X6 PBin(1)
#define X5 PAin(5)
#define X0 PAin(1)
#define X7 PAin(4)
#define X8 PCin(8)
#define X9 PCin(9)
#define X10 PCin(10)
#define X11 PCin(11)


#define Y0 PAout(8)
#define Y1 PAout(11)







#define Y2 PBout(5)
#define Y3 PBout(6)

#define Y4 PBout(13)
#define Y5 PBout(12)
#define Y6 PCout(15)
#define Y7 PAout(0)


#define Y8 PDout(8)
#define Y9 PDout(9)
#define Y10 PDout(10)
#define Y11 PDout(11)



#define Y12 yout[0]
#define Y13 yout[1]
#define Y14 yout[2]
#define Y15 yout[3]
#define Y16 yout[4]
#define Y17 yout[5]
#define Y18 yout[6]
#define Y19 yout[7]
#define Y20 yout[8]
#define Y21 yout[9]
#define Y22 yout[10]
#define Y23 yout[11]

void initio();
void shurulvbo(void);
char yout_set(char weizhi,char zhi);
char in_get(char weizhi);
extern char yout[32];
extern char xin[30];
u16 yout_change(void);//
void outzhi(u16 s);
void yout_closeall();
void xfenjie(u16 s);//将收到的数据值,分解成x12_x23
char in_allget();
char yout_get(char weizhi);
extern char xin[30];
#endif
