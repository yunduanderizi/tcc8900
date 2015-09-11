//-----------------------------------------------------------------------------
//
// si475xFMRX.h
//
// Contains the function prototypes for the functions contained in si475xFMRX.c
//
//-----------------------------------------------------------------------------
#ifndef _SI475XFMRX_H_
#define _SI475XFMRX_H_
#include "radioCommon.h"
//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
extern unsigned char PoweredUp;

//-----------------------------------------------------------------------------
// Function prototypes for si475xFMRX.c
//-----------------------------------------------------------------------------
void si475xFMRX_powerup(void);
void si475xFMRX_powerdown(void);
void si475xFMRX_initialize(void);
void si475xFMRX_configure(void);
void si475xFMRX_set_volume(u8 volume);
void si475xFMRX_mute(u8 mute);
u8 si475xFMRX_tune(u8 quick, u16 frequency);
u8 si475xFMRX_seek(u8 seekup, u8 seekmode);
u16 si475xFMRX_get_frequency(void);
u8 si475xFMRX_get_rssi(void);
void fmRdsStatus(u8 intack, u8 mtfifo);
void setFmSnrRssi(u16 rssi,u16 snr);

//-----------------------------------------------------------------------------
// Function prototypes for autoseek.c
//-----------------------------------------------------------------------------
u8 si475xFMRX_autoseek(void);

//-----------------------------------------------------------------------------
// Function prototypes for si475x_low.c
//-----------------------------------------------------------------------------
void si475x_set_property(u16 propNumber, u16 propValue);
int si475x_getPartInformation(int size,char * buf);

#endif
