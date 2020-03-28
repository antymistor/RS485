#ifndef __GILL_H
#define __GILL_H
#include "sys.h"

#define GILLADDRESS 'Q'
#define GILLSTX  0x02
#define GILLETX  0x03
#define GILLUNIT 'M'
#define GILLENABLEPULL '?'
#define GILLDISABLEPULL '!'
#define GILLRETURNSIZE 22
extern u16 winddirection;
extern float windspeed;
void GILL_toread(void);
void GILL_enablepull(void);
void GILL_disablepull(void);
u8 GILL_crc(u8 *buffer);
#endif
