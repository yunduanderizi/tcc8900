#ifndef RECORD_H
#define RECORD_H
#include "alsa/asoundlib.h"

#define CH 1
#define PCMBUFFSIZE 163840

int InitPcm(int sampleRate);
int Recording();
int getSampleRate(int m_sample);

#endif
