#include "intrins.h"

//
// void write_eeprom()
// {         
//     IapErase(0x0400);  //扇区擦除
// 	  IapProgram(0x0400 + 1,id);
// 	  IapProgram(0x0400 + 2,num1/256);IapProgram(0x0400 + 3,num1%256);
    
// 	  IapProgram(0x0400 + 4,sw1/256);IapProgram(0x0400 + 5,sw1%256);	
	  
// 	  IapProgram(0x0400 + 6,sw2/256);IapProgram(0x0400 + 7,sw2%256);
    
// 	  IapProgram(0x0400 + 8,sw3);
	
//     IapProgram(0x0400 + 9,wd1);
	
// 	  IapProgram(0x0400 + 10,sw4/256);IapProgram(0x0400 + 11,sw4%256);
	
// 	  IapProgram(0x0400 + 13,ks);
  
//     IapProgram(0x0400 + 60,a_a);
	
// }
// /******************把数据从单片机内部eeprom中读出来*****************/
// void read_eeprom()																  
// { uchar t,t1;
// 	id = IapRead(0x0400 + 1);

	
// 	t = IapRead(0x0400 + 2);t1=IapRead(0x0400 + 3);num1=t*256+t1;
// 	t = IapRead(0x0400 + 4);t1=IapRead(0x0400 + 5);sw1=t*256+t1;
	
//   t = IapRead(0x0400 + 6);t1=IapRead(0x0400 + 7);sw2=t*256+t1;
	
//   sw3 = IapRead(0x0400 + 8);
// 	wd1 = IapRead(0x0400 + 9);
	
	
// 	t = IapRead(0x0400 + 10);t1=IapRead(0x0400 + 11);sw4=t*256+t1;
	
// 	ks = IapRead(0x0400 + 13);
	
// 	a_a = IapRead(0x0400 + 60);
// }