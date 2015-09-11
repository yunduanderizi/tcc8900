#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "i2ccontrol.h"
#include "dbusApplication.h"
#include "dbusClient.h"
#include "SchoolID.h"
#include "dbusServer.h"
#include "VDRStudentIdTable.h"

//#define CLINET_IN_SERVER



#ifdef CLINET_IN_SERVER
DBusClientC *id_Device;
#endif

DBusServerC *id_DbusServer;

#define I2C_READ_DATA_USAR	0x1E
#define READ_MCU_SPEED_ADDR     0x1B
#define MCUADDR 0x54
#define IC_ID_LEN	10

using namespace std;

static int index_cnt = 0;

int I2c_read_school_card(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
    ret = i2cHander.readBlock(MCUADDR, I2C_READ_DATA_USAR, buf, len);
	if(ret < 0)
	{
		printf("mcu I2c read byte err: %d\n", ret);
		return -1;
	}

	return ret;
}

#ifdef CLINET_IN_SERVER
void IdDbusNotifyHandler(DBusNotifyC *notify)
{
	SchoolIDDbusNotifyC *school_notify = (SchoolIDDbusNotifyC *)notify;

	printf("count_all:%d\n", school_notify->count_all);
	printf("id_num:%s\n", school_notify->id_num); 
	printf("id_flag:%d\n", school_notify->flag);
}
#endif

string cstr_to_string(char *p_str) 
{ 
        return p_str; 
}
static inline char* only_daytime(time_t _daytime, char *buf)
{
	if(NULL == buf)
		return buf;
	
	struct tm *daytime = localtime(&_daytime);
	int moring = (daytime->tm_hour >= 0 && daytime->tm_hour < 12)?0:1;
	sprintf(buf, "%4d%02d%02d%d", daytime->tm_year + 1900, daytime->tm_mon + 1, daytime->tm_mday, moring);

	return buf;
}	

