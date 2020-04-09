#ifndef __RS232_H
#define __RS232_H
#include "sys.h"
#include "Config.h"
void RS232_init(u32 bound);
#define RS232_TX		GPIO_Pin_9	//����RS485��TX����	 
#define RS232_TX_PORT	GPIOA	
#define RS232_RX		GPIO_Pin_10	//����RS485��RX����	 
#define RS232_RX_PORT	GPIOA
#define RS232_RE		GPIO_Pin_8	//����RS485��RE����	 
#define RS232_RE_PORT	GPIOA

#define RS232recbufflen 100


void RS232_modeset(RSMODE mode);
void RS232_senddata(u8 *buf,u8 len);

#endif
