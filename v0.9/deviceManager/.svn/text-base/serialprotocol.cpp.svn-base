#include <pthread.h>
#include <boost/locale.hpp>

#include "serialprotocol.h"
#include "VDRDeployParaTable.h"
#include "VDRRunParaTable.h"
#include "VDRTrackInfoTable.h"
#include "ElectronicFenceTable.h"
#include "DriversLoginTable.h"

static int sp_fd = -1;
static unsigned char cmd_buf[MAX_LEN];
static int cmd_len = 0;
static int cmd_flag = 0;
static pthread_mutex_t sp_mutex;

unsigned char BCDToDec(unsigned char bcd_number)
{
	return((bcd_number>>4)*10 + (bcd_number&0x0f));
}

unsigned char DECToBCD(unsigned char dec_number)
{
	return(((dec_number/10)<<4) | (dec_number%10));
}


void get_sendmessage_head(unsigned char *sendbuf,unsigned char msgid,int msgdatalength)
{
	sendbuf[0] = 0x55;
	sendbuf[1] = 0x7A;
	sendbuf[2] = msgid & 0xFF;
	sendbuf[3] = (msgdatalength >> 8) & 0xFF;
	sendbuf[4] = msgdatalength & 0xFF;
	sendbuf[5] = 0x00;
	return;
}

int  check_sum(unsigned char *buf,int len)
{
	int i;
	unsigned char temp = 0;

	for(i=0; i<len; i++)
	{
		temp = temp ^ buf[i];
	}
	//printf("checksum=0x%02x len=%d\n",temp,len);
	return temp;
}

void sendmessage_error(unsigned char msgid)
{
 	unsigned char wrtbuf[5];

	memset(wrtbuf,'\0',sizeof(wrtbuf));
	wrtbuf[0] = 0x55;
	wrtbuf[1] = 0x7A;
	wrtbuf[2] = msgid;
	wrtbuf[3] = 0x00;
	wrtbuf[4] = check_sum(wrtbuf, 4);
	WriteComPort(sp_fd, wrtbuf, 5); 
}

void sendmessage_driver_infomation()
{
    int msg_len = 21;
    unsigned char wrtbuf[46];
    int drivercode;
    string driver_licsnum;
    int ret = -1;

    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x01,msg_len);

    DriversLoginTable *table = new DriversLoginTable();
    ret = table->RefreshBottom();
    if(ret < 0)
    {
        memset(wrtbuf+6, '0', 21);
    }
    else
    {
        table->GetStringValue(VDR_DRV_DRIVER_LICENSE_NUM, driver_licsnum);
        delete table;
        
        memcpy(wrtbuf+9, driver_licsnum.c_str(), driver_licsnum.length()); //18
    }

    wrtbuf[msg_len+6] = check_sum(wrtbuf, msg_len+6);

    WriteComPort(sp_fd, wrtbuf, msg_len + 7);

}

void sendmessage_real_time()
{
 	int msg_len = 6;
	unsigned char wrtbuf[14];
	time_t t; 
	struct tm *tm_t; 	

	time(&t); 
	tm_t=localtime(&t); 	
    
	memset(wrtbuf,'\0',sizeof(wrtbuf));
	get_sendmessage_head(wrtbuf,0x02,msg_len);

	wrtbuf[6] = DECToBCD(tm_t->tm_year-100);
	wrtbuf[7] = DECToBCD(tm_t->tm_mon + 1);
	wrtbuf[8] = DECToBCD(tm_t->tm_mday);
	wrtbuf[9] = DECToBCD(tm_t->tm_hour);
	wrtbuf[10] = DECToBCD(tm_t->tm_min);
	wrtbuf[11] = DECToBCD(tm_t->tm_sec);
	wrtbuf[msg_len+6] = check_sum(wrtbuf, msg_len+6);
	
 	WriteComPort(sp_fd, wrtbuf, msg_len+7);
}

