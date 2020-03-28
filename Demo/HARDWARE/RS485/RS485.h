#ifndef __RS485_H
#define __RS485_H
#include "sys.h"

void RS485_init(u32 bound);
#define RS485_TX		GPIO_Pin_2	//定义RS485的TX引脚	 
#define RS485_TX_PORT	GPIOA	
#define RS485_RX		GPIO_Pin_3	//定义RS485的RX引脚	 
#define RS485_RX_PORT	GPIOA
#define RS485_RE		GPIO_Pin_1	//定义RS485的RE引脚	 
#define RS485_RE_PORT	GPIOA

#define RS485recbufflen 100
typedef enum
{
SEND,
RECEIVE
}RS485MODE;

typedef enum
{
 NONE,
 PELCOd,
 PELCOp,
 MODBUS,
 GILLFORMAT,
}PROTOCOL_STATUS;

void RS485_modeset(RS485MODE mode);
void RS485_senddata(u8 *buf,u8 len);

#endif
