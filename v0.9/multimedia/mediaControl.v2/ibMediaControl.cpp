/********************************************************
*文件名：ibmediacontrolc.cpp
*描   述：处理和mplayer的进程交互
*作   者:   胡丰凯
*时   间:   2011-8-16
*
************************************************************/
#include "ibMediaControl.h"
#include "trace.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include "tccfb_ioctl.h"
#include "stdlib.h"


//#define MPLAYER_PATH "mplayer"
#define MPLAYER_PATH "nice"
namespace MediaControl2
{
//-----------------------------------------------------------------------------------------------------------
//-IBMediaControlCData 类
    class IBMediaControlCData
    {
        public:
            IBMediaControlCData(IBMediaControlC* q_main);
            bool parseMediaInfo(const QString & line_data);
            bool parseStateChanged(const QString & line_data);
            bool parseTimeTick(const QString & line_data);
            bool parseTotalTime(const QString & line_data);
            bool parseError(const QString & line_data);

            int    parseMediaTypeByName(const QString & file_name);
            QStringList getAudioSupportList(void);
            QStringList getVideoSupportList(void);

       protected:
            bool isTotalTime(const QString & line_data,int &total_tme);
            bool isFileName(const QString & line_data,QString &file_name);
            bool isPlayState(const QString & line_data);
            bool isPauseState(const QString & line_data);
            bool isStopState(const QString & line_data);
            bool isPlayFinishState(const QString & line_data);
            bool isTimeTick(const QString & line_data,int &current);
            bool isVideoMedia(const QString & file_name);
            bool isMusicMedia(const QString & file_name);
            bool isNetMusicMedia(const QString & file_name);
            bool isFileNotFound(const QString & line_data);

            QString timeIntToString(int time,int format);
            QString intTo2String(int);

            void timeClear(void);
            void initVideoFrameBuffer(void);

            void frameBufferClose(char* dev);
            void frameBufferOpen(char* dev);
            void frameBufferSetAlpha(char* dev,int value);
            QString getEqualizerString(void);


        private:
            QRegExp m_rx_file_name; //解析播放的文件名
            QRegExp m_rx_play_state;//解析开始播放状态
            QRegExp m_rx_pause_state;//解析暂停播放状态
            QRegExp m_rx_play_finish_state;//解析播放完成状态
            QRegExp m_rx_time_tick;//解析时间更新
            QRegExp m_rx_total_length;//解析媒体文件播放总时长
            QRegExp m_rx_stop_quit;////解析播放退出中止状态
            QRegExp m_rx_crash_quit;////解析播放死掉中止状态
            QRegExp m_rx_qprocess_crash_quit;//
            QRegExp m_rx_file_not_found;//解析是否为视屏媒体





            QString m_s_total_time;//总的播放时间字符串
            QString m_s_current_media;//当前播放时间字符串
            int m_current_play_state;//当前的播放器状态
            int m_i_current_time;//当前播放时间秒为单位
            int m_i_total_time;//总的播放放时间
            int m_time_format;//时间字符串显示的格式

            int m_pause_count;//处理暂停之后重新播放，mplayer不发播放信号
            bool m_fullscreen_window_mode;
            bool m_video_sreen_keeping;
          //  bool m_video_media; //是否为视屏媒体
            Media_Type m_media_type;//媒体格式
            IBMediaControlC * q;
            bool m_is_fb0_open;//视屏层是否已打开
            int m_audio_effect;//当前的音效模式
            int m_equalizer_array[MAX_EQUALIZER];

         friend class IBMediaControlC;
    };

   IBMediaControlCData::IBMediaControlCData(IBMediaControlC* q_main):
   	m_rx_file_name("^(ID_FILENAME=)(.*)$"),
       m_rx_play_state("^Starting playback"),
       m_rx_pause_state("^ID_PAUSED$"),
       m_rx_play_finish_state("^ID_EXIT=EOF$"),
       m_rx_time_tick("^[AV]: *([0-9,:.-]+)"),
       m_rx_total_length("^ID_LENGTH=(.*)"),
       m_rx_stop_quit("^ID_EXIT=QUIT$"),
       m_rx_crash_quit("^- MPlayer crashed. This shouldn't happen"),
       m_rx_qprocess_crash_quit("^QProcess Crashed"),
       m_rx_file_not_found("^File not found"),
       m_current_play_state(MEDIA_RUNNING_STATE_STOP),
       m_time_format(TIME_FORMAT_1_2),
       q(q_main)
   {
       memset(m_equalizer_array,0,sizeof(m_equalizer_array));
       m_fullscreen_window_mode = false;
       m_video_sreen_keeping = false;
       m_is_fb0_open =false;
       m_audio_effect = MPLAYER_AUDIO_EFFCET_NULL;
       timeClear();
   }

