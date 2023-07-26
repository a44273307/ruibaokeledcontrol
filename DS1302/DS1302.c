#include "DS1302.h"
extern void  delay_us(uint16_t Counter );
//DS302_CE端口初始化
void DS1302_CE_Init()
{
	//定义DS1302_CE_IO结构体
	GPIO_InitTypeDef DS1302_CE_InitStruct = {0};
	HAL_GPIO_WritePin (DS1302_CE_Port ,DS1302_CE_Pin ,GPIO_PIN_RESET );
	//IO状态定义
	DS1302_CE_InitStruct .Pin 	= DS1302_CE_Pin;
	DS1302_CE_InitStruct .Mode 	= GPIO_MODE_OUTPUT_PP ;
	DS1302_CE_InitStruct .Pull 	= GPIO_NOPULL ;
	DS1302_CE_InitStruct .Speed = GPIO_SPEED_FREQ_HIGH ;
	//DS1302_CE初始化
	HAL_GPIO_Init (DS1302_CE_Port ,&DS1302_CE_InitStruct );
}


//DS1302_SCLK端口初始化
void DS1302_SCLK_Init()
{
	//定义DS1302_SCLK_IO结构体
	GPIO_InitTypeDef DS1302_SCLK_InitStruct = {0};
	HAL_GPIO_WritePin (DS1302_SCLK_Port ,DS1302_SCLK_Pin ,GPIO_PIN_RESET );
	//IO状态定义
	DS1302_SCLK_InitStruct .Pin 	= DS1302_SCLK_Pin;
	DS1302_SCLK_InitStruct .Mode 	= GPIO_MODE_OUTPUT_PP ;
	DS1302_SCLK_InitStruct .Pull 	= GPIO_NOPULL ;
	DS1302_SCLK_InitStruct .Speed = GPIO_SPEED_FREQ_HIGH ;
	//DS1302_SCLK初始化
	HAL_GPIO_Init (DS1302_SCLK_Port ,&DS1302_SCLK_InitStruct );
	
}

//DS1302_DATA输入模式
void DS1302_DATA_INPUT_MODE()
{
	//定义DS1302_DATA_IO结构体
	GPIO_InitTypeDef DS1302_DATA_InitStruct = {0};
	//IO状态定义
	DS1302_DATA_InitStruct .Pin 	= DS1302_DATA_Pin;
	DS1302_DATA_InitStruct .Mode 	= GPIO_MODE_INPUT ;
	DS1302_DATA_InitStruct .Pull 	= GPIO_NOPULL ;
	DS1302_DATA_InitStruct .Speed = GPIO_SPEED_FREQ_HIGH ;
	//DS1302_DATA初始化
	HAL_GPIO_Init (DS1302_DATA_Port ,&DS1302_DATA_InitStruct );
	
}

//DS1302_DATA输出模式
void DS1302_DATA_OUTPUT_MODE()
{
	//定义DS1302_DATA_IO结构体
	GPIO_InitTypeDef DS1302_DATA_InitStruct = {0};
	HAL_GPIO_WritePin (DS1302_DATA_Port ,DS1302_DATA_Pin ,GPIO_PIN_RESET );
	//IO状态定义
	DS1302_DATA_InitStruct .Pin 	= DS1302_DATA_Pin;
	DS1302_DATA_InitStruct .Mode 	= GPIO_MODE_OUTPUT_PP ;
	DS1302_DATA_InitStruct .Pull 	= GPIO_NOPULL ;
	DS1302_DATA_InitStruct .Speed = GPIO_SPEED_FREQ_HIGH ;
	//DS1302_DATA初始化
	HAL_GPIO_Init (DS1302_DATA_Port ,&DS1302_DATA_InitStruct );
}

void DS1302_Init()
{
	DS1302_GPIO_CLK_ENABLE;
	
	DS1302_CE_Init() ;
	DS1302_SCLK_Init ();
	DS1302_DATA_OUTPUT_MODE ();
}

void DS1302WriteByte(uint8_t data)
{
	DS1302_DATA_OUTPUT_MODE();
	
	uint8_t i;
	DS1302_SCLK_RESET ;
	delay_us (1);
	for(i=0;i<8;i++)
	{
		DS1302_SCLK_RESET ;
		if((data&0x01)==1)
			DS1302_DATA_SET ;
		else
			DS1302_DATA_RESET ;
		delay_us (1);
		DS1302_SCLK_SET ;
		delay_us  (1);
		data>>=1;
	}
	
}


uint8_t DS1302ReadByte()
{
	DS1302_DATA_INPUT_MODE ();
	
	uint8_t i,data=0;
	delay_us (1);
	for(i=0;i<8;i++)
	{
		DS1302_SCLK_SET ;
		delay_us (1);
		DS1302_SCLK_RESET ;
		delay_us (1);
		data >>=1;
		if(HAL_GPIO_ReadPin (DS1302_DATA_Port ,DS1302_DATA_Pin )==1)
			data |=0x80;
		
	}
	DS1302_SCLK_RESET ;
	DS1302_CE_RESET ;
	DS1302_DATA_OUTPUT_MODE() ;
	
	return data ;
	
}


int8_t DS1302Read(uint8_t cmd)
{
	uint8_t data;
	DS1302_CE_RESET ;
	DS1302_SCLK_RESET ;
	
	DS1302_CE_SET ;
	DS1302WriteByte (cmd );
	data = DS1302ReadByte ();
	
	DS1302_SCLK_RESET ;
	DS1302_CE_RESET ;
	DS1302_DATA_RESET ;
	
	return (data );
	
}

