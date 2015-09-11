/***
**  @ file: nmea_type.h
*
*/

#ifndef _NMEA_TYPE_H
#define _NMEA_TYPE_H

// definition
#ifndef NULL
#define NULL (0)
#endif

#ifndef FALSE
#define FALSE (0)
#define TRUE (!FALSE)
#endif

#define DECODE_GSV
#define DECODE_GSA

#define ERR_CHKSUM (0x01)
#define ERR_BEGIN_CHAR (0x02)
#define ERR_MSG_HEADER (0x04)
#define ERR_MSG_INCOMPLETE (0x08)
#define ERR_INTERFAE (0x80)

// msg mask
#define GGA_DECODE (0x01)
#define GSA_DECODE (0x02)
#define GSV_DECODE (0x04)
#define RMC_DECODE (0x08)
#define GLL_DECODE (0x10)
#define VTG_DECODE (0x20)
#define ANT_DECODE (0x80)

#define PVT_DECODE (GGA_DECODE | RMC_DECODE)
#define SV_DECODE  (GSA_DECODE | GSV_DECODE)

#define NMEA_CHANS (12)

// typedef
typedef enum 
{
	BD_IDX = 0,
	GPS_IDX,
	MAX_SYS
}tSVSYS;

typedef struct _SVDATA
{
	char nSvInView; // 可见卫星数
	char nSvsUsed;  // 使用的卫星数
	unsigned char aucUsedSvid[NMEA_CHANS];
	struct  
	{
		unsigned char svid;
		unsigned char cno;
		unsigned short elv;
		unsigned short amz;
	}aSvInView[NMEA_CHANS];
}tSVData;
// nmea data formation
typedef struct _NMEA
{
	char nMsgMask; // NMEA消息指示，表示已经解码的消息，例如:0x3,表示已经解码了GGA和RMC消息。
	               // 用于表示结构体的数据信息是否有效，每次收到GGA消息时，会将所有的数据清空，
				   // 标示开始重新接收一组新的数据
	char cFixFlag; // RMC,定位有效性标示：'V'=无效；'A'=有效
	char cFixQaulityIdx; // GGA,
	char nUsedSvCount; // GGA,
    char nRxRunMode;    //运行mode
	char nAnttenaStatus; // 天线检测语句
	char cLatIdx;  // 纬度的方向指示，'N'= 北纬；'S'=南纬; 0 = 未知状态
	char cLonIdx;  // 经度的方向指示，'W'= 西经；'E'=东经; 0 = 未知状态
	unsigned short usHours; // UTC时间：时-hh
	unsigned short usMinutes; // UTC时间：分-mm
	float fSeconds;			// UTC时间：秒-ss.sss
	double fLatitude; // 纬度，度分格式(ddmm.mmmm)
	double fLongitude; // 经度，度分格式(dddmm.mmmm)
	double fLatInDegree; // 纬度，单位：度(deg)
	double fLonInDegree; // 经度，单位：度(deg)
	float fAltitude;	// 天线离海平面的高度-椭球体高度（WGS84/CGS2000）
	float fGeoidHeight;  // 大地水准面高度
	float fGroundVel; // 水平速度（地速），单位：节（knot）
	float fHeading;   // 航向角，指北时为0,单位：度(deg)
	float fHdop;
	unsigned short usYear; // UTC时间，年
	unsigned char ucMonth; // UTC时间，月
	unsigned char ucDay; // UTC时间，日
	unsigned char ucFixModeIndex;
	tSVData aSvInfo[MAX_SYS];  // 0:BD, 1: GPS
}tNmea_Msg;

// function declare
//extern unsigned char CheckSum(const char *pMsg,int len);
extern unsigned int NmeaDecode(const char* pStr,tNmea_Msg *pNmea);
extern unsigned int DecodeGga(const char* pStr,tNmea_Msg *pNmea);
extern unsigned int DecodeGsa(const char* pStr,tNmea_Msg *pNmea);
extern unsigned int DecodeGsv(const char* pStr,tNmea_Msg *pNmea);
extern unsigned int DecodeRmc(const char* pStr,tNmea_Msg *pNmea);
extern unsigned int DecodeAntennaChk(const char* pStr,char *AntSts);
extern unsigned int IntegrityAndCorrectCheck(const char *pStr);
extern unsigned char NmeaGetRxRunMode(void);
extern double format2deg(double x);

// std lib
extern long str2lx(const char *s,char **endptr,int base);
extern double str2dx(const char *s,char **endptr);
extern int strlenx(const char* pstr);
unsigned char CheckSum(const char *pMsg,int *Idx);

#endif
