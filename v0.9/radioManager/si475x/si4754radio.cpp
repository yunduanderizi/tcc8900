#include "si4754radio.h"
#include "radioDefines.h"
#include <QTimer>
#include <QSettings>
#include <QRegExp>
#include "stdlib.h"
#include <QFile>
#ifdef __cplusplus
extern "C" {
#endif
#include "si475xAMRX.h"
#include "si475xFMRX.h"
#include "radioCommon.h"

#ifdef __cplusplus
}
#endif
#define RADIO_CONFIG_FILE_PATHNAME		"/nand3/applicationdata/radio.ini"
#define AM_PATCH_FILE   "/nand3/applicationdata/fix_amrx_patch.csg"

si4754Radio::si4754Radio():m_radioMode(RM_FREQ_SECTION_NULL),
    m_currentFrequency(-1),
    m_isSearching(false),
    m_fmRssi(20),
    m_amRssi(20),
    m_fmSnr(3),
    m_amSnr(3),
    m_volumn(32)
{
    readParameterConfig();
	unsigned char * pPatch=NULL;
	u32 line = readPatchFile(AM_PATCH_FILE,&pPatch);
	setAMPatch(pPatch);
	setAMPatchLine(line);
}
int si4754Radio::closeRadio()
{
//    mute();
    return 0;
}

int si4754Radio::changeChannel(int channel)
{
    if(isFM(channel))//FM的台
    {
        if(m_radioMode == RM_FREQ_SECTION_AM)//当前是AM模式
        {
            closeAM();//关闭AM才能初始化FM
            InitFM();
        }
        else if(m_radioMode== RM_FREQ_SECTION_NULL)//当前没有设置模式
        {
            closeAM();//关闭FM才能初始化AM
            InitFM();
        }
        m_radioMode = RM_FREQ_SECTION_FM;
        changeFM(channel);

    }
    else if(isAM(channel))//AM的台
    {
        if(m_radioMode == RM_FREQ_SECTION_FM)//当前是FM模式
        {
            closeFM();//关闭FM才能初始化AM
            InitAM();
        }
        else if(m_radioMode == RM_FREQ_SECTION_NULL)//当前没有设置模式
        {
            closeFM();//关闭FM才能初始化AM
            InitAM();
        }
        m_radioMode = RM_FREQ_SECTION_AM;
        changeAM(channel);

    }
    return 1;
}

int si4754Radio::beginSearch(int channel)
{
    int res = -1;
    if(isFM(channel))//FM的台
    {
        changeChannel(channel);
        if(m_radioMode != RM_FREQ_SECTION_FM)
        {
            qWarning("[%s] FM channel ,but not FM mode",__func__);
            return -1;
        }
        m_isSearching = true;
       // mute();
        changeFM(channel);
        QTimer::singleShot(500,this,SLOT(slotSearchChannel()));
        res =0;
    }
    else
    {
        changeChannel(channel);
        if(m_radioMode != RM_FREQ_SECTION_AM)
        {
            qWarning("[%s] AM channel ,but not AM mode",__func__);
            return -1;
        }
        m_isSearching = true;
        //mute();
        changeAM(channel);
        QTimer::singleShot(500,this,SLOT(slotSearchChannel()));
        res =0;
    }
    return res;

}

int si4754Radio::stopSearch(void)
{
   m_isSearching = false;
   return 0;
}
int si4754Radio::getVersion(int size,char* buf)
{
    int res = -1;
   res = si475x_getPartInformation(size,buf);
    return res;
}

void si4754Radio::slotSearchChannel(void)
{
    int res = -1;
    if(m_isSearching)
    {
        if(m_radioMode == RM_FREQ_SECTION_FM)
        {
            res =  seekFM();
            if(!res)//为0表示收到台
            {
                int channel = si475xFMRX_get_frequency();
                if(isFM(channel*10))//在FM范围内,超范围的台丢弃
                {
                    emit searchedOneChannel(channel*10);
                    QTimer::singleShot(100,this,SLOT(slotSearchChannel()));
                }
                else
                {
                    emit searchEnd(RM_SEARCH_R_OK);
                }
            }
            else//收不到台表示收台完成
            {
                emit searchEnd(RM_SEARCH_R_OK);
            }
        }
        else
        {
            res =  seekAM();
            if(!res)//为0表示收到台
            {
                int channel = si475xAMRX_get_frequency();
                if(isAM(channel))//在AM范围内,超范围的台丢弃
                {
                    emit searchedOneChannel(channel);
                    QTimer::singleShot(100,this,SLOT(slotSearchChannel()));
                }
                else
                {
                    emit searchEnd(RM_SEARCH_R_OK);
                }
            }
            else//收不到台表示收台完成
            {
                emit searchEnd(RM_SEARCH_R_OK);
            }
        }
    }
    else
    {
        emit searchEnd(RM_SEARCH_R_CANCEL);
    }
}

