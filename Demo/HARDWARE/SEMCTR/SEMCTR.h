#ifndef __SEMCTR_H
#define __SEMCTR_H
#include "sys.h"



//MODE: MX106 R!!
//SEMOTOR address
#define semaddress0      0x03
#define semaddress1      0x04
#define broadcastaddress 0xfe
#define SEMHEAD          0xff
#define SEMRETURNSIZE    10
//instruction 
#define SEMping         0x01 //Ping	Instruction that checks whether the Packet has arrived to a device with the same ID as Packet ID
#define SEMread         0x02 //Read	Instruction to read data from the Device
#define SEMwrite        0x03 //Write	Instruction to write data on the Device
#define SEMregwrite     0x04 //Reg Write	Instruction that registers the Instruction Packet to a standby status;
                             //Packet is later executed through the Action instruction
#define SEMaction       0x05 //Instruction that executes the Packet that was registered beforehand using Reg Write
#define SEMfactoryreset 0x06 //Factory Reset	Instruction that resets the Control Table to its initial factory default settings
#define SEMreboot       0x08 //Reboot	Instruction that reboots DYNAMIXEL(See applied products in the description)
#define SEMsyncwrite    0x83 //Sync Write	For multiple devices, Instruction to write data on the same Address with the same length at once
#define SEMbulkread     0x92 //Bulk Read	For multiple devices, Instruction to write data on different Addresses with different lengths at once
                             //This command can only be used with MX series.

//register address  EEPROM area
#define SEMidreg        0x03
#define SEMbaudreg      0x04 //0:2M 1:1M 3:0.5M ...16:115200 207:9600
#define SEMdelayreg     0x05 //unit:2us
#define SEMcwlimitreg   0x06
#define SEMccwlimitreg  0x08

//register address  ram area
#define SEMtorqueenable 0x18
#define SEMD            0x1a
#define SEMI            0x1b
#define SEMP            0x1c
#define SEMgoalposition 0x1e
#define SEMmovingspeed  0x20
#define SEMtorquelimit  0x22
#define SEMnowposition  0x24 //max=4095 unit:0.088deg
#define SEMnowspeed     0x26 //range:0~2047 ,>=1024 means cw direction otherwise means ccw,unit:0.11rpm
#define SEMnowload      0x28
#define SEMnowvoltage   0x2a
#define SEMTtemperature 0x2b

//user cofig
#define P1              0x20
#define I1              0x00
#define D1              0x00
#define P2              0x20
#define I2              0x00
#define D2              0x00

u8 getcksm(u8 *buff,u8 len);
void SEM_reboot(void);
void SEM_pidinit(void);
void SEM_setposition(u8 id,u16 position);
void SEM_setallposition(u16 position,u16 position2); //syncwrite
void SEM_readposition(u8 id);
void SEM_readspeed(u8 id);
void SEM_readall(void);

extern u16 motor0position;
extern u16 motor1position;
extern u16 motor0speed;
extern u16 motor1speed;
#endif
