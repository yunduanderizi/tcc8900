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

class IvTtsC
{
public:
	static IvTtsC* object();
	static void releaseObject();

	int PlayText(const char* words);

private:
//	static IvTTS *thisObject;
	static IvTtsC *thisObject;
	IvTtsC();
	~IvTtsC();

	void _ttsCreate();
	void _ttsRelease();

private:

	ivHTTS				m_hTTS;
	ivTResPackDescExt		m_tResPackDesc;
	ivPByte				m_pHeap;
       
};

