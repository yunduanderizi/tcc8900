#include "radioManager.h"
#include "abstractRadio.h"

#include "radioManagerOld.h"
#include "si4754radio.h"

RadioManagerC::RadioManagerC()
{
    m_radio=NULL;
    m_radio = radioFactory(RADIO_SI4754);
    if(m_radio)
    {
        connect(m_radio,SIGNAL(searchChannelChanged(int,int)),this,SIGNAL(searchChannelChanged(int,int)));
        connect(m_radio,SIGNAL(searchedOneChannel(int)),this,SIGNAL(searchedOneChannel(int)));
        connect(m_radio,SIGNAL(searchEnd(int)),this,SIGNAL(searchEnd(int)));
    }
}
RadioManagerC::~RadioManagerC()
{
    if(m_radio)
        delete m_radio;
}

int RadioManagerC::openRadio(int frequencySection,int channel,int volumeArg)
{
    if(m_radio)
        return m_radio->openRadio(frequencySection,channel,volumeArg);
    else
        return -1;
}

int RadioManagerC::closeRadio()
{
    if(m_radio)
        return m_radio->closeRadio();
    else
        return -1;
}


int RadioManagerC::adjustVolume(int volumeArg,int mode)
{
    if(m_radio)
        return m_radio->adjustVolume(volumeArg,mode);
    else
        return -1;
}

int RadioManagerC::getVolume()
{
    if(m_radio)
        return m_radio->getVolume();
    else
        return -1;

}


int RadioManagerC::changeChannel(int channel)
{
    if(m_radio)
        return m_radio->changeChannel(channel);
    else
        return -1;
}


int RadioManagerC::beginSearch(int channel)
{
    if(m_radio)
        return m_radio->beginSearch(channel);
    else
        return -1;
}

int RadioManagerC::stopSearch()
{
    if(m_radio)
        return m_radio->stopSearch();
    else
        return -1;

}
int RadioManagerC::getVersion(int size,char* buf)
{
    if(m_radio)
    {
        return m_radio->getVersion(size,buf);
    }
    return -1;
}

AbstractRadio* RadioManagerC::radioFactory(Radio_Type  radioType)
{
    AbstractRadio * pRadio =NULL;
    switch(radioType)
    {
        case RADIO_SI4754_OLD:
            pRadio =  new RadioManagerOld;
            break;
        case RADIO_SI4754:
             pRadio = new si4754Radio;
            break;
        default:
            break;
    }
    return pRadio;
}


