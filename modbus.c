//----------------------------------------------------------------------------//
//�˴���ֻ֧����ΪModbus��վ�豸��Modbus RTUģʽ
//
//֧�ֵĹ����룺
//0x03 �����ּĴ�������������ּĴ�����ֵ����Ч��λΪ0-99��
//0x06 д�����Ĵ�����д��һ���Ĵ�����ֵ����Ч��ַΪ0-99��
//0x10 д����Ĵ�����д�����Ĵ�����ֵ����Ч��ַΪ0-99��
//
//֧�ֵ��쳣�룺
//0x01 �Ƿ������루��֧�ֵĹ����룩
//0x02 �Ƿ����ݵ�ַ����ʼ��ַ������Ч��Χ�ڣ�
//0x03 �Ƿ�����ֵ������ʼ��ַ�Ļ����ϣ������ǲ��Ϸ��ģ�
//----------------------------------------------------------------------------//

#include "MODBUS.h"
#include "uart.h"
#include "stc15f2k60s2.h"
#include <string.h>
// #include "sys.h"
// #include "delay.h"
// #include "usart.h"
// #include "timer.h"
// #include "modbusCRC.h"
// #include "dma.h"
// #include "io.h"
// #include "24cxx.h"
// #include "yunxingkongzhi.h"
// #include "kongzhiban.h"
// #include "pingmu.h"


int Slave_Address=1;

#define Modbus_ReadHoldingReg	3
#define Modbus_WriteSingleReg	6
#define Modbus_WriteMultipleReg	10       
/* �������� ------------------------------------------------------------------*/
uint8_t Modbus_Send_Buff[Modbus_Max_Send_Buff];                //�������ݻ�����
uint8_t Modbus_Rcv_Buff[Modbus_Max_Rcv_Buff];                //�������ݻ�����
uint8_t Modbus_Timeout_Cnt;                                                        //��ʱ���жϼ���
uint8_t Modbus_Rcv_Cnt;                                                                //�����ֽڼ���
uint8_t        Modbus_Rcv_flag;                                                        //�豸�������״̬��־
uint8_t Modbus_Cmd_flag;                                                        //�豸�����������״̬��־
uint8_t Modbus_Exe_flag;                                                        //�豸��������ִ��״̬��־
uint8_t Modbus_Function;                                                        //��վ�豸��ִ�еĹ���

volatile uint16_t HoldingReg[100] = {0, 0, 0, 0};                                //���ּĴ���
int UART3_Handler=0;
/* �������� ------------------------------------------------------------------*/

//----------------------------------------------------------------------------//
//�������ܣ���λ���㷨CRC16У�飬��Modbus��CRC���Ҫ���иߵ��ֽڽ����������ֽ���ǰ�����ֽ��ں�
//��ڲ�����puchMsg��Ҫ����CRCУ�����Ϣ��usDataLen����Ϣ���ֽ���
//���ڲ��������������CRCУ���룬16λ����
//����޸ģ�2015.11.29
//��ע��
//----------------------------------------------------------------------------//
void init_485()
{
	
}
char cuncu_485()
 {
	// printf("cuncu_485 %d",1);
	return 1;
 }
uint16_t Modbus_CRC16(uint8_t *puchMsg, uint8_t usDataLen)
{
        uint16_t CRC_Cal = 0xFFFF;
        uint8_t CRC_High, CRC_Low;
        uint8_t i, j;
        
        for(j = 0; j < usDataLen; j++)
        {
                CRC_Cal = CRC_Cal ^ *puchMsg++;
                
                for (i = 0; i < 8; i++)
                {
                        if((CRC_Cal & 0x0001) == 0x0001)
                        {
                                CRC_Cal = CRC_Cal >> 1;
                                CRC_Cal = CRC_Cal ^ 0xA001;
                        }
                        else
                        {
                                CRC_Cal = CRC_Cal >> 1;
                        }
                }
        }
        
        CRC_High = (uint8_t)(CRC_Cal >> 8);
        CRC_Low = (uint8_t)(CRC_Cal & 0x00FF);
        
        return (CRC_Low << 8 | CRC_High);
        
//        return CRC_Cal;

}

