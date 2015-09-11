#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/types.h>
#include <assert.h>
#include "i2ctl.h"
/* -- i2c.h -- */


/*
 * I2C Message - used for pure i2c transaction, also from /dev interface
 */
struct i2c_msg {
	__u16 addr;	/* slave address			*/
	__u16 flags;
#define I2C_M_TEN	0x10	/* we have a ten bit chip address	*/
#define I2C_M_RD	0x01
#define I2C_M_NOSTART	0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
	__u16 len;		/* msg length				*/
	__u8 *buf;		/* pointer to msg data			*/
};

/* To determine what functionality is present */

#define I2C_FUNC_I2C			0x00000001
#define I2C_FUNC_10BIT_ADDR		0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING	0x00000004 /* I2C_M_{REV_DIR_ADDR,NOSTART,..} */
#define I2C_FUNC_SMBUS_HWPEC_CALC	0x00000008 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_READ_WORD_DATA_PEC  0x00000800 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA_PEC 0x00001000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_PROC_CALL_PEC	0x00002000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL_PEC 0x00004000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL	0x00008000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_QUICK		0x00010000
#define I2C_FUNC_SMBUS_READ_BYTE	0x00020000
#define I2C_FUNC_SMBUS_WRITE_BYTE	0x00040000
#define I2C_FUNC_SMBUS_READ_BYTE_DATA	0x00080000
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA	0x00100000
#define I2C_FUNC_SMBUS_READ_WORD_DATA	0x00200000
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA	0x00400000
#define I2C_FUNC_SMBUS_PROC_CALL	0x00800000
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA	0x01000000
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK	0x04000000 /* I2C-like block xfer  */
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK	0x08000000 /* w/ 1-byte reg. addr. */
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK_2	 0x10000000 /* I2C-like block xfer  */
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK_2 0x20000000 /* w/ 2-byte reg. addr. */
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA_PEC  0x40000000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA_PEC 0x80000000 /* SMBus 2.0 */

#define I2C_FUNC_SMBUS_BYTE I2C_FUNC_SMBUS_READ_BYTE | \
                            I2C_FUNC_SMBUS_WRITE_BYTE
#define I2C_FUNC_SMBUS_BYTE_DATA I2C_FUNC_SMBUS_READ_BYTE_DATA | \
                                 I2C_FUNC_SMBUS_WRITE_BYTE_DATA
#define I2C_FUNC_SMBUS_WORD_DATA I2C_FUNC_SMBUS_READ_WORD_DATA | \
                                 I2C_FUNC_SMBUS_WRITE_WORD_DATA
#define I2C_FUNC_SMBUS_BLOCK_DATA I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_BLOCK_DATA
#define I2C_FUNC_SMBUS_I2C_BLOCK I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
                                  I2C_FUNC_SMBUS_WRITE_I2C_BLOCK
#define I2C_FUNC_SMBUS_I2C_BLOCK_2 I2C_FUNC_SMBUS_READ_I2C_BLOCK_2 | \
                                   I2C_FUNC_SMBUS_WRITE_I2C_BLOCK_2
#define I2C_FUNC_SMBUS_BLOCK_DATA_PEC I2C_FUNC_SMBUS_READ_BLOCK_DATA_PEC | \
                                      I2C_FUNC_SMBUS_WRITE_BLOCK_DATA_PEC
#define I2C_FUNC_SMBUS_WORD_DATA_PEC  I2C_FUNC_SMBUS_READ_WORD_DATA_PEC | \
                                      I2C_FUNC_SMBUS_WRITE_WORD_DATA_PEC

#define I2C_FUNC_SMBUS_READ_BYTE_PEC		I2C_FUNC_SMBUS_READ_BYTE_DATA
#define I2C_FUNC_SMBUS_WRITE_BYTE_PEC		I2C_FUNC_SMBUS_WRITE_BYTE_DATA
#define I2C_FUNC_SMBUS_READ_BYTE_DATA_PEC	I2C_FUNC_SMBUS_READ_WORD_DATA
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA_PEC	I2C_FUNC_SMBUS_WRITE_WORD_DATA
#define I2C_FUNC_SMBUS_BYTE_PEC			I2C_FUNC_SMBUS_BYTE_DATA
#define I2C_FUNC_SMBUS_BYTE_DATA_PEC		I2C_FUNC_SMBUS_WORD_DATA

#define I2C_FUNC_SMBUS_EMUL I2C_FUNC_SMBUS_QUICK | \
                            I2C_FUNC_SMBUS_BYTE | \
                            I2C_FUNC_SMBUS_BYTE_DATA | \
                            I2C_FUNC_SMBUS_WORD_DATA | \
                            I2C_FUNC_SMBUS_PROC_CALL | \
                            I2C_FUNC_SMBUS_WRITE_BLOCK_DATA | \
                            I2C_FUNC_SMBUS_WRITE_BLOCK_DATA_PEC | \
                            I2C_FUNC_SMBUS_I2C_BLOCK

