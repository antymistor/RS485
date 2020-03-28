#include "sys.h"
#include "MODBUS.h"	
#include "delay.h"
#include "RS485.h"
#include "misc.h"
#include "Config.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
u8 MODBUS_PACKET[10];
u16 hfwordtemp;

u16  pressureformat;
u16  pressureint;
float pressurefloat;
float pressurepercent;

unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
void MODBUS_SEND(u8 CMD,u16 address,u16 data);
void MODBUS_SEND2(u8 CMD,u16 registeraddress,u16 data,u8 objectaddress);
void MODBUS_init(void)
{
	//RS485BaudRate
	//setaddress
	MODBUS_SEND2(SET,MODBUSADDRESS,barometeraddress,0x00);
	//setBaudRate
	MODBUS_SEND(SET,MODBUSBAUD,0x0003);//9600
	//setPartity
	MODBUS_SEND(SET,MODBUSPARITY,0x0000);//None
	//setUnit
	MODBUS_SEND(SET,PRESSUREUNIT,0x0000);//Pa
  //setfre
	MODBUS_SEND(SET,SAMPLEFRE,0x0000);//10Hz
}

void MODBUS_toread(void)
{
	MODBUS_SEND(READ,PRESSUREFORMAT,0x0006);	
}

void MODBUS_SEND2(u8 CMD,u16 registeraddress,u16 data,u8 objectaddress)
{
	MODBUS_PACKET[0]=(u8)objectaddress;   
	MODBUS_PACKET[1]=CMD;
	MODBUS_PACKET[2]=(u8)registeraddress;
	MODBUS_PACKET[3]=(u8)(registeraddress>>8);
	MODBUS_PACKET[4]=(u8)data;
	MODBUS_PACKET[5]=(u8)(data>>8);
	hfwordtemp=CRC16_MODBUS(MODBUS_PACKET,6);
	MODBUS_PACKET[6]=(u8)hfwordtemp;
	MODBUS_PACKET[7]=(u8)(hfwordtemp>>8);
	RS485_senddata(MODBUS_PACKET,8);
}
void MODBUS_SEND(u8 CMD,u16 registeraddress,u16 data)
{
	MODBUS_PACKET[0]=(u8)barometeraddress;     //depend on inital address
	MODBUS_PACKET[1]=CMD;
	MODBUS_PACKET[2]=(u8)registeraddress;
	MODBUS_PACKET[3]=(u8)(registeraddress>>8);
	MODBUS_PACKET[4]=(u8)data;
	MODBUS_PACKET[5]=(u8)(data>>8);
	hfwordtemp=CRC16_MODBUS(MODBUS_PACKET,6);
	MODBUS_PACKET[6]=(u8)hfwordtemp;
	MODBUS_PACKET[7]=(u8)(hfwordtemp>>8);
	RS485_senddata(MODBUS_PACKET,8);
}
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
{
	int i;
	unsigned char tmp[4];
	tmp[0] = 0;
	for (i = 0; i< 8; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (7 - i);
	}
	dBuf[0] = tmp[0];
 
}
 
void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf)
{
	int i;
	unsigned short tmp[4];
	tmp[0] = 0;
	for (i = 0; i< 16; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}

unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)  
{  
    unsigned short wCRCin = 0xFFFF;  
    unsigned short wCPoly = 0x8005;  
    unsigned char wChar = 0;  
     
     while (usDataLen--)     
     {  
         wChar = *(puchMsg++);  
         InvertUint8(&wChar, &wChar);  
         wCRCin ^= (wChar << 8); 
         for(int i = 0; i < 8; i++)  
        {  
             if(wCRCin & 0x8000) 
             {
                wCRCin = (wCRCin << 1) ^ wCPoly;  
             }
            else  
             {
                wCRCin = wCRCin << 1; 
             }            
         }  
    }  
    InvertUint16(&wCRCin, &wCRCin);  
    return (wCRCin) ;  
 } 