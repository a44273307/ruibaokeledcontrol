

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "chuankou.H" 


void sendbyte1(char x)
{
  HAL_UART_Transmit(&UART1_Handler,&x,1,0xFFFFFFF);
}
void sendbyte4(char c)//����4����һ���ֽڵĺ���
{
  HAL_UART_Transmit(&UART2_Handler,&c,1,0xffff);
}
void printf4(char *puts)
{
	 while(*puts) 
	{
		sendbyte4(*puts);
		puts++;
	}
}



