
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
#include <linux/fb.h>

//#include "tccfb_ioctl.h"

struct fb_fix_screeninfo fb_finfo;
struct fb_var_screeninfo fb_vinfo;
static char *frame_buffer=NULL;
static int fb_line_len;
static size_t fb_size;
static int fb_dev_fd = -1;
static char input_filename[256];
static int  vo_3d = 0;
static char fb_dev_name[20]="/dev/fb2";

void put_byte(FILE *s, unsigned int b)
{
    char tmp = b;
    fwrite(&tmp, sizeof(char), 1, s);
}

void put_le16(FILE *s, unsigned int val)
{
    put_byte(s, val);
    put_byte(s, val >> 8);
}

unsigned int get_byte(FILE *s)
{
   unsigned int tmp;
   fread(&tmp, sizeof(char), 1, s);
   return tmp&0xFF;
}

unsigned int get_le32(FILE *s)
{
    unsigned int val;
    val = get_byte(s);
    val |= get_byte(s) << 8;
    val |= get_byte(s) << 16;
    val |= get_byte(s) << 24;
    return val;
}

static void lots_of_printf(void)
{
	printf( "var info:\n");
	printf( "xres: %u\n", fb_vinfo.xres);
	printf( "yres: %u\n", fb_vinfo.yres);
	printf( "xres_virtual: %u\n", fb_vinfo.xres_virtual);
	printf( "yres_virtual: %u\n", fb_vinfo.yres_virtual);
	printf( "xoffset: %u\n", fb_vinfo.xoffset);
	printf( "yoffset: %u\n", fb_vinfo.yoffset);
	printf( "bits_per_pixel: %u\n", fb_vinfo.bits_per_pixel);
	printf( "grayscale: %u\n", fb_vinfo.grayscale);
	printf( "red: %lu %lu %lu\n",
			(unsigned long) fb_vinfo.red.offset,
			(unsigned long) fb_vinfo.red.length,
			(unsigned long) fb_vinfo.red.msb_right);
	printf( "green: %lu %lu %lu\n",
			(unsigned long) fb_vinfo.green.offset,
			(unsigned long) fb_vinfo.green.length,
			(unsigned long) fb_vinfo.green.msb_right);
	printf( "blue: %lu %lu %lu\n",
			(unsigned long) fb_vinfo.blue.offset,
			(unsigned long) fb_vinfo.blue.length,
			(unsigned long) fb_vinfo.blue.msb_right);
	printf( "transp: %lu %lu %lu\n",
			(unsigned long) fb_vinfo.transp.offset,
			(unsigned long) fb_vinfo.transp.length,
			(unsigned long) fb_vinfo.transp.msb_right);
	printf( "nonstd: %u\n", fb_vinfo.nonstd);
	printf( "activate: %u\n", fb_vinfo.activate);
	printf( "height: %u\n", fb_vinfo.height);
	printf( "width: %u\n", fb_vinfo.width);
	printf( "accel_flags: %u\n", fb_vinfo.accel_flags);
	printf( "timing:\n");
	printf( "pixclock: %u\n", fb_vinfo.pixclock);
	printf( "left_margin: %u\n", fb_vinfo.left_margin);
	printf( "right_margin: %u\n", fb_vinfo.right_margin);
	printf( "upper_margin: %u\n", fb_vinfo.upper_margin);
	printf( "lower_margin: %u\n", fb_vinfo.lower_margin);
	printf( "hsync_len: %u\n", fb_vinfo.hsync_len);
	printf( "vsync_len: %u\n", fb_vinfo.vsync_len);
	printf( "sync: %u\n", fb_vinfo.sync);
	printf( "vmode: %u\n", fb_vinfo.vmode);
	printf( "fix info:\n");
	printf( "framebuffer size: %d bytes\n", fb_finfo.smem_len);
	printf( "type: %lu\n", (unsigned long) fb_finfo.type);
	printf( "type_aux: %lu\n", (unsigned long) fb_finfo.type_aux);
	printf( "visual: %lu\n", (unsigned long) fb_finfo.visual);
	printf( "line_length: %lu bytes\n", (unsigned long) fb_finfo.line_length);
	printf( "id: %.16s\n", fb_finfo.id);
	printf( "smem_start: %p\n", (void *) fb_finfo.smem_start);
	printf( "xpanstep: %u\n", fb_finfo.xpanstep);
	printf( "ypanstep: %u\n", fb_finfo.ypanstep);
	printf( "ywrapstep: %u\n", fb_finfo.ywrapstep);
	printf( "mmio_start: %p\n", (void *) fb_finfo.mmio_start);
	printf( "mmio_len: %u bytes\n", fb_finfo.mmio_len);
	printf( "accel: %u\n", fb_finfo.accel);
	printf( "other:\n");
}
static int fb_preinit()
{
	fb_dev_fd = open (fb_dev_name, O_RDWR);
	if (fb_dev_fd == -1) 
	{
		printf(" Can't open /dev/fb0 %s\n", strerror(errno));
		return -1;
	}
       else
            printf("open device is %s \n",fb_dev_name);
        	
	if (ioctl(fb_dev_fd, FBIOGET_FSCREENINFO, &fb_finfo)) {
		printf("Can't get FSCREENINFO: %s\n", strerror(errno));
		return -1;
	}
	if (ioctl(fb_dev_fd,FBIOGET_VSCREENINFO,&fb_vinfo)){
		printf("Error reading variable information\n");
		return -1;
	}
	
	fb_line_len = fb_finfo.line_length;
	fb_size = fb_finfo.smem_len;
	frame_buffer = NULL;

	if ((frame_buffer = (char *) mmap(0, fb_size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fb_dev_fd, 0)) == (char *) -1) 
	{
		printf("Can't mmap /dev/fb0 %s\n", strerror(errno));
		return -1;
	}

   // memset(frame_buffer, '\0', fb_line_len *fb_vinfo.yres);
	return 0;	
}

