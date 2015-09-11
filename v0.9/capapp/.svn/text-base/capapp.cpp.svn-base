#include "capapp.h"

DBusServerC *cap_DbusServer;
DBusClientC *ca_ClientJT808;
DBusClientC *re_Device;

int resolution[][2] = {
	{320, 240},
	{640, 480},
	{800, 600},
	{1024,768},
	{176, 144},
	{352, 288},
	{704, 288},
	{704, 576}
};

unsigned char chan[] = {0x40, 0x4C, 0x48, 0x44}; 

static struct cap_obj *capobj;
extern CameraDevice dev;
static int handle_image_exit = 0;
static int dbus_no = 0;
static int in_reverse = 0;//1: in reverse  0:out reverse

void *dev_mem;
int fd_mem;

#define irq_ts_enable() (*(int *)dev_mem |= (1 << 5)) 
#define irq_ts_disable() (*(int *)dev_mem &= ~(1 << 5))

int I2c_read_video_type(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;
	len++;

	if (i2cHander.checkOK() == 0)
	{
		bq_printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(SENSER_I2C_ADDRESS, SENSER_I2C_VIDETYPE, value);
    
	return ret;
}

int i2c_write_video_channle(unsigned char value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		bq_printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeByte(SENSER_I2C_ADDRESS, SENSER_I2C_CHANNEL, value);
    
	return ret;
}

int video_type_change()
{
	unsigned char video_type_value = -1;
	unsigned char video_type_mode = -1;
	int change_flag = 0;
	
	int i = 10;
	while(i--)
	{
		I2c_read_video_type(video_type_value, 1);
		bq_printf("vodeo_type_value:%d\n", video_type_value);
		if(PAL_IN_VALUE == video_type_value || NTSC_IN_VALUE == video_type_value)
			break;
		usleep(500);
	}

	video_type_mode = (NTSC_IN_VALUE == video_type_value)?(NTSC_IN_MODE):(PAL_IN_MODE);
	bq_printf("video_type_mode:%d\n", video_type_mode);

	if(dev.video_mode !=  video_type_mode)
	{
		change_flag = 1;
		dev.video_mode = video_type_mode;
	}

	return change_flag;
}

int video_type_init()
{
	unsigned char videtype = -1;
	int i = 7;
	
	while(i--)
	{
		I2c_read_video_type(videtype, 1);
		usleep(100);
	}
	bq_printf("videotype:%x\n", videtype);
	
	if(NTSC_IN_VALUE == videtype)
	{
		dev.preview_height = PREVIEW_HEIGHT_N;
		dev.video_mode = 0;
	}
	else
	{
		dev.preview_height = PREVIEW_HEIGHT;
		dev.video_mode = 1;
	}

	dev.video_type_change = video_type_change;
	return 0;
}
  
string cstr_to_string(char *p_str) 
{ 
        return p_str; 
}

void *handle_image(void *param)
{
	 CameraDevice *dev = (CameraDevice *)param;
	 int width = -1;
	 int height = -1;

	 struct pic_info picinfo;

	 while(!handle_image_exit)
	 {
		 if(queue_empty(&dev->Q) == 0)
		 {
			usleep(50*100);
			continue;
		 }

	     char buffer_src[1024] = {0};
	 	 char buffer_det[1024] = {0};
		 memset(&picinfo, 0, sizeof(struct pic_info));
		 de_queue(&dev->Q, (char *)&picinfo);
		 sprintf(buffer_src, "%s_src.jpg", picinfo.name);
		 sprintf(buffer_det, "%s.jpg", picinfo.name);

		 bq_printf("handle resolution:%d\n", picinfo.m_resolution);
		 if(picinfo.m_resolution < 1 || picinfo.m_resolution > 8)
		 	picinfo.m_resolution = 1;

		 width = resolution[picinfo.m_resolution-1][0];
		 height = resolution[picinfo.m_resolution-1][1];
		 
		 int alarm_signal 	= 0;
		 int status_flag 	= 0;
		 int latitude 		= 0;
		 int longitude 		= 0;
		 int altitude 		= 0;
		 int orientation 	= 0;
		 int gps_speed 		= 0;
		 int occur_time 	= 0;

        #if 0
		 VDRLocStatus1Table *loacl_table = new VDRLocStatus1Table();
    	 loacl_table->RefreshBottom();
    	 loacl_table->GetIntValue(VDR_LOCS_ALARM_SIGNAL, alarm_signal);
    	 loacl_table->GetIntValue(VDR_LOCS_STATUS_FLAG, status_flag);
    	 loacl_table->GetIntValue(VDR_LOCS_LATITUDE, latitude);
    	 loacl_table->GetIntValue(VDR_LOCS_LONGITUDE, longitude);
		 loacl_table->GetIntValue(VDR_LOCS_ALTITUDE, altitude);
    	 loacl_table->GetIntValue(VDR_LOCS_ORIENTATION, orientation);
		 loacl_table->GetIntValue(VDR_LOCS_GPS_SPEED, gps_speed);
		 loacl_table->GetIntValue(VDR_LOCS_OCCUR_TIME, occur_time);
    	 delete loacl_table;
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
        occur_time = locbuf[num-1].itime;

		 VDRCaptureTable *table = new VDRCaptureTable();
		 string filepath;
		 if(table->GetRowCount() >= 100)
		 {
			table->RefreshTop();	
			table->GetStringValue(VDR_CAP_FILE_PATH, filepath);
			remove(filepath.c_str());
			sync();
		 }

#if 0
		 scale_jpeg(buffer_src, buffer_det, width, height);
		 remove(buffer_src);
		 unlink(buffer_src);
		 sync();
#endif
		 
	 	 table->SetStringValue(VDR_CAP_FILE_PATH, cstr_to_string(buffer_det));
	     table->SetIntValue(VDR_CAP_FILE_TIME, picinfo.m_captime);
	     table->SetIntValue(VDR_CAP_FILE_TYPE, 0);
		 table->SetIntValue(VDR_CAP_FILE_QUALITY, picinfo.m_quality);
		 table->SetIntValue(VDR_CAP_RESOLUTION, picinfo.m_resolution);
		 table->SetIntValue(VDR_CAP_BRIGHTNESS, picinfo.m_brightness);
		 table->SetIntValue(VDR_CAP_CONTRAST, 1);
		 table->SetIntValue(VDR_CAP_SATUATION, picinfo.m_saturation);
		 table->SetIntValue(VDR_CAP_CHROMA, picinfo.m_chroma);
		 table->SetIntValue(VDR_CAP_EVENT, picinfo.m_event);
		 table->SetIntValue(VDR_CAP_CHANNEL_ID, capobj->chancur_status);
		 table->SetIntValue(VDR_CAP_ALARM_SIGNAL, alarm_signal);
		 table->SetIntValue(VDR_CAP_STATUS_FLAG, status_flag);
		 table->SetIntValue(VDR_CAP_LATITUDE, latitude);
		 table->SetIntValue(VDR_CAP_LONGITUDE, longitude);
		 table->SetIntValue(VDR_CAP_ALTITUDE, altitude);
		 table->SetIntValue(VDR_CAP_ORIENTATION, orientation);
		 table->SetIntValue(VDR_CAP_GPS_SPEED, gps_speed);
		 table->SetIntValue(VDR_CAP_OCCUR_TIME, occur_time);
		 table->Commit();
		 delete table;
	
		 scale_jpeg(buffer_src, buffer_det, width, height);
		 remove(buffer_src);
		 unlink(buffer_src);
		 sync();
	}

	return NULL;
}

int image_post_init()
{
	int ret = -1;

	pthread_t camapp_threads;	
	ret = pthread_create(&camapp_threads, NULL, handle_image, (void *)&dev);
	if(ret < 0)
	{
		bq_printf("create handle_image error!\n");
		return ret;
	}
	pthread_detach(camapp_threads);

	return ret;

}

void image_post_uninit()
{
	handle_image_exit = 1;
	return;
}

int switch_cam(int mode)
{
	if(mode < 0 || mode > 3)
		return -1;

	i2c_write_video_channle(chan[mode]);
	capobj->chancur_status = mode;

	return 0;
}

void DeviceReverseOutDbusNotifyHandler(DBusNotifyC *notify)
{
    printf("--- DeviceReverseOutDbusNotifyHandler: in ---\n");
	if(notify->getIdentifier()!= DEVICE_MANAGER_NOTF_ID_REVERSE)
        return;

	DeviceReverseDbusNotifyC *reverse_info = (DeviceReverseDbusNotifyC*)notify;
	
	CapappReverseDbusNotifyC notifyreverse;	

	if(!!reverse_info->flag)
		return;
		
    dbus_no = reverse_info->icount;
    if(in_reverse)
    {	
    	if(capobj->capold_status == CAPAPP_IN_DELAY)
    	{
    		irq_ts_enable();
    		camera_delay();
    		capobj->capcur_status = CAPAPP_IN_DELAY;
    	}
    	switch_cam(capobj->chanold_status);			
    	capobj->capcur_status = capobj->capold_status;
        in_reverse = 0;
    }    
	
	notifyreverse.reverse = false;
    notifyreverse.icount = reverse_info->icount;
	cap_DbusServer->sendNotify(&notifyreverse);
	
	printf("--- DeviceReverseOutDbusNotifyHandler: end ---\n");
	return;
}

void JT808CamDbusNotifyHandler(DBusNotifyC *notify)
{
    if(notify->getIdentifier()!= JT808_DBUS_NTF_ID_CAMERA)
        return;

	JT808CameraDbusReqC    request;
    DBusReplyC  *reply = NULL;
    int ret = -1;
	unsigned int interval = 0;
	int timeout = 0;
	char *name = (char *)malloc(sizeof(char)*1024);

	capobj->capold_status = capobj->capcur_status;
	capobj->chanold_status = capobj->chancur_status;

	JT808CameraDbusNotifyC *cam_info = (JT808CameraDbusNotifyC*)notify;

	bq_printf("m_command:%d\n", cam_info->m_command);
	bq_printf("m_channel:%d\n", cam_info->m_channel);

	if(cam_info->m_channel < 1 || cam_info->m_channel > 4)
	{
		bq_printf("channel num error:%d\n", cam_info->m_channel);
		return;
	}
	
	cam_info->m_channel -= 1;

	if(cam_info->m_channel != capobj->chancur_status)
	{
		switch_cam(cam_info->m_channel);
		sleep(1);
	}	
	cam_info->m_channel += 1;

	if(cam_info->m_command <= 0 || cam_info->m_command > 0xFFFF)
	{
		bq_printf("pictures num error:%d\n", cam_info->m_command);
		return;
	}

	if(CAPAPP_IN_PHOTO == capobj->capcur_status)
	{
		bq_printf("already in take picture status!\n");
		return;
	}

	capobj->capcur_status = CAPAPP_IN_PHOTO;
	if(!capobj->pthoto_status)
	{
		capobj->pthoto_status = PHOTO_IN_BACKEND;
	}

	interval = (cam_info->m_interval > 1)?cam_info->m_interval:1;

	int i = 0;
	long first_time = -1;
	long last_time = -1;
	unsigned long time = -1;

	dev.picinfo.m_channel 		= cam_info->m_channel;
	dev.picinfo.m_command 		= cam_info->m_command;
	dev.picinfo.m_interval 		= cam_info->m_interval;
	dev.picinfo.m_flag 			= cam_info->m_flag;
	dev.picinfo.m_resolution 	= cam_info->m_resolution;
	dev.picinfo.m_quality 		= cam_info->m_quality;
	dev.picinfo.m_brightness 	= cam_info->m_brightness;
	dev.picinfo.m_saturation 	= cam_info->m_saturation;
	dev.picinfo.m_chroma 		= cam_info->m_chroma;
	dev.picinfo.m_event			= capobj->pthoto_status;

	//irq_ts_disable();
	for(i = 0; i < cam_info->m_command; i++)
	{	
		memset(name, 0x0, 1024);
		time = camera_cap(cam_info->m_resolution, &name);
		strcat(name, ".jpg");
		if(!i)
			first_time = time;
		if(i == cam_info->m_command - 1)
			last_time = time;

		while((my_time() - time) < interval)
		{
			usleep(50*1000);
		}
	}
	//irq_ts_enable();

	timeout = pow(2, 31);
	while(access(name, F_OK) == -1)  
    {  
    	bq_printf("[JT808CamDbusNotifyHandler] still in!!!!!!!!!!!!!!!\n");
  		usleep(50*1000);
		timeout -= 50*1000;
		if(timeout < 0)
		{
			free(name);
			name = NULL;
			return;
		}
    }
		
		request.m_tableIndex = first_time;
		request.stop_time = last_time;
		request.trans_flag = !cam_info->m_flag;
	    ret = ca_ClientJT808->sendService(&request,reply,5*1000);
	    if(ret < 0)
	    {
	        bq_printf("Send Service Error. Error Code: %d\n",ret);
	    }
	    else
	    {
	        if(reply->getIdentifier()== DBUS_REP_ID_COMMON)
	        {
	            bq_printf("Received Common Reply. result:%d\n",reply->result);
	        }
	        else if(reply->getIdentifier()== JT808_DBUS_REP_ID_CAMERA)
	        {
	            bq_printf("Reply ok\n");
	        }
	        else
	        {
	            bq_printf("Reply not recongnized.\n");
	        }
	        ret = 0;
	        delete reply;
	    }
	
	if(capobj->chanold_status != capobj->chancur_status )
	{
		switch_cam(capobj->chanold_status);
		sleep(1);
	}

	capobj->capcur_status = CAPAPP_IN_DELAY;
	
	if(capobj->capold_status == CAPAPP_IN_VIDEO)
	{
		irq_ts_disable();
		camera_preview();
		capobj->capcur_status = CAPAPP_IN_VIDEO;
	}

	free(name);
	name = NULL;

	//capobj->capcur_status = CAPAPP_IN_DELAY;
    return;    
}

void DeviceAlarmDbusNotifyHandler(DBusNotifyC *notify)
{
	bq_printf("come in!\n");

	if(notify->getIdentifier()!= DEVICE_MANAGER_NOTF_ID_CAMERA)
        return;
	
	int i = 0;
	int ret = -1;
	int cap_num = 0;
	int timeout = 0;
	long first_time = -1;
	unsigned long time = -1;
	DBusReplyC  *reply    =  NULL;
	JT808CameraDbusReqC    request;
	DeviceCameraDbusNotifyC *cam_info = (DeviceCameraDbusNotifyC*)notify;

	bq_printf("event error:%d\n", cam_info->m_event);

	if(cam_info->m_event < 0 || cam_info->m_event > PHOTO_IN_OHTERALARM)
	{
		bq_printf("event error:%d\n", cam_info->m_event);
		return;
	}
	else if(cam_info->m_event < PHOTO_IN_OHTERALARM)
	{
		capobj->pthoto_status = cam_info->m_event;

		for(i = 0; i < 4; i++)
		{
			JT808CameraDbusNotifyC info;
			bq_printf("channelall:%d, channel:%d\n", cam_info->m_channel, (cam_info->m_channel & (int)pow(2, i)) >> i);
			if((cam_info->m_channel & (int)pow(2, i)) >> i)
			{	
				info.m_channel = i + 1;
				info.m_command = cam_info->m_command;
				info.m_interval = cam_info->m_interval;
				info.m_flag = (info.m_flag & (int)pow(2, i)) >> i;
				info.m_resolution = cam_info->m_resolution;
				info.m_quality = 0;
				info.m_brightness = 0;
				info.m_saturation = 0;
				info.m_chroma = 0;

				JT808CamDbusNotifyHandler((DBusNotifyC *)&info);
			}
		}

		capobj->pthoto_status = PHOTO_IN_BACKEND;
		return;
	}

	bq_printf("here!\n");

	capobj->capold_status = capobj->capcur_status;
	capobj->chanold_status = capobj->chancur_status;

	char *name = (char *)malloc(sizeof(char)*1024);
	capobj->capcur_status = CAPAPP_IN_PHOTO; 
	capobj->pthoto_status = PHOTO_IN_OHTERALARM;

	dev.picinfo.m_command 		= cam_info->m_command;
	dev.picinfo.m_interval 		= cam_info->m_interval;
	dev.picinfo.m_flag 			= cam_info->m_flag;
	dev.picinfo.m_resolution 	= cam_info->m_resolution;
	dev.picinfo.m_quality 		= 0;
	dev.picinfo.m_brightness 	= 0;
	dev.picinfo.m_saturation 	= 0;
	dev.picinfo.m_event			= cam_info->m_event;

	for(i = 0; i < 4; i++)
	{
		dev.picinfo.m_channel =  capobj->chancur_status + 1;
		memset(name, 0x0, 1024);
		time = camera_cap(cam_info->m_resolution, &name);
		strcat(name, ".jpg");
		if(!i)
			first_time = time;

		while((my_time() - time) < (unsigned int)cam_info->m_interval)
		{
			usleep(50*1000);
		}

		int state = capobj->chancur_status + 1;
		cap_num = offset(state);
		switch_cam(cap_num);
		sleep(1);
	}

	timeout = pow(2, 31);
	while(access(name, F_OK) == -1)  
    {  
    	bq_printf("[DeviceAlarmDbusNotifyHandler] still in!!!!!!!!!!!!!!!!\n");
  		usleep(200*1000);
		timeout -= 200*1000;
		if(timeout < 0)
		{
			free(name);
			name = NULL;
			return;
		}
    }
    fflush(NULL);

	if(cam_info->m_flag)
	{
		request.m_tableIndex = first_time;
        request.trans_flag = 1;
	    ret = ca_ClientJT808->sendService(&request,reply,5*1000);
	    if(ret < 0)
	    {
	        bq_printf("Send Service Error. Error Code: %d\n",ret);
	    }
	    else
	    {
	        if(reply->getIdentifier()== DBUS_REP_ID_COMMON)
	        {
	            bq_printf("Received Common Reply. result:%d\n",reply->result);
	        }
	        else if(reply->getIdentifier()== JT808_DBUS_REP_ID_CAMERA)
	        {
	            bq_printf("Reply ok\n");
	        }
	        else
	        {
	            bq_printf("Reply not recongnized.\n");
	        }
	        ret = 0;
	        delete reply;
	    }
	}
	
	free(name);
	name = NULL;

	if(capobj->capold_status == CAPAPP_IN_VIDEO)
	{
		irq_ts_disable();
		camera_preview();
		capobj->capcur_status = CAPAPP_IN_VIDEO;
	}

	capobj->pthoto_status = PHOTO_IN_BACKEND;

}

void UiSwitchChannelHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	CapappDbusReqC * req = (CapappDbusReqC *)request;
	reply;
	
	int cap_num = 0;
	if(capobj->capcur_status == CAPAPP_IN_REVERSE || capobj->capcur_status == CAPAPP_IN_PHOTO)
		return;

	if(req->display_flag)
	{
		if(capobj->capcur_status != CAPAPP_IN_VIDEO)
		{
			irq_ts_disable();
			camera_preview();
			capobj->capcur_status = CAPAPP_IN_VIDEO;
		}
		
		if(req->switch_flag)
		{	
			int state = capobj->chancur_status + 1;
			cap_num = offset(state);
			switch_cam(cap_num);
		}
		
	}
	else
	{
		irq_ts_enable();
		camera_delay();
		capobj->capcur_status = CAPAPP_IN_DELAY;
	}

	return;
}

