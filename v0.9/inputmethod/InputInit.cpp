#include <QApplication>

#include "InputClient.h"
#include "InputServer.h"
#include "swtichslectewidget.h"
#include "pinyininputmethod.h"
#include "handinput.h"
#include "letterwidget.h"
#include "digitwidget.h"
#include "characterwidget.h"
#include "editwidget.h"

#ifdef NEW_FUNCTION_VOICE1
#include "voiceinput.h"
#include "VoiceFunControl.h"
#include "voiceDefine.h"
#include "dbusClientProxy.h"
#include "dbusMessage.h"

using namespace Voice1;
#endif

static void YT_inputServerInit()
{
	YTInputServerC	*server=YTInputServerC::instance();

	server->setMaskPic("/applicaton/work/a.png");
	server->registerInputMethodSelect(new SwitchWidget());

	QString digitalPicFile[3];      
	server->registerInputMethod(YT_INPUT_METHOD_DIGITAL,DigitWidget::factory,digitalPicFile);

	QString alphaPicFile[3];
	PinyinInput::NewLargeLable();
	PinyinInput::NewPINLable();
	server->registerInputMethod(YT_INPUT_METHOD_SPELL,PinyinInput::factory,alphaPicFile);

	QString handPicFile[3];
	server->registerInputMethod(YT_INPUT_METHOD_HANDWRITE,handinput::factory,handPicFile);

	QString englishPicFile[3];
	LetterWidget::NewLargeLable();
	server->registerInputMethod(YT_INPUT_METHOD_ALPHA,LetterWidget::factory,englishPicFile);

	QString symbolPicFile[3];
	CharacterWidget::NewLargeLable();
	server->registerInputMethod(YT_INPUT_METHOD_SYMBOL,CharacterWidget::factory,symbolPicFile);

	QString editPicFile[3];
	server->registerInputMethod(YT_INPUT_METHOD_EDIT,EditWidget::factory,editPicFile);
	
#ifdef NEW_FUNCTION_VOICE1
	QString voicePicFile[3];
	server->registerInputMethod(YT_INPUT_METHOD_VOICE,VoiceInputWidget::factory,voicePicFile);
#endif
}

static void YT_inputClientInit()
{
	YTInputClientC *client=new YTInputClientC();

	client->setCurrentInputMethod(YT_INPUT_METHOD_NONE);
	client->setInputMask(YT_INPUT_METHOD_ALL);
}

void YT_inputInit()
{
	if(qApp->type()==QApplication::GuiServer)
	{
		YT_inputServerInit();
	}
	YT_inputClientInit();
}
void YT_installEdit(QWidget *w)
{
	YTInputClientC::installEditWidget(w); 	
}
