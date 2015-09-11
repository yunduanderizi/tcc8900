#include "JT808MessageHelper.h"
#include <boost/locale.hpp>
#include "LogUtility.h"
#include "Utility.h"
#include "VDREventsTable.h"
#include "VDRDeployParaTable.h"
#include "VDRRunParaTable.h"
#include "VDRAccelerateTable.h"
#include "ConfigManager.h"
#include "VDRContactsTable.h"
#include "ElectronicFenceTable.h"
#include "VDRTrackInfoTable.h"
#include "DriversLoginTable.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "VDRMonitorTable.h"
#include "DeviceManagerDbusMessage.h"
#include "../wm/dbusClientProxy.h"
#include "NaviDbusMessage.h"
#include <dbusClient.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <boost/locale.hpp>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "8H.h"
#include "9H.h"
#include "10H.h"
#include "11H.h"
#include "12H.h"
#include "13H.h"
#include "14H.h"
#include "15H.h"
#include "mangqu.h"
#include "JT808GnssManager.h"

//#define HBTEST
#define NHB
//for multi-packet update && re-translate
#define NUM_PACKAGE5    5
#define ENCRYPT_M1	74056196
#define ENCRYPT_IA1	90505889
#define ENCRYPT_IC1	47932445

#define ENCRYPT_M2	24511279	
#define ENCRYPT_IA2	13570514
#define ENCRYPT_IC2	28762643
typedef map<int, vector<unsigned char> > MultiPacketMap;
typedef map<int, vector<unsigned char> >::iterator MultiPacketIta;
typedef vector<int> Vint;
typedef vector<int>::size_type VType;
vector<unsigned char> tmpv;
MultiPacketMap mpmap;
Vint unReceivedNum;

extern DBusServerC *g_pDbusServer;
extern DBusClientProxyC *sp_ClientGps;
extern DBusServerC    *g_pDbusServer;
//extern int begin_gnss;
//extern int inter_gnss;
using namespace std;
static unsigned char phone1[]={0x31,0x33,0x32,0x34,0x30,0x31,0x33,0x31,0x33,0x36,0x33};
static unsigned char jt808_buf[JT808_FILE_SEGMENT_SIZE];

static int count_write=0;
static string updatepathname;
unsigned short first_seq;
int update_type;
vector<unsigned char > update_manuID;
int update_version_len;
int update_length;
time_t update_finish_time;
time_t update_install_time;
string update_version;

int register_flag = 0;
vector<unsigned char> StringToBCD(string &simiccid)
{
	vector<unsigned char> simbcd;
	simbcd.clear();
	int i = simiccid.size();
	if(i <= 0)
		return simbcd;
	for(int m=0; 2*m < i;m++)
	{
		simbcd.push_back(((CharToBCD(simiccid[2*m])<<4) & 0xf0) + CharToBCD(simiccid[2*m+1]));
	}
	return simbcd;
}
extern DBusClientC *g_pDbusClient;
typedef struct JT808Para
{
    JT808Para()
    {
        colid = -1;
        len = 0;
        type = ENUM_SQLite_COLUMN_INTEGER;
    }
    JT808Para(int col, EnumSQLiteColumnType t, int l)
    {
        colid = col;
        type = t;
        len = l;
    }
    int colid;
    EnumSQLiteColumnType type;
    int len;
} StruJT808Para;

static map<int, StruJT808Para> JT808Paras;
static map<int, StruJT808Para> JT808ReadOnlyParas;