//----------------------------------------------------------------------------//
//�������ܣ�Modbus��ʼ��
//��ڲ�����ID�Ǵ�վվ��
//���ڲ�������
//����޸ģ�2015.11.20
//��ע��
//----------------------------------------------------------------------------//
void Modbus_Init(void)
{
        uint16_t i;
        
        //----------------------------------------------------------//
        //Modbus��ر�����ʼ��
        //----------------------------------------------------------//
        Modbus_Timeout_Cnt = 0;
        Modbus_Rcv_Cnt = 0;
        Modbus_Rcv_flag = 0;
        Modbus_Cmd_flag = 0;
        Modbus_Exe_flag = 0;
        
        for(i = 0; i < Modbus_Max_Rcv_Buff; i++)                //������ջ�����
        {
                Modbus_Rcv_Buff[i] = '\0';
        }
        
        for(i = 0; i < Modbus_Max_Send_Buff; i++)                //������ͻ�����
        {
                Modbus_Send_Buff[i] = '\0';
        }
        
        //----------------------------------------------------------//
        //TIM2��ʱ��ʹ��
        //----------------------------------------------------------//
        //TIM_Cmd(TIM2, ENABLE);
}
int rectimes;
void chuankou1jisuuan(unsigned char ans)
{
        Modbus_Rcv_Buff[Modbus_Rcv_Cnt]=ans;
	Modbus_Rcv_Cnt++;
        rectimes=2;
}
int recover=0;
int bijiao(const char *a,const char *b)
{
        int i;
        for(i=0;i<6;i++)
        {
            if(a[i]!=b[i])
            {
                return 0;
            }
        }
        return 1;
}

void chuliguankji()
{
    if(1==bijiao(Modbus_Rcv_Buff,"@STCISP#"))
    {
        IAP_CONTR=0x60;
    }
}
void time1msjisuan()
{
    if(rectimes>0)
    {
        rectimes--;
        if(rectimes==0)
        {
           chuliguankji();
            recover=1;
        }
    }    
}

void Modbus_Cmd(void);
void Modbus_Exe(void);

void jishouokjisuan()
{
        
	Modbus_Cmd_flag=1;//���ݽ�����,,�����жϱ�־λ..
	Modbus_Cmd();     //���ݴ���,,          
	Modbus_Exe();     //�����귢...    
        Modbus_ClearBuff();//Ū������������     
}

