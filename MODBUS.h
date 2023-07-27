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

extern uint8_t Modbus_Send_Buff[Modbus_Max_Send_Buff];                //�������ݻ�����
extern uint8_t Modbus_Rcv_Buff[Modbus_Max_Rcv_Buff];                //�������ݻ�����
void HAL_UART_Transmit_DMA_485(int *huart,uint8_t *pData, uint16_t Size);
void Modbus_Init(void);
char cuncu_485();
void init_485();
extern volatile int HoldingReg[10];
void chuankou1jisuuan(unsigned char ans);
extern void time1msjisuan();
void jishouokjisuan();




#define write_485_predaowei HoldingReg[3]//���п���Ԥ��
#define write_485_kongzhi HoldingReg[4]//���п��ƿ���
#define write_shuju2 HoldingReg[5]//���..


#define kz485fuwei HoldingReg[6]//��λ..


#define kz485pre HoldingReg[7]//��λ..
#define kz485thing HoldingReg[8]//��λ..
#define kz485style HoldingReg[9]//��λ..
#define kz485power HoldingReg[0x0A]//��λ..
#define kz485error HoldingReg[0x0B]//��λ..
#define kz485add HoldingReg[0x0C]//��λ..
#define kz485low HoldingReg[0x0D]//��λ..



#define prekz485 HoldingReg[0x0e]//����..
#define kz485daowei HoldingReg[0x10]//����..



#define busLEdon HoldingReg[1]
//��ѹ 2-3
//����  4-5








extern int recover;

#endif



