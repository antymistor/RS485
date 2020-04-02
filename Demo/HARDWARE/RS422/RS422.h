#ifndef __RS422_H
#define __RS422_H
#include "sys.h"

void RS422_init(u32 bound);
#define RS422_TX		GPIO_Pin_11	//定义RS485的TX引脚	 
#define RS422_TX_PORT	GPIOA	
#define RS422_RX		GPIO_Pin_12	//定义RS485的RX引脚	 
#define RS422_RX_PORT	GPIOA
#define RS422_RE		GPIO_Pin_13	//定义RS485的RE引脚	 
#define RS422_RE_PORT	GPIOA

#define RS422recbufflen 100
typedef enum
{
SEND,
RECEIVE
}RS422MODE;

void RS422_modeset(RS422MODE mode);
void RS422_senddata(u8 *buf,u8 len);

#endif