#define BEGIN_GNSS 	0x00000002
void JT808MessageHelper::Init(JT808MessageHandler *handler)
{
    m_handler = handler;

    JT808Paras.clear();
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0001, StruJT808Para((int)VDR_PARA_HEART_BEAT_INTERVAL,       ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0002, StruJT808Para((int)VDR_PARA_TCP_REPLY_TIMEOUT,         ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0003, StruJT808Para((int)VDR_PARA_TCP_RETRY_TIMES,           ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0004, StruJT808Para((int)VDR_PARA_UDP_REPLY_TIMEOUT,         ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0005, StruJT808Para((int)VDR_PARA_UDP_RETRY_TIMES,           ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0006, StruJT808Para((int)VDR_PARA_SMS_REPLY_TIMEOUT,         ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0007, StruJT808Para((int)VDR_PARA_SMS_RETRY_TIMES,           ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0010, StruJT808Para((int)VDR_PARA_MAIN_SERVER_APN,           ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0011, StruJT808Para((int)VDR_PARA_MAIN_SERVER_USERNAME,      ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0012, StruJT808Para((int)VDR_PARA_MAIN_SERVER_PASSWORD,      ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0013, StruJT808Para((int)VDR_PARA_MAIN_SERVER_ADDRESS,       ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0014, StruJT808Para((int)VDR_PARA_SECOND_SERVER_APN,         ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0015, StruJT808Para((int)VDR_PARA_SECOND_SERVER_USERNAME,    ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0016, StruJT808Para((int)VDR_PARA_SECOND_SERVER_PASSWORD,    ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0017, StruJT808Para((int)VDR_PARA_SECOND_SERVER_ADDRESS,     ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0018, StruJT808Para((int)VDR_PARA_TCP_PORT,                  ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0019, StruJT808Para((int)VDR_PARA_UDP_PORT,                  ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x001A, StruJT808Para((int)VDR_PARA_IC_MAIN_SERVER_IP_ADDR,    ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x001B, StruJT808Para((int)VDR_PARA_IC_MAIN_SERVER_TCP_PORT,   ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x001C, StruJT808Para((int)VDR_PARA_IC_MAIN_SERVER_UDP_PORT,   ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x001D, StruJT808Para((int)VDR_PARA_IC_SECOND_SERVER_IP_ADDR,     ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0020, StruJT808Para((int)VDR_PARA_LOC_REPORT_POLICY,         ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0021, StruJT808Para((int)VDR_PARA_LOC_REPORT_SCENARIO,       ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0022, StruJT808Para((int)VDR_PARA_NOLOGIN_REPORT_INTERVAL,   ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0027, StruJT808Para((int)VDR_PARA_SLEEP_REPORT_INTERVAL,     ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0028, StruJT808Para((int)VDR_PARA_EMERGENCY_REPORT_INTERVAL, ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0029, StruJT808Para((int)VDR_PARA_DEFAULT_REPORT_INTERVAL,   ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x002C, StruJT808Para((int)VDR_PARA_DEFAULT_REPORT_DISTANCE,   ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x002D, StruJT808Para((int)VDR_PARA_NOLOGIN_REPORT_DISTANCE,   ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x002E, StruJT808Para((int)VDR_PARA_SLEEP_REPORT_DISTANCE,     ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x002F, StruJT808Para((int)VDR_PARA_EMERGENCY_REPORT_DISTANCE, ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0030, StruJT808Para((int)VDR_PARA_TURN_ANGLE,                ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0031, StruJT808Para((int)VDR_PARA_FENCE_RADIUS,              ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0040, StruJT808Para((int)VDR_PARA_PLATFORM_TEL,              ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0041, StruJT808Para((int)VDR_PARA_RESET_TEL,                 ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0042, StruJT808Para((int)VDR_PARA_FACTORY_RESET_TEL,         ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0043, StruJT808Para((int)VDR_PARA_PLATFORM_SMS,              ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0044, StruJT808Para((int)VDR_PARA_REPORT_SMS,                ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0045, StruJT808Para((int)VDR_PARA_CALL_ANSWER_POLICY,        ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0046, StruJT808Para((int)VDR_PARA_CALL_DURATION_ONCE,        ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0047, StruJT808Para((int)VDR_PARA_CALL_DURATION_MONTH,       ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0048, StruJT808Para((int)VDR_PARA_MONITOR_TEL,               ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0049, StruJT808Para((int)VDR_PARA_MONITOR_SMS,               ENUM_SQLite_COLUMN_STRING,      0)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0050, StruJT808Para((int)VDR_PARA_WARNING_MASK,              ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0051, StruJT808Para((int)VDR_PARA_WARNING_SMS_MASK,          ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0052, StruJT808Para((int)VDR_PARA_WARNING_PHOTO_MASK,        ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0053, StruJT808Para((int)VDR_PARA_WARNING_PHOTOSAVE_MASK,    ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0054, StruJT808Para((int)VDR_PARA_WARNING_KEY_MASK,          ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0055, StruJT808Para((int)VDR_PARA_MAX_SPEED,                 ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0056, StruJT808Para((int)VDR_PARA_OVERSPEED_DURATION,        ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0057, StruJT808Para((int)VDR_PARA_CONT_DRIVE_THRESHOLD,      ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0058, StruJT808Para((int)VDR_PARA_DAY_DRIVE_THRESHOLD,       ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0059, StruJT808Para((int)VDR_PARA_REST_MIN,                  ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x005A, StruJT808Para((int)VDR_PARA_PARKING_MAX,               ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x005B, StruJT808Para((int)VDR_PARA_OVERSPEED_WARNING,         ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x005C, StruJT808Para((int)VDR_PARA_TIRED_DRIVE_WARNING,       ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x005D, StruJT808Para((int)VDR_PARA_COLLISION_ALARM,           ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x005E, StruJT808Para((int)VDR_PARA_ROLLOVER_ALARM,            ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0064, StruJT808Para((int)VDR_PARA_TIME_CAPTURE_SET,          ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0065, StruJT808Para((int)VDR_PARA_DISTANCE_CAPTURE_SET,      ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0070, StruJT808Para((int)VDR_PARA_PHOTO_QUALITY,             ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0071, StruJT808Para((int)VDR_PARA_PHOTO_BRIGHTNESS,          ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0072, StruJT808Para((int)VDR_PARA_PHOTO_CONTRAST,            ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0073, StruJT808Para((int)VDR_PARA_PHOTO_SATUATION,           ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0074, StruJT808Para((int)VDR_PARA_PHOTO_CHROMA,              ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0090, StruJT808Para((int)VDR_PARA_GNSS_LOC_MODE,             ENUM_SQLite_COLUMN_INTEGER,     1)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0091, StruJT808Para((int)VDR_PARA_GNSS_BAUD_RATE,            ENUM_SQLite_COLUMN_INTEGER,     1)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0092, StruJT808Para((int)VDR_PARA_GNSS_DATA_OUT_RATE,        ENUM_SQLite_COLUMN_INTEGER,     1)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0093, StruJT808Para((int)VDR_PARA_GNSS_DATA_COLLECT_RATE,    ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0094, StruJT808Para((int)VDR_PARA_GNSS_DATA_UPLOAD_MODE,     ENUM_SQLite_COLUMN_INTEGER,     1)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0095, StruJT808Para((int)VDR_PARA_GNSS_DATA_UPLOAD_SET,      ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0100, StruJT808Para((int)VDR_PARA_CAN_1_COLLECT_INTERVAL,    ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0101, StruJT808Para((int)VDR_PARA_CAN_1_UPLOAD_INTERVAL,     ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0102, StruJT808Para((int)VDR_PARA_CAN_2_COLLECT_INTERVAL,    ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0103, StruJT808Para((int)VDR_PARA_CAN_2_UPLOAD_INTERVAL,     ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0110, StruJT808Para((int)VDR_PARA_CAN_ID_COLLECT_SET,        ENUM_SQLite_COLUMN_SEQUENCE,     8)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0111, StruJT808Para((int)VDR_PARA_OTHER_CAN_ID_COLLECT_SET,  ENUM_SQLite_COLUMN_SEQUENCE,     8)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0194, StruJT808Para((int)VDR_DEPLOY_PARA_ID,       	 	 ENUM_SQLite_COLUMN_SEQUENCE,     7)));
    JT808Paras.insert(map<int, StruJT808Para>::value_type(0x0195, StruJT808Para((int)VDR_DEPLOY_PARA_PHONE_NUMBER,       ENUM_SQLite_COLUMN_STRING,     0)));
    
    JT808ReadOnlyParas.clear();
    JT808ReadOnlyParas.insert(map<int, StruJT808Para>::value_type(0x0080, StruJT808Para((int)VDR_DEPLOY_PARA_DEPLOY_MILEAGE,        ENUM_SQLite_COLUMN_INTEGER,     4)));
    JT808ReadOnlyParas.insert(map<int, StruJT808Para>::value_type(0x0081, StruJT808Para((int)VDR_DEPLOY_PARA_PROVINCE_ID,        ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808ReadOnlyParas.insert(map<int, StruJT808Para>::value_type(0x0082, StruJT808Para((int)VDR_DEPLOY_PARA_CITY_ID,            ENUM_SQLite_COLUMN_INTEGER,     2)));
    JT808ReadOnlyParas.insert(map<int, StruJT808Para>::value_type(0x0083, StruJT808Para((int)VDR_DEPLOY_PARA_PLATE_ID,           ENUM_SQLite_COLUMN_STRING,      0)));
    JT808ReadOnlyParas.insert(map<int, StruJT808Para>::value_type(0x0084, StruJT808Para((int)VDR_DEPLOY_PARA_PLATE_COLOR,        ENUM_SQLite_COLUMN_INTEGER,     1)));

}

//获取key??

char* GetValue( char* pData, int len, string strKey, string& strVale )
{
	char* pTemp = strstr( pData, strKey.c_str() );
	if( pTemp == NULL )
		return NULL;
	pTemp += strKey.length()+2; 
	len -= strKey.length()+2;
	char* pTemp1 = pTemp;
	for( int i = 0; i < len ; i++ )
	{
		if( pTemp[i] == ',' || pTemp[i] == '}' || pTemp[i] == ']' )
		{
			pTemp1 = pTemp+i;
			break;
		}
	}
	if( pTemp1 - pTemp > 1 && pTemp1 - pTemp < len )
	{
		char* chTemp = new char[pTemp1 - pTemp+1];
		memcpy( chTemp, pTemp, pTemp1 - pTemp );
		chTemp[pTemp1 - pTemp-1]=0;
		if( chTemp[pTemp1 - pTemp] == '\"' )
		{
			chTemp[pTemp1 - pTemp] = 0;
		}
		if( chTemp[0]=='\"' )
		{
			strVale = chTemp+1;
		}
		else
		{
			strVale = chTemp;
		}
		delete[]  chTemp;
		return pTemp1+1;
	}
	return NULL;
}
/**
  * 初始??
  *	@param	    pData	    [in]	数据段，gbk编码
  *	@param	    nLen	    [in]	数据长度
  *	@param	    pPOI	    [out]	目的地内??
  *	@return		int			[out]	返回点的个数
  */
int GetData( char* pData, int nLen, tagPOI* pPOI )
{
	//{"ID":24577,"COORD":[{"LAT":40.921302,"LNG":115.23456,"POI":"海淀"?.?.?.?.?.?,{"LAT":41.921302,"LNG":114.23456,"POI":"?"
	char* pTemp = pData;
	const char* strComm = "\"ID\":24577";
	const char* strLan = "LAT";
	const char* strLon = "LNG";
	const char* strPoiName = "POI";
	char chTemp[64];
	int iCount = 0;

	pTemp = strstr( pTemp, strComm );
	if( pTemp == NULL )
		return iCount;
	pTemp += strlen( strComm )+1;
	char* pTemp1 = NULL;
	for( int i =0; i < 5; i++ )
	{
		string strValue;
		if( nLen < 0 )
			break;
		//--
		pTemp1  = GetValue( pTemp,nLen, strLan, strValue );
		if( !pTemp1 )
			break;
		pPOI[i].dlan = atof( strValue.c_str());
		nLen -= pTemp1 - pTemp;
		pTemp = pTemp1;
		//经度
		pTemp1  = GetValue( pTemp,nLen, strLon, strValue );
		if( !pTemp1 )
			break;
		pPOI[i].dlon = atof( strValue.c_str());
		nLen -= pTemp1 - pTemp;
		pTemp = pTemp1;
		//经度
		pTemp1  = GetValue( pTemp,nLen, strPoiName, strValue );
		if( !pTemp1 )
			break;
		pPOI[i].strName = strValue;
		nLen -= pTemp1 - pTemp;
		pTemp = pTemp1;
		//--
		iCount++;
	}
	return iCount;
}
JT808MessageHandler *JT808MessageHelper::m_handler = NULL;
int JT808MessageHelper::seq_bak= 0;

void JT808MessageHelper::SendGzipPacket(unsigned char *file_name)
{
	int i=0, n;
	unsigned char buf[1024];
	FILE * fp = fopen((const char *)file_name, "rb");	
	printf("open file: %s error\n", file_name);
	if(fp < 0)
		printf("open file: %s error\n", file_name);
	if((n = fread(buf, 1, 1024, fp)) > 0)
	{
		if(ferror(fp) != 0)
			return ;
		StruJT808Packet *thepacket = m_handler->CreatePacket();

		JT808Packet &packet = thepacket->packet;

		while( i < n)
		{
			packet.push_back(buf[i]);
			printf("%d ", buf[i]);
			if(i%16==0)
				printf("\n");
			i++;
		}

	    packet.push_front(i&0xff);
	    packet.push_front((i>>8)&0xff);
	    packet.push_front((i>>16)&0xff);
	    packet.push_front((i>>24)&0xff);
		printf("open file: %s error %d\n", file_name, n);
	    m_handler->SendBody(thepacket, JT808_PACKET_UP_GZIP_TRANSMISSION, JT808_CRYTO_NONE);
	}
		printf("open file: %s error\n", file_name);
}
bool encrypt(unsigned int key, unsigned int M1, unsigned int IA1, unsigned int IC1, JT808Packet &buf, unsigned int len)
{
	if(key == 0) key = 1;
	unsigned int mkey = M1;
	if(0 == mkey) mkey = 1;
	int i = len;
	int num = buf.size();
	while(i < num){
		key = IA1 * (key % mkey) + IC1;
		buf[i++] ^= (unsigned char)((key>>20) & 0xFF);
	}
	return true;
}
int JT808MessageHelper::SendRegister(int flag, unsigned char phone[])
{
    int province, city, city1, platecolor;
    vector<unsigned char> manuID, modelNO, modelID;
    string plateNO= "", carVIN= "", mphone= "";
    int i, j , k=0;
    
    VDRDeployParaTable *table = new VDRDeployParaTable();
    VDRDayInfoTable *table2 = new VDRDayInfoTable();
	printf("********************************** flag = %d\n", flag);
    if((flag == 1) && (phone != NULL))
    {
	    string auth = "";
		printf("*********************** enter  flag == 1\n");
/*
	    for(i=0; i<11; i++)
	    {
		    printf("_____%d \n", phone[i]);
		    mphone.push_back(phone[i]);
	    }
	    cout<<"new phone is "<< mphone << endl;

	    if(table->SetStringValue((int)VDR_DEPLOY_PARA_PHONE_NUMBER, mphone) < 0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister set fplateNO phone failed");
		    delete table;
		    return -1;
	    }

	    */
	    system("cp /nand2/tables/vdrsetting.db.bak /nand2/tables/vdrsetting.db");
        if(table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE,0)<0)
	    {
	    	LogUtility::Log(LOG_LEVEL_DEBUG,"JT808MessageHandler::ResetFactory set mileage faild.");
		    delete table;
		    return -1;
	    }
	    if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendAuth get auth token failed");
		    delete table;
		    return -1;
	    }
        table->Commit();
	    delete table;

	    table2->RefreshBottom();
	    if(table2->SetIntValue(VDR_DAY_TOTAL_MILEAGE,0)<0)
	    {
	    	LogUtility::Log(LOG_LEVEL_DEBUG,"JT808MessageHandler::ResetFactory set mileage faild.");
		delete table2;
		return -1;
	    }
	    if(table2->SetIntValue(VDR_DAY_DRIVE_MILEAGE,0)<0)
	    {
	    	LogUtility::Log(LOG_LEVEL_DEBUG,"JT808MessageHandler::ResetFactory set mileage faild.");
		delete table2;
		return -1;
	    }
	    table2->Commit();
	    delete table2;
	    
        SendJT808SettingUpdateNotify();  
	    
	    JT808ParkingDbusNotifyC notify;
	    notify.flag = 6; 
	    g_pDbusServer->sendNotify(&notify);

	    m_handler->m_registered = 0;
	    m_handler->auth_failure = 0;
	    m_handler->m_authfailure = 0;
	    m_handler->m_authorized = 0;
	    ConnectionManager::Instance()->Reconnect();
	    return 0;
    }


    if(table->GetIntValue((int)VDR_DEPLOY_PARA_PROVINCE_ID, province) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get province failed");
        delete table;
        return -1;
    }
    
    if(table->GetIntValue((int)VDR_DEPLOY_PARA_CITY_ID, city) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get city failed");
        delete table;
        return -1;
    }
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get city %d \n",city);
	city1= city%10000;
    
    if(table->GetIntValue((int)VDR_DEPLOY_PARA_PLATE_COLOR, platecolor) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get plate color failed");
        delete table;
        return -1;
    }

    if(table->GetSequenceValue((int)VDR_DEPLOY_PARA_MANUFACTORY_ID, manuID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get manufactory failed");
        delete table;
        return -1;
    }

    if(table->GetSequenceValue((int)VDR_DEPLOY_PARA_MODEL_NO, modelNO) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get modelNO failed");
        delete table;
        return -1;
    }
    if(table->GetSequenceValue((int)VDR_DEPLOY_PARA_ID, modelID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get modelID failed");
        delete table;
        return -1;
    }


    if(table->GetStringValue((int)VDR_DEPLOY_PARA_PLATE_ID, plateNO) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get plateNO failed");
        delete table;
        return -1;
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendRegister plateNO %s", plateNO.c_str());

    if(table->GetStringValue((int)VDR_DEPLOY_PARA_CAR_VIN, carVIN) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get carVIN failed");
        delete table;
        return -1;
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendRegister carVIN %s", carVIN.c_str());

    delete table;
    delete table2;
    StruJT808Packet *thepacket = m_handler->CreatePacket();

    JT808Packet &packet = thepacket->packet;
    if(flag == 2 || flag == 12)
    {
	    register_flag = 1;//使用前锁定开会后需要注??	   
		if(flag == 2){
		    m_handler->PushByte(packet, 0x00);
		    m_handler->PushLong(packet, 0x00);
	    }else{
		    m_handler->PushByte(packet, 0x01);
		    k = manuID[0] - '0';
		    for(j=1; j<5; j++){
			    k = 10*k + manuID[j] - '0';
		    }
		    m_handler->PushLong(packet, k);
		    printf("k is %d \n", k);
	    }
	    printf("k is %d and flag is %d \n", k, flag);
	    for(i=0; i<11; i++)
	    {
		    //m_handler->PushByte(packet, phone1[i]);
		    m_handler->PushByte(packet, phone[i]);
		    printf("i is %d = %d \n", i, phone[i]);
	    }
	    m_handler->PushByte(packet, 0x00);
    }
	m_handler->PushWord(packet, (unsigned short)province);
	m_handler->PushWord(packet, (unsigned short)city1);
	m_handler->PushSequence(packet, manuID, 5);
	m_handler->PushSequence(packet, modelNO, 20);
	m_handler->PushSequence(packet, modelID, 7);
#ifdef YA
    m_handler->PushLong(packet, 0);
#endif
    if(flag == 2 || flag == 12)
		m_handler->PushByte(packet, (unsigned char)0);
	else
		m_handler->PushByte(packet, (unsigned char)platecolor);
    if(flag == 2 || flag == 12) 
    {
#if 0
	    string plate_gb_no = boost::locale::conv::from_utf<char>(plateNO, JT808_CHAR_SET);
	    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendRegister plateNOgb %s", plate_gb_no.c_str());
	    m_handler->PushString(packet, plate_gb_no);
#endif
	    for(i=0;i<9;i++)
	    {
	    	m_handler->PushByte(packet,0x00);
	    }
	    //m_handler->PushString(packet, plateNO);
	    //m_handler->PushWord(packet, 0x00);
	    string car_vin= boost::locale::conv::from_utf<char>(carVIN, JT808_CHAR_SET);
	    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendRegister plateNOgb %s", car_vin.c_str());
	    m_handler->PushString(packet, car_vin);
	    m_handler->PushByte(packet, 0x00);
    }else if(flag == 3){
	    string car_vin= boost::locale::conv::from_utf<char>(carVIN, JT808_CHAR_SET);
	    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendRegister plateNOgb %s", car_vin.c_str());
	    m_handler->PushString(packet, car_vin);
	    m_handler->PushByte(packet, 0x00);
    }else{
	    string plate_gb_no = boost::locale::conv::from_utf<char>(plateNO, JT808_CHAR_SET);
	    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendRegister plateNOgb %s", plate_gb_no.c_str());
	    m_handler->PushString(packet, plate_gb_no);
    }

    m_handler->GetPhoneNumber();
    if(flag == 2)
	    m_handler->SendBody(thepacket, JT808_PACKET_ID_QUICK_REGISTER, JT808_CRYTO_NONE);
    else if(flag == 12)
    {
	    for(int kk=0; kk < packet.size(); kk++)
	    {
		    printf("%02x ", packet[kk]);
		    if(kk!= 0 && kk %16 == 0)
			    printf("\n");
	    }
	    if(k == 70712)
		    encrypt(k, ENCRYPT_M1, ENCRYPT_IA1, ENCRYPT_IC1, packet, 5);
	    else
		    encrypt(k, ENCRYPT_M2, ENCRYPT_IA2, ENCRYPT_IC2, packet, 5);
	    m_handler->SendBody(thepacket, JT808_PACKET_ID_QUICK_REGISTER, JT808_CRYTO_RSA);
    }else
	    m_handler->SendBody(thepacket, JT808_PACKET_ID_REGISTER, JT808_CRYTO_NONE);
    return 0;
}

int JT808MessageHelper::SendLogout()
{
    StruJT808Packet *thepacket = m_handler->CreatePacket();

    m_handler->SendBody(thepacket, JT808_PACKET_ID_LOGOUT, JT808_CRYTO_NONE);
    
    return 0;
}

int JT808MessageHelper::SendAuth()
{
    m_handler->GetPhoneNumber();
    string auth;
    
    VDRDeployParaTable *table = new VDRDeployParaTable();
    
    if(table->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendAuth get auth token failed");
        delete table;
        return -1;
    }
    
    delete table;
    
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    
    if(ConfigManager::m_control == 0){
	    m_handler->PushString(packet, ConfigManager::m_auth);
    }else 
	    m_handler->PushString(packet, auth);

    m_handler->SendBody(thepacket, JT808_PACKET_ID_AUTH, JT808_CRYTO_NONE);
    
    return 0;
}

void JT808MessageHelper::SendXsjlErrorAck(unsigned short seq, unsigned short id, int result)
{
    unsigned char checksum = 0x2F;
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    thepacket->reply = false;
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushWord(packet, seq);
    m_handler->PushByte(packet, (unsigned char)id);

    m_handler->PushByte(packet, 0x55);
    m_handler->PushByte(packet, 0x7A);
    if(result != 0x00){
	    m_handler->PushByte(packet, (unsigned char)result);
	    m_handler->PushByte(packet, 0x00);
	    if(result == 0xFA)
		    m_handler->PushByte(packet, 0xd5);
	    else
		    m_handler->PushByte(packet, 0xd4);
    }else{
    	    m_handler->PushByte(packet, (unsigned char)id);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushByte(packet, 0x00);

	    checksum ^= (unsigned char)id; 
	    m_handler->PushByte(packet, checksum);
    }

    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
    
    return;
}

void JT808MessageHelper::SendAck(unsigned short seq, unsigned short id, int result)
{
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    thepacket->reply = true;
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushWord(packet, seq);
    m_handler->PushWord(packet, id);
    m_handler->PushByte(packet, (unsigned char)result);

    m_handler->SendBody(thepacket, JT808_PACKET_TERM_COMMON_REP, JT808_CRYTO_NONE);
    
    return;
}
void JT808MessageHelper::push_deploy_para(JT808Packet & packet)
{	
    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
    
    for(map<int, StruJT808Para>::iterator it = JT808ReadOnlyParas.begin(); it != JT808ReadOnlyParas.end(); it ++)
    {
        int id = it->first;
        StruJT808Para &para = it->second;
        m_handler->PushLong(packet, (unsigned long)id);
        if(para.type == ENUM_SQLite_COLUMN_INTEGER)
        {
            int value = 0;
            deploytable->GetIntValue(para.colid, value);
            if(para.len == 1)
            {
                m_handler->PushByte(packet, 0x01);
                m_handler->PushByte(packet, (unsigned char)value);
            }
            else if(para.len == 2)
            {
                m_handler->PushByte(packet, 0x02);
                m_handler->PushWord(packet, (unsigned short)value);
            }
            else
            {
                m_handler->PushByte(packet, 0x04);
                m_handler->PushLong(packet, (unsigned long)value);
            }
        }
        else if(para.type == ENUM_SQLite_COLUMN_STRING)
        {
            string value = "";
            string gb_value = boost::locale::conv::from_utf<char>(value, JT808_CHAR_SET);
            deploytable->GetStringValue(para.colid, gb_value);
            m_handler->PushByte(packet, (unsigned char)(gb_value.length()));
            m_handler->PushString(packet, gb_value);
        }
        else
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendParameters para type not match 2");
        }
    }
    
    delete deploytable;

}
void JT808MessageHelper::SendParameters(unsigned short seq)
{
    int paracount, i=0;
    StruJT808Packet *thepacket = m_handler->CreatePacket();

    thepacket->reply = true;
    paracount = JT808Paras.size();
    paracount += JT808ReadOnlyParas.size();
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushWord(packet, (unsigned short)seq);
    m_handler->PushByte(packet, (unsigned char)paracount-3);
    
    VDRRunParaTable *table = new VDRRunParaTable();
    
    for(map<int, StruJT808Para>::iterator it = JT808Paras.begin(); it != JT808Paras.end(); it ++)
    {
        int id = it->first;
	if(id > 0x74 && i == 0)
	{
		i=1;
		push_deploy_para(packet);
	}
	if(id > 0x110) break;
        StruJT808Para &para = it->second;
        m_handler->PushLong(packet, (unsigned long)id);
        if(para.type == ENUM_SQLite_COLUMN_INTEGER)
        {
            int value = 0;
            table->GetIntValue(para.colid, value);
            if(para.len == 1)
            {
                m_handler->PushByte(packet, 0x01);
                m_handler->PushByte(packet, (unsigned char)value);
            }
            else if(para.len == 2)
            {
                m_handler->PushByte(packet, 0x02);
                m_handler->PushWord(packet, (unsigned short)value);
            }
            else
            {
                m_handler->PushByte(packet, 0x04);
                m_handler->PushLong(packet, (unsigned long)value);
            }
        }
        else if(para.type == ENUM_SQLite_COLUMN_STRING)
        {
            string value = "";
            table->GetStringValue(para.colid, value);
            m_handler->PushByte(packet, (unsigned char)(value.length() ));
            m_handler->PushString(packet, value);
        }
        else if(para.type == ENUM_SQLite_COLUMN_SEQUENCE)
        {
            vector<unsigned char> value;
            table->GetSequenceValue(para.colid, value);
	    /* normal method
            m_handler->PushByte(packet, (unsigned char)(value.size() ));
            m_handler->PushSequence(packet, value, value.size());
	    */
            m_handler->PushByte(packet, (unsigned char)(para.len ));
            m_handler->PushSequence(packet, value, para.len);
            printf("=======================%d\n", (unsigned char)(value.size() ));
        }
        else
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendParameters para type not match 1");
        }
    }
    
    delete table;

   /* 
    m_handler->PushLong(packet, (unsigned long)0x0080);
    m_handler->PushByte(packet, (unsigned char)0x04);
    m_handler->PushLong(packet, (unsigned long)0x00);
    */
    
    m_handler->SendBody(thepacket, JT808_PACKET_QUERY_PARAMETERS_REP, JT808_CRYTO_NONE);
    return;
}

void JT808MessageHelper::SendParameters(unsigned short seq, map<int ,int>&idlist)
{
    int paracount;
    if((paracount=idlist.size())<=0)return;
    map<int, int>::iterator ltmp;
    for(ltmp = idlist.begin(); ltmp != idlist.end(); ltmp++)
	    printf("^^^^^^^^^^^^^^^%d\n", ltmp->first);
    StruJT808Packet *thepacket = m_handler->CreatePacket();

    thepacket->reply = true;
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushWord(packet, (unsigned short)seq);
    m_handler->PushByte(packet, (unsigned char)paracount);
    
    VDRRunParaTable *table = new VDRRunParaTable();
    
    for(map<int, StruJT808Para>::iterator it = JT808Paras.begin(); it != JT808Paras.end(); it ++)
    {
        int id = it->first;
	printf("--------------------------------------*********************find id is %d\n", id);
	if(idlist.count(id)<=0)continue;
	printf("--------------------------------------*********************find id is %d\n", id);
        StruJT808Para &para = it->second;
        m_handler->PushLong(packet, (unsigned long)id);
        if(para.type == ENUM_SQLite_COLUMN_INTEGER)
        {
            int value = 0;
            table->GetIntValue(para.colid, value);
            if(para.len == 1)
            {
                m_handler->PushByte(packet, 0x01);
                m_handler->PushByte(packet, (unsigned char)value);
            }
            else if(para.len == 2)
            {
                m_handler->PushByte(packet, 0x02);
                m_handler->PushWord(packet, (unsigned short)value);
            }
            else
            {
                m_handler->PushByte(packet, 0x04);
                m_handler->PushLong(packet, (unsigned long)value);
            }
        }
        else if(para.type == ENUM_SQLite_COLUMN_STRING)
        {
            string value = "";
            table->GetStringValue(para.colid, value);
            m_handler->PushByte(packet, (unsigned char)(value.length() + 1));
            m_handler->PushString(packet, value);
        }
        else
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendParameters para type not match 1");
        }
    }
    
    delete table;

    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
    
    for(map<int, StruJT808Para>::iterator it = JT808ReadOnlyParas.begin(); it != JT808ReadOnlyParas.end(); it ++)
    {
        int id = it->first;
	if(idlist.count(id)<=0)continue;
        StruJT808Para &para = it->second;
        m_handler->PushLong(packet, (unsigned long)id);
        if(para.type == ENUM_SQLite_COLUMN_INTEGER)
        {
            int value = 0;
            deploytable->GetIntValue(para.colid, value);
            if(para.len == 1)
            {
                m_handler->PushByte(packet, 0x01);
                m_handler->PushByte(packet, (unsigned char)value);
            }
            else if(para.len == 2)
            {
                m_handler->PushByte(packet, 0x02);
                m_handler->PushWord(packet, (unsigned short)value);
            }
            else
            {
                m_handler->PushByte(packet, 0x04);
                m_handler->PushLong(packet, (unsigned long)value);
            }
        }
        else if(para.type == ENUM_SQLite_COLUMN_STRING)
        {
            string value = "";
            string gb_value = boost::locale::conv::from_utf<char>(value, JT808_CHAR_SET);
            deploytable->GetStringValue(para.colid, gb_value);
            m_handler->PushByte(packet, (unsigned char)(gb_value.length() + 1));
            m_handler->PushString(packet, gb_value);
        }
        else
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendParameters para type not match 2");
        }
    }
    
    delete deploytable;
    
    //m_handler->PushLong(packet, (unsigned long)0x0080);
    //m_handler->PushByte(packet, (unsigned char)0x04);
    //m_handler->PushLong(packet, (unsigned long)0x00);
    
    m_handler->SendBody(thepacket, JT808_PACKET_QUERY_PARAMETERS_REP, JT808_CRYTO_NONE);
    return;
}
void JT808MessageHelper::SendAttribute(unsigned short seq)
{
    int term_type,gnssAttri,gprsAttri;
    vector<unsigned char> manuID, modelNO, modelID, simv;
    string hardvers,firmvers, simNO;

    StruJT808Packet *thepacket = m_handler->CreatePacket();

    thepacket->reply = true;
    JT808Packet &packet = thepacket->packet;

    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
   /* 
    if(deploytable->GetIntValue((int)VDR_DEPLOY_PARA_VDR_TYPE, term_type) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get province failed");
        delete deploytable;
        return ;
    }
    */
    term_type=7;
    if(deploytable->GetSequenceValue((int)VDR_DEPLOY_PARA_MANUFACTORY_ID, manuID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get city failed");
        delete deploytable;
        return ;
    }
    
    if(deploytable->GetSequenceValue((int)VDR_DEPLOY_PARA_MODEL_NO, modelNO) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get plate color failed");
        delete deploytable;
        return ;
    }

    if(deploytable->GetSequenceValue((int)VDR_DEPLOY_PARA_ID, modelID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get manufactory failed");
        delete deploytable;
        return ;
    }
    if(deploytable->GetStringValue((int)VDR_DEPLOY_PARA_VDR_SIM_ICCID, simNO) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get modelNO failed");
        delete deploytable;
        return ;
    }
    cout << simNO<<endl;
    simv = StringToBCD(simNO);
#if 0
    for(int i=0;i<10;i++)
    {
	    cout<<simv[i]<<endl;
    }
#endif
    if(deploytable->GetStringValue((int)VDR_DEPLOY_PARA_HARDWARE_VERSION, hardvers) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get modelID failed");
        delete deploytable;
        return ;
    }


    if(deploytable->GetStringValue((int)VDR_DEPLOY_PARA_VERSION, firmvers) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get plateNO failed");
        delete deploytable;
        return ;
    }
    
    /*
    if(deploytable->GetIntValue((int)VDR_DEPLOY_PARA_GNSS_ATTRIBUTE, gnssAttri) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get plateNO failed");
        delete deploytable;
        return ;
    }

    if(deploytable->GetIntValue((int)VDR_DEPLOY_PARA_COMMUNICATE_ATTRIBUTE, gnssAttri) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get plateNO failed");
        delete deploytable;
        return ;
    }
*/
    gnssAttri=3;
    gprsAttri=9;
    delete deploytable;

    m_handler->PushWord(packet, (unsigned short)term_type);
    m_handler->PushSequence(packet, manuID, 5);
    m_handler->PushSequence(packet, modelNO, 20);
    m_handler->PushSequence(packet, modelID, 7);
    m_handler->PushSequence(packet, simv, 10);


    string plate_gb_to = boost::locale::conv::from_utf<char>(hardvers, JT808_CHAR_SET);
    m_handler->PushByte(packet, plate_gb_to.length());
    m_handler->PushString(packet, plate_gb_to);

    plate_gb_to = boost::locale::conv::from_utf<char>(firmvers, JT808_CHAR_SET);
    m_handler->PushByte(packet, plate_gb_to.length());
    m_handler->PushString(packet, plate_gb_to);

    m_handler->PushByte(packet,gnssAttri); 
    m_handler->PushByte(packet,gprsAttri); 
    
    m_handler->SendBody(thepacket, JT808_PACKET_QUERY_ATTRIBUTE_REP, JT808_CRYTO_NONE);
    return;
}

void JT808MessageHelper::PackLocation(VDRLocStatus1Table *table, JT808Packet &packet,int lock_door)
{
    int alarmflag = 0;
    table->GetIntValue((int)VDR_LOCS_ALARM_SIGNAL, alarmflag);
    m_handler->PushLong(packet, alarmflag);
    int statusflag = 0;
    table->GetIntValue((int)VDR_LOCS_STATUS_FLAG, statusflag);
#if 0
    if(lock_door != WY_MAGIC)
    {
	    if( lock_door == 1 )
		    alarmflag |= 0x80000000;
	    else
		    alarmflag &= ~(0x80000000);

    }
#endif
    m_handler->PushLong(packet, statusflag);
    int latitude = 39;
    table->GetIntValue((int)VDR_LOCS_LATITUDE, latitude);
    m_handler->PushLong(packet, latitude);
    int longitude = 116;
    table->GetIntValue((int)VDR_LOCS_LONGITUDE, longitude);
    m_handler->PushLong(packet, longitude);
    int altitude = 0;
    table->GetIntValue((int)VDR_LOCS_ALTITUDE, altitude);
    m_handler->PushWord(packet, (unsigned short)altitude);
    int gpsspeed = 0;
    table->GetIntValue((int)VDR_LOCS_GPS_SPEED, gpsspeed);
    m_handler->PushWord(packet, gpsspeed);
    int orient = 0;
    table->GetIntValue((int)VDR_LOCS_ORIENTATION, orient);
    m_handler->PushWord(packet, (unsigned short)orient);
    vector<unsigned char> bcdTime;
    int tableIndex = 0;
    table->GetIntValue((int)VDR_LOCS_OCCUR_TIME, tableIndex);
    GetTime((long long)tableIndex, bcdTime);
    m_handler->PushSequence(packet, bcdTime, 6);

    unsigned char id = (unsigned char)JT808_LOC_ADDITION_MILEAGE;
    unsigned char len = 4;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    int mileage = 0;
    table->GetIntValue((int)VDR_LOCS_TOTAL_MILEAGE, mileage);
    m_handler->PushLong(packet, mileage);
    
    id = (unsigned char)JT808_LOC_ADDITION_OILAGE;
    len = 2;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    int oilage = 0;
    table->GetIntValue((int)VDR_LOCS_OIL_MASS, oilage);
    m_handler->PushWord(packet, oilage);

    id = (unsigned char)JT808_LOC_ADDITION_SPEED;
    len = 2;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    int vdrspeed = 0;
    table->GetIntValue((int)VDR_LOCS_VDR_SPEED, vdrspeed);
    m_handler->PushWord(packet, vdrspeed);

#ifdef NHB
    if(alarmflag & 0x01)
    {
        int alarmid = 0;
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        table->GetIntValue((int)VDR_LOCS_EMERGENCY_ALARM_ID, alarmid);
        m_handler->PushWord(packet, alarmid);
    }
#endif

    if(alarmflag & 0x02)
    {
        int type = 0, area = 0;
        id = (unsigned char)JT808_LOC_ADDITION_OVERAPEED;
        m_handler->PushByte(packet, id);
        table->GetIntValue((int)VDR_LOCS_OVERSPEED_ALARM, type);
        table->GetIntValue((int)VDR_LOCS_OVERSPEED_REGIONID, area);
        if(type == 0)
        {
            len = 1;
            m_handler->PushByte(packet, len);
            m_handler->PushByte(packet, type);
        }
        else
        {
            len = 5;
            m_handler->PushByte(packet, len);
            m_handler->PushByte(packet, type);
            m_handler->PushLong(packet, area);
        }
    }

    if(alarmflag & 0x300000)
    {
        int type = 0, area = 0, dir = 0;
        id = (unsigned char)JT808_LOC_ADDITION_FENCE;
        len = 6;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        table->GetIntValue((int)VDR_LOCS_INOUT_ALARM, type);
        table->GetIntValue((int)VDR_LOCS_INOUT_REGIONID, area);
        table->GetIntValue((int)VDR_LOCS_INOUT_DIRECTION, dir);
        m_handler->PushByte(packet, type);
        m_handler->PushLong(packet, area);
        m_handler->PushByte(packet, dir);
    }

    if(alarmflag & 0x400000)
    {
        int linetime = 0, line = 0, result = 0;
        id = (unsigned char)JT808_LOC_ADDITION_LINE_SHORT;
        len = 7;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        table->GetIntValue((int)VDR_LOCS_SHORT_LONG_LINEID, line);
        table->GetIntValue((int)VDR_LOCS_SHORT_LONG_TIME, linetime);
        table->GetIntValue((int)VDR_LOCS_SHORT_LONG_RESULT, result);
        m_handler->PushLong(packet, line);
        m_handler->PushWord(packet, linetime);
        m_handler->PushByte(packet, result);
    }
#ifdef NHB
    {//0x25
	int status;
        id = (unsigned char)JT808_LOC_ADDITION_STATE;
	len =4;
        table->GetIntValue((int)VDR_LOCS_VDR_NEW_HB_STATUS, status);
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet, status);
    }
    {//2A
	int io;
        id = (unsigned char)JT808_LOC_ADDITION_IO ;
	len =2;
        table->GetIntValue((int)VDR_LOCS_IO_STATUS, io);
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, io);
    }
    {//0x2B
   	int  analog;
        id = (unsigned char)JT808_LOC_ADDITION_AD;
	len = 4;
        table->GetIntValue((int)VDR_LOCS_ANALOG_QUANTITY, analog);
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet, analog);
    }
    {//0x30
   	int  wireless;
        id = (unsigned char)JT808_LOC_ADDITION_WIRELESS ;
	len = 1;
        table->GetIntValue((int)VDR_LOCS_WIRELESS_INTENSITY, wireless);
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, wireless);
    }
    {//0x30
   	int  gnss_num;
        id = (unsigned char)JT808_LOC_ADDITION_GNSS;
	len = 1;
        table->GetIntValue((int)VDR_LOCS_GNSS_SATELLITE_SUM, gnss_num);
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, gnss_num);
    }
#endif
}

void JT808MessageHelper::SendLocationQueryResp(unsigned short seq)
{
	JT808ParkingDbusNotifyC notify;
	notify.flag = 5; 
	g_pDbusServer->sendNotify(&notify);
	seq_bak = seq;
	return;
    VDRLocStatus1Table *table = new VDRLocStatus1Table();
    
    if(table->RefreshBottom() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendLocationQueryResp failed to RefreshBottom");
        delete table;
        return;
    }
    
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    thepacket->reply = true;
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushWord(packet, seq);
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendLocationQueryResp %d %d", seq, packet.size());

    
    PackLocation(table, packet);

    delete table;

    m_handler->SendBody(thepacket, JT808_PACKET_LOC_QUERY_REP, JT808_CRYTO_NONE);
}

void JT808MessageHelper::SendLocation(long long index)
{
	VDRLocStatus1Table *table = new VDRLocStatus1Table();

	int current_index;
	if(index==-1)
		current_index=table->RefreshBottom();
	else{
		current_index= table->RefreshNext(index);
	}
	if(current_index < 0) 
	{    
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendLocation failed to refresh table");
		delete table;
		return;
	} 

	StruJT808Packet *thepacket = m_handler->CreatePacket();
	JT808Packet &packet = thepacket->packet;
	thepacket->reply = true;

	PackLocation(table, packet);

	delete table;

	m_handler->SendBody(thepacket, JT808_PACKET_LOC_REPORT, JT808_CRYTO_NONE);
}

#if 1
void JT808MessageHelper::SendLocationMultiO(void )
{
	int i;
	for (int i=0; i < MAX_MANGQU; i++)
	{
		StruJT808Packet *thepacket = m_handler->CreatePacket();
		thepacket->reply =false;
		JT808Packet &packet = thepacket->packet;
		for(int tmp=0; tmp<mangqu_array[i]; tmp++)
		{
			m_handler->PushByte(packet, mangqu[i][tmp]);
		}
		m_handler->PushByteFront(packet, 1);
		m_handler->PushWordFront(packet, mangqu_array[i]/30);
		m_handler->SendBody(thepacket, JT808_PACKET_LOC_MULTI_REPORT, JT808_CRYTO_NONE);
	}
}
#endif

/**********************************/

void JT808MessageHelper::SetLocationPacket(JT808Packet &packet,struct LocStatusData &locData )
{
    int count = 28+14;
#ifdef NHB
    if(locData.alarm_signal& 0x01)
    {
        count+=4;
    }
#endif

    if(locData.alarm_signal& 0x02)
    {
        if(locData.overspeed_flag == 0)
        {
            count+= 3;
        }
        else
        {
            count+= 7;
        }
    }
    if(locData.alarm_signal& 0x300000)
    {
#ifdef NHB
        //id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        count+= 4;
#endif
        //id = (unsigned char)JT808_LOC_ADDITION_FENCE;
        count += 8;
    }
    else if(locData.alarm_signal& 0x800000){
#if 0
       // id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        count += 4;
       // m_handler->PushByte(packet, id);
      //  m_handler->PushByte(packet, len);
       // m_handler->PushWord(packet, locData.emergency_num);
#endif
    }

    if(locData.alarm_signal& 0x400000)
    {
       // id = (unsigned char)JT808_LOC_ADDITION_LINE_SHORT;
        count += 9;
        //m_handler->PushByte(packet, id);
        //m_handler->PushByte(packet, len);
        //m_handler->PushLong(packet, locData.fence_line_id);
       // m_handler->PushWord(packet, locData.fence_line_time);
       // m_handler->PushByte(packet, locData.fence_line_result);
    }
#ifdef NHB
    {//0x25
       // id = (unsigned char)JT808_LOC_ADDITION_STATE;
        count +=6;
       // m_handler->PushByte(packet, id);
       // m_handler->PushByte(packet, len);
       // m_handler->PushLong(packet,locData.vdr_status_new_hb);
    }
    {//2A
        if(locData.io_status == 2)
        {
        //    id = (unsigned char)JT808_LOC_ADDITION_IO ;
            count +=4;
          //  m_handler->PushByte(packet, id);
           // m_handler->PushByte(packet, len);
           // m_handler->PushWord(packet, locData.io_status);
        }
    }
    {//0x2B
       // id = (unsigned char)JT808_LOC_ADDITION_AD;
        count +=6;
        //m_handler->PushByte(packet, id);
       // m_handler->PushByte(packet, len);
       // m_handler->PushLong(packet, locData.analog_quantity);
    }
    {//0x30
       // id = (unsigned char)JT808_LOC_ADDITION_WIRELESS ;
        count +=3;
       // m_handler->PushByte(packet, id);
      //  m_handler->PushByte(packet, len);
      //  m_handler->PushByte(packet, locData.wireless_intensity);
    }
    {//0x30
       // int  gnss_num;
       // id = (unsigned char)JT808_LOC_ADDITION_GNSS;
        count +=3;
      //  m_handler->PushByte(packet, id);
       // m_handler->PushByte(packet, len);
      //  m_handler->PushByte(packet, locData.gnss_satellite_sum);
    }
 //   m_handler->PushLong(packet, locData.alarm_signal);
#endif
    m_handler->PushWord(packet, count);
    //---
    m_handler->PushLong(packet, locData.alarm_signal);
    m_handler->PushLong(packet, locData.status_falg);
    m_handler->PushLong(packet, locData.latitude);
    m_handler->PushLong(packet, locData.longitude);
    m_handler->PushWord(packet, (unsigned short)locData.altitude);
    m_handler->PushWord(packet, locData.gps_speed);
    m_handler->PushWord(packet, (unsigned short)locData.orientation);
    vector<unsigned char> bcdTime;
    GetTime((long long)locData.itime, bcdTime);
    m_handler->PushSequence(packet, bcdTime, 6);

    unsigned char id = (unsigned char)JT808_LOC_ADDITION_MILEAGE;
    unsigned char len = 4;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    m_handler->PushLong(packet, locData.total_mileage);

    id = (unsigned char)JT808_LOC_ADDITION_OILAGE;
    len = 2;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    m_handler->PushWord(packet, 0);

    id = (unsigned char)JT808_LOC_ADDITION_SPEED;
    len = 2;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    m_handler->PushWord(packet, locData.vdr_speed);

#ifdef NHB
    if(locData.alarm_signal& 0x01)
    {
        int alarmid = 0;
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, locData.emergency_num);
    }
#endif

    if(locData.alarm_signal& 0x02)
    {
        id = (unsigned char)JT808_LOC_ADDITION_OVERAPEED;
        m_handler->PushByte(packet, id);
        if(locData.overspeed_flag == 0)
        {
            len = 1;
            m_handler->PushByte(packet, len);
            m_handler->PushByte(packet, locData.overspeed_flag);
        }
        else
        {
            len = 5;
            m_handler->PushByte(packet, len);
            m_handler->PushByte(packet, locData.overspeed_flag);
            m_handler->PushLong(packet, locData.overspeed_id);
        }
    }


    if(locData.alarm_signal& 0x300000)
    {
#ifdef NHB
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, locData.emergency_num);
#endif

        id = (unsigned char)JT808_LOC_ADDITION_FENCE;
        len = 6;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, locData.fence_inout_alarm);
        m_handler->PushLong(packet, locData.fence_inout_id);
        m_handler->PushByte(packet, locData.fence_inout_direction);
    }else if(locData.alarm_signal& 0x800000){
#if 0
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, locData.emergency_num);
#endif
    }

    if(locData.alarm_signal& 0x400000)
    {
        id = (unsigned char)JT808_LOC_ADDITION_LINE_SHORT;
        len = 7;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet, locData.fence_line_id);
        m_handler->PushWord(packet, locData.fence_line_time);
        m_handler->PushByte(packet, locData.fence_line_result);
    }
#ifdef NHB
    {//0x25
        id = (unsigned char)JT808_LOC_ADDITION_STATE;
        len =4;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet,locData.vdr_status_new_hb);
    }
    {//2A
        if(locData.io_status == 2) {
            id = (unsigned char)JT808_LOC_ADDITION_IO ;
            len =2;
            m_handler->PushByte(packet, id);
            m_handler->PushByte(packet, len);
            m_handler->PushWord(packet, locData.io_status);
        }
    }
    {//0x2B
        id = (unsigned char)JT808_LOC_ADDITION_AD;
        len = 4;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet, locData.analog_quantity);
    }
    {//0x30
        id = (unsigned char)JT808_LOC_ADDITION_WIRELESS ;
        len = 1;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, locData.wireless_intensity);
    }
    {//0x30
        int  gnss_num;
        id = (unsigned char)JT808_LOC_ADDITION_GNSS;
        len = 1;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, locData.gnss_satellite_sum);
    }
  #endif
    printf( "JT808MessageHelper::SetLocationPacket*** = count %d  \n ",count );
}




/****************************** */




void JT808MessageHelper::SendLocationMulti(struct LocStatusData *locData,  int num, int lock_door)
{
	if(num <=0 ) return;
	int i=0, j = num/NUM_PACKAGE5,k,m;
	printf("total num is %d------%d---------\n", num, j);
	for(k=0; k<j; k++)
	{
        sleep(1);
		StruJT808Packet *thepacket = m_handler->CreatePacket();
		//thepacket->reply = false;
		thepacket->reply = true;
		JT808Packet &packet = thepacket->packet;
		for(m=0 ; m< NUM_PACKAGE5; m++)
		{
			//m_handler->PushWord(packet, 0x1c);
			if( i > num )
			{
                printf("SendLocationMulti error   times   m==%d ,i=%d  ",m,i);
                break;
			}
            printf("SendLocationMulti   times   m=%d i=%d  ",m,i);
            SetLocationPacket(packet,locData[i++]);
            
/*                  
                    m_handler->PushLong(packet, locData[i].alarm_signal);
			m_handler->PushLong(packet, locData[i].status_falg);
			m_handler->PushLong(packet, locData[i].latitude);
			m_handler->PushLong(packet, locData[i].longitude);
			m_handler->PushWord(packet, (unsigned short)locData[i].altitude);
			m_handler->PushWord(packet, locData[i].gps_speed);
			m_handler->PushWord(packet, (unsigned short)locData[i].orientation);
			vector<unsigned char> bcdTime;
			GetTime((long long)locData[i].itime, bcdTime);
			m_handler->PushSequence(packet, bcdTime, 6);
			bcdTime.clear();
*/
		}
		m_handler->PushByteFront(packet, 1);
		m_handler->PushWordFront(packet, NUM_PACKAGE5);
		m_handler->SendBody(thepacket, JT808_PACKET_LOC_MULTI_REPORT, JT808_CRYTO_NONE);
	}
	k = num % NUM_PACKAGE5;
	if(k != 0)
	{
        sleep(1);
        StruJT808Packet *thepacket = m_handler->CreatePacket();
       // thepacket->reply = false;
        JT808Packet &packet = thepacket->packet;
       thepacket->reply = true;
        for(m=0 ; m< k; m++)
        {
            //m_handler->PushWord(packet, 0x1c);
            if( i > num )
			{
                printf("SendLocationMulti error   times   m==%d ,i=%d  ",m,i);
                break;
			}
            printf("SendLocationMulti   times   m=%d i=%d  ",m,i);
            SetLocationPacket(packet,locData[i++]);/*
                        m_handler->PushLong(packet, locData[i].alarm_signal);
                        m_handler->PushLong(packet, locData[i].status_falg);
                        m_handler->PushLong(packet, locData[i].latitude);
                        m_handler->PushLong(packet, locData[i].longitude);
                        m_handler->PushWord(packet, (unsigned short)locData[i].altitude);
                        m_handler->PushWord(packet, locData[i].gps_speed);
                        m_handler->PushWord(packet, (unsigned short)locData[i].orientation);
                        vector<unsigned char> bcdTime;
                        GetTime((long long)locData[i].itime, bcdTime);
                        m_handler->PushSequence(packet, bcdTime, 6);
                        bcdTime.clear();*/
        }
        m_handler->PushByteFront(packet, 1);
        m_handler->PushWordFront(packet, k);
        m_handler->SendBody(thepacket, JT808_PACKET_LOC_MULTI_REPORT, JT808_CRYTO_NONE);
	}
}
void JT808MessageHelper::SendLocationbypass(struct LocStatusData &locData,int lock_door)
{
	static int flag_local=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = true;
	JT808Packet &packet = thepacket->packet;

    if(locData.type == 5)
    {
        m_handler->PushWord(packet, seq_bak);
    }
    m_handler->PushLong(packet, locData.alarm_signal);
#if 0
    if(lock_door != WY_MAGIC)
    {
	    if( lock_door == 1 )
		    locData.alarm_signal|= 0x80000000;
	    else
		    locData.alarm_signal&= ~(0x80000000);
    }
#endif
    m_handler->PushLong(packet, locData.status_falg);
    m_handler->PushLong(packet, locData.latitude);
    m_handler->PushLong(packet, locData.longitude);
    printf("locData.longitude=======%lf   \n ",locData.longitude);
    m_handler->PushWord(packet, (unsigned short)locData.altitude);
    m_handler->PushWord(packet, locData.gps_speed);
    m_handler->PushWord(packet, (unsigned short)locData.orientation);
    vector<unsigned char> bcdTime;
    GetTime((long long)locData.itime, bcdTime);
    m_handler->PushSequence(packet, bcdTime, 6);

    unsigned char id = (unsigned char)JT808_LOC_ADDITION_MILEAGE;
    unsigned char len = 4;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    m_handler->PushLong(packet, locData.total_mileage);
    
    id = (unsigned char)JT808_LOC_ADDITION_OILAGE;
    len = 2;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    m_handler->PushWord(packet, 0);

    id = (unsigned char)JT808_LOC_ADDITION_SPEED;
    len = 2;
    m_handler->PushByte(packet, id);
    m_handler->PushByte(packet, len);
    m_handler->PushWord(packet, locData.vdr_speed);

#ifdef NHB
    if(locData.alarm_signal& 0x01)
    {
        int alarmid = 0;
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, locData.emergency_num);
    }
#endif

    if(locData.alarm_signal& 0x02)
    {
        id = (unsigned char)JT808_LOC_ADDITION_OVERAPEED;
        m_handler->PushByte(packet, id);
        if(locData.overspeed_flag == 0)
        {
            len = 1;
            m_handler->PushByte(packet, len);
            m_handler->PushByte(packet, locData.overspeed_flag);
        }
        else
        {
            len = 5;
            m_handler->PushByte(packet, len);
            m_handler->PushByte(packet, locData.overspeed_flag);
            m_handler->PushLong(packet, locData.overspeed_id);
        }
    }


    if(locData.alarm_signal& 0x300000)
    {
#ifdef NHB
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, locData.emergency_num);
#endif

        id = (unsigned char)JT808_LOC_ADDITION_FENCE;
        len = 6;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, locData.fence_inout_alarm);
        m_handler->PushLong(packet, locData.fence_inout_id);
        m_handler->PushByte(packet, locData.fence_inout_direction);
    }else if(locData.alarm_signal& 0x800000){
#if 0
        id = (unsigned char)JT808_LOC_ADDITION_EMERGENCY_ALRM;
        len = 2;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushWord(packet, locData.emergency_num);
#endif
    }

    if(locData.alarm_signal& 0x400000)
    {
        id = (unsigned char)JT808_LOC_ADDITION_LINE_SHORT;
        len = 7;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet, locData.fence_line_id);
        m_handler->PushWord(packet, locData.fence_line_time);
        m_handler->PushByte(packet, locData.fence_line_result);
    }
#ifdef NHB
    {//0x25
        id = (unsigned char)JT808_LOC_ADDITION_STATE;
	len =4;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet,locData.vdr_status_new_hb);
    }
    {//2A
	    if(locData.io_status == 2) {
		    id = (unsigned char)JT808_LOC_ADDITION_IO ;
		    len =2;
		    m_handler->PushByte(packet, id);
		    m_handler->PushByte(packet, len);
		    m_handler->PushWord(packet, locData.io_status);
	    }
    }
    {//0x2B
        id = (unsigned char)JT808_LOC_ADDITION_AD;
	len = 4;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushLong(packet, locData.analog_quantity);
    }
    {//0x30
        id = (unsigned char)JT808_LOC_ADDITION_WIRELESS ;
	len = 1;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, locData.wireless_intensity);
    }
    {//0x30
   	int  gnss_num;
        id = (unsigned char)JT808_LOC_ADDITION_GNSS;
	len = 1;
        m_handler->PushByte(packet, id);
        m_handler->PushByte(packet, len);
        m_handler->PushByte(packet, locData.gnss_satellite_sum);
    }
#endif
    if(locData.type == 5)
	m_handler->SendBody(thepacket, JT808_PACKET_LOC_QUERY_REP, JT808_CRYTO_NONE);
    else
	m_handler->SendBody(thepacket, JT808_PACKET_LOC_REPORT, JT808_CRYTO_NONE);
}
void JT808MessageHelper::SendCarControl(unsigned short seq,int flag)
{
	VDRLocStatus1Table *table = new VDRLocStatus1Table();

	int current_index = table->RefreshBottom();
	if(current_index < 0) 
	{    
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendLocation failed to refresh table");
		delete table;
		return;
	} 

	StruJT808Packet *thepacket = m_handler->CreatePacket();
	JT808Packet &packet = thepacket->packet;

    	m_handler->PushWord(packet, seq);
    

	PackLocation(table, packet, flag);

	delete table;

	m_handler->SendBody(thepacket, JT808_PACKET_CAR_CONTROL_REP, JT808_CRYTO_NONE);
}

void JT808MessageHelper::HandleRequryTermParameters(JT808Packet &rec_packet, int body, int len, unsigned short seq)
{
    VDRRecordTable *rec_table;
    VDRCaptureTable *cap_table;
    StruMediaDataInfo info;
    string file_name;
    int gpstime;
    int accu_len = 0;
    map<int, int> idlist;
    
    if(len < 5)
    {
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp len invalid");
	    return;
    }

    int num = JT808_GET_BYTE(rec_packet, body);
    printf("*************** num = %d\n");
    body ++;
    accu_len ++;


    if(len - accu_len < 2 * num)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp len not enough");
        return;
    }
    
    idlist.clear();
    for(int i = 0; i < num; i ++)
    {
        int id = JT808_GET_LONG(rec_packet, body);
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp retx id %d", id);
        body += 4;
        idlist.insert(map<int, int>::value_type(id, 1));
    }

    if(idlist.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp no id needs to be retx");
        return;
    }
    

    SendParameters(seq, idlist);   
    
    return;
}
void JT808MessageHelper::SendAnswer(unsigned short seq, unsigned char answer)
{
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    thepacket->reply = true;
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushWord(packet, seq);
    m_handler->PushByte(packet, answer);

    m_handler->SendBody(thepacket, JT808_PACKET_ANSWER, JT808_CRYTO_NONE);
    
    return;
}

void JT808MessageHelper::SendEvents(unsigned char * events, unsigned char num)
{
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    
    for(int i=0;i<num;i++){
	    m_handler->PushByte(packet, events[i]);
    }

    m_handler->SendBody(thepacket, JT808_PACKET_EVENT_REPORT, JT808_CRYTO_NONE);
    
    return;
}

void JT808MessageHelper::SendElecBillInfo()
{
	static unsigned char dzyd[8]= {0xb5, 0xe7, 0xd7, 0xd3, 0xd4, 0xcb, 0xb5, 0xa5};
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	JT808Packet &packet = thepacket->packet;
	thepacket->reply = true;

	m_handler->PushLong(packet, 0x00000008);
	for(int i=0; i<8; i++){
		m_handler->PushByte(packet, dzyd[i]);
	}

	m_handler->SendBody(thepacket, JT808_PACKET_ELEC_BILL, JT808_CRYTO_NONE);

	return;
}
#ifdef NHB 
void JT808MessageHelper::SendDriverInfo(int timeIndex)
{
    DriversLoginTable *table = new DriversLoginTable();
    string name, id, cert, authority;
    int ic_in_out,ic_time,ic_result;

    vector<unsigned char> vtime, valtime;

    if(timeIndex !=0x8888 && timeIndex>0 && table->Refresh(timeIndex) < 0)
    {
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to refresh table");
	    delete table;
	    return;
    }else if(table->RefreshBottom() < 0)
    {
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to refresh table");
	    delete table;
	    return;
    }

    if(table->GetIntValue((int)VDR_DRV_OCCUR_TIME, ic_time) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver name");
        delete table;
        return;
    }

    if(table->GetIntValue((int)VDR_DRV_LOGIN_LOGOUT, ic_in_out) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver name");
        delete table;
        return;
    }
    
    if(ic_in_out == 0x00)
    {
    	    delete table;
	    if(timeIndex == 0x8888)
		    return;

    	    ic_in_out= 0x02;
	    StruJT808Packet *thepacket = m_handler->CreatePacket();
	    JT808Packet &packet = thepacket->packet;

	    m_handler->PushByte(packet, ic_in_out);
	    GetTime(ic_time, vtime);
	    m_handler->PushSequence(packet, vtime, 6); 

	    m_handler->SendBody(thepacket, JT808_PACKET_DRIVER_INFO, JT808_CRYTO_NONE);

	    return;
    }

    if(table->GetIntValue((int)VDR_DRV_IC_CARD_READ_RESULT, ic_result) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver name");
        delete table;
        return;
    }

    if(ic_result!= 0x00)
    {
    	    delete table;

	    StruJT808Packet *thepacket = m_handler->CreatePacket();
	    JT808Packet &packet = thepacket->packet;

	    m_handler->PushByte(packet, ic_in_out);
	    GetTime(ic_time, vtime);
	    m_handler->PushSequence(packet, vtime, 6); 
	    m_handler->PushByte(packet, ic_result);

	    m_handler->SendBody(thepacket, JT808_PACKET_DRIVER_INFO, JT808_CRYTO_NONE);

	    return;
    }

    if(table->GetStringValue((int)VDR_DRV_DRIVER_NAME, name) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver name");
        delete table;
        return;
    }

    if(table->GetStringValue((int)VDR_DRV_DRIVER_LICENSE_NUM, id) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver id");
        delete table;
        return;
    }
    
    if(table->GetStringValue((int)VDR_DRV_QUALIFICATION_NUM, cert) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver cert");
        delete table;
        return;
    }

    if(table->GetStringValue((int)VDR_DRV_CERTIFYING_AUTHORITY, authority) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver authority");
        delete table;
        return;
    }

    if(table->GetSequenceValue((int)VDR_DRV_VAILD_DATA, valtime) <0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get valid time");
        delete table;
        return;
    }
    
    delete table;
    
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    
    m_handler->PushByte(packet, ic_in_out);
    GetTime(ic_time, vtime);
    m_handler->PushSequence(packet, vtime, 6); 
    m_handler->PushByte(packet, ic_result);
    string utf8_name = boost::locale::conv::from_utf<char>(name, "GBK");
    m_handler->PushByte(packet, utf8_name.length());
    m_handler->PushString(packet,utf8_name);
    //m_handler->PushSequence(packet, id, 20);
    m_handler->PushSequence(packet, cert, 20);
    utf8_name = boost::locale::conv::from_utf<char>(authority, "GBK");
    m_handler->PushByte(packet, utf8_name.length());
    m_handler->PushString(packet, utf8_name);
    m_handler->PushSequence(packet, valtime, 4);

    m_handler->SendBody(thepacket, JT808_PACKET_DRIVER_INFO, JT808_CRYTO_NONE);
    
    return;
}
#else
void JT808MessageHelper::SendDriverInfo(int timeIndex)
{
    DriversLoginTable *table = new DriversLoginTable();
    string name, id, cert, authority;
    if(timeIndex>0 && table->Refresh(timeIndex) < 0)
    {
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to refresh table");
	    delete table;
	    return;
    }else if(table->RefreshBottom() < 0)
    {
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to refresh table");
	    delete table;
	    return;
    }
    
    if(table->GetStringValue((int)VDR_DRV_DRIVER_NAME, name) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver name");
        delete table;
        return;
    }

    if(table->GetStringValue((int)VDR_DRV_DRIVER_LICENSE_NUM, id) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver id");
        delete table;
        return;
    }
    
    if(table->GetStringValue((int)VDR_DRV_QUALIFICATION_NUM, cert) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver cert");
        delete table;
        return;
    }

    if(table->GetStringValue((int)VDR_DRV_CERTIFYING_AUTHORITY, authority) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendDriverInfo failed to get driver authority");
        delete table;
        return;
    }
    
    delete table;
    
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    
    string utf8_name = boost::locale::conv::to_utf<char>(name, JT808_CHAR_SET);
    m_handler->PushByte(packet, utf8_name.length());
    m_handler->PushString(packet,utf8_name);
    m_handler->PushSequence(packet, id, 20);
    m_handler->PushSequence(packet, cert, 40);
    m_handler->PushByte(packet, authority.length());
    m_handler->PushString(packet, authority);

    m_handler->SendBody(thepacket, JT808_PACKET_DRIVER_INFO, JT808_CRYTO_NONE);
    
    return;
}
#endif

void JT808MessageHelper::SendMedia8805(ENUM_JT808_MEDIA_TYPE type, int index,int flag, int id)
{
    StruMediaDataInfo info;
    int gpstime = 0;
    string file_name;
    
    info.index = id;
    info.type = type;
    
    printf("%d::current type is %d +++++%d++++++++++\n",__LINE__,type,index);
    switch(type)
    {
        case ENUM_JT808_MEDIA_VIDEO:
        {
            VDRCaptureTable  *table = new VDRCaptureTable();
            if(table->RefreshTop() < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMedia query capture table failed");
                delete table;
                return;
            }

            table->GetIntValue((int)VDR_REC_FILE_TIME, info.index);
            table->GetIntValue((int)VDR_CAP_EVENT, info.event);
            table->GetIntValue((int)VDR_CAP_CHANNEL_ID, info.chan);
            table->GetStringValue((int)VDR_CAP_FILE_PATH, file_name);
            table->GetIntValue((int)VDR_CAP_FILE_TYPE, info.format);
            if(info.format == (int)VDR_CAPTURE_FORMAT_JPG)
            {
                info.format = ENUM_JT808_FILE_FORMAT_JPG;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_TIF;
            }

            table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue((int)VDR_CAP_STATUS_FLAG, info.statusflag);
            table->GetIntValue((int)VDR_CAP_LATITUDE, info.latitude);
            table->GetIntValue((int)VDR_CAP_LONGITUDE, info.longitude);
            table->GetIntValue((int)VDR_CAP_ALTITUDE, info.altitude);
            table->GetIntValue((int)VDR_CAP_ORIENTATION, info.orient);
            table->GetIntValue((int)VDR_CAP_GPS_SPEED, info.gpsspeed);
            table->GetIntValue((int)VDR_CAP_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
    	    info.type = 0x00;
            
            delete table;
            break;
        }
        case ENUM_JT808_MEDIA_PHOTO:
        {
            VDRCaptureTable  *table = new VDRCaptureTable();
            if(table->Refresh(index) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMedia query capture table failed");
                delete table;
                return;
            }

            table->GetIntValue((int)VDR_CAP_EVENT, info.event);
            table->GetIntValue((int)VDR_CAP_CHANNEL_ID, info.chan);
            table->GetStringValue((int)VDR_CAP_FILE_PATH, file_name);
            table->GetIntValue((int)VDR_CAP_FILE_TYPE, info.format);
            if(info.format == (int)VDR_CAPTURE_FORMAT_JPG)
            {
                info.format = ENUM_JT808_FILE_FORMAT_JPG;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_TIF;
            }

            table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue((int)VDR_CAP_STATUS_FLAG, info.statusflag);
            table->GetIntValue((int)VDR_CAP_LATITUDE, info.latitude);
            table->GetIntValue((int)VDR_CAP_LONGITUDE, info.longitude);
            table->GetIntValue((int)VDR_CAP_ALTITUDE, info.altitude);
            table->GetIntValue((int)VDR_CAP_ORIENTATION, info.orient);
            table->GetIntValue((int)VDR_CAP_GPS_SPEED, info.gpsspeed);
            table->GetIntValue((int)VDR_CAP_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
            
            delete table;
            break;
        }
        case ENUM_JT808_MEDIA_AUDIO:
        {
            VDRRecordTable  *table = new VDRRecordTable();
            if(table->Refresh(index) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMedia query record table failed");
                delete table;
                return;
            }

            table->GetIntValue((int)VDR_REC_EVENT, info.event);
            info.chan = 0;
            table->GetStringValue((int)VDR_REC_FILE_PATH, file_name);
            table->GetIntValue((int)VDR_REC_FILE_TYPE, info.format);
            if(info.format == (int)VDR_RECORD_FORMAT_WAV)
            {
                info.format = ENUM_JT808_FILE_FORMAT_OTHER;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_OTHER;
            }

            table->GetIntValue((int)VDR_REC_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue((int)VDR_REC_STATUS_FLAG, info.statusflag);
            table->GetIntValue((int)VDR_REC_LATITUDE, info.latitude);
            table->GetIntValue((int)VDR_REC_LONGITUDE, info.longitude);
            table->GetIntValue((int)VDR_REC_ALTITUDE, info.altitude);
            table->GetIntValue((int)VDR_REC_ORIENTATION, info.orient);
            table->GetIntValue((int)VDR_REC_GPS_SPEED, info.gpsspeed);

            table->GetIntValue((int)VDR_REC_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
            
            delete table;
            break;
        }
        default:
            return;
    }
#if 0
    if(type == ENUM_JT808_MEDIA_PHOTO)
    {
    	printf("current type is %d ---------------\n",type);
	    StruJT808Packet *thepacket = m_handler->CreatePacket();
	    //thepacket->reply = true;
	    JT808Packet &packet = thepacket->packet;
#ifndef NHB
	    m_handler->PushLong(packet, 0x335D5D2C);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushByte(packet, 0x41);
	    m_handler->PushByte(packet, 0x01);
	    m_handler->SendBody(thepacket, JT808_PACKET_MEDIA_INFO, JT808_CRYTO_NONE);
#else
	    thepacket->reply = true;
	    m_handler->PushWord(packet, seq_bak);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushWord(packet, 0x01);
	    //m_handler->PushLong(packet, index);
	    m_handler->PushLong(packet, 0x02);
	    m_handler->SendBody(thepacket, JT808_PACKET_CAMERA_START_RSP, JT808_CRYTO_NONE);
#endif
    }
#endif
    if(flag == 1){
	    map<int, int> idlist;
	    idlist.clear();
	    SendMediaFile(info, file_name, idlist);
    }
}
void JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_TYPE type, int index,int flag, int stop_time )
{
    StruMediaDataInfo info;
    int gpstime = 0;
    string file_name;
    
    info.index = index;
    info.type = type;
    
    printf("%d::current type is %d +++++%d++++++++++\n",__LINE__,type,index);
    switch(type)
    {
        case ENUM_JT808_MEDIA_VIDEO:
        {
            VDRCaptureTable  *table = new VDRCaptureTable();
            if(table->RefreshTop() < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMedia query capture table failed");
                delete table;
                return;
            }

            table->GetIntValue((int)VDR_REC_FILE_TIME, info.index);
            table->GetIntValue((int)VDR_CAP_EVENT, info.event);
            table->GetIntValue((int)VDR_CAP_CHANNEL_ID, info.chan);
            table->GetStringValue((int)VDR_CAP_FILE_PATH, file_name);
            table->GetIntValue((int)VDR_CAP_FILE_TYPE, info.format);
            if(info.format == (int)VDR_CAPTURE_FORMAT_JPG)
            {
                info.format = ENUM_JT808_FILE_FORMAT_JPG;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_TIF;
            }

            table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue((int)VDR_CAP_STATUS_FLAG, info.statusflag);
            table->GetIntValue((int)VDR_CAP_LATITUDE, info.latitude);
            table->GetIntValue((int)VDR_CAP_LONGITUDE, info.longitude);
            table->GetIntValue((int)VDR_CAP_ALTITUDE, info.altitude);
            table->GetIntValue((int)VDR_CAP_ORIENTATION, info.orient);
            table->GetIntValue((int)VDR_CAP_GPS_SPEED, info.gpsspeed);
            table->GetIntValue((int)VDR_CAP_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
    	    info.type = 0x00;
            
            delete table;
            break;
        }
        case ENUM_JT808_MEDIA_PHOTO:
        {
            VDRCaptureTable  *table = new VDRCaptureTable();
            if(table->Refresh(index) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMedia query capture table failed");
                delete table;
                return;
            }

            table->GetIntValue((int)VDR_CAP_EVENT, info.event);
            table->GetIntValue((int)VDR_CAP_CHANNEL_ID, info.chan);
            table->GetStringValue((int)VDR_CAP_FILE_PATH, file_name);
            table->GetIntValue((int)VDR_CAP_FILE_TYPE, info.format);
            if(info.format == (int)VDR_CAPTURE_FORMAT_JPG)
            {
                info.format = ENUM_JT808_FILE_FORMAT_JPG;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_TIF;
            }
            table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue((int)VDR_CAP_STATUS_FLAG, info.statusflag);
            table->GetIntValue((int)VDR_CAP_LATITUDE, info.latitude);
            table->GetIntValue((int)VDR_CAP_LONGITUDE, info.longitude);
            table->GetIntValue((int)VDR_CAP_ALTITUDE, info.altitude);
            table->GetIntValue((int)VDR_CAP_ORIENTATION, info.orient);
            table->GetIntValue((int)VDR_CAP_GPS_SPEED, info.gpsspeed);
            table->GetIntValue((int)VDR_CAP_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
            
            delete table;
            break;
        }
        case ENUM_JT808_MEDIA_AUDIO:
        {
            VDRRecordTable  *table = new VDRRecordTable();
            if(table->Refresh(index) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMedia query record table failed");
                delete table;
                return;
            }

            table->GetIntValue((int)VDR_REC_EVENT, info.event);
            info.chan = 0;
            table->GetStringValue((int)VDR_REC_FILE_PATH, file_name);
            table->GetIntValue((int)VDR_REC_FILE_TYPE, info.format);
            if(info.format == (int)VDR_RECORD_FORMAT_WAV)
            {
                info.format = ENUM_JT808_FILE_FORMAT_OTHER;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_OTHER;
            }

            table->GetIntValue((int)VDR_REC_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue((int)VDR_REC_STATUS_FLAG, info.statusflag);
            table->GetIntValue((int)VDR_REC_LATITUDE, info.latitude);
            table->GetIntValue((int)VDR_REC_LONGITUDE, info.longitude);
            table->GetIntValue((int)VDR_REC_ALTITUDE, info.altitude);
            table->GetIntValue((int)VDR_REC_ORIENTATION, info.orient);
            table->GetIntValue((int)VDR_REC_GPS_SPEED, info.gpsspeed);

            table->GetIntValue((int)VDR_REC_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
            
            delete table;
            break;
        }
        default:
            return;
    }
    if(type == ENUM_JT808_MEDIA_PHOTO)
    {
    	printf("current type is %d ---------------\n",type);
	    StruJT808Packet *thepacket = m_handler->CreatePacket();
	    //thepacket->reply = true;
	    JT808Packet &packet = thepacket->packet;
#ifndef NHB
	    m_handler->PushLong(packet, 0x335D5D2C);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushByte(packet, 0x41);
	    m_handler->PushByte(packet, 0x01);
	    m_handler->SendBody(thepacket, JT808_PACKET_MEDIA_INFO, JT808_CRYTO_NONE);
#else
	    thepacket->reply = true;
	    m_handler->PushWord(packet, seq_bak);
	    m_handler->PushByte(packet, 0x00);
	    m_handler->PushWord(packet, 0x01);
	    //m_handler->PushLong(packet, index);
	    m_handler->PushLong(packet, 0x02);
	    m_handler->SendBody(thepacket, JT808_PACKET_CAMERA_START_RSP, JT808_CRYTO_NONE);
#endif
    }
    if(flag == 1){
	    map<int, int> idlist;
	    idlist.clear();
	    SendMediaFile(info, file_name, idlist);
    }
}

void JT808MessageHelper::SendTransmissionData(int type, int total, unsigned char *buf)
{
	int i;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	JT808Packet &packet = thepacket->packet;
	m_handler->PushByte(packet, type);
	for(i=0;i<total;i++)
	{
		m_handler->PushByte(packet, buf[i]);
		//printf("%c ",buf[i]);
		//if(i%16)
		//	printf("\n");
	}

	m_handler->SendBody(thepacket, JT808_PACKET_UP_TRANSPARENT_TRANSMISSION, JT808_CRYTO_NONE);

	return;
}

void JT808MessageHelper::SendCanData(int type, int total, unsigned char *buf)
{
	int i;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	JT808Packet &packet = thepacket->packet;
	for(i=0;i<total;i++)
		m_handler->PushByte(packet, buf[i]);

	m_handler->SendBody(thepacket, JT808_PACKET_CAN_INFO, JT808_CRYTO_NONE);

	return;
}

void JT808MessageHelper::CreateMediaFilePacket(JT808Packet &packet, StruMediaDataInfo &info, int buflen,int flag_first)
{
	if(1==flag_first){
		m_handler->PushLong(packet, info.index);
		m_handler->PushByte(packet, info.type);
		m_handler->PushByte(packet, info.format);
		m_handler->PushByte(packet, info.event);
		m_handler->PushByte(packet, 0x01);
		//m_handler->PushByte(packet, info.chan);
		m_handler->PushLong(packet, info.alarmflag);
		m_handler->PushLong(packet, info.statusflag);
		m_handler->PushLong(packet, info.latitude);
		m_handler->PushLong(packet, info.longitude);
		m_handler->PushWord(packet, (unsigned short)info.altitude);
		m_handler->PushWord(packet, info.gpsspeed);
		m_handler->PushWord(packet, (unsigned short)info.orient);
		m_handler->PushSequence(packet, info.gpstime, 6);
	}
	for(int i = 0; i < buflen; i ++)
	{
		m_handler->PushByte(packet, jt808_buf[i]);
	}
}

void JT808MessageHelper::SendMediaFile(StruMediaDataInfo &info, string &file_name, map<int, int> &idlist)
{
    int readlen = JT808_FILE_SEGMENT_SIZE;
    bool read_over = false;
    int count = 0;
    int fd = open(file_name.c_str(), O_RDONLY);
    if(fd < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile failed to open file %s.", file_name.c_str());
	perror("why file open failed:");
        return;
    }
    
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    thepacket->reply = false;
    while(!read_over)
    {
        int ret = read(fd, jt808_buf + (JT808_FILE_SEGMENT_SIZE - readlen), readlen);
        if(ret < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile failed to read file %s.", file_name.c_str());
            read_over = true;
            continue;
        }
        if(ret == 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile read end of file %s.", file_name.c_str());
            read_over = true;
            continue;
        }
        if(ret < readlen)
        {
            readlen -= ret;
            continue;
        }
        readlen = JT808_FILE_SEGMENT_SIZE;
        
        count ++;
        if((idlist.size() > 0) && (idlist.count(count) <= 0))
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::SendMediaFile this packet will be ignored %d.", count);
            continue;
        }
        
        thepacket->packetarray.push_back(JT808_NEW_PACKET);
        thepacket->segment_id.push_back(count);
        JT808Packet &packet = *thepacket->packetarray.back();
        CreateMediaFilePacket(packet, info, JT808_FILE_SEGMENT_SIZE,count);
    }
    
    close(fd);
    
    if(readlen != JT808_FILE_SEGMENT_SIZE)
    {
        count ++;
        thepacket->packetarray.push_back(JT808_NEW_PACKET);
        thepacket->segment_id.push_back(count);
        JT808Packet &packet = *thepacket->packetarray.back();
        CreateMediaFilePacket(packet, info, JT808_FILE_SEGMENT_SIZE - readlen,count);
    }
    
    if(thepacket->packetarray.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
        m_handler->DestroyPacket(thepacket);
        return;
    }

    thepacket->total_segment = count;
    m_handler->SendBody(thepacket, JT808_PACKET_MEDIA_DATA, JT808_CRYTO_NONE);
}

void JT808MessageHelper::HandleRegisterResult(int result, string &auth)
{
    if(result != (int)ENUM_REGISTER_SUCCESS)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult register failed(%d)", result);
    }
    else
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult register failed(%d, %s)", result, auth.c_str());
        VDRDeployParaTable *table = new VDRDeployParaTable();
        if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
            delete table;
            return;
        }
        table->Commit();
        delete table;
        m_handler->m_registered = true;
        SendAuth();
    }

    SendJT808RegisterNotify(result);
    
    return;
}
void JT808MessageHelper::SendUpdataResult(void)
{
	int result;
	int type;
	VDRUpdateTable *table=new VDRUpdateTable();
	if((table->RefreshBottom())<0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"get the data is failed!");
		delete table;
		return;
	}
	table->GetIntValue(VDR_UPDATE_RESULT,result);
	table->GetIntValue(VDR_UPDATE_TYPE,type);
	printf("result is:%d\n",result);
	printf("type is :%d\n",type);
	if(((result >> 4) & 0x0F)==1)
	{
		StruJT808Packet *thepacket=m_handler->CreatePacket();
		thepacket->reply= true;
		JT808Packet &packet=thepacket->packet;
		result=result & 0x0F;
		m_handler->PushByte(packet,type);
		m_handler->PushByte(packet,result);
		m_handler->SendBody(thepacket,JT808_PACKET_UPDATE_TERM_REP,JT808_CRYTO_NONE);
		table->SetIntValue(VDR_UPDATE_RESULT,result);
		table->Commit();
	}
	delete table;
	return;
}
void JT808MessageHelper::SendUpgradResult(void)
{
	unsigned char buff[10]={0};
	unsigned char buff_w[10]={0};
	buff_w[0]=3;
	int fd;
	int n;
	int res;
	if((fd=open("update.file",O_RDWR|O_CREAT))<0)
	{
		printf("open file faild!\n");
		return ;
	}
	if((n=read(fd,buff,1))==-1)
	{
	       	printf("read data faile!\n");
		close(fd);
		return ;
	}
	printf("+++++++++++n is %d\n++++++++++",n);
	close(fd);
	if(buff[0]==1||buff[0]==0)
	{
		StruJT808Packet *thepacket =  m_handler ->CreatePacket();
		thepacket->reply = true;
		JT808Packet &packet = thepacket->packet;
		m_handler->PushByte(packet,0);
		m_handler->PushByte(packet,buff[0]);
		m_handler->SendBody(thepacket,JT808_PACKET_UPDATE_TERM_REP,JT808_CRYTO_NONE);
		int fp;
		int re;
		if((fp=open("update.file",O_RDWR|O_CREAT))<0)
		{
			printf("open the file faild!\n");
			return ;
		}
		if((re=write(fp,buff_w,1))!=1)
		{
			printf("set data faild!\n");
			close(fp);
			return;
		}
		close(fp);
	}
#if 0
	VDRUpdateTable *table =new VDRUpdateTable();
	if((table->RefreshBottom())<0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"get the data is failed!");
		delete table;
		return ;
	}
	table->GetIntValue(VDR_UPDATE_RESULT,result);
	table->GetIntValue(VDR_UPDATE_TYPE,type);
	StruJT808Packet *thepacket =  m_handler ->CreatePacket();
	thepacket->reply = true;
	JT808Packet &packet = thepacket->packet;
	m_handler->PushByte(packet,type);
	m_handler->PushByte(packet,result);
	m_handler->SendBody(thepacket,JT808_PACKET_UPDATE_TERM_REP,JT808_CRYTO_NONE);
	if((table->DeleteAll())<0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"delete update table failed!");
		delete table;
		return ;
	}
	delete table;
#endif
	return;
}
void JT808MessageHelper::HandleCommonReply(int id, int result)
{
    switch(id)
    {
        case JT808_PACKET_ID_LOGOUT:
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCommonReply get logout reply(0x%04x, %d)", id, result);
	    {
    		string auth = "";
		    //ShutdownMachineReqC srequest;
		    DBusReplyC  *reply=NULL;
		    int ret;

		    JT808LogoffDbusNotifyC notify;
		    notify.result=result;
            g_pDbusServer->sendNotify(&notify);
            if(result == 0)
		    {
			    VDRDeployParaTable *table = new VDRDeployParaTable();
			    if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
			    {    
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
				    delete table;
				    return;
			    }    
			    table->Commit();
			    delete table;
			    system("rm /nand3/tables/monitor.db -fr");
	    		system("cp /nand2/tables/vdrsetting.db.bak /nand2/tables/vdrsetting.db");
			    ConnectionManager::Instance()->Stop();
			    /*
			    ret = g_pDbusClient->sendService(&srequest,reply,5*1000);
			    if(ret < 0)
			    {
				    printf("Send Service Error. Error Code: %d\n",ret);
			    }
			    */
		    }
	    }
	    break;
        case JT808_PACKET_ID_REGISTER:
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCommonReply get logout reply(0x%04x, %d)", id, result);
	    {
		    JT808RegisterDbusNotifyC notify;
		    notify.result= result;
		    g_pDbusServer->sendNotify(&notify);
	    }
	    break;
        case JT808_PACKET_ID_QUICK_REGISTER:
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCommonReply get logout reply(0x%04x, %d)", id, result);
	    {
		    if(register_flag)
		    {
		    	unsigned char buff[]="123"; //目前不清楚电话号码用的是哪个	
			//SendRegister(3,buff);
			string auth="";
			VDRDeployParaTable *table = new VDRDeployParaTable();
			if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
				delete table;
				return;
			}
			table->Commit();
			delete table;
			ConfigManager::m_control = 1;
			register_flag=0;
			m_handler->quick_register = 0x88;
	    		ConnectionManager::Instance()->Reconnect();
		    }
		    else
		    {
		    	JT808RegisterDbusNotifyC notify;
		    	if((result == 0x00) || (result == 0x05))
		    	{
			    string auth="xunye";
			    VDRDeployParaTable *table = new VDRDeployParaTable();
			    if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
				    delete table;
				    return;
			    }
			    table->Commit();
			    delete table;
			    notify.result=0x0A;
		    	}
		    	else 
		    		notify.result= result;
		    	g_pDbusServer->sendNotify(&notify);
		    
		    }
	    }
	    break;
        case JT808_PACKET_ID_AUTH:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCommonReply get authorize reply(0x%04x, %d)", id, result);
            if(result == (int)ENUM_JT808_COMMON_OK)
            {
            	SendDriverInfo(0);
                m_handler->m_authorized = true;
                if(ConfigManager::GetHelloFlag())
                {
			//first send one item
			m_handler->SendHello();
			StartTimer(m_handler->m_helloTimer);
                }
		SendJT808AuthenticationNotify(0);
		//SendDriverInfo(0x8888);
		//SendElecBillInfo();
            }
            else
            {
		    SendJT808AuthenticationNotify(1);
                //ConnectionManager::Instance()->Reconnect();
            }
	    SendUpdataResult();
	    SendUpgradResult();
            break;
        case JT808_PACKET_LOC_REPORT:
            SendJT808LocReportNotify(result);
            break;
        case JT808_PACKET_ID_HELLO:
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleCommonReply restart hello.");
            //StartTimer(m_handler->m_helloTimer);
            break;
        default:
            break;
    }
    return;
}

void JT808MessageHelper::HandlePacketResendRequest(JT808Packet &rec_packet, int body, int len, unsigned short seq)
{
    VDRRecordTable *rec_table;
    VDRCaptureTable *cap_table;
    StruMediaDataInfo info;
    string file_name;
    int gpstime;
    unsigned short rseq;
    int accu_len = 0;
    map<int, int> idlist;
    
    if(len < 2)
    {
    	    SendAck(seq, (unsigned short)JT808_PACKET_RESEND_REQUEST, (int)ENUM_JT808_COMMON_ERROR);
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp len invalid");
	    return;
    }

    rseq = JT808_GET_SHORT(rec_packet, body);
    body += 2;
    accu_len += 2;
    int num = JT808_GET_BYTE(rec_packet, body);
    body ++;
    accu_len ++;

    printf("HandlePacketMediaDataRsp len is %d accu_len is %d num is %d \n", len, accu_len, num);

    if(len - accu_len < 2 * num)
    {
    	SendAck(seq, (unsigned short)JT808_PACKET_RESEND_REQUEST, (int)ENUM_JT808_COMMON_ERROR);
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp len not enough");
        return;
    }
    
    idlist.clear();
    for(int i = 0; i < num; i ++)
    {
        int id = JT808_GET_SHORT(rec_packet, body);
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp retx id %d", id);
        body += 2;
        idlist.insert(map<int, int>::value_type(id, 1));
    }

    SendAck(seq, (unsigned short)JT808_PACKET_RESEND_REQUEST, (int)ENUM_JT808_COMMON_OK);
    if(idlist.size() <= 0)
    {
	    //m_handler-> PopQueue(JT808_PACKET_DATA_SEND, rseq);
	    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp no id needs to be retx");
	    return;
    }else{
    	    m_handler->RSendMessage( rseq, idlist);
    }
    
    //SendMediaFile(info, file_name, idlist);
    
    return;
}

void JT808MessageHelper::HandleSetParameters(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int accu_len = 0;
    int flag_gnss=0,gnss_mode=1,gnss_baud=1,gnss_out=0,gnss_fre=0, gnss_upmode = 0, gnss_value = 0;
    StruJT808Para para ;
    JT808ParkingDbusNotifyC notify;
    notify.flag = 0;
    int cstc=0;
    int rec =0;
    
    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetParameters len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_SET_PARAMETERS, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    int count = JT808_GET_BYTE(packet, body);
    accu_len ++;
    body ++;
    
    VDRRunParaTable *table = new VDRRunParaTable();
    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
    VDRAccelerationTable *accelerationtable = new VDRAccelerationTable();

    for(int i = 0; i < count; i ++)
    {
        if((len - accu_len) < 6)
        {
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetParameters len not enough");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_PARAMETERS, (int)ENUM_JT808_COMMON_ERROR);
		delete table;
		delete deploytable;
		return;
	}

	int id = JT808_GET_LONG(packet, body);
	body += 4;
	int paralen = JT808_GET_BYTE(packet, body);
	body ++;
	accu_len += 5;
	if((len - accu_len) < paralen)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetParameters paralen not enough");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_PARAMETERS, (int)ENUM_JT808_COMMON_ERROR);
		delete table;
		delete deploytable;
		return;
	}

	if(JT808Paras.count(id) <= 0) 
	{	
		if(JT808ReadOnlyParas.count(id) <= 0){
			LogUtility::Log(LOG_LEVEL_WARN, "JT808MessageHelper::HandleSetParameters unknown ID(%04x)", id);
			body += len;
			accu_len += len;
			continue;
		}else
			para = JT808ReadOnlyParas[id];

	}else
		para = JT808Paras[id];
    
    

#ifdef HBTEST
	if(id == 0x005A){		 	
		if(cstc == 0)
			notify.flag = 1;
		else if(cstc ==1)
			notify.flag = 4;
	}else if(id == 0x0013){		 	
		cstc=1;
		continue;
	}else if(id == 0x0057){
		notify.flag = 2;
	}else if(id == 0x0111){
		notify.flag = 3;
	}else if(id == 0x0017){
		continue;
	}
#endif
	if(id == 0x0081 || id == 0x0082 || id == 0x0083 || id == 0x0084) {
		rec =1;
	}

	if(id == 0x18 || id == 0x19)
		rec = 2;

	if(para.type == ENUM_SQLite_COLUMN_INTEGER)
	{
		int value;
		if(para.len != paralen)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetParameters paralen not match");
			SendAck(seq, (unsigned short)JT808_PACKET_SET_PARAMETERS, (int)ENUM_JT808_COMMON_ERROR);
			delete table;
			delete deploytable;
			return;
		}
		if(para.len == 1)
		{
			value = JT808_GET_BYTE(packet, body);
		}
		else if(para.len == 2)
		{
			value = JT808_GET_SHORT(packet, body);
		}
		else
		{
			value = JT808_GET_LONG(packet, body);
		}

		body += paralen;
		accu_len += paralen;
		if(id == 0x5D) 
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "value is %d", value);
    			int value1= value&0xff;
    			int value2= (value>>8) & 0xff;
			accelerationtable->SetIntValue(VDR_ACCELERATION_COLLIDE_TIME, value1);
			accelerationtable->SetIntValue(VDR_ACCELERATION_COLLIDE_MIN, value2);
			table->SetIntValue(VDR_PARA_COLLISION_ALARM, value);
		}else if(id == 0x5E)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "rollover value is %d", value);
			accelerationtable->SetIntValue(VDR_ACCELERATION_SIDE_TURN_ANGLE, value);
			table->SetIntValue(VDR_PARA_ROLLOVER_ALARM, value);
		}else if(id<0x80 || id>=0x90)
			table->SetIntValue(para.colid, value);
		else
			deploytable->SetIntValue(para.colid, value);

		switch(id){
		case 0x90:
			gnss_mode = value;
			if(BEGIN_GNSS == gnss_mode)
				flag_gnss=1;
			else 
				flag_gnss=2;
                ConfigManager::m_iGNSSMode = value;
			break;
		case 0x91:
			gnss_baud = value;
                ConfigManager::m_iGNSSBaud = value;
			break;
		case 0x92:
			gnss_out = value;
                ConfigManager::m_iGNSSOut = value;
			break;
		case 0x93:
			gnss_fre = value;
                ConfigManager::m_iGNSSFre = value;
			break;
		case 0x94:
			gnss_upmode = value;
                ConfigManager::m_iGNSSUpLoadMode = value;
			break;
		case 0x95:
			gnss_value = value;
                ConfigManager::m_iGNSSUpLoadSet = value;
			break;
		default:
			break;
		}
	}
	else if(para.type == ENUM_SQLite_COLUMN_STRING)
	{
		string value = "";
		for(int j = 0; j < paralen; j ++)
		{
			value.push_back(JT808_GET_BYTE(packet, body));
			body ++;
		}

    		string utf_value = boost::locale::conv::to_utf<char>(value, JT808_CHAR_SET);
		accu_len += paralen;

		if(id<0x80||id>=0x90)
			table->SetStringValue(para.colid, utf_value);
		//else if(id != 0x13 && id != 0x17)
		else 
			deploytable->SetStringValue(para.colid, utf_value);
	}
	else if(para.type == ENUM_SQLite_COLUMN_SEQUENCE)
	{
		SQLiteSequence value;
		for(int j = 0; j < paralen; j ++)
		{
			value.push_back(JT808_GET_BYTE(packet, body));
			body ++;
		}

		accu_len += paralen;

		if(id<0x80||id>=0x90)
			table->SetSequenceValue(para.colid, value);
		else 
			deploytable->SetSequenceValue(para.colid, value);
	}
	else
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetParameters para type not support");
		body += paralen;
		accu_len += paralen;
		continue;
	}
    }
    
    table->Commit();
    deploytable->Commit();
    accelerationtable->Commit();
    delete table;
    delete deploytable;
    delete accelerationtable;
    SendAck(seq, (unsigned short)JT808_PACKET_SET_PARAMETERS, (int)ENUM_JT808_COMMON_OK);
    //ConfigManager::Refresh();
    
    m_handler->RefreshConfig();
    g_JT808GnssManager.SetGnssUp( gnss_upmode, gnss_value );
    SendJT808SettingUpdateNotify();
    if(flag_gnss == 1 )
    {
	    printf("begin gnss mode is %d- %d -%d- %d ======================\n",gnss_upmode,gnss_baud,gnss_out,gnss_fre);
	    SendJT808GnssUpdateNotify(gnss_mode,gnss_baud,gnss_out,gnss_fre);
    }else if(flag_gnss == 2 )
    {
	    printf("stop gnss mode is %d- %d -%d- %d ======================\n",gnss_mode,gnss_baud,gnss_out,gnss_fre);
	    SendJT808GnssUpdateNotify(gnss_mode,gnss_baud,gnss_out,gnss_fre);
    }
    #ifdef HBTEST
    g_pDbusServer->sendNotify(&notify);
    #endif
    if(rec >= 1)
    {
	    sleep(3);
	    if(rec == 1){
		    //如果是修改注册参数，则清空鉴权码
		    m_handler->m_registered = false;
		    m_handler->m_authorized = false;
		    string auth="";
		    VDRDeployParaTable *table = new VDRDeployParaTable();
		    if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
		    {
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
			    delete table;
			    return;
		    }
		    table->Commit();
		    delete table;

	    }
	    ConnectionManager::Instance()->Reconnect();
    }
    return;
}

void JT808MessageHelper::HandleTermControl(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int accu_len = 0;
    int ret = -1;
    ShutdownMachineReqC srequest;
    RebootMachineReqC   rrequest;
    DBusReplyC  *reply=NULL;
    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    int cmd = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    string phonenumber = "";
    
    switch(cmd)
    {
        case ENUM_JT808_TERM_CONTROL_UPGRADE:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd upgrade");
	    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	//    SendJT808CallBackNotify( 2, phonenumber);
            break;
        case ENUM_JT808_TERM_CONTROL_SHUTDOWN:
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd shutdown");
	    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	    ConnectionManager::Instance()->Close();
	    sleep(5);
	    ret = g_pDbusClient->sendService(&srequest,reply,5*1000);
	    if(ret < 0)
	    {
		    printf("Send Service Error. Error Code: %d\n",ret);
	    }
            return;
        case ENUM_JT808_TERM_CONTROL_RESET:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd reset");
            SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	    sleep(5);
	    ret = g_pDbusClient->sendService(&rrequest,reply,5*1000);
	    if(ret < 0)
	    {
		    printf("Send Service Error. Error Code: %d\n",ret);
	    }
            return;
        case ENUM_JT808_TERM_CONTROL_RESET_FACTORY:
	    {
            SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	    sleep(3);
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd reset factory");
	    system("cp /nand2/tables/vdrsetting.db.bak /nand2/tables/vdrsetting.db");
	    SendJT808CallBackNotify( 4, phonenumber);
        g_JT808GnssManager.StopGnssUp();
	    ConfigManager::Refresh();
	    m_handler->ResetFactory();
	    SendJT808SettingUpdateNotify();
	    JT808ParkingDbusNotifyC notify;
	    notify.flag = 6;
	    g_pDbusServer->sendNotify(&notify);
	    ConnectionManager::Instance()->Close();
            return;
	    }
        case ENUM_JT808_TERM_CONTROL_CLOSE_DATA:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd close data");
            SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	    sleep(2);
	    ConnectionManager::Instance()->Stop();
	    SendJT808CallBackNotify( 2, phonenumber);
	    sleep(1);
	    system("./ppp-off ");
	    system("killall ppp-on");
	    //system("ifconfig eth0 down");
	    printf("ifconfig eth0 down");
	    StartTimer(m_handler->m_Timer_Deadzone);
	    ConnectionManager::Instance()->b_zone = true;
            return;
        case ENUM_JT808_TERM_CONTROL_CLOSE_NETWORK:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd close data & phone");
            SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	    sleep(2);
	    ConnectionManager::Instance()->Stop();
	    SendJT808CallBackNotify( 2, phonenumber);
	    sleep(1);
	    system("./ppp-off ");
	    system("killall ppp-on ");
	    //system("ifconfig eth0 down");
	    printf("ifconfig eth0 down");
	    StartTimer(m_handler->m_Timer_Deadzone);
	    ConnectionManager::Instance()->b_zone = true;
            return;
        case ENUM_JT808_TERM_CONTROL_SERVER:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd server");
            break;
        default:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl cmd invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_NOT_SUPPORT);
            return;
    }

    if((len - accu_len) < 9)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
        SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    if(cmd == ENUM_JT808_TERM_CONTROL_UPGRADE){
	    int tcpport = 0, udpport = 0, timeout = 0;
	    vector<char> mfrs;
	  //  body ++;
	    string results[ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_BUTT];
	    for(int i = 0; i < ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_BUTT; i ++)
	    {
		    if(i == ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_TCPPORT)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    tcpport = JT808_GET_SHORT(packet, body);
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleTermControl tcp port is %d", tcpport);
			    body += 2;
			    accu_len += 2;	
		    }
		    else if(i == ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_UDPPORT)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    udpport = JT808_GET_SHORT(packet, body);
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleTermControl udp port is %d", udpport);
			    body += 2;
			    accu_len += 2;	
		    }
		    else if(i == ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_TIME)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    timeout = JT808_GET_SHORT(packet, body);
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleTermControl timeout is %d", timeout);
			    body += 2;
			    accu_len += 2;
		    }
		    else if(i == ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_MFRS)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    mfrs.clear();
			    for(int j=0; j < 5;j++){
				    mfrs.push_back(JT808_GET_BYTE(packet, body));
				    body += 1;
				    accu_len += 1;
			    }
		    }
		    else
		    {
			    results[i] = ""; 
			    while((len > accu_len) && (JT808_GET_BYTE(packet, body) != ';'))
			    {
				    results[i].push_back(JT808_GET_BYTE(packet, body));
				    body ++;
				    accu_len ++;
			    }
			    	
			    if(len <= accu_len)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl no enough item");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
		    }

		    body ++;
		    accu_len ++;
	    }
    	    VDRDeployParaTable *table = new VDRDeployParaTable();
	    vector<unsigned char>manuID;
	    manuID.clear();
	    if(table->GetSequenceValue((int)VDR_DEPLOY_PARA_MANUFACTORY_ID,manuID)<0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::get data faild");
		    delete table;
		    return ;
	    }
	    char buff1[16]={0};
	    for(int i=0;i<manuID.size();i++)
	    {
	    	buff1[i]=manuID[i];
	    }
	    string manu_ID(buff1);

	    char buff2[16]={0};
	    for(int j=0;j<mfrs.size();j++)
	    {
	   	buff2[j]=mfrs[j]; 
	    }
	    string Mfrs(buff2);
            printf("DB manuID is:%s\n",manu_ID.c_str()) ;
	    printf("cmd Mfrs is:%s\n",Mfrs.c_str());
            printf("size is:%d\n",mfrs.size());	    
	    printf("version is:%s\n",results[ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_FIRM].c_str());
	    printf("adress is :%s\n",results[ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_ADDRESS].c_str());
	    if(strcmp(manu_ID.c_str(),Mfrs.c_str())==0)
	    {
	  	if(table->SetStringValue((int)VDR_DEPLOY_PARA_VERSION, results[ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_FIRM])<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::Update table vsersion failed");
			delete table;
			return;
		}  
		table->Commit();
#if 0
		VDRUpdateTable *up_table1 = new VDRUpdateTable();
		if(up_table1->SetIntValue((int)VDR_UPDATE_RESULT,0)<0)
		{    
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::Update Set result failed");
			delete up_table1;
			return;
		}     
		if(up_table1->SetIntValue((int)VDR_UPDATE_TYPE,0)<0)
		{    
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::Update Set type failed");
			delete up_table1;
			return ;
		}   		
		up_table1->Commit();
#endif	
		sleep(3);
		delete table;
		int fp;
		if((fp=open("update.file",O_RDWR|O_CREAT))<0)
		{
			printf("faile!\n");
			return ;
		}
		unsigned char buff_w1[10]={0};
		int res;
		buff_w1[0]=0;
		if((res=write(fp,buff_w1,1))!=1)
		{
			 printf("write data faile!\n");
			 close(fp);
			 return;
		}
		close(fp);		
		sleep(3);
		system("mount -rw -o /application");
		system("cp /application/data/version.ini.bak /application/data/version.ini");
		SendJT808CallBackNotify( 5, phonenumber);         //重启
		return ;
	    }
#if 0 
	    VDRUpdateTable *up_table2 = new VDRUpdateTable();
	    if(up_table2->SetIntValue((int)VDR_UPDATE_RESULT,1)<0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::Update Set result failed");
		    delete up_table2;
		    return ;												            
	    } 
	    if(up_table2->SetIntValue((int)VDR_UPDATE_TYPE,0)<0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::Update Set type failed");
		    delete up_table2;
		    return ;
	    }
	    up_table2->Commit();
#endif
	    int fd;
	    if((fd=open("update.file",O_RDWR|O_CREAT))<0)
	    {
	    	printf("faile!\n");
		return ;
	    }	    
	    unsigned char buff_w2[10]={0};
	    int re;
	    buff_w2[0]=1;
	    if((re=write(fd,buff_w2,1))!=1)
	    {
		    printf("write data faile!\n");
		    close(fd);
		    return ;
	    }
	    close(fd);
	    sleep(3);
	    SendJT808CallBackNotify( 5, phonenumber);	  //重启 
    }else if(cmd == ENUM_JT808_TERM_CONTROL_SERVER){
	    int control = 1;
	    int tcpport = 0, udpport = 0, timeout = 0;
	    body ++;
	    string results[ENUM_JT808_TERM_CONTROL_SERVER_ITEM_BUTT];
	    for(int i = 0; i < ENUM_JT808_TERM_CONTROL_SERVER_ITEM_BUTT; i ++)
	    {
		    if(i == ENUM_JT808_TERM_CONTROL_SERVER_ITEM_CONTROL)
		    {
			    if((len - accu_len) < 1)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    control = JT808_GET_BYTE(packet, body);
			    body ++;
			    accu_len ++;
			    if(control)
			    {
				    break;
			    }
		    }
		    else if(i == ENUM_JT808_TERM_CONTROL_SERVER_ITEM_TCPPORT)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    tcpport = JT808_GET_SHORT(packet, body);
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleTermControl tcp port is %d", tcpport);
			    body += 2;
			    accu_len += 2;
		    }
		    else if(i == ENUM_JT808_TERM_CONTROL_SERVER_ITEM_UDPPORT)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    udpport = JT808_GET_SHORT(packet, body);
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleTermControl udp port is %d", udpport);
			    body += 2;
			    accu_len += 2;
		    }
		    else if(i == ENUM_JT808_TERM_CONTROL_SERVER_ITEM_TIME)
		    {
			    if((len - accu_len) < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    timeout = JT808_GET_SHORT(packet, body);
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleTermControl timeout is %d", timeout);
			    body += 2;
			    accu_len += 2;
		    }
		    else
		    {
			    results[i] = "";
			    while((len > accu_len) && (JT808_GET_BYTE(packet, body) != ';'))
			    {
				    results[i].push_back(JT808_GET_BYTE(packet, body));
				    body ++;
				    accu_len ++;
			    }

			    if(len <= accu_len)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl no enough item");
				    SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
		    }

		    body ++;
		    accu_len ++;
	    }

	    ConfigManager::SetControl(results[ENUM_JT808_TERM_CONTROL_SERVER_ITEM_AUTH],results[ENUM_JT808_TERM_CONTROL_SERVER_ITEM_ADDRESS], tcpport, timeout, control);
    }
    
    //SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
    
    return;
}
void JT808MessageHelper::HandleTermUpdate(JT808Packet &packet, int body,int len, unsigned short seq, unsigned short sum, unsigned short packetNum)
{
	int accu_len =0;
	int ret = -1;
 	RebootMachineReqC rrequest;
	DBusReplyC *reply=NULL;

	if(len<1)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::HandleTermUpdatedate len invalid");
		SendAck(seq, (unsigned short) JT808_PACKET_UPDATE_TERM,(int)ENUM_JT808_COMMON_ERROR);
		return;
	}

	if(packetNum==0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::HandleTermUpdatedate fail data");
//		SendAck(seq, (unsigned short) JT808_PACKET_UPDATE_TERM,(int)ENUM_JT808_COMMON_ERROR);
		StruJT808Packet *thepacket=m_handler->CreatePacket();
		thepacket->reply= true;
		JT808Packet &packet1=thepacket->packet;
		update_type = JT808_GET_BYTE(packet,body);
		m_handler->PushByte(packet1,update_type);
		m_handler->PushByte(packet1,1);
		m_handler->SendBody(thepacket,JT808_PACKET_UPDATE_TERM_REP,JT808_CRYTO_NONE);
		return;
	}
	if(packetNum==1)
	{
		unReceivedNum.clear();
		unReceivedNum.resize(sum+1, 1);
		unReceivedNum[0] = 0;
		mpmap.clear();
		update_install_time = time(NULL);
		update_type = JT808_GET_BYTE(packet,body);
		body++;
		accu_len++;

		update_manuID.clear();
		for(int i=0;i<5;i++)
		{
			update_manuID.push_back((unsigned char) JT808_GET_BYTE(packet,body));
			body++;
			accu_len++;
		}
		update_version_len = JT808_GET_BYTE(packet,body);
		body++;

	//	string version(packet[body],packet[body+update_version_len-1]);
		char s1[update_version_len];
		for(int i=0;i<=update_version_len;i++)
		{
			s1[i]=packet[body+i];
		}
		string s2(s1);
		update_version=s2;
		cout<<"version is:"<<endl;
		cout<<update_version<<endl;
	//	update_version=version;
		
		body +=update_version_len;
		
		update_length = JT808_GET_LONG(packet,body);				
		
		body++;
		body++;
		accu_len++;
		accu_len++;
		//		string update_packet(packet[body],packet[body+length-1]);
		//rember first packet seq
		first_seq=seq;	

		tmpv.clear();
		for(int i=12+11+update_version_len+4+1;i<packet.size()-1-1;i++)
		{					
			tmpv.push_back(packet[i]);
		}
		mpmap.insert(make_pair(packetNum, tmpv));
	}else{
	
		tmpv.clear();
		for(int i=16+1;i<packet.size()-1-1;i++)
		{					
			tmpv.push_back(packet[i]);
		}
		mpmap.insert(make_pair(packetNum, tmpv));
	
	}

	unReceivedNum[packetNum] = 0;

	if(count_write==0)
	{
#if 0
		updatepathname = JT808_UPDATE_PATH+numToString<int>(update_install_time);
		ofstream updatefile(updatepathname.c_str());
		for(int i=12+11+update_version_len+4+1;i<packet.size()-1-1;i++)
		{					
			updatefile<<packet[i];
		}
		updatefile.close();
#endif
	}
	if(count_write>0)
	{
#if 0
		ofstream file;
		file.open(updatepathname.c_str(),ios::app);
		for(int i=16+1;i<packet.size()-1-1;i++)
		{
			file<<packet[i];
				printf("%02x ",packet[i]);									
				if((i + 1)%16 ==0)											
				{	
					printf("\r\n");											
				}	
		}
		file.close();
#endif
	}
	count_write++;
#if 0
	if(packetNum!=count_write)
	{
		SendAck(seq,(unsigned short)JT808_PACKET_UPDATE_TERM,(int)ENUM_JT808_COMMON_ERROR);
		if(unlink(updatepathname.c_str())<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::HandleTermUpdatedate failed to remove file %s",updatepathname.c_str());
		}						
		return;
	}
#endif
	if(packetNum == sum)
	{
		StruJT808Packet *thepacket= m_handler->CreatePacket();
		JT808Packet &packet =thepacket->packet;
		thepacket->reply = true;
		m_handler->PushWord(packet,first_seq);

		int sum = accumulate(unReceivedNum.begin(), unReceivedNum.end(), 0);
		if(sum >0){
			vector<unsigned short> tv;
			tv.clear();
			for(VType i =1; i< unReceivedNum.size(); i++)
				if(unReceivedNum[i] == 1)
					tv.push_back(i);

			m_handler->PushByte(packet,tv.size());
			for(int i=0;i< tv.size(); i++)
				m_handler->PushWord(packet,tv[i]);
		}else{
			m_handler->PushByte(packet, 0);
		}

		m_handler->SendBody(thepacket,JT808_PACKET_RESEND_REQUEST,JT808_CRYTO_NONE);
	}
	if(count_write==sum)
	{
		cout<<"++++++++++++++over+++++++++++++"<<endl;

		updatepathname = JT808_UPDATE_PATH+numToString<int>(update_install_time);
		ofstream updatefile(updatepathname.c_str());
		for(int i=1; i<mpmap.size()+1; i++)
		{
			MultiPacketIta mIterator;
			if((mIterator = mpmap.find(i)) != mpmap.end())
			{
				for(int j=0; j < mIterator->second.size();j++)
					updatefile<<mIterator->second[j];
			}
		}
		updatefile.close();

		update_finish_time=time(NULL);
		//	SendAck(seq,(unsigned short)JT808_PACKET_UPDATE_TERM,(int)ENUM_JT808_COMMON_OK);
		StruJT808Packet *thepacket= m_handler->CreatePacket();

		thepacket->reply = true;

		JT808Packet &packet =thepacket->packet;
		m_handler->PushWord(packet,first_seq);
		m_handler->PushByte(packet,0);
		m_handler->SendBody(thepacket,JT808_PACKET_RESEND_REQUEST,JT808_CRYTO_NONE);

		sleep(5);

		VDRUpdateTable *table = new VDRUpdateTable();
		if(table->SetIntValue((int)VDR_UPDATE_START_TIME,update_install_time)<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::get install_time failed");
			delete table;
			return;	
		}
		if(table->SetIntValue((int)VDR_UPDATE_TYPE,update_type)<0)
		{      
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::get type failed");
			delete table;		
			return;
		}
		if(table->SetSequenceValue((int)VDR_UPDATE_MANUFACTORY_ID,update_manuID)<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::get manuID failed");
			delete table;
			return;
		}
		if(table->SetIntValue((int)VDR_UPDATE_FINISH_TIME,update_finish_time)<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::get finishtime failed");
			delete table;
			return;
		}
		table->SetStringValue((int)VDR_UPDATE_VERSION,update_version);
		table->SetStringValue((int)VDR_UPDATE_PACKAGE_URL,updatepathname);
		if(table->SetIntValue((int)VDR_UPDATE_RESULT,3)<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::get result failed");
			delete table;	
			return;
		}
		if(table->Commit()<0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHelper::HandleTermUpdate commit failed");			
			SendAck(seq,(unsigned short)JT808_PACKET_UPDATE_TERM,(int)ENUM_JT808_COMMON_FAIL);
			delete table;
			return;
		}
		delete table;
		g_pDbusClient->sendService(&rrequest,reply,5*1000);
		count_write = 0;
	}															
	return;
}
#if 0
void JT808MessageHelper::HandleTermUpdate(JT808Packet &packet, int body, int len, unsigned short seq)
{
	int accu_len = 0;
	int ret = -1;
	ShutdownMachineReqC srequest;
	RebootMachineReqC   rrequest;
	DBusReplyC  *reply=NULL;

	if(len < 1)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleTermControl len invalid");
		SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
		return;
	}

	time_t install_time=time(NULL);
	int type= JT808_GET_BYTE(packet, body);
	body ++;
	accu_len ++;

	vector<unsigned char> manuID; 
	manuID.clear();
	for(int i=0;i<5;i++){
		manuID.push_back((unsigned char)JT808_GET_BYTE(packet, body));
		body ++;
		accu_len ++;
	}

	int version_len=JT808_GET_BYTE(packet, body);
	string version(packet[body],packet[body+version_len-1]); 
		body +=version_len;

	int length=JT808_GET_SHORT(packet, body);
	body ++;
	body ++;
	accu_len ++;
	accu_len ++;
	string update_packet(packet[body],packet[body+length-1]); 


	string updatepathname=JT808_UPDATE_PATH+numToString<int>(install_time);
	ofstream updatefile(updatepathname.c_str());

	updatefile<<updatepathname.c_str()<<flush;
	updatefile.close();

	SendAck(seq, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
	VDRUpdateTable *table = new VDRUpdateTable();
	if(table->SetIntValue((int)VDR_UPDATE_START_TIME, install_time) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get province failed");
		delete table;
		return ;
	}

	if(table->SetIntValue((int)VDR_UPDATE_TYPE, type) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get province failed");
		delete table;
		return ;
	}
	if(table->SetSequenceValue((int)VDR_UPDATE_MANUFACTORY_ID, manuID) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get city failed");
		delete table;
		return ;
	}
	table->SetStringValue((int)VDR_UPDATE_VERSION, version);
	table->SetStringValue((int)VDR_UPDATE_PACKAGE_URL, updatepathname);

	if(table->SetIntValue((int)VDR_UPDATE_RESULT, 3) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get province failed");
		delete table;
		return ;
	}

	if(table->SetIntValue((int)VDR_UPDATE_FINISH_TIME, 0) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendRegister get province failed");
		delete table;
		return ;
	}
	if(table->Commit() < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook commit failed");
		SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_FAIL);
		delete table;
		return;
	}


	SendAck(seq, (unsigned short)JT808_PACKET_UPDATE_TERM, (int)ENUM_JT808_COMMON_OK);
	delete table;
	return;
}
#endif
void JT808MessageHelper::HandleTempLocControl(JT808Packet &packet, int body, int len, unsigned short seq)
{
    if(len < 2)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerTempLocControl len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_TEMP_LOC_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    int duration =0;
    int interval = JT808_GET_SHORT(packet, body);
    body += 2;
    if(interval != 0)
	    duration = JT808_GET_LONG(packet, body);
    
    SendAck(seq, (unsigned short)JT808_PACKET_TEMP_LOC_CONTROL, (int)ENUM_JT808_COMMON_OK);
    
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerTempLocControl interval %d, duration %d", interval, duration);
    
    SendJT808TempLocReportNotify(interval,duration);
    
    return;
}
void JT808MessageHelper::HandleAlarmConfirm(JT808Packet &packet, int body, int len, unsigned short seq)
{
    if(len < 5)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerTempLocControl len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_TEMP_LOC_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    int seq1= JT808_GET_SHORT(packet, body);
    body += 2;
    int confirm= JT808_GET_LONG(packet, body);
    
    SendAck(seq, (unsigned short)JT808_PACKET_MANUAL_CONFIRM, (int)ENUM_JT808_COMMON_OK);


    SendJT808LocReportNotify(confirm);
    
    return;
}

void JT808MessageHelper::HandleTextMessage(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int accu_len = 0;
    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerTextMessage len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_TEXT_MESSAGE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    unsigned char flag = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    string text = "";
    while(accu_len < len)
    {
        text.push_back(JT808_GET_BYTE(packet, body));
        body ++;
        accu_len ++;
    }
    
    string utf_value = boost::locale::conv::to_utf<char>(text, JT808_CHAR_SET);
    
    SendAck(seq, (unsigned short)JT808_PACKET_TEXT_MESSAGE, (int)ENUM_JT808_COMMON_OK);
    
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerTextMessage flag %02x, text %s", flag, utf_value.c_str());
    
    SendJT808TextNotify(flag&0x01, flag&0x04, flag&0x08, flag&0x10, flag&0x20, utf_value);
    //SendJT808TextNotify(flag&0x01, 0x04, 0x08, flag&0x10, flag&0x20, utf_value);
    
    return;
}

void JT808MessageHelper::HandleEvent(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char type;
    int total_event;
    int accu_len = 0;
    int update_event=0;
    int upnum=0;
    unsigned char up_event[16];

    type = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    VDREventsTable *table = new VDREventsTable();
    switch(type){
	    default:
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook invalid type"); 
		    SendAck(seq, (unsigned short)JT808_PACKET_EVENT, (int)ENUM_JT808_COMMON_ERROR); 
		    delete table; 
		    return;
	    case (unsigned char)ENUM_JT808_EVENTS_DELETEALL:
		    if(table->DeleteAll() < 0) 
		    {    
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook deleteall failed");
			    SendAck(seq, (unsigned short)JT808_PACKET_EVENT, (int)ENUM_JT808_COMMON_FAIL);
			    delete table;
			    return;
		    }  
		    break;
	    case (unsigned char)ENUM_JT808_EVENTS_UPDATE:
		    if(table->DeleteAll() < 0)
		    {
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleEvent deleteall failed");
			    SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_FAIL);
			    delete table;
			    return;
		    }
	    case(unsigned char )ENUM_JT808_EVENTS_ADD:	
    			update_event=1;
	    case(unsigned char )ENUM_JT808_EVENTS_CHANGE:	
		    total_event = JT808_GET_BYTE(packet, body);
		    body ++;
		    accu_len ++;

		    while(accu_len < len)
		    {
			    if(len - accu_len < 2)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerEvent len invalid");
				    SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    int id = JT808_GET_BYTE(packet, body);
			    body ++;
			    accu_len ++;

			    if(update_event == 1 ) 
			    {
				up_event[upnum++]=id;		    	
			    }

			    int eventlen = JT808_GET_BYTE(packet, body);
			    body += 1;
			    accu_len += 1;
			    if(accu_len + eventlen > len)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::Handlerevent len not enough");
				    SendAck(seq, (unsigned short)JT808_PACKET_EVENT, (int)ENUM_JT808_COMMON_ERROR);
				    return;
			    }
			    string event;
			    for(int i = 0; i < eventlen; i ++)
			    {
				    event.push_back(JT808_GET_BYTE(packet, body));
				    accu_len ++;
				    body ++;
			    }
			    string utf_event = boost::locale::conv::to_utf<char>(event, JT808_CHAR_SET);
			    table->SetIntValue((int)VDR_EVENTS_ID, id);
			    table->SetStringValue((int)VDR_EVENTS_NAME, utf_event);
			    if(table->Commit() < 0)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleEvents commit failed");
				    SendAck(seq, (unsigned short)JT808_PACKET_EVENT, (int)ENUM_JT808_COMMON_FAIL);
				    delete table;
				    return;
			    }
		    }
#if 0
		    if(update_event == 1 ) 
		    {
			SendEvents(up_event, upnum);
		    }
#endif
		    break;
	    case (unsigned char)ENUM_JT808_EVENTS_DELETESOME:
		    total_event = JT808_GET_BYTE(packet, body);
		    body ++;
		    accu_len ++;
		    while(accu_len < len)
		    {
			    int id = JT808_GET_BYTE(packet, body);
			    body ++;
			    accu_len ++;
			    if(table->Delete(id) < 0)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleEvents commit failed");
				    SendAck(seq, (unsigned short)JT808_PACKET_EVENT, (int)ENUM_JT808_COMMON_FAIL);
				    delete table;
				    return;
			    }
		    }
		    break;
    }
    SendAck(seq, (unsigned short)JT808_PACKET_EVENT, (int)ENUM_JT808_COMMON_OK);
    delete table;

    return;
}
void JT808MessageHelper::HandleQuestion(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char flag;
    int question_len;
    int accu_len = 0;
    string question;
    map<int, string> answers;
    if(len < 2)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    flag = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    question_len = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    if(accu_len + question_len > len)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion ques len not enough(%d, %d, %d)", flag, question_len, len);
        SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    for(int i = 0; i < question_len; i ++)
    {
        question.push_back(JT808_GET_BYTE(packet, body));
        accu_len ++;
        body ++;
    }
    
    string utf_question = boost::locale::conv::to_utf<char>(question, JT808_CHAR_SET);
    
    answers.clear();
    while(accu_len < len)
    {
        if(len - accu_len < 3)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion answer len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_ERROR);
            return;
        }
        int id = JT808_GET_BYTE(packet, body);
        body ++;
        accu_len ++;
        if(answers.count(id) > 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion answer repeated");
            SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_ERROR);
            return;
        }
        int answerlen = JT808_GET_SHORT(packet, body);
        body += 2;
        accu_len += 2;
        if(accu_len + answerlen > len)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion answer len not enough");
            SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_ERROR);
            return;
        }
        string answer;
        for(int i = 0; i < answerlen; i ++)
        {
            answer.push_back(JT808_GET_BYTE(packet, body));
            accu_len ++;
            body ++;
        }
        string utf_answer = boost::locale::conv::to_utf<char>(answer, JT808_CHAR_SET);
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion answer %d, %s", id, utf_answer.c_str());
        answers.insert(map<int, string>::value_type(id, utf_answer));
    }
    
    SendAck(seq, (unsigned short)JT808_PACKET_QUESTION, (int)ENUM_JT808_COMMON_OK);

    question = boost::locale::conv::to_utf<char>(question, JT808_CHAR_SET);
    
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion flag %d, question %s", flag, utf_question.c_str());
    
    SendJT808QuestionNotify(seq, flag & 0x01, flag & 0x10, flag & 0x08, flag & 0x10, utf_question, answers);
    
    return;
}

void JT808MessageHelper::HandleCallBack(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char flag;
    string phonenumber = "";
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCallBack len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_CALL_BACK, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    flag = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    while(accu_len < len)
    {
        phonenumber.push_back(JT808_GET_BYTE(packet, body));
        accu_len ++;
        body ++;
    }
    
    SendAck(seq, (unsigned short)JT808_PACKET_CALL_BACK, (int)ENUM_JT808_COMMON_OK);
    
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCallBack type %d, phone %s", flag, phonenumber.c_str());
    
    SendJT808CallBackNotify(flag, phonenumber);
    
    return;
}

void JT808MessageHelper::HandlePhoneBook(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char type;
    int total_num;
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    type = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    VDRContactsTable *table = new VDRContactsTable();
    
    switch(type)
    {
        default:
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook invalid type");
            SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        
        case (unsigned char)ENUM_JT808_CONTACTS_CLEAR:
            if(table->DeleteAll() < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook deleteall failed");
                SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_FAIL);
                delete table;
                return;
            }
            break;
        case (unsigned char)ENUM_JT808_CONTACTS_UPDATE:
            if(table->DeleteAll() < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook deleteall failed");
                SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_FAIL);
                delete table;
                return;
            }
        case (unsigned char)ENUM_JT808_CONTACTS_APPEND:
        case (unsigned char)ENUM_JT808_CONTACTS_MODIFY:
            if(len < 2)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook len invalid 2");
                SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_ERROR);
                return;
            }
            total_num = JT808_GET_BYTE(packet, body);
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook total num is %d", total_num);
            body ++;
            accu_len ++;
            while(accu_len < len)
            {
                if(len - accu_len < 3)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook contact len invalid");
                    SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_ERROR);
                    delete table;
                    return;
                }
                int flag = JT808_GET_BYTE(packet, body);
                body ++;
                accu_len ++;
                int phonelen = JT808_GET_BYTE(packet, body);
                body ++;
                accu_len ++;
                if(len - accu_len < phonelen)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook phone len invalid");
                    SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_ERROR);
                    delete table;
                    return;
                }
                string phonenum;
                for(int i = 0; i < phonelen; i ++)
                {
                    phonenum.push_back(JT808_GET_BYTE(packet, body));
                    accu_len ++;
                    body ++;
                }
                int namelen = JT808_GET_BYTE(packet, body);
                body ++;
                accu_len ++;
                if(len - accu_len < namelen)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook name len invalid");
                    SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_ERROR);
                    delete table;
                    return;
                }
                string name;
                for(int i = 0; i < namelen; i ++)
                {
                    name.push_back(JT808_GET_BYTE(packet, body));
                    accu_len ++;
                    body ++;
                }
                string utf8_name = boost::locale::conv::to_utf<char>(name, JT808_CHAR_SET);
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook name %s, phone %s, flag %d", utf8_name.c_str(), phonenum.c_str(), flag);
                table->SetIntValue((int)VDR_CONTACTS_FLAG, flag);
                table->SetStringValue((int)VDR_CONTACTS_PHONE_NUMBER, phonenum);
                table->SetStringValue((int)VDR_CONTACTS_NAME, utf8_name);
                if(table->Commit() < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePhoneBook commit failed");
                    SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_FAIL);
                    delete table;
                    return;
                }
            }
            break;
    }
   
    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_PHONE_BOOK, (int)ENUM_JT808_COMMON_OK);
    
    SendJT808ContactsUpdateNotify();
    
    return;
}

void JT808MessageHelper::HandleCarControl(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char flag;
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCarControl len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_CAR_CONTROL, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    flag = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
   
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCarControl flag is %d\n", flag);
    SendAck(seq, (unsigned short)JT808_PACKET_CAR_CONTROL, (int)ENUM_JT808_COMMON_OK);
    
    SendCarControl(seq, flag);
    return;
}

void JT808MessageHelper::HandleSetCircle(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char attr;
    int total_num;
    int accu_len = 0;
    int flag=0;

    if(len < 2)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    attr = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    total_num = JT808_GET_BYTE(packet, body);
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle total num is %d", total_num);
    body ++;
    accu_len ++;
    
    VDRCircleFenceTable *table = new VDRCircleFenceTable();

    while(accu_len < len)
    {
        if(len - accu_len < 18)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle item len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        }
        int id = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        int area_attr = JT808_GET_SHORT(packet, body);
        body += 2;
        accu_len += 2;
        unsigned int latitude = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        unsigned int longitude = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        unsigned int radius = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;

        //int start_time = 0, stop_time = 0;
        SQLiteSequence bcd_start_time,bcd_stop_time;
        bcd_start_time.clear();
        bcd_stop_time.clear();
        if(area_attr & 0x01)
        {
            if(len - accu_len < 12)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle time len invalid");
                SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
                delete table;
                return;
            }
            for(int i = 0; i < 6; i ++)
            {
                bcd_start_time.push_back(JT808_GET_BYTE(packet, body));
                body ++;
                accu_len ++;
            }
            //start_time = GetTime(bcd);
            for(int i = 0; i < 6; i ++)
            {
                bcd_stop_time.push_back(JT808_GET_BYTE(packet, body));
                body ++;
                accu_len ++;
            }
            //stop_time = GetTime(bcd);
        }else{
	
		for(int i = 0; i < 6; i ++)
		{
			bcd_start_time.push_back(0);
			bcd_stop_time.push_back(0);
		}
	
	}
        int max_speed = 0;
        int duration = 0;
        if(area_attr & 0x02)
        {
            if(len - accu_len < 3)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle speed len invalid");
                SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
                delete table;
                return;
            }
            max_speed= JT808_GET_SHORT(packet, body);
            body += 2;
            accu_len += 2;
            
            duration = JT808_GET_BYTE(packet, body);
            body ++;
            accu_len ++;
        }

        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle id %d", id);
        
        switch(attr)
        {
            default:
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle speed len invalid");
                SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
                delete table;
                return;
            case (unsigned char)ENUM_JT808_FENCE_UPDATE:
		if(flag==0){
			if(table->DeleteAll() < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle DeleteAll failed");
				SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_FAIL);
				delete table;
				return;
			}
			SendJT808FenceUpdateNotify(0,0,0,-1);
			flag=1;
		}
            case (unsigned char)ENUM_JT808_FENCE_APPEND:
            case (unsigned char)ENUM_JT808_FENCE_MODIFY:
                table->SetIntValue((int)VDR_CIRCLE_REGION_ID, id);
                table->SetIntValue((int)VDR_CIRCLE_REGION_ATTRIBUTE, area_attr);
                table->SetIntValue((int)VDR_CIRCLE_CENTER_LATITUDE, latitude);
                table->SetIntValue((int)VDR_CIRCLE_CENTER_LONGITUDE, longitude);
                table->SetIntValue((int)VDR_CIRCLE_RADIUS, radius);
                table->SetSequenceValue((int)VDR_CIRCLE_BEGIN_TIME, bcd_start_time);
                table->SetSequenceValue((int)VDR_CIRCLE_END_TIME, bcd_stop_time);
                table->SetIntValue((int)VDR_CIRCLE_MAX_SPEED, max_speed);
                table->SetIntValue((int)VDR_CIRCLE_OVER_SPEED_DURA, duration);
                if(table->Commit() < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetCircle commit failed");
                    SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_FAIL);
                    delete table;
                    return;
                }            
		if(attr==ENUM_JT808_FENCE_MODIFY)SendJT808FenceUpdateNotify(0,0,id,0);
		else if((attr==ENUM_JT808_FENCE_APPEND)||(attr==ENUM_JT808_FENCE_UPDATE))SendJT808FenceUpdateNotify(1,0,id,0);
                break;
        }

    }

    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_SET_CIRCLE, (int)ENUM_JT808_COMMON_OK);
#if 0
    int id;
    VDRCircleFenceTable *table1 = new VDRCircleFenceTable();
    SQLiteSequence aaa;
    table1->RefreshBottom();
    table1->GetSequenceValue((int)VDR_CIRCLE_BEGIN_TIME, aaa);
    table1->GetIntValue((int)VDR_CIRCLE_REGION_ID, id);
    printf("time is *************** %d\n",id);
    for(SQLiteSequence::size_type aa=0;aa<aaa.size();aa++)
	    printf("time is *************** %d\n",aaa[aa]);
    delete table1;
#endif
    return;
}

void JT808MessageHelper::HandleDeleteCircle(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int total_num;
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    total_num = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    if(len != (1 + total_num * 4))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle len not match(%d, %d)", len, total_num);
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    VDRCircleFenceTable *table = new VDRCircleFenceTable();
    if(total_num >= 0)
    {
	int all = table->GetRowCount();
	printf("all cirlce is %d ======================\n", all);
	if(all<= 0 || all <total_num)
	{
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle ::no content");
    	    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
	    return;
	}
    }
    if(total_num == 0)
    {
        if(table->DeleteAll() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(0,0,0,-1);
    	SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_OK);
        delete table;
	return;
    }

    while(accu_len < len)
    {
        if(len - accu_len < 4)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle item len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        }
        int id = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;

        if(table->Delete(id) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(-1,0,id,0);
    }

    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_OK);
    return;
}

void JT808MessageHelper::HandleSetRectangle(JT808Packet &packet, int body, int len, unsigned short seq)
{
    unsigned char attr;
    int total_num;
    int accu_len = 0;
    int flag=0;

    if(len < 2)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    attr = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    total_num = JT808_GET_BYTE(packet, body);
    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle total num is %d", total_num);
    body ++;
    accu_len ++;
    
    VDRRectangleFenceTable *table = new VDRRectangleFenceTable();

    while(accu_len < len)
    {
        if(len - accu_len < 22)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle item len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        }
        int id = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        int area_attr = JT808_GET_SHORT(packet, body);
        body += 2;
        accu_len += 2;
        unsigned int latitude1 = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        unsigned int longitude1 = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        unsigned int latitude2 = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;
        unsigned int longitude2 = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;

        //int start_time = 0, stop_time = 0;
        SQLiteSequence bcd_start_time,bcd_stop_time;
        bcd_start_time.clear();
        bcd_stop_time.clear();
        if(area_attr & 0x01)
        {
            if(len - accu_len < 12)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle time len invalid");
                SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
                delete table;
                return;
            }
            for(int i = 0; i < 6; i ++)
            {
                bcd_start_time.push_back(JT808_GET_BYTE(packet, body));
                body ++;
                accu_len ++;
            }
            //start_time = GetTime(bcd);
            for(int i = 0; i < 6; i ++)
            {
                bcd_stop_time.push_back(JT808_GET_BYTE(packet, body));
                body ++;
                accu_len ++;
            }
            //stop_time = GetTime(bcd);
        }else{
	
		for(int i = 0; i < 6; i ++)
		{
			bcd_start_time.push_back(0);
			bcd_stop_time.push_back(0);
		}
	
	}
        int max_speed = 0;
        int duration = 0;
        if(area_attr & 0x02)
        {
            if(len - accu_len < 3)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle speed len invalid");
                SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
                delete table;
                return;
            }
            max_speed= JT808_GET_SHORT(packet, body);
            body += 2;
            accu_len += 2;
            
            duration = JT808_GET_BYTE(packet, body);
            body ++;
            accu_len ++;
        }

        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle id %d", id);
        
        switch(attr)
        {
            default:
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle speed len invalid");
                SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
                delete table;
                return;
            case (unsigned char)ENUM_JT808_FENCE_UPDATE:
		if(flag==0){
			if(table->DeleteAll() < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle DeleteAll failed");
				SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_FAIL);
				delete table;
				return;
			}
			SendJT808FenceUpdateNotify(0,1,0,-1);
			flag=1;
		}
            case (unsigned char)ENUM_JT808_FENCE_APPEND:
            case (unsigned char)ENUM_JT808_FENCE_MODIFY:
                table->SetIntValue((int)VDR_RECT_REGION_ID, id);
                table->SetIntValue((int)VDR_RECT_REGION_ATTRIBUTE, area_attr);
                table->SetIntValue((int)VDR_RECT_UPLEFT_LATITUDE, latitude1);
                table->SetIntValue((int)VDR_RECT_UPLEFT_LONGITUDE, longitude1);
                table->SetIntValue((int)VDR_RECT_LOWRIGTH_LATITUDE, latitude2);
                table->SetIntValue((int)VDR_RECT_LOWRIGHT_LONGITUDE, longitude2);
                table->SetSequenceValue((int)VDR_RECT_BEGIN_TIME, bcd_start_time);
                table->SetSequenceValue((int)VDR_RECT_END_TIME, bcd_stop_time);
                table->SetIntValue((int)VDR_RECT_MAX_SPEED, max_speed);
                table->SetIntValue((int)VDR_RECT_OVER_SPEED_DURA, duration);
                if(table->Commit() < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetRectangle commit failed");
                    SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_FAIL);
                    delete table;
                    return;
                }            
		if(attr==ENUM_JT808_FENCE_MODIFY)SendJT808FenceUpdateNotify(0,1,id,0);
		else if((attr==ENUM_JT808_FENCE_APPEND)||(attr==ENUM_JT808_FENCE_UPDATE))SendJT808FenceUpdateNotify(1,1,id,0);
                break;
        }

    }

    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_SET_RECTANGLE, (int)ENUM_JT808_COMMON_OK);
    
    SendJT808RectangleFenceUpdateNotify();
    
    return;
}

