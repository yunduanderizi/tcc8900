#include "parseConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "error_types.h"



/**********************************************************************************

0 -- //成功
-1 -- //没找到
-2 -- //读配置文件出错
-3 -- //buf空间不够
-4 -- //有语法错误
-5 -- //枚举超出范围
//字母(大小写)，空格，table，_，-，为有效字段

***********************************************************************************/


static int gi_last_line = 0;									//记录前一次从配置文件读出来的行的行号
static Line_Info	*gp_line_info_head = NULL;				//行信息链表头
static char *gc_file_name = NULL;							//配置文件名


//打印配置文件错误信息
//i_line_id : 行号
//pc_str : 错误原因
int IBParseConfigTool:: _config_file_error_message(int i_line_id, char *pc_str = "")
{
	if( NULL != gc_file_name )
	{
		if( i_line_id >= 0 )
		{
			printf("[controler] An error occurred when parsing config file %s line %d because %s !\n", gc_file_name,  i_line_id, pc_str);
		}
		else
		{
			printf("[controler] An error occurred when parsing config file %s because %s !\n", gc_file_name,  pc_str);
		}
	}
	else
	{
		printf("[controler] there isn't config file opened!\n");
	}

	return 0;
}

//分配一个新的Line_Info,并存入行信息链表
//返回值: 成功返回0
//                   失败返回负值
int IBParseConfigTool:: _add_info(int i_line_id, Line_Type line_type, char *pc_type_name, char *pc_content)
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
			printf("malloc error\n");
			abort();
		}
		strcpy(pInfo->pcTypeName, pc_type_name);
	}
	if( NULL != pc_content )
	{
		pInfo->pcContent = (char*)malloc(strlen(pc_content)+1);
		if( NULL == pInfo->pcContent )
		{
			printf("malloc error\n");
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
 Line_Info* IBParseConfigTool:: _get_line_info(char* pc_section_name, char* pc_item_name)
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
bool IBParseConfigTool::_check_str_valid(char* p_str, Check_Type type)
{
	//'\r' ASCII 是13      回车，将当前位置移到本行头
	//'\n' ASCII是10      换行，将当前位置移到下一行
	//ASCII 9是TAB，
	char all[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		             'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
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
				printf(" value = %d \n",*pTemp);
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
bool IBParseConfigTool:: _check_str_num(char *pc_str)
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
void IBParseConfigTool::_trim_str(char* p_str)
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


//解析配置文件信息，结果存入结构体数组configInfo中
//返回值: 成功0
//                   装载错误R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR
//check_type         0 -- 只校验段名和条目名，不校验条目内容
//                         1 -- 校验段名、条目名和条目内容
int IBParseConfigTool::read_config_load(const char *filename, const char check_type)
{
	FILE *pf_fd;
	char tmpbuf[1023+2];
	int retval = R_INNOV_BOX_OK, i_linetype = 0;
	int line = 0;

	read_config_unload();

	pf_fd = fopen(filename,"r");
	if(!pf_fd)
	{
		printf("[common] error: config file (%s) cann't be opened\n", filename);
		return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
	}

	gc_file_name = (char*)malloc(strlen(filename)+1);
	if( NULL == gc_file_name )
	{
		printf("[controler] malloc error!\n");
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
			return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
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
					_config_file_error_message(line, (char *)"there cann't be any character before '['");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				
				pc_str1 = strsep(&pc_str2,"]");
				//检查"]"后的字符串是否为空
				if(!_check_str_valid(pc_str2,CheckBlank))
				{
					_config_file_error_message(line, (char *)"there cann't be any character after ']'");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				//校验"["和"]"之间的字符串
				if(!_check_str_valid(pc_str1,CheckAll))
				{
					_config_file_error_message(line, (char *)"there is invalid character between '[' and ']'");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				_trim_str(pc_str1);
				//判断段名是否为空
				if(strcmp(pc_str1, "") == 0) 
				{
					_config_file_error_message(line, (char *)"section name cann't be blank");
					read_config_unload();
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				//判断段名是否超过255长度限制
				if( strlen(pc_str1) > 255 )
				{
					_config_file_error_message(line, (char *)"length of section name is up to 255");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				//将段载入内存
				_add_info(line,LineSection,pc_str1,NULL);
				break;
			}
			case 2:
			{
				pc_str1 = strsep(&pc_str2,"=");

				_trim_str(pc_str1);
				_trim_str(pc_str2);

				if( !_check_str_valid(pc_str1,CheckAll) )
				{
					char tmp[1024];
					sprintf(tmp, "there is invalid character in item name (%s)", pc_str1);
					_config_file_error_message(line, tmp);
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				//校验条目名和条目内容
				if( check_type ==1 )
				{

					if( !_check_str_valid(pc_str2,CheckAll) )
					{
						char tmp[1024];
						sprintf(tmp, "there is invalid character in item content (%s)", pc_str2);
						_config_file_error_message(line, tmp);
						read_config_unload();	
						return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;

					}
				}

				//判断条目名是否为空
				if(strcmp(pc_str1, "") == 0)
				{
					_config_file_error_message(line, (char *)"item name cann't be blank");
					read_config_unload();
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				//判断条目名是否超过255长度限制
				if( strlen(pc_str1) > 255 )
				{
					_config_file_error_message(line, (char *)"length of item name is up to 255");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				//判断条目内容是否超过255长度限制
				if( strlen(pc_str2) > 255 )
				{
					_config_file_error_message(line, (char *)"length of item content is up to 255");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				_add_info(line,LineItem,pc_str1,pc_str2);
				break;
			}
			case 3:
			{				
				if(!_check_str_valid(pc_str2,CheckBlank))
				{
					_config_file_error_message(line, (char *)"this line cann't be parsed");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
			}

				
		}
		if(retval<0)
			break;
	}
	fclose(pf_fd);
	return retval;

}

//读取pc_section_name段中item_num个条目名及条目值
//返回值: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- 没有找到条目
int IBParseConfigTool::read_config_items(char* pc_section_name,int item_num, char **item_buf, char ** context_buf)
{
	int retval = R_INNOV_BOX_OK;	

   	Line_Info *pTemp = gp_line_info_head;
	bool bFindSection = false;//是否发现段
	int num=0;
	
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
                            *item_buf=pTemp->pcTypeName; 
                            *context_buf=pTemp->pcContent; 
                            item_buf++;
                            context_buf++;
                            num++;
                            if(num >= item_num)
                                break;
			}
		}

		pTemp = pTemp->pNext;
	}

      if( !bFindSection)
            retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND; 
	return retval;
    
}

//读取pc_section_name段中pc_item_name条目的字符串信息
//返回值: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- 没有找到指定条目
//                   R_INNOV_BOX_COMMON_BUF_LACKED -- buf空间不够
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len)
{
	int retval = R_INNOV_BOX_OK;		
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( p_tmpline->pcContent )
		{
			if( i_len < ((int)strlen(p_tmpline->pcContent)+1) )
			{
				retval = R_INNOV_BOX_COMMON_BUF_LACKED;
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
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;	
	}
	return retval;
}

//读取pc_section_name段中pc_item_name条目的数值信息
//返回值: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- 没有找到指定条目
//                   R_INNOV_BOX_COMMON_ITEM_NOT_NUMERICAL -- 条目内容不是数值
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, int *pi_value)
{
	int retval = R_INNOV_BOX_OK;	
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline =  _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( _check_str_num(p_tmpline->pcContent) )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pi_value = atoi(p_tmpline->pcContent);
		}
		else
		{
			retval = R_INNOV_BOX_COMMON_ITEM_NOT_NUMERICAL;			
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;	
	}
	return retval;
}

//读取pc_section_name段中pc_item_name条目的布尔信息
//返回值: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- 没有找到指定条目
//                   R_INNOV_BOX_COMMON_ITEM_NOT_BOOLEAN -- 条目内容不是布尔类型
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge)
{
	int retval = R_INNOV_BOX_OK;
	gi_last_line = -1;//清除上一次读取的行的记录
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( 0 == strcmp(p_tmpline->pcContent, "yes") )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pb_judge = true;
		}
		else if( 0 == strcmp(p_tmpline->pcContent, "no") )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pb_judge = false;
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a bool type");
			retval = R_INNOV_BOX_COMMON_ITEM_NOT_BOOLEAN;
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;	
	}
	return retval;
}
//读取pc_section_name段中pc_item_name条目的枚举信息
//返回值: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- 没有找到指定条目
//                   R_INNOV_BOX_COMMON_ITEM_NOT_ENUMERATED -- 条目内容不是枚举类型
//                   R_INNOV_BOX_COMMON_ITEM_ENUMER_OUT_RANGE -- 条目内容所代表的枚举类型越界
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area)
{
	int retval = R_INNOV_BOX_OK;
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
				retval = R_INNOV_BOX_COMMON_ITEM_ENUMER_OUT_RANGE;
			}
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a enum type");
			retval = R_INNOV_BOX_COMMON_ITEM_NOT_ENUMERATED;
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;		
	}
	return retval;
}

//释放结构体数组gpp_line_info保存的所有配置信息
int IBParseConfigTool::read_config_unload()
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
	
	return R_INNOV_BOX_OK;
}

//打印上一行的逻辑错误信息
void IBParseConfigTool::read_config_logic_error(char * pc_error_message)
{
    	_config_file_error_message(gi_last_line, pc_error_message);
}

