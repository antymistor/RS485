#include "sys.h"
#include "SEMCTR.h"	
#include "delay.h"
#include "RS485.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

u8 SEM_PACKET[20];

u16 motor0position;
u16 motor1position;
u16 motor0speed;
u16 motor1speed;
u8 getcksm(u8 *buff,u8 len)
{
	u8 temp=0x00;
	while(len>2)
  {
		temp +=buff[len-1];
		--len;
	}
	return ~temp;
}
void SEM_pidinit(void){
	//sync write
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=broadcastaddress;
	SEM_PACKET[3]=0x0c;    //LEN=12
	SEM_PACKET[4]=SEMsyncwrite ;    //INST sync write
	SEM_PACKET[5]=SEMD;    //starting address
	SEM_PACKET[6]=3;    //length of data towrite
	SEM_PACKET[7]=semaddress0;   //[1st Device] ID
	SEM_PACKET[8]=D1;   //D of motor1
	SEM_PACKET[9]=I1;   //P of motor1
	SEM_PACKET[10]=P1;   //I of motor1
	SEM_PACKET[11]=semaddress1;   //[2nd Device] ID
	SEM_PACKET[12]=D2;   //D of motor2
	SEM_PACKET[13]=I2;   //P of motor2
	SEM_PACKET[14]=P2;   //I of motor2
	SEM_PACKET[15]=getcksm(SEM_PACKET,15);
	RS485_senddata(SEM_PACKET,16);
	
}
void SEM_reboot(void){
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=broadcastaddress;
	SEM_PACKET[3]=0x02;    
	SEM_PACKET[4]=SEMreboot;    //reboot
	SEM_PACKET[5]=getcksm(SEM_PACKET,5);
	RS485_senddata(SEM_PACKET,6);
}
void SEM_setposition(u8 id,u16 position){
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=id;
	SEM_PACKET[3]=0x05;    //LEN=12
	SEM_PACKET[4]=SEMwrite;    //write
	SEM_PACKET[5]=SEMgoalposition;  
	SEM_PACKET[6]=(u8)position;  
	SEM_PACKET[7]=(u8)(position>>8);
	SEM_PACKET[8]=getcksm(SEM_PACKET,8);
	RS485_senddata(SEM_PACKET,9);
}
void SEM_setallposition(u16 position,u16 position2){
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=broadcastaddress;
	SEM_PACKET[3]=0x0a;    //LEN=10
	SEM_PACKET[4]=SEMsyncwrite;    //INST sync write
	SEM_PACKET[5]=SEMD;    //starting address
	SEM_PACKET[6]=2;    //length of data towrite
	SEM_PACKET[7]=semaddress0;   //[1st Device] ID
	SEM_PACKET[8]=(u8)position;  
	SEM_PACKET[9]=(u8)(position>>8);   
	SEM_PACKET[10]=semaddress1;   //[1st Device] ID
	SEM_PACKET[11]=(u8)position2;  
	SEM_PACKET[12]=(u8)(position2>>8);  
	SEM_PACKET[13]=getcksm(SEM_PACKET,13);
	RS485_senddata(SEM_PACKET,14);
}//syncwrite
void SEM_readposition(u8 id){
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=id;
	SEM_PACKET[3]=0x05;    //LEN=12
	SEM_PACKET[4]=SEMread;    //write
	SEM_PACKET[5]=SEMnowposition;  
	SEM_PACKET[6]=0x02;
	SEM_PACKET[7]=getcksm(SEM_PACKET,7);
	RS485_senddata(SEM_PACKET,8);
}
void SEM_readspeed(u8 id){
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=id;
	SEM_PACKET[3]=0x05;    //LEN=12
	SEM_PACKET[4]=SEMread;    //write
	SEM_PACKET[5]=SEMnowspeed;  
	SEM_PACKET[6]=0x02;
	SEM_PACKET[7]=getcksm(SEM_PACKET,7);
	RS485_senddata(SEM_PACKET,8);
}
void SEM_readall(void){
  SEM_PACKET[0]=0xff;
	SEM_PACKET[1]=0xff;
	SEM_PACKET[2]=broadcastaddress;
	SEM_PACKET[3]=0x09;    //LEN=9
	SEM_PACKET[4]=SEMbulkread;    //INST sync write
	SEM_PACKET[5]=0x00;    //reserved as 0x00
	
	SEM_PACKET[6]=0x04;    //4 bytes to read 
	SEM_PACKET[7]=semaddress0;  
	SEM_PACKET[8]=SEMnowposition;  
	
	SEM_PACKET[9]=0x04;    //4 bytes to read
	SEM_PACKET[10]=semaddress1;  
	SEM_PACKET[11]=SEMnowposition;  
	
	SEM_PACKET[12]=getcksm(SEM_PACKET,12);
	RS485_senddata(SEM_PACKET,13);
}



 