void JT808MessageHelper::HandleDeleteRectangle(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int total_num;
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteRectangle len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    total_num = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    if(len != (1 + total_num * 4))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteRectangle len not match(%d, %d)", len, total_num);
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    VDRRectangleFenceTable *table = new VDRRectangleFenceTable();
    if(total_num >= 0)
    {
	int all = table->GetRowCount();
	printf("all cirlce is %d ======================\n", all);
	if(all<= 0 || all <total_num)
	{
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle ::no content");
    	    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
	    return;
	}
    }
    if(total_num==0)
    {
        if(table->DeleteAll() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteRectangle Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(0,1,0,-1);
    	SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_OK);
        delete table;
	return;
    }

    while(accu_len < len)
    {
        if(len - accu_len < 4)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteRectangle item len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        }
        int id = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;

        if(table->Delete(id) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteRectangle Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(-1,1,id,0);
    }

    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_RECTANGLE, (int)ENUM_JT808_COMMON_OK);
    return;
}

void JT808MessageHelper::HandleSetPolygon(JT808Packet &packet, int body, int len, unsigned short seq)
{
	unsigned char attr;
	int total_num;
	int accu_len = 0;
	int flag=0;

	if(len < 2)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon len invalid");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
		return;
	}

	VDRPolygonFenceTable *table = new VDRPolygonFenceTable();

	int id = JT808_GET_LONG(packet, body);
	body += 4;
	accu_len += 4;
	int area_attr = JT808_GET_SHORT(packet, body);
	body += 2;
	accu_len += 2;
	//int start_time = 0, stop_time = 0;
        SQLiteSequence bcd_start_time,bcd_stop_time;
        bcd_start_time.clear();
        bcd_stop_time.clear();
	if(area_attr & 0x01)
	{
		if(len - accu_len < 12)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon time len invalid");
			SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
			delete table;
			return;
		}
		for(int i = 0; i < 6; i ++)
		{
			bcd_start_time.push_back(JT808_GET_BYTE(packet, body));
			body ++;
			accu_len ++;
		}
		//start_time = GetTime(bcd);
		for(int i = 0; i < 6; i ++)
		{
			bcd_stop_time.push_back(JT808_GET_BYTE(packet, body));
			body ++;
			accu_len ++;
		}
		//stop_time = GetTime(bcd);
	}else{
		for(int i = 0; i < 6; i ++)
		{
			bcd_start_time.push_back(0);
			bcd_stop_time.push_back(0);
		}
	}
	int max_speed = 0;
	int duration = 0;
	if(area_attr & 0x02)
	{
		if(len - accu_len < 3)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon speed len invalid");
			SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
			delete table;
			return;
		}
		max_speed= JT808_GET_SHORT(packet, body);
		body += 2;
		accu_len += 2;

		duration = JT808_GET_BYTE(packet, body);
		body ++;
		accu_len ++;
	}

	if(len - accu_len < 2)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon item len invalid 2");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
		delete table;
		return;
	}

	int point_number = JT808_GET_SHORT(packet, body);
	body += 2;
	accu_len += 2;
	if(len - accu_len < (point_number * 8))
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon position len invalid");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
		delete table;
		return;
	}

	SQLiteSequence points;
	for(int i = 0; i < point_number * 8; i ++)
	{
		points.push_back(JT808_GET_BYTE(packet, body));
		body ++;
		accu_len ++;
	}

	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon id %d", id);

	table->SetIntValue((int)VDR_POL_REGION_ID, id);
	table->SetIntValue((int)VDR_POL_REGION_ATTRIBUTE, area_attr);
	table->SetIntValue((int)VDR_POL_POINT_COUNT, point_number);
	table->SetSequenceValue((int)VDR_POL_POINT_LATLONG, points);
	table->SetSequenceValue((int)VDR_POL_BEGIN_TIME, bcd_start_time);
	table->SetSequenceValue((int)VDR_POL_END_TIME,bcd_stop_time);
	table->SetIntValue((int)VDR_POL_MAX_SPEED, max_speed);
	table->SetIntValue((int)VDR_POL_OVER_SPEED_DURA, duration);
	if(table->Commit() < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetPolygon commit failed");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_FAIL);
		delete table;
		return;
	}            
	SendJT808FenceUpdateNotify(1,2,id,0);

	delete table;
	SendAck(seq, (unsigned short)JT808_PACKET_SET_POLYGON, (int)ENUM_JT808_COMMON_OK);
	return;
}