char dmatime=0;
void DMA1_Channel2_IRQHandler(void)
{

}
//----------------------------------------------------------------------------//
//�������ܣ�Modbus�����������
//��ڲ�������
//���ڲ�������
//����޸ģ�2015.12.11
//��ע��
//----------------------------------------------------------------------------//
void Modbus_Cmd(void)
{
        uint8_t Modbus_CRC_Rcv_Hi;                //���յ���ModbusCRCУ������ֽ�
        uint8_t Modbus_CRC_Rcv_Lo;                //���յ���ModbusCRCУ������ֽ�
        uint16_t Modbus_CRC_Rcv;                //���յ���ModbusCRCУ����
        uint16_t Modbus_CRC_Cal;                //���ݽ��յ������ݼ��������CRCֵ
        
        //----------------------------------------------------------//
        //��ʼ�������
        //----------------------------------------------------------//
        if(Modbus_Cmd_flag == 1)
        {
                if(Modbus_Rcv_Cnt > 4)                //������յ���һ֡���ֽ�������4 ����ȷ��֡�ĳ�����������Χ
                {
                        Modbus_CRC_Rcv_Lo = Modbus_Rcv_Buff[Modbus_Rcv_Cnt - 2];                                        //���յ���ModbusCRCУ������ֽ�
                        Modbus_CRC_Rcv_Hi = Modbus_Rcv_Buff[Modbus_Rcv_Cnt - 1];                                        //���յ���ModbusCRCУ������ֽ�
                        Modbus_CRC_Rcv = (uint16_t)(Modbus_CRC_Rcv_Lo << 8 | Modbus_CRC_Rcv_Hi);        //���յ���ModbusCRCУ���루16λ��
                        Modbus_CRC_Cal = Modbus_CRC16(Modbus_Rcv_Buff, Modbus_Rcv_Cnt - 2);                        //���ݽ��յ������ݼ���CRCֵ
                        if(Modbus_CRC_Cal == Modbus_CRC_Rcv)                //��������CRCֵ����ܵ�CRCֵ���
                        {
                                //USART_SendByte(USART1, 0xAC);
                                if(Slave_Address == Modbus_Rcv_Buff[0])        //����Ǳ�����ַ
                                {
                                        switch(Modbus_Rcv_Buff[1])                        //��switch��֧�����ȷ������
                                        {
                                        case Modbus_ReadHoldingReg:                                                //����Ƕ�����Ĵ���
                                                Modbus_Function = Modbus_ReadHoldingReg;        //����վ�豸��ִ�еĹ��ܸ�ֵΪ������Ĵ���
                                                Modbus_Exe_flag = 1;                                                //�豸��������ִ��״̬
                                                break;                                                                                //������֧���
                                                
                                        case Modbus_WriteSingleReg:
                                                Modbus_Function = Modbus_WriteSingleReg;        //����վ�豸��ִ�еĹ��ܸ�ֵΪд�����Ĵ���
                                                Modbus_Exe_flag = 1;                                                //�豸��������ִ��״̬
                                                break;                                                                                //������֧���
                                        
                                        case Modbus_WriteMultipleReg:
                                                Modbus_Function = Modbus_WriteMultipleReg;        //����վ�豸��ִ�еĹ��ܸ�ֵΪд����Ĵ���
                                                Modbus_Exe_flag = 1;                                                //�豸��������ִ��״̬
                                                break;                                                                                //������֧���
                                                
                                        default:
                                                Modbus_ErrorHandling(0x01);                //���й����붼�����ϣ��򷵻ع���������쳣��Ӧ����
                                                return;
                                        }
                                }
                                
                                else                //������ս������ݻ������ͷ������ݻ�����
                                {
                                        Modbus_ClearBuff();
                                }
                        }
                        
                        else                //������ս������ݻ������ͷ������ݻ�����
                        {
                                Modbus_ClearBuff();
                        }
                        
                }
                
                else                //������ս������ݻ������ͷ������ݻ�����
                {
                        Modbus_ClearBuff();
                }
                
                Modbus_Cmd_flag = 0;                //�豸�˳��������״̬��־
        }
}

//----------------------------------------------------------------------------//
//�������ܣ�Modbus����ִ�к���
//��ڲ�������
//���ڲ�������
//����޸ģ�2015.12.6
//��ע��
//----------------------------------------------------------------------------//
void Modbus_Exe(void)
{
        if(Modbus_Exe_flag == 1)
        {
                switch(Modbus_Function)
                {
                case Modbus_ReadHoldingReg:
                        Modbus_ReadHoldingReg_Process();
                        break;
                        
                case Modbus_WriteSingleReg:
                        Modbus_WriteSingleReg_Process();
                        break;
                
                case Modbus_WriteMultipleReg:
                        Modbus_WriteMultipleReg_Process();
                        break;
                        
                }
                Modbus_Exe_flag = 0;
        }
}

