#include "chengxu_wirteup.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "io.h"
#include "pingmu.h"
#include "stepkong.h"
#include "stmflash.h"

u16 stepzhi[40*20];
char readeeprom()
{
	int i,j,k,s;
	int step;
	long sum;
	STEPENBER onestep;
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)stepzhi,40*20);
	for(i=0;i<40;i++)
	{
		k=i*20;
		onestep=stepmenber[i];
		for(j=0;j<4;j++)
		{
			onestep.xshuru[j]=stepzhi[k];
			k++;
		}
		for(j=0;j<4;j++)
		{
			onestep.xdshuru[j]=stepzhi[k];
			k++;
		}
		s=stepzhi[k]*256;
		k++;
			onestep.time=stepzhi[k]+s;
		k++;
		onestep.rule=stepzhi[k];
		k++;
		for(j=0;j<4;j++)
		{
			onestep.yshuchu[j]=stepzhi[k];
			k++;
		}
		for(j=0;j<4;j++)
		{
			onestep.ydshuchu[j]=stepzhi[k];
			k++;
		}
		k=i*20;
		sum=0;
		for(j=0;j<20;j++)
		{
			sum=sum+stepzhi[j];
		}
		if(sum==0xff*20)
		{
			return(i);
		}
	}
}
char writeeeprom_onestep(char yeshu,long weizhi)
{
	u32 dizhi;
	int i,j,k;
	int step;
	STEPENBER onestep;
	onestep=stepmenber[yeshu]; 
	dizhi=FLASH_SAVE_ADDR+weizhi;
			for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.xshuru[j]; 
			k++;
		}
			for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.xdshuru[j];
			k++;
		}
		stepzhi[k]=onestep.time/256;
		k++;
		stepzhi[k]=onestep.time%256;
		k++;
	  stepzhi[k]=onestep.rule;
		k++;
		for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.yshuchu[j];
			k++;
		}
		for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.ydshuchu[j];
			k++;
		}
		k=20;
    STMFLASH_Write(dizhi,(u16*)stepzhi,k);//把数据写到eeprom里面..		
}
void writeeeprom_total(char yeshu)
{
	int i,j,k;
	int step;
	STEPENBER onestep;
	long sum;
	for(i=0;i<40;i++)
	{
		k=i*20;
		onestep=stepmenber[i];  
		for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.xshuru[j]; 
			k++;
		}
		for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.xdshuru[j];
			k++;
		}                           
		stepzhi[k]=onestep.time/256;
		k++;
		stepzhi[k]=onestep.time%256;
		k++;
	  stepzhi[k]=onestep.rule;
		k++;
		for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.yshuchu[j];
			k++;
		}
		for(j=0;j<4;j++)
		{
			stepzhi[k]=onestep.ydshuchu[j];
			k++;
		}
		k=i*20;
		sum=0;
		for(j=0;j<20;j++)
		{
			sum=sum+stepzhi[j];
		}
		if(sum==0xff*20)
		{
	    break;
		}
	}
	j=1;
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)stepzhi,k);//把数据写到eeprom里面..
}