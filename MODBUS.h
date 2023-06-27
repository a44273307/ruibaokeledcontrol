#ifndef _MODBUS_H_
#define _MODBUS_H_


#include "comhead.h"


extern void Modbus_ClearBuff();
extern void chuankou1jisuuan(unsigned char ans);
extern unsigned short Modbus_CRC16(unsigned char *puchMsg, unsigned char usDataLen);


extern volatile unsigned short HoldingReg[100];
extern void time1msjisuan();

#define regdianya HoldingReg[3]
#define regdianliu HoldingReg[4]
#define reglight   HoldingReg[5]  //光敏电阻的值。。

// 温度值   6




#endif



