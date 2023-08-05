#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tongxin2.h"
#define use 1
#define use 1
#define nouse 0


VectorInfo Vectorpingmu[maxzhi];
VectorInfo VectorDiannao[maxzhi];
VectorInfo VectorToPingmu[maxzhi];

int getemptyindex2(VectorInfo* Vectorbase)
{
    int i;
    for(i = 0; i < maxzhi; i++)
    {
        if(Vectorbase[i].falg == 0)
            return i;
    }
    return i;
}

int empty2(VectorInfo* Vectorbase)
{
    if(0 == getemptyindex2(Vectorbase))
    {
        return 1;
    }
    return 0;
}

int isfull2(VectorInfo* Vectorbase)
{
    if(maxzhi == getemptyindex2(Vectorbase))
    {
        return 1;
    }
    return 0;
}

void VectorPush(VectorInfo* Vectorbase, int weizhi, int zhi)
{
    int dizhi;
    int i, j, k;
    // printf("VectorPush %d %d",weizhi,zhi);
    dizhi = getemptyindex2(Vectorbase);
    // 有就更新
    for(i = 0; i < dizhi; i++)
    {
        if(Vectorbase[i].weizhi == weizhi)
        {
            Vectorbase[i].zhi = zhi;
            return;
        }
    }
    // 满了丢
    if(maxzhi == getemptyindex2(Vectorbase))
    {
        return;
    }
    // 入。
    Vectorbase[dizhi].weizhi = weizhi;
    Vectorbase[dizhi].zhi = zhi;
    Vectorbase[dizhi].falg = 1;
}

int VectorGet(VectorInfo* Vectorbase, VectorInfo* get)
{
    int i, j, k;
    int dizhi = getemptyindex2(Vectorbase);
    if(0 == getemptyindex2(Vectorbase))
    {
        return 0;
    }
    memcpy(get, &Vectorbase[0], sizeof(VectorInfo));
    for(i = 0; i < dizhi - 1; i++)
    {
        memcpy(&Vectorbase[i], &Vectorbase[i + 1], sizeof(VectorInfo));
    }
    // printf("VectorGet %d %d",get->weizhi,get->zhi);

    Vectorbase[dizhi - 1].falg = nouse;
    return 1;
}
