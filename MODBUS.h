#ifndef _MODBUS_H_
#define _MODBUS_H_




typedef unsigned char uint8_t;
typedef unsigned char u8;
typedef unsigned short uint16_t;
#define Modbus_Max_Send_Buff	30

#define len_HoldingReg	10
#define Modbus_Max_Rcv_Buff	10
void Modbus_ReadHoldingReg_Process(void);
void Modbus_WriteSingleReg_Process(void);
void Modbus_WriteMultipleReg_Process(void);
void Modbus_ErrorHandling(uint8_t ErrorType);
void Modbus_ClearBuff(void);

extern uint8_t Modbus_Send_Buff[Modbus_Max_Send_Buff];                //发送数据缓冲区
extern uint8_t Modbus_Rcv_Buff[Modbus_Max_Rcv_Buff];                //接收数据缓冲区
void HAL_UART_Transmit_DMA_485(int *huart,uint8_t *pData, uint16_t Size);
void Modbus_Init(void);
char cuncu_485();
void init_485();
extern volatile int HoldingReg[10];
void chuankou1jisuuan(unsigned char ans);
extern void time1msjisuan();
void jishouokjisuan();




#define write_485_predaowei HoldingReg[3]//运行控制预设
#define write_485_kongzhi HoldingReg[4]//运行控制控制
#define write_shuju2 HoldingReg[5]//清错..


#define kz485fuwei HoldingReg[6]//复位..


#define kz485pre HoldingReg[7]//复位..
#define kz485thing HoldingReg[8]//复位..
#define kz485style HoldingReg[9]//复位..
#define kz485power HoldingReg[0x0A]//复位..
#define kz485error HoldingReg[0x0B]//复位..
#define kz485add HoldingReg[0x0C]//复位..
#define kz485low HoldingReg[0x0D]//复位..



#define prekz485 HoldingReg[0x0e]//运行..
#define kz485daowei HoldingReg[0x10]//运行..



#define busLEdon HoldingReg[1]
//电压 2-3
//电流  4-5








extern int recover;

#endif