//send 360h accumulative total trip mileage
void sendmessage_360h_total_mileage()
{
    int msg_len = 8;
    unsigned char wrtbuf[23];
    int mileage = 0, tmpmeter = 0, tmptime=0, index = 0;
    time_t seconds;
    struct tm *tm_t; 
    
    time(&seconds);
	tm_t=localtime(&seconds); 
    seconds = seconds - 360*60*60;

    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x03,msg_len);
    
    VDRLocStatus2Table *table = new VDRLocStatus2Table();
    index = table->RefreshBottom(); //table->RefreshNext((int)seconds-360*60*60);
    if(index < 0)
    {
        mileage = 0;
    }
    else
    {
        while(1)
        {
            table->GetIntValue(VDR_LOCS_OCCUR_TIME, tmptime);
            table->GetIntValue(VDR_LOCS_VDR_DISTANCE, tmpmeter);
            if(tmptime < (int)seconds)
                break;
            mileage += tmpmeter;
            index = table->RefreshPrevious(index);
            if(index < 0)
                break;
        }
        delete table;
    }

	if(mileage >= 0 && mileage <= 99)
	{
		wrtbuf[6] = 0x00;
		wrtbuf[7] = 0x00;
		wrtbuf[8] = DECToBCD(mileage);
	}
	else if(mileage > 99 && mileage <= 9999)
	{
		wrtbuf[6] = 0x00;
		wrtbuf[7] = DECToBCD(mileage / 100);
		wrtbuf[8] = DECToBCD(mileage % 100);
	}
	else if(mileage > 9999 && mileage <= 999999)
	{
		wrtbuf[6] = DECToBCD(mileage / 10000);
		wrtbuf[7] = DECToBCD((mileage / 100) % 100);
		wrtbuf[8] = DECToBCD(mileage % 100);						
	}

	wrtbuf[9] = DECToBCD(tm_t->tm_year-100);
	wrtbuf[10] = DECToBCD(tm_t->tm_mon + 1);
	wrtbuf[11] = DECToBCD(tm_t->tm_mday);
	wrtbuf[12] = DECToBCD(tm_t->tm_hour);
	wrtbuf[13] = DECToBCD(tm_t->tm_min);

	wrtbuf[msg_len+6] = check_sum(wrtbuf, msg_len+6);

	WriteComPort(sp_fd, wrtbuf, msg_len + 7);

}

void sendmessage_speedpulse_coefficient()
{
    int msg_len = 3;
    unsigned char wrtbuf[17];
    int spco;

    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf, 0x04, msg_len);

    VDRDeployParaTable *table = new VDRDeployParaTable();
    table->GetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, spco);
    delete table;

    wrtbuf[6] = (spco >> 16) & 0xFF;
    wrtbuf[7] = (spco >> 8) & 0xFF;
    wrtbuf[8] = spco & 0xFF;
    wrtbuf[msg_len+6] = check_sum(wrtbuf, msg_len+6);
    							
    WriteComPort(sp_fd, wrtbuf, msg_len + 7);
}

void sendmessage_vehicle_infomation()
{
    int msg_len = 41;
    unsigned char wrtbuf[48];
    string vincode,plateid, platesort;

    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x06,msg_len);

    VDRDeployParaTable *table = new VDRDeployParaTable();
    table->GetStringValue(VDR_DEPLOY_PARA_PLATE_ID, plateid);
    table->GetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, platesort);
    table->GetStringValue(VDR_DEPLOY_PARA_CAR_VIN, vincode);
    delete table;

    plateid = boost::locale::conv::from_utf<char>(plateid, "GBK");
    platesort = boost::locale::conv::from_utf<char>(platesort, "GBK");

    printf("vin=%s,id=%s,type=%s\n",vincode.c_str(),plateid.c_str(),platesort.c_str());    

    memcpy(wrtbuf + 6,vincode.c_str(),vincode.length());      //17
    memcpy(wrtbuf + 23,plateid.c_str(),plateid.length());     //12
    memcpy(wrtbuf + 35,platesort.c_str(),platesort.length()); //12
    wrtbuf[msg_len+6] = check_sum(wrtbuf, msg_len+6);

    WriteComPort(sp_fd, wrtbuf, msg_len + 7);
	
}

