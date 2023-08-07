#ifndef _TONGXIN_H
#define _TONGXIN_H




typedef struct {
    int weizhi;
    int zhi;
    int falg;
} Alltongxininfo;
extern void show();
extern void push(int weizhi,int zhi);
extern void pushshow(int weizhi,int zhi);
extern int pop();
extern int pop2(  Alltongxininfo *get);
extern int empty();
#endif