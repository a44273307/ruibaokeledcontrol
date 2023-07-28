
#include "stc32g.h"
#include "config.h"


#include <stdio.h>
#include "tongxin.h"
#include "func.h"
#define use 1
#define use 1
#define nouse 0
#define maxzhi 10 

static Alltongxininfo tongxindata[maxzhi];

int  getemptyindex()
{
    int i;
    for(i=0;i<maxzhi;i++)
    {
        if(tongxindata[i].falg==0)
        return i;
    }
    return i;
}
int empty()
{
    if(0==getemptyindex())
    {
        return 1;
    }
    return 0;
}
int isfull()
{
    if(maxzhi==getemptyindex())
    {
        return 1;
    }
    return 0;
}

void push(int weizhi,int zhi)
{
    int dizhi;
    int i;
    dizhi=getemptyindex();
    // 有就更新
    for(i=0;i<dizhi;i++)
    {
        if(tongxindata[i].weizhi==weizhi)
        {
            tongxindata[i].zhi=zhi;
            return;
        }
    }
    // 满了丢
    if(maxzhi==getemptyindex())
    {
        return ;
    }
    // 入。
    // dizhi=dizhi;
    tongxindata[dizhi].weizhi=weizhi;
    tongxindata[dizhi].zhi=zhi;
    tongxindata[dizhi].falg=1;
}
void show()
{
    int i;
    int dizhi=getemptyindex();
    for(i=0;i<dizhi;i++)
    {
     //   printf("dizhi[%d]-[%d]\n",tongxindata[i].weizhi,tongxindata[i].zhi);
    }
}
void pushshow(int weizhi,int zhi)
{
    push(weizhi,zhi);
    show();
}
void deal(Alltongxininfo get)
{
    

}
int pop()
{
    int i;
    int dizhi=getemptyindex();
    Alltongxininfo get;
    if(0==getemptyindex())
    {
        return 0;
    }
    memcpy(&get,&tongxindata[0],sizeof(Alltongxininfo));
    for(i=0;i<dizhi-1;i++)
    {
        memcpy(&tongxindata[i],&tongxindata[i+1],sizeof(Alltongxininfo));
    }
    return 1;
}

int pop2(Alltongxininfo *get)
{
    int i;
    int dizhi=getemptyindex();
    if(0==getemptyindex())
    {
        return 0;
    }
    memcpy(get,&tongxindata[0],sizeof(Alltongxininfo));
    for(i=0;i<dizhi-1;i++)
    {
        memcpy(&tongxindata[i],&tongxindata[i+1],sizeof(Alltongxininfo));
    }
    tongxindata[dizhi-1].falg=nouse;
    return 1;
}



