#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "expat.h"
#include "writecard.h"


#define BUFFSIZE   8192 
char Buff[BUFFSIZE];
char value[16][1024];
int Depth;

#define FILE_NAME		"ICsetting.xml"

enum READ_ZONE
{
	DATA_ZONE,//数据区
	PROC_ZONE,//保护区
	SCUR_ZONE //密码区
};

struct sle5542_ioctl_data {	
	unsigned int  userzone;		//区域
	unsigned char password[4];  //密码
};

#define SLE5542_IOCTL_BASE      'C'
#define SLE5542_READ_ZONE        _IOW(SLE5542_IOCTL_BASE, 0, struct sle5542_ioctl_data)	//选择读写区域
#define SLE5542_CKECK_PASSWD     _IOW(SLE5542_IOCTL_BASE, 1, struct sle5542_ioctl_data) //验证密码
#define SLE5542_CHANGE_PASSWD    _IOW(SLE5542_IOCTL_BASE, 2, struct sle5542_ioctl_data) //改写密码

unsigned char DECToBCD(unsigned char dec_number)
{
	return(((dec_number/10)<<4) | (dec_number%10));
}

int sum_for_xor(char *buf, int len)
{
	int i = 0;
	int result = 0;

	if(!buf)
		return result;
	
	for(i = 0; i < len; i++)
	{
		result ^= buf[i]; 
	}

	return result;
}

void start(void *data, const char *el, const char **attr)
{
	if(data || el || *attr)
		return;
} /* End of start handler */ 
 
