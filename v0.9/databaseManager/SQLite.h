#ifndef SQLITE_H
#define SQLITE_H
#include "sqlite3.h"

#define SIZECONTENT 500
#define SIZENUM 30
#define DB_PATH "/nand3/userdata/"
#define DB_SERV_NAME "Service.db"

//短信箱
typedef struct _SmsInfo                   
{                                                                    
	int nId;                                   // 序号                  
	char chFromPhone[24];            // 来源号码             
	char chFriendName[12];           // 好友姓名        
	char chTime[36];                      // 短信时间                
	char chContent[256];                // 短信内容(文字描述)  
	char chPoiName[46];                // Poi名称                 
	double dLon;                          // 经度                           
	double dLat;                           // 纬度                       
	int byStatus;                          // 0未读 1已读        
	char name[22];
	int bookId;
}SmsInfo;
//通讯录结构体
typedef struct _teleBookInfo		
{
	int nId;						//通讯录的ID
	char friendName[22];  			//人名
	char friendPhone[24];   			//手机号
	char friendEmail[48];				//邮箱
	char friendCompany[46];			//公司
	char friendComAddress[62];		//公司地址
	char friendFamAddress[62];		//家庭住址
	char friendPhoto[64];				//照片路径
	char friendTelephoneNum[24];		//固定电话号码
	char friendSound[24];			//铃声
	int friendVideo;					//是否支持视频
}teleBookInfo;	
//通话记录结构体
typedef struct _teleHistoryInfo		
{
	int nId;			//通话记录ID
	int phoneType;		//通话类型，1拨出电话2未接来电3已接来电
	char phoneNum[24];	//电话号码
	char startTime[36];	//开始时间
	char hTime[36]	;
	int video;//是否是视频电话记录
	char name[22];
	int bookId;
}teleHistoryInfo;
//发件箱结构体
typedef struct _sendMsg
{
	int nId;                                   // 序号                  
	char chToPhoneNumber[24];            // 来源号码               
	char chToTime[36];                      // 短信时间                
	char chToContent[500];                // 短信内容(文字描述)  
	char name[22];
	int bookId;
}sendInfo;

//读取短消息
int PoiMsg_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadPoiMsg(SmsInfo **pPoiMsg, int *nCount);
int Sql_ReleasePoiMsg(SmsInfo **pPoiMsg, int *nCount);

//读取当前最新的短信
int PoiMsgLatest_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadLatestPoiMsg(SmsInfo *pPoiMsg);

//读取当前最新的发件箱的短信
int Sql_ReadLatestSendMsg(sendInfo *pPoiMsg);



//读取当前最新的电话记录的id
int Sql_ReadLatestTelephoneHistoryId(int *id);
//读取当前最新的通讯录的id
int Sql_ReadLatestTelephoneBookId(int *id);
//读取当前最新的发件箱的id
int  Sql_ReadLatestSendMsgId(int *id);


 
//读取通讯录
int TelephoneBook_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadTelephoneBook(teleBookInfo **pTeleBook, int *nCount);
//通过人名查询通讯录中的第一条
int Sql_ReadTelephoneBookByName(teleBookInfo *pTeleBook,char *phoneName);
//释放查询通讯录数据的内存
int Sql_ReleaseTelephoneBook(teleBookInfo **pTeleBook, int *nCount);

//匹配人名的通讯录记录
int TelephoneBook_Allcallback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadAllTelephoneBookByName(teleBookInfo **pTeleBook,int *nCount,char *phoneName);
//释放查询的结果的内存
int Sql_ReleaseAllTelephoneBook(teleBookInfo **pTeleBook, int *nCount);

//通过电话号码查询通讯
int Sql_ReadTelephoneBookByPhoneNum(teleBookInfo *pTeleBook,char *phoneNum);
//通过固话号码查询通讯录
int Sql_ReadTelephoneBookByTelephoneNum(teleBookInfo *pTeleBook,char *phoneNum);

//删除所有短信
int Sql_DeleteAllMsg();
//更新短信状态
int Sql_UpdateMsg(int msgId);
//读取未接来电
int TelephoneHistory_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadTelephoneHistory(teleHistoryInfo **pTeleBook, int *nCount);
//读取已接电话
int TelephoneCalled_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadTelephoneCalled(teleHistoryInfo **pTeleBook, int *nCount);
//读取已拨电话
int TelephoneDialed_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadTelephoneDialed(teleHistoryInfo **pTeleBook, int *nCount);
//插入电话记录
int Sql_InsertTelephoneHistory(  void *pInsertData, int dataSize, int type);
//全部删除未接来电
int  Sql_DeleteMiss();
//全部删除已接来电
int  Sql_DeleteCalled();
//全部删除已拨电话
int  Sql_DeleteDial();
//删除单条短信
int Sql_DeletePoiMsgByIndex(int nIndex);
//删除单条通话记录
int Sql_DeleteTeleHistoryByIndex(int nIndex);
//全部删除通讯录
int  Sql_DeleteTelephoneBook();
//删除单条通讯录
int Sql_DeleteTeleBookByIndex(int nIndex);
int Sql_ReleaseTelephoneHistory(teleHistoryInfo **pTeleHistory,  int *nCount);
//录入通讯录
int Sql_InsertTelephoneBook(  void *pInsertData,int dataSize);
//更新通讯录
int Sql_UpdateTelephoneBookById(void *pUpdateData,int dataSize,int id);
//读取发件箱
int SendMsgBox_callback(void *NotUsed, int argc, char **argv, char **azColName);
int Sql_ReadSendMsgBox( sendInfo**pPoiMsg, int *nCount);
//插入发件箱
int Sql_InsertSendMsgBox(  void *pInsertData, int dataSize);
//释放查询发件箱数据的内存
int Sql_ReleaseSendMsgBox(sendInfo **pPoiMsg, int *nCount);	
//清空发件箱
int Sql_DeleteAllSendMsgBox();
//删除单条发件箱中记录
int Sql_DeleteSendMsgBoxByIndex(int nIndex);

#endif