void JT808MessageHelper::HandleDeletePolygon(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int total_num;
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeletePolygon len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    total_num = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    if(len != (1 + total_num * 4))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeletePolygon len not match(%d, %d)", len, total_num);
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    VDRPolygonFenceTable *table = new VDRPolygonFenceTable();
    if(total_num >= 0)
    {
	int all = table->GetRowCount();
	printf("all cirlce is %d ======================\n", all);
	if(all <= 0 || all <total_num)
	{
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle ::no content");
    	    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
	    return;
	}
    }
    if(total_num==0)
    {
        if(table->DeleteAll() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeletPolygon Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(0,2,0,-1);
    	SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_OK);
        delete table;
	return;
    }

    while(accu_len < len)
    {
        if(len - accu_len < 4)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeletePolygon item len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        }
        int id = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;

        if(table->Delete(id) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeletePolygon Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(-1,2,id,0);
    }

    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_POLYGON, (int)ENUM_JT808_COMMON_OK);
    return;
}

void JT808MessageHelper::HandleSetLine(JT808Packet &packet, int body, int len, unsigned short seq)
{
	unsigned char attr;
	int total_num;
	int accu_len = 0;
	int flag=0;

	if(len < 2)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine len invalid");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_ERROR);
		return;
	}


	VDRLineFenceTable *table = new VDRLineFenceTable();

	int id = JT808_GET_LONG(packet, body);
	body += 4;
	accu_len += 4;
	int area_attr = JT808_GET_SHORT(packet, body);
	body += 2;
	accu_len += 2;
	//int start_time = 0, stop_time = 0;
        SQLiteSequence bcd_start_time,bcd_stop_time;
        bcd_start_time.clear();
        bcd_stop_time.clear();
	if(area_attr & 0x01)
	{
		if(len - accu_len < 12)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine time len invalid");
			printf("JT808MessageHelper::HandleSetLine time len invalid %d & %d \n",len,accu_len);
			SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_ERROR);
			delete table;
			return;
		}
		for(int i = 0; i < 6; i ++)
		{
			bcd_start_time.push_back(JT808_GET_BYTE(packet, body));
			body ++;
			accu_len ++;
		}
		//start_time = GetTime(bcd);
		for(int i = 0; i < 6; i ++)
		{
			bcd_stop_time.push_back(JT808_GET_BYTE(packet, body));
			body ++;
			accu_len ++;
		}
		//stop_time = GetTime(bcd);
	}else{
		for(int i = 0; i < 6; i ++)
		{
			bcd_start_time.push_back(0);
			bcd_stop_time.push_back(0);
		}
	
	}

	int line_number = JT808_GET_SHORT(packet, body);
	body += 2;
	accu_len += 2;

	SQLiteSequence lines;
	for(int i = 0; i < line_number; i ++)
	{
		if(len - accu_len < 18)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine position len invalid");
			SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_ERROR);
			delete table;
			return;
		}
		for(int j = 0; j < 16; j ++)
		{
			lines.push_back(JT808_GET_BYTE(packet, body));
			body ++;
			accu_len ++;
		}
		unsigned char line_width= JT808_GET_BYTE(packet, body);
		lines.push_back(line_width); // Attr
		body ++;
		accu_len ++;
		unsigned char line_attr = JT808_GET_BYTE(packet, body);
		lines.push_back(line_attr); // Attr
		body ++;
		accu_len ++;
		if(line_attr & 0x01)
		{
			if(len - accu_len < 4)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine thres len invalid");
				SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_ERROR);
				delete table;
				return;
			}
			for(int j = 0; j < 4; j ++)
			{
				lines.push_back(JT808_GET_BYTE(packet, body));
				body ++;
				accu_len ++;
			}
		}
		if(line_attr & 0x02)
		{
			if(len - accu_len < 3)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine thres len invalid");
				SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_ERROR);
				delete table;
				return;
			}
			for(int j = 0; j < 3; j ++)
			{
				lines.push_back(JT808_GET_BYTE(packet, body));
				body ++;
				accu_len ++;
			}
		}
	}

	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine id %d", id);

	table->SetIntValue((int)VDR_LINE_REGION_ID, id);
	table->SetIntValue((int)VDR_LINE_REGION_ATTRIBUTE, area_attr);
	table->SetSequenceValue((int)VDR_LINE_BEGIN_TIME, bcd_start_time);
	table->SetSequenceValue((int)VDR_LINE_END_TIME, bcd_stop_time);
	table->SetIntValue((int)VDR_LINE_INFLEXION_COUNT, line_number);
	table->SetSequenceValue((int)VDR_LINE_INFLEXION_INFO, lines);
	if(table->Commit() < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleSetLine commit failed");
		SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_FAIL);
		delete table;
		return;
	}            
	SendJT808FenceUpdateNotify(1,3,id,0);

	delete table;
	SendAck(seq, (unsigned short)JT808_PACKET_SET_LINE, (int)ENUM_JT808_COMMON_OK);
	return;
}

