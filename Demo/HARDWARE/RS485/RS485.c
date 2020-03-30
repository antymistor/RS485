#include "sys.h"
#include "RS485.h"	
#include "delay.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "MODBUS.h"
#include "GILL.h"
#include "SEMCTR.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//RS485 Pin-Assign
//RS485_TX:PA2
//RS485_RX:PA3
//RS485_RE:PA1
typedef union{
	u8 ch[4];
	float fl;
}c4d;

PROTOCOL_STATUS readstatus=NONE;//0:free 1:MODEBUS 2:...
u8 RS485recbuff[RS485recbufflen];
u8 RS485recounter=0;
c4d tempunion;

u16 tempu16=0;
u32 tempu32=0;
void RS485_init(u32 bound){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
  GPIO_InitStructure.GPIO_Pin = RS485_TX | RS485_RX; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(RS485_TX_PORT,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = RS485_RX;
	GPIO_Init(RS485_RX_PORT,&GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = RS485_RE; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(RS485_RE_PORT,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure); 
	
  USART_Cmd(USART2, ENABLE);
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_ResetBits(RS485_RE_PORT,RS485_RE);
	
}

void RS485_modeset(RS485MODE mode){
	if(mode==RECEIVE){
		GPIO_ResetBits(RS485_RE_PORT,RS485_RE);
	}
	else if(mode==SEND){
		GPIO_SetBits(RS485_RE_PORT,RS485_RE);
	}
}


void RS485_senddata(u8 *buf,u8 len)
{
	u8 t;
	RS485_modeset(SEND);		
  	for(t=0;t<len;t++)		
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET){} 
    USART_SendData(USART2,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
	RS485_modeset(RECEIVE);			
}

void USART2_IRQHandler(void)
{
	u8 res;	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	 	
	  res =USART_ReceiveData(USART2);
		if(readstatus==NONE&&res==(u8)(barometeraddress))
		{
			readstatus=MODBUS;
			RS485recounter=0;
		  RS485recbuff[RS485recounter]=res;
			++RS485recounter;
			return;
		}
		if(readstatus==NONE&&res==GILLSTX)
		{
			readstatus=GILLFORMAT;
			RS485recounter=0;
		  RS485recbuff[RS485recounter]=res;
			++RS485recounter;
			return;
		}
		if(readstatus==NONE&&res==SEMHEAD)
		{
			readstatus=SEMOTOR;
			RS485recounter=0;
		  RS485recbuff[RS485recounter]=res;
			++RS485recounter;
			return;
		}
		switch(readstatus){
			case NONE:
				break;
			case PELCOd:
				break;
			case PELCOp:
				break;
			case MODBUS:
				if(RS485recounter>=(MODBUSRETURNSIZE-1))
				{
				 RS485recbuff[RS485recounter]=res;
				 RS485recounter=0;
				 readstatus=NONE;
				 
				 tempu16=CRC16_MODBUS(RS485recbuff,MODBUSRETURNSIZE-2);
				 if(tempu16==((u16)RS485recbuff[15]+(((u16)RS485recbuff[16])<<8)))
					{
					 pressureformat=(u16)RS485recbuff[3]+(((u16)RS485recbuff[4])<<8);
					 pressureint=(u16)RS485recbuff[5]+(((u16)RS485recbuff[6])<<8);
					 //tempu32=(u32)RS485recbuff[7]|(((u32)RS485recbuff[8])<<8)|(((u32)RS485recbuff[9])<<16)|(((u32)RS485recbuff[10])<<24);
					 //pressurefloat=*(float *)(&tempu32);
					 //tempu32=(u32)RS485recbuff[11]|(((u32)RS485recbuff[12])<<8)|(((u32)RS485recbuff[13])<<16)|(((u32)RS485recbuff[14])<<24);
					 //pressurepercent=*(float *)(&tempu32);;
					 
					 //stm32 is little Endian Mode!!!
						tempunion.ch[0]=RS485recbuff[7];
						tempunion.ch[1]=RS485recbuff[8];
						tempunion.ch[2]=RS485recbuff[9];
						tempunion.ch[3]=RS485recbuff[10];
						pressurefloat=tempunion.fl;
						tempunion.ch[0]=RS485recbuff[11];
						tempunion.ch[1]=RS485recbuff[12];
						tempunion.ch[2]=RS485recbuff[13];
						tempunion.ch[3]=RS485recbuff[14];
						pressurefloat=tempunion.fl;
					}
					
				}
				else{
				RS485recbuff[RS485recounter]=res;
				++RS485recounter;
				}
				break;
		 case GILLFORMAT:
			  if(RS485recounter>=(GILLRETURNSIZE-1)){
				 RS485recbuff[RS485recounter]=res;
				 RS485recounter=0;
				 readstatus=NONE;
				 if(GILL_crc(RS485recbuff))
				 {
				 winddirection=(RS485recbuff[3]-'0')*100
					            +(RS485recbuff[4]-'0')*10
					            +(RS485recbuff[5]-'0');
				 windspeed=(float)((RS485recbuff[7]-'0')*100
					        +(RS485recbuff[8]-'0')*10
			            +(RS485recbuff[9]-'0'))
				          +(float)(RS485recbuff[11]-'0')/10
						      +(float)(RS485recbuff[12]-'0')/100;
				 }
				}
				else{
				RS485recbuff[RS485recounter]=res;
				++RS485recounter;
				}
				break;
		 case SEMOTOR:
			 if(RS485recounter>=(SEMRETURNSIZE-1))
			 {
				 RS485recbuff[RS485recounter]=res;
				 RS485recounter=0;
				 readstatus=NONE;
				 if(getcksm(RS485recbuff,SEMRETURNSIZE-1)==RS485recbuff[SEMRETURNSIZE-1])
         {
					 if(RS485recbuff[2]==semaddress0){
						 motor0position=(u16)RS485recbuff[5]+((u16)RS485recbuff[6])<<8;
						 motor0speed=(u16)RS485recbuff[7]+((u16)RS485recbuff[8])<<8;
					 }
					 else if(RS485recbuff[2]==semaddress1){
						 motor1position=(u16)RS485recbuff[5]+((u16)RS485recbuff[6])<<8;
						 motor1speed=(u16)RS485recbuff[7]+((u16)RS485recbuff[8])<<8;
					 }
				 }
			 }
			 else
       {
				RS485recbuff[RS485recounter]=res;
				++RS485recounter;
			 }
		}
			
	}  											 
} 

 



