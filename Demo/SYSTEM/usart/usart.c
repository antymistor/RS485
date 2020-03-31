#include "sys.h"
#include "usart.h"	
#include "delay.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					
#endif

  
#if 1
#pragma import(__use_no_semihosting)             
            
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	//while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	 while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
	USART6->DR = (u8) ch;      
	return ch;
}
#endif
 
	

void uart_init(u32 bound){
  
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_USART6); 
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART6, &USART_InitStructure); 
	USART_Cmd(USART6, ENABLE); 
	
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);

	
}


//void USART6_IRQHandler(void)                	
//{
//	u8 Res;
//	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  
//	{
//		Res =USART_ReceiveData(USART6);
//		
//  } 

//} 