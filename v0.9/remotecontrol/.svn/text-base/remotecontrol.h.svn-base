#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

#define CHAR_LEN 64

#include <string>
using namespace std;

struct RemoteCtrlInfo
{
    int sendpolicy;
    int sendnum;
    int interval;
    int startpolicy;  
};

struct PostInfo
{
    unsigned char device_id[CHAR_LEN];
    int     position;       //是否定位， 0 未定位， 1定位成功
    int     ilng;		    // 当前GPS坐标经度
    int     ilat;		    // 当前GPS坐标纬度
    int     itime;		    // 当前的终端时间
    int     ialti;          //当前海拔信息
    int     lng_flag;       //当前gps坐标的经度 东经---0   西经---1
    int     lat_flag;       //当前gps坐标的纬度 北纬---0   南纬---1
    char version[CHAR_LEN]; //版本信息
    char vdr_model[CHAR_LEN];
};





int curl_upload(char *url, char *send_buffer, char *receive_buffer);






#endif

