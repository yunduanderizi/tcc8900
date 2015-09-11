#include <pthread.h>
#include "Utility.h"
#include "GB19056Proxy.h"
#include "serialprotocol2012.h"
#include "devicemanager.h"
#include "GeneralTimer.h"
#include "mcuprotocol.h"
#include "DeviceManagerDbusMessage.h"
#include "dbusServer.h"

static int sp_fd = -1;
static unsigned char cmd_buf[MAX_LEN];
static int cmd_len = 0;
static int cmd_flag = 0;
static pthread_mutex_t sp_mutex;
int loc1_lock_flag = 0;
int not_process_loc1 = 0;
int mileage_flag = 0;// 1: need to update deploy mileage

extern pthread_mutex_t engine_list_mutex_loc;
extern DBusServerC *dev_DbusServer;

static int in_DB9_check = 0;
GeneralTimerItem *DB9statustimer = NULL;        
GeneralTimerItem *DB9mileagetimer = NULL;        
extern void data_up_pass_through_info(unsigned char *msgbuf);

int  check_sum(unsigned char *buf,int len)
{
	int i;
	unsigned char temp = 0;

	for(i=0; i<len; i++)
	{
		temp = temp ^ buf[i];
	}
	return temp;
}

int parse_readbuf(unsigned char *readbuf,int readnum,unsigned char *msgcountbuf)
{
	int msgcount = 0;
	int i=0,j=0;
	unsigned char temp[20];
	int datalength;

	for(i=0; i<readnum; i++)
	{
		if(readbuf[i] == 0xAA && readbuf[i+1] == 0x75)
		{
			temp[j] = i;
			j++;
		}				
	}

	for(i = 0;i<j;i++)
	{
		if((readnum -temp[i]) >= 5)
		{
			datalength = readbuf[temp[i] + 3]<<8 | readbuf[temp[i] + 4];
			if((temp[i] + datalength + 7 - 1) <= readnum)
			{
				msgcountbuf[msgcount] = temp[i];
				msgcount++;
			}
		}
	}
	
	return msgcount;
}


//pthread
void* read_serial_data_thread(void *)
{
	int read_fd;
	int len = 0, rev_len=0;
	int data_len=0, i=0,j=0;
	unsigned char rev_buf[MAX_LEN];
	unsigned char *ptr = NULL;

	read_fd = OpenComPort(4, 115200, 8,  "1", 'O');
	if(read_fd < 0)
	{		
		printf("Can not open the serial port\n");		
		return 0;	
	}

	memset(rev_buf, 0, sizeof(rev_buf));

	while(1)
	{
		if(rev_len > MAX_LEN/2)
			memset(rev_buf, 0, sizeof(rev_buf));
		
		len = ReadComPort(read_fd, rev_buf+rev_len, MAX_LEN);
		if(len < 0)
		{
			printf("ReadComPort failed \n");
			break;			
		}
		else 
		{
			rev_len += len;
next:
			for(i=0; i<rev_len; i++)
			{
				if(rev_buf[i] == 0xAA && rev_buf[i+1] == 0x75)
				{
                    if(rev_buf[i+2] == 0xC4)
                        mileage_flag = 2;
					ptr = rev_buf+i;
					printf("i=%d, rev_len=%d\n",i,rev_len);
					break;
				}				
			}

			if((rev_len-i) >= 7)
			{
				data_len = (ptr[3]<<8) | ptr[4];
				
				if((rev_len-i) >= (7+data_len))
				{
					pthread_mutex_lock(&sp_mutex);
					if(cmd_flag)
					{
						memcpy(cmd_buf+cmd_len, ptr, 7+data_len);
						cmd_len = cmd_len+7+data_len;
					}
					else
					{
						memset(cmd_buf, 0, MAX_LEN);
						memcpy(cmd_buf, ptr, 7+data_len);
						cmd_len = 7+data_len;
					}
					cmd_flag = 1;
						
					pthread_mutex_unlock(&sp_mutex);
					
					rev_len = rev_len-i-data_len-7;
					for(j=0; j<rev_len;j++)
					{
						rev_buf[j] = rev_buf[i+7+data_len+j];
                        printf(" 0x%02x ",rev_buf[j]);
					}
                    printf("\n");
                    if(rev_len >= 7)
                        goto next;
					
				}
			}
		}	
	}
	if(read_fd)
		CloseComPort(read_fd);
	return 0;
}

