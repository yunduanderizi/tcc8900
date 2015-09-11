#include <stdio.h>
#include "voiceInterface.h"
#include "VoiceDbusApp.h"
#include "dbusServer.h"
#include "dbusMessage.h"
#include "voice_setting.h"
#include "iv_TTS.h"
#include "aiTalk.h"

#if 1
extern IvTTS			*gs_pIvTTS;/*指向语音合成对象的指针*/
#endif
extern AiTalk			*gs_pAiTalk;/*指向语音识别对象的指针*/

extern DBusServerC	*pServer;


void VoiceRequestServiceHandler(DBusRequestC * request,DBusReplyC * & reply)
{
	BEGINFUNC_USING_BY_VOICE

	bool enableInAiTalk = true;
	bool prohibitAiTalk = false;

	if(request->getIdentifier() != REQ_ID_VOICE)
	{
		return;
	}
	
#if 0  //test notify
	VoiceNotifyC *notifyData = new VoiceNotifyC();
#endif

	VoiceRequestC *data = (VoiceRequestC*)request;
	VoiceReplyC *dataReply = new VoiceReplyC();


	/*获得语音合成接口(IvTTSInterface) 的指针*/
#if 1
	IvTTSInterface* pIvTTSInterface = (IvTTSInterface*)gs_pIvTTS;
#endif

	/*提供给主线程调用的语音识别接口*/
	AiTalkInterfaceForMainthread* pAiTalkInterfaceForMainthread = (AiTalkInterfaceForMainthread*)gs_pAiTalk;

	switch(data->RequestCInfor.VoiceCmd)
	{
		case PlayStr: 			//合成播放文本
#if 1
			if(NULL != pIvTTSInterface)
			{
				pIvTTSInterface->PlayString(User_MainThread, data->RequestCInfor.CmdCon.Content);
				dataReply->ReplyCInfor.Res = Ok;
				break;
			}
			else
			{
				dataReply->ReplyCInfor.Res = Fail;
				break;
			}
#endif
		case Mute:
			
		case Unmute:
			
		case Open_amplifier:
			
		case Close_amplifier:

		case Change_audio_input:
			
		case Change_audio_output:

		case Get_aitalk_state:
			if( NULL != pAiTalkInterfaceForMainthread )
			{
				if(pAiTalkInterfaceForMainthread->GetIdentifyState())
				{
					dataReply->ReplyCInfor.Res = Ok;
					break;
				}
				else
				{
					dataReply->ReplyCInfor.Res = Fail;
					break;
				}
			}				
		case Prohibit_identify:
			if( NULL != pAiTalkInterfaceForMainthread )
			{
#if 0
				pAiTalkInterfaceForMainthread->ProhibitEnterIdentifyMode();
#endif
				dataReply->ReplyCInfor.Res = Ok;
				prohibitAiTalk = true;
				break;
			}
			else
			{
				dataReply->ReplyCInfor.Res = Fail;
				break;
			}
		case Allow_identify:
			if( NULL != pAiTalkInterfaceForMainthread )
			{
#if 0
				pAiTalkInterfaceForMainthread->AllowEnterIdentifyMode();
#endif
				dataReply->ReplyCInfor.Res = Ok;
				prohibitAiTalk = false;
				break;
			}
			else
			{
				dataReply->ReplyCInfor.Res = Fail;
				break;
			}
		case StartAiTalk:
			if(!enableInAiTalk)
			{
				dataReply->ReplyCInfor.Res = BeMute;
				fprintf(stderr, "StartAiTalk...........................BeMute.........................\n");
				break;
			}
			if(prohibitAiTalk)
			{
				dataReply->ReplyCInfor.Res = ProhibitAitalk;
				fprintf(stderr, "StartAiTalk.......................ProhibitAitalk.............................\n");
				break;
			}
			if(NULL != pAiTalkInterfaceForMainthread)
			{
				if(pAiTalkInterfaceForMainthread->StartIdentify())
				{
					dataReply->ReplyCInfor.Res = Ok;
					fprintf(stderr, "StartAiTalk.........................Ok...........................\n");
					break;
				}
				else
				{
					dataReply->ReplyCInfor.Res = Fail;
					fprintf(stderr, "StartAiTalk............................Fail........................");
					break;
				}
			}
			else
			{
				dataReply->ReplyCInfor.Res = Fail;
				break;
			}
		case StopAiTalk:
			if(NULL != pAiTalkInterfaceForMainthread)
			{
				pAiTalkInterfaceForMainthread->StopIdentify();
				dataReply->ReplyCInfor.Res = Ok;
				break;
			}
			else
			{
				dataReply->ReplyCInfor.Res = Fail;
				break;
			}
		default:
			dataReply->ReplyCInfor.Res = Fail;
			break;
	}

	reply = dataReply;

	BEGINFUNC_USING_BY_VOICE
}

/*
void Notify(VoiceNotifyC data)//, DBusServerC pServer
{
	VoiceNotifyC notifyData;
	
	notifyData.NotifyCInfor = data.NotifyCInfor;
	
	pServer->sendNotify(&notifyData);
}
*/
