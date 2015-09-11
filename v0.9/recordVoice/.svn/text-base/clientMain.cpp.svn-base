#include "clientMain.h"
#include "record.h"
#include "wav_parser.h"

#ifdef VERSION_FOR_CHECK

#ifdef __cplusplus
extern "C" {
#endif
//#include "sp_enc.h"
#include "interf_enc.h"
#ifdef __cplusplus
}
#endif

#endif

/*Manage local storage space*/
void *rm_old_wavFile(void *arg) {
    int rows=0;
    signed int remain;
    int sample_rate, duration, RequiredSize, fileTime;
    string filePath;
    ThreadArg_t *args = (ThreadArg_t *)arg;

    VDRRecordTable rcdTable;
    if ((rows = rcdTable.GetRowCount()) >= 1) {
/*        printf("=============== rows=%d ===============\n", rows); */
        rcdTable.RefreshAll(VDR_REC_FILE_TIME);
        int iflag=0, index;
        /* Ensure that number of wavFiles saved in local and records number 
        in database is not greater than MAX_RECORD_TABLE_ROWS*/
        if (rows >= MAX_RECORD_TABLE_ROWS) {
            rcdTable.GetStringValue(iflag, VDR_REC_FILE_PATH, filePath);
            rcdTable.GetIntValue(iflag, VDR_REC_FILE_TIME, fileTime);
            remove(filePath.c_str());
            rcdTable.Delete(fileTime);
            iflag = 1;
/*            printf("=============== Records up to max: remove %s===============\n",filePath.c_str());*/
        }
        /*Ensure that local storage space is enough when recording be required.*/
        RequiredSize = 2 * (getSampleRate(args->arg_1)) * (args->arg_2);
        remain = (MAX_RECORD_STORAGE * 1024 * 1024) - RequiredSize;
        for (index=(rows-1); index >= iflag; index--) {
            rcdTable.GetIntValue(index, VDR_REC_SAMPLE_RATE, sample_rate);
	     rcdTable.GetIntValue(index, VDR_REC_RECORD_DURA, duration);
            remain -= (2 * sample_rate * duration);
            if (remain < 0) {
                rcdTable.GetStringValue(index, VDR_REC_FILE_PATH, filePath);
                rcdTable.GetIntValue(index, VDR_REC_FILE_TIME, fileTime);
                remove(filePath.c_str());
                rcdTable.Delete(fileTime);
/*                printf("=============== storage full: remove %s ===============\n", filePath.c_str());*/
            }
        }
        system("sync");
    }
    return((void *)0);
}