   /*!
        所有计数时间清0
    */
    void IBMediaControlCData::timeClear(void)
    {
        m_i_current_time=0;
        m_i_total_time = 0;
        m_pause_count = 0;
        m_s_total_time.clear();
        m_s_current_media.clear();
        //m_media_type = MEDIA_TYPE_MUSIC;//默认为音频文件.
    }

    /*!
         打开视屏文件时需要设置视屏层
     */
    void IBMediaControlCData::initVideoFrameBuffer(void)
    {
        frameBufferOpen((char*)"/dev/fb2");
        //frameBufferOpen((char*)"/dev/fb1");
        frameBufferOpen((char*)"/dev/fb0");
        m_is_fb0_open = true;
        //frameBufferSetAlpha((char*)"/dev/fb1",0);

    }

    /*!
         关闭指定的frameBuffer层，
         \param  char* dev frameVBuffer设备名
     */
    void IBMediaControlCData::frameBufferClose(char* dev)
    {

        int fd=open(dev,O_RDWR);
        int disable=0;

        if(fd>=0)
        {
                ioctl(fd,TCC_LCD_FB_IOCTL_DISP_ONOFF,&disable);
                ioctl(fd, TCC_LCD_FB_IOCTL_CHROMAKEY_ONOFF, &disable);
                close(fd);
                trace((char *)"#########Set FrameBuffer Close######################\n");
        }
        else
        {
                trace((char *)"open %s error\n",dev);
        }
    }

    /*!
         开打指定的frameBuffer层，
         \param  char* dev frameVBuffer设备名
     */
    void IBMediaControlCData::frameBufferOpen(char* dev)
    {

        unsigned char mask[3]={255,255,255};
        unsigned int value[3]={0,0,0};
        int fd=open(dev,O_RDWR);
        int on=1;
        //int disable=0;

         if(fd){
                ioctl(fd,TCC_LCD_FB_IOCTL_DISP_ONOFF,&on);
                ioctl(fd, TCC_LCD_FB_IOCTL_CHROMAKEY_SET_MASK, mask);
                ioctl(fd, TCC_LCD_FB_IOCTL_CHROMAKEY_SET_VALUE, value);
                ioctl(fd, TCC_LCD_FB_IOCTL_CHROMAKEY_ONOFF, &on);
                close(fd);
                trace((char *)"#########Set FrameBuffer Open######################\n");
        }
    }

    QString IBMediaControlCData::getEqualizerString(void)
    {
            QString s_res;
            for(int i =0;i<MAX_EQUALIZER;i++)
            {
                s_res += QString::number(m_equalizer_array[i]);
                if(i<MAX_EQUALIZER-1)
                   s_res += QString(":");
            }
            return s_res;
    }

    /*!
         设指定的视屏混合层和颜色，
         \param  char* dev frameVBuffer 指定的透明混合成
         \param  int value  透明色默认为纯黑 0.
     */
    void IBMediaControlCData::frameBufferSetAlpha(char* dev,int value)
    {

        int on =1;
        int fd=open(dev,O_RDWR);

        if(fd>=0)
        {
                ioctl(fd, TCC_LCD_FB_IOCTL_ALPHA_SET, &value);
                ioctl(fd, TCC_LCD_FB_IOCTL_ALPHA_ONOFF, &on);
                close(fd);
                trace((char *)"#########FrameBuffer Alpha Set######################\n");
        }
        else
        {
                trace((char *)"open %s error\n",dev);
        }
    }

   /*!
     解析mplayer输出的信息，来获取当前播放的文件名.文件类型，是否为视屏文件类型
     \param 【IN】QString &line_data mplaer 输出的一行信息
    */
    bool  IBMediaControlCData::parseMediaInfo(const QString &line_data)
    {
        bool res= false;
        QString filename;
        //获取媒体文件名
        res = isFileName(line_data,filename);
        if(res)
        {
            m_s_current_media = filename;
            emit q->mediaChanged(filename);
            return true;
        }

        return false;

    }
    /*!
        解析播放器状态的改变，这包括以下几个状态
        1.播放
        2.暂停
        3.播放完成
        4.播放停止，用户stop()后触发信号
        其中暂停后的恢复在timeTick中处理。
      \param 【IN】QString &line_data mplaer 输出的一行信息
     */
    bool IBMediaControlCData::parseStateChanged(const QString & line_data)
    {
        bool res= false;
        res = isPlayState(line_data);//是否开始播放
        if(res)
        {
            m_current_play_state =MEDIA_RUNNING_STATE_PLAYING;
            emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_PLAYING);

            m_pause_count = 0;
            return true;
        }
        res = isPauseState(line_data);//是否暂停
        if(res)
        {
            m_current_play_state = MEDIA_RUNNING_STATE_PAUSE;
            emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_PAUSE);

