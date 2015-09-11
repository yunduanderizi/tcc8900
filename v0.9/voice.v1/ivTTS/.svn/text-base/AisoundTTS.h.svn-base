#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <iostream>
#include <unistd.h>
#include "ivTTS.h"
#include "alsa/asoundlib.h"

#define DSP "/dev/"
#define MAXSIZE 1024

/* constant for TTS heap size */
#define ivTTS_HEAP_SIZE		50000 /* ªÏ∫œ£¨“Ù–ß */

class Aisound
{
public:
	Aisound();
	~Aisound();
	int PlayText(const char* words);

private:
	ivHTTS hTTS;
	ivPByte pHeap;
	ivTResPackDescExt	 tResPackDesc;
//	ivTTTsUserInfo tUserInfo;
	int count; 
	int length;
 
	ivTTSErrID ivCall ParamChangeCB(
		ivPointer       pParameter,		/* [in] user callback parameter */
		ivUInt32		nParamID,		/* [in] parameter id */
		ivUInt32		nParamValue );	/* [in] parameter value */
	ivTTSErrID ivCall ProgressCB(
		ivPointer       pParameter,		/* [in] user callback parameter */
		ivUInt32		iProcPos,		/* [in] current processing position */
		ivUInt32		nProcLen );		/* [in] current processing length */
	int Open_Pcm();
	void Release_Pcm();



};

