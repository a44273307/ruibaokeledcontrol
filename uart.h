#ifndef __UART_H_
#define __UART_H_

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

#define S3RI  0x01              //S3CON.0
#define S3TI  0x02              //S3CON.1
#define S3RB8 0x04              //S3CON.2
#define S3TB8 0x08              //S3CON.3

#define		TI4					(S4CON & 2) != 0	/* 判断TI3是否发送完成							     */
#define		RI4					(S4CON & 1) != 0	/* 判断RI3是否接收完成							     */
#define		SET_TI4()			S4CON |=  2			/* 设置TI3(引起中断)							     */
#define		CLR_TI4()			S4CON &= ~2			/* 清除TI3										     */
#define		CLR_RI4()			S4CON &= ~1			/* 清除RI3										     */

#define S4RI  0x01              //S4CON.0
#define S4TI  0x02              //S4CON.1
#define BAUD9600   0   //波特率9600
#define BAUD115200 1   //波特率115200
#define MAIN_Fosc 24000000L          //系统频率
 extern bit busy4;
 extern bit busy2;
 extern bit busy3;

void Timer0Init(void);
void sendbyte1(unsigned char ch);
void UartInit(void);
void Uart23Init(void);
void Uart4Init(void);
void SendData(unsigned char ch);
void sendbyte2(unsigned char ch);
void sendbyte3(unsigned char ch);
void sendbyte4(unsigned char ch);

void PrintString(const char *puts);
void print3(char *p);
void print4(char *p);
#endif
