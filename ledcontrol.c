
#include "ledcontrol.h"


sbit out1 = P1 ^ 6; //
sbit out2 = P3 ^ 2;


sbit fen = P5 ^ 4;


/**************************************
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5us()
{
    unsigned char i;

    i = 20;
    while (--i)
        ;
}

/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
extern void delay_ms(int m);
void Delay5ms()
{
    delay_ms(5);
}

/**************************************
起始信号
**************************************/
sbit SCL = P2 ^ 5; // IIC时钟引脚定义
sbit SDA = P2 ^ 4; // IIC数据引脚定义
typedef unsigned char BYTE;

void BH1750_Start()
{
    SDA = 1;    // 拉高数据线
    SCL = 1;    // 拉高时钟线
    Delay5us(); // 延时
    SDA = 0;    // 产生下降沿
    Delay5us(); // 延时
    SCL = 0;    // 拉低时钟线
}

/**************************************
停止信号
**************************************/
void BH1750_Stop()
{
    SDA = 0;    // 拉低数据线
    SCL = 1;    // 拉高时钟线
    Delay5us(); // 延时
    SDA = 1;    // 产生上升沿
    Delay5us(); // 延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;  // 写应答信号
    SCL = 1;    // 拉高时钟线
    Delay5us(); // 延时
    SCL = 0;    // 拉低时钟线
    Delay5us(); // 延时
}

/**************************************
接收应答信号
**************************************/
bit BH1750_RecvACK()
{
    SCL = 1;    // 拉高时钟线
    Delay5us(); // 延时
    CY = SDA;   // 读应答信号
    SCL = 0;    // 拉低时钟线
    Delay5us(); // 延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void BH1750_SendByte(BYTE dat)
{
    BYTE i;

    for (i = 0; i < 8; i++) // 8位计数器
    {
        dat <<= 1;  // 移出数据的最高位
        SDA = CY;   // 送数据口
        SCL = 1;    // 拉高时钟线
        Delay5us(); // 延时
        SCL = 0;    // 拉低时钟线
        Delay5us(); // 延时
    }
    BH1750_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE BH1750_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                // 使能内部上拉,准备读取数据,
    for (i = 0; i < 8; i++) // 8位计数器
    {
        dat <<= 1;
        SCL = 1;    // 拉高时钟线
        Delay5us(); // 延时
        dat |= SDA; // 读数据
        SCL = 0;    // 拉低时钟线
        Delay5us(); // 延时
    }
    return dat;
}

//*********************************
#define SlaveAddress 0x46 // 定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                          // ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                // 起始信号
    BH1750_SendByte(SlaveAddress); // 发送设备地址+写信号
    BH1750_SendByte(REG_Address);  // 内部寄存器地址，
    //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    BH1750_Stop(); // 发送停止信号
}

//********单字节读取*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  uchar REG_data;
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress);           //发送设备地址+写信号
    BH1750_SendByte(REG_Address);                   //发送存储单元地址，从0开始
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=BH1750_RecvByte();              //读出寄存器数据
    BH1750_SendACK(1);
    BH1750_Stop();                           //停止信号
    return REG_data;
}
*/
//*********************************************************
//
// 连续读出BH1750内部数据
//
//*********************************************************

int Multiple_read_BH1750()
{
    int dis_data; // 变量
    BYTE BUF[8]={0}; // 接收数据缓存区
    uchar i;
    BH1750_Start();                    // 起始信号
    BH1750_SendByte(SlaveAddress + 1); // 发送设备地址+读信号

    for (i = 0; i < 3; i++) // 连续读取2个地址数据，存储中BUF
    {
        BUF[i] = BH1750_RecvByte(); // BUF[0]存储0x32地址中的数据
        if (i == 3)
        {

            BH1750_SendACK(1); // 最后一个数据需要回NOACK
        }
        else
        {
            BH1750_SendACK(0); // 回应ACK
        }
    }

    BH1750_Stop(); // 停止信号
    dis_data = BUF[0];
    dis_data = (dis_data << 8) + BUF[1]; // 合成数据，即光照数据
    return dis_data;
    // Delay5ms();

}

// 初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750()
{
    Single_Write_BH1750(0x01);
}
