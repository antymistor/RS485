#ifndef __AP1_H
#define __AP1_H
#include "sys.h"
#include "stdbool.h"
//Construction
//Head+Length+Sequence+Payload+checksum

#define AP1Head1        0x55
#define AP1Head2        0xAA
#define AP1Setspeed     0x01
#define AP1Getbat       0x02
#define AP1Reset        0x05
#define AP1Clearencoder 0x06
#define AP1Error        0xff

//error code


//Mechanical para
#define PI 3.1415936f
#define wheel_radius 0.125f   //Unit:m
#define encoder_resolution 1600 
#define PID_RATE 50        //Unit:Hz

//User define
#define AP1lenposition 2
typedef enum
{
  Error_None,
	out_of_power,
	over_current,
	uart_breakdown,
	wheel_stuck,
}AP1errorstate;

typedef struct
{
	u16 L_encoder;
	u16 R_encoder;
	u8 Power;
	bool resetstate;
	bool clearstate;
	AP1errorstate errorstate;
}AP1Car;

extern AP1Car ap1;
void AP1_reset(void);
void AP1_clearencoder(void);
void AP1_getbat(void);
void AP1_setspeed(short Lspeed,short Rspeed);
u8 AP1_getchecksum(u8 *buff,u8 len);


#endif
