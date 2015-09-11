#ifndef IBMEDIACONTROL_H
#define IBMEDIACONTROL_H
#include "myprocess.h"

namespace MediaControl
{
    class IBMediaControlCData;
    class IBMediaControlC : public MyProcess
    {
    Q_OBJECT
public:

        enum{
            M_STOP_STATE,
            M_PLAYING_STATE,
            M_PUASE_STATE,
            M_FININED_STATE,//播放完成状态
            M_ERROR_STATE,//播放出错，可能文件不存在,可能解析包出错
        };
        typedef enum
        {
            TIME_1_2_FORMAT,//例如:  1:59 123:29
            TIME_2_2_FORMAT,//例如:   01:59 223:29
            TIME_2_2_2_FORMAT,//例如：00:01:02  00:32:32
            MAX_TIME_FORMAT,
        }M_TIME_FORMAT;

        enum
        {
            M_MUSIC_TYPE,///音乐
            M_NET_MUSIC_TYPE,//网络资源
            M_VIDEO_TYPE,////视频
        };


        typedef enum
        {
            MPLAYER_AUDIO_EFFCET_NULL,//无音效
            MPLAYER_POP,//流行音乐
            MPLAYER_CLASSIC,//古典
            MPLAYER_JAZ,//爵士
            MPLAYER_COUNTRY,//乡村音乐
            MPLAYER_ROCK,//摇滚
            MPLAYER_MAX_AUDIO_EFFCET,//
        }M_AUDIO_EFFCET;

    IBMediaControlC(QObject * parent);
    static IBMediaControlC * object(QObject * parent);
    //~IBMediaControlC();
    /*!
      *启动mplayer 进程
     \param QString & fileName 要播放的资源名称
     \param int seek 从指定位置开始播放以秒为单位，当小于2时从头开始播放
    */
    bool play(const QString & fileName, int seek=0);
    /*!
    *  暂停 mplayer播放和。
    */
    void pause();

    /*!
    *  继续播放暂停 的mplayer
    */
    void resume();

    /*!
            改变当前播放的位置,以秒为位置
    */
    void changePosition(int pos);
    /*!
    *  获取播放器当前播放时间,以秒(s)为单位
    */
    int getPlayPosition(void);

    /*!
    *  停止 mplayer播放。
    */
    void stop();
    /*!
    *  获取播放器当前状态
    */
    int currentState(void);
    /*!
      设置mplayer上报时间的格式,，仅对音频文件有效，视屏一直都为TIME_2_2_2_FORMAT
        enum
        {
            TIME_1_2_FORMAT,//例如:  1:59 123:29
            TIME_2_2_FORMAT,//例如:   01:59 223:29
            TIME_2_2_2_FORMAT,//例如：00:01:02  00:32:32
        };
      */
    void setTimeFormat(M_TIME_FORMAT);
    /*!
      获取mplayer报时间的格式,仅对音频文件有效，视屏一直都为TIME_2_2_2_FORMAT
    */
    int getTimeFormat(void);

    /*!
    *  获取播放器当前播放总时间,以秒(s)为单位,在stop状态为返回值为0
    */
    int getMediaLength(void);
    /*!
    * 设置视屏显示全窗口模式
    */
    void setFullScreenWindowMode(bool b_fullscren);
    /*!
    * 获取视屏显示全窗口模式
    */
    bool isFullScreenWindowMode(void);

    /*!
    * 设置音效模式
            MPLAYER_POP,//流行音乐
            MPLAYER_CLASSIC,//古典
            MPLAYER_JAZ,//爵士
            MPLAYER_COUNTRY,//乡村音乐
            MPLAYER_ROCK,//摇滚
    */
    int setAudioEeffect(M_AUDIO_EFFCET);
    /*!
    * 获取当前音效模式
    */
    int getAudioEeffect(void);

    /*!
    * 打开视屏显示层
    */
    void openVideoScreen(void);
    /*!
    * 关闭视屏显示层
    */
    void closeVideoScreen(void);

    /*!
      *
      *设置视屏显示层保持模式，此位设置后在播放视频时将不再重新打开视屏显示的framebuffer。播放过一次之后这个标志失效。重新变为false;
      \param bool b_keep 设置为视屏保持模式。
    */
    void setVideoScreenKeepingMode(bool b_keep);
signals:
    /*!
        播放的媒体文件改变了如：/nand2/1.mp3
        文件全名
      */
      void mediaChanged(QString name_path);
      /*!
          播放的状态改变了,以下状态
          \param int meida_type
                M_MUSIC_TYPE,///音乐
                M_NET_MUSIC_TYPE,//网络资源
                M_VIDEO_TYPE,////视频
          \param int new_state
            MPLAYER_STOP_STATE,//停止，是指mplayer进程已退出
            MPLAYER_PLAYING_STATE,//播放，当前为播放状态
            MPLAYER_PUASE_STATE,//暂停，当前为暂停状态
            MPLAYER_FININED_STATE,//播放完成状态,但mplayer未退出
     */
      void mplayerStateChanged(int meida_type,int new_state);
      /*!
        更新当前播放时间
        int current 当前播放时间，以秒计数 如：1
        QString s_current 当前播放时间字符串，以秒计数 如：00:03
        int total    当前总的时间,以秒计数如:293
      */
      void currentTimeChanged(int current,QString s_current,int total,QString s_total);

private slots:
    /**
     * 解析mplayer的行输出
     *
     * \param QString & line mplayer输出的一行
     */
    void parseLine(const QString & line);

protected:
    /*!
     * 初始化解析字符变量
      */
    void init(void);
    /*!
      向mplayer进程发送命令
      */
    bool sendCommand(const QString & command);
    /*!
      获取mplayer 启动时的参数
      */
    QStringList getArglist(void);
    static IBMediaControlC* this_object;
    IBMediaControlCData * d;
    friend class IBMediaControlCData;

};
}

#endif // IBMEDIACONTROL_H
