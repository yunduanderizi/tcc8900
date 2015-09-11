#include "parseConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "debugMonitor.h"


namespace AudioControl1
{





/**********************************************************************************

0 -- //成功
-1 -- //没找到
-2 -- //读配置文件出错
-3 -- //buf空间不够
-4 -- //有语法错误
-5 -- //枚举超出范围
//字母(大小写)，空格，table，_，-，为有效字段

***********************************************************************************/


//行语法检查的类型
typedef enum _Check_Type
{
	CheckAll = 0,						//字符要在字母(大小写)、数字、-、_、空格、table、/n、/r的范围内，其余算非法
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



static int gi_last_line = 0;									//记录前一次从配置文件读出来的行的行号
static Line_Info	*gp_line_info_head = NULL;				//行信息链表头
static char *gc_file_name = NULL;							//配置文件名


//打印配置文件错误信息
//i_line_id : 行号
//pc_str : 错误原因
static int _config_file_error_message(int i_line_id, char *pc_str = "")
{
	IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] error: parsing config file (%s) line (%d) error causes (%s)\n", gc_file_name,  i_line_id, pc_str);
	return 0;
}

//分配一个新的Line_Info,并存入行信息链表
//返回值: 成功返回0
//                   失败返回负值
static int _add_info(int i_line_id, Line_Type line_type, char *pc_type_name, char *pc_content)
{
	Line_Info *pTemp = gp_line_info_head;
	Line_Info *pInfo = (Line_Info *)malloc(sizeof(Line_Info));
	if( NULL == pInfo )
	{
		printf("file(%s)function(%s) malloc error!!!\n", __FILE__, __FUNCTION__);
	}

	pInfo->pNext = NULL;
	pInfo->iLineNum = i_line_id;
	pInfo->LineType = line_type;
	pInfo->pcTypeName = NULL;
	pInfo->pcContent = NULL;
	if( NULL != pc_type_name )
	{
		pInfo->pcTypeName = (char*)malloc(strlen(pc_type_name)+1);
		if( NULL == pInfo->pcTypeName )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "malloc error\n");
			abort();
		}
		strcpy(pInfo->pcTypeName, pc_type_name);
	}
	if( NULL != pc_content )
	{
		pInfo->pcContent = (char*)malloc(strlen(pc_content)+1);
		if( NULL == pInfo->pcContent )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "malloc error\n");
			abort();
		}
		strcpy(pInfo->pcContent, pc_content);
	}
	while( 1 )
	{
		if( NULL == pTemp )
		{//链表为空
			gp_line_info_head = pInfo;
			break;
		}

		if( NULL == pTemp->pNext)
		{//找到行尾
			pTemp->pNext = pInfo;
			break;
		}

		pTemp = pTemp->pNext;
	}

	return 0;	
}

//从行信息链表中找到和段名、条目名匹配的Line_Info
//返回值: 返回找到的行信息
//                   找不到返回NULL
static const Line_Info* _get_line_info(char* pc_section_name, char* pc_item_name)
{
	Line_Info *pTemp = gp_line_info_head;
	bool bFindSection = false;//是否发现段
	
	while( NULL != pTemp )
	{
		if( LineSection == pTemp->LineType )
		{
			if( bFindSection )
			{//原来已经找到了段，现在又碰到一个新段
				pTemp = NULL;
				break;
			}
						
			if( 0 == strcmp(pTemp->pcTypeName, pc_section_name) )
			{//段名相符
				bFindSection = true;
			}
		}
		else if( LineItem == pTemp->LineType )
		{
			if( bFindSection )
			{//已经找到了段，才去找条目
				if( 0 == strcmp(pTemp->pcTypeName, pc_item_name) )
				{//条目名相符
					break;
				}
			}
		}

		pTemp = pTemp->pNext;
	}

	return pTemp;
}

//校验一个字符串是否合法
//返回值: 合法返回true
//                   不合法返回false
static bool _check_str_valid(char* p_str, Check_Type type)
{
	//'\r' ASCII 是13      回车，将当前位置移到本行头
	//'\n' ASCII是10      换行，将当前位置移到下一行
	//ASCII 9是TAB，
	char all[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		             'A','B','C','D','D','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		             '0','1','2','3','4','5','6','7','8','9',
                             '-','_',' ','/','.',13,10,9,0};

	char blank[] = {'-','_',' ',13,10,9,0};		

	char *pCollect = NULL;//用于校验的字符集
	char *pTemp = NULL;
	char *pTempCollect = NULL;
	bool bResult = true;//校验结果
	
	if( CheckAll == type )
	{
		pCollect = all;
	}
	else if( CheckBlank == type )
	{
		pCollect = blank;
	}

	if( NULL != pCollect )
	{
		pTemp = p_str;
		while( 0 != *pTemp )
		{
			pTempCollect = pCollect;
			bool bFind = false;//是否找到了某个字符
			//开始校验某个字符
			while( 0 != *pTempCollect )
			{
				if( *pTempCollect == *pTemp )
				{
					bFind = true;
					break;
				}
				pTempCollect++;
			}
			if( !bFind )
			{
				//没找到，说明这个字符不在用于校验的字符集中
				//printf(" value = %d \n",*pTemp);
				bResult = false;
				break;
			}
			pTemp++;
		}
	}

	return bResult;
}

