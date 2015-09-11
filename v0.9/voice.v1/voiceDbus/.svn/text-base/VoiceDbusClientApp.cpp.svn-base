#include "dbusClient.h"
#include "dbusMessage.h"
#include "VoiceDbusClientApp.h"
#include "iv_TTS.h"

extern IvTTS			*gs_pIvTTS;/*指向语音合成对象的指针*/
//IvTTSInterface* DbusIvTTSInterface = (IvTTSInterface*)gs_pIvTTS;

void PhoneNotifyHandler(DBusNotifyC *notify)
{
	printf("IN voice !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	if(notify->getIdentifier()!=NOTF_ID_PHONED)
	{
		fprintf(stderr, "notify->getIdentifier()!=NOTF_ID_PHONED\n");
		return;
	}

	PhonedNotifyC *notifyData = (PhonedNotifyC*)notify;

	switch (notifyData->NotifyCInfor.Type.TypeName)
	{
		case MSG_PHONE_CJTSERV_IND:
			if(notifyData->NotifyCInfor.Type.Status == STATUS_NEW_MSG)
			{
				fprintf(stderr, "[Client]: PhoneNum = [%s], MsgCommon = [%s], Time = [%s]....\n", 
				notifyData->NotifyCInfor.cpPhoneNum, notifyData->NotifyCInfor.cpMsgCommon, notifyData->NotifyCInfor.cpTime);
				if(NULL != gs_pIvTTS)
				{
					gs_pIvTTS->PlayString(User_AiTalk, "您有一条新的短信请注意查收");
					break;
				}
			}
			break;
		case MSG_PHONE_ALL:
			if(notifyData->NotifyCInfor.Type.Status == STATUS_SEND_MSG && notifyData->NotifyCInfor.Type.Result == RESULT_SEND_MSG_SUCCESS)
			{
				fprintf(stderr, "[Client]: Send SMS finish....\n");
			}
			else if(notifyData->NotifyCInfor.Type.Status == STATUS_SEND_MSG && notifyData->NotifyCInfor.Type.Result == RESULT_SEND_MSG_FAIL)
			{
				fprintf(stderr, "[Client]: Send SMS Fail....\n");
			}
			break;
		default:
			break;
	}
}
