#include "si47xxFMRX.h"
#include "si475xFMRX.h"
void si47xxFMRX_initialize( int rssi, int snr )
{
    setFmSnrRssi((u16)rssi,(u16)snr);
   si475xFMRX_initialize();
}

void si47xxFMRX_set_volume(int volume)
{
    si475xFMRX_set_volume((u8)(volume&0xff));
}

void si47xxFMRX_powerdown(void)
{
    si475xFMRX_powerdown();
}

void si47xxFMRX_tune(int channel)
{
     si475xFMRX_tune(0,(u16)(channel&0xffff));
}

u8  si47xxFMRX_seek(int upDown, int stopFlag)
{
    return si475xFMRX_seek((u8)(upDown),(u8)stopFlag);
}

u16 si47xxFMRX_get_frequency(void)
{
   return si475xFMRX_get_frequency();
}

u8 si47xxFMRX_get_rssi(void)
{
   return si475xFMRX_get_rssi();
}
