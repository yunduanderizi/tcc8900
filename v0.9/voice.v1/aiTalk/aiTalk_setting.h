/****************************************************************/
/****************************************************************/
/**  本文件服务于语音识别功能的词典设置（在141这个项目中，只需要词典的概念 **/
/**  不需要场景、和句式，因此我们只定义了用于词典设置的结构）            **/
/****************************************************************/
/****************************************************************/

#ifndef AITALK_SETTING_H
#define AITALK_SETTING_H

#include "../../project_config/common_config.h"

#define VOICE_LOG_OUT

#define VOICE_LOG_PATH	INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"voicelog"

/*词典设置结构 -- 用于设置词与遥控码的对应关系*/
typedef struct _Lexicon_Setting
{
        char* 		item;       /*词*/
        char*		KeyCode;     /*遥控码*/
}Lexicon_Setting;

#endif // AITALK_SETTING_H

