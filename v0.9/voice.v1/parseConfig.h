#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H

//行语法检查的类型
typedef enum _Check_Type
{
	CheckAll = 0,						//字符要在字母(大小写)、数字、-、_、/、点. 、空格、table、/n、/r的范围内，其余算非法
	CheckBlank,							//字符要在空格、table、/n、/r的范围内，其余算非法
}Check_Type;

//行类型
typedef enum _Line_Type
{
	LineSection = 0,						//段行
	LineItem,							//条目行
}Line_Type;

//行信息结构体
typedef struct _Line_Info{
	int iLineNum;										//记录该结构体信息在配置文件中的行号
	Line_Type LineType;								//该结构体的行类型
	char *pcTypeName;								//记录条目类型的名字或者段类型的名字
	char *pcContent;									//如果结构体属于条目类型，记录条目类型的值
	struct _Line_Info *pNext;							//指向下一个节点的指针
}Line_Info;


class IBParseConfigTool
{
public:
	//解析配置文件信息，结果存入结构体数组configInfo中
	static int read_config_load(const char *filename, const char check_type=0);
	//读取pc_section_name段中item_num 的条目和条目值
	static int read_config_items(char* pc_section_name, int item_num, char **item_buf, char ** context_buf);
	//读取pc_section_name段中pc_item_name条目的字符串信息
	static int read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len);
	//读取pc_section_name段中pc_item_name条目的数值信息
	static int read_config(char* pc_section_name, char* pc_item_name, int *pi_value);
	//读取pc_section_name段中pc_item_name条目的布尔信息
	static int read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge);
	//读取pc_section_name段中pc_item_name条目的枚举信息
	static int read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area);
	//释放结构体中保存的所有配置信息
	static int read_config_unload();
	//打印逻辑错误信息
	static void read_config_logic_error(char *pc_error_message);

private:
	static int _config_file_error_message(int i_line_id, char *pc_str);
	static int  _add_info(int i_line_id, Line_Type line_type, char *pc_type_name, char *pc_content);
	static  Line_Info*  _get_line_info(char* pc_section_name, char* pc_item_name);
	static bool _check_str_valid(char* p_str, Check_Type type);
	static bool  _check_str_num(char *pc_str);
	static void _trim_str(char* p_str);

};


#endif
