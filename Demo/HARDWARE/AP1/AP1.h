#ifndef __AP1_H
#define __AP1_H
#include "sys.h"

//Construction
//Head+Length+Sequence+Payload+checksum

#define AP1Head1        0x55
#define AP1Head2        0xAA
#define AP1Setspeed     0x01
#define AP1Getbat       0x02
#define AP1Reset        0x05
#define AP1Clearencoder 0x06
#define AP1Error        0xff

void AP1_reset(void);
void AP1_clearencoder(void);
void AP1_getbat(void);
void AP1_setspeed(short Lspeed,short Rspeed);
u8 AP1_getchecksum(u8 *buff,u8 len);


#endif
