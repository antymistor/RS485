#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "RS485.h"
#include "MODBUS.h"
#include "GILL.h"
#include "Config.h"
int main(void)
{ 
	delay_init(100);		  //初始化延时函数
	RS485_init( RS485BaudRate);
	MODBUS_init();
	GILL_enablepull();
	//clock_init();
	while(1)
	{
		
  }
}
