int open_mem()
{
	fd_mem = open("/dev/mem", O_RDWR);
	if(fd_mem < 0)
	{
		bq_printf("open /dev/mem error!\n");
		return -1;
	}

    dev_mem = mmap(0, 1024*4, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, 0xF0401000);    
	if(NULL == dev_mem)
	{
		bq_printf("mmap error!\n");
		return -1;
	}

	irq_ts_enable();

	return 0;
}

int close_mem()
{
	irq_ts_enable(); 	
	munmap(dev_mem, 1024*4);
	close(fd_mem);

	return 0;
}

struct cap_obj *capobj_create(int size)
{
	if(size < 0)
		size = 0;
	
	struct cap_obj *capobj = (struct cap_obj *)malloc(sizeof(struct cap_obj) + size);
	if(NULL == capobj)
	{
		bq_printf("malloc capobj error!\n");
		return NULL;
	}
	memset(capobj, 0x0, sizeof(struct cap_obj) + size);

	return capobj;
}

void capobj_destory()
{
	free(capobj);
	capobj = NULL;
}

int capobj_init(struct cap_obj *capobj)
{
	if(NULL == capobj)
	{
		bq_printf("error param!\n");
		return -1;
	}
	
	capobj->capcur_status = CAPAPP_IN_DELAY;
	capobj->capold_status = CAPAPP_IN_DELAY;
	capobj->pthoto_status = PHOTO_IN_BACKEND;
	capobj->chancur_status = CHANNEL_IN_REVERSE;
	capobj->chanold_status = CHANNEL_IN_REVERSE;

	capobj->school_custom = 0;

	capobj->pri = (void *)(capobj + 1);

	return 0;
}

void UiReverseInDbusNotifyHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    printf("--- UiReverseInDbusNotifyHandler: in ---\n");
	CapappReverseDbusReqC * req = (CapappReverseDbusReqC *)request;
	reply;

	if(!req->reverse || capobj->capcur_status == CAPAPP_IN_REVERSE)
		return;

    if(dbus_no > req->icount)
        return;
    
    in_reverse = 1;
    dbus_no = req->icount;
    
	capobj->chanold_status = capobj->chancur_status;
	capobj->capold_status = capobj->capcur_status;
	
	bq_printf("current is %d\n", capobj->capcur_status);

	if(capobj->chancur_status != CHANNEL_IN_CVBS)
	{	
		i2c_write_video_channle(chan[CHANNEL_IN_CVBS]);
	}
	
	if(capobj->capcur_status == CAPAPP_IN_DELAY)
	{
		irq_ts_disable();
		camera_preview();
		capobj->capcur_status = CAPAPP_IN_REVERSE;
	}
    printf("--- UiReverseInDbusNotifyHandler: end ---\n");

}

int cap_dbus_init()
{
	cap_DbusServer = new DBusServerC(CAPAPP_DBUS_SERVER_ID, CAPAPP_DBUS_SERVER_OBJ);
	cap_DbusServer->registerService(CAPAPP_REQ_ID_REVERSE, UiSwitchChannelHandler, CapappDbusReqC::factory);	
	cap_DbusServer->registerService(CAPAPP_REQ_ID_REVERSEIN, UiReverseInDbusNotifyHandler, CapappReverseDbusReqC::factory);
    if(cap_DbusServer->connect() < 0)
    {
        bq_printf("Cann't connect to capapp DBUS server.\n");
        return -1;
    }

	ca_ClientJT808 = new DBusClientC(JT808_DBUS_SERVER_ID, JT808_DBUS_SERVER_OBJ);
	ca_ClientJT808->registerReply(JT808_DBUS_REP_ID_CAMERA, JT808CameraDbusRepC::factory);
	ca_ClientJT808->registerNotify(JT808_DBUS_NTF_ID_CAMERA,JT808CamDbusNotifyHandler,JT808CameraDbusNotifyC::factory);

	if(ca_ClientJT808->connect()<0)
    {
        bq_printf("Cann't connect to jt808 DBUS server.\n");
        return -1;
    }

	re_Device = new DBusClientC(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
	re_Device->registerNotify(DEVICE_MANAGER_NOTF_ID_REVERSE, DeviceReverseOutDbusNotifyHandler, DeviceReverseDbusNotifyC::factory);
	re_Device->registerNotify(DEVICE_MANAGER_NOTF_ID_CAMERA, DeviceAlarmDbusNotifyHandler,DeviceCameraDbusNotifyC::factory);

	if(re_Device->connect()<0)
    {
        printf("Cann't connect to re_Device DBUS server.\n");
        return -1;
    }

	return 0;
}

void cap_dbus_uninit()
{
	delete re_Device;
	delete ca_ClientJT808;
	delete cap_DbusServer;
}

void camera_uninit()
{
    image_post_uninit();

	camera_close();
	
	close_mem();

	capobj_destory();

	cap_dbus_uninit();
}


int main(int argc, char **argv)
{
	DBusApplicationC app(argc, argv);

	atexit(camera_uninit);

	if(cap_dbus_init())
	{
		bq_printf("cap_dbus_init error!\n");
		return -1;
	}
	
	bq_printf("connet successfully!\n");

	capobj = capobj_create(0);
	if(NULL == capobj)
	{
		bq_printf("create capobj error!\n");
		goto err_capobj_create;
	}

	if(capobj_init(capobj) < 0)
	{
		bq_printf("init capobj error!\n");
		goto err_capobj_init;	
	}

	if(open_mem() < 0)
	{
		bq_printf("open or map error!\n");
		goto err_open_mem;
	}

	if(camera_init((void *)video_type_init) < 0)
	{
		bq_printf("camera_init error!\n");
		goto err_camera_init;
	}
	
	if(image_post_init() < 0)
	{
		bq_printf("image_post_init error!\n");
		goto err_image_post_init;
	}

#ifdef STUDENT_SWIPE_CARD
	switch_cam(CHANNEL_IN_IN1);
	irq_ts_disable();
	camera_preview();
	capobj->capcur_status = CAPAPP_IN_VIDEO;
	capobj->school_custom = 1;
#endif

	#ifdef _8925_VERSION
	system("rm -rf /nand2/innovdata/photo/*src.jpg");
	#else
	system("rm -rf /nand3/innovdata/photo/*src.jpg");
	#endif
	sync();
#if 0
//-----------------------------------------------------------------------
/* 以下是测试拍照用的，能拍图片，但会有段错误 */
    if(argc > 2 && !strcmp(argv[1], "test"))
    {
        printf("=== test capapp start ===\n");
        JT808CameraDbusNotifyC test_notify;

        test_notify.m_channel = 0x04;
        test_notify.m_command = atoi(argv[2]);
        test_notify.m_interval = 1;
        test_notify.m_flag = 0;
        test_notify.m_resolution = 0x02;
        test_notify.m_quality = 0;
        test_notify.m_brightness = 0;
        test_notify.m_saturation = 0;
        test_notify.m_chroma = 0;
        
        sleep(2);
        JT808CamDbusNotifyHandler(&test_notify);
        sleep(2);
        printf("ls -l /nand3/innovdata/photo/\n");
        system("ls -l /nand3/innovdata/photo/");
        printf("=== test capapp end ===\n");
    }

//-----------------------------------------------------------------------
#endif

	return app.loop();

err_image_post_init:
	image_post_uninit();
	camera_close();
err_camera_init:
	close_mem();
err_open_mem:
err_capobj_init:
	capobj_destory();
err_capobj_create:
	cap_dbus_uninit();
	
	return -1;
}