//校验一个字符串是否是数字
//返回值: 是数字返回true
//                   不是数字返回false
bool _check_str_num(char *pc_str)
{
	if( NULL == pc_str )
	{
		return false;
	}

	for(int i=0; i<(int)strlen(pc_str); i++)
	{
		if( *(pc_str+i) < 48 || *(pc_str+i) >57 )
		{
			return false;
		}	
	}

	return true;
}

//去字符串左右空格
static void _trim_str(char* p_str)
{
	char *pFirst = NULL;//第一个非空字符
	char *pLast = NULL;//最后一个非空字符

	if( NULL == p_str )
	{
		return;
	}
	
	int len = strlen(p_str);
	if( 0 == len )
	{//空字符串没必要去空格
		return;
	}
	
	//首先找到第一个非空字符
	pFirst = p_str;
	while(1)
	{
		if( ' ' != *pFirst && 9 != *pFirst && 10 != *pFirst && 13 != *pFirst )
		{
			break;
		}

		pFirst++;
	}

	//找到最后一个非空字符
	pLast = p_str+len-1;
	while( pLast >= p_str )
	{
		if( ' ' != *pLast && 9 != *pLast && 10 != *pLast && 13 != *pLast )
		{
			break;
		}

		pLast--;
	}

	//拷贝非空字符串
	if( pLast >= pFirst )
	{
		*(pLast+1) = 0;
		strcpy(p_str, pFirst);
	}
	else
	{
		strcpy(p_str, "");
	}

	return;
}

void tools_trimStr(char* pStr)
{
	_trim_str(pStr);
}

//解析配置文件信息，结果存入结构体数组configInfo中
//返回值: 成功0
//                   装载错误-1
int read_config_load(const char *filename)
{
	FILE *pf_fd;
	char tmpbuf[1023+2];
	int retval = 0, i_linetype = 0;
	int line = 0;

	read_config_unload();

	pf_fd = fopen(filename,"r");
	if(!pf_fd)
	{
		//IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] error: config file (%s) cann't be opened\n", filename);
		return -1;
	}

	gc_file_name = (char*)malloc(strlen(filename)+1);
	if( NULL == gc_file_name )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] malloc error!\n");
		abort();
	}
	strcpy(gc_file_name, filename);
		
	while(fgets(tmpbuf,1023+2,pf_fd) )
	{
		line ++;
		//首先判断读出的行是否小于等于1023
		if( strlen(tmpbuf) > 1023 )
		{
			_config_file_error_message(line, (char *)"length of line is up to 1023");
			read_config_unload();	
			return -5;
		}
		
		char *pc_str1 = NULL , *pc_str2 = NULL;
		pc_str1 = tmpbuf;
		pc_str2 = strsep(&pc_str1,"#");
		if(strchr(pc_str2,'[') && strchr(pc_str2,']'))
		{
			i_linetype = 1;//段行
		}
		else if(strchr(pc_str2,'='))
		{
			i_linetype = 2;//条目行
		}
		else
		{
			i_linetype = 3;//空行
		}

		switch(i_linetype)
		{
			case 1:
			{
				pc_str1 = strsep(&pc_str2,"[");
				//检查"["前的字符串是否为空
				if(!_check_str_valid(pc_str1,CheckBlank))
				{					
					_config_file_error_message(line, (char *)"syntax error");
					read_config_unload();	
					return -1;
				}
				
				pc_str1 = strsep(&pc_str2,"]");
				//检查"]"后的字符串是否为空
				if(!_check_str_valid(pc_str2,CheckBlank))
				{
					_config_file_error_message(line, (char *)"syntax error");
					read_config_unload();	
					return -1;
				}

				//校验"["和"]"之间的字符串
				if(!_check_str_valid(pc_str1,CheckAll))
				{
					_config_file_error_message(line, (char *)"syntax error");
					read_config_unload();	
					return -1;
				}
				_trim_str(pc_str1);
				//判断段名是否为空
				if(strcmp(pc_str1, "") == 0) 
				{
					_config_file_error_message(line, (char *)"section name cann't be blank");
					read_config_unload();
					return -1;
				}
				//判断段名是否超过255长度限制
				if( strlen(pc_str1) > 255 )
				{
					_config_file_error_message(line, (char *)"length of section name is up to 255");
					read_config_unload();	
					return -5;
				}
				//将段载入内存
				_add_info(line,LineSection,pc_str1,NULL);
				break;
			}
			case 2:
			{
				pc_str1 = strsep(&pc_str2,"=");
				//校验条目名和条目内容
				if(!(_check_str_valid(pc_str1,CheckAll) && _check_str_valid(pc_str2,CheckAll)))
				{
					_config_file_error_message(line, (char *)"syntax error");
					read_config_unload();	
					return -1;
				}
				_trim_str(pc_str1);
				_trim_str(pc_str2);
				//判断条目名是否为空
				if(strcmp(pc_str1, "") == 0)
				{
					_config_file_error_message(line, (char *)"syntax error");
					read_config_unload();
					return -1;
				}
				//判断条目名是否超过255长度限制
				if( strlen(pc_str1) > 255 )
				{
					_config_file_error_message(line, (char *)"length of item name is up to 255");
					read_config_unload();	
					return -5;
				}

				//判断条目内容是否超过255长度限制
				if( strlen(pc_str2) > 255 )
				{
					_config_file_error_message(line, (char *)"length of item content is up to 255");
					read_config_unload();	
					return -1;
				}

				_add_info(line,LineItem,pc_str1,pc_str2);
				break;
			}
			case 3:
			{				
				if(!_check_str_valid(pc_str2,CheckBlank))
				{
					_config_file_error_message(line, (char *)"syntax error");
					read_config_unload();	
					return -1;
				}
			}

				
		}
		if(retval<0)
			break;
	}
	fclose(pf_fd);
	return retval;

}

