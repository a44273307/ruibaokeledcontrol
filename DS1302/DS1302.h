#ifndef __DS1302_H
#define __DS1302_H

#include "stm32f1xx_hal.h"

//DS1302_GPIO_CLK
#define DS1302_GPIO_CLK_ENABLE		__HAL_RCC_GPIOA_CLK_ENABLE()

//DS1302Òý½Å¶¨Òå
#define DS1302_CE_Pin			GPIO_PIN_8
#define DS1302_CE_Port		GPIOA

#define DS1302_DATA_Pin		GPIO_PIN_12
#define DS1302_DATA_Port	GPIOA

#define DS1302_SCLK_Pin		GPIO_PIN_11
#define DS1302_SCLK_Port	GPIOA

#define	DS1302_DATA_SET			HAL_GPIO_WritePin(DS1302_DATA_Port,DS1302_DATA_Pin,GPIO_PIN_SET)
#define DS1302_DATA_RESET		HAL_GPIO_WritePin(DS1302_DATA_Port,DS1302_DATA_Pin,GPIO_PIN_RESET)

#define	DS1302_CE_SET			HAL_GPIO_WritePin(DS1302_CE_Port,DS1302_CE_Pin,GPIO_PIN_SET)
#define DS1302_CE_RESET		HAL_GPIO_WritePin(DS1302_CE_Port,DS1302_CE_Pin,GPIO_PIN_RESET)

#define	DS1302_SCLK_SET			HAL_GPIO_WritePin(DS1302_SCLK_Port,DS1302_SCLK_Pin,GPIO_PIN_SET)
#define DS1302_SCLK_RESET		HAL_GPIO_WritePin(DS1302_SCLK_Port,DS1302_SCLK_Pin,GPIO_PIN_RESET)


int8_t DS1302Read(uint8_t cmd);
void DS1302Write(uint8_t cmd,uint8_t data);
void DS1302_Init(void );

uint8_t  GetSeconds(void );
void SetSconds(uint8_t Sconds);

uint8_t  GetMinutes(void );
void SetMinutes(uint8_t Sconds);

uint8_t  GetHour(void );
void SetHour(uint8_t Hour);

uint8_t  GetDate(void );
void SetDate(uint8_t Date);

uint8_t  GetMonth(void );
void SetMonth(uint8_t Month);

uint8_t  GetDay(void );
void SetDay(uint8_t Day);

uint8_t  GetYear(void );
void SetYear(uint8_t Year);

void Set_Year_Month_Date(uint8_t Year,uint8_t Month,uint8_t Date);
void Set_Hour_Minutes_Seconds(uint8_t Hour,uint8_t Minutes,uint8_t Seconds);

#endif
