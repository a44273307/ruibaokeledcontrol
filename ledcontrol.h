#ifndef _ledcontrol_H_
#define _ledcontrol_H_
#include "comhead.h"

extern void Init_BH1750();


extern void Single_Write_BH1750(uchar REG_Address);
extern int Multiple_read_BH1750();
#endif
