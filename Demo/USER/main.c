#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "RS485.h"
#include "MODBUS.h"
#include "GILL.h"
#include "SEMCTR.h"
#include "MTI.h"

#include "Config.h"
int main(void)
{ 
	delay_init(100);		  //��ʼ����ʱ����
	RS485_init( RS485BaudRate);
	MODBUS_init();
	GILL_enablepull();
	SEM_pidinit();
	MTI_init();
	//clock_init();
	while(1)
	{
		
  }
}
















