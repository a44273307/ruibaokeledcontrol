/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"


CONFIG_MSG  ConfigMsg;																	/*���ýṹ��*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM�洢��Ϣ�ṹ��*/
SPI_HandleTypeDef hspi_w5500;

/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*����Ĭ��IP��Ϣ*/
uint8 local_ip[4]  ={169,254,24,123};										/*����W5500Ĭ��IP��ַ*/
uint8 subnet[4]    ={255,255,0,0};										/*����W5500Ĭ����������*/
uint8 gateway[4]   ={169,254,24,1};											/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};									/*����W5500Ĭ��DNS*/

uint16 local_port=5000;	                       					/*���屾�ض˿�*/

/*����Զ��IP��Ϣ*/
uint8  remote_ip[4]={192,168,3,230};			  						/*Զ��IP��ַ*/
uint16 remote_port=8000;																/*Զ�˶˿ں�*/

/*IP���÷���ѡ��������ѡ��*/
uint8	ip_from=IP_FROM_EEPROM;				

uint8         dhcp_ok   = 0;													   			/*dhcp�ɹ���ȡIP*/
uint32	      ms        = 0;															  	/*�������*/
uint32	      dhcp_time = 0;															  	/*DHCP���м���*/
__IO uint8_t  ntptimer  = 0;															  	/*NPT�����*/

/**
*@brief		Ӳ����λW5500
*@param		��
*@return	��
*/
void reset_w5500(void)
{
  HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN,GPIO_PIN_RESET);
  delay_ms(10);  
  HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN,GPIO_PIN_SET);
  delay_ms(1600);
}

/**
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{		
   /*���ƶ����������Ϣ�����ýṹ��*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
		printf(" ʹ�ö����IP��Ϣ����W5500\r\n");
	
	/*ʹ��EEPROM�洢��IP����*/	
	if(ip_from==IP_FROM_EEPROM)
	{
		/*��EEPROM�ж�ȡIP������Ϣ*/
		read_config_from_eeprom();		
		
		/*�����ȡEEPROM��MAC��Ϣ,��������ã����ʹ��*/		
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			printf(" IP from EEPROM\r\n");
			/*����EEPROM������Ϣ�����õĽṹ�����*/
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
		}
		else
		{
			printf(" EEPROMδ����,ʹ�ö����IP��Ϣ����W5500,��д��EEPROM\r\n");
			write_config_to_eeprom();	/*ʹ��Ĭ�ϵ�IP��Ϣ������ʼ��EEPROM������*/
		}			
	}

	/*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*����DHCP��ȡ��������Ϣ�����ýṹ��*/
		if(dhcp_ok==1)
		{
			printf(" IP from DHCP\r\n");		 
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		{
			printf(" DHCP�ӳ���δ����,���߲��ɹ�\r\n");
			printf(" ʹ�ö����IP��Ϣ����W5500\r\n");
		}
	}
		
	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	printf(" W5500 IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void gpio_for_w5500_config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  WIZ_SPIx_RCC_CLK_ENABLE();
  WIZ_SPI_GPIO_ClK_ENABLE();
  WIZ_SPIx_SCS_CLK_ENABLE();
	WIZ_INT_CLK_ENABLE();
  WIZ_RESET_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = WIZ_SPIx_SCLK_PIN|WIZ_SPIx_MOSI_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = WIZ_SPIx_MISO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = WIZ_SPIx_SCS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStruct);

  /* Disable the Serial Wire Jtag Debug Port SWJ-DP */
  //__HAL_AFIO_REMAP_SWJ_DISABLE();

  hspi_w5500.Instance = WIZ_SPIx;
  hspi_w5500.Init.Mode = SPI_MODE_MASTER;
  hspi_w5500.Init.Direction = SPI_DIRECTION_2LINES;
  hspi_w5500.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi_w5500.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi_w5500.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi_w5500.Init.NSS = SPI_NSS_SOFT;
  hspi_w5500.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi_w5500.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi_w5500.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi_w5500.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi_w5500.Init.CRCPolynomial = 7;
  HAL_SPI_Init(&hspi_w5500);
	
  /*����INT����*/	
  GPIO_InitStruct.Pin = WIZ_INT_PIN;                  
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStruct);

  /*����RESET����*/
  HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = WIZ_RESET_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(WIZ_RESET_PORT, &GPIO_InitStruct);
}

/**
*@brief		W5500Ƭѡ�ź����ú���
*@param		val: Ϊ��0����ʾƬѡ�˿�Ϊ�ͣ�Ϊ��1����ʾƬѡ�˿�Ϊ��
*@return	��
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
	  HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_RESET);
	}
	else if (val == HIGH)
	{
	  HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_SET);
	}
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

uint8_t SPI_SendByte(uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&hspi_w5500,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=0XFF;
  
  return d_read; 
}

/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data��д���8λ����
*@return	��
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}

/**
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data����д��ĵ�ַ����ȡ����8λ����
*@return	��
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();                               
   return data;    
}

/**
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}

/**
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}

/**
*@brief		д������Ϣ��EEPROM
*@param		��
*@return	��
*/

typedef uint32_t  u32;
typedef uint16_t u16;
#define FLASH_SAVE_ADDR2  0X08021000
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void write_config_to_eeprom(void)
{
  int len=sizeof(EEPROM_MSG);
  STMFLASH_Write(FLASH_SAVE_ADDR2,(u16 *)ConfigMsg.mac,EEPROM_MSG_LEN/2);
  // EEPROM_WriteBytes(ConfigMsg.mac,dAddr,(uint8)EEPROM_MSG_LEN/2);				
  delay_ms(10);																							
}

/**
*@brief		��EEPROM��������Ϣ
*@param		��
*@return	��
*/
void read_config_from_eeprom(void)
{
  STMFLASH_Read(FLASH_SAVE_ADDR2,(u16 *)EEPROM_MSG.mac,EEPROM_MSG_LEN/2);
  // STMFLASH_Write(ConfigMsg.mac,dAddr,(uint8)EEPROM_MSG_LEN/2);
  	// EEPROM_ReadBytes(EEPROM_MSG.mac,0,EEPROM_MSG_LEN);
  	delay_us(10);
}

/**
*@brief		STM32��ʱ��2��ʼ��
*@param		��
*@return	��
*/
void timer2_init(void)
{
	
}

/**
  * ��������: ������ģʽ�¶�ʱ���Ļص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  timer2_isr();
}

/**
*@brief		dhcp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void dhcp_timer_init(void)
{
  timer2_init();																	
}

/**
*@brief		ntp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void ntp_timer_init(void)
{
  timer2_init();																	
}

/**
*@brief		��ʱ��2�жϺ���
*@param		��
*@return	��
*/
void timer2_isr(void)
{
  ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP��ʱ��1S*/
	  #ifndef	__NTP_H__
	  ntptimer++;																						/*NTP����ʱ���1S*/
	  #endif
  }
}
/**
*@brief		STM32ϵͳ����λ����
*@param		��
*@return	��
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" ϵͳ�����С���\r\n");
  JumpAddress = *(__IO uint32_t*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

