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
	char  rule;//�������ǵ㶯������.
	int  time;//��ʱ1
	int time1;
	char yshuchu[4];
	char ydshuchu[4];
	char bushu;//������һ��..
}STEPENBER;//һ��ҳ��ֵ�Ľṹ�嶨��

extern STEPENBER   stepmenber[yemazongshu];
extern char zhuangtaiflag;



char jisuan_xshuru(char k);
char jisuan_xdshuru(char k);
char jisuan_yshuchu(char k);
char jisuan_ydshuchu(char k);
char jisuan_step(char i);
void step_init(void);
void getstep(char i);//��0x0b��ʼ,ҳ��������10..
void upchengxu(char yeshu);
char downloadchengxu(void);
int yzhiget();
int xzhiget();
#endif