            m_pause_count = 0;
            return true;
        }
        res = isPlayFinishState(line_data);//是否播放完成
        if(res)
        {
            m_current_play_state = MEDIA_RUNNING_STATE_FINISHED;
            emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_FINISHED);
            m_pause_count = 0;
            return true;
        }
        res = isStopState(line_data);//是否点了stop键或mplayer已退出
        if(res)
        {
            if(m_current_play_state != MEDIA_RUNNING_STATE_STOP)//非用户点击
            {
                m_current_play_state = MEDIA_RUNNING_STATE_FINISHED;
                emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_FINISHED);
                m_pause_count = 0;
                return true;
            }
            else
            {
                //m_current_play_state = MEDIA_RUNNING_STATE_STOP;
                //emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_STOP);
                //m_current_play_state = MEDIA_RUNNING_STATE_CRASH;//commented for bug:can't play the next and previous when playing
                //emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_CRASH);//commented for bug:can't play the next and previous when playing
                timeClear();//mplayer已退出时间都 要清零
                return true;
            }
        }

        return false;
    }

    /*!
        解析播放器上报的时间参数，并从中获取，当前时间，总时间。
      \param 【IN】QString &line_data mplaer 输出的一行信息
     */
    bool IBMediaControlCData::parseTimeTick(const QString & line_data)
    {
        bool res= false;
        int current = 0;
        QString s_current_time;

        res = isTimeTick(line_data,current);
        if(res)
        {

            if( m_current_play_state != MEDIA_RUNNING_STATE_PLAYING)//暂停后的恢复,没有结信号只能这里处理
            {
                m_pause_count++;
                if(m_pause_count>=3)//300毫秒后响应响,避免，暂停后讯间，多上报一次时间
                {
                   m_current_play_state = MEDIA_RUNNING_STATE_PLAYING;
                    emit q->sigMediaStateChanged(MEDIA_RUNNING_STATE_PLAYING);
                    m_pause_count = 0;
                }
            }
            if(m_i_current_time != current)
            {
                m_i_current_time = current;
                if(MEDIA_TYPE_MOVIE==m_media_type)
                    s_current_time = timeIntToString(m_i_current_time,TIME_FORMAT2_2_2);//数据时间转成字符串
                else
                    s_current_time = timeIntToString(m_i_current_time,m_time_format);//数据时间转成字符串
                 //trace("%d %s   %d %s \n",m_i_current_time,s_current_time.toLocal8Bit().data(),m_i_total_time,m_s_total_time.toLocal8Bit().data());
                emit q->sigCurrentTimeChanged(m_i_current_time,s_current_time,m_i_total_time,m_s_total_time);
            }
        }
        return res;
    }

    /*!
        解析播放器上报的媒体总时间
      \param 【IN】QString &line_data mplaer 输出的一行信息
     */
    bool IBMediaControlCData::parseTotalTime(const QString & line_data)
    {
        bool res= false;
        int i_total_time;
        res = isTotalTime(line_data,i_total_time);
        if(res)
        {
            m_i_total_time = i_total_time;
            if(MEDIA_TYPE_MOVIE ==m_media_type)
                m_s_total_time = timeIntToString(m_i_total_time,TIME_FORMAT2_2_2);
            else
                m_s_total_time = timeIntToString(m_i_total_time,m_time_format);
            return true;
        }
        return false;
    }

    /*!
        解析播放器中报的错误
      \param 【IN】QString &line_data mplaer 输出的一行信息
     */
    bool IBMediaControlCData::parseError(const QString & line_data)
    {
        bool res= false;
        res = isFileNotFound(line_data);
        if(res)
        {
                //上报一出错
                emit q->sigMediaPlayError(FILE_STATUS_NOT_OPEN);
                return true;
        }

        return false;

    }

    /*!
            根据输入的名字来判断是什么文件类型的媒体。
      */
    int    IBMediaControlCData::parseMediaTypeByName(const QString & file_name)
    {
           QString s_name = file_name.toLower();
           bool res =false;
           res= isNetMusicMedia(s_name);
           if(res)
           {
               m_media_type = MEDIA_TYPE_NET_MUSIC;
               return MEDIA_TYPE_NET_MUSIC;
           }
           res= isMusicMedia(s_name);
           if(res)
           {
               m_media_type = MEDIA_TYPE_MUSIC;
               return MEDIA_TYPE_MUSIC;
           }
           res= isVideoMedia(s_name);
           if(res)
           {
               m_media_type = MEDIA_TYPE_MOVIE;
               return MEDIA_TYPE_MOVIE;
           }
		   return 0;
    }


    QStringList IBMediaControlCData::getAudioSupportList(void)
    {
        QStringList audio_list;
        audio_list<<".mp3";
        audio_list<<".wma";
        audio_list<<".mid";
        audio_list<<".wav";
        audio_list<<".voc";
        return audio_list;
    }

    QStringList IBMediaControlCData::getVideoSupportList(void)
    {
        QStringList video_list;
        video_list<<".rmvb";
        video_list<<".rm";
        video_list<<".mp4";
        video_list<<".mpg";
        video_list<<".avi";
        video_list<<".wmv";
        video_list<<".dat";
        video_list<<".mov";
        video_list<<".mpeg";
        video_list<<".ts";
        video_list<<".flv";
        video_list<<".mkv";
        video_list<<".mka";
        video_list<<".m2v";
        video_list<<".asf";
        video_list<<".vob";
        return video_list;

    }


    /*!
        判断指定字符串中是否含有文件名
        预期输入为: ID_LENGTH=239.00
        输出为:239
                 true
      \param 【IN】QString &line_data mplaer 输出的一行信息
      \param 【OUT】int &total_tme 做为输出时间,以秒为单位.
     */
    bool IBMediaControlCData::isTotalTime(const QString & line_data,int &total_tme)
    {
        int index = -1;
        index = m_rx_total_length.indexIn(line_data);
        if(index>-1)
        {
            total_tme = (int)m_rx_total_length.cap(1).toDouble();
            return true;
        }
        return false;
    }

    /*!
        判断指定字符串中是否含有文件名
        预期输入为: ID_FILENAME=/nand2/1.mp3
        输出为:/nand2/1.mp3
                 true
      \param 【IN】QString &line_data mplaer 输出的一行信息
      \param 【OUT】QString &file_name 做为输出
     */
   bool IBMediaControlCData::isFileName(const QString &line_data, QString &file_name)
   {

        int index = -1;
        index = m_rx_file_name.indexIn(line_data);
        if(index>-1)//找到正在播放的文件名
        {
           file_name =  m_rx_file_name.cap(2);// cap(0)是ID_FILENAME=/nand2/1.mp3 ,cap(1)是ID_FILENAME,cap(2)是/nand2/1.mp3
           trace((char *)"\n%s",file_name.toLocal8Bit().data());
           return true;
        }
        return false;
   }

   /*!
       判断是否进入播放状态。
       预期输入为: Starting playback...
       输出为: true
     \param 【IN】QString &line_data mplaer 输出的一行信息
    */
   bool IBMediaControlCData::isPlayState(const QString & line_data)
   {
       int index = -1;
        index = m_rx_play_state.indexIn(line_data);
        if(index >-1)
        {
            trace((char *)"\nStarting playback...\n");
            return true;

        }
        return false;
   }
   /*!
       判断是否进入暂停状态。
       预期输入为: ID_PAUSED
       输出为: true
     \param 【IN】QString &line_data mplaer 输出的一行信息
    */
    bool IBMediaControlCData::isPauseState(const QString & line_data)
    {
        int index = -1;
         index = m_rx_pause_state.indexIn(line_data);
         if(index >-1)
         {
             trace((char *)"\nID_PAUSED\n");
             return true;
         }
         return false;
    }

     /*!
         判断是否进入播放自动完成状态。
         预期输入为: ID_EXIT=EOF
         输出为: true
       \param 【IN】QString &line_data mplaer 输出的一行信息
      */
      bool IBMediaControlCData::isPlayFinishState(const QString & line_data)
      {
          int index = -1;
           index = m_rx_play_finish_state.indexIn(line_data);
           if(index >-1)
           {
               trace((char *)"\nID_EXIT=EOF\n");
               return true;
           }
           return false;
      }
      /*!
          判断mplayer  是否已停止或退出进程.
          预期输入为: ID_EXIT=QUIT或MPlayer crashed. This shouldn't happen
          输出为: true
        \param 【IN】QString &line_data mplaer 输出的一行信息
       */
      bool IBMediaControlCData::isStopState(const QString & line_data)
      {
          int index = -1;
           index = m_rx_stop_quit.indexIn(line_data);
           if(index >-1)
           {
               trace((char *)"\nID_EXIT=QUIT\n");
               return true;
           }
           else
           {
               index = m_rx_crash_quit.indexIn(line_data);
               if(index >-1)
               {
                   trace((char *)"\n- MPlayer crashed. This shouldn't happen\n");
                   return true;
               }
               else{
                   index = m_rx_qprocess_crash_quit.indexIn(line_data);
                   if(index >-1)
                   {
                       trace((char *)"\nQProcess crash\n");
                       return true;
                   }
               }
           }
           return false;

      }
      /*!
          判断是否是mplayer播放的是否为普通音频文件
             \param 【IN】QString &file_name 输入的文件名
       */
      bool IBMediaControlCData::isMusicMedia(const QString & file_name)
      {
            QStringList m_list = getAudioSupportList();
            int i = 0;
            for(i=0;i<m_list.count();i++)
            {
                if(file_name.indexOf(m_list.at(i))>=0)
                {
                        return true;
                }

            }
            return false;
      }

      /*!
          判断是否是提示文件不存
             \param 【IN】QString &file_name 输入的文件名
       */

      bool IBMediaControlCData::isFileNotFound(const QString & line_data)
      {
          int index = -1;
           index = m_rx_file_not_found.indexIn(line_data);
           if(index >-1)
            {
                trace((char *)"\n File not found \n");
                return true;
            }
           return false;
      }

      /*!
          判断是否是mplayer播放的是否为网络音频文件
             \param 【IN】QString &file_name 输入的文件名
       */
      bool IBMediaControlCData::isNetMusicMedia(const QString & file_name)
      {
          if(file_name.at(0) !='/')
              return true;
          else
              return false;
      }

      /*!
          判断是否是mplayer播放的是否为视屏文件
             \param 【IN】QString &file_name 输入的文件名
       */
      bool IBMediaControlCData::isVideoMedia(const QString & file_name)
      {
          QStringList m_list = getVideoSupportList();
          int i = 0;
          for(i=0;i<m_list.count();i++)
          {
              if(file_name.indexOf(m_list.at(i))>=0)
              {
                      return true;
              }

          }
          return false;
      }

      /*!
          判断是否是mplayer上报时间
            A:   0.9 (00.9) of 239.0 (03:59.0) 11.3% 45%  //音频输入
        \param 【IN】QString &line_data mplaer 输出的一行信息
       */
      bool IBMediaControlCData::isTimeTick(const QString & line_data,int &current)
      {
          int index = -1;
          double i_now =0.0;
          QRegExp l_rx_video_tick;//解析时间更新
          l_rx_video_tick.setPattern("[V]: *([0-9,:.-]+)");

          QString s_current;
           index = m_rx_time_tick.indexIn(line_data);
           if(index >-1)
           {
                s_current = m_rx_time_tick.cap(1);

                i_now =  s_current.toDouble();
                //处理seek出错时返回如下时间信息
                //A:  -0.0 (unknown) of 239.0 (03:59.0)  0.0% 38%   751120
                if(0 == (int)i_now)
                {
                   // trace("%d %s \n",m_pause_count,line_data.toLocal8Bit().data());
                    if(line_data.indexOf("(unknown)")>=0)
                    {
                        return false;
                    }
                    //handle special video output like,audio time eq 0 ,but video time not eq 0
                    //0 A:  -0.0 V: 465.6 A-V:-465.599 ct: -0.100 13955/13955 ??% ??% ??,?% 0 0 39%
                    if(l_rx_video_tick.indexIn(line_data))
                    {
                        if(index >-1)
                        {
                            s_current = l_rx_video_tick.cap(1);
                            i_now = s_current.toDouble();
                        }
                    }

                }
                if( (i_now+0.5)>= m_i_total_time)
                    current =(int)i_now+1;
                else
                     current =(int)i_now;
                if(current>m_i_total_time)
                    current = m_i_total_time;
                return true;
           }
           return false;
      }

      /*!
          把数字时间转处字符串
        \param 【IN】int time 输入数字时间
        \param 【IN】int format 输入将转换成的格式如下
            TIME_FORMAT_1_2,//e.g:  1:59 123:29
            TIME_FORMAT_2_2,//e.g:   01:59 223:29
            TIME_FORMAT2_2_2,//e.g：00:01:02  00:32:32
       */
      QString IBMediaControlCData::timeIntToString(int time,int format)
      {
            QString res;
            QString s_hour,s_minute,s_second;
            int hour=0;
            int minute =0;
            int second=0;
            hour = time/3600;
            second =  time%60;
            switch(format)
            {
                case TIME_FORMAT_1_2:
                   minute = time/60;
                    s_second =intTo2String(second);
                    s_minute =QString::number(minute);
                    res = s_minute+QString(":")+s_second;
                    break;
                case TIME_FORMAT_2_2:
                    minute = time/60;
                    s_second =intTo2String(second);
                    s_minute =intTo2String(minute);
                    res = s_minute+QString(":")+s_second;
                    break;
                case TIME_FORMAT2_2_2:
                    minute = (time/60)%60;
                    s_second =intTo2String(second);
                    s_minute =intTo2String(minute);
                    s_hour = intTo2String(hour);
                    res =s_hour+ QString(":")+s_minute+QString(":")+s_second;
                    break;
            }
            return res;
      }

      /*!
          把数字时间成两位以上的格式
          数字  1--->字符串 “01”
          数字  991--->字符串 “991”
       */
      QString IBMediaControlCData::intTo2String(int data)
      {
          QString res;
            if(data<10)
              res = QString::number(0)+QString::number(data);
            else
              res = QString::number(data);
            return res;
      }