//----------------------------------------------------------------------------//
//�������ܣ�������0x03�������ּĴ���
//��ڲ�������
//���ڲ�������
//����޸ģ�2015.12.5
//��ע��
//----------------------------------------------------------------------------//
void Modbus_ReadHoldingReg_Process(void)
{
        uint8_t Send_Cnt;                        //�����ֽ�����
        uint16_t StartAddress_Reg;        //Ҫ��ȡ�ļĴ�����ʼ��ַ
        uint16_t Num_Reg;                        //Ҫ��ȡ�ļĴ���������
        uint16_t CRC_Cal;                        //CRCУ����
        uint16_t i, j;                                //��ʱ����
        
        StartAddress_Reg = Modbus_Rcv_Buff[2] << 8 | Modbus_Rcv_Buff[3];        //�ӽ������ݻ������õ�Ҫ��ȡ�ļĴ�����ʼ��ַ
        Num_Reg = Modbus_Rcv_Buff[4] << 8 | Modbus_Rcv_Buff[5];                                //�ӽ������ݻ������õ�Ҫ��ȡ�ļĴ�������
        
        if(StartAddress_Reg < 100)                //�Ĵ�����ʼ��ַ����ȷ��Χ��
        {
                if(StartAddress_Reg + Num_Reg < 100 && Num_Reg > 0)                //��ʼ��ַ+�Ĵ�������λ����ȷ��Χ�� ���� �Ĵ���������ȷ
                {
																			
                        Send_Cnt = 3 + (Num_Reg << 1) + 2;                //���㷢���ֽ�����
                        Modbus_Send_Buff[0] = Slave_Address;                        //��վ��ַ
                        Modbus_Send_Buff[1] = Modbus_ReadHoldingReg;        //������
                        Modbus_Send_Buff[2] = Num_Reg << 1;                                //�Ĵ����ֽ����� ���� �Ĵ���������2
                        
                        for(i = StartAddress_Reg, j = 3; i < StartAddress_Reg + Num_Reg; i++, j += 2)        //��ȡ�Ĵ���������
                        {
                                Modbus_Send_Buff[j] = (uint8_t)(HoldingReg[i] >> 8);
                                Modbus_Send_Buff[j + 1] = (uint8_t)(HoldingReg[i] & 0x00FF);
                        }
        
                        CRC_Cal = Modbus_CRC16(Modbus_Send_Buff, 3 + (Num_Reg << 1));                                //���㷢�����ݵ�CRCУ����
                        Modbus_Send_Buff[3 + (Num_Reg << 1)] = (uint8_t)(CRC_Cal >> 8);                                //���ǵ��ֽ�
                        Modbus_Send_Buff[3 + (Num_Reg << 1) + 1] = (uint8_t)(CRC_Cal & 0x00FF);                //���Ǹ��ֽ�
        
                        //USART_SendString(USART1, Modbus_Send_Buff, Send_Cnt);                        //������Ӧ����
                        HAL_UART_Transmit_DMA_485(&UART3_Handler,(u8*)Modbus_Send_Buff,Send_Cnt);
                        //Modbus_ClearBuff();                //��ս������ݻ������ͷ������ݻ�����
                }
                else
                {
                        Modbus_ErrorHandling(0x03);                //�Ƿ�����ֵ
                }
        }
        
        else
        {
                Modbus_ErrorHandling(0x02);                        //�Ƿ����ݵ�ַ
        }
        
}

