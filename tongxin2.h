#ifndef _TONGXIN2_H
#define _TONGXIN2_H




typedef struct {
    int weizhi;
    int zhi;
    int falg;
} Alltongxininfo2;
extern void show2();
extern void push2(int weizhi,int zhi);
extern void pushshow2(int weizhi,int zhi);

int pop22(  Alltongxininfo2 *get);
#endif