//send two date accumulative total trip mileage
void sendmessage_twodate_total_mileage()
{
    int msg_len = 8;
    unsigned char wrtbuf[23];
    int mileage = 0, tmpmeter = 0, tmptime=0, index = 0;
    struct tm *tm_t;
    time_t seconds;

    time(&seconds);  
    tm_t=localtime(&seconds); 	
    seconds -= 24*60*60-tm_t->tm_hour*60*60-tm_t->tm_min*60-tm_t->tm_sec;

    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x08,msg_len);
    
    VDRLocStatus2Table *table = new VDRLocStatus2Table();
    index = table->RefreshBottom(); //table->RefreshNext(seconds);
    if(index < 0)
    {
        mileage = 0;
    }
    else
    {
        while(1)
        {
            table->GetIntValue(VDR_LOCS_OCCUR_TIME, tmptime);
            table->GetIntValue(VDR_LOCS_VDR_DISTANCE, tmpmeter);
            if(tmptime < (int)seconds)
                break;
            mileage += tmpmeter;
            index = table->RefreshPrevious(index);
            if(index < 0)
                break;
        }
        delete table;
    }

    if(mileage >= 0 && mileage <= 99)
    {
    	wrtbuf[6] = 0x00;
    	wrtbuf[7] = 0x00;
    	wrtbuf[8] = DECToBCD(mileage);
    }
    else if(mileage > 99 && mileage <= 9999)
    {
    	wrtbuf[6] = 0x00;
    	wrtbuf[7] = DECToBCD(mileage / 100);
    	wrtbuf[8] = DECToBCD(mileage % 100);
    }
    else if(mileage > 9999 && mileage <= 999999)
    {
    	wrtbuf[6] = DECToBCD(mileage / 10000);
    	wrtbuf[7] = DECToBCD((mileage / 100) % 100);
    	wrtbuf[8] = DECToBCD(mileage % 100);						
    }

    wrtbuf[9] = DECToBCD(tm_t->tm_year-100);
    wrtbuf[10] = DECToBCD(tm_t->tm_mon + 1);
    wrtbuf[11] = DECToBCD(tm_t->tm_mday);
    wrtbuf[12] = DECToBCD(tm_t->tm_hour);
    wrtbuf[13] = DECToBCD(tm_t->tm_min);

    wrtbuf[msg_len+6] = check_sum(wrtbuf, msg_len+6);

    WriteComPort(sp_fd, wrtbuf, msg_len + 7);

}

int read_track_locstatus_table(int type, int timesec, unsigned char *buf)
{
    int tmpsec = 0, tmpspeed=0, index=0;
    int i,j, count, num=0;
    struct tm *tm_t; 
    time_t seconds;
    int *itime=NULL, *ispeed=NULL;
    
    VDRLocStatus2Table *table = new VDRLocStatus2Table();
    index = table->RefreshBottom(); //table->RefreshNext(itime);
    if(index < 0)
    {
        printf("table don't have data \n");
        return 0;
    }
    if(type == 1)
    {
        itime = (int *)malloc(21600);
        ispeed = (int *)malloc(21600);
    }
    else
    {
        itime = (int *)malloc(3000);
        ispeed = (int *)malloc(3000);
    }
    
    while(1)
    {
        table->GetIntValue(VDR_LOCS_OCCUR_TIME, itime[num]);
        table->GetIntValue(VDR_LOCS_VDR_SPEED, ispeed[num]);
        if(itime[num] < timesec)
            break;
        num++;
        index = table->RefreshPrevious(index);
        if(index < 0)
             break;
    }
    delete table;
    
    i = 0;
    j = 1;
    count = 1;
    while(1)
    {
        tmpsec = itime[num-1];
        tmpspeed = ispeed[num-1];
retry:    
        seconds = tmpsec;
        tm_t=localtime(&seconds);
        count = 1;
        
        buf[i*65 + 0] = DECToBCD(tm_t->tm_year-100);
        buf[i*65 + 1] = DECToBCD(tm_t->tm_mon + 1);
        buf[i*65 + 2] = DECToBCD(tm_t->tm_mday);
        buf[i*65 + 3] = DECToBCD(tm_t->tm_hour);
        buf[i*65 + 4] = DECToBCD(tm_t->tm_min);
        buf[i*65 + 5] = tmpspeed/10;

        num--;
        if(num == 0)
        {
            for(j=1; j<60; j++)
                buf[i*65 + 5 + j] = 0x00;
             break;
        }

last:          
        tmpsec = itime[num-1];
        tmpspeed = ispeed[num-1];
        
        if((tmpsec-seconds)> 59*60)
        {
            for(j=count; j<60; j++)
                buf[i*65 + 5 + j] = 0x00;
            goto retry;
        }
        else
        {
            for(j=count; j<(tmpsec-seconds)/60; j++)
                buf[i*65 + 5 + j] = 0x00;
            buf[i*65 + 5 + j] = tmpspeed/10;
            count = j+1;

            index = table->RefreshNext(index);
            if(index < 0)
            {
                for(j=count ; j<60; j++)
                    buf[i*65 + 5 + j] = 0x00;
                 break;
            }
            else
                goto last;
        }
        
        num--;
        if(num == 0)
            break;

        i++;
    }
    free(itime);
    free(ispeed);
    count = i*65;
    
    return count;
}