//读取pc_section_name段中pc_item_name条目的字符串信息
//返回值: -1 -- 没有找到指定条目
//                   -2 -- buf空间不够
int read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len)
{
	int retval = 0;		
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( p_tmpline->pcContent )
		{
			if( i_len < ((int)(strlen(p_tmpline->pcContent))+1) )
			{
				retval = -2;
			}
			else
			{
				gi_last_line = p_tmpline->iLineNum;
				strcpy(pc_buf, p_tmpline->pcContent);
			}
		}
		else
		{
			if( i_len > 0 )
			{
				pc_buf[0] = 0;
			}
		}
	}
	else
	{
		//IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] error: cann't find item(%s) on section(%s) on config file (%s)\n", pc_item_name, pc_section_name, gc_file_name);
		retval = -1;	
	}
	return retval;
}

//读取pc_section_name段中pc_item_name条目的数值信息
//返回值: -1 -- 没有找到指定条目
//                   -2 -- 条目内容不是数值
int read_config(char* pc_section_name, char* pc_item_name, int *pi_value)
{
	int retval = 0;	
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline =  _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( _check_str_num(p_tmpline->pcContent) )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pi_value = atoi(p_tmpline->pcContent);
			retval = 0;
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a number");
			retval = -2;			
		}
	}
	else
	{
		//IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] error: cann't find item(%s) on section(%s) on config file (%s)\n", pc_item_name, pc_section_name, gc_file_name);
		retval = -1;	
	}
	return retval;
}

//读取pc_section_name段中pc_item_name条目的布尔信息
//返回值: -1 -- 没有找到指定条目
//                   -2 -- 条目内容不是布尔类型
int read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge)
{
	int retval = 0;
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( 0 == strcmp(p_tmpline->pcContent, "yes") )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pb_judge = true;
			retval = 0;
		}
		else if( 0 == strcmp(p_tmpline->pcContent, "no") )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pb_judge = false;
			retval = 0;
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a bool type");
			retval = -2;
		}
	}
	else
	{
		//IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] error: cann't find item(%s) on section(%s) on config file (%s)\n", pc_item_name, pc_section_name, gc_file_name);
		retval = -1;	
	}
	return retval;
}
//读取pc_section_name段中pc_item_name条目的枚举信息
//返回值: -1 -- 没有找到指定条目
//                   -2 -- 条目内容不是枚举类型
//                   -3 -- 条目内容所代表的枚举类型越界
int read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area)
{
	int retval = 0;
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( _check_str_num(p_tmpline->pcContent) )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pi_num = atoi(p_tmpline->pcContent);
			retval = 0;			
			if( i_area < *pi_num || 0 > *pi_num )
			{
				_config_file_error_message(p_tmpline->iLineNum, (char *)"item content is over area");
				retval = -3;
			}
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a enum type");
			retval = -2;
		}
	}
	else
	{
		//IBD_PRINTF(DEBUG_INFOMATION, "[parseConfig] error: cann't find item(%s) on section(%s) on config file (%s)\n", pc_item_name, pc_section_name, gc_file_name);	
		retval = -1;		
	}
	return retval;
}

