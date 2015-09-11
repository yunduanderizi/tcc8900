#ifndef _JT808_FIFO_MESSAGE_HANDLER_H_
#define _JT808_FIFO_MESSAGE_HANDLER_H_


#define JT808_FIFO_POSITION_INFO                         1
#define JT808_FIFO_IC_CARD_INFO                          10
#define JT808_FIFO_TRANSMISSION_INFO                     11






typedef struct
{
	int command;
	int cPositioning;
	int fX;
	int fY;
	int iTime;
	int iSysTime;
	int iOrientation;
	int iVelocity;
	int ilatitude;
	int ilongitude;
	int ialtitude;
	int cMode;
	int cSatellitenum;
	int cBDSatellitenum;
}CNaviGPSInfoReportNotifyC;

typedef struct 
{
	int command;
	int time;
}JT808DriverInfoReqC;

typedef struct
{
	int command;
	long long index;
}LocReportNotify;

typedef struct
{
	int command;
	int alarm;
}JT808LocReportNotify;

typedef struct
{
	int command;
	int interval;
	int validity;

}JT808TempLocReportDbusNotifyC;

typedef struct
{
	int command;

}JT808SettingUpdateDbusNotifyC;

typedef struct
{
	int command;
}RebootMachineReqC;

typedef struct
{
	int command;
}ShutdownMachineReqC;

typedef struct 
{
	int command;
	int mode;
	int baudrate;
	int outfrequency;
	int samplerate;
}CGNSSAttrChangeNotifyC;

typedef struct
{
	int command;
	int flag;
	int type;
	int id;
	int state;

}CNaviElectronicFenceNotifyC;

typedef struct
{
	int command;
}JT808RectangleFenceUpdateDbusNotifyC;

typedef struct
{
	int command;
	int type;
	int total;
	int errnum;
	unsigned char *buf;
}JT808DataTransmitReqC;

typedef struct
{
	int command;
	int flag;
}JT808ParkingDbusNotifyC;

int analyze_data_to_808(unsigned char* buff);
int handle_ic_card_info(unsigned char* buff);
int handle_position_info(unsigned char* buff);
int handle_transmissionData_info(unsigned char* buff);
#endif
