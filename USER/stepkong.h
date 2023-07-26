#ifndef _STEPKONG_H
#define _STEPKONG_H

#include "IO.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"


#include "usmart.h"
 
 
 
#include "led.h"
#include "key.h"
#include "timer.h"
#include "io.h"
#include "pingmu.h"
#define yemazongshu 40
typedef struct
{
	char xshuru[4];
	char xdshuru[4];
	char  rule;//连续还是点动的问题.
	int  time;//延时1
	int time1;
	char yshuchu[4];
	char ydshuchu[4];
	char bushu;//代表哪一步..
}STEPENBER;//一个页面值的结构体定义

extern STEPENBER   stepmenber[yemazongshu];
extern char zhuangtaiflag;



char jisuan_xshuru(char k);
char jisuan_xdshuru(char k);
char jisuan_yshuchu(char k);
char jisuan_ydshuchu(char k);
char jisuan_step(char i);
void step_init(void);
void getstep(char i);//从0x0b开始,页码数量是10..
void upchengxu(char yeshu);
char downloadchengxu(void);
int yzhiget();
int xzhiget();
#endif
