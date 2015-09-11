#include "record.h"


snd_pcm_t *capture_handle;
snd_pcm_uframes_t frames;
snd_pcm_stream_t stream;
snd_pcm_hw_params_t *hw_params = NULL;

unsigned int period_time;

int getSampleRate(int m_sample){
  
    switch (m_sample){
        case 0:return 8000;
        case 1:return 11000;
        case 2:return 23000;
        case 3:return 32000;
        default:
               return 0;
    }
}

//初始化Pcm设备
int InitPcm(int m_sample){

    int err = 0;
    unsigned int val;
    int dir = 0;

    snd_pcm_hw_params_alloca(&hw_params);
    stream = SND_PCM_STREAM_CAPTURE;
    //打开一个pcm设备，参数1是设备结构体，参数2是设备名称，参数3是打开模式拍照、播放等，参数4是打开方式：标准、非阻塞、异步
    if((err = snd_pcm_open(&capture_handle,"plug:tcc",SND_PCM_STREAM_CAPTURE,0)) < 0){
   
        printf("cannot open audio device(%d)\n",err);
        return -1;
    }
    //先使用默认值初始化pcm设备硬件参数结构体
    if((err = snd_pcm_hw_params_any(capture_handle,hw_params)) < 0){
    
        printf("fail to initialize hardware parameter structure(%d)\n",err);
        return -1;
    }
    //获取周期长度(帧数)
    snd_pcm_hw_params_get_period_size(hw_params, &frames,0);
 
  //  printf("frames = %d.............\n",(int)frames);

    //设置设备数据在缓冲区内的存储方式，交错和非交错
    if((err = snd_pcm_hw_params_set_access(capture_handle,hw_params,SND_PCM_ACCESS_RW_INTERLEAVED)) < 0){
        printf("fail to set access type (%d)\n",err);
        return -1;
    }
    //设置音频样本格式：8位、16位、32位
    if((err = snd_pcm_hw_params_set_format(capture_handle,hw_params,SND_PCM_FORMAT_S16_LE)) < 0){
    
        printf("fail to set sample format (%d)\n",err);
        return -1;
    }
    //设置采样频率
    if((val = getSampleRate(m_sample)) == 0){
    
        printf("Error: getSampleRate(m_sample), m_sample is error.\n");
        return -1;
    }
    //printf("samplerate:%d\n",(int)val);
    if((err = snd_pcm_hw_params_set_rate_near(capture_handle,hw_params,&val,0)) < 0){

        printf("fail to set sample rate(%d)\n",err);
        return -1;
    }
    //设置设备通道，1为单声道、2为立体声
    val = CH;//频道1(单声道)
    if((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, val)) < 0){
    
        printf("fail to set channel count (%d)\n", err);
        return -1;
    }
    //向驱动中写设置好的硬件参数
    if((err = snd_pcm_hw_params(capture_handle,hw_params)) < 0){
       
        printf("fail to set parameters(%d)\n",err);
        return -1;
    }
    //Prepare PCM for use
    if((err = snd_pcm_prepare(capture_handle)) < 0){

        printf("fail to prepare audio interface for use (%d)\n",err);
        return -1;
    }

    frames = 6144;
    //如果未指定pcm的周期长度(帧数)，使用一个最接近的周期长度
    if((err = snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir)) < 0){
    
        printf("fail to snd_pcm_hw_params_set_period_size_near failed! err = %d\n",err);
        return -1;
    }
    //获取音频设备处理一次数据的周期
    int status = snd_pcm_hw_params_get_period_time(hw_params, &val, &dir);
    if(status < 0){
    
        printf("fail to get pcm_period_time\n");
        return -1;
    }
    period_time = val;
    //printf("period time = %d us...................\n", val);
    system("amixer cset numid=16 on");//若不开，无法录音
    system("amixer cset numid=7 on");//若不开，无法录音
    system("amixer cset numid=8 on");//不开会有很大杂音
   
    return 0;
}

int Recording()
{
    char mono[PCMBUFFSIZE];
    int iRet, iLen;
    extern int wavFd;

    memset(mono, 0, sizeof(char) * PCMBUFFSIZE);
    iLen = snd_pcm_readi(capture_handle, mono, frames);
    if(iLen != (int)frames){
        printf("read from audio interface failed (%d)\n", iLen);
        if((iRet = snd_pcm_prepare(capture_handle)) < 0){
            printf("snd_pcm_prepare failed! err : %d\n", iRet);
            return -1;
        }
    }else if(iLen < 0){
        printf("error from read: %s\n", snd_strerror(iLen));
        return -1;
    }
    /*printf("Get sound success, iLen is %d, frames is %d, strlen(mono) is %d\n,"*,
            iLen, (int)frames, strlen(mono));
    */

    iRet = write(wavFd, mono, sizeof(char) * frames *2);
    if(iRet < 0){
        printf("Fail to write pcm file!\n");
        return -1;
    }
    return iRet;
}


