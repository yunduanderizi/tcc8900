//-----------------------------------------------------------------------------
//
// si475xAMRX.h
//
// Contains the function prototypes for the functions contained in si475xAMRX.c
//
//-----------------------------------------------------------------------------
#ifndef _SI475XAMRX_H_
#define _SI475XAMRX_H_
#include "radioCommon.h"
//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
extern unsigned char PoweredUp;

//-----------------------------------------------------------------------------
// Function prototypes for si475xAMRX.c
//-----------------------------------------------------------------------------
void si475xAMRX_powerup(void);
void si475xAMRX_powerdown(void);
void si475xAMRX_initialize( void);
void si475xAMSWLWRX_initialize(void);
void si475xAMRX_configure(void);
void si475xAMRX_set_volume(u8 volume);
void si475xAMRX_mute(u8 mute);
u8 si475xAMRX_tune(u16 frequency);
u8 si475xAMRX_seek(u8 seekup, u8 seekmode);
u16 si475xAMRX_get_frequency(void);
u8 si475xAMRX_get_rssi(void);
void setAmSnrRssi(u16 rssi,u16 snr);
void setAMPatch(u8 * p);
void setAMPatchLine(u32 line);

//-----------------------------------------------------------------------------
// Function prototypes for AMRXautoseek.c
//-----------------------------------------------------------------------------
u8 si475xAMRX_autoseek(void);

void si475x_set_property(u16 propNumber, u16 propValue);

#endif
