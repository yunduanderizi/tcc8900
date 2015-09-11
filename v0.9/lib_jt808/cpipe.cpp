#include "cpipe.hpp"
#include "tool.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdarg>

CFifo::CFifo(const char *name, int rw):fifo_name(name),rwflag(rw)
{
	if ((mkfifo(name , FILE_MODE) < 0) && (errno != EEXIST))
		error(1, 1, "can't create %s", name);

	if(rwflag == 0){
		fd = Open(name, O_RDONLY, 0);
	}else{
		fd = Open(name, O_WRONLY, 0);
	}
}
int CFifo::Open(const char *name, int oflag, ...)
{
	int fd;
	va_list ap;
	mode_t mode;
	
//	oflag |= O_NONBLOCK; //will terminate and error =6 
	if(oflag & O_CREAT) {
		va_start(ap, oflag);
		mode = va_arg(ap, mode_t);
		if( (fd = open(name, oflag, mode)) == -1)
			error(1, 1, "open error for %s", name);
		va_end(ap);
	}else{
		if((fd = open(name, oflag)) == -1)
			error(1, 1, "open error for %s", name);
	}
	return (fd);
}

ssize_t 
CFifo::Read(void *ptr, size_t nbytes)
{       
	ssize_t         n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		error(1, 1, "read %d error", fd);
	return(n);
}

int CFifo::Write(void *ptr, size_t nbytes)
{
	int ret;
	if ((ret = write(fd, ptr, nbytes)) != nbytes)
		error(1, 1, "write error");
}

CFifo::~CFifo()
{
	if(fd>0)
		close(fd);
	if(rwflag != 0)
		unlink(fifo_name.c_str());
}
