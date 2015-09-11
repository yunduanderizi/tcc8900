#include "audio_control_message.h"

// audio control
void AudioCtrlRequestC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Ctr_Req_devId=%d\n", devId);
    fprintf(fp,"\t Ctr_Req_operId=%d\n", operId);
    fprintf(fp,"\t Ctr_Req_volume=%d\n\n", volume);
    fprintf(fp,"\t Ctr_Req_on_off=%d\n\n", on_off);
*/
}


void AudioCtrlReplyC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Ctr_Rep_devId=%d\n", devId);
    fprintf(fp,"\t Ctr_Rep_volume=%d\n\n", volume);
    fprintf(fp,"\t Ctr_Rep_on_off=%d\n\n", on_off);
*/
}

void AudioCtrlNotifyC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Ctr_Noti_devId=%d\n", devId);
    fprintf(fp,"\t Ctr_Noti_volume=%d\n\n", volume);
    fprintf(fp,"\t Ctr_Noti_on_off=%d\n\n", on_off);
    fprintf(fp,"\t Ctr_Noti_sound_status=%d\n\n", soundStatus);
*/
}

// audio set
void AudioSetRequestC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Set_Req_devId=%d\n", devId);
    fprintf(fp,"\t Set_Req_operId=%d\n", operId);
    fprintf(fp,"\t Set_Req_volume=%d\n\n", volume);
    fprintf(fp,"\t Set_Req_on_off=%d\n\n", on_off);
    fprintf(fp,"\t Set_Req_soundStatus=0x%2x\n\n", soundStatus);
*/
}

void AudioSetReplyC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Set_devId=%d\n", devId);
    fprintf(fp,"\t Set_volume=%d\n\n", volume);
    fprintf(fp,"\t Set_on_off=%d\n\n", on_off);
    fprintf(fp,"\t Set_soundStatus=0x%2x\n\n", soundStatus);
*/
}

void AudioSetNotifyC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Set_devId=%d\n", devId);
    fprintf(fp,"\t Set_volume=%d\n\n", volume);
    fprintf(fp,"\t Set_on_off=%d\n\n", on_off);
    fprintf(fp,"\t Set_soundStatus=0x%2x\n\n", soundStatus);
*/
}

// audio get
void AudioGetRequestC::print(FILE *fp)
{
    fprintf(fp,"\t Get_Req! \n");
}

void AudioGetReplyC::print(FILE *fp)
{
    for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
    {
        fprintf(fp,"\tGet_Rep\t devid=%d, on_off=%d, volume=%d, soundStatus=0x%2x\n",   \
                allInfo[i].id.logicId,  allInfo[i].on_off, allInfo[i].volume, allInfo[i].soundStatus
                );
    }
}

void AudioGetNotifyC::print(FILE *fp)
{
/*
    fprintf(fp,"\t Get_Noti_devId=%d\n", devId);
    fprintf(fp,"\t Set_Noti_volume=%d\n\n", volume);
    fprintf(fp,"\t Set_Noti_on_off=%d\n\n", on_off);
*/
}

// audio speed set vol
void AudioSpeedSetVolRequestC::print(FILE *fp)
{
}

void AudioSpeedSetVolReplyC::print(FILE *fp)
{
}

void AudioSpeedSetVolNotifyC::print(FILE *fp)
{
}