/*generate filename */
char* generate_filename(time_t ctime){
    
    struct tm* tm;
    time_t filetime = ctime;
	
#ifndef VERSION_FOR_CHECK
    char *suffix = (char*)".wav";
#else
	char *suffix = (char*)".amr";
#endif
	
    static char filename[32] = {0};

    tm = localtime(&filetime);
    tm->tm_year += 1900;//epoch
    tm->tm_mon += 1;//0~11
    
    sprintf(filename, "%04d%02d%02d%02d%02d%02d%s", tm->tm_year, tm->tm_mon, tm->tm_mday,tm->tm_hour, tm->tm_min, tm->tm_sec, suffix);
    
    return filename;
}
//send request when record is finish
void send_record_finishReq(int filetime){
    JT808RecordDbusReqC request;
    request.m_tableIndex = filetime;
    int result = pClient->sendService(&request);
    if(result < 0){
        printf("Send record finish requets Error. Error Code: %d\n", result);
    }
    return;
}
//fill RecordTableInfo Struct
int write_RecordTableInfo_Struct(int recordTimeLen, time_t filetime){

	#ifdef _8925_VERSION
	char *commonpath = (char*)"/nand2/innovdata/record/";
	#else
    char *commonpath = (char*)"/nand3/innovdata/record/";
	#endif
    char *oldfilename = (char*)"record.wav";
	char *oldamrname = (char*)"record.amr";

    char newfilename[32] = {0},
         oldfilepath[128] = {0},
         oldwavpath[128] = {0},
         newfilepath[128] = {0};
	
#ifdef VERSION_FOR_CHECK
    sprintf(oldfilepath,"%s%s", commonpath, oldamrname);
	sprintf(oldwavpath, "%s%s", commonpath, oldfilename);
#else
	sprintf(oldfilepath,"%s%s", commonpath, oldfilename);
#endif

    strcpy(newfilename, generate_filename(filetime));
    sprintf(newfilepath, "%s%s", commonpath, newfilename);
    rename(oldfilepath, newfilepath);/*rename*/

#ifdef 	VERSION_FOR_CHECK
	remove(oldwavpath);
#endif

    strcpy(recordTableInfo.filepath, newfilepath);
    recordTableInfo.filetime = filetime;
    recordTableInfo.filetype = WAV;
    recordTableInfo.record_duration = recordTimeLen;
    recordTableInfo.sample_rate = getSampleRate(m_sample);
#if 0
    printf("recordTableInfo.filepath:%s\n", recordTableInfo.filepath);
    printf("recordTableInfo.filetime:%d\n", recordTableInfo.filetime);
    printf("recordTableInfo.filetype:%d\n", recordTableInfo.filetype);
    printf("recordTableInfo.record_duration:%d\n", recordTableInfo.record_duration);
#endif
    return 0;
}
//write record table
void write_record_table(RecordTableInfo_t *recordTableInfo){

    int alarm_signal, status_flag, event = 0,
        longitude, latitude, altitude, 
        orientation, gps_speed, info_curr_time;

#if 0
    VDRLocStatus1Table *locStatus1table = new VDRLocStatus1Table();
    locStatus1table->RefreshBottom();
    locStatus1table->GetIntValue(VDR_LOCS_ALARM_SIGNAL, alarm_signal);
    locStatus1table->GetIntValue(VDR_LOCS_STATUS_FLAG, status_flag);
    locStatus1table->GetIntValue(VDR_LOCS_LONGITUDE, longitude);
    locStatus1table->GetIntValue(VDR_LOCS_LATITUDE, latitude);
    locStatus1table->GetIntValue(VDR_LOCS_ALTITUDE, altitude);
    locStatus1table->GetIntValue(VDR_LOCS_ORIENTATION, orientation);
    locStatus1table->GetIntValue(VDR_LOCS_GPS_SPEED, gps_speed);
    locStatus1table->GetIntValue(VDR_LOCS_OCCUR_TIME, info_curr_time);
    delete locStatus1table;
#endif
    FILE *fp = NULL;
    int num = 5;
    struct LocStatusData locbuf[5];
    
    memset(locbuf,0,sizeof(struct LocStatusData)*5);

    fp = fopen("/nand2/loc0809.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open loc0809.vdr %s\n", strerror(errno));
	}
    else
    {
        fread(locbuf,sizeof(struct LocStatusData),num,fp);
        fclose(fp);
    }
    alarm_signal = locbuf[num-1].alarm_signal;
    status_flag = locbuf[num-1].status_falg;
    longitude = locbuf[num-1].longitude;
    latitude = locbuf[num-1].latitude;
    altitude = locbuf[num-1].altitude;
    orientation = locbuf[num-1].orientation;
    gps_speed = locbuf[num-1].gps_speed;
    info_curr_time = locbuf[num-1].itime;

    printf("alarm = %d\n",alarm_signal);
    printf("status = %d\n",status_flag);
    printf("longitude = %d\n",longitude);
    printf("latitude = %d\n",latitude);
    printf("altitude = %d\n",altitude);
    printf("orientation = %d\n",orientation);
    printf("gps_speed = %d\n",gps_speed);
    printf("info_curr_time = %d\n",info_curr_time);

    VDRRecordTable *recordtable = new VDRRecordTable();
    recordtable->SetStringValue(VDR_REC_FILE_PATH,recordTableInfo->filepath);
    recordtable->SetIntValue(VDR_REC_FILE_TIME,recordTableInfo->filetime);
    recordtable->SetIntValue(VDR_REC_FILE_TYPE,recordTableInfo->filetype);
    recordtable->SetIntValue(VDR_REC_RECORD_DURA,recordTableInfo->record_duration);
    recordtable->SetIntValue(VDR_REC_SAMPLE_RATE, recordTableInfo->sample_rate);
    recordtable->SetIntValue(VDR_REC_EVENT, event);
    recordtable->SetIntValue(VDR_REC_ALARM_SIGNAL, alarm_signal);
    recordtable->SetIntValue(VDR_REC_STATUS_FLAG, status_flag);
    recordtable->SetIntValue(VDR_REC_LATITUDE, latitude);
    recordtable->SetIntValue(VDR_REC_LONGITUDE, longitude);
    recordtable->SetIntValue(VDR_REC_ALTITUDE, altitude);
    recordtable->SetIntValue(VDR_REC_ORIENTATION, orientation);
    recordtable->SetIntValue(VDR_REC_GPS_SPEED, gps_speed);
    recordtable->SetIntValue(VDR_REC_OCCUR_TIME, info_curr_time);
    recordtable->Commit();
    delete recordtable;
    
    return ;
}