//Upload recent 2 days  vehicle speed data
void sendmessage_twodate_speeddata()
{
    unsigned char wrtbuf[MAX_LEN*4];
    struct tm *tm_t; 
    int len = 0;
	time_t seconds;
    
    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x09, 65);

    time(&seconds);
    tm_t=localtime(&seconds); 	
    seconds -= 24*60*60-tm_t->tm_hour*60*60-tm_t->tm_min*60-tm_t->tm_sec;

    len = read_track_locstatus_table(2,seconds, wrtbuf+6);
    
    wrtbuf[3] = (len >> 8) & 0xFF;
    wrtbuf[4] = len & 0xFF;

    wrtbuf[len+6] = check_sum(wrtbuf, len+6);
    WriteComPort(sp_fd, wrtbuf, len+7);
}

//Upload recently 360 h  vehicle speed data
void sendmessage_360h_speeddata()
{
    unsigned char wrtbuf[MAX_LEN*20];
    int seconds=0,  len = 0;
	
    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x05, 65);

    seconds= time((time_t*)NULL);
    seconds -= 360*60*60;

    len = read_track_locstatus_table(1,seconds, wrtbuf+6);
    
    wrtbuf[3] = (len >> 8) & 0xFF;
    wrtbuf[4] = len & 0xFF;

    wrtbuf[len+6] = check_sum(wrtbuf, len+6);
    WriteComPort(sp_fd, wrtbuf, len+7);
}

void sendmessage_accident_points()
{
    unsigned char wrtbuf[MAX_LEN*3];
    vector<unsigned char>  speed,status;    
    int i = 0, count = 0, ret=0, tmpsec=0;
    time_t seconds; 
    struct tm *tm_t; 

    time(&seconds); 
    tm_t=localtime(&seconds); 	

    memset(wrtbuf,'\0',sizeof(wrtbuf));
    get_sendmessage_head(wrtbuf,0x07,100*2+6);

    VDRAccidentTable *table = new VDRAccidentTable();
    ret = table->RefreshTop();
    if(ret < 0)
    {
        wrtbuf[3] = 0x00;
        wrtbuf[4] = 0x00;
        wrtbuf[6] = check_sum(wrtbuf, 6);
        WriteComPort(sp_fd, wrtbuf, 7);
        return ;
    }
    while(1)
    {
        speed.clear();
        status.clear();
        table->GetIntValue(VDR_RUNS_OCCUR_TIME, tmpsec);
        table->GetSequenceValue(VDR_RUNS_SPEED, speed);
        table->GetSequenceValue(VDR_RUNS_STATUS, status);
        seconds = tmpsec;
        tm_t=localtime(&seconds);  
        wrtbuf[6+count*206+0] = DECToBCD(tm_t->tm_year-100);
        wrtbuf[6+count*206+1] = DECToBCD(tm_t->tm_mon + 1);
        wrtbuf[6+count*206+2] = DECToBCD(tm_t->tm_mday);
        wrtbuf[6+count*206+3] = DECToBCD(tm_t->tm_hour);
        wrtbuf[6+count*206+4] = DECToBCD(tm_t->tm_min);
        wrtbuf[6+count*206+5] = DECToBCD(tm_t->tm_sec);
        for(i = 0; i<100; i++)
        {
            wrtbuf[12 + count*206 + i*2 + 0] = speed[i];
            wrtbuf[12 + count*206 + i*2 + 1] = status[i];
        } 
        
        count++;
        ret = table->RefreshNext();
        if(ret < 0)
            break ;
    }
    delete table;

    wrtbuf[3] = (count*206 >> 8) & 0xFF;
    wrtbuf[4] = count*206 & 0xFF;
    wrtbuf[count*206+6] = check_sum(wrtbuf, count*206+6);
    WriteComPort(sp_fd, wrtbuf, count*206+7);
}

