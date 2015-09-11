#ifndef INNOV_BOX_TYPES_H
#define INNOV_BOX_TYPES_H

//Innov Box Return values 
typedef enum _Innov_Box_Retval
{
	R_INNOV_BOX_END = -10000,

	R_INNOV_BOX_COMMON_END = -599,
	R_INNOV_BOX_COMMON_PARAMETER_ERR,							//参数错误
	R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR,						//load配置文件失败
	R_INNOV_BOX_COMMON_DEBUG_UNINITIALIZED,					//没有找到指定模块，或者debug没有初始化
	R_INNOV_BOX_COMMON_DEBUG_LEVEL_ERR,						//调用debug_printf 或debug_printf_mod时i_level的设置不正确
	R_INNOV_BOX_COMMON_ITEM_NOFOUND,							//没有找到指定条目
	R_INNOV_BOX_COMMON_BUF_LACKED,								//buf空间不足
	R_INNOV_BOX_COMMON_ITEM_NOT_NUMERICAL,					//条目内容不是数值
	R_INNOV_BOX_COMMON_ITEM_NOT_ENUMERATED,					//条目内容不是枚举值
	R_INNOV_BOX_COMMON_ITEM_NOT_BOOLEAN,						//条目内容不是布尔值
	R_INNOV_BOX_COMMON_ITEM_ENUMER_OUT_RANGE,				//条目内容是枚举类型，但超出了设定的范围
	//放入common的返回值
	R_INNOV_BOX_COMMON_BEGIN = -300,

	R_INNOV_BOX_CONTROL_ERR_END = -299,
	R_INNOV_BOX_CONTROL_COMMUNICATION_WITH_MONITOR_ERR,	//与controler_monitor进程建立通讯连接错误
	R_INNOV_BOX_CONTROL_INI_ERR,									//读取配置文件controler.ini错误
	R_INNOV_BOX_CONTROL_SERVER_INIT_ERR,						//controler和其他进程的通讯服务初始化错误
	R_INNOV_BOX_CONTROL_CFG_ERR,								//读环境参数配置文件control.cfg错误
	R_INNOV_BOX_CONTROL_SELECT_ERR,								//select错误
	R_INNOV_BOX_CONTROL_FORK_ERR,								//fork错误
	R_INNOV_BOX_CONTROL_SOCKET_ERR,								//socket错误
	R_INNOV_BOX_CONTROL_PARAMETER_ERR,							//参数错误
	R_INNOV_BOX_CONTROL_INVALID_MESSAGE,						//非法消息
	//放入controler的错误返回值......
	R_INNOV_BOX_CONTROL_ERR_BEGIN = -200,

	
	R_INNOV_BOX_NET_END = -199,
	//放入netmanager的返回值
	R_INNOV_BOX_NET_IWLIST_ERR,
	R_INNOV_BOX_NET_MASK_ERR,
	R_INNOV_BOX_NET_IP_ERR,
	R_INNOV_BOX_NET_3GINIT_ERR,
	R_INNOV_BOX_NET_WIFIINIT_ERR,
	R_INNOV_BOX_NET_INIT_ERR,
	R_INNOV_BOX_NET_INIT_ONCE,
	R_INNOV_BOX_NET_BEGIN = -100,


	//放入diskmanager的返回值
	R_INNOV_BOX_DISK_END = -99,
	R_INNOV_BOX_DISK_MOUNT_ERR,
	R_INNOV_BOX_DISK_CHECK_OK,
	R_INNOV_BOX_DISK_INIT = -20,
	

	R_INNOV_BOX_SOCK_CREATE_ERR ,
	R_INNOV_BOX_SOCK_BIND_ERR,
	R_INNOV_BOX_SOCK_RECV_ERR,
	R_INNOV_BOX_SOCK_BUFLEN_ERR,
	R_INNOV_BOX_SOCK_BUFMALLOC_ERR,
	R_INNOV_BOX_SOCK_TIMEOUT,
	R_INNOV_BOX_SOCK_OPT_ERR = -10,


	R_INNOV_BOX_ERROR = -1,
	R_INNOV_BOX_OK = 0,


	R_INNOV_BOX_CONTROL_MESSAGE_BEGIN = 200,
	R_INNOV_BOX_CONTROL_TIMEOUT,								//select读消息超时
	//放入controler的消息返回值......
	R_INNOV_BOX_CONTROL_MESSAGE_END = 299,
	
}Innov_Box_Retval;

typedef int 		IB_CONN_HNDL;
typedef void *  	XML_ELEMENT;


//innnov box debug  level 
#define IB_DBGL_FATAL			0x01       // will exit/abort
#define IB_DBGL_ERR			       0x02       // continues
#define IB_DBGL_WARN			0x04       // only warning
#define IB_DBGL_INFO				0x08      // help message
#define IB_DBGL_DEBUG             	0x10      // debug 

#define IB_DBGL_ON				0x1F
#define IB_DBGL_OFF 				0x10

#define IB_CONN_ETH                       0x01
#define IB_CONN_WIFI                     0x02
#define IB_CONN_3G                         0x04
#define IB_CONN_ALL                       0x1F

#define IB_CONN_HNDL_NULL   		R_INNOV_BOX_ERROR
#define XML_ELEMENT_NULL     		NULL

#endif
