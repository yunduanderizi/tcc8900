#ifndef SMSCMDPROTOCOL_H
#define SMSCMDPROTOCOL_H

#include <QObject>
#include <QMap>
#include <QStringList>

/******************************************
起始标识  密码	指令类型	 指令	指令内容
1字节    6字节	不定长	 不定长	不定长
‘#’    “123456”	RD或ST
******************************************/

#define SMS_START_LABEL '#'

//指令类型 查询指令为RD; 设置指令为ST; 其他指令自定义
typedef enum _SMS_CMD_TYPE
{
    QUERY_CMD_TYPE=0,
    SETUP_CMD_TYPE,
    ERROR_CMD_TYPE
}SMS_CMD_TYPE;

//指令
typedef enum _SMS_CMD_SELF
{
    MDM_CMD=0,
    MIP_CMD,
    DSN_CMD,
    APN_CMD,
    LIC_CMD,
    LPC_CMD,
    DTM_CMD,
    ATM_CMD,
    STM_CMD,
    MSP_CMD,
    SPM_CMD,
    FAC_CMD,
    PSP_CMD,
    COUNT_CMD //代表总数，无实际意思
}SMS_CMD_SELF;

//指令结构体
typedef struct _SMS_CMD_STRUCT
{
    SMS_CMD_TYPE type;
    SMS_CMD_SELF cmd;
    QString passwd;
    QStringList args;
    QString reply;
    char phone[32];
}SMS_CMD_STRUCT;

//指令判断结果
typedef enum _SMS_CMD_PARSE_RESULT
{
    PASSWD_ERROR_PARSE=0, //Password error
    UNKNOW_CMD_PARSE,     //Unknow Cmd
    FORMAT_ERROR_PARSE,   //Format error
    OTHER_ERROR_PARSE,    //其他错误原因
    MODIFY_PHONE_PARSE,   //超级密码修改短信指令号码
    SUCCESS_PARSE         //成功
}SMS_CMD_PARSE_RESULT;

//此头文件必须放在 SMS_CMD_STRUCT 后,里边会用到此定义 [zhenyubin 2015/08/27]
#include "phoneMonitorVoiceReportBase.h"

class SmsCmdProtocol : public QObject
{
    Q_OBJECT
public:
    explicit SmsCmdProtocol(QObject *parent = 0);

    //获取 指令冒号后边的数字参数
    bool getCmdIntArgs(QString src,QString &dest);
    SMS_CMD_PARSE_RESULT parseSmsCommand(ReportSMSData smsData,SMS_CMD_STRUCT &result);
    QString packageSmsCommand(SMS_CMD_STRUCT send,bool isSet=false);

    void readAuthorizePhone();
    void modifyAuthorizePhone(QString phone);

private:
    QStringList cmdArrayList;
    QString phoneNumber;
};

#endif // SMSCMDPROTOCOL_H
