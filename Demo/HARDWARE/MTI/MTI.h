#ifndef __MTI_H
#define __MTI_H
#include "sys.h"

//MTDATA2 EXAMPLE
/*
FA FF 36 31                                               Preamble+BID+MID+LEN
10 20 02 DF C5                                            XDI_PacketCOunter    U16            
10 60 04 00 45 9D A0                                      XDI_SampleTimeFine   U32
40 20 0C BE DC 9A FA 3F 54 9F 37 41 1C BB 70              XDI_Acceleration  R R R       R=Float32   m/s^2
80 20 0C BB AA 5C 80 3B 8C 55 01 BB 81 33 00              XDI_RateOfTurn    R R R       R=Float32   rad/s           
E0 20 04 00 00 00 81                                      XDI_StatusWord    U32
45
*/
//MTDATA2 EXAMPLE2
/*
FA FF 36 40                                               Preamble+BID+MID+LEN
20 10 10 x1 x2 x3 x4 y1 y2 y3 y4 z1 z2 z3 z4 w1 w2 xw3 w4 XDI_Quaternion    R R R R     R=Float32   rad/s 
40 20 0C x1 x2 x3 x4 y1 y2 y3 y4 z1 z2 z3 z4              XDI_Acceleration  R R R       R=Float32   m/s^2
80 20 0C x1 x2 x3 x4 y1 y2 y3 y4 z1 z2 z3 z4              XDI_RateOfTurn    R R R       R=Float32   rad/s  
C0 20 0C x1 x2 x3 x4 y1 y2 y3 y4 z1 z2 z3 z4              XDI_MagneticField R R R           R=Float32   a.u.
??                                                        checksum
*/
//XBUS Format
//Preamble+BID+MID+LEN+DATA+CHECKSUM

#define Preamble 0xFA
#define BID      0xFF
#define MTIreturnsize 69

//MIT instruction
#define ReqBaudrate   0x18
#define SetBaudrate   0x18
//0x09 9600,0x02 115200
#define WakeUp            0x3E //to host
#define WakeUpAck         0x3F//to mt
#define GotoConfig        0x30//to mt
#define GotoMeasurement   0x10//tomt
#define Reset             0x40//to mt
#define ReqDID            0x00//to mt
#define DeviceID          0x01//to host
#define ReqProductCode    0x1c//to mt

#define Error             0x42//to host
#define Warning           0x43//to host
#define SetOptionFlags    0x48//to mt    !!!datalen=8,4set+4clear
#define ReqLocationID     0x84//to mt   
#define SetLocationID     0x84//to mt  //2 bytes
#define ReqTransmitDelay  0xDC
#define SetTransmitDelay  0xDC//2 bytes  1 tick=33.9ns  20us->2.2ms  590->65535

//Configuration messages
#define ReqConfiguration  0x0C  
#define Configuration     0x0D//to host 118bytes
#define ReqOutputConfiguration   0xC0 //to mt
#define SetOutputConfiguration   0xC0
#define ReqData           0x34
#define MTData2           0x36//to host!!
#define ReqSyncSettings   0x2c
#define SetSyncSettings   0x2c
#define SetFilterProfile  0x64
#define SetOutputSkipfactor 0xD4


//User Config
#define Baudrate  0x09   //9600
#define Outputfre 0x01 //1Hz

u8 get_mti_checksum(u8 *buff,u8 len);
void MTI_init(void);
void MTI_setoutputconfig(void);
void MTI_setbaudrate(void);
void MTI_setfilterprofile(void);
//General 
//FA DD 64 02 00 27 checksum
void MTI_setoutputskipfactor(void);
//FA FF D4 02 FF FF checksum
void MTI_gotomeasurement(void);

extern float quaternion[4];
extern float acceleration[3];
extern float rateofturn[3];
extern float magneticfield[3];
#endif
