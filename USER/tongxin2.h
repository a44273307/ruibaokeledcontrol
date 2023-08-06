#ifndef _TONGXIN2_H
#define _TONGXIN2_H


#define maxzhi 20 

typedef struct {
    int weizhi;
    int zhi;
    int falg;
} VectorInfo;


extern VectorInfo Vectorpingmu[maxzhi];
extern VectorInfo VectorDiannao[maxzhi];
extern VectorInfo VectorToPingmu[maxzhi];


int getemptyindex2(VectorInfo* Vectorpingmu);
int VectorIsEmpty(VectorInfo* Vectorpingmu);
int isfull2(VectorInfo* Vectorpingmu);



void VectorPush(VectorInfo* Vectorpingmu, int weizhi, int zhi);
int VectorGet(VectorInfo* Vectorpingmu, VectorInfo* get);












#endif