//释放结构体数组gpp_line_info保存的所有配置信息
int read_config_unload()
{
	Line_Info *p_next,*p_tmpline ;
	p_tmpline = gp_line_info_head;
	while(p_tmpline)
	{
		p_next = p_tmpline->pNext;
		if( NULL != p_tmpline->pcTypeName )
		{
			free(p_tmpline->pcTypeName);
			p_tmpline->pcTypeName = NULL;
		}
		if( NULL != p_tmpline->pcContent )
		{
			free(p_tmpline->pcContent);
			p_tmpline->pcContent = NULL;
		}
		free(p_tmpline);
		p_tmpline = p_next;
	}

	gp_line_info_head = NULL;
	
	if( NULL != gc_file_name )
	{
		free(gc_file_name);
		gc_file_name = NULL;
	}
	
	return 0;
}

//打印上一行的逻辑错误信息
void read_config_logic_error()
{
	if( NULL != gc_file_name )
	{
		if( gi_last_line >= 0 )
		{
			_config_file_error_message(gi_last_line, (char *)"logic error");
			//IBD_PRINTF(DEBUG_INFOMATION, " ----config  file[%s]   in  line[%d]   have  a  logic  error   ----\n",gc_file_name,gi_last_line);
		}
		else
		{
			//没有读出任何行
			//printf(" ----There isn't a line readed , or a error occur when a line is readed !!! ----\n");
		}
	}
	else
	{
		//没有config 文件被load
		//printf(" ----There isn't any config file loaded!!!   ----\n");
	}
}


/*

int testConfig()
{
	_syntax_error("controler.ini", 123);

	_add_info(123, LineSection, "communication", NULL);
	_add_info(123, LineItem, "mode", "0");

	printf("head(%x), head next(%x) head next next(%x)\n", gp_line_info_head, gp_line_info_head->pNext, gp_line_info_head->pNext->pNext);

	const Line_Info* pTmp = _get_line_info("communication", "mode");
	printf("pTmp(%x)\n", pTmp);

	const Line_Info* pTmp1 = _get_line_info("communication1", "mode");
	printf("pTmp1(%x)\n", pTmp1);

	bool result = _check_str_valid("fasdf  	", CheckAll);
	printf("result(%d)\n", result);

	result = _check_str_valid("fasd&f  	", CheckAll);
	printf("result(%d)\n", result);

	result = _check_str_valid("fasdf  	", CheckBlank);
	printf("result(%d)\n", result);

	result = _check_str_valid("   		  	", CheckBlank);
	printf("result(%d)\n", result);

	char test[] = "  sad		  ";
	_trim_str(test);
	printf("test(%s)\n", test);

	char test1[] = "000";
	_trim_str(test1);
	printf("test1(%s)\n", test1);
}

*/


/*解析一个字符串，将它按照空格或者table键分隔成多个字符串
pOldStr原始字符串
ppStr解析出的字符串数组
iStrArraylen数组的长度
注意:新字符串不重新分配空间，利用pStr中的空间，只是将其中一些位置变为0
*/
int ParseStr(char* pOldStr, char **ppStr, int iStrArraylen)
{
	int len = strlen(pOldStr)+1;
	int num = 0;
	char *pBefore = NULL;

	for(int i=0; i<iStrArraylen; i++)
	{
		ppStr[i] = NULL;
	}
	
	for(int i=0; i<len; i++)
	{
		if( ' ' == pOldStr[i] || '	' == pOldStr[i] || '\0' == pOldStr[i])
		{
			if('\0' == pOldStr[i])
				break;
			pOldStr[i] = 0;
			if( NULL != pBefore )
			{
				ppStr[num++] = pBefore;
				if( num >= iStrArraylen )
				{
					break;
				}
			}	
			pBefore = NULL;
		}
		else
		{
			if( NULL == pBefore )
			{
				pBefore = &(pOldStr[i]);
			}
		}
	}

	if( NULL != pBefore )
	{
		ppStr[num++] = pBefore;
	}	

	ppStr[num] = NULL;
	
	return num;
}

bool CheckStrNum(char *pc_str)
{
	if( NULL == pc_str )
	{
		return false;
	}

	for(int i=0; i<(int)strlen(pc_str); i++)
	{
		if( *(pc_str+i) < 48 || *(pc_str+i) >57 )
		{
			return false;
		}	
	}

	return true;
}

}