void JT808MessageHelper::HandleDeleteLine(JT808Packet &packet, int body, int len, unsigned short seq)
{
    int total_num;
    int accu_len = 0;

    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteLine len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    total_num = JT808_GET_BYTE(packet, body);
    body ++;
    accu_len ++;
    
    if(len != (1 + total_num * 4))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteLine len not match(%d, %d)", len, total_num);
        SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
    
    VDRLineFenceTable *table = new VDRLineFenceTable();
    if(total_num >= 0)
    {
	int all = table->GetRowCount();
	printf("all cirlce is %d ======================\n", all);
	if(all<= 0 || all <total_num)
	{
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteCircle ::no content");
    	    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_CIRCLE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
	    return;
	}
    }
    if(total_num==0)
    {
        if(table->DeleteAll() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeletLine Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(0,3,0,-1);
    	SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_OK);
        delete table;
	return;
    }

    while(accu_len < len)
    {
        if(len - accu_len < 4)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteLine item len invalid");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_ERROR);
            delete table;
            return;
        }
        int id = JT808_GET_LONG(packet, body);
        body += 4;
        accu_len += 4;

        if(table->Delete(id) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDeleteLine Delete failed");
            SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
    	SendJT808FenceUpdateNotify(-1,3,id,0);
    }

    delete table;
    SendAck(seq, (unsigned short)JT808_PACKET_DELETE_LINE, (int)ENUM_JT808_COMMON_OK);
    return;
}