//----------------------------------------------------------------------------//
//�������ܣ�������0x06��д�����Ĵ���
//��ڲ�������
//���ڲ�������
//����޸ģ�2015.12.6
//��ע��
//----------------------------------------------------------------------------//
void Modbus_WriteSingleReg_Process(void)
{
        uint8_t Send_Cnt;                        //�����ֽ�����
        uint16_t Address_Reg;                //Ҫд��ļĴ�����ַ
        uint16_t Value_Reg;                        //Ҫд��ļĴ���ֵ
        uint16_t CRC_Cal;                        //CRCУ����
        
        Address_Reg = Modbus_Rcv_Buff[2] << 8 | Modbus_Rcv_Buff[3];                //�ӽ������ݻ������õ�Ҫд��ļĴ�����ַ
        Value_Reg = Modbus_Rcv_Buff[4] << 8 | Modbus_Rcv_Buff[5];                //�ӽ������ݻ������õ�Ҫд��ļĴ���ֵ
        
        if(Address_Reg < 100)                //�Ĵ�����ʼ��ַ����ȷ��Χ��
        {
							
                Send_Cnt = 6 + 2;                //���㷢���ֽ�����
                
                HoldingReg[Address_Reg] = Value_Reg;                //��Ҫд��ļĴ���ֵд��Ĵ���
                
                Modbus_Send_Buff[0] = Slave_Address;                                                                        //��վ��ַ
                Modbus_Send_Buff[1] = Modbus_WriteSingleReg;                                                        //������
                Modbus_Send_Buff[2] = (uint8_t)(Address_Reg >> 8);                                                //�Ĵ�����ַ���ֽ�
                Modbus_Send_Buff[3] = (uint8_t)(Address_Reg & 0x00FF);                                        //�Ĵ�����ַ���ֽ�
                Modbus_Send_Buff[4] = (uint8_t)(HoldingReg[Address_Reg] >> 8);                        //�Ĵ���ֵ���ֽ�
                Modbus_Send_Buff[5] = (uint8_t)(HoldingReg[Address_Reg] & 0x00FF);                //�Ĵ���ֵ���ֽ�
                
                CRC_Cal = Modbus_CRC16(Modbus_Send_Buff, 6);                        //���㷢�����ݵ�CRCУ����
                Modbus_Send_Buff[6] = (uint8_t)(CRC_Cal >> 8);                        //���ǵ��ֽ�
                Modbus_Send_Buff[7] = (uint8_t)(CRC_Cal & 0x00FF);                //���Ǹ��ֽ�
                
               // USART_SendString(USART1, Modbus_Send_Buff, Send_Cnt);        //������Ӧ����
                HAL_UART_Transmit_DMA_485(&UART3_Handler,(u8*)Modbus_Send_Buff,Send_Cnt);
                //Modbus_ClearBuff();                //��ս������ݻ������ͷ������ݻ�����
        }
        
        else
        {
                Modbus_ErrorHandling(0x02);                        //�Ƿ����ݵ�ַ
        }
}

//----------------------------------------------------------------------------//
//�������ܣ�������0x10��д����Ĵ���
//��ڲ�������
//���ڲ�������
//����޸ģ�2015.12.9
//��ע��
//----------------------------------------------------------------------------//
void Modbus_WriteMultipleReg_Process(void)
{
        uint8_t Send_Cnt;                        //�����ֽ�����
        uint16_t StartAddress_Reg;        //Ҫд��ļĴ�����ʼ��ַ
        uint16_t Num_Reg;                        //Ҫд��ļĴ���������
        uint16_t CRC_Cal;                        //CRCУ����
        uint16_t i, j;                                //��ʱ����
        
        StartAddress_Reg = Modbus_Rcv_Buff[2] << 8 | Modbus_Rcv_Buff[3];        //�ӽ������ݻ������õ�Ҫд��ļĴ�����ʼ��ַ
        Num_Reg = Modbus_Rcv_Buff[4] << 8 | Modbus_Rcv_Buff[5];                                //�ӽ������ݻ������õ�Ҫд��ļĴ�������
        
        if(StartAddress_Reg < 100)                        //�Ĵ�����ʼ��ַ����ȷ��Χ��
        {
                if(StartAddress_Reg + Num_Reg < 100 && Num_Reg > 0)                                //��ʼ��ַ+�Ĵ�������λ����ȷ��Χ�� ���� �Ĵ���������ȷ                        
                {
                        for(i = StartAddress_Reg, j = 7; i < StartAddress_Reg + Num_Reg; i++, j += 2)        //��Ҫд��ļĴ���ֵд��Ĵ���
                        {
                                HoldingReg[i] = Modbus_Rcv_Buff[j] << 8 | Modbus_Rcv_Buff[j + 1];
                        }
                        Send_Cnt = 6 + 2;
                
                        Modbus_Send_Buff[0] = Slave_Address;                                                //��վ��ַ
                        Modbus_Send_Buff[1] = Modbus_WriteMultipleReg;                                //������
                        Modbus_Send_Buff[2] = (uint8_t)(StartAddress_Reg >> 8);                //�Ĵ�����ʼ��ַ���ֽ�
                        Modbus_Send_Buff[3] = (uint8_t)(StartAddress_Reg & 0x00FF);        //�Ĵ�����ʼ��ַ���ֽ�
                        Modbus_Send_Buff[4] = (uint8_t)(Num_Reg >> 8);                                //�Ĵ����������ֽ�
                        Modbus_Send_Buff[5] = (uint8_t)(Num_Reg & 0x00FF);                        //�Ĵ����������ֽ�
                        
                        CRC_Cal = Modbus_CRC16(Modbus_Send_Buff, 6);                        //���㷢�����ݵ�CRCУ����
                        Modbus_Send_Buff[6] = (uint8_t)(CRC_Cal >> 8);                        //���ǵ��ֽ�
                        Modbus_Send_Buff[7] = (uint8_t)(CRC_Cal & 0x00FF);                //���Ǹ��ֽ�
                        
                       // USART_SendString(USART1, Modbus_Send_Buff, Send_Cnt);        //������Ӧ����
                        HAL_UART_Transmit_DMA_485(&UART3_Handler,(u8*)Modbus_Send_Buff,Send_Cnt);
                        //Modbus_ClearBuff();                //��ս������ݻ������ͷ������ݻ�����
                }
                
                else
                {
                        Modbus_ErrorHandling(0x03);
                }
        }
        
        else
        {
                Modbus_ErrorHandling(0x02);
        }
}