void ClosePcmAndRecordFile(){
    extern snd_pcm_t *capture_handle;
    snd_pcm_drain(capture_handle);
    snd_pcm_close(capture_handle);
    close(wavFd);
}

//TimeRecording pcm read loops
int TimeLenRecordLoops(int timeLen){
    extern unsigned int period_time;
    return (int)(timeLen * 1000000 / period_time + 3);
}

//fixed time length record
int fixedTimeLenRecord(int timeLen){
    
    int rawDataBytes = 0, bytes_per_record = 0;
    extern WAVContainer_t wav_file_header;
    time_t filetime;
    ThreadArg_t thrd_arg;

    if(InitPcm(m_sample) < 0){
        printf("fail to init PCM!\n");
        return -1;
    }
	#ifdef _8925_VERSION
	if((wavFd = open("/nand2/innovdata/record/record.wav", O_WRONLY|O_CREAT|O_TRUNC,664)) < 0){
	#else
    if((wavFd = open("/nand3/innovdata/record/record.wav", O_WRONLY|O_CREAT|O_TRUNC,664)) < 0){
	#endif
        printf("Can't Create record file!\n");
        return -1;
    }

#ifdef VERSION_FOR_CHECK
	#ifdef _8925_VERSION
	amrfd = open("/nand2/innovdata/record/record.amr", O_WRONLY|O_CREAT|O_TRUNC,664);
	#else
	amrfd = open("/nand3/innovdata/record/record.amr", O_WRONLY|O_CREAT|O_TRUNC,664);
	#endif
	if(amrfd < 0)
	{
		printf("can not create arm file!\n");
		return -1;
	}
#endif		

    if (timeLen >= MAX_TIME_LENGTH)   
        timeLen = MAX_TIME_LENGTH;

    thrd_arg.arg_1=m_sample;
    thrd_arg.arg_2=timeLen;
    pthread_create(&pthread_storageControl, NULL, rm_old_wavFile, (void *)&thrd_arg);

    int loops = TimeLenRecordLoops(timeLen);
    if(loops < 0){
        printf("fail to get Recording loops\n");
        return -1;
    }

#ifndef VERSION_FOR_CHECK
    FillWavFileHeader(&wav_file_header, WAV_FILE_LENGTH, m_sample);
    if(WriteWavHeader(wavFd, &wav_file_header) < 0){
        printf("Fail to prepare write wav header to wav file\n");
        return -1;
    }
#endif

    printf("Start Recording......\n");
    while(loops > 0){
        if((bytes_per_record = Recording()) > 0){
            rawDataBytes += bytes_per_record;
            loops--;
        }        
    }

#ifdef VERSION_FOR_CHECK
	ClosePcmAndRecordFile();//close pcm and pcm file fd
	
   	/* requested mode */
   	enum Mode req_mode = MR122;
	int dtx = 0;
	/* input speech vector */
   	short speech[160];
	unsigned char serial_data[32];

    int byte_counter, frames_int = 0, bytes = 0;

	#define AMR_MAGIC_NUMBER "#!AMR\n"

   	bytes = write(amrfd, AMR_MAGIC_NUMBER, strlen(AMR_MAGIC_NUMBER));
    int *enstate = (int *)Encoder_Interface_init(dtx);

	extern snd_pcm_t *capture_handle;
	extern snd_pcm_uframes_t frames;

	int cnt = -1;
	#ifdef _8925_VERSION
	wavFd = open("/nand2/innovdata/record/record.wav", O_RDONLY);
	#else
	wavFd = open("/nand3/innovdata/record/record.wav", O_RDONLY);
	#endif

	lseek(wavFd, 0, SEEK_SET);
    /* read file */
	do{
       cnt = read(wavFd, speech, sizeof (Word16) * 160);

       frames_int ++;
 
       /* call encoder */
       byte_counter = Encoder_Interface_Encode(enstate, req_mode, speech, serial_data, 0);
 
       bytes += byte_counter;
       write(amrfd, serial_data, sizeof(UWord8)*byte_counter);
    }while(cnt > 0);
    Encoder_Interface_exit(enstate);
	close(wavFd);
	fflush(fdopen(amrfd, "w+"));
	

    time(&filetime);//get system time
    //ClosePcmAndRecordFile();//close pcm and pcm file fd
#else
	FillWavFileHeader(&wav_file_header, rawDataBytes, m_sample);
	lseek(wavFd, 0, SEEK_SET);
	 if(WriteWavHeader(wavFd, &wav_file_header) < 0){
        printf("Fail to write real wav header to wav file\n");
        return -1;
    }
    time(&filetime);//get system time
    ClosePcmAndRecordFile();//close pcm and pcm file fd
#endif	

    pthread_join(pthread_storageControl, NULL);   // wait for exit of thread storageControl.

    /*update DB*/
    write_RecordTableInfo_Struct(timeLen, filetime);
    write_record_table(&recordTableInfo);
    
    system("sync");

    if(m_flag == UPLOAD)
        send_record_finishReq(recordTableInfo.filetime);
    record_status = RECORD_STATUS_READY;

    printf("Stop Recording......\n");
    printf("Record complete timeLen = %d(s)\n", timeLen);
    return 0;
}

//cacl control record timeLen
int record_timeLen_calc(int readPcmCount){
    extern unsigned int period_time;
    return (int)(readPcmCount * period_time / 1000000 + 0.5);
}
//control record
int ControlRecord(){

    int rawDataBytes = 0, bytes_per_record = 0;
    extern WAVContainer_t wav_file_header;
    time_t filetime;
    ThreadArg_t thrd_arg;

    if(InitPcm(m_sample) < 0){
        printf("fail to init PCM!\n");
        return -1;
    }
	#ifdef _8925_VERSION
	if((wavFd = open("/nand2/innovdata/record/record.wav", O_WRONLY|O_CREAT|O_TRUNC, 664)) < 0){
	#else
    if((wavFd = open("/nand3/innovdata/record/record.wav", O_WRONLY|O_CREAT|O_TRUNC, 664)) < 0){
	#endif
        printf("Can't Create record file!\n");
        return -1;
    }
    int loops = 0, record_timeLen;
    int max_loops = TimeLenRecordLoops(MAX_TIME_LENGTH); //ADD: leihailong, 2012-11-08

    thrd_arg.arg_1 = m_sample;
    thrd_arg.arg_2 = MAX_TIME_LENGTH;
    pthread_create(&pthread_storageControl, NULL, rm_old_wavFile, (void *)&thrd_arg);

    record_status = RECORD_STATUS_START;
    FillWavFileHeader(&wav_file_header, WAV_FILE_LENGTH,m_sample);
    if(WriteWavHeader(wavFd, &wav_file_header) < 0){
        printf("Fail to prepare write wav header to wav file\n");
        return -1;
    }
    printf("Start Recording......\n");
    while(record_status == RECORD_STATUS_START){
        if((bytes_per_record = Recording()) > 0){
            rawDataBytes += bytes_per_record;
            loops++;
        }
        if (loops >= max_loops)  //ADD: leihailong, 2012-11-08
	    break;
    }
    record_timeLen = record_timeLen_calc(loops);

    FillWavFileHeader(&wav_file_header, rawDataBytes, m_sample);
    lseek(wavFd, 0, SEEK_SET);
    if(WriteWavHeader(wavFd, &wav_file_header) < 0){
        printf("Fail to write real wav header to wavfile\n");
        return -1;
    }
    time(&filetime);
    ClosePcmAndRecordFile();//close pcm and record file fd

    pthread_join(pthread_storageControl, NULL);   // wait for exiting of thread storageControl 
    
    /*update sqlite3 (Record Table)*/
    write_RecordTableInfo_Struct(record_timeLen, filetime);
    write_record_table(&recordTableInfo);
   
    system("sync");
    
    if(m_flag == UPLOAD)
        send_record_finishReq(recordTableInfo.filetime);
    record_status = RECORD_STATUS_READY;

    printf("Stop Recording......\n");
    printf("Record complete timeLen = %d(s)\n", record_timeLen);
    
    return 0;
}

void *pthread_control_record_handler(void *arg){

    while(1){
    
        pthread_mutex_lock(&control_record_mutex);
        pthread_cond_wait(&control_record_cond, &control_record_mutex);
        ControlRecord();
        pthread_mutex_unlock(&control_record_mutex);
    }
}

void RecordNotifyHandler(DBusNotifyC *notify){

    if(notify->getIdentifier() != JT808_DBUS_NTF_ID_RECORD)
        return;
    JT808RecordDbusNotifyC *recordNotify = (JT808RecordDbusNotifyC *)notify;
#if 0
    printf("m_command = %d\n", recordNotify->m_command);
    printf("m_falg = %d\n", recordNotify->m_flag);
    printf("m_time = %d\n", recordNotify->m_time);
    printf("m_sample = %d\n", recordNotify->m_sample);
#endif
    if(recordNotify->m_command == START_RECORD){//start record
        if(record_status == RECORD_STATUS_READY){
            m_sample = recordNotify->m_sample;
            m_flag = recordNotify->m_flag;
            if(recordNotify->m_time == 0){
                pthread_cond_signal(&control_record_cond);
            }else{
                fixedTimeLenRecord(recordNotify->m_time);//fixed timelength record
            }
        }else{
            printf("Can't Start Record: record status isn't READY\n");
        }
    }
    if(recordNotify->m_command == STOP_RECORD){//stop record
        if(record_status == RECORD_STATUS_START){
            m_sample = recordNotify->m_sample;
            record_status = RECORD_STATUS_STOP;
        }else{
            printf("Can't Stop Record: record status isn't Start\n");
        }
   }
   #ifdef _8925_VERSION
   system("amixer cset numid=8 23"); 
   #endif
    return ;
}

int main(int argc, char *argv[])
{
    DBusApplicationC app(argc, argv);
   
    pClient = new DBusClientC(JT808_DBUS_SERVER_ID, JT808_DBUS_SERVER_OBJ);

    pClient->registerNotify(JT808_DBUS_NTF_ID_RECORD, RecordNotifyHandler, JT808RecordDbusNotifyC::factory);
    if(pClient->connect() < 0){
        printf("DBusClient_Reocord Cann't connect to DBUS server. \n");
        return -1;
    }

    pthread_create(&pthread_control_record, NULL, pthread_control_record_handler, NULL);    
    pthread_detach(pthread_control_record);
    
    return app.loop();
}
