#ifndef __IO_H__
#define __IO_H__

#include "typedefs.h"

int io2w_init(void);
int  io2w_write(u8 number_bytes, u8 *data_out);
int   io2w_read(u8  number_bytes, u8 *data_in);

#endif  