void DS1302Write(uint8_t cmd,uint8_t data)
{
	DS1302_CE_RESET ;
	DS1302_SCLK_RESET ;
	
	DS1302_CE_SET ;
	DS1302WriteByte (cmd );
	
	DS1302WriteByte (data );
	
	DS1302_SCLK_RESET ;
	DS1302_CE_RESET ;
	DS1302_DATA_RESET ;
	
}


uint8_t  GetSeconds(void )
{
	uint8_t Sconds_H,Sconds_L,Sconds = 0;
	Sconds = DS1302Read (0x81);
	Sconds_L = Sconds & 0x0f;
	Sconds = Sconds >> 4;
	Sconds_H = Sconds & 0x07;
	Sconds = 10 * Sconds_H +Sconds_L ;
	return Sconds ;
}

void SetSconds(uint8_t Sconds)
{
	uint8_t Sconds_H,Sconds_L;
	Sconds_L = Sconds %10;
	Sconds_H = Sconds /10;
	Sconds = Sconds_H << 4;
	Sconds = Sconds +Sconds_L ;
	DS1302Write (0x80,Sconds );
}


uint8_t  GetMinutes(void )
{
	uint8_t Minutes_H,Minutes_L,Minutes = 0;
	Minutes = DS1302Read (0x83);
	Minutes_L = Minutes & 0x0f;
	Minutes = Minutes  >> 4;
	Minutes_H = Minutes & 0x07;
	Minutes = 10 * Minutes_H + Minutes_L ;
	return Minutes ;
}



void SetMinutes(uint8_t Minutes)
{
	uint8_t Minutes_H,Minutes_L;
	Minutes_L = Minutes %10;
	Minutes_H = Minutes /10;
	Minutes = Minutes_H << 4;
	Minutes = Minutes + Minutes_L ;
	DS1302Write (0x82,Minutes );
}

uint8_t  GetHour(void )
{
	uint8_t Hour_H,Hour_L,Hour = 0;
	Hour = DS1302Read (0x85);
	Hour_L = Hour & 0x0f;
	Hour = Hour >> 4;
	Hour_H = Hour & 0x03;
	Hour = 10 * Hour_H + Hour_L ;
	return Hour ;
}

void SetHour(uint8_t Hour)
{
	uint8_t Hour_H,Hour_L;
	Hour_L = Hour %10;
	Hour_H = Hour /10;
	Hour  = Hour_H << 4;
	Hour = Hour + Hour_L ;
	DS1302Write (0x84,Hour );
}

uint8_t  GetDate(void )
{
	uint8_t Date_H,Date_L,Date = 0;
	Date = DS1302Read (0x87);
	Date_L = Date & 0x0f;
	Date = Date >> 4;
	Date_H = Date & 0x03;
	Date = 10 * Date_H + Date_L ;
	return Date ;
}

void SetDate(uint8_t Date)
{
	uint8_t Date_H,Date_L;
	Date_L = Date % 10;
//	printf("Debuf:%d",Date_L);
	Date_H = Date /10;
//	printf("Debuf:%d",Date_H);
	Date  = Date_H << 4;
//	printf("Debuf:%d",Date);
	Date = Date + Date_L ;
//	printf("Debuf:%d",Date);
	DS1302Write (0x86,Date );
}

uint8_t  GetMonth(void )
{
	uint8_t Month_H,Month_L,Month = 0;
	Month = DS1302Read (0x89);
	Month_L = Month  & 0x0f;
	Month  = Month  >> 4;
	Month_H = Month  & 0x01;
	Month  = 10 * Month_H + Month_L ;
	return Month  ;
}

void SetMonth(uint8_t Month)
{
	uint8_t Month_H,Month_L;
	Month_L = Month % 10;
	Month_H = Month /10;
	Month  = Month_H << 4;
	Month  = Month + Month_L ;
	DS1302Write (0x88,Month );
}

uint8_t  GetDay(void )
{
	uint8_t Day_H,Day_L,Day = 0;
	Day = DS1302Read (0x8B);
	Day_L = Day  & 0x07;
	Day  = Day  >> 4;
	Day_H = Day  & 0x00;
	Day  = 10 * Day_H + Day_L ;
	return Day  ;
}

void SetDay(uint8_t Day)
{
	uint8_t Day_H,Day_L;
	Day_L = Day % 10;
	Day_H = Day /10;
	Day  = Day_H << 4;
	Day  = Day + Day_L ;
	DS1302Write (0x8A,Day );
}

uint8_t  GetYear(void )
{
	uint8_t Year_H,Year_L,Year = 0;
	Year  = DS1302Read (0x8D);
//	printf("Debug:%d",Year );
	Year_L = Year   & 0x0f;
	Year   = Year   >> 4;
	Year_H = Year   & 0x0f;
	Year   = 10 * Year_H + Year_L ;
	return Year   ;
}

void SetYear(uint8_t Year)
{
	uint8_t Year_H,Year_L;
	Year_L  = Year  % 10;
	Year_H  = Year  /10;
	Year   = Year_H  << 4;
	Year   = Year  + Year_L  ;
//	printf("Debug:%d",Year );
	DS1302Write (0x8C,Year  );
}

void Set_Year_Month_Date(uint8_t Year,uint8_t Month,uint8_t Date)
{
	SetYear (Year );
	SetMonth (Month );
	SetDate (Date );
}

void Set_Hour_Minutes_Seconds(uint8_t Hour,uint8_t Minutes,uint8_t Seconds)
{
	SetHour (Hour );
	SetMinutes (Minutes );
	SetSconds (Seconds );
}