void sendmessage_fatigue_driving()
{
	unsigned char wrtbuf[MAX_LEN];
	int i, ret, ibegin, iend;
	time_t seconds ;
    struct tm *tm_t; 
    string driver_licsnum;

    time(&seconds);
    tm_t=localtime(&seconds); 	
    seconds -= 24*60*60-tm_t->tm_hour*60*60-tm_t->tm_min*60-tm_t->tm_sec;

	memset(wrtbuf,'\0',sizeof(wrtbuf));
	get_sendmessage_head(wrtbuf,0x11, 28);

    VDRTiredTable *table = new VDRTiredTable();
    ret = table->RefreshNext(seconds);
    if(ret < 0)
    {
        wrtbuf[3] = 0x00;
        wrtbuf[4] = 0x00;
        wrtbuf[6] = check_sum(wrtbuf, 6);
        WriteComPort(sp_fd, wrtbuf, 7);
        return ;
    }
    i = 0;
    while(1)
    {
        table->GetIntValue(VDR_TIRED_BEGIN_TIME, ibegin);
        table->GetIntValue(VDR_TIRED_END_TIME, iend);
        table->GetStringValue(VDR_TIRED_DRIVER_LICENSE_NUM, driver_licsnum);
        seconds = ibegin;
        tm_t=localtime(&seconds); 
        
		memcpy(wrtbuf+6+28*i, driver_licsnum.c_str(), 18);
		wrtbuf[24+i*28 + 0] = DECToBCD(tm_t->tm_year-100);			
		wrtbuf[24+i*28 + 1] = DECToBCD(tm_t->tm_mon + 1);
		wrtbuf[24+i*28 + 2] = DECToBCD(tm_t->tm_mday);
		wrtbuf[24+i*28 + 3] = DECToBCD(tm_t->tm_hour);			
		wrtbuf[24+i*28 + 4] = DECToBCD(tm_t->tm_min);
        seconds = ibegin;
		tm_t=localtime(&seconds); 
		wrtbuf[24+i*28 + 5] = DECToBCD(tm_t->tm_year-100);			
		wrtbuf[24+i*28 + 6] = DECToBCD(tm_t->tm_mon + 1);
		wrtbuf[24+i*28 + 7] = DECToBCD(tm_t->tm_mday);
		wrtbuf[24+i*28 + 8] = DECToBCD(tm_t->tm_hour);			
		wrtbuf[24+i*28 + 9] = DECToBCD(tm_t->tm_min);
        

        ret = table->RefreshNext();
        if(ret < 0)
            break;  
        i++;
    }
    delete table;

    wrtbuf[3] = (i*28 >> 8) & 0xFF;
    wrtbuf[4] = i*28 & 0xFF;

	wrtbuf[i*28+6] = check_sum(wrtbuf, i*28+6);
	WriteComPort(sp_fd, wrtbuf, i*28+7);

}

void sendmessage_setup_response(char msgid)
{
	int wrt = 0;
	unsigned char wrtbuf[7];

	memset(wrtbuf,'\0',sizeof(wrtbuf));
	get_sendmessage_head(wrtbuf,msgid,0);
	wrtbuf[6] = check_sum(wrtbuf, 6);
	wrt = WriteComPort(sp_fd, wrtbuf, 7);
}
void setup_driver_infomation(unsigned char *parsebuf)
{
    char tmpbuf[20];
	int driverid=0;
	string driverlicsnum;
    
    memset(tmpbuf,0, sizeof(tmpbuf));
	driverid = (parsebuf[0]<<16)|(parsebuf[1]<<8)|parsebuf[2];
    memcpy(tmpbuf,parsebuf+3, 18);
    driverlicsnum = string(tmpbuf);
	printf("driverid=%d,driverlicsnum=%s,\n",driverid,driverlicsnum.c_str());
}

