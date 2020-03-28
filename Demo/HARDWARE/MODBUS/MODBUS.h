#ifndef __MODBUS_H
#define __MODBUS_H
#include "sys.h"

//objectaddress
#define barometeraddress 0x0001

//CMD construction
#define READ 0x03
#define SET  0x06
#define MSET 0x10

//key register address
#define PRESSUREFORMAT  0x0000
#define PRESSUREINT     0x0001
#define PRESSUREFLOAT   0x0002
#define PRESSUREPERCENT 0x0004

#define PRESSURETRI     0x0008
#define FORMATZERO      0x000B
#define FORMATFULL      0x000C
#define DECIMALPLACES   0x000D
#define PRESSUREUNIT    0x000E
#define MODBUSADDRESS   0x000F
#define MODBUSBAUD      0x0010
#define MODBUSPARITY    0x0011
#define SAMPLEFRE       0x0012

//return size
#define MODBUSRETURNSIZE 17
extern u16  pressureformat;
extern u16  pressureint;
extern float pressurefloat;
extern float pressurepercent;

void MODBUS_init(void);
void MODBUS_toread(void);
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
#endif