//-----------------------------------------------------------------------------------------------------------
//-IBMediaControlC类
IBMediaControlC * IBMediaControlC::this_object =NULL;
IBMediaControlC::IBMediaControlC(QObject * parent):MyProcess(parent)
{
    init();
    connect(this,SIGNAL(lineAvailable(QString)),this,SLOT(parseLine(QString)));
}

IBMediaControlC * IBMediaControlC::object()
{
    if( NULL == this_object )
    {
        this_object = new IBMediaControlC(NULL);
    }
    return this_object;
}


void IBMediaControlC::parseLine(const QString & line)
{

    bool res =false;
    res = d->parseTimeTick(line);
    if(res)
        return;
    res = d->parseMediaInfo(line);
    if(res)
        return;
    res = d->parseStateChanged(line);
    if(res)
        return;
    res = d->parseTotalTime(line);
    if(res)
        return;
    res = d->parseError(line);
    if(res)
        return;
    //trace("IBMediaControlC::parseLine=%s \n",line.toLocal8Bit().data());
}


void IBMediaControlC::init()
{
    d = new IBMediaControlCData(this);
}

/*!
*  暂停 mplayer播放。
*/
void IBMediaControlC::pause()
{
        d->m_current_play_state = MEDIA_RUNNING_STATE_PAUSE;
        emit sigMediaStateChanged(MEDIA_RUNNING_STATE_PAUSE);
        sendCommand("pause");

}
void IBMediaControlC::resume()
{
   // if(d->m_current_play_state == MEDIA_RUNNING_STATE_PAUSE)
    sendCommand("pause");

}
/*!
        改变当前播放的位置
        \param int pos 要播放的时间
*/
void IBMediaControlC::changePosition(int pos)
{
    if(pos > 0 && pos < d->m_i_total_time)
    {
        sendCommand(QString("pausing_keep ")+QString("seek ") + QString::number(pos) +QString(" 2"));
    }
}
/*!
  *取得当前的播放媒体类型
*/
Media_Type IBMediaControlC::getMediaType(void)
{
    return d->m_media_type;
}
/*!
  *启动mplayer 进程
 \param QString & fileName 要播放的资源名称
 \param Media_Type type 媒体模式
 \param int seek 从指定位置开始播放以秒为单位，当小于2时从头开始播放
*/
bool IBMediaControlC::play( const QString & fileName, Media_Type type,int seek)
{
	if( d->m_media_type != type )
	{
		d->m_media_type = MEDIA_TYPE_NONE;
	}
    if(isRunning())
    {
    /*
        if(fileName == d->m_s_current_media && d->m_current_play_state != MEDIA_RUNNING_STATE_STOP)//如果正在播放放当前的文件
        {
            pause();
            return true;
        }*/
        stop();
    }

    //获取媒体文件类型，并对视屏文件进行特别处理
    //l_type = d->parseMediaTypeByName(fileName);
    if(d->m_media_type !=type)
    {
        d->m_media_type = type;
        emit sigMediaTypeChanged(type);
    }
//    if(type == MEDIA_TYPE_MOVIE)
//    {
//        if(!d->m_video_sreen_keeping)
//        {
//            d->initVideoFrameBuffer();//打开视屏文件时需要初始化
//        }
//        else
//            d->m_video_sreen_keeping = false;
//    }

    //设置播放参数
    QStringList args = getArglist();

    //记忆上次的视屏播放模式
    if(true == isFullScreenWindowMode())
    {
        args<<"-fs";
    }
#ifdef _8925_VERSION
    if(seek >2)
    {
        QString str = QString::number(seek);
        args<<"-ss";
        args<<str;
    }
#endif
    /*QString s_equalizer  =QString("-af");
    args<<s_equalizer;
    s_equalizer= QString("equalizer=")+d->getEqualizerString();
    args <<s_equalizer;*/
    args << fileName;

    start(MPLAYER_PATH, args);
    bool res=waitForStarted();
    //小于2s 从头播放
#ifndef _8925_VERSION
    if(seek >2)
    {
        sendCommand(QString("pausing_keep ")+QString("seek ") + QString::number(seek) +QString(" 2"));
    }
#endif
    return res;

}