void end(void *data, const char *el) 
{
	if(data || el)
		return;
} /* End of end handler */


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
	printf("value:%s\n", value[Depth -1]);

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

	char path_buffer[1024] = {0};
	if(*(path + strlen(path)) == '/')
	{
		sprintf(path_buffer, "%s%s", path, FILE_NAME);
	}
	else
	{
		sprintf(path_buffer, "%s/%s", path, FILE_NAME);
	}

	FILE *fp = fopen(path_buffer, "r");
	if(!fp)
	{
		printf("open file error!\n");
		ret =  NO_FILE;
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
 
      len = fread(Buff, 1, BUFFSIZE, fp);
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



int read_card(int fd)
{
	char buffer[1024] = {0};

	lseek(fd, 0, SEEK_SET);
	int i = 0;
	read(fd, buffer, 28);//前28个字节厂商固定
	for(i = 0; i < 28; i++)
	{
		printf("%x", buffer[i]);
		printf("\n");
	}
	printf("\n");
	memset(buffer, 0, 28);
	
	read(fd, buffer, 4);//这四个字节为写入的
	printf("FLAG:%s\n",buffer);
	
	read(fd, buffer, 18);//这18个字节为驾驶员编号
	printf("Number:");
	for(i = 0; i < 18; i++)
	{
		printf("%c", buffer[i]);
	}
	printf("\n");
	
	read(fd, buffer, 3);//这三个字节是有效期
	for(i = 0; i < 3; i++)
	{
		printf("abuf[%d]:%x\n", i, buffer[i]);
	}
	
	read(fd, buffer, 20);//这18个字节是从业资格证号
	printf("QualificationID:");
	for(i = 0; i < 20; i++)
	{
		printf("%c", buffer[i]);
	}
	printf("\n");
	
	memset(buffer, 0, 16);//这16个字节是驾驶员名字
	read(fd, buffer, 16);
	printf("Name:%s\n", buffer);	

	return 1;
}


int _write_card(int fd, struct sle5542_ioctl_data *sle5542, char value[][1024])
{
	int ret = -1;

	char write_buffer[128] = {0};
#if 0
	char head_buffer[28] = {0xa2,0x13,0x10,0x91,
							  0xff,0xff,0x81,0x15,
							  0xff,0xff,0xff,0xff,
							  0xff,0xff,0xff,0xff,
							  0xff,0xff,0xff,0xff,
							  0xff,0xd2,0x76,0x00,
							  0x00,0x40,0x00,0xff};
	memcpy(write_buffer, head_buffer, 28);
#endif	

	ret = ioctl(fd, SLE5542_CKECK_PASSWD, sle5542);
	if(ret < 0)
	{
		printf("error in checkout ioctl!\n");
		return -1;
	}

	lseek(fd, 0, SEEK_SET);
	read(fd, write_buffer, 28);//前28个字节厂商固定
	lseek(fd, 0, SEEK_SET);

	unsigned int _year = 0;
	unsigned int _month = 0;
	unsigned int _day = 0; 
	sscanf(value[3], "%d-%d-%d", &_year, &_month, &_day);
	_year %= 100;
	//printf("year:%d\n month:%d\n day:%d\n", _year, _month, _day);

	unsigned char year = DECToBCD(_year);
	unsigned char month = DECToBCD(_month);
	unsigned char day = DECToBCD(_day);

	//printf("year:%x\n month:%x\n day:%x\n", year, month, day);
	
	memcpy(&write_buffer[28], "HTKJ", 4);
	memcpy(&write_buffer[32], value[0], 18);	//驾驶证编号  
	memcpy(&write_buffer[50], &year, 1);		//有效日期
	memcpy(&write_buffer[51], &month, 1);	
	memcpy(&write_buffer[52], &day, 1);	
	memcpy(&write_buffer[53], value[2], 20);	//从业资格证号
	int level = atoi(value[1]);
	printf("current yuan:%d\n", level);
	if(0 == level)
	{
		memcpy(&write_buffer[73], "驾驶员", 16);	//驾驶员姓名
	}
	else if(1 == level)
	{
		memcpy(&write_buffer[73], "管理员", 16);
	}

	int sum = sum_for_xor(write_buffer, 127);
	write_buffer[127] = sum;
	int i = 0;
	for(i = 0; i < 128; i++)
	{
		printf("%02x ", write_buffer[i]);
		if((i+1)%4 == 0)
		{
			printf("\n");
		}
	}

	char *p = write_buffer;
	sum = 0;
	int cnt = 0;
	for(i = 0; i < 32; i++)
	{
		cnt = write(fd, p, 4);
		if(cnt <= 0)
		{
			fprintf(stdout, "write data error!\n");
			return -1;
		}
		sum += cnt;
		p += 4;
	}
	fprintf(stdout, "wirte %d data successfully!\n", sum);

	return sum;
}

int do_write_card(char value[][1024])
{
	int fd = -1;
	int ret = 0;
	//char buffer[1024] = {0};
	
	// 1.打开设备文件		
	fd = open("/dev/sle5542", O_RDWR);
	if(fd < 0)
	{
		printf("open file error!\n");
		return -1;
	}

	// 2.选择操作区域
	struct sle5542_ioctl_data sle5542;

	sle5542.userzone = DATA_ZONE;
	ret = ioctl(fd, SLE5542_READ_ZONE, &sle5542);
	if(ret < 0)
	{
		printf("error in ioctl slecect zone!\n");
		return -1;
	}

	// 3.写数据
	unsigned char pass[3] = {0xFF, 0xFF, 0xFF};
	memcpy(sle5542.password, pass, sizeof(pass));

	fd_set wfds;
   	struct timeval tv;
   	int retval;

    FD_ZERO(&wfds);
    FD_SET(fd, &wfds);
 
    tv.tv_sec = 0;
    tv.tv_usec = 50;
    
    retval = select(fd + 1, NULL, &wfds, NULL, &tv);
    if (retval == -1)
    {
        ret = WRITE_ERROR;
    }
	else if (retval)
    {  
    	if(FD_ISSET(fd, &wfds))
 	 	{
 	 		printf("before write!\n");
 			ret = _write_card(fd, &sle5542, value);
			if(ret < 0)
			{
				ret = WRITE_ERROR;
			}
			else
			{
				ret = WRITE_SUCCESS;
			}
 	 	}
 
    }
    else
    {
 		ret = NO_CARD;
    }

	// 4.关闭文件
	close(fd);

	return ret;
}

 

int write_card(char *path) 
{
	int ret = 0;

	Depth = 0;
	memset(value, 0x0, 16*1024);
	memset(Buff, 0x0, BUFFSIZE);
	ret = parser_xml_file(path);
	if(ret < 0)
		return ret;
	
	return do_write_card(value);
	
} /* End of main */


#if 0
int main()
{
	int ret  = -1;
	ret  = write_card("./");
	printf("ret is %d\n", ret);

	// 1.打开设备文件		
	int fd = open("/dev/sle5542", O_RDWR);
	if(fd < 0)
	{
		printf("open file error!\n");
		return -1;
	}

	read_card(fd);
}
#endif

