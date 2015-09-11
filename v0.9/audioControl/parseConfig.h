#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H



namespace AudioControl1
{



/**********************************************************************************

0 -- //成功
-1 -- //没找到
-2 -- //读配置文件出错
-3 -- //buf空间不够
-4 -- //有语法错误

//字母(大小写)，空格，table，_，-，为有效字段

***********************************************************************************/


int testConfig();

//解析配置文件信息，结果存入结构体数组configInfo中
int read_config_load(const char *filename);
//读取pc_section_name段中pc_item_name条目的字符串信息
int read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len);
//读取pc_section_name段中pc_item_name条目的数值信息
int read_config(char* pc_section_name, char* pc_item_name, int *pi_value);
//读取pc_section_name段中pc_item_name条目的布尔信息
int read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge);
//读取pc_section_name段中pc_item_name条目的枚举信息
int read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area);
//释放结构体中保存的所有配置信息
int read_config_unload();
//打印逻辑错误信息
void read_config_logic_error();
//去字符串左右空格
void tools_trimStr(char* pStr);

int ParseStr(char* pOldStr, char **ppStr, int iStrArraylen);

bool CheckStrNum(char *pc_str);


}

#endif
