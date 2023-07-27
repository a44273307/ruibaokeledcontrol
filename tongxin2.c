


#include <stdio.h>
#include "tongxin2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define use 1
#define use 1
#define nouse 0
#define maxzhi 10 

static Alltongxininfo2 tongxindata2[maxzhi];

int  getemptyindex2()
{
    int i;
    for(i=0;i<maxzhi;i++)
    {
        if(tongxindata2[i].falg==0)
        return i;
    }
    return i;
}
int empty2()
{
    if(0==getemptyindex2())
    {
        return 1;
    }
    return 0;
}
int isfull2()
{
    if(maxzhi==getemptyindex2())
    {
        return 1;
    }
    return 0;
}

void push2(int weizhi,int zhi)
{
    int dizhi;
    int i;
    dizhi=getemptyindex2();
    // 有就更新
    for(i=0;i<dizhi;i++)
    {
        if(tongxindata2[i].weizhi==weizhi)
        {
            tongxindata2[i].zhi=zhi;
            return;
        }
    }
    // 满了丢
    if(maxzhi==getemptyindex2())
    {
        return ;
    }
    // 入。
    // dizhi=dizhi;
    tongxindata2[dizhi].weizhi=weizhi;
    tongxindata2[dizhi].zhi=zhi;
    tongxindata2[dizhi].falg=1;
}
int pop22(Alltongxininfo2 *get)
{
    int i;
    int dizhi=getemptyindex2();
    if(0==getemptyindex2())
    {
        return 0;
    }
    memcpy(get,&tongxindata2[0],sizeof(Alltongxininfo2));
    for(i=0;i<dizhi-1;i++)
    {
        memcpy(&tongxindata2[i],&tongxindata2[i+1],sizeof(Alltongxininfo2));
    }
    tongxindata2[dizhi-1].falg=nouse;
    return 1;
}



