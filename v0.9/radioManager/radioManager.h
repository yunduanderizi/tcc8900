#ifndef RADIO_MANAGER_H_20100826
#define RADIO_MANAGER_H_20100826

#include <QObject>
#include "radioDefines.h"

class AbstractRadio;
typedef enum _RADIO_TYPE_
{
    RADIO_SI4754_OLD,
    RADIO_SI4754
}Radio_Type;

class RadioManagerC:public QObject
{
	Q_OBJECT
public:
	RadioManagerC();
	~RadioManagerC();

public:
	int openRadio(int frequencySection,int channel,int volume);
	int closeRadio();

	int adjustVolume(int volume,int mode);
	int getVolume();

	int changeChannel(int channel);

	int beginSearch(int channel);
	int stopSearch();
    int getVersion(int size,char* buf);

signals:
	void searchChannelChanged(int lastChannel,int currentChannel);
	void searchedOneChannel(int channel);
	void searchEnd(int status);

private:
    AbstractRadio * radioFactory(Radio_Type  radioType);
    AbstractRadio * m_radio;
};

#endif