#if 0
void JT808MessageHelper::CreateDriveInfoPacket(JT808Packet &packet, int seq, int comm, int buflen,int flag_first)
{
	if(1==flag_first){
		m_handler->PushWord(packet, seq);
		m_handler->PushByte(packet, comm);
	}
	for(int i = 0; i < buflen; i ++)
	{
		m_handler->PushByte(packet, jt808_buf[i]);
	}
}
#endif

void JT808MessageHelper::HandleDataAcq(JT808Packet &packet, int body, int len, unsigned short seq)
{
	
    if(len < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDataAcq len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    unsigned char checksum = 0;
    int tmp, i;
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    thepacket->reply = false; 
#ifdef NHB
    
    vector<unsigned char> dataacq;
    vector<unsigned char> datasend;
    JT808Packet &packetb = thepacket->packet;
    int length;
    switch(packet[body])
    {
	    case 0x00:
	    case 0x01:
	    case 0x02:
	    case 0x03:
	    case 0x04:
	    case 0x05:
	    case 0x06:
	    case 0x07:
		    dataacq.push_back(0xAA);
		    dataacq.push_back(0x75);
		    for(tmp=body;tmp<body+len;tmp++)
			    dataacq.push_back(packet[tmp]);
		    dataacq.push_back(0x00);
		    dataacq.push_back(0x00);
		    dataacq.push_back(0x00);
		    for(vsize a=0;a<dataacq.size();a++)
		    {
			    checksum ^= dataacq[a];
		    }
		    dataacq.push_back(checksum);

		    GB19056Proxy<2012>::Instance()->ProcessRequest(dataacq, datasend);

		    if(datasend[2]==0xFA)
		    {
		    	    SendXsjlErrorAck(seq, packet[body], 0xFA);
			    return;
		    }

		    length=4+(datasend[3]<<8)+datasend[4]+2+1;

		    m_handler->PushWord(packetb, seq);
		    m_handler->PushByte(packetb, datasend[2]);
		    for(tmp=0;tmp<length;tmp++)
		    {
			    m_handler->PushByte(packetb, datasend[tmp]);
		    }
		    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
		    break;
#if 0
	    case 0x08:
	    case 0x09:
	    case 0x10:
	    case 0x11:
	    case 0x12:
	    case 0x13:
	    case 0x14:
	    case 0x15:
		    for(tmp=body+1;tmp<body+len;tmp++)
			    dataacq.push_back(packet[tmp]);

		    GB19056Proxy<2012>::Instance()->ProcessRequest(dataacq, datasend);

		    if(datasend[2]==0xFA)
		    {
		    	    SendXsjlErrorAck(seq, packet[body], 0xFA);
			    return;
		    }
		    /*
		    datasend.clear();
		    for (i = 0; i< 4096 ; i++)
			    datasend.push_back((unsigned char)(i % 255));
		    length= datasend.size();
			    */

		    length=7+(datasend[3]<<8)+datasend[4];
		    //printf("total num is %d ===============\n", datasend.size());

		    Send0700xx(seq, length, packet[body], datasend);
		    /*
		    m_handler->PushWord(packetb, seq);
		    m_handler->PushByte(packetb, datasend[2]);
		    for(tmp=0;tmp<length;tmp++)
		    {
			    m_handler->PushByte(packetb, datasend[tmp]);
		    }
		    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
		    */
		    break;
#endif
	    case 0x08:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    Send070008(seq, packet[body]);
		    }
		    break;
	    case 0x09:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    Send070009(seq, packet[body]);
		    }
		    break;
	    case 0x10:
//	    case 0x0a:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    Send070010(seq, packet[body]);
		    }
		    break;
	    case 0x11:
