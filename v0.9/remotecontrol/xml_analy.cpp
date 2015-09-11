#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "expat.h"

#include "xml_analy.h"


#define WRITE_ERROR -1
#define NO_FILE -2


#define BUFF_SIZE 8192
char Buff[BUFF_SIZE];
char value[16][512]; 
int Depth;


void start(void *data, const char *el, const char **attr)
{
    if(data || el || *attr)
        return;
} 

void end(void *data, const char *el) 
{
    if(data || el)
        return;
}


void ElementData(void *data, const char*el, int len)
{
    if(data)
        printf("data:%s\n", (char *)data);

    const char *ptr = el;
    const char *ptr_end = el + len - 1;

    if(strlen(el) <= 0)
    {
        return;
    }

    while(ptr <= ptr_end)
    {
        if((*ptr != ' ') && (*ptr != '\t') && (*ptr != '\r') && (*ptr != '\n'))
        {
            break;
        }
        ptr ++;
    }

    while (ptr <= ptr_end)
    {
        if((*ptr_end != ' ') && (*ptr_end != '\t') && (*ptr_end != '\r') && (*ptr_end != '\n'))
        {
            break;
        }
        ptr_end --;
    }

    if(ptr_end < ptr)
    {
        return;
    }

    memcpy(value[Depth++], ptr, ptr_end - ptr + 1);
}

int parser_xml_file(char *path)
{ 
    int ret = 0;

    if(NULL == path)
    {
        printf("path error!\n");
        ret = NO_FILE;
        return ret;
    }

    char *path_buffer = path;

	FILE *fp = fopen(path_buffer, "r");
    if(!fp)
    {
        printf("open file error!\n");
        ret =  NO_FILE;
        fclose(fp);
        return ret;
    }

	XML_Parser p = XML_ParserCreate(NULL);
    if(!p)
    {
        fprintf(stderr, "Couldn't allocate memory for parser\n");
        ret =  WRITE_ERROR;
        fclose(fp);
        return ret;
    }

	XML_SetElementHandler(p, start, end);
	XML_SetCharacterDataHandler(p, ElementData);

    for(;;)
    {
    int done;
    int len;

        len = fread(Buff, 1, BUFF_SIZE, fp);
        if (ferror(fp)) 
        {
            fprintf(stderr, "Read error\n");
            ret =  WRITE_ERROR;
            break;
        }

        done = feof(fp);

        if(!XML_Parse(p, Buff, len, done)) 
        {
            fprintf(stderr, "Parse error at line %d:\n%s\n",
			(int)XML_GetCurrentLineNumber(p),
			XML_ErrorString(XML_GetErrorCode(p)));
            ret =  WRITE_ERROR;
            break;
        }

        if (done)
            break;
    }

	fclose(fp);

    return ret;
}

int xml_analy(int ctrl[16], char *path, int num)
{
    int i;

	memset(value, 0x0, sizeof(value));
    Depth = 0;
    parser_xml_file(path);
    for(i = 0; i < num; i++)
        ctrl[i] = atoi(value[i]);
   
    return 0;
}


