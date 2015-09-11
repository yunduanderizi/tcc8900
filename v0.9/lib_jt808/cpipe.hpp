#ifndef _C_PIPE_
#define _C_PIPE_
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string>

using namespace std;

class CFifo{
private:
#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define READ	0
#define WRITE 	1
	string fifo_name;
	int rwflag;
//	int fd;
	CFifo();
	int Open(const char *name, int oflag, ...);
public:
	int fd;
	CFifo(const char * name, int rw);
	ssize_t Read(void *where, size_t n);
	ssize_t Write(void *what, size_t n);
	~CFifo();
};
#endif