//	    case 0x0b:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    Send070011(seq, packet[body]);
		    }
		    break;
	    case 0x12:
//	    case 0x0c:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    Send070012(seq, packet[body]);
		    }
		    break;
	    case 0x13:
//	    case 0x0d:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    //	    Send070013(seq, packet[body]);
			    m_handler->PushWord(packetb, seq);
			    m_handler->PushByte(packetb, packet[body]);
			    length = sizeof(elec_log);
			    for(tmp=0;tmp<length;tmp++)
			    {
				    m_handler->PushByte(packetb, elec_log[tmp]);
			    }
			    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
		    }
		    break;
	    case 0x14:
//	    case 0x0e:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    m_handler->PushWord(packetb, seq);
		    m_handler->PushByte(packetb, packet[body]);
		    length = sizeof(param_record);
		    for(tmp=0;tmp<length;tmp++)
		    {
			    m_handler->PushByte(packetb, param_record[tmp]);
		    }
		    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
		    break;
	    case 0x15:
//	    case 0x0f:
		    //SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
		    {
			    Send070015(seq, packet[body]);
		    }
		    break;
	    default:
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDataAcq command %d invalid", packet[body]);
		    SendXsjlErrorAck(seq, packet[body], 0xFA);
		    break;
    }
#else
    SendAck(seq, (unsigned short)JT808_PACKET_DATA_ACQ, (int)ENUM_JT808_COMMON_OK);
    JT808Packet &packetb = thepacket->packet;
    m_handler->PushWord(packetb, seq);
    m_handler->PushByte(packetb, 0x01);
    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
#endif

    return;
}


void JT808MessageHelper::HandleParaSet(JT808Packet &packet, int body, int len, unsigned short seq)
{
    if((len < 1) || (packet[body+3] < 0x82))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleDataAcq len invalid");
	SendXsjlErrorAck(seq, packet[body], 0xFB);
        return;
    }

    unsigned char checksum = 0;
    int tmp;
    vector<unsigned char> dataacq;
    vector<unsigned char> datasend;

    printf("cmd is :\n");
    for(tmp=body+1;tmp<body+len;tmp++)
    {
	    dataacq.push_back(packet[tmp]);
	    printf("%d ", packet[tmp]);
    }
    printf("\n");

    GB19056Proxy<2012>::Instance()->ProcessRequest(dataacq, datasend);

    if(datasend[2]==0xFB)
    {
	    SendXsjlErrorAck(seq, packet[body], 0xFB);
    }else
	    SendXsjlErrorAck(seq, packet[body], 0x00);
#if 0

    int length=4+(datasend[3]<<8)+datasend[4]+2;
    StruJT808Packet *thepacket = m_handler->CreatePacket();

    JT808Packet &packetb = thepacket->packet;
    for(tmp=2;tmp<length;tmp++)
    {
	    m_handler->PushByte(packetb, datasend[tmp]);
    }

    m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);

#endif
    return;
}


void JT808MessageHelper::HandleCameraStart(JT808Packet &packet, int body, int len, unsigned short seq)
{
    StruCameraCommand command;
    if(len < 12)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleCameraStart len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_CAMERA_START, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }
#ifndef NHB 
    command.chan = 4; 
#else
    command.chan = JT808_GET_BYTE(packet, body);
#endif
    body ++;
    command.command = JT808_GET_SHORT(packet, body);
    body += 2;
    command.interval = JT808_GET_SHORT(packet, body);
    body += 2;
    command.flag = JT808_GET_BYTE(packet, body);
    body ++;
    command.resolution = JT808_GET_BYTE(packet, body);
    body ++;
    command.quality = JT808_GET_BYTE(packet, body);
    body ++;
    command.brightness = JT808_GET_BYTE(packet, body);
    body ++;
    command.contrast = JT808_GET_BYTE(packet, body);
    body ++;
    command.saturation = JT808_GET_BYTE(packet, body);
    body ++;
    command.chroma = JT808_GET_BYTE(packet, body);
    body ++;

//    SendAck(seq, (unsigned short)JT808_PACKET_CAMERA_START, (int)ENUM_JT808_COMMON_OK);
    
//#ifdef NHB
#if 0
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    //thepacket->reply = true;
    JT808Packet &packet1 = thepacket->packet;
    m_handler->PushWord(packet1, seq);
    m_handler->PushByte(packet1, 0);
    m_handler->PushByte(packet1, 1);
    m_handler->PushLong(packet1, time(NULL));
    m_handler->SendBody(thepacket, JT808_PACKET_CAMERA_START_RSP, JT808_CRYTO_NONE);
#endif

    seq_bak=seq;
    SendJT808CameraStartNotify(command);
    //JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_PHOTO, 1369749199);
    
    return;
}

void JT808MessageHelper::HandleRecordStart(JT808Packet &packet, int body, int len, unsigned short seq)
{
    StruRecordCommand command;
    if(len < 5)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRecordStart len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_RECORD_START, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    command.command = JT808_GET_BYTE(packet, body);
    body ++;
    command.duration = JT808_GET_SHORT(packet, body);
    body += 2;
    command.flag = JT808_GET_BYTE(packet, body);
    body ++;
    command.sample = JT808_GET_BYTE(packet, body);
    body ++;

    SendAck(seq, (unsigned short)JT808_PACKET_RECORD_START, (int)ENUM_JT808_COMMON_OK);
#ifdef NHB
    SendJT808RecordStartNotify(command);
#endif
    
    return;
}

void JT808MessageHelper::HandleQueryMediaIndex(JT808Packet &rec_packet, int body, int len, unsigned short seq)
{
    if(len < 15)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_INDEX_QUERY, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    int req_type = JT808_GET_BYTE(rec_packet, body);
    body ++;
    int req_chan = JT808_GET_BYTE(rec_packet, body);
    body ++;
    int req_event = JT808_GET_BYTE(rec_packet, body);
    body ++;
    
    vector<unsigned char> bcd;
    bcd.clear();
    for(int i = 0; i < 6; i ++)
    {
        bcd.push_back(JT808_GET_BYTE(rec_packet, body));
        body ++;
    }
    int start_time = GetTime(bcd);
    bcd.clear();
    for(int i = 0; i < 6; i ++)
    {
        bcd.push_back(JT808_GET_BYTE(rec_packet, body));
        body ++;
    }
    int stop_time = GetTime(bcd);

   LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper%d::___%d___%d__%d___%d",__LINE__,start_time,stop_time,seq,req_type);
    int item_count = 0;
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    thepacket->reply = true;

    if(req_type == (int)ENUM_JT808_MEDIA_AUDIO)
    {
        VDRRecordTable *table = new VDRRecordTable();
        if(table->RefreshBetween((int)VDR_REC_FILE_TIME, start_time, stop_time, true) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex query failed");
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_INDEX_QUERY, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            m_handler->DestroyPacket(thepacket);
            return;
        }
        
        for(int i = 0; i < table->GetResultsCount(); i ++)
        {
            int event;
            table->GetIntValue(i, (int)VDR_REC_EVENT, event);
            if(event != req_event)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex rec event not match(%d, %d)", event, req_event);
                continue;
            }

            int file_time;
            table->GetIntValue(i, (int)VDR_REC_FILE_TIME, file_time);
            m_handler->PushLong(packet, file_time);
            m_handler->PushByte(packet, req_type);
            m_handler->PushByte(packet, req_chan);
            m_handler->PushByte(packet, req_event);
            
            int alarmflag = 0;
            table->GetIntValue(i, (int)VDR_REC_ALARM_SIGNAL, alarmflag);
            m_handler->PushLong(packet, alarmflag);
            int statusflag = 0;
            table->GetIntValue(i, (int)VDR_REC_STATUS_FLAG, statusflag);
            m_handler->PushLong(packet, statusflag);
            int latitude = 0;
            table->GetIntValue(i, (int)VDR_REC_LATITUDE, latitude);
            m_handler->PushLong(packet, latitude);
            int longitude = 0;
            table->GetIntValue(i, (int)VDR_REC_LONGITUDE, longitude);
            m_handler->PushLong(packet, longitude);
            int altitude = 0;
            table->GetIntValue(i, (int)VDR_REC_ALTITUDE, altitude);
            m_handler->PushWord(packet, (unsigned short)altitude);
            int orient = 0;
            table->GetIntValue(i, (int)VDR_REC_ORIENTATION, orient);
            m_handler->PushWord(packet, (unsigned short)orient);
            int gpsspeed = 0;
            table->GetIntValue(i, (int)VDR_REC_GPS_SPEED, gpsspeed);
            m_handler->PushWord(packet, gpsspeed);
            vector<unsigned char> bcdTime;
            int tableIndex = 0;
            table->GetIntValue(i, (int)VDR_REC_OCCUR_TIME, tableIndex);
            GetTime((long long)tableIndex, bcdTime);
            m_handler->PushSequence(packet, bcdTime, 6);
            
            item_count ++;
        }
        
        delete table;
    }
    else if(req_type == (int)ENUM_JT808_MEDIA_PHOTO)
    {
        VDRCaptureTable *table = new VDRCaptureTable();
	if(start_time == 943920000)
	{
		if(table->RefreshBetween((int)VDR_CAP_FILE_TIME, 0, 0x71AE5E91, true) < 0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex query failed");
			SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_INDEX_QUERY, (int)ENUM_JT808_COMMON_FAIL);
			delete table;
			m_handler->DestroyPacket(thepacket);
			return;
		}

		for(int i = 0; i < table->GetResultsCount(); i ++)
		{
			int event;
			table->GetIntValue(i, (int)VDR_CAP_EVENT, event);
			if(event != req_event)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex photo event not match(%d, %d)", event, req_event);
				continue;
			}

			int chan;
			table->GetIntValue(i, (int)VDR_CAP_CHANNEL_ID, chan);
			if((req_chan != 0)&&(chan != req_chan))
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex photo chan not match(%d, %d)", chan, req_chan);
				continue;
			}

			int file_time;
			table->GetIntValue(i, (int)VDR_CAP_FILE_TIME, file_time);
			//m_handler->PushLong(packet, file_time);
			m_handler->PushLong(packet, i+1);
			m_handler->PushByte(packet, req_type);
			//m_handler->PushByte(packet, req_chan);
			m_handler->PushByte(packet, 0x01);
			m_handler->PushByte(packet, req_event);

			int alarmflag = 0;
			table->GetIntValue(i, (int)VDR_CAP_ALARM_SIGNAL, alarmflag);
			m_handler->PushLong(packet, alarmflag);
			int statusflag = 0;
			table->GetIntValue(i, (int)VDR_CAP_STATUS_FLAG, statusflag);
			m_handler->PushLong(packet, statusflag);
			int latitude = 0;
			table->GetIntValue(i, (int)VDR_CAP_LATITUDE, latitude);
			m_handler->PushLong(packet, latitude);
			int longitude = 0;
			table->GetIntValue(i, (int)VDR_CAP_LONGITUDE, longitude);
			m_handler->PushLong(packet, longitude);
			int altitude = 0;
			table->GetIntValue(i, (int)VDR_CAP_ALTITUDE, altitude);
			m_handler->PushWord(packet, (unsigned short)altitude);
			int orient = 0;
			table->GetIntValue(i, (int)VDR_CAP_ORIENTATION, orient);
			m_handler->PushWord(packet, (unsigned short)orient);
			int gpsspeed = 0;
			table->GetIntValue(i, (int)VDR_CAP_GPS_SPEED, gpsspeed);
			m_handler->PushWord(packet, gpsspeed);
			vector<unsigned char> bcdTime;
			int tableIndex = 0;
			table->GetIntValue(i, (int)VDR_CAP_OCCUR_TIME, tableIndex);
			GetTime((long long)tableIndex, bcdTime);
			m_handler->PushSequence(packet, bcdTime, 6);

			item_count ++;
		}
	}else {
		if(table->RefreshBottom() < 0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex query failed");
			SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_INDEX_QUERY, (int)ENUM_JT808_COMMON_FAIL);
			delete table;
			m_handler->DestroyPacket(thepacket);
			return;
		}
#if 0
			int event;
			table->GetIntValue(i, (int)VDR_CAP_EVENT, event);
			if(event != req_event)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex photo event not match(%d, %d)", event, req_event);
				continue;
			}

			int chan;
			table->GetIntValue(i, (int)VDR_CAP_CHANNEL_ID, chan);
			if(chan != req_chan)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaIndex photo event not match(%d, %d)", chan, req_chan);
				continue;
			}
#endif

			int file_time;
			//table->GetIntValue((int)VDR_CAP_FILE_TIME, file_time);
			//m_handler->PushLong(packet, file_time);
			m_handler->PushLong(packet, 0x02);
			m_handler->PushByte(packet, req_type);
			m_handler->PushByte(packet, req_chan);
			m_handler->PushByte(packet, req_event);

			int alarmflag = 0;
			table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, alarmflag);
			m_handler->PushLong(packet, alarmflag);
			int statusflag = 0;
			table->GetIntValue((int)VDR_CAP_STATUS_FLAG, statusflag);
			m_handler->PushLong(packet, statusflag);
			int latitude = 0;
			table->GetIntValue((int)VDR_CAP_LATITUDE, latitude);
			m_handler->PushLong(packet, latitude);
			int longitude = 0;
			table->GetIntValue((int)VDR_CAP_LONGITUDE, longitude);
			m_handler->PushLong(packet, longitude);
			int altitude = 0;
			table->GetIntValue((int)VDR_CAP_ALTITUDE, altitude);
			m_handler->PushWord(packet, (unsigned short)altitude);
			int orient = 0;
			table->GetIntValue((int)VDR_CAP_ORIENTATION, orient);
			m_handler->PushWord(packet, (unsigned short)orient);
			int gpsspeed = 0;
			table->GetIntValue((int)VDR_CAP_GPS_SPEED, gpsspeed);
			m_handler->PushWord(packet, gpsspeed);
			vector<unsigned char> bcdTime;
			int tableIndex = 0;
			table->GetIntValue((int)VDR_CAP_OCCUR_TIME, tableIndex);
			GetTime((long long)tableIndex, bcdTime);
			m_handler->PushSequence(packet, bcdTime, 6);
	
	}

        delete table;
    }
    else
    {
#ifndef NHB 
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_INDEX_QUERY, (int)ENUM_JT808_COMMON_OK);
            m_handler->PushByte(packet, req_type);
            m_handler->PushByte(packet, 0x00);
            m_handler->PushByte(packet, 0x00);
            
            m_handler->PushLong(packet, 0x00);
            m_handler->PushLong(packet, 0x00000000);
            m_handler->PushLong(packet, 0x26259D3);
            m_handler->PushLong(packet, 0x6EAC814);

            m_handler->PushWord(packet, 0x00);
            m_handler->PushWord(packet, 0x00);
            m_handler->PushWord(packet, 0x00);

            vector<unsigned char> bcdTime;
	    bcdTime.push_back(0x27);
	    bcdTime.push_back(0x04);
	    bcdTime.push_back(0x23);
	    bcdTime.push_back(0x08);
	    bcdTime.push_back(0x54);
	    bcdTime.push_back(0x09);
            m_handler->PushSequence(packet, bcdTime, 6);

            m_handler->PushByte(packet, 0x01);
            m_handler->PushByte(packet, 0x04);
            m_handler->PushLong(packet, 0x4F);

            m_handler->PushByte(packet, 0x03);
            m_handler->PushByte(packet, 0x02);
            m_handler->PushWord(packet, 0x00);
            
            item_count ++;
#endif
    }
    
    m_handler->PushWordFront(packet, (unsigned short)item_count);
    m_handler->PushWordFront(packet, (unsigned short)seq);
    
//    SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_INDEX_QUERY, (int)ENUM_JT808_COMMON_OK);
    
    m_handler->SendBody(thepacket, JT808_PACKET_MEDIA_INDEX_QUERY_RSP, JT808_CRYTO_NONE);
    
    return;
}

