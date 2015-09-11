#ifndef SI4754RADIO_H
#define SI4754RADIO_H
#include "abstractRadio.h"

class si4754Radio:public AbstractRadio
{
    Q_OBJECT

public:
    si4754Radio();
    int closeRadio();
    int changeChannel(int /*channel*/);
    int beginSearch(int /*channel*/);
    int stopSearch();
    int getVersion(int size,char* buf);

private slots:
    void slotSearchChannel(void);
private:
    void readParameterConfig(void);
	int readPatchFile(QString fileName,unsigned char **buf);
    void InitFM(void);
    void InitAM(void);
    void closeFM(void);
    void closeAM(void);
    void changeFM(int channel);
    void changeAM(int channel);
    int seekFM(void);
    int seekAM(void);
    void mute(void);
    void unMute(void);
    bool isFM(int channel);
    bool isAM(int channel);
    int m_radioMode;//AM/FM模式 或者初始化模式
    int m_currentFrequency;//当前模式
    bool m_isSearching;//是否正在搜索
    int m_fmRssi;//FM信号质量
    int m_amRssi;//am信号质量
    int m_fmSnr;//FM信噪比
    int m_amSnr;//am信噪比
    int m_volumn;//FM/AM音量

};

#endif // SI4754RADIO_H
