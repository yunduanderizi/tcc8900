/**
 * @file   bsManagerErrCode.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110302
 * 
 * @brief  
 *      bsManager module's error code 
 * 
 */
#ifndef BSMANAGERERRCODE_H_20110302
#define BSMANAGERERRCODE_H_20110302


//common error code		(0 ~ -100)
#define BS_E_OK 									0
#define BS_E_ARG_INVALID 						-1
#define BS_E_URL_FILE_ERROR					-2
#define BS_E_URL_HTTP_ERROR					-3
#define BS_E_URL_XML_PARSE_ERROR				-4
#define BS_E_BUSY 								-5
#define BS_E_GET_TUID_ERROR					-6
#define BS_E_GET_UUID_ERROR					-7
#define BS_E_USER_NOT_LOGIN					-8
#define BS_E_DATABASE_ERROR					-9
#define BS_E_INVALID_AP							-10
#define BS_E_USER_NOT_PERMISSION				-11
#define BS_E_UNSUPPORTED						-12

//function return value of BsTrafficNoticeC		(-201 ~ -300)
#define BS_TRAFFIC_CITY_NAME_LONGER		-201
#define BS_TRAFFIC_NOTICE_E_NO_SETCITY 		  -202
#define BS_TRAFFIC_DOWNLOAD_ERROR				-203

//function return value of BsLocationUploadC		(-301 ~ -320)
#define BS_LOCATION_UPLOAD_E_NO_GPS_DBUS_OBJECT			-300
#define BS_LOCATION_UPLOAD_E_NO_LOGIN_OBJECT				-301
#define BS_LOCATION_UPLOAD_E_GET_LOCATION_INFO_FAIL		-302

//function return value of BsWeatherForecastC		(-401 ~ -500)
#define BS_WEATHER_FORECAST_E_NO_CONTENT_URL	-401
#define BS_WEATHER_CITY_NAME_LONGER						-402

//function return value of BsStockInfoC		(-501 ~ -600)
#define BS_STOCK_E_PRASE_XML				-501
#define BS_STOCK_E_WRITEDATA_ERROR   		-502
#define BS_STOCK_E_GET_CONTENT_ERROR 	-503
#define BS_STOCK_E_DOWN_CHART_ERROR		-504

//function return value of BsNewsC		(-601 ~ -700)
#define BS_NEW_DOWNLOAD_ERROR 		-601
#define BS_NEW_PARSE_XML_ERROR		-602
#define BS_NEW_FILE_OPEN_FAIL 		-603

//function return value of BsAuthenticationReportC		(-801 ~ -900)
#define BS_AUTHENTICATION_GET_INFO_ERROR 		-801
#define BS_AUTHENTICATION_NO_MATCHING 			-802
#define BS_AUTHENTICATION_TIME_NO_OUT			-803
#define BS_AUTHENTICATION_UP_DATA_ERROR		-804
#define BS_AUTHENTICATION_GET_DATA_ERROR		-804

//function return value of BsCarInfoUploadC		(-901 ~ -1000)
#define BS_CARINFOUPLOAD_GET_POWER_ON_OFF_REASON_ERROR 		-901

//function return value of BsLoginC				(-1001 ~ -1100)
#define	BK_RETURN_LOGIN_FAILURE				-1001
#define	BK_RETURN_LOGOUT_FAILURE				-1002
#define	BK_RETURN_GET_TUID_FAILURE			-1003
#define	BK_RETURN_USER_CANCEL				-1004
#define	BK_RETURN_DATABASE_ERROR			-1005
#define	BK_RETURN_UI_FACTORY_NULL			-1006
#define   BK_RETURN_USER_GO_BACK				-1007
#define   BK_RETURN_USER_GO_MAIN_MENU		-1008


//function return value of BsPeccancyHintC			(-1101 ~ -1200)
#define BS_PECCANCY_HINT_E_MKDIR_FAILURE		-1101

//function return value of BsCarInfoMonitor		(-1201 ~ -1300)
#define BS_CAR_INFO_MONITOR_E_CAR_INFO_NOT_SUPPORT		-1201
#define BS_CAR_INFO_MONITOR_E_GET_CURRENT_ERROR  		 -1202
#define BS_CAR_INFO_MONITOR_E_CANCEL_ERROR   			-1203
#define BS_CAR_INFO_MONITOR_E_ADD_ERROR 			  -1204
#define BS_CAR_INFO_MONITOR_E_GET_VALID_ERROR  			-1205
#define BS_CAR_INFO_MONITOR_E_CAR_MONITOR_NOT_START	-1206
#define BS_CAR_INFO_MONITOR_E_ADD_SAME_CAR_INFO         -1207
#define BS_CAR_INFO_MONITOR_E_CAR_DATA_UNSUPPORT_SET		-1208
#define BS_CAR_INFO_MONITOR_E_CAR_DATA_UNUSED				-1209

//function return value of BsUcsInfoGather		(-1301 ~ -1400)
#define BS_UCS_INFO_GATHER_E_NOT_INFO		-1301

//function return value of BsUcsInfoGather		(-1401 ~ -1500)
#define BS_CLIENT_INIT_FAILURE					-1401

#endif


