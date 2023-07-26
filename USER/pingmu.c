
 #include "pingmu.H"
 #include "stepkong.h"
 
 static char pingmustate=0;
 
 
char  buf[30]; //保存串口4接收的数据
char Recv_Flag4=0 ;  //接受数据标志 起始标志到后
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
void sendbyte4(u8 c)//串口4发送一个字节的函数
{
  HAL_UART_Transmit(&UART2_Handler,&c,1,0xffffff);
}
void pingmu_write(int weizhi,int value)//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
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
	
//数据传输到曲线,,
//5A A5 04 84 01 0020  01是曲线0,02是曲线1 ,,,
}
void pingmu_writelong(int dizhi,int length,int *s)//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
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
//数据传输到曲线,,
//5A A5 04 84 01 0020  01是曲线0,02是曲线1 ,,,
}
void pingmu_changepage(int value)//把数据曲线上传到显示屏..weizhi代表位置,value代表值.
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
//数据传输到曲线,,
//5A A5 04 84 01 0020  01是曲线0,02是曲线1 ,,,
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
 static char readpingmu(int a,int b)//a是地址,b是长度.这个是发送上传多少的数据上去.
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

int pingmu_read(int a)//a是地址,下载某个地址的值,并且返回..
 {
	 int i,j;
   readpingmu(a,1);	 
	 j=0;
		   while(j<40)//4s钟没读到,报错.	   
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
 //显示汉字的程序.
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
 char pingmu_readlong(int dizhi,int length,int *s)//a是地址,length长度,s保存值..
 {
	 int i,j,k;
   readpingmu(dizhi,length);	 
	 j=0;
	 while(j<40)//4s钟没读到,报错.	   
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
	 j=buf[0];//数组最后一位..//2,3是位置.4是长度.5,6开始是数据.
	 for(i=0;i<length;i++)
	 {
		 k=6+i*2;
		 *s=buf[k-1]+buf[k];
		 s++;
	 }
	 	pngmu_free();
	 return(1);
 }
 /*发送过来的数据格式5A A5 06 83 00 00 01 00 02
 5A A5 数据帧头
 06 是长度
 00 00是开始地址
 01 是读取的长度
 00 02是返回的值,也就是数据值. 
 该函数会把06 83 00 00 01 00 02保存在buf数组中.
 如果地址是00 00 .屏幕设置的00 00的数据自动下传,所以是自动下穿的数据.
 */

 void chuankou2jisuuan()//5A A5 06 83 00 00 01 00 02
{
	 if(Recv_Flag4==2)//标志位..发送读取命令前,应该将其清零..
		{ 
		   buf[uart_Index4] = S4BUF;
			if(uart_Index4==buf[0])
			{
			  returnnum=buf[buf[0]];//读取的是最后一位的数值.
				uart_Index4=0;//多少位的标志量.....
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
			Recv_Flag4 = 1;  //接受数据标志 起始标志到后
		}
		else if(Recv_Flag4==1&&S4BUF==0xA5)
		{	
		    uart_Index4 = 0;	//清零方便计数帧
		 	Recv_Flag4 = 2;  //接受数据标志 起始标志到后    
	   	}
		else
		{	
			Recv_Flag4=0;		
		}
}

