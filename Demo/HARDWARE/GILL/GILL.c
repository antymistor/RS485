#include "sys.h"
#include "GILL.h"	
#include "delay.h"
#include "RS485.h"
#include "misc.h"
#include "Config.h"

u16 winddirection=0;
float windspeed=0;
u8 GILL_PACKET[10];
void GILL_toread(void)
{
	GILL_PACKET[0]=GILLADDRESS;
	RS485_senddata(GILL_PACKET,1);
}
void GILL_enablepull(void)
{
	GILL_PACKET[0]=GILLENABLEPULL;
	RS485_senddata(GILL_PACKET,1);
}
void GILL_disablepull(void)
{
	GILL_PACKET[0]=GILLDISABLEPULL;
	RS485_senddata(GILL_PACKET,1);
}

u8 GILL_crc(u8 *buffer)
{
	u8 crcget;
	u8 crctemp=0x00;
	u8 counter=18;
	if(buffer[1]!=GILLADDRESS){return 0;}
	crcget=(buffer[20]-'0')*10+(buffer[21]-'0');
	while(counter)
	{
		crctemp ^=buffer[counter];
		counter--;
	}
	if(crctemp!=crcget){return 0;}
	return 1;
	
}