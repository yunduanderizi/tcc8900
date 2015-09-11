#ifndef PLAYSIGNSOUND_H
#define PLAYSIGNSOUND_H

/*
使用说明:

播放声音:用类"PlaySignSound"中的函数"void play_sign_sound(char *file_name, int loop_play)"
其中参数"file_name"是要播放的声音文件的路径名，参数"loop_play"控制是
否循环播放，循环播放:loop_play = 1，只播放一次:loop_play = 0

停止播放声音:用类"PlaySignSound"中的函数"void stop_sign_sound()"

PlaySignSound类中的结束播放信号:每播放完一个声音文件，PlaySignSound类内部会发出
一个信号"finishedPlay()"。可以通过此信号检测是否播完。
*/

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "asoundlib.h"

class PlaySignSound: public QThread
{
	Q_OBJECT
		
public:
	typedef enum _SoundType
	{
		Sound_Type_Ring = 0,
		Sound_Type_Smsring,
	}SoundType;

	PlaySignSound();
	~PlaySignSound();


	void play_sign_sound(char *file_name, int loop_play, SoundType type);
	void stop_sign_sound();

signals:
	void finishedPlay();
	
private:
	int is_loop_play;

	int asound_status;
	char sound_file_name[100];

	QMutex m_mutex;
	QWaitCondition  m_quitCond;

private:
	int fd;	
	int asound_size;
	long loops;
	int err;
	char *buffer;
	char *p_buffer;
	int channels;
	unsigned int val;
	int pcm_dir;
	snd_pcm_uframes_t frames;
	snd_pcm_hw_params_t* params;
	snd_pcm_t* handle;
	SoundType m_soundType;

	int init_asound();
	int play_sign_sound();
	void release_asound();

	void run();
};

#endif // WIDGET_H