//----------------------------------------------------------------------------//
//�������ܣ�������
//��ڲ�����ErrorType�Ǵ�������
//���ڲ�������
//����޸ģ�2015.12.11
//��ע��
//----------------------------------------------------------------------------//
void Modbus_ErrorHandling(uint8_t ErrorType)
{
        uint16_t CRC_Cal;                        //CRCУ����
        
        switch(ErrorType)                        //��switch��֧�����ȷ��Modbus�쳣��
        {
        case 0x01:                                        //�Ƿ�������
                Modbus_Send_Buff[0] = Slave_Address;                                        //��վ��ַ
                Modbus_Send_Buff[1] = Modbus_Rcv_Buff[1] + 0x80;                //�쳣������
                Modbus_Send_Buff[2] = 0x01;                                                                //�쳣��
                CRC_Cal = Modbus_CRC16(Modbus_Send_Buff, 3);                        //���㷢�����ݵ�CRCУ����
                Modbus_Send_Buff[3] = (uint8_t)(CRC_Cal >> 8);                        //���ǵ��ֽ�
                Modbus_Send_Buff[4] = (uint8_t)(CRC_Cal & 0x00FF);                //���Ǹ��ֽ�
                
                //USART_SendString(USART1, Modbus_Send_Buff, 5);                        //�����쳣��Ӧ����
				        HAL_UART_Transmit_DMA_485(&UART3_Handler,(u8*)Modbus_Send_Buff,5);
                break;
                
        case 0x02:                                        //�Ƿ����ݵ�ַ
                Modbus_Send_Buff[0] = Slave_Address;                                        //��վ��ַ
                Modbus_Send_Buff[1] = Modbus_Rcv_Buff[1] + 0x80;                //�쳣������
                Modbus_Send_Buff[2] = 0x02;                                                                //�쳣��
                CRC_Cal = Modbus_CRC16(Modbus_Send_Buff, 3);                        //���㷢�����ݵ�CRCУ����
                Modbus_Send_Buff[3] = (uint8_t)(CRC_Cal >> 8);                        //���ǵ��ֽ�
                Modbus_Send_Buff[4] = (uint8_t)(CRC_Cal & 0x00FF);                //���Ǹ��ֽ�
                
                //USART_SendString(USART1, Modbus_Send_Buff, 5);                        //�����쳣��Ӧ����
				        HAL_UART_Transmit_DMA_485(&UART3_Handler,(u8*)Modbus_Send_Buff,5);
                break;
                
        case 0x03:                                        //�Ƿ�����ֵ
                Modbus_Send_Buff[0] = Slave_Address;                                        //��վ��ַ
                Modbus_Send_Buff[1] = Modbus_Rcv_Buff[1] + 0x80;                //�쳣������
                Modbus_Send_Buff[2] = 0x03;                                                                //�쳣��
                CRC_Cal = Modbus_CRC16(Modbus_Send_Buff, 3);                        //���㷢�����ݵ�CRCУ����
                Modbus_Send_Buff[3] = (uint8_t)(CRC_Cal >> 8);                        //���ǵ��ֽ�
                Modbus_Send_Buff[4] = (uint8_t)(CRC_Cal & 0x00FF);                //���Ǹ��ֽ�
                
                //USART_SendString(USART1, Modbus_Send_Buff, 5);                        //�����쳣��Ӧ����
				        HAL_UART_Transmit_DMA_485(&UART3_Handler,(u8*)Modbus_Send_Buff,5);
                break;
//        default:
//                return;
                
        }
        
        //Modbus_ClearBuff();                //��ս������ݻ������ͷ������ݻ�����
}

