#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

#define STR_LEN 32

#include <string>
using namespace std;

struct updateInfo
{
    char device_id[STR_LEN]; //tuid
    char start[STR_LEN]; // 升级开始时间
    char end[STR_LEN]; // 升级结束时间
    char old_version[STR_LEN]; //可选，如果升级前无版本，则不包括该字段
    char new_version[STR_LEN]; //当前版本
    char result[STR_LEN]; //升级结果，成功则为"OK", 其他为失败原因
    char old_model[STR_LEN]; //旧版本的终端型号
    char new_model[STR_LEN]; //新版本的终端型号
};

#endif

