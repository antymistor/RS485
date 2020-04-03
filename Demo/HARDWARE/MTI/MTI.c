#include "sys.h"
#include "MTI.h"	
#include "delay.h"
#include "RS485.h"
#include "misc.h"
#include "Config.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
u8 mtibuf[100];

float quaternion[4];
float acceleration[3];
float rateofturn[3];
float magneticfield[3];

u8 get_mti_checksum(u8 *buff,u8 len)
{
	u8 temp=0x00;
	while(len>1)
  {
		temp +=buff[len-1];
		--len;
	}
	return ~temp;
}
void MTI_init(void){
	MTI_setoutputconfig();
	MTI_setbaudrate();
	MTI_setfilterprofile();
	MTI_setoutputskipfactor();
	MTI_gotomeasurement();
}
void MTI_setoutputconfig(void)
{
		mtibuf[0]=Preamble;mtibuf[1]=BID;mtibuf[2]=SetOutputConfiguration;mtibuf[3]=0x10;
		//quaternion 1Hz;
		mtibuf[4]=0x20;mtibuf[5]=0x10;mtibuf[6]=0x00;mtibuf[7]=Outputfre;
		//acceleration 1Hz
		mtibuf[8]=0x40;mtibuf[9]=0x20;mtibuf[10]=0x00;mtibuf[11]=Outputfre;
		//rate of turn  1Hz
		mtibuf[12]=0x80;mtibuf[13]=0x20;mtibuf[14]=0x00;mtibuf[15]=Outputfre;
		//magnetic field 1Hz
		mtibuf[16]=0xC0;mtibuf[17]=0x20;mtibuf[18]=0x00;mtibuf[19]=Outputfre;
	  //get checksum
		mtibuf[20]= get_mti_checksum(mtibuf,20);
		RS485_senddata(mtibuf,21);
	  delay_ms(10);
}
void MTI_setbaudrate(void)
{
  mtibuf[0]=Preamble;mtibuf[1]=BID;mtibuf[2]=SetBaudrate;mtibuf[3]=0x01;
	//9600
	mtibuf[4]=Baudrate;
	mtibuf[5]= get_mti_checksum(mtibuf,5);
	RS485_senddata(mtibuf,6);
	delay_ms(10);
}


void MTI_setfilterprofile(void)
{
	mtibuf[0]=Preamble;mtibuf[1]=BID;mtibuf[2]=SetFilterProfile;mtibuf[3]=0x02;
	//General 
	mtibuf[4]=0x00;mtibuf[5]=0x27;
	mtibuf[6]= get_mti_checksum(mtibuf,6);
	RS485_senddata(mtibuf,7);
	delay_ms(10);
}
void MTI_setoutputskipfactor(void)
{
	mtibuf[0]=Preamble;mtibuf[1]=BID;mtibuf[2]=SetOutputSkipfactor;mtibuf[3]=0x02;
	//General 
	mtibuf[4]=0xFF;mtibuf[5]=0xFF;
	mtibuf[6]= get_mti_checksum(mtibuf,6);
	RS485_senddata(mtibuf,7);
	delay_ms(10);
}
void MTI_gotomeasurement(void)
{
 //FA FF 10 00 F1
	mtibuf[0]=Preamble;mtibuf[1]=BID;mtibuf[2]= GotoMeasurement;mtibuf[3]=0x00;
	mtibuf[4]= get_mti_checksum(mtibuf,4);
	RS485_senddata(mtibuf,5);
	delay_ms(1);
}
void MTI_ReqData(void)
{
	mtibuf[0]=Preamble;mtibuf[1]=BID;mtibuf[2]= ReqData;mtibuf[3]=0x00;
	mtibuf[4]= get_mti_checksum(mtibuf,4);
	RS485_senddata(mtibuf,5);
}