void JT808MessageHelper::HandleQueryMediaSingleIndex(JT808Packet &rec_packet, int body, int len, unsigned short seq)
{
    VDRRecordTable *cap_table;
    VDRCaptureTable *rec_table;
    int file_time;
    
    if(len < 5)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaSingleIndex len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    int id = JT808_GET_LONG(rec_packet, body);
    body += 4;
    int flag = JT808_GET_BYTE(rec_packet, body);
    body ++;

    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    thepacket->reply = true;

    rec_table = new VDRCaptureTable();
#ifdef NHB
    if(rec_table->RefreshBottom() < 0)
#else
    if(rec_table->Refresh(id) < 0)
#endif
    {
    LogUtility::Log(LOG_LEVEL_DEBUG, "%d::HandleQueryMediaSingleIndex tring to remove the cap row and file .",__LINE__); 
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleQueryMediaSingleIndex query record table failed");
        delete rec_table;
    	cap_table = new VDRRecordTable();
        if(cap_table->Refresh(id) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaSingleIndex query capture table failed");
            delete cap_table;
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY, (int)ENUM_JT808_COMMON_FAIL);
            m_handler->DestroyPacket(thepacket);
            return;
        }
        int event;
        cap_table->GetIntValue((int)VDR_REC_EVENT, event);
        cap_table->GetIntValue((int)VDR_REC_FILE_TIME, file_time);
        string file_name;
        cap_table->GetStringValue((int)VDR_REC_FILE_PATH, file_name);

        m_handler->PushLong(packet, file_time);
        m_handler->PushByte(packet, (unsigned char)ENUM_JT808_MEDIA_AUDIO);
        m_handler->PushByte(packet, 0);
        m_handler->PushByte(packet, event);
        
        int alarmflag = 0;
        cap_table->GetIntValue((int)VDR_REC_ALARM_SIGNAL, alarmflag);
        m_handler->PushLong(packet, alarmflag);
        int statusflag = 0;
        cap_table->GetIntValue((int)VDR_REC_STATUS_FLAG, statusflag);
        m_handler->PushLong(packet, statusflag);
        int latitude = 0;
        cap_table->GetIntValue((int)VDR_REC_LATITUDE, latitude);
        m_handler->PushLong(packet, latitude);
        int longitude = 0;
        cap_table->GetIntValue((int)VDR_REC_LONGITUDE, longitude);
        m_handler->PushLong(packet, longitude);
        int altitude = 0;
        cap_table->GetIntValue((int)VDR_REC_ALTITUDE, altitude);
        m_handler->PushWord(packet, (unsigned short)altitude);
        int gpsspeed = 0;
        cap_table->GetIntValue((int)VDR_REC_GPS_SPEED, gpsspeed);
        m_handler->PushWord(packet, gpsspeed);
        int orient = 0;
        cap_table->GetIntValue((int)VDR_REC_ORIENTATION, orient);
        m_handler->PushWord(packet, (unsigned short)orient);
        vector<unsigned char> bcdTime;
        int tableIndex = 0;
        cap_table->GetIntValue((int)VDR_REC_OCCUR_TIME, tableIndex);
        GetTime((long long)tableIndex, bcdTime);
        m_handler->PushSequence(packet, bcdTime, 6);

        if(flag)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleQueryMediaSingleIndex tring to remove the rec row %d and file %s.", id, file_name.c_str());
            if(cap_table->Delete(id) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaSingleIndex failed to delete rec row %d.", id);
            }
            
            if(unlink(file_name.c_str()) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaSingleIndex failed to remove file %s.", file_name.c_str());
            }
        }

        delete cap_table;
        
    }
    else
    {
    LogUtility::Log(LOG_LEVEL_DEBUG, "%d::HandleQueryMediaSingleIndex tring to remove the cap row and file .",__LINE__); 
        int event;
        rec_table->GetIntValue((int)VDR_CAP_EVENT, event);
        int chan;
        rec_table->GetIntValue((int)VDR_CAP_CHANNEL_ID, chan);
        rec_table->GetIntValue((int)VDR_CAP_FILE_TIME, file_time);
        string file_name;
        rec_table->GetStringValue((int)VDR_CAP_FILE_PATH, file_name);
#ifdef NHB
        m_handler->PushLong(packet, id);
#else 
        m_handler->PushLong(packet, file_time);
#endif
        m_handler->PushByte(packet, (unsigned char)ENUM_JT808_MEDIA_PHOTO);
        m_handler->PushByte(packet, chan);
        m_handler->PushByte(packet, event);
        
        int alarmflag = 0;
        rec_table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, alarmflag);
        m_handler->PushLong(packet, alarmflag);
        int statusflag = 0;
        rec_table->GetIntValue((int)VDR_CAP_STATUS_FLAG, statusflag);
        m_handler->PushLong(packet, statusflag);
        int latitude = 0;
        rec_table->GetIntValue((int)VDR_CAP_LATITUDE, latitude);
        m_handler->PushLong(packet, latitude);
        int longitude = 0;
        rec_table->GetIntValue((int)VDR_CAP_LONGITUDE, longitude);
        m_handler->PushLong(packet, longitude);
        int altitude = 0;
        rec_table->GetIntValue((int)VDR_CAP_ALTITUDE, altitude);
        m_handler->PushWord(packet, (unsigned short)altitude);
        int orient = 0;
        rec_table->GetIntValue((int)VDR_CAP_ORIENTATION, orient);
        m_handler->PushWord(packet, (unsigned short)orient);
        int gpsspeed = 0;
        rec_table->GetIntValue((int)VDR_CAP_GPS_SPEED, gpsspeed);
        m_handler->PushWord(packet, gpsspeed);
        vector<unsigned char> bcdTime;
        int tableIndex = 0;
        rec_table->GetIntValue((int)VDR_CAP_OCCUR_TIME, tableIndex);
        GetTime((long long)tableIndex, bcdTime);
        m_handler->PushSequence(packet, bcdTime, 6);

        if(flag)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandleQueryMediaSingleIndex tring to remove the cap row %d and file %s.", id, file_name.c_str());
            if(rec_table->Delete(id) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaSingleIndex failed to delete cap row %d.", id);
            }
            
            if(unlink(file_name.c_str()) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleQueryMediaSingleIndex failed to remove file %s.", file_name.c_str());
            }
        }
        
        delete rec_table;
    }
    
    m_handler->PushWordFront(packet, 1);
    m_handler->PushWordFront(packet, (unsigned short)seq);
    
    SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY, (int)ENUM_JT808_COMMON_OK);
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "%d::HandleQueryMediaSingleIndex tring to remove the cap row %d and file .",__LINE__,file_time); 
    //m_handler->SendBody(thepacket, JT808_PACKET_MEDIA_INDEX_QUERY_RSP, JT808_CRYTO_NONE);
    SendMedia8805((ENUM_JT808_MEDIA_TYPE )0, file_time, 1, id);
    
    return;
}

void JT808MessageHelper::HandlePacketMediaDataRsp(JT808Packet &rec_packet, int body, int len, unsigned short)
{
    VDRRecordTable *rec_table;
    VDRCaptureTable *cap_table;
    StruMediaDataInfo info;
    string file_name;
    int gpstime;
    int accu_len = 0;
    map<int, int> idlist;
    
    if(len < 5)
    {
	    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp len invalid");
	    return;
    }

    int index = JT808_GET_LONG(rec_packet, body);
    body += 4;
    accu_len += 4;
    int num = JT808_GET_BYTE(rec_packet, body);
    body ++;
    accu_len ++;

    m_handler->PopQueue(index);

    if(len - accu_len < 2 * num)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp len not enough");
        return;
    }
    
    idlist.clear();
    for(int i = 0; i < num; i ++)
    {
        int id = JT808_GET_SHORT(rec_packet, body);
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp retx id %d", id);
        body += 2;
        idlist.insert(map<int, int>::value_type(id, 1));
    }

    if(idlist.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp no id needs to be retx");
        return;
    }
    
    info.index = index;
    
    rec_table = new VDRRecordTable();
    if(rec_table->Refresh(index) < 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaDataRsp query record table failed");
        delete rec_table;
        cap_table = new VDRCaptureTable();
        if(cap_table->Refresh(index) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaDataRsp query capture table failed");
            delete cap_table;
            return;
        }
        
        cap_table->GetIntValue((int)VDR_CAP_EVENT, info.event);
        cap_table->GetIntValue((int)VDR_CAP_CHANNEL_ID, info.chan);
        cap_table->GetStringValue((int)VDR_CAP_FILE_PATH, file_name);
        info.type = (int)ENUM_JT808_MEDIA_PHOTO;

        cap_table->GetIntValue((int)VDR_CAP_FILE_TYPE, info.format);
        if(info.format == (int)VDR_CAPTURE_FORMAT_JPG)
        {
            info.format = ENUM_JT808_FILE_FORMAT_JPG;
        }
        else
        {
            info.format = ENUM_JT808_FILE_FORMAT_TIF;
        }
        cap_table->GetIntValue((int)VDR_CAP_ALARM_SIGNAL, info.alarmflag);
        cap_table->GetIntValue((int)VDR_CAP_STATUS_FLAG, info.statusflag);
        cap_table->GetIntValue((int)VDR_CAP_LATITUDE, info.latitude);
        cap_table->GetIntValue((int)VDR_CAP_LONGITUDE, info.longitude);
        cap_table->GetIntValue((int)VDR_CAP_ALTITUDE, info.altitude);
        cap_table->GetIntValue((int)VDR_CAP_ORIENTATION, info.orient);
        cap_table->GetIntValue((int)VDR_CAP_GPS_SPEED, info.gpsspeed);
        cap_table->GetIntValue((int)VDR_CAP_OCCUR_TIME, gpstime);
        GetTime((long long)gpstime, info.gpstime);
        
        delete cap_table;
    }
    else
    {
        rec_table->GetIntValue((int)VDR_REC_EVENT, info.event);
        info.chan = 0;
        rec_table->GetStringValue((int)VDR_REC_FILE_PATH, file_name);
        rec_table->GetIntValue((int)VDR_REC_FILE_TYPE, info.format);
        if(info.format == (int)VDR_RECORD_FORMAT_WAV)
        {
            info.format = ENUM_JT808_FILE_FORMAT_WAV;
        }
        else
        {
            info.format = ENUM_JT808_FILE_FORMAT_MP3;
        }

        rec_table->GetIntValue((int)VDR_REC_ALARM_SIGNAL, info.alarmflag);
        rec_table->GetIntValue((int)VDR_REC_STATUS_FLAG, info.statusflag);
        rec_table->GetIntValue((int)VDR_REC_LATITUDE, info.latitude);
        rec_table->GetIntValue((int)VDR_REC_LONGITUDE, info.longitude);
        rec_table->GetIntValue((int)VDR_REC_ALTITUDE, info.altitude);
        rec_table->GetIntValue((int)VDR_REC_ORIENTATION, info.orient);
        rec_table->GetIntValue((int)VDR_REC_GPS_SPEED, info.gpsspeed);

        rec_table->GetIntValue((int)VDR_REC_OCCUR_TIME, gpstime);
        GetTime((long long)gpstime, info.gpstime);

        delete rec_table;
    }
    
    SendMediaFile(info, file_name, idlist);
    
    return;
}

void JT808MessageHelper::HandlePacketMediaUpload(JT808Packet &rec_packet, int body, int len, unsigned short seq)
{
#ifndef NHB
    SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_OK);

    int readlen = JT808_FILE_SEGMENT_SIZE;
    bool read_over = false;
    int count = 0;
    
    StruJT808Packet *thepacket = m_handler->CreatePacket();
    JT808Packet &packet = thepacket->packet;
    thepacket->reply = false;
    
    m_handler->PushLong(packet, 0x345678);
    m_handler->PushByte(packet, 0x02);
    m_handler->PushByte(packet, 0x03);
    m_handler->PushByte(packet, 0x01);
    m_handler->PushByte(packet, 0x01);

    m_handler->PushLong(packet, 0x00);
    m_handler->PushLong(packet, 0x00);
    m_handler->PushLong(packet, 0x00);
    m_handler->PushLong(packet, 0x00);
    m_handler->PushLong(packet, 0x00);
    m_handler->PushLong(packet, 0x00);
    m_handler->PushLong(packet, 0x00);

    m_handler->SendBody(thepacket, JT808_PACKET_MEDIA_DATA, JT808_CRYTO_NONE);
    return;
#endif
    StruMediaDataInfo info;
    map<int, int> idlist;
    int gpstime;
    
    idlist.clear();
    if(len < 16)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload len invalid");
        SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_ERROR);
        return;
    }

    int req_type = JT808_GET_BYTE(rec_packet, body);
    body ++;
    int req_chan = JT808_GET_BYTE(rec_packet, body);
    body ++;
    int req_event = JT808_GET_BYTE(rec_packet, body);
    body ++;
    
    info.event = req_event;
    info.type = req_type;
    info.chan = req_chan;
    
    vector<unsigned char> bcd;
    bcd.clear();
    for(int i = 0; i < 6; i ++)
    {
        bcd.push_back(JT808_GET_BYTE(rec_packet, body));
        body ++;
    }
    int start_time = GetTime(bcd);
    bcd.clear();
    for(int i = 0; i < 6; i ++)
    {
        bcd.push_back(JT808_GET_BYTE(rec_packet, body));
        body ++;
    }
    int stop_time = GetTime(bcd);
    int flag = JT808_GET_BYTE(rec_packet, body);
    body ++;

    if(req_type == (int)ENUM_JT808_MEDIA_AUDIO)
    {
        VDRRecordTable *table = new VDRRecordTable();
        if(table->RefreshBetween((int)VDR_REC_FILE_TIME, start_time, stop_time, true) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload query failed");
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
       if(table->GetResultsCount()==0); 
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload query failed");
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
        SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_OK);
        
        for(int i = 0; i < table->GetResultsCount(); i ++)
        {
            int event;
            table->GetIntValue(i, (int)VDR_REC_EVENT, event);
            if(event != req_event)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload rec event not match(%d, %d)", event, req_event);
                continue;
            }
            
            info.chan = 0;

            string file_name;
            table->GetIntValue(i, (int)VDR_REC_FILE_TIME, info.index);
            table->GetStringValue(i, (int)VDR_REC_FILE_PATH, file_name);
            table->GetIntValue(i, (int)VDR_REC_FILE_TYPE, info.format);
            if(info.format == (int)VDR_RECORD_FORMAT_WAV)
            {
                info.format = ENUM_JT808_FILE_FORMAT_WAV;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_MP3;
            }

            table->GetIntValue(i, (int)VDR_REC_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue(i, (int)VDR_REC_STATUS_FLAG, info.statusflag);
            table->GetIntValue(i, (int)VDR_REC_LATITUDE, info.latitude);
            table->GetIntValue(i, (int)VDR_REC_LONGITUDE, info.longitude);
            table->GetIntValue(i, (int)VDR_REC_ALTITUDE, info.altitude);
            table->GetIntValue(i, (int)VDR_REC_ORIENTATION, info.orient);
            table->GetIntValue(i, (int)VDR_REC_GPS_SPEED, info.gpsspeed);
            table->GetIntValue(i, (int)VDR_REC_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
            
            SendMediaFile(info, file_name, idlist);

            if(flag)
            {
                LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaUpload tring to remove the rec row %d and file %s.", info.index, file_name.c_str());
                if(table->Delete(info.index) < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload failed to delete rec row %d.", info.index);
                }
                
                if(unlink(file_name.c_str()) < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload failed to remove file %s.", file_name.c_str());
                }
            }
        }
        
        delete table;
    }
    else if(req_type == (int)ENUM_JT808_MEDIA_PHOTO)
    {
        VDRCaptureTable *table = new VDRCaptureTable();
        if(table->RefreshBetween((int)VDR_CAP_FILE_TIME, start_time, stop_time, true) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload query failed");
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
       if(table->GetResultsCount()==0); 
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload query failed");
            SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_FAIL);
            delete table;
            return;
        }
        SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_OK);
        for(int i = 0; i < table->GetResultsCount(); i ++)
        {
            int event;
            table->GetIntValue(i, (int)VDR_CAP_EVENT, event);
            if(event != req_event)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload photo event not match(%d, %d)", event, req_event);
                continue;
            }
            
            int chan;
            table->GetIntValue(i, (int)VDR_CAP_CHANNEL_ID, chan);
            if(chan != req_chan)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload photo event not match(%d, %d)", chan, req_chan);
                continue;
            }
            
            string file_name;
            table->GetIntValue(i, (int)VDR_CAP_FILE_TIME, info.index);
            table->GetStringValue(i, (int)VDR_CAP_FILE_PATH, file_name);
            table->GetIntValue(i, (int)VDR_CAP_FILE_TYPE, info.format);
            if(info.format == (int)VDR_CAPTURE_FORMAT_JPG)
            {
                info.format = ENUM_JT808_FILE_FORMAT_JPG;
            }
            else
            {
                info.format = ENUM_JT808_FILE_FORMAT_TIF;
            }
            table->GetIntValue(i, (int)VDR_CAP_ALARM_SIGNAL, info.alarmflag);
            table->GetIntValue(i, (int)VDR_CAP_STATUS_FLAG, info.statusflag);
            table->GetIntValue(i, (int)VDR_CAP_LATITUDE, info.latitude);
            table->GetIntValue(i, (int)VDR_CAP_LONGITUDE, info.longitude);
            table->GetIntValue(i, (int)VDR_CAP_ALTITUDE, info.altitude);
            table->GetIntValue(i, (int)VDR_CAP_ORIENTATION, info.orient);
            table->GetIntValue(i, (int)VDR_CAP_GPS_SPEED, info.gpsspeed);
            table->GetIntValue(i, (int)VDR_CAP_OCCUR_TIME, gpstime);
            GetTime((long long)gpstime, info.gpstime);
            
            SendMediaFile(info, file_name, idlist);
            
            if(flag)
            {
                LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::HandlePacketMediaUpload tring to remove the cap row %d and file %s.", info.index, file_name.c_str());
                if(table->Delete(info.index) < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload failed to delete cap row %d.", info.index);
                }
                
                if(unlink(file_name.c_str()) < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload failed to remove file %s.", file_name.c_str());
                }
            }
        }

        delete table;
    }
    else
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlePacketMediaUpload unsupported type", req_type);
        SendAck(seq, (unsigned short)JT808_PACKET_MEDIA_UPLOAD, (int)ENUM_JT808_COMMON_NOT_SUPPORT);
        return;
    }
    
    return;
}
void JT808MessageHelper::HandlePacketDownTransparentTransmission(JT808Packet &packet, int body, int len, unsigned short seq)
{
	int i;
	int accu_len = 0;
	string gbk_string; 
	char buf[512]={0};
	struct tagPOI arrayPOI[5];

	JT808DataTransmitNotifyC notify;
	if(len < 2)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRecordStart len invalid");
		SendAck(seq, (unsigned short)JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION, (int)ENUM_JT808_COMMON_ERROR);
		return;
	}
	int type= JT808_GET_BYTE(packet, body);
	notify.type= type;
	notify.total=len-1;
	body++;
	accu_len++;
	switch(type)
	{
		case JT808_TT_USERE:
			for (i=0;accu_len<= len;accu_len++)
				buf[i++]=JT808_GET_BYTE(packet,body++);
			gbk_string = boost::locale::conv::from_utf<char>(buf, "GBK");
			SendAck(seq, (unsigned short)JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION, (int)ENUM_JT808_COMMON_OK);
			{
				int n=  GetData((char *)gbk_string.c_str(), len, arrayPOI);
				CNaviRoadBookReqC        request;	
				request.SetBookTitle("");//QString(tr("")).toLatin1().data()
				request.SetDescription("");//QString(tr("")).toLatin1().data()
				for(int i=0; i<n; i++)
				{
					request.AddBookItem((char *)arrayPOI[i].strName.c_str(), arrayPOI[i].dlon, arrayPOI[i].dlan);
					printf("%d::name is %s, lon is %f ,lat is %f \n",__LINE__,arrayPOI[i].strName.c_str(), arrayPOI[i].dlon, arrayPOI[i].dlan);
				}
				/*
				//京P3MP06        116.34373600000000       39.76802800000000	北京市京开辅路沁芳??				request.AddBookItem(QString(tr("北京市京开辅路沁芳??)).toLatin1().data(), (float)116.343736, (float)39.768028);
				//京P3MP06	116.34244800000000	39.76857600000000	北京市G45大广高速沁芳园
				request.AddBookItem(QString(tr("北京市G45大广高速沁芳园")).toLatin1().data(), (float)116.342448, (float)39.768576);
				//京P3MP06	116.34232800000000	39.76985600000000	北京市盛坊路三利工业??				request.AddBookItem(QString(tr("北京市盛坊路三利工业??)).toLatin1().data(), (float)116.342328, (float)39.769856);
				//京P3MP06	116.34499200000000	39.76979200000000	北京市盛坊路杰奥??				request.AddBookItem(QString(tr("北京市盛坊路杰奥??)).toLatin1().data(), (float)116.344992, (float)39.769792);
				//京P3MP06	116.34801600000000	39.76334000000000	北京市大兴区广茂大街泰中美大酒店
				request.AddBookItem(QString(tr("北京市大兴区广茂大街泰中美大酒店")).toLatin1().data(), (float)116.348016, (float)39.763340);
				*/

				DBusReplyC      *reply=NULL;
				int result = sp_ClientGps->sendService(&request,reply,1*1000);

				if(result<0 ||reply->getIdentifier()!=REP_ID_NAVI_COMMON)
				{
					printf("Send Service Error. Error Code: %d\n",result);
				}
				else
				{
					printf("Received Common Reply. result:%d\n",reply->result);
				}
				delete reply;
				JT808MultiPointDbusNotifyC notify;
				g_pDbusServer->sendNotify(&notify);
			}
			break;
		case JT808_TT_GNSS: 
			break;
		case JT808_TT_COM1:
		case JT808_TT_COM2:
			for (i=0;accu_len<= len;accu_len++)
				notify.buf[i++]=JT808_GET_BYTE(packet,body++);
			notify.errnum=0x00;
			g_pDbusServer->sendNotify(&notify);
			break;
		case JT808_TT_USERB:
			SendAck(seq, (unsigned short)JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION, (int)ENUM_JT808_COMMON_OK);
		case JT808_TT_IC: 
			for (i=0;accu_len<= len;accu_len++)
				notify.buf[i++]=JT808_GET_BYTE(packet,body++);
			notify.errnum=0x00;
			g_pDbusServer->sendNotify(&notify);
			break;
		default:
			if((type>=JT808_TT_USERB)&&(type<=JT808_TT_USERE))
			{
				g_pDbusServer->sendNotify(&notify);
				SendAck(seq, (unsigned short)JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION, (int)ENUM_JT808_COMMON_OK);
			}else
				SendAck(seq, (unsigned short)JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION, (int)ENUM_JT808_COMMON_OK);
			break;
	}

	if(type == JT808_TT_IC){
		ConfigManager::m_control = 1;
		ConnectionManager::Instance()->Close();
	}

	return;
}
void JT808MessageHelper::Send0700xx(int seq, int length, int comm, vector<unsigned char> & send)
{
	int count=0;
	int total, current = length, num1 = 0;
	int num = length / 900 + 1;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = true; 
	for (int i=0; i < num; i++)
	{
		count++;
		total = 900;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
			total = 897;
		}
		
		for(int tmp=0; tmp<total && tmp < current; tmp++)
		{
			m_handler->PushByte(packet, send[num1 + tmp]);
		}
		num1 += total ;
		if(current > total)
			current -= total; 
		printf("count is %d ; total is %d ; current is %d ;num1 is %d =================\n",count ,total, current, num1);
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}

void JT808MessageHelper::Send070008(int seq, int comm)
{
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = false; 
	for (int i=0; i < SPED_NUM; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		
		for(int tmp=0; tmp<sped_array[i]; tmp++)
		{
			m_handler->PushByte(packet, sped_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}
void JT808MessageHelper::Send070009(int seq, int comm)
{
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = false; 
	for (int i=0; i < POSITION_ACEPT; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		for(int tmp=0; tmp<acept_array[i]; tmp++)
		{
			m_handler->PushByte(packet, acept_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}
void JT808MessageHelper::Send070010(int seq, int comm)
{
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = false; 
	for (int i=0; i < ACC_NUM; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		for(int tmp=0; tmp<accident_array[i]; tmp++)
		{
			m_handler->PushByte(packet, accident_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}
void JT808MessageHelper::Send070011(int seq, int comm)
{
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = false; 
	for (int i=0; i < TIRED_NUM; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		for(int tmp=0; tmp<tired_array[i]; tmp++)
		{
			m_handler->PushByte(packet, tired_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}
void JT808MessageHelper::Send070012(int seq, int comm)
{
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply =false ; 
	for (int i=0; i < DRIVER_NUM; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		for(int tmp=0; tmp<driver_array[i]; tmp++)
		{
			m_handler->PushByte(packet, driver_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}
void JT808MessageHelper::Send070013(int seq, int comm)
{
#if 0
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = false; 
	for (int i=0; i < ELEC_NUM; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		for(int tmp=0; tmp<elec_array[i]; tmp++)
		{
			m_handler->PushByte(packet, elec_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
#endif
}
void JT808MessageHelper::Send070015(int seq, int comm)
{
	int count=0;
	StruJT808Packet *thepacket = m_handler->CreatePacket();
	thepacket->reply = false; 
	for (int i=0; i < SPEED_NUM; i++)
	{
		count++;
		thepacket->packetarray.push_back(JT808_NEW_PACKET);
		thepacket->segment_id.push_back(count);
		JT808Packet &packet = *thepacket->packetarray.back();
		if(count == 1){
			m_handler->PushWord(packet, seq);
			m_handler->PushByte(packet, comm);
		}
		for(int tmp=0; tmp<speed_array[i]; tmp++)
		{
			m_handler->PushByte(packet, speed_log[i][tmp]);
		}
	}
	if(thepacket->packetarray.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::SendMediaFile no packet created.");
		m_handler->DestroyPacket(thepacket);
		return;
	}

	thepacket->total_segment = count;
	m_handler->SendBody(thepacket, JT808_PACKET_DATA_SEND, JT808_CRYTO_NONE);
}
