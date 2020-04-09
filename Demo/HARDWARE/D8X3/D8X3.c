#include "sys.h"
#include "D8X3.h"	
#include "delay.h"
#include "RS485.h"
#include "RS422.h"
#include "misc.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

char PELCO_D_PACKET[7];
char PELCO_P_PACKET[8];
void RS485_senddata(u8 *buf,u8 len);


void D8X3CMD(u16 cmd,u8 data)
{
	u16 datatemp;
#ifdef PELCOD
  PELCO_D_PACKET[0]=SYSCHAR;
	PELCO_D_PACKET[1]=D8X3_address;
	PELCO_D_PACKET[2]=(u8)((cmd&0xff00)>>8);
	PELCO_D_PACKET[3]=(u8)(cmd&(0x00ff));
	datatemp= D8X3_MODATA(cmd,data)
	PELCO_D_PACKET[4]=(u8)((datatemp&0xff00)>>8);
	PELCO_D_PACKET[5]=(u8)(datatemp&(0x00ff));
	PELCO_D_PACKET[6]=crc(PELCO_D_PACKET[1],PELCO_D_PACKET[2],PELCO_D_PACKET[3],PELCO_D_PACKET[4],PELCO_D_PACKET[5]);
	#if D8X3mode==1
	RS422_senddata(PELCO_D_PACKET,7);
	#else
	RS485_senddata(PELCO_D_PACKET,7);
	#endif
	
#endif
#ifdef PELCOP
	PELCO_P_PACKET[0]=STX;
	PELCO_P_PACKET[1]=D8X3_address;
	PELCO_P_PACKET[2]=(u8)((cmd&0xff00)>>8);
	PELCO_P_PACKET[3]=(u8)(cmd&(0x00ff));
	datatemp= D8X3_MODATA(cmd,data);
	PELCO_P_PACKET[4]=(u8)((datatemp&0xff00)>>8);
	PELCO_P_PACKET[5]=(u8)(datatemp&(0x00ff));
	PELCO_P_PACKET[6]=ETX;
	PELCO_P_PACKET[7]=crc(PELCO_D_PACKET[1],PELCO_D_PACKET[2],PELCO_D_PACKET[3],PELCO_D_PACKET[4],PELCO_D_PACKET[5]);
	
	#if D8X3mode==1
	RS422_senddata(PELCO_D_PACKET,8);
	#else
	RS485_senddata(PELCO_D_PACKET,8);
	#endif
#endif
	
}