void si4754Radio::readParameterConfig(void)
{
    QSettings set(RADIO_CONFIG_FILE_PATHNAME,QSettings::IniFormat,0);
    m_fmRssi = set.value("/fm_config/rssi",QVariant(m_fmRssi)).toInt();
    m_fmSnr = set.value("/fm_config/snr",QVariant(m_fmSnr)).toInt();
    m_amRssi = set.value("/am_config/rssi",QVariant(m_amRssi)).toInt();
    m_amSnr = set.value("/am_config/snr",QVariant(m_amSnr)).toInt();
	m_volumn = set.value("/fm_config/vol",QVariant(m_volumn)).toInt();
    m_volumn = 255;
	qWarning("[FM] RSSI= %d,SNR = %d  [AM] RSSI =%d ,SNR = %d m_volumn =%d\n",m_fmRssi,m_fmSnr,m_amRssi,m_amSnr,m_volumn);
}
int si4754Radio::readPatchFile(QString fileName,unsigned char **bufOut)
{
	int resCount=0;

	QFile f;
	QByteArray data;
	QList<QByteArray> line_data_list;
	QRegExp dataExp;
	QString sPartern = QString("^0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S)*");
	dataExp.setPattern(sPartern);

	f.setFileName(fileName);
	f.open(QIODevice::ReadOnly);
	data =f.readAll();
	f.close();
	line_data_list = data.split('\n');
	qWarning("[%s] [%d]\n",__func__,line_data_list.count());
	unsigned char * buf =(unsigned char * )malloc(line_data_list.count()*8);
	*bufOut = buf;
	unsigned char *p = buf;
	int res = 0;
	for(int i=0;i<line_data_list.count();i++)
	 {
		QString sData =line_data_list.at(i);
		//printf("%s\n",sData.toLocal8Bit().data());
		res = dataExp.indexIn(sData);
		//printf("res = %d\n",res);
		if(res>=0)
		{
			resCount++;
			for(int j=0;j<8;j++)
			{
				*p++ = (unsigned char)((dataExp.cap(j+1).toInt(0,16))&0xff);
				//printf("[%s][%d] ",(dataExp.cap(j+1)).toLocal8Bit().data(),*(p-1));
			}
			//printf("\n");
		}
	 }
	 printf("resCount=[%d]\n",resCount);
	 return resCount;

}

void si4754Radio::InitFM(void)
{
   setFmSnrRssi((u16)m_fmRssi,(u16)m_fmSnr);
   si475xFMRX_initialize();
   si475xFMRX_set_volume(m_volumn);
	//wait_ms(100);
	//si475xFMRX_mute(0);
}

void si4754Radio::InitAM(void)
{
    setAmSnrRssi((u16)m_amRssi,(u16)m_amSnr);
    si475xAMRX_initialize();
    si475xAMRX_set_volume(m_volumn);
	//wait_ms(100);
	//si475xAMRX_mute(0);

}
void si4754Radio::closeFM(void)
{
	si475xFMRX_mute(1);
	si475xFMRX_powerdown();
}

void si4754Radio::closeAM(void)
{
	si475xAMRX_mute(1);
	si475xAMRX_powerdown();
}

void si4754Radio::changeFM(int channel)
{
    si475xFMRX_tune(0,(u16)((channel/10)&0xffff));

}

void si4754Radio::changeAM(int channel)
{
    si475xAMRX_tune((u16)(channel&0xffff));
}

int si4754Radio::seekFM(void)
{
    return si475xFMRX_seek(1,1);// seek up, 0 = stop at band limits

}

int si4754Radio::seekAM(void)
{
    return si475xAMRX_seek(1,1);// seek up, 0 = wrap at band limits
}

void si4754Radio::mute(void)
{
    if(m_radioMode == RM_FREQ_SECTION_FM)
    {
        si475xFMRX_mute(1);
    }
    else if(m_radioMode == RM_FREQ_SECTION_AM)
    {
        si475xAMRX_mute(1);
    }
}

void si4754Radio::unMute(void)
{
    if(m_radioMode == RM_FREQ_SECTION_FM)
    {
        si475xFMRX_mute(0);
    }
    else if(m_radioMode == RM_FREQ_SECTION_AM)
    {
        si475xAMRX_mute(0);
    }
}

bool si4754Radio::isFM(int channel)
{
    if(channel>= RM_FM_FREQ_MIN && channel <= RM_FM_FREQ_MAX)
        return true;
    return false;
}

bool si4754Radio::isAM(int channel)
{
    if(channel>= RM_AM_FREQ_MIN && channel <= RM_AM_FREQ_MAX)
        return true;
    return false;

}
