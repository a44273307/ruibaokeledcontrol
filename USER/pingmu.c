
 #include "pingmu.H"
 #include "stepkong.h"
 
 static char pingmustate=0;
 
 
char  buf[30]; //���洮��4���յ�����
char Recv_Flag4=0 ;  //�������ݱ�־ ��ʼ��־����
char uart_Index4=0;
char returnnum=0;
int chuankou2flag=0;
char pingmuget=0;
void delayms(int i)
{
	 delay_ms(i);
}
void pingmu_busy()
{
	pingmustate=1;
}
void pngmu_free()
{
	pingmustate=0;
}
char getpingmustate(void)
{
	return(pingmustate);
}
void sendbyte4(u8 c)//����4����һ���ֽڵĺ���
{
  HAL_UART_Transmit(&UART2_Handler,&c,1,0xffffff);
}
void pingmu_write(int weizhi,int value)//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
{
	int i,j;
	pingmu_busy();
	i=value/256;
	j=value%256;
	
	sendbyte4(0x5A);  
	sendbyte4(0xA5); 
  sendbyte4(0x05); 
  sendbyte4(0x82); 
	sendbyte4(weizhi/256);		  
	sendbyte4(weizhi%256); 
	sendbyte4(i); 
	sendbyte4(j);
	pngmu_free();
	
//���ݴ��䵽����,,
//5A A5 04 84 01 0020  01������0,02������1 ,,,
}
void pingmu_writelong(int dizhi,int length,int *s)//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
{
	int i,j;
	char changdu,k;
		pingmu_busy();
	i=dizhi/256;
	j=dizhi%256;
	changdu=3+length*2;
	sendbyte4(0x5A);  
	sendbyte4(0xA5); 
  sendbyte4(changdu); 
  sendbyte4(0x82); 
	sendbyte4(i);		  
	sendbyte4(j);
	for(k=0;k<length;k++)
	{
		i=*s/256;
		j=*s%256;
		sendbyte4(i); 
		sendbyte4(j);
		s++;
	}
	pngmu_free();
//���ݴ��䵽����,,
//5A A5 04 84 01 0020  01������0,02������1 ,,,
}
void pingmu_changepage(int value)//�����������ϴ�����ʾ��..weizhi����λ��,value����ֵ.
{
	int i,j;
  	pingmu_busy();
	i=value/256;
	j=value%256;
	sendbyte4(0x5A);  
	sendbyte4(0xA5); 
  sendbyte4(0x04); 
  sendbyte4(0x80);
  sendbyte4(0x03);   
	sendbyte4(i); 
	sendbyte4(j);
	pngmu_free();
//���ݴ��䵽����,,
//5A A5 04 84 01 0020  01������0,02������1 ,,,
}
void pingmu_bee(char i)
{
//	5A A5 03 80 02 01
//    5A A5 04 80 03 00 01
	pingmu_busy();
     sendbyte4(0x5a);
	 sendbyte4(0xa5);
	 sendbyte4(0x03);
	 sendbyte4(0x80);
	 sendbyte4(0x02);
	 sendbyte4(i);
		pngmu_free();
}
 static char readpingmu(int a,int b)//a�ǵ�ַ,b�ǳ���.����Ƿ����ϴ����ٵ�������ȥ.
 {
 	 int i;
	 pingmu_busy();
	for(i=0;i<30;i++)
	 buf[i]=0x60;
	 chuankou2flag=0;
 	 sendbyte4(0x5a);
	 sendbyte4(0xa5);
	 sendbyte4(0x04);
	 sendbyte4(0x83);
	 sendbyte4(0x00);
	 sendbyte4(a);
	 sendbyte4(b);
	 return(0);
 }

int pingmu_read(int a)//a�ǵ�ַ,����ĳ����ַ��ֵ,���ҷ���..
 {
	 int i,j;
   readpingmu(a,1);	 
	 j=0;
		   while(j<40)//4s��û����,����.	   
		   {
		   		delayms(20);
		      j++;
				  if(chuankou2flag>0)
				  break;	
		   }
			 	if(j>=40)
		   {
		   	  printf("read error");
				
				  return(20);
		   }
			 j=buf[0];
			 i=buf[j-1]*256+buf[j];
			 	pngmu_free();
			 return(i);
 }
 void snedlong4(char *puts)
{
	 while(*puts) 
	{
		sendbyte4(*puts);
		puts++;
	}
}
 //��ʾ���ֵĳ���.
 void pm_hanzhi(int dizhi,char *s)
{
		int i,j;
	char k;
	char length=0x0b;
		pingmu_busy();
		i=dizhi/256;
	j=dizhi%256;
	 sendbyte4(0x5A);  
	sendbyte4(0xA5);
	length=strlen(s)+3;
	sendbyte4(length);
	sendbyte4(0x82);
	sendbyte4(i);		  
	sendbyte4(j);
	snedlong4(s);
	pngmu_free();
}
 char pingmu_readlong(int dizhi,int length,int *s)//a�ǵ�ַ,length����,s����ֵ..
 {
	 int i,j,k;
   readpingmu(dizhi,length);	 
	 j=0;
	 while(j<40)//4s��û����,����.	   
	 {
			delayms(20);
			j++;
			if(chuankou2flag>0)
			break;	
	 }
		if(j>=40)
	 {
			printf("read error");
			return(20);
	 }
	 j=buf[0];//�������һλ..//2,3��λ��.4�ǳ���.5,6��ʼ������.
	 for(i=0;i<length;i++)
	 {
		 k=6+i*2;
		 *s=buf[k-1]+buf[k];
		 s++;
	 }
	 	pngmu_free();
	 return(1);
 }
 /*���͹��������ݸ�ʽ5A A5 06 83 00 00 01 00 02
 5A A5 ����֡ͷ
 06 �ǳ���
 00 00�ǿ�ʼ��ַ
 01 �Ƕ�ȡ�ĳ���
 00 02�Ƿ��ص�ֵ,Ҳ��������ֵ. 
 �ú������06 83 00 00 01 00 02������buf������.
 �����ַ��00 00 .��Ļ���õ�00 00�������Զ��´�,�������Զ��´�������.
 */

 void chuankou2jisuuan()//5A A5 06 83 00 00 01 00 02
{
	 if(Recv_Flag4==2)//��־λ..���Ͷ�ȡ����ǰ,Ӧ�ý�������..
		{ 
		   buf[uart_Index4] = S4BUF;
			if(uart_Index4==buf[0])
			{
			  returnnum=buf[buf[0]];//��ȡ�������һλ����ֵ.
				uart_Index4=0;//����λ�ı�־��.....
				Recv_Flag4=0;//	
        chuankou2flag=1;
         if(buf[2]==0&&buf[3]==0)
				 {
				 }					 
			}
			uart_Index4++;
		}
		else
		if(S4BUF==0x5A)
		{
			Recv_Flag4 = 1;  //�������ݱ�־ ��ʼ��־����
		}
		else if(Recv_Flag4==1&&S4BUF==0xA5)
		{	
		    uart_Index4 = 0;	//���㷽�����֡
		 	Recv_Flag4 = 2;  //�������ݱ�־ ��ʼ��־����    
	   	}
		else
		{	
			Recv_Flag4=0;		
		}
}

