#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tongxin.h"
#define use 1
#define use 1
#define nouse 0
#define maxzhi 20 

Alltongxininfo tongxindata[maxzhi];
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
    int i,j,k;
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
    int i,j,k;
    int dizhi=getemptyindex();
    for(i=0;i<dizhi;i++)
    {
        printf("dizhi[%d]-[%d]\n",tongxindata[i].weizhi,tongxindata[i].zhi);
    }
}
void pushshow(int weizhi,int zhi)
{
    push(weizhi,zhi);
    show();
}
void deal(Alltongxininfo get)
{
    printf("dizhi[%d]-[%d]-[%d]\n",get.weizhi,get.zhi,get.falg);

}
void* xmemcpy(void* destination, const void* source, size_t num) 
{
size_t i;   
	char* dest = (char*)destination;
    const char* src = (const char*)source;

    for ( i = 0; i < num; i++) {
        dest[i] = src[i];
    }

    return destination;
}

int pop()
{
    int i,j,k;
    int dizhi=getemptyindex();
    Alltongxininfo get;
    if(0==getemptyindex())
    {
        return 0;
    }
    xmemcpy(&get,&tongxindata[0],sizeof(Alltongxininfo));
    for(i=0;i<dizhi-1;i++)
    {
        xmemcpy(&tongxindata[i],&tongxindata[i+1],sizeof(Alltongxininfo));
    }   
    return 1;
}

int pop2(Alltongxininfo *get)
{
    int i;
    int dizhi=getemptyindex();
    if(0==dizhi)
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