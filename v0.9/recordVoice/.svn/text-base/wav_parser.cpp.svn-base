#ifndef _WAV_PARSER_H
#define _WAV_PARSER_H


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "wav_parser.h"
#include "record.h"
#define WAV_PRINT_MSG

WAVContainer_t wav_file_header;

const char *WAV_P_FmtString(uint16_t fmt){

    switch (fmt){
        case WAV_FMT_PCM:
            return "PCM";
            break;
        case WAV_FMT_IEEE_FLOAT:
            return "IEEE FLOAT";
            break;
        case WAV_FMT_DOLBY_AC3_SPDIF:
            return "DOLBY AC3 SPDIF";
            break;
        case WAV_FMT_EXTENSIBLE:
            return "EXTENSIBLE";
            break;
        default:
            break;
    }
    return "NON Support Fmt";
}
void PrintWavHeader(WAVContainer_t *container)
{
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/n");
	printf("\n");
	printf("File Magic:         [%c%c%c%c]/n", 
		(char)(container->header.magic), 
		(char)(container->header.magic>>8), 
		(char)(container->header.magic>>16), 
		(char)(container->header.magic>>24));
	printf("File Length:        [%d]\n", container->header.length);
	printf("File Type:          [%c%c%c%c]\n",
		(char)(container->header.type), 
		(char)(container->header.type>>8), 
		(char)(container->header.type>>16), 
		(char)(container->header.type>>24));
		
	printf("/n");

	printf("Fmt Magic:          [%c%c%c%c]\n",
		(char)(container->format.magic), 
		(char)(container->format.magic>>8), 
		(char)(container->format.magic>>16), 
		(char)(container->format.magic>>24));
	printf("Fmt Size:           [%d]\n", container->format.fmt_size);
	printf("Fmt Format:         [%s]\n", WAV_P_FmtString(container->format.format));
	printf("Fmt Channels:       [%d]\n", container->format.channels);
	printf("Fmt Sample_rate:    [%d](HZ)\n", container->format.sample_rate);
	printf("Fmt Bytes_p_second: [%d]\n", container->format.bytes_p_second);
	printf("Fmt Blocks_align:   [%d]\n", container->format.blocks_align);
	printf("Fmt Sample_length:  [%d]\n", container->format.sample_length);
	
	printf("/n");

	printf("Chunk Type:         [%c%c%c%c]\n",
		(char)(container->chunk.type), 
		(char)(container->chunk.type>>8), 
		(char)(container->chunk.type>>16), 
		(char)(container->chunk.type>>24));
	printf("Chunk Length:       [%d]\n", container->chunk.length);
	
	printf("\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

int WAV_HeaderCheckValid(WAVContainer_t *container)
{
	if (container->header.magic != WAV_RIFF ||
		container->header.type != WAV_WAVE ||
		container->format.magic != WAV_FMT ||
		container->format.fmt_size != LE_INT(16) ||
		(container->format.channels != LE_SHORT(1) && container->format.channels != LE_SHORT(2)) ||
		container->chunk.type != WAV_DATA) {
		
		fprintf(stderr, "non standard wav file.\n");
		return -1;
	}
	return 0;
}

int ReadWavHeader(int fd, WAVContainer_t *container)
{
	assert((fd >=0) && container);

	if (read(fd, &container->header, sizeof(container->header)) != sizeof(container->header) || 
		read(fd, &container->format, sizeof(container->format)) != sizeof(container->format) ||
		read(fd, &container->chunk, sizeof(container->chunk)) != sizeof(container->chunk)) {

		fprintf(stderr, "Error WAV_ReadHeader\n");
		return -1;
	}

	if (WAV_HeaderCheckValid(container) < 0)
		return -1;

#ifdef WAV_PRINT_MSG
	PrintWavHeader(container);
#endif

	return 0;
}

int WriteWavHeader(int wavfd, WAVContainer_t *wav_header)
{
	assert((wavfd >=0) && wav_header);
	
	if (WAV_HeaderCheckValid(wav_header) < 0)
		return -1;

	if (write(wavfd, &wav_header->header, sizeof(wav_header->header)) != sizeof(wav_header->header) || 
		write(wavfd, &wav_header->format, sizeof(wav_header->format)) != sizeof(wav_header->format) ||
		write(wavfd, &wav_header->chunk, sizeof(wav_header->chunk)) != sizeof(wav_header->chunk)) {
		fprintf(stderr, "Error WAV_WriteHeader\n");
		return -1;
	}

#ifdef WAV_PRINT_MSG
	PrintWavHeader(wav_header);
#endif
	return 0;
}

int FillWavFileHeader(WAVContainer_t *wav_header, int pcmFileLen, int m_sample){

    assert(wav_header && (pcmFileLen > 0));
    int sampleRate;
    if((sampleRate = getSampleRate(m_sample)) == 0){
        printf("Error:getSamepleRate() in FillWavFileHeader()\n");
        return -1;
    }
    /*RIFF*/

    wav_header->header.magic = WAV_RIFF;
    wav_header->header.type = WAV_WAVE;

    /*fmt*/
    wav_header->format.magic = WAV_FMT;
    wav_header->format.fmt_size = LE_INT(16);
    wav_header->format.format = LE_SHORT(WAV_FMT_PCM);
    wav_header->format.channels = LE_SHORT(CH);
    wav_header->format.sample_rate = LE_SHORT(sampleRate);
    wav_header->format.sample_length = LE_SHORT(16);
    wav_header->format.blocks_align = LE_SHORT(CH * wav_header->format
            .sample_length / 8);
    wav_header->format.bytes_p_second = LE_INT((uint16_t)
            (wav_header->format.blocks_align) * sampleRate);
    /*data sub-chunk*/
    wav_header->chunk.type = WAV_DATA;
    wav_header->chunk.length = LE_INT((uint32_t)pcmFileLen);

    wav_header->header.length = LE_INT((uint32_t)
            (wav_header->chunk.length) + 
            sizeof(wav_header->chunk) + 
            sizeof(wav_header->format) + 
            sizeof(wav_header->header) - 8);
    return 0;
}

#endif