int parse_command_line( int argc, char **argv)
{
	int i;
  	char *opt;
  	int no_more_opts = 0;

	for (i = 1; i < argc; i++) 
	{
		opt = argv[i];
		/* check for -- (no more options id.) except --help! */
		if ((*opt == '-') && (*(opt+1) == '-') && (*(opt+2) == 0))
		{
			no_more_opts = 1;
			if (i+1 >= argc)
			{
				printf( "'--' indicates no more options, but no filename was given on the command line.\n");
			    goto err_out;
			}
		continue;
		}     			
		if ((no_more_opts == 0) && (*opt == '-') && (*(opt+1) != 0)) /* option */
		{
			opt++;
			printf( "this_opt = option: %s\n", opt);

			if(strcasecmp(opt,"3d") == 0) 
			{
				vo_3d = 1;
			}
			else if(strcasecmp(opt,"fb0") == 0)
			{
			        memset(fb_dev_name,'\0',sizeof(fb_dev_name));
				strcpy(fb_dev_name,"/dev/fb0");
			}
			else if(strcasecmp(opt,"fb1") == 0)
			{
			        memset(fb_dev_name,'\0',sizeof(fb_dev_name));
				strcpy(fb_dev_name,"/dev/fb1");
			}
			else if(strcasecmp(opt,"fb2") == 0)
			{
			        memset(fb_dev_name,'\0',sizeof(fb_dev_name));
				strcpy(fb_dev_name,"/dev/fb2");
			}
			else 
			{
				vo_3d = 0;
			}
		}
		else /* filename */
		{
			printf("Adding file %s\n",argv[i]);
			memset(input_filename,'\0',sizeof(input_filename));
		    strcpy(input_filename, argv[i]);
		}
  }

  return 1;
	
 err_out:
  return 0;
}

int main(int argc, char *argv[])
{	
	unsigned int header_size, width, height;
	unsigned int tmpmix16, tmpR, tmpG, tmpB;
	int i = 0;
	int ret;
	int offset=0,vo_x=0,vo_y=0;
	char *srcbuf;

	//parse_command_line(argc,argv);

    memset(input_filename,'\0',sizeof(input_filename));
    strcpy(input_filename, argv[1]);

	ret = fb_preinit();
	if(ret < 0)
	    return -1;
    
    //lots_of_printf();	

	FILE* input = fopen( input_filename , "r" );
	if(input == NULL)
	{
		printf("can't open %s\n", input_filename);
		return -1;
	}
	
	fseek(input, 10, SEEK_SET);
	header_size = get_le32(input);
	//fseek(input, 18, SEEK_SET);
	//width = get_le32(input);
	//height = get_le32(input);
	//printf("w: %d, h: %d\n", width, height);

    width = 800;
    height = 480;
	srcbuf = malloc(2*width*height);

	offset = (fb_vinfo.yoffset+vo_y+height)*fb_vinfo.xres_virtual*fb_vinfo.bits_per_pixel/8+(fb_vinfo.xoffset+vo_x)*fb_vinfo.bits_per_pixel/8;
		
   fseek(input, header_size, SEEK_SET);
	
	while(!feof(input))
	{
		tmpB = get_byte(input);
		tmpG = get_byte(input);
		tmpR = get_byte(input);

		tmpB = tmpB&0x00F8;
		tmpG = tmpG&0x00FC;
		tmpR = tmpR&0x00F8;
		tmpmix16 = (tmpR<<8) | (tmpG<<3) | (tmpB>>3);

		srcbuf[i] = tmpmix16;
		srcbuf[i+1] = tmpmix16 >> 8;
		i=i+2;
	}
    fclose(input);

	for(i=0;i<height;i++)
	{
		memcpy(frame_buffer + offset-i*fb_vinfo.xres_virtual*fb_vinfo.bits_per_pixel/8 ,srcbuf +i*2*width,width*fb_vinfo.bits_per_pixel/8);
	}


	if(frame_buffer)
		munmap(frame_buffer, fb_size);
	frame_buffer = NULL;

	
	if(fb_dev_fd != -1)	
		close(fb_dev_fd);

	return 0;
}

