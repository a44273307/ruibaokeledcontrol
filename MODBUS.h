#ifndef _MODBUS_H_
#define _MODBUS_H_





extern void Modbus_ClearBuff();
extern void chuankou1jisuuan(unsigned char ans);



extern volatile unsigned short HoldingReg[100];
extern void time1msjisuan();

#define regdianya HoldingReg[3]
#define regdianliu HoldingReg[4]
#define reglight   HoldingReg[5]





#endif



