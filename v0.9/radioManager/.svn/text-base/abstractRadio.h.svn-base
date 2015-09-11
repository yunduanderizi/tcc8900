#ifndef ABSTRACTRADIO_H
#define ABSTRACTRADIO_H
#include <QObject>
class AbstractRadio:public QObject
{
    Q_OBJECT

public:
    virtual  int openRadio(int /*frequencySection*/,int /*channel*/,int /*volume*/){return -1;}
    virtual int closeRadio(){return -1;}

    virtual int adjustVolume(int /*volume*/,int /*mode*/){return -1;}
    virtual int getVolume(){return -1;}

    virtual int changeChannel(int /*channel*/){return -1;}

    virtual int beginSearch(int /*channel*/){return -1;}
    virtual int stopSearch(){return -1;}
    virtual int getVersion(int /*size*/,char* /*buf*/){return -1;}
signals:
    void searchChannelChanged(int lastChannel,int currentChannel);
    void searchedOneChannel(int channel);
    void searchEnd(int status);

};

#endif // ABSTRACTRADIO_H