void process_DB9_check_command(unsigned char *buf, int len)
{
    if(buf[len] != check_sum(buf, len))
        return ;
    DeviceDB9CheckDbusNotifyC notify;

    switch(buf[2])
    {
        case (unsigned char)ENUM_GB19056_CMD_CHECK_IN:
            if(!in_DB9_check)
            {
                in_DB9_check = 1;
                StartTimer(DB9statustimer);
                notify.flag = 1;
                dev_DbusServer->sendNotify(&notify);
            }
            else
            {
                StopTimer(DB9statustimer);
                StartTimer(DB9statustimer);
            }
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_MILEAGE:
            I2c_write_DB9_7_check_data(DB9_MILEAGE_CHECK);
            StartTimer(DB9mileagetimer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_PULSE_FACTOR:
            I2c_write_DB9_7_check_data(DB9_PULSE_FACTOR_CHECK);
            StopTimer(DB9mileagetimer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_TIME:
            I2c_write_DB9_7_check_data(DB9_TIME_CHECK);
            StopTimer(DB9mileagetimer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_OUT:
            in_DB9_check = 0;
            notify.flag = 0;
            dev_DbusServer->sendNotify(&notify);
            I2c_write_DB9_7_check_data(DB9_OUT_CHECK);
            StopTimer(DB9statustimer);
            StopTimer(DB9mileagetimer);
            break;
    }

}

void* write_serial_data_thread(void *)
{
	int i=0,j=0;
	unsigned char redbuf[MAX_LEN];
	unsigned char msgbuf[MAX_LEN];
	unsigned char msgcountbuf[20];
	int datalength;
	int len=0,flag=0;
    vector<unsigned char> input, output;

	/* open comport */
	sp_fd = OpenComPort(4, 115200, 8,  "1", 'O');
	if(sp_fd < 0)
	{		
		printf("Can not open the serial port\n");		
		return 0;	
	}
	
	while(1)
	{
		if(cmd_flag)
		{
			pthread_mutex_lock(&sp_mutex);
			memcpy(redbuf, cmd_buf, cmd_len);
			len = cmd_len;
			cmd_flag = 0;
			flag = 1;			
			pthread_mutex_unlock(&sp_mutex);
		}
		
		if(flag)
		{
			flag = 0;
			
			int msgcount = parse_readbuf(redbuf,len,msgcountbuf);

			for(i = 0;i<msgcount;i++)
			{
				memset(msgbuf,'\0',sizeof(msgbuf));
				
				datalength = redbuf[msgcountbuf[i]+3]<<8 | redbuf[msgcountbuf[i] + 4];
				memcpy(msgbuf, redbuf+msgcountbuf[i], datalength + 7);

                input.clear();
                output.clear();
                for(j=0; j<datalength + 7; j++ )
                {
                    input.push_back(msgbuf[j]);
                }
                
                if((!in_DB9_check && (msgbuf[2] == ENUM_GB19056_CMD_CHECK_IN)) || in_DB9_check)
                {
                    process_DB9_check_command(msgbuf, datalength+6);
                }
                
                if(msgbuf[2]==ENUM_GB19056_CMD_QUERY_MILEAGE || msgbuf[2]==ENUM_GB19056_CMD_CHECK_MILEAGE || 
                    msgbuf[2]==ENUM_GB19056_CMD_QUERY_SPEED_RECORD || msgbuf[2]==ENUM_GB19056_CMD_QUERY_LOC_RECORD)
                {
                    pthread_mutex_lock (&engine_list_mutex_loc);
                    loc1_lock_flag = 1;
                }

                if(msgbuf[2]==ENUM_GB19056_CMD_QUERY_SPEED_RECORD || msgbuf[2]==ENUM_GB19056_CMD_QUERY_LOC_RECORD)
                {
                    not_process_loc1 = 1;
                }
           #if 0
                if(msgbuf[2]==0x1F)
                    data_up_pass_through_info(msgbuf);
                else
           #endif
               {
                GB19056Proxy<2012>::Instance()->ProcessRequest(input, output);

                if(loc1_lock_flag)
                {
                    pthread_mutex_unlock (&engine_list_mutex_loc);
                    loc1_lock_flag = 0;
                }

                if((output[2] == 0xFA) ||(output[2] == 0xFB))
                    datalength = 5;
                else
                    datalength = (output[3]<<8|output[4]) + 7;
                
                if(datalength <= 7 || (output[2] != 0x08 && output[2]!=0x09))
                {
                    not_process_loc1 = 0;
                }
                
                WriteComPort(sp_fd, &(output[0]), datalength);
                switch(output[2])
                {
                    case (unsigned char)ENUM_GB19056_CMD_SET_STATUS_CONFIG_2:
                        read_deploy_status_config_table();
                        break;
                    case (unsigned char)ENUM_GB19056_CMD_SET_PULSE_RATE:
                        read_deploy_para_table_set_pluse();
                        break;
                    case (unsigned char)ENUM_GB19056_CMD_SET_LAP_PULSE_FACTOR:
                        read_deploy_para_table_set_pluse();
                        break;
                }
              }
			}
			
		}
		else
			usleep(500);
	
	}
	
	if(sp_fd!=-1)
		CloseComPort(sp_fd);
    return 0;
}
void DB9_check_handler(string &name, void *data, void *data2)
{
    int value = (int)data;
    vector<unsigned char> input, output;
    input.clear();
    output.clear();

    if(value == 1)
    {
        DeviceDB9CheckDbusNotifyC notify;
        notify.flag = 0;
        dev_DbusServer->sendNotify(&notify);
        in_DB9_check = 0;
        I2c_write_DB9_7_check_data(DB9_OUT_CHECK);
        StopTimer(DB9mileagetimer);
        input.push_back(0xAA);
        input.push_back(0x75);
        input.push_back(0xE4);
        input.push_back(0x00);
        input.push_back(0x00);
        input.push_back(0x00);
        input.push_back(0x3B);
        
        GB19056Proxy<2012>::Instance()->ProcessRequest(input, output);
    }

    if(value == 2)
    {
        int len = 0;
        
        input.push_back(0xAA);
        input.push_back(0x75);
        input.push_back(0xE1);
        input.push_back(0x00);
        input.push_back(0x00);
        input.push_back(0x00);
        input.push_back(0x3E);
        
        pthread_mutex_lock (&engine_list_mutex_loc);
        GB19056Proxy<2012>::Instance()->ProcessRequest(input, output);
        pthread_mutex_unlock (&engine_list_mutex_loc);
        if((output[2] == 0xFA) ||(output[2] == 0xFB))
            len = 5;
        else
            len = (output[3]<<8|output[4]) + 7;
        WriteComPort(sp_fd, &(output[0]), len);
    }
}

void serial_protocol_init()
{
	pthread_t sp_read, sp_write;

    DB9statustimer = CreateTimer("DB9status", 6000, (void *)1, DB9_check_handler, false);
    DB9mileagetimer = CreateTimer("DB9mileage", 1000, (void *)2, DB9_check_handler, true);
	pthread_mutex_init(&sp_mutex, NULL);
	pthread_create(&sp_read, NULL,read_serial_data_thread, NULL);
	pthread_create(&sp_write, NULL,write_serial_data_thread, NULL);

    return;
}

void data_down_pass_through_info(unsigned char *msgbuf,int len)
{
    unsigned char buf[1024];
    
    memset(buf, 0, 1024);
    #if 0
    buf[0] = 0x55;
    buf[1] = 0x7A;
    buf[2] = 0x1F;
    buf[3] = (unsigned char)((len >> 8) & 0xFF);
    buf[4] = (unsigned char)(len & 0xFF);
    buf[5] = 0x00;
    memcpy(buf+6, msgbuf, len);
    buf[6+len] = check_sum(buf, 6+len);
    #endif
    memcpy(buf, msgbuf, len);

    
    for(int j=0; j<len;j++)
    {
        
        printf(" 0x%02x ",buf[j]);
    }
    printf("\n");  
    WriteComPort(sp_fd, buf,len);
}