/*
 * Data for SMBus Messages
 */
#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */
#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */
union i2c_smbus_data {
	__u8 byte;
	__u16 word;
	__u8 block[I2C_SMBUS_BLOCK_MAX + 3]; /* block[0] is used for length */
                          /* one more for read length in block process call */
	                                            /* and one more for PEC */
};

/* smbus_access read or write markers */
#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

/* SMBus transaction types (size parameter in the above functions)
   Note: these no longer correspond to the (arbitrary) PIIX4 internal codes! */
#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_DATA    6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
#define I2C_SMBUS_BLOCK_DATA_PEC    8		/* SMBus 2.0 */
#define I2C_SMBUS_PROC_CALL_PEC     9		/* SMBus 2.0 */
#define I2C_SMBUS_BLOCK_PROC_CALL_PEC  10	/* SMBus 2.0 */
#define I2C_SMBUS_WORD_DATA_PEC	   11		/* SMBus 2.0 */


/* ----- commands for the ioctl like i2c_command call:
 * note that additional calls are defined in the algorithm and hw
 *	dependent layers - these can be listed here, or see the
 *	corresponding header files.
 */
				/* -> bit-adapter specific ioctls	*/
#define I2C_RETRIES	0x0701	/* number of times a device address      */
				/* should be polled when not            */
                                /* acknowledging 			*/
#define I2C_TIMEOUT	0x0702	/* set timeout - call with int 		*/


/* this is for i2c-dev.c	*/
#define I2C_SLAVE	0x0703	/* Change slave address			*/
				/* Attn.: Slave address is 7 or 10 bits */
#define I2C_SLAVE_FORCE	0x0706	/* Change slave address			*/
				/* Attn.: Slave address is 7 or 10 bits */
				/* This changes the address, even if it */
				/* is already taken!			*/
#define I2C_TENBIT	0x0704	/* 0 for 7 bit addrs, != 0 for 10 bit	*/

#define I2C_FUNCS	0x0705	/* Get the adapter functionality */
#define I2C_RDWR	0x0707	/* Combined R/W transfer (one stop only)*/
#define I2C_PEC		0x0708	/* != 0 for SMBus PEC                   */
#if 0
#define I2C_ACK_TEST	0x0710	/* See if a slave is at a specific address */
#endif

#define I2C_SMBUS	0x0720	/* SMBus-level access */

/* -- i2c.h -- */


/* Note: 10-bit addresses are NOT supported! */

/* This is the structure as used in the I2C_SMBUS ioctl call */
struct i2c_smbus_ioctl_data {
	char read_write;
	__u8 command;
	int size;
	union i2c_smbus_data *data;
};

/* This is the structure as used in the I2C_RDWR ioctl call */
struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	/* pointers to i2c_msgs */
	int nmsgs;		/* number of i2c_msgs */
};


static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}


static inline __s32 i2c_smbus_write_quick(int file, __u8 value)
{
	return i2c_smbus_access(file,value,0,I2C_SMBUS_QUICK,NULL);
}

static inline __s32 i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

static inline __s32 i2c_smbus_write_byte(int file, __u8 value)
{
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,value,
	                        I2C_SMBUS_BYTE,NULL);
}

static inline __s32 i2c_smbus_read_byte_data(int file, __u8 command)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BYTE_DATA,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command,
                                              __u8 value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BYTE_DATA, &data);
}

static inline __s32 i2c_smbus_read_word_data(int file, __u8 command)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_WORD_DATA,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}

static inline __s32 i2c_smbus_write_word_data(int file, __u8 command,
                                              __u16 value)
{
	union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_WORD_DATA, &data);
}

