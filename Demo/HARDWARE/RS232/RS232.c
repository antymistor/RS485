#include "sys.h"
#include "RS232.h"	
#include "delay.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "AP1.h"
#include "Config.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//RS422 Pin-Assign
//RS422_TX:PA11
//RS422_RX:PA12
//RS422_RE:PA13


PROTOCOL_STATUS readstatus232=NONE;//0:free 1:MODEBUS 2:...

u8 RS232recbuff[RS232recbufflen];
u8 RS232recounter=0;
u8 RS232full=100;

void RS232_init(u32 bound){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART6);
	
  GPIO_InitStructure.GPIO_Pin = RS232_TX | RS232_RX; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(RS232_TX_PORT,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = RS232_RX;
	GPIO_Init(RS232_RX_PORT,&GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = RS232_RE; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(RS232_RE_PORT,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure); 
	
  USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_ResetBits(RS232_RE_PORT,RS232_RE);
	
}

void RS232_modeset(RSMODE mode){
	if(mode==RECEIVE){
		GPIO_ResetBits(RS232_RE_PORT,RS232_RE);
	}
	else if(mode==SEND){
		GPIO_SetBits(RS232_RE_PORT,RS232_RE);
	}
}


void RS232_senddata(u8 *buf,u8 len)
{
	u8 t;
	RS232_modeset(SEND);		
  	for(t=0;t<len;t++)		
	{
	  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){} 
    USART_SendData(USART1,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
	RS232_modeset(RECEIVE);			
}

void USART1_IRQHandler(void)
{
	u8 res;	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	 	
	  res =USART_ReceiveData(USART1);
	 if(readstatus232==NONE&&res==(u8)(AP1Head1))
		{
			readstatus232=AP1;
			RS232recounter=0;
			RS232recbuff[RS232recounter]=res;
			++RS232recounter;
			return;
		}
		switch(readstatus232)
		{
			case AP1:
				if(RS232recounter==AP1lenposition)
				{
				  RS232recbuff[RS232recounter]=res;
				  ++RS232recounter;
				  RS232full=res+5;
				}
				else if(RS232recounter==RS232full-1)
				{
					RS232recbuff[RS232recounter]=res;
					RS232recounter=0;
				  readstatus232=NONE;
					RS232full=100;
					switch(RS232recbuff[4])
					{
						case AP1Setspeed:
							ap1.L_encoder=((u16)RS232recbuff[5])<<8+(u16)RS232recbuff[6];
						  ap1.R_encoder=((u16)RS232recbuff[7])<<8+(u16)RS232recbuff[8];
							break;
						case AP1Getbat:
							ap1.Power=RS232recbuff[5];
							break;
						case AP1Reset:
							ap1.resetstate=(bool)RS232recbuff[5];
							break;
						case AP1Clearencoder:
							ap1.clearstate=(bool)RS232recbuff[5];
							break;
						case AP1Error:
							ap1.errorstate=(AP1errorstate) RS232recbuff[5];
								
							break;
						default:
							break;
					}
					
				}
				else{
				RS232recbuff[RS232recounter]=res;
				++RS232recounter;
				}
				
				break;
			default:
				break;
		}
	}
	
} 

 