void setup_vehicle_infomation(const unsigned char *parsebuf)
{
    char tmpbuf[20];
    string vincode,plateid, platesort;

    memset(tmpbuf,0, sizeof(tmpbuf));
    memcpy(tmpbuf,parsebuf, 17);
    vincode = string(tmpbuf);
    memcpy(tmpbuf,parsebuf+17, 12);
    plateid = string(tmpbuf);
    memcpy(tmpbuf,parsebuf+29,12);
    platesort = string(tmpbuf);
    
    printf("vin=%s,id=%s,type=%s\n",vincode.c_str(),plateid.c_str(),platesort.c_str());
    
    plateid = boost::locale::conv::to_utf<char>(plateid, "GBK");
    platesort = boost::locale::conv::to_utf<char>(platesort, "GBK");
    
    VDRDeployParaTable *table = new VDRDeployParaTable();
    table->SetStringValue(VDR_DEPLOY_PARA_PLATE_ID, plateid);
    table->SetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, platesort);
    table->SetStringValue(VDR_DEPLOY_PARA_CAR_VIN, vincode);
    table->Commit();
    delete table;
}

void setup_speedpulse_coefficient(const unsigned char *parsebuf)
{
    int spco ;
    spco = (parsebuf[0] << 16)|(parsebuf[1] << 8)| parsebuf[2];

    VDRDeployParaTable *table = new VDRDeployParaTable();
    table->SetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, spco);    
    table->Commit();
    delete table;
}

void setup_real_time(const unsigned char *parsebuf)
{
	char cmd[50];
	
	int iyear = BCDToDec(parsebuf[0]);
	int imonth = BCDToDec(parsebuf[1]);
	int idate = BCDToDec(parsebuf[2]);
	int ihour = BCDToDec(parsebuf[3]);
	int imin = BCDToDec(parsebuf[4]);
    
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "date -s %02d%02d%02d%02d20%02d", imonth,idate,ihour,imin,iyear);
	system(cmd);
}

int parse_readbuf(unsigned char *readbuf,int readnum,unsigned char *msgcountbuf)
{
	int msgcount = 0;
	int i=0,j=0;
	unsigned char temp[MAX_LEN/5];
	int datalength;
	int readmsg_length;

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
			readmsg_length = datalength + 7;
			if((temp[i] + readmsg_length - 1) <= readnum)
			{
				if(readbuf[temp[i]+readmsg_length-1] == check_sum(readbuf+temp[i], readmsg_length-1))
				{
					msgcountbuf[msgcount] = temp[i];
					msgcount++;
				}
				else
				{
					switch(readbuf[temp[i] + 2])
					{
						case ACQUISITION_VERSION_NUMBER:
						case ACQUISITION_DRIVER_INFOMATION:
						case ACQUISITION_REAL_TIME:
						case ACQUISITION_360H_TOTAL_MILEAGE:
						case ACQUISITION_SPEEDPULSE_COEFFICIENT:
						case ACQUISITION_VEHICLE_INFOMATION:
						case ACQUISITION_TWODAY_TOTAL_MILEAGE:
						case ACQUISITION_360H_SPEEDDATA:
						case ACQUISITION_TWODAY_SPEEDDATA:
						case ACQUISITION_ACCIDENT_POINTS:
						case ACQUISITION_FATIGUE_DRIVING:
								sendmessage_error(UPLOAD_RESPONSE_FRAME_SIGN);
							break;
						case SETUP_VEHICLE_INFOMATION:
						case SETUP_REAL_TIME:
						case SETUP_SPEEDPULSE_COEFFICIENT:
								sendmessage_error(DOWNLOAD_RESPONSE_FRAME_SIGN);
							break;
						default:	
							break;
					}
				}
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

	read_fd = OpenComPort(1, 9600, 8,  "1", 'O');
	if(read_fd < 0)
	{		
		printf("Can not open the serial port\n");		
		return 0;	
	}
    printf("--OpenComPort--ok-\n");

	memset(rev_buf, 0, sizeof(rev_buf));

	while(1)
	{
	    printf("---------read_serial_data_thread-------------\n");
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

			for(i=0; i<rev_len; i++)
			{
				if(rev_buf[i] == 0xAA && rev_buf[i+1] == 0x75)
				{
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
						rev_buf[j] = rev_buf[i+7+data_len];
					}
					
				}
			}
		}	
	}
	if(read_fd)
		CloseComPort(read_fd);
	
}

