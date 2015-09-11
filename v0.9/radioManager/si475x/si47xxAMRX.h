#ifndef SI47XXAMRX_H
#define SI47XXAMRX_H
#include "typedefs.h"

void si47xxAMRX_initialize( int rssi, int snr );
void si47xxAMRX_set_volume(int volume);
void si47xxAMRX_powerdown(void);
void si47xxAMRX_tune(int channel);
u8  si47xxAMRX_seek(int upDown, int stopFlag);
u16 si47xxAMRX_get_frequency(void);
u8 si47xxAMRX_get_rssi(void);

#endif // SI47XXAMRX_H
