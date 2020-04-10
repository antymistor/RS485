#include "sys.h"
#include "AP1.h"	
#include "delay.h"
#include "RS232.h"
#include "misc.h"
#include "Config.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
u8 AP1_PACKET[20];

u8 sequencecounter=0x01;
AP1Car ap1={0,0,50,true,true,Error_None};


void AP1_reset(void)
{
	++sequencecounter;
  AP1_PACKET[0]=AP1Head1;
	AP1_PACKET[1]=AP1Head1;
	AP1_PACKET[2]=0x02;
	AP1_PACKET[3]=sequencecounter;
	AP1_PACKET[4]=AP1Reset;
	AP1_PACKET[5]=00;
	AP1_PACKET[6]=AP1_getchecksum(AP1_PACKET,6);
	RS232_senddata(AP1_PACKET,7);
	sequencecounter=0x00;
	
}
void AP1_clearencoder(void)
{
	AP1_reset();
	++sequencecounter;
  AP1_PACKET[0]=AP1Head1;
	AP1_PACKET[1]=AP1Head2;
	AP1_PACKET[2]=0x02;
	AP1_PACKET[3]=sequencecounter;
	AP1_PACKET[4]=AP1Clearencoder;
	AP1_PACKET[5]=00;
	AP1_PACKET[6]=AP1_getchecksum(AP1_PACKET,6);
	RS232_senddata(AP1_PACKET,7);
}

void AP1_setspeed(short Lspeed,short Rspeed)
{
	AP1_reset();
  ++sequencecounter;
  AP1_PACKET[0]=AP1Head1;
	AP1_PACKET[1]=AP1Head2;
	AP1_PACKET[2]=0x09;
	AP1_PACKET[3]=sequencecounter;
	AP1_PACKET[4]=AP1Setspeed;
	
	AP1_PACKET[5]=(u8)(Lspeed>>8);
	AP1_PACKET[6]=(u8)(Lspeed);
	
	AP1_PACKET[7]=(u8)(Rspeed>>8);
	AP1_PACKET[8]=(u8)(Rspeed);
	
	AP1_PACKET[9]=AP1_PACKET[10]=AP1_PACKET[11]=AP1_PACKET[12]=0x00;
	
	AP1_PACKET[13]=AP1_getchecksum(AP1_PACKET,13);
	RS232_senddata(AP1_PACKET,14);
}

void AP1_getbat(void)
{
	AP1_reset();
  ++sequencecounter;
  AP1_PACKET[0]=AP1Head1;
	AP1_PACKET[1]=AP1Head2;
	AP1_PACKET[2]=0x02;
	AP1_PACKET[3]=sequencecounter;
	AP1_PACKET[4]=AP1Getbat;
	AP1_PACKET[5]=00;
	AP1_PACKET[6]=AP1_getchecksum(AP1_PACKET,6);
	RS232_senddata(AP1_PACKET,7);
}
u8 AP1_getchecksum(u8 *buff,u8 len)
{
  u8 crctemp=0;
	while(len--)
  {
		crctemp ^=buff[len];
	}
	return crctemp;
}