void* write_serial_data_thread(void *)
{
	int i;
	unsigned char redbuf[MAX_LEN];
	unsigned char parsebuf[MAX_LEN];
	unsigned char msgbuf[MAX_LEN*2];
	unsigned char msgcountbuf[MAX_LEN/5];
	int datalength;
	int len=0,flag=0;

	/* open comport */
	sp_fd = OpenComPort(1, 9600, 8,  "1", 'O');
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
	#if 0
		printf("....................\n");
			for(int t=0; t<cmd_len;t++)
			{
				printf("0x%02x ",redbuf[t]);
			}
			printf("\n");				
	#endif
			
			pthread_mutex_unlock(&sp_mutex);
		}
		
		if(flag)
		{
			flag = 0;
			
			int msgcount = parse_readbuf(redbuf,len,msgcountbuf);

			for(i = 0;i<msgcount;i++)
			{
				memset(msgbuf,'\0',sizeof(msgbuf));
				memset(parsebuf,'\0',sizeof(parsebuf));
				
				datalength = redbuf[msgcountbuf[i]+3]<<8 | redbuf[msgcountbuf[i] + 4];
				memcpy(msgbuf, redbuf+msgcountbuf[i], datalength + 7);
			#if 0
				for(int t=0;t<datalength+7;t++)
					printf(" 0x%02x ",msgbuf[t]);
				printf("msgbuf[2]=0x%02x\n",msgbuf[2]);
			#endif
				
				switch(msgbuf[2])
				{
					case ACQUISITION_DRIVER_INFOMATION:
						sendmessage_driver_infomation();
						break;
					case ACQUISITION_REAL_TIME:
						sendmessage_real_time();
						break;
					case ACQUISITION_360H_TOTAL_MILEAGE:
						sendmessage_360h_total_mileage();
						break;
					case ACQUISITION_SPEEDPULSE_COEFFICIENT:
						sendmessage_speedpulse_coefficient();
						break;
					case ACQUISITION_VEHICLE_INFOMATION:
						sendmessage_vehicle_infomation();
						break;
					case ACQUISITION_TWODAY_TOTAL_MILEAGE:
						sendmessage_twodate_total_mileage();
						break;
					case ACQUISITION_360H_SPEEDDATA:
						sendmessage_360h_speeddata();
						break;
					case ACQUISITION_TWODAY_SPEEDDATA:
						sendmessage_twodate_speeddata();
						break;
					case ACQUISITION_ACCIDENT_POINTS:
						sendmessage_accident_points();
						break;
					case ACQUISITION_FATIGUE_DRIVING:
						sendmessage_fatigue_driving();
						break;
					case SETUP_DRIVER_INFOMATION:
						memcpy(parsebuf,msgbuf + 6,datalength);
						setup_driver_infomation(parsebuf);
						sendmessage_setup_response(SETUP_DRIVER_INFOMATION);
						break;
					case SETUP_VEHICLE_INFOMATION:
						memcpy(parsebuf,msgbuf + 6,datalength);
						setup_vehicle_infomation(parsebuf);
						sendmessage_setup_response(SETUP_VEHICLE_INFOMATION);
						break;
					case SETUP_REAL_TIME:
						memcpy(parsebuf,msgbuf + 6,datalength);
						setup_real_time(parsebuf);
						sendmessage_setup_response(SETUP_REAL_TIME);
						break;
					case SETUP_SPEEDPULSE_COEFFICIENT:
						memcpy(parsebuf,msgbuf + 6,datalength);
						setup_speedpulse_coefficient(parsebuf);
						sendmessage_setup_response(SETUP_SPEEDPULSE_COEFFICIENT);
						break;
					default:	
						break;
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

void serial_protocol_init()
{
	pthread_t sp_read, sp_write;

	pthread_mutex_init(&sp_mutex, NULL);
	pthread_create(&sp_read, NULL,read_serial_data_thread, NULL);
	pthread_create(&sp_write, NULL,write_serial_data_thread, NULL);

    return;
}
