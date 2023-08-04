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


int getemptyindex2(VectorInfo* Vectorpingmu)
{
    int i;
    for(i = 0; i < maxzhi; i++)
    {
        if(Vectorpingmu[i].falg == 0)
            return i;
    }
    return i;
}

int empty2(VectorInfo* Vectorpingmu)
{
    if(0 == getemptyindex2(Vectorpingmu))
    {
        return 1;
    }
    return 0;
}

int isfull2(VectorInfo* Vectorpingmu)
{
    if(maxzhi == getemptyindex2(Vectorpingmu))
    {
        return 1;
    }
    return 0;
}

void VectorPush(VectorInfo* Vectorpingmu, int weizhi, int zhi)
{
    int dizhi;
    int i, j, k;
    dizhi = getemptyindex2(Vectorpingmu);
    // 有就更新
    for(i = 0; i < dizhi; i++)
    {
        if(Vectorpingmu[i].weizhi == weizhi)
        {
            Vectorpingmu[i].zhi = zhi;
            return;
        }
    }
    // 满了丢
    if(maxzhi == getemptyindex2(Vectorpingmu))
    {
        return;
    }
    // 入。
    Vectorpingmu[dizhi].weizhi = weizhi;
    Vectorpingmu[dizhi].zhi = zhi;
    Vectorpingmu[dizhi].falg = 1;
}

int VectorGet(VectorInfo* Vectorpingmu, VectorInfo* get)
{
    int i, j, k;
    int dizhi = getemptyindex2(Vectorpingmu);
    if(0 == getemptyindex2(Vectorpingmu))
    {
        return 0;
    }
    memcpy(get, &Vectorpingmu[0], sizeof(VectorInfo));
    for(i = 0; i < dizhi - 1; i++)
    {
        memcpy(&Vectorpingmu[i], &Vectorpingmu[i + 1], sizeof(VectorInfo));
    }
    Vectorpingmu[dizhi - 1].falg = nouse;
    return 1;
}



