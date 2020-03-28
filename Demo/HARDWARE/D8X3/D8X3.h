#ifndef __D8X3_H
#define __D8X3_H
#include "sys.h"

#define D8X3_address 0x00
#define PELCOP 
//#define PELCOD


#ifdef PELCOP
	#define STX 0xA0
	#define ETX 0xAF
	#define crc(a1,a2,a3,a4,a5) ((a1)^(a2)^(a3)^(a4)^(a5))
#endif

#ifdef PELCOD
  #define SYSCHAR 0xFF
	#define crc(a1,a2,a3,a4,a5) (( (a1)+(a2)+(a3)+(a4)+(a5) )%(0x0100))
#endif

//MOTION reset
#define D8X3_reset     0x0000
//MOTION CMD
#define D8X3_right     0x0002
#define D8X3_left      0x0004
#define D8X3_up        0x0008
#define D8X3_down      0x0010
#define D8X3_auto      0x9000
// ~MOTION CMD
#define D8X3_zoomtele  0x0020
#define D8X3_zoomwide  0x0040
#define D8X3_focusfar  0x0080
#define D8X3_focusnear 0x0100
#define D8X3_irisopen  0x0200
#define D8X3_irisclose 0x0400

//~MOTION DATA
#define D8X3_UMODATA   0x0000
#define D8X3_MODATA(a,b) ( (a)>(4)?((u16)b):((u16)b<<8))  
//
void D8X3CMD(u16 cmd,u8 data);
#endif