void IBMediaControlC::stop()
{
    bool res =false;
    d->m_current_play_state = MEDIA_RUNNING_STATE_STOP;

    kill();

    res =  waitForFinished(3000);
    if(!res)
    {
        trace((char *)"stop quit error,kill mplayer\n ");
        system("killall mplayer");
    }
    return;
    sendCommand("quit");
   res =  waitForFinished(3000);
   if(!res)
   {
       trace((char *)"stop quit error,kill mplayer\n ");
       system("killall mplayer");
   }
}
/*!
*  获取播放器当前状态
*/
int IBMediaControlC::currentState(void)
{
    return d->m_current_play_state;
}

/*!
*  获取播放器当前播放时间,以秒(s)为单位
*/
int IBMediaControlC::getPlayPosition(void)
{
    return d->m_i_current_time;
}
/*!
*  获取播放器当前播放总时间,以秒(s)为单位
*/
int IBMediaControlC::getMediaLength(void)
{
    return d->m_i_total_time;
}
/*!
* 设置视屏显示全窗口模式
*/
void IBMediaControlC::setFullScreenWindowMode(bool b_fullscren)
{
    if(d->m_fullscreen_window_mode != b_fullscren)
    {
        d->m_fullscreen_window_mode =b_fullscren;
        sendCommand(QString("vo_fullscreen ")+ QString::number(1));
        if(d->m_current_play_state == MEDIA_RUNNING_STATE_PAUSE)//处理暂停时全屏切换花屏的问题
        {
            changePosition(d->m_i_current_time);
        }
     }

}
/*!
* 获取视屏显示全窗口模式
*/
bool IBMediaControlC::isFullScreenWindowMode(void)
{
    return d->m_fullscreen_window_mode;
}
/*!
* 设置音效模式
     MPLAYER_POP,//流行音乐
     MPLAYER_CLASSIC,//古典
     MPLAYER_DANCE,//爵士
     MPLAYER_RB,//RB
     MPLAYER_BASSS,//重低音
     MPLAYER_ROCK,//摇滚
     MPLAYER_USER_SET,//自定义
*/
int IBMediaControlC::setAudioEeffect(M_AUDIO_EFFCET autdio_effect)
{
    d->m_audio_effect = autdio_effect;
    Audio_Equalizer_S eq= getAudioEqualize(autdio_effect);
    if(autdio_effect != MPLAYER_USER_SET)//
    {
        setAudioEqualizer(eq);
    }
	return 0;
}

        /*!
        * 获取当前音效模式
        */
        int IBMediaControlC::getAudioEeffect(void)
        {
            return d->m_audio_effect;
        }

        /*!
           设置10个均衡器
           \param int eq_arrary[] 均衡器参数数组，数组长度必须为10
        */
        void IBMediaControlC::setAudioEqualizer(Audio_Equalizer_S eq)
        {
            memcpy(d->m_equalizer_array,eq.iEqualizer,sizeof(d->m_equalizer_array));
            d->m_audio_effect = MPLAYER_USER_SET;
            QString s_equalizer = d->getEqualizerString();
            QString s_cmd = QString("af_cmdline equalizer ") + s_equalizer;
            printf(" sendCommand %s\n",s_cmd.toLocal8Bit().data());
            sendCommand(s_cmd);
        }

        /*!
           获取10个均衡器的设置
       \return 均衡器参数数
        */
       Audio_Equalizer_S IBMediaControlC::getAudioEqualize(M_AUDIO_EFFCET autdio_effect)
       {
           Audio_Equalizer_S eq;
           memset(eq.iEqualizer,0,sizeof(eq.iEqualizer));
           switch(autdio_effect)
           {
               case MPLAYER_POP: //流行
               {
                   int i_pop_array[MAX_EQUALIZER]={-2,0,2,4,0,-2,-4,-2,0,0,};
                   memcpy(eq.iEqualizer,i_pop_array,sizeof(eq.iEqualizer));
               }
                   break;
               case MPLAYER_CLASSIC://古典
               {
                   int i_classic_array[MAX_EQUALIZER]={-2,0,2,4,-1,-3,-2,0,1,3,};
                   memcpy(eq.iEqualizer,i_classic_array,sizeof(eq.iEqualizer));
               }
                   break;
               case MPLAYER_DANCE: //舞曲
               {
                   int i_jaz_array[MAX_EQUALIZER]={-1,4,3,-1,-2,-5,-2,-3,0,4,};
                   memcpy(eq.iEqualizer,i_jaz_array,sizeof(eq.iEqualizer));
               }
                   break;
               case MPLAYER_RB: //RB
               {
                   int i_counrty_array[MAX_EQUALIZER]={ -1,0,2,2,-1,-2,-1,0,2,4,};
                   memcpy(eq.iEqualizer,i_counrty_array,sizeof(eq.iEqualizer));

               }
                   break;
               case MPLAYER_ROCK://摇滚
               {
                   int i_rock_array[MAX_EQUALIZER]={-2,0,2,4,-1,-3,-2,0,1,3,};
                   memcpy(eq.iEqualizer,i_rock_array,sizeof(eq.iEqualizer));
               }
                   break;
               case MPLAYER_BASS://重低音
               {
                   int i_bass_array[MAX_EQUALIZER]={-2,4,4,0,-5,-6,-4,-3,0,4,};
                   memcpy(eq.iEqualizer,i_bass_array,sizeof(eq.iEqualizer));
               }
                   break;
               case MPLAYER_USER_SET:
                   memcpy(eq.iEqualizer,d->m_equalizer_array,sizeof(eq.iEqualizer));
                   break;
               case MPLAYER_AUDIO_EFFCET_NULL:
                   break;


               default:
                   break;
           }

            return eq;
        }

        /*!
        * 打开视屏显示层
        */
        void IBMediaControlC::openVideoScreen(void)
        {
            //for double screen output ,can't open fb0 twice.otherise will cause flower screen on TV end.
           // if(false == d->m_is_fb0_open)//driver layer will rememeber this state.
            qWarning("----------------------------IBMediaControlC::openVideoScreen\n");
            {
                d->initVideoFrameBuffer();
                d->frameBufferOpen((char*)"/dev/fb0");
            }
        }

        /*!
        * 关闭视屏显示层
        */
        void IBMediaControlC::closeVideoScreen(void)
        {
           // if(true == d->m_is_fb0_open)
            qWarning("----------------------------IBMediaControlC::closeVideoScreen");
            {
                d->frameBufferClose((char*)"/dev/fb0");
            }
        }


        /*!
          *
          *设置视屏显示层保持模式，此位设置后在播放视频时将不再重新打开视屏显示的framebuffer。播放过一次之后这个标志失效。重新变为false;
          \param bool b_keep 设置为视屏保持模式。
        */
        void IBMediaControlC::setVideoScreenKeepingMode(bool b_keep)
        {
            d->m_video_sreen_keeping =b_keep;
        }

        /*!
          设置手机上报时间的格式,仅对音频文件有效，视屏一直都为TIME_2_2_2_FORMAT
            enum
            {
                TIME_1_2_FORMAT,//例如:  1:59 123:29
                TIME_2_2_FORMAT,//例如:   01:59 223:29
                TIME_2_2_2_FORMAT,//例如：00:01:02  00:32:32
            };
          */
        void IBMediaControlC::setTimeFormat(MEDIA_TIME_FORMAT type)
        {
                d->m_time_format = type;
        }
        int IBMediaControlC::getTimeFormat(void)
        {
            return d->m_time_format;
        }
        /*!
        */
        QString IBMediaControlC::transferTimeFormat(int time,Media_Type type)
        {
                  if(MEDIA_TYPE_MOVIE == type)
                      return d->timeIntToString(time,TIME_FORMAT2_2_2);
                  else
                      return d->timeIntToString(time,d->m_time_format);
        }

        /*!
        *  进程通讯接口，给mplayer发送指今
        */
        bool IBMediaControlC::sendCommand(const QString & command)
        {
            bool result = false;

            if (isRunning())
            {
                qint64 nbBytes = write(command.toLocal8Bit() + "\n");
                if (nbBytes != -1)
                {
                    result = true;
                }
            }

            return result;
        }

        QStringList IBMediaControlC::getArglist(void)
        {
            QStringList args;



            //MPlayer slave mode
	    args << "-n";
	    args << "-10";
	    args << "mplayer";
            args << "-slave";
            args << "-noquiet";//自动上报时间和系统时间
            args << "-identify";//上报ID等信息


            //额外的参数
           // args << "-framedrop";

            return args;
        }

        void IBMediaControlC::mediaTypeChangeToNone(void)
        {
                if( d->m_media_type != MEDIA_TYPE_NONE )
                {
                        d->m_media_type = MEDIA_TYPE_NONE;
                        emit sigMediaTypeChanged(MEDIA_TYPE_NONE);
                }

                stop();
        }




        }//namespace MediaControl2


