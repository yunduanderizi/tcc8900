
#ifndef VOICE_SETTING_H
#define VOICE_SETTING_H

#include "debugMonitor.h"
#include "../project_config/common_config.h"

/* 如果需要记录日志，VOICE_LOG定义为1，否则定义为0 */
/* 科大讯飞语音识别的日志将打印到AitalkLog.esl ，本文件的日志将通过printf打印输出*/

#define VOICE_LOG					(0)

#if		VOICE_LOG
	#define VOICE_PRINT    					printf
	#define BEGINFUNC_USING_BY_VOICE		printf("[%s] %s begin \n", __FILE__, __FUNCTION__);
	#define ENDFUNC_USING_BY_VOICE		printf("[%s] %s end \n", __FILE__, __FUNCTION__ );
	#define AISOUND						1

#else
	#define VOICE_PRINT
	#define BEGINFUNC_USING_BY_VOICE
	#define ENDFUNC_USING_BY_VOICE
	#define AISOUND						0
#endif




#define DBUG_LOG
#ifdef DBUG_LOG
#define dbug_out( fmt, arg... )  do{                        \
		IBD_PRINTF(4, fmt, ##arg); 			\
	}while( 0 )
#else
#define dbug_out( fmt, arg... )
#endif




/* 打开/关闭语音识别的模式 */
/* 0 -- 打开/关闭语音识别会播放提示语音*/
/* 1 -- 打开/关闭语音识别会播放提示音(很短的声音)*/
/* 2 -- 打开/关闭语音识别不会有任何提示*/

#define VOICE_OPEN_CLOSE_MODE			(1)



//ABCDEF

//#define  PCM_DATA_FILE_MOTHER_NAME    "voicefile/pcmliu"



//char motherName[256];

//strcpy(motherName, ABCDEF);
//strcat(motherName, PCM_DATA_FILE_MOTHER_NAME);
/*下面定义了voice中需要打开的文件( 部分)的全名*/

//PCM调试文件的母名，调试时，从麦克风收到的数据，为存储到以这个母名打头的文件中
#define PCM_DATA_FILE_MOTHER_NAME		INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"voicefile/pcmliu" 

//语音识别的适应文件，语音识别模块会学习不同人的发音，学习结果会记录在这个文件中
#define VOICE_IDENTIFY_ADAPT_FILE			(INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"AitalkData.bin")

//语音识别日志文件
#define AITALK_LOG_FILE						INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"Aitalk.log"	

#define IVESR_FILE	"ivESR.irf"

#define RESOURCE_FILE	"Resource.irf"
#endif // VOICE_SETTING_H

