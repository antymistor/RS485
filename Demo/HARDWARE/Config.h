#ifndef __Config_H
#define __Config_H
#include "sys.h"
typedef enum
{
 NONE,
 PELCOd,
 PELCOp,
 MODBUS,
 GILLFORMAT,
 SEMOTOR,
 MTI,
 AP1,
}PROTOCOL_STATUS;
typedef union{
	u8 ch[4];
	float fl;
}c4d;

typedef enum
{
SEND,
RECEIVE
}RSMODE;

#define RS485BaudRate  9600
#define RS422BaudRate  9600
#define RS232BaudRate  115200

#endif