void *recoder_thread(void *)
{
	unsigned char temp[128] = {0};

	SchoolIDDbusNotifyC notify;

	while(1)
	{
		unsigned char id[128] = {0};
		I2c_read_school_card(id, IC_ID_LEN);		
		time_t read_time = time(NULL);
	
		if(('\0' == id[0]) || ('\0' == id[IC_ID_LEN - 1]) || !memcmp(temp, id, sizeof(id)))
		{
#if 0	
			if(0x30 == id[0])
			{
				int i = 0;
				printf("idnum:");
				for(i = 0; i < 10; i++)
				{
					printf("%x ", id[i]);
				}
				printf("\n");
			}
#endif			
			usleep(300*1000);
			continue;
		}

		printf("id:%s\n", id);

		char str_read_time[128] = {0};
		only_daytime(read_time, str_read_time);
		printf("str_read_time:%s\n", str_read_time);
		int int_read_time = atoi(str_read_time); 
		printf("int_read_time:%d\n", int_read_time);
	
		int recoder_time = 0;				
		VDRStudentIdUpTable *id_table = new VDRStudentIdUpTable();
		id_table->UpRefreshByEqualToStr(VDR_STUDENT_ID_NUMBER, cstr_to_string((char *)id));
		int index = id_table->GetResultsCount();
		printf("count:%d\n", index);

		char str_recoder_time[128] = {0};
		int i = 0;
		for(i = index - 1; i >= 0; i--)
		{
			id_table->GetIntValue(i, VDR_STUDENT_ID_READ_TIME, recoder_time);
			//printf("read_time:%s\n", read_time);			
			only_daytime(recoder_time, str_recoder_time);			
			printf("recoder_time:%s\n", str_recoder_time);
			if(!strcmp(str_read_time, str_recoder_time))
			{
				break;
			}
		}
		delete id_table;
		
		printf("str_recoder_time:%s\n", str_recoder_time);

		if(NULL == str_read_time)
			continue;
		
		if(!recoder_time || strcmp(str_read_time, str_recoder_time))
		{
			printf("read_time:%d\n", recoder_time);
			
			VDRStudentIdUpTable *id_table = new VDRStudentIdUpTable();
			int  ret = id_table->RefreshBottom();
			if(ret == 0)
			{
				id_table->GetIntValue(VDR_STUDENT_ID_INDEX, index_cnt);
				index_cnt++;
			}
		
			id_table->SetStringValue(VDR_STUDENT_ID_NUMBER, cstr_to_string((char *)id));
			id_table->SetIntValue(VDR_STUDENT_ID_READ_TIME,(int)read_time); 		
			id_table->SetIntValue(VDR_STUDENT_ID_INDEX, index_cnt);
			id_table->Commit();
			delete id_table;


			int cnt = 0;
			int tmp_recoder_time;
			VDRIdCountTable *old_cnt_table = new VDRIdCountTable();
			if(!old_cnt_table->Refresh(int_read_time) || !old_cnt_table->RefreshBottom())
			{
				old_cnt_table->GetIntValue(VDR_STUDENT_ID_DAYTIME, tmp_recoder_time);
				printf("refresh!\n");
			}

			if(int_read_time == tmp_recoder_time)
			{
				old_cnt_table->GetIntValue(VDR_STUDENT_ID_COUNT, cnt);
			}
			printf("tmp_recoder_time:%d\n", tmp_recoder_time);
			printf("cnt:%d\n", cnt);
			delete old_cnt_table;
			
			cnt++;			
			VDRIdCountTable *new_cnt_table = new VDRIdCountTable();
			printf("str_read_time:%s\n", str_read_time);
			new_cnt_table->SetIntValue(VDR_STUDENT_ID_DAYTIME, int_read_time);
			new_cnt_table->SetIntValue(VDR_STUDENT_ID_COUNT, cnt);
			new_cnt_table->Commit();
			delete new_cnt_table;
					
			memcpy(notify.id_num, id, sizeof(id));
			notify.count_all = cnt;
			notify.flag_done = 0;
			notify.flag_ampm = int_read_time;
			id_DbusServer->sendNotify(&notify);
		}
		else
		{
			printf("come in already!\n");
			notify.flag_done = 1;
			id_DbusServer->sendNotify(&notify);
		}
		
		//usleep(50*1000);
	}

	return NULL;
}

int student_id_recoder()
{
	int ret = -1;

	pthread_t id_threads;	
	ret = pthread_create(&id_threads, NULL, recoder_thread, NULL);
	if(ret < 0)
	{
		printf("create handle_image error!\n");
		return ret;
	}
	pthread_detach(id_threads);

	return ret;

}


int main(int argc, char **argv)
{
	DBusApplicationC app(argc, argv);


	id_DbusServer = new DBusServerC(SCHOOL_ID_DBUS_SERVER_ID, SCHOOL_ID_DBUS_SERVER_OBJ);	
	id_DbusServer->registerService(SCHOOL_ID_REQ_ID_REVERSE, NULL, SchoolIDDbusReqC::factory);
	if(id_DbusServer->connect() < 0)
    {
        printf("Cann't connect to device manager id_DbusServer server.\n");
        return -1;
    }

#ifdef CLINET_IN_SERVER
	id_Device = new DBusClientC(SCHOOL_ID_DBUS_SERVER_ID, SCHOOL_ID_DBUS_SERVER_OBJ);
	id_Device->registerReply(SCHOOL_ID_REP_ID_REVERSE, SchoolIDDbusRepC::factory);
	id_Device->registerNotify(SCHOOL_ID_NOTF_ID_REVERSE,IdDbusNotifyHandler,SchoolIDDbusNotifyC::factory);

	if(id_Device->connect()<0)
    {
        printf("Cann't connect to id_DbusServer DBUS server.\n");
        return -1;
    }

#endif

	student_id_recoder();

	return app.loop();
}
