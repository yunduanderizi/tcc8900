#include "si47xxAMRX.h"
#include "si475xAMRX.h"


void si47xxAMRX_initialize( int rssi, int snr )
{
    setAmSnrRssi((u16)rssi,(u16)snr);
   si475xAMRX_initialize();

}

void si47xxAMRX_set_volume(int volume)
{
    si475xAMRX_set_volume((u8)(volume&0xff));

}

void si47xxAMRX_powerdown(void)
{
    si475xAMRX_powerdown();
}

void si47xxAMRX_tune(int channel)
{
    si475xAMRX_tune((u16)(channel&0xffff));
}

u8  si47xxAMRX_seek(int upDown, int stopFlag)
{
    return si475xAMRX_seek((u8)(upDown),(u8)stopFlag);
}

u16 si47xxAMRX_get_frequency(void)
{
    return si475xAMRX_get_frequency();
}

u8 si47xxAMRX_get_rssi(void)
{
    return si475xAMRX_get_rssi();
}
