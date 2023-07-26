#include "bujindianji2.h"
#include "io.h"
static int weizhixinhao=0;
static int weizhinow=0;
static void stop()
{
	int i;
	for(i=4;i<8;i++)
	{
		yout_set(i,0);
	}
}
static void setstep(int step)
{
  stop();
	yout_set(step+4,1);
}
static void zheng()
{
	if(weizhinow>2048)
	{
		stop();
		return;
	}
	weizhinow++;
	weizhixinhao++;
	if(weizhixinhao>=4)
	{
		weizhixinhao=0;
	}
	setstep(weizhixinhao);
}
static void fan()
{
	if(weizhinow<0)
	{
		stop();
		return;
	}
	weizhinow--;
	weizhixinhao--;
	if(weizhixinhao<0)
	{
		weizhixinhao=3;
	}
	setstep(weizhixinhao);
}
void dianjirun2(int fangxiang)
{
	if(fangxiang==0)zheng();
	else fan();
}