static inline __s32 i2c_smbus_process_call(int file, __u8 command, __u16 value)
{
	union i2c_smbus_data data;
	data.word = value;
	if (i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                     I2C_SMBUS_PROC_CALL,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}


/* Returns the number of read bytes */
static inline __s32 i2c_smbus_read_block_data(int file, __u8 command,
                                              __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

static inline __s32 i2c_smbus_write_block_data(int file, __u8 command,
                                               __u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BLOCK_DATA, &data);
}

/* Returns the number of read bytes */
static inline __s32 i2c_smbus_read_i2c_block_data(int file, __u8 command,
                                                  __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
	                      I2C_SMBUS_I2C_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

static inline __s32 i2c_smbus_write_i2c_block_data(int file, __u8 command,
                                               __u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_I2C_BLOCK_DATA, &data);
}

/* Returns the number of read bytes */
static inline __s32 i2c_smbus_block_process_call(int file, __u8 command,
                                                 __u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	if (i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                     I2C_SMBUS_BLOCK_PROC_CALL,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}


//void help(void) __attribute__ ((noreturn));


void help(void)
{
  FILE *fptr;
  char s[100];

  fprintf(stderr,"Syntax: i2cset I2CBUS CHIP-ADDRESS DATA-ADDRESS VALUE [MODE]\n");
  fprintf(stderr,"  MODE is 'b[yte]' or 'w[ord]' (default b)\n");
  fprintf(stderr,"  I2CBUS is an integer\n");
  if((fptr = fopen("/proc/bus/i2c", "r"))) {
    fprintf(stderr,"  Installed I2C busses:\n");
    while(fgets(s, 100, fptr))
      fprintf(stderr, "    %s", s);
    fclose(fptr);
  }
 // exit(1);
 }

int i2copen(int i2cbus)
{
  char filename1[20];
  char filename2[20];
  char filename3[20];
  int e1,e2,e3;
  int file;
  char *filename;
  sprintf(filename1,"/dev/i2c-%d",i2cbus);
  sprintf(filename2,"/dev/i2c%d",i2cbus);
  sprintf(filename3,"/dev/i2c/%d",i2cbus);
  if ((file = open(filename1,O_RDWR)) < 0) {
    e1 = errno;
    if ((file = open(filename2,O_RDWR)) < 0) {
      e2 = errno;
      if ((file = open(filename3,O_RDWR)) < 0) {
        e3 = errno;
        if(e1 == ENOENT && e2 == ENOENT && e3 == ENOENT) {
 //         fprintf(stderr,"Error: Could not open file `%s', `%s', or `%s': %s\n",filename1,filename2,filename3,strerror(ENOENT));
        }
        if (e1 != ENOENT) {
 //       fprintf(stderr,"Error: Could not open file `%s' : %s\n",filename1,strerror(e1));
          if(e1 == EACCES)
          	{
		 	;
          	}
  //          fprintf(stderr,"Run as root?\n");
        }
        if (e2 != ENOENT) {
//          fprintf(stderr,"Error: Could not open file `%s' : %s\n", filename2,strerror(e2));
          if(e2 == EACCES)
 //           fprintf(stderr,"Run as root?\n");
 		{	
 			;
          	}
        }
        if (e3 != ENOENT) {
 //         fprintf(stderr,"Error: Could not open file `%s' : %s\n",filename3,strerror(e3));
          if(e3 == EACCES)
          {
		  ;
          }
 //           fprintf(stderr,"Run as root?\n");
        }
       return -1;
      } else {
         filename = filename3;
      }
    } else {
       filename = filename2;
    }
  } else {
    filename = filename1;
  }
  return file;
}

__s32 i2cset(int file,int address,__u8 *values,__u8 length)
{
	int ret;
	struct i2c_rdwr_ioctl_data queue;
	struct i2c_msg msg;
	msg.addr = address;
	msg.flags = 0;
	msg.buf = values;
	msg.len = length;
	queue.msgs = &msg;
	queue.nmsgs = 1;
	ret = ioctl(file,I2C_RDWR,&queue);
	return ret;

}
int i2cset(int file,int address,int daddress,int value,char b_w = 'b')
{
	int size,res;
	long funcs;
	if(b_w == 'b')
		size = I2C_SMBUS_BYTE_DATA;
	else if(b_w == 'w')
		size = I2C_SMBUS_WORD_DATA;
	else {
//		fprintf(stderr,"Error: Invalid mode!\n");
//		help();
		return -1;
	}
	if( (address<0) || (address>0x7f) ){
//		fprintf(stderr,"Error: Chip address invalid!\n");
//		help();
		return -1;
	}
	if( (daddress<0) || (daddress>0xff) ){
//		fprintf(stderr,"Error: Data address invalid!\n");
//		help();
		return -1;
	}
	if( (value<0) || ( (size==I2C_SMBUS_BYTE_DATA) && (value>0xff) ) || ( (size==I2C_SMBUS_WORD_DATA) && (value>0xffff) ) ){
//		fprintf(stderr,"Error: Data value out of range!\n");
//		help();
		return -1;
	}
	/* check adapter functionality */
	if (ioctl(file,I2C_FUNCS,&funcs) < 0) {
//		fprintf(stderr,"Error: Could not get the adapter functionality matrix: %s\n",strerror(errno));
//		exit(1);
		return -1;
	}
	switch(size) {
		case I2C_SMBUS_BYTE_DATA:
			if( ! (funcs & (I2C_FUNC_SMBUS_WRITE_BYTE_DATA | I2C_FUNC_SMBUS_READ_BYTE_DATA) ) ) {
	//			fprintf(stderr, "Error: Adapter for i2c bus %d", i2cbus);
	//			fprintf(stderr, " does not have byte write capability\n");
	//			exit(1);
				return -1;
			}
			break;
		case I2C_SMBUS_WORD_DATA:
			if( !(funcs & (I2C_FUNC_SMBUS_WRITE_WORD_DATA | I2C_FUNC_SMBUS_READ_WORD_DATA) ) ) {
	//			fprintf(stderr, "Error: Adapter for i2c bus %d", i2cbus);
	//			fprintf(stderr, " does not have word write capability\n");
	//			exit(1);
				return -1;
			}
			break;
	}

	res = -1;
	int nTimes = (address==0x50)? 100:1;
	while( (res<0) && (nTimes--) )
	{
		/* use FORCE so that we can write registers even when a driver is also running */
		if ( (res=ioctl(file,I2C_SLAVE_FORCE,address)) < 0) 
		{
			fprintf(stderr,"Error: Could not set address to %d: res is %d error is %s, try times is %d\n", address, res, strerror(errno), nTimes);
			usleep( 10000 );
		}
		else
		{
			if(size == I2C_SMBUS_WORD_DATA)
				res = i2c_smbus_write_word_data(file, daddress, value);
			else
				res = i2c_smbus_write_byte_data(file, daddress, value);

			if(res<0)
			{
				fprintf(stderr, "Warning - write data failed: res is %d error is %s, try times is %d\n", res, strerror(errno), nTimes);
				usleep( 10000 );
			}
		}
	}

	return res;
}

int i2cget(int file,int address,int daddress,char b_w = 'b')
{
	int size,res;
	long funcs;
	if( b_w == 'b' )
		size = I2C_SMBUS_BYTE_DATA;
	else if( b_w == 'w')
		size = I2C_SMBUS_WORD_DATA;
	else {
//		fprintf(stderr,"Error: Invalid mode!\n");
//		help();
		return -1;
	}
	if( (address<0) || (address>0x7f) ){
//		fprintf(stderr,"Error: Chip address invalid!\n");
//		help();
		return -1;
	}
	if( (daddress<0) || (daddress>0xff) ){
//		fprintf(stderr,"Error: Data address invalid!\n");
//		help();
		return -1;
	}
	/* check adapter functionality */
	if (ioctl(file,I2C_FUNCS,&funcs) < 0) {
//		fprintf(stderr,"Error: Could not get the adapter functionality matrix: %s\n",strerror(errno));
//		exit(1);
		return -1;
	}
	switch(size) {
		case I2C_SMBUS_BYTE_DATA:
			if(!(funcs & (I2C_FUNC_SMBUS_WRITE_BYTE_DATA | I2C_FUNC_SMBUS_READ_BYTE_DATA))) {
		//		fprintf(stderr, "Error: Adapter for i2c bus %d", i2cbus);
		//		fprintf(stderr, " does not have byte write capability\n");
		//		exit(1);
				return -1;
			}
			break;
		case I2C_SMBUS_WORD_DATA:
			if(!(funcs & (I2C_FUNC_SMBUS_WRITE_WORD_DATA | I2C_FUNC_SMBUS_READ_WORD_DATA))) {
		//		fprintf(stderr, "Error: Adapter for i2c bus %d", i2cbus);
		//		fprintf(stderr, " does not have word write capability\n");
		//		exit(1);
				return -1;
			}
			break;
	}
	/* use FORCE so that we can write registers even when a driver is also running */
	if (ioctl(file,I2C_SLAVE_FORCE,address) < 0) {
	//	fprintf(stderr,"Error: Could not set address to %d: %s\n",address,strerror(errno));
	//	exit(1);
		return -1;
	}
	if(size == I2C_SMBUS_WORD_DATA) {
		res = i2c_smbus_read_word_data(file, daddress);
	} else {
		res = i2c_smbus_read_byte_data(file, daddress);
	}
	if(res<0) {
//		fprintf(stderr, "Warning - readback failed\n");
		return -1;
	}
	return res;
}
__s32 i2cget(int file,int address,__u8 *values,__u8 length)
{
        int ret;
        struct i2c_rdwr_ioctl_data queue;
        struct i2c_msg msg;
        msg.addr = address;
        msg.flags = 1;
        msg.buf = values;
        msg.len = length;
        queue.msgs = &msg;
        queue.nmsgs = 1;
        ret = ioctl(file,I2C_RDWR,&queue);
        return ret;
}
int i2cget(int file,int address)
{
	int i;
	i = -1;
	ioctl(file,I2C_SLAVE_FORCE,address);
	i = i2c_smbus_read_byte(file);
	printf("read = %d",i);
	return i;
}
void i2cclose(int file)
{
	int res;
	res = close(file);
//	if(res == 0)
//		fprintf(stderr,"i2c device was closed");
//	else
//		fprintf(stderr,"i2c device close failed");
}

