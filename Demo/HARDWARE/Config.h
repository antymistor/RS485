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
}PROTOCOL_STATUS;
typedef union{
	u8 ch[4];
	float fl;
}c4d;



#define RS485BaudRate  9600
#define RS422BaudRate  9600

#endif
