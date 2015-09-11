
#include "dbusMessage.h"
#include "voiceDefine.h"
#include "message/voiceMessage.h"
#include "dbusApplication.h"
#include "myDbus.h"
#include "ivTts.h"
#include "audioControlLib.h"
#include "AiPlay.h"
#include "pcmPlayerManager.h"

int main(int argc, char *argv[])
{
	DBusApplicationC app(argc, argv);

	MyDbusC::object()->initDbus();
	IBD_INIT("voice1", MyDbusC::object()->getDbusServer());

	AudioControl1::AudioControlLibC::object();

	//AiPlayC::object();

	PcmPlayerManagerC::object();

	IvTtsC::object()->PlayText("语音输入开始");
	IvTtsC::object()->PlayText("语音模块开启");

	return app.loop();
}

