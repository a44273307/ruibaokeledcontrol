#include "stepkong.h"
STEPENBER   stepmenber[yemazongshu];
char zhuangtaiflag=0;
char jisuan_xshuru(char k)
{
	STEPENBER onestep=stepmenber[k];
	char i,j;
	for(i=0;i<4;i++)
	{
		j=onestep.xshuru[i];//读入输入的量..
		if(j!=0)
		{
			if(xin[j]!=0)
				return(0);
		}
	}
	return(1);
}
char jisuan_xdshuru(char k)
{
	STEPENBER onestep=stepmenber[k];
		char i,j;
	for(i=0;i<4;i++)
	{
		j=onestep.xdshuru[i];//读入输入的量..
		if(j!=0)
		{
			if(xin[j]!=1)
				return(0);
		}
	}
	return(1);
}
char jisuan_yshuchu(char k)
{
	char i,j;
	STEPENBER onestep=stepmenber[k];
	for(i=0;i<4;i++)
	{
		j=onestep.yshuchu[i];//读入输入的量..
		if(j!=0)
		{
			yout_set(j,0);
		}
	}
	return(1);
}
char jisuan_ydshuchu(char k)
{
		char i,j;
	STEPENBER onestep=stepmenber[k];
	for(i=0;i<4;i++)
	{
		j=onestep.ydshuchu[i];//读入输入的量..
		if(j!=0)
		{
			yout_set(j,1);
		}
	}
	return(1);
}
int xzhiget()
{
	char j;
	int s=0;
	for(j=0;j<8;j++)
	{
		if (xin[j]==0)
		{
			s=s|(0x01<<(7-j));
		}
	}
	return(s);
}
int yzhiget()
{
	char j;
	int s=0;
	for(j=0;j<8;j++)
	{
		if (yout_get(j)==0)
		{
			s=s|(0x01<<(7-j));
		}
	}
	return(s);
}
char jisuan_step(char j)
{
		int i=0;
	int jj;
	while(1)
	{
		if(jisuan_xshuru(j)==1)
		if(jisuan_xdshuru(j)==1)
		{
			jisuan_yshuchu(j);
			jisuan_ydshuchu(j);
			break;
		}
		if(zhuangtaiflag>0)
		{
			yout_closeall();
			return(0);
		}
		delay_ms(2);
	}
	printf("success%d",j);
	return(1);
}
void step_init(void)
{
	char i,k;
	STEPENBER j;
	for(i=0;i<yemazongshu;i++)
	{
			j=stepmenber[i];
		  j.bushu=i;
			j.rule=0;
			j.time=0;
			j.time1=0;
		  for(k=0;k<4;k++)
		{
			j.xdshuru[k]=0;
			j.xshuru[k]=0;
			j.ydshuchu[k]=0;
			j.yshuchu[k]=0;
		}
	}
}
static int zhi[10];
void getstep(char i)//从0x0b开始,页码数量是10..
{
	
	int dizhi;
	char k,s;
		STEPENBER j;
		j=stepmenber[i];
	dizhi=0x0b+i*10;
	pingmu_readlong(dizhi,10,&zhi[0]);
	for(k=0;k<4;k++)
	{
		if(zhi[k]<255)
			j.xshuru[k]=zhi[k];
		else
			j.xdshuru[k]=0xffff-zhi[k];
	}
  j.time=zhi[4];
	j.rule=zhi[5];
	for(k=6;k<10;k++)
	{
				if(zhi[k]<255)
			j.yshuchu[k-6]=zhi[k];
		else
			j.ydshuchu[k-6]=0xffff-zhi[k];
	}
	stepmenber[i]=j;
}
char downloadchengxu(void)
{
	char i,j;
	long tt;
	for(i=0;i<100;i++)
	{
		getstep(i);
		tt=0;
		for(j=0;j<10;j++)
		{
			tt=tt+zhi[j];
		}
		if(tt==0)
			return(i);
	}
}

void upchengxu(char yeshu)
{
		char i,m,k;
	char dizhi;
		STEPENBER j;
	
	dizhi=0x0b+(i-1)*10;
	for(i=0;i<yeshu;i++)
	{
			j=stepmenber[i];
		for(m=0;m<4;m++)
		zhi[m]=0;
		zhi[4]=j.time;
		zhi[5]=j.rule;
		k=0;
		
		for(m=0;m<4;m++)
		{
			if(j.xshuru[m]!=0)
			{
				zhi[k]=j.xshuru[m];
				k++;
			}
			if(j.xdshuru[m]!=0)
			{
				zhi[k]=j.xdshuru[m];
				k++;
			}
		}
		k=6;
			for(m=6;m<10;m++)
		{
			if(j.yshuchu[m-6]!=0)
			{
				zhi[k]=j.yshuchu[m-6];
				k++;
			}
			if(j.ydshuchu[m-6]!=0)
			{
				zhi[k]=j.ydshuchu[m-6];
				k++;
			}
		}
		pingmu_writelong(dizhi,5,&zhi[0]);
		delay_ms(100);
		pingmu_writelong(dizhi+5,5,&zhi[5]);
		delay_ms(100);
	}
}