//----------------------------------------------------------------------------//
//�������ܣ���ս������ݻ������ͷ������ݻ�����
//��ڲ�������USAR
//���ڲ�������
//����޸ģ�2015.12.5
//��ע��
//----------------------------------------------------------------------------//
void Modbus_ClearBuff(void)
{
        uint16_t i;
        for(i = 0; i < Modbus_Max_Rcv_Buff; i++)                //������ջ�����
        {
                Modbus_Rcv_Buff[i] = '\0';
        }
        Modbus_Rcv_Cnt = 0;                        //�����ֽڼ�����0
        
        for(i = 0; i < Modbus_Max_Send_Buff; i++)                //������ͻ�����
        {
                Modbus_Send_Buff[i] = '\0';
        }
        
}

//----------------------------------------------------------------------------//
//�������ܣ���һ���ֽڵĸ�4λʮ���������浽��һ���ֽڵĵ�4λ��
//��ڲ�����һ���ֽڵ�����
//���ڲ�������һ���ֽ�
//����޸ģ�2015.11.28
//��ע��
//----------------------------------------------------------------------------//
uint8_t High4BitsToOneByte(uint8_t Byte)
{
        uint8_t tempByte;
        
        tempByte = (Byte >> 4) & 0x0F;
        
        return tempByte; 
}

//----------------------------------------------------------------------------//
//�������ܣ���һ���ֽڵĵ�4λʮ���������浽��һ���ֽڵĵ�4λ��
//��ڲ�����һ���ֽڵ�����
//���ڲ�������һ���ֽ�
//����޸ģ�2015.11.28
//��ע��
//----------------------------------------------------------------------------//
uint8_t Low4BitsToOneByte(uint8_t Byte)
{
        uint8_t tempByte;
        
        tempByte = Byte & 0x0F;
        
        return tempByte; 
}

//----------------------------------------------------------------------------//
//�������ܣ��ѵ�4λ16������ת��Ϊ��OLED�ֿ��϶�Ӧ��0~9��A~F
//��ڲ�����HexByte�ǵ�4λ16������
//���ڲ�����OLED�ֿ��϶�Ӧ��0~9��A~F
//����޸ģ�2015.11.28
//��ע��
//----------------------------------------------------------------------------//
uint8_t HexToOLEDAsc(uint8_t HexByte)
{
        if((HexByte >= 0x00) && (HexByte <= 0x09))                        //����0~9
        {
                HexByte += 0x30;
        }
        else if((HexByte >= 0x0A) && (HexByte <= 0x0F))        //����A~F
        {
                HexByte += 0x37;
        }
        else
        {
                HexByte = 0xff;
        }
        
        return HexByte; 
}
void HAL_UART_Transmit_DMA_485(int *huart, uint8_t *pData, uint16_t Size)
{
        int i;
        for(i=0;i<Size;i++)
        {
                sendbyte1(pData[i]);
        }
}