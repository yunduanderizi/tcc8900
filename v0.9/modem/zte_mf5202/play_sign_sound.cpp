#include "play_sign_sound.h"
#include "debugMonitor.h"
PlaySignSound::PlaySignSound()
{
	asound_status = -1;
	channels = 0;
	m_soundType = Sound_Type_Ring;
        
}

PlaySignSound::~PlaySignSound()
{

}

int PlaySignSound::init_asound()
{
	fprintf(stderr,"snd_pcm_open\n");

	char device[50];
	if( Sound_Type_Ring == m_soundType )
	{
		sprintf(device, "plug:ring");
	}
	else if( Sound_Type_Smsring == m_soundType )
	{
		sprintf(device, "plug:smsring");
		//sprintf(device, "plug:ring");
	}
	else
	{

	}
	if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		fprintf(stderr,"snd_pcm_open failed!\n");
		
		return -1;
	}

	fprintf(stderr,"params\n");

	snd_pcm_hw_params_alloca(&params);

	if ( (err = snd_pcm_hw_params_any(handle, params)) < 0 )
	{
		fprintf(stderr,"snd_pcm_hw_params_any failed!   err = %d\n", err);

		return -1;
	}
	
	if ( (err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0 )
	{
		fprintf(stderr,"snd_pcm_hw_params_set_access failed!   err = %d\n", err);

		return -1;
	}
	
	if ( (err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0 )
	{
        	fprintf(stderr,"snd_pcm_hw_params_set_format failed!   err = %d\n", err);

		return -1;
	}
	
	channels = val = 1;

	if ( (err = snd_pcm_hw_params_set_channels(handle, params, val)) < 0 )
	{
		fprintf(stderr,"snd_pcm_hw_params_set_channels failed!   err = %d\n", err);

		return -1;
	}
	
	val = 16000;

	if ( (err = snd_pcm_hw_params_set_rate_near(handle, params, &val, &pcm_dir)) < 0 )
	{
		fprintf(stderr,"snd_pcm_hw_params_set_rate_near failed!   err = %d\n", err);

		return -1;
	}

	fprintf(stderr,"set_hw_params!\n");

	if ( (err = snd_pcm_hw_params(handle, params)) < 0 )
	{
		fprintf(stderr,"snd_pcm_hw_params-failed!	err = %d err: %s\n", err, snd_strerror(err));

		return -1;
	}
	
	fprintf(stderr,"snd_pcm_hw_params_get_period_size!\n");

	if ( (err = snd_pcm_hw_params_get_period_size(params, &frames, &pcm_dir)) < 0 )
	{
		fprintf(stderr,"snd_pcm_hw_params_get_period_size failed!   err = %d\n", err);

		return -1;
	}
	
	fprintf(stderr,"-----------frames = %d\n", frames);	

	asound_size = frames * channels * 2;

	buffer = (char *)malloc(sizeof(char) * asound_size);

	p_buffer = buffer;

	return 0;
}

int PlaySignSound::play_sign_sound()
{
	fprintf(stderr,"begin open sound_file!\n");	

	if ( (fd = open(sound_file_name, O_RDONLY)) < 0 )
	{
		fprintf(stderr,"open file failed!\n");

		return -1;
	}

	fprintf(stderr,"-----------size = %d\n", asound_size);	

	while(asound_status)
	{
		err = read(fd,buffer,asound_size);
	
		if(err<0)
		{
			break;
		}
		else if(err < asound_size)
		{
			fprintf(stderr,"end of file!\n");

			if(is_loop_play)
			{
				lseek(fd,0,SEEK_SET);
			}
			else
			{
				break;
			}
		}

		int currentFrames =err/2;

		err = snd_pcm_writei(handle, buffer,currentFrames);
		if ( err == -EPIPE )
		{
			fprintf(stderr,"underrun occurred! err: %s\n", snd_strerror(err));
			if( (err = snd_pcm_prepare(handle)) < 0 )
			{
				fprintf(stderr,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(err));
			}
		}
		else if ( err < 0 )
		{
			fprintf(stderr,"err writei!\n");
		}
		else if ( err != (int)frames )
		{
			//fprintf(stderr,"short writei!\n");
		}
	}
	
	return 0;
}

void PlaySignSound::release_asound()
{
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(p_buffer);
}

void PlaySignSound::run()
{
	init_asound();
	play_sign_sound();
	fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
	release_asound();
	fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);

	IBD_ENTER_FUNC
		
	emit finishedPlay();

	m_mutex.lock();
	asound_status =-1;
        m_quitCond.wakeOne();
	m_mutex.unlock();

}

void PlaySignSound::play_sign_sound(char *file_name, int loop_play, SoundType type)
{
	m_mutex.lock();

	m_soundType = type;
	
	if (asound_status ==1)
	{
		asound_status = 0;
		m_quitCond.wait(&m_mutex);
		wait();
		asound_status = -1;
	}
	
	is_loop_play = loop_play;
		
	strcpy(sound_file_name,file_name);
		
	asound_status =1;

	start();

	m_mutex.unlock();
}

void PlaySignSound::stop_sign_sound()
{
	m_mutex.lock();

	if (asound_status ==1)
	{
		asound_status = 0;
		m_quitCond.wait(&m_mutex);
		wait();
	}	

	asound_status = -1;

	m_mutex.unlock();
}


