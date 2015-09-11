#include "cv.h"
#include "highgui.h"
#include <math.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>

int scalejpeg(char *srcName, char *destName, unsigned int width, unsigned int height);

