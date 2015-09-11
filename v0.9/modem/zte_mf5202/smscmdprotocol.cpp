#include "smscmdprotocol.h"
#include "debugMonitor.h"
#include <QFile>
#include <QTextStream>

#define PHONE_CONFIG_FILE "/nand3/applicationdata/smscommand.cfg"
#define DEFAULT_AUTOR_PHONE "00000000000"

SmsCmdProtocol::SmsCmdProtocol(QObject *parent) :
    QObject(parent)
{
    phoneNumber=DEFAULT_AUTOR_PHONE;
    //此数组与 SMS_CMD_SELF 枚举对应
    cmdArrayList<<"MDM"<<"MIP"<<"DSN"<<"APN"<<"LIC"<<"LPC"<<"DTM"<<"ATM"<<"STM"<<"MSP"<<"SPM"<<"FAC"<<"PSP";
    readAuthorizePhone();
}

//读取短信指令号码 [zhenyubin 2015/09/02]
void SmsCmdProtocol::readAuthorizePhone()
{
    QString temp="";
    bool exist=false;
    QFile file(PHONE_CONFIG_FILE);

    exist=file.exists();
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---------SMS---open smscommand.cfg failed !\n")
        return;
    }
    QTextStream in(&file);
    if(exist)
    {
        if(!in.atEnd())
        {
            temp=in.readLine();
            phoneNumber=temp;
            IBD_PRINTF(DEBUG_INFOMATION, "---------SMS---read smscommand.cfg phone:%s!\n",phoneNumber.toLatin1().data())
        }
        else
        {
            in<<phoneNumber;
            in.flush();
            IBD_PRINTF(DEBUG_INFOMATION, "---------SMS---smscommand.cfg is null and write phone:%s!\n",phoneNumber.toLatin1().data())
        }
    }
    else
    {
        in<<phoneNumber;
        in.flush();
        IBD_PRINTF(DEBUG_INFOMATION, "---------SMS---smscommand.cfg not exists and write phone:%s!\n",phoneNumber.toLatin1().data())
    }

    file.close();
}

//超级密码修改短信指令号码 [zhenyubin 2015/09/02]
void SmsCmdProtocol::modifyAuthorizePhone(QString phone)
{
    QFile file(PHONE_CONFIG_FILE);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---------SMS---modify open smscommand.cfg failed !\n")
        return;
    }

    QTextStream out(&file);
    phoneNumber=phone;
    out<<phoneNumber;
    out.flush();
    IBD_PRINTF(DEBUG_INFOMATION, "---------SMS---modify smscommand.cfg phone:%s!\n",phoneNumber.toLatin1().data())

    file.close();
}

bool SmsCmdProtocol::getCmdIntArgs(QString src, QString &dest)
{
    //假如参数后跟字母，如端口号6000@，后跟字母，只取前边的数字
    bool ok=false;
    QString str=src;

    dest="";
    for(int i=0;i<str.length();i++)
    {
        if(str[i]>=QChar('0') &&str[i]<=QChar('9'))
        {
            dest.append(str[i]);
        }
        else break;
    }

    if(dest.length()<=0) return false;
    dest.toInt(&ok); //判断是否为数字
    return ok;
}

SMS_CMD_PARSE_RESULT SmsCmdProtocol::parseSmsCommand(ReportSMSData smsData, SMS_CMD_STRUCT &result)
{
    bool isSuperPasswd=false;
    bool isInitNumber=false;
    bool isRightNumber=false;

    //判断号码是否为授权号码
    if(strcmp(phoneNumber.toLatin1().data(),DEFAULT_AUTOR_PHONE) == 0)
    {
        isInitNumber=true;//号码为初始 00000000000,任何号码，只要是设置查询手机号码的指令都可以接受
    }
    else
    {
        if(strcmp(phoneNumber.toLatin1().data(),result.phone) != 0)
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---command phone:[%s] is not Authorized:[%s]!\n",result.phone,phoneNumber.toLatin1().data())
            //return FORMAT_ERROR_PARSE;
            isRightNumber=false;
        }
        else
        {
            isRightNumber=true;
        }
    }

    //找#
    IBD_PRINTF(DEBUG_INFOMATION, "---SMS---content:[%s]\n",smsData.SMSContent)
    QString content=smsData.SMSContent;
    content=content.trimmed(); //去掉开始和结尾的空格

    if(content[0] != QChar('#'))
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---not find #! \n")
        return OTHER_ERROR_PARSE;
    }

    if(content.length()<9) //#123456STMDM:0（0:IP;1：域名） 最少保证#123456ST 前9为个数
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---cmd count less! \n")
        return OTHER_ERROR_PARSE;
    }

    //判断密码是否正确
    QString passwd=content.mid(1,6);
    result.passwd=passwd;
    if((result.passwd.compare("123456")!=0) && (result.passwd.compare("888928")!=0))
    {
        return PASSWD_ERROR_PARSE;
    }
    if(result.passwd.compare("888928") == 0)
    {
        isSuperPasswd=true;
    }

    //判断是查询还是设置
    QString cmdType=content.mid(7,2);
    if(cmdType.compare("ST",Qt::CaseInsensitive) == 0)
    {
        result.type=SETUP_CMD_TYPE;
    }
    else if(cmdType.compare("RD",Qt::CaseInsensitive) == 0)
    {
        result.type=QUERY_CMD_TYPE;
    }
    else
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---not ST or RD! \n")
        return OTHER_ERROR_PARSE;
    }

    //解析查询指令
    if(result.type == QUERY_CMD_TYPE)
    {
        //初始为00000000000 时，任何手机只能设置 查询号码
        if(isInitNumber)
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---cuurent phone is 00000000000,you must set you phone!\n")
            return FORMAT_ERROR_PARSE;
        }

        if(isSuperPasswd) //用超级密码查询，返回密码错误
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---supper passwd can't query data!\n")
            return PASSWD_ERROR_PARSE;
        }

        if(isRightNumber == false)
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---1 phone number error!\n")
            return FORMAT_ERROR_PARSE;
        }

        QString cmd=content.mid(9,content.length()-9);
        int cmdIndex=cmdArrayList.indexOf(cmd);
        if(cmdIndex == -1)
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---cmd get query cmd error ! \n")
            return UNKNOW_CMD_PARSE;
        }

        result.cmd=(SMS_CMD_SELF)cmdIndex;

        //暂时不执行此命令 读取连接方式
        if(result.cmd == MDM_CMD)
        {
            return UNKNOW_CMD_PARSE;
        }

        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---success query cmd ! \n")
        return SUCCESS_PARSE;
    }

    //解析设置指令
    int index=content.indexOf(":",0,Qt::CaseInsensitive);
    if(index<0 || index<9)
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---set cmd not find : ! \n")
        return UNKNOW_CMD_PARSE;
    }

    QString cmd=content.mid(9,index-9);
    int cmdIndex=cmdArrayList.indexOf(cmd);
    if(cmdIndex == -1)
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---set cmd less than 9 ! \n")
        return FORMAT_ERROR_PARSE;
    }
    result.cmd=(SMS_CMD_SELF)cmdIndex;

    //获取指令设置参数
    QString args=content.right(content.length()-(index+1));
    if(args.length()<=0)
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---not find cmd self ! \n")
        return FORMAT_ERROR_PARSE;
    }
    result.args.clear();

    //初始为00000000000 时，任何手机只能设置查询号码
    if(isInitNumber)
    {
        if(result.cmd != PSP_CMD)
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---00000000000 not PSP set phone Cmd! \n")
            return FORMAT_ERROR_PARSE;
        }
    }
    if(isSuperPasswd) //用超级密码设置,无论是否为之前配置的号码，都只能执行PSP命令
    {
        if(result.cmd != PSP_CMD)
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---supper passwd can't set data!\n")
            return UNKNOW_CMD_PARSE;
        }
        if((!isInitNumber) && (!isRightNumber))
        {
            IBD_PRINTF(DEBUG_INFOMATION, "---SMS---not right phone number!\n")
            return FORMAT_ERROR_PARSE;
        }
    }
    else if(isRightNumber == false)
    {
        IBD_PRINTF(DEBUG_INFOMATION, "---SMS---2 phone number error!\n")
        return FORMAT_ERROR_PARSE;
    }
    switch(result.cmd)
    {
        case MDM_CMD: //设置连接方式 0:IP 1：域名
        {
            return UNKNOW_CMD_PARSE;
            //暂时不用
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MDM args connect type error1:%s! \n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            if((temp == "0") || (temp == "1"))
            {
                result.args<<temp;
            }
            else
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MDM args connect type error2:%s! \n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            break;
        }
        case MIP_CMD: //设置IP和端口 192.168.000.082,6000
        {
            int i=args.indexOf(",",Qt::CaseInsensitive);
            if(i<0)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MIP args ip and port error1:%s! \n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }

            QStringList list=args.split(",");
            if(list.count()!=2)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MIP args ip and port error2:%s! \n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }

            QStringList ip=list[0].split(".");
            if(ip.count()!=4)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MIP args ip error:%s! \n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<list[0]; //IP

            //假如端口为6000ad，后跟字母，只取前边的数字
            QString temp;
            if(getCmdIntArgs(list[1],temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MIP args port error:%s! \n",list[1].toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case DSN_CMD: //13800138000 终端SIM卡号
        {
            QString temp="";
            for(int i=0;i<args.length();i++)
            {
                if(args[i]<QChar('0') || args[i]>QChar('9'))
                {
                    break;
                }
                else
                {
                    temp.append(args[i]);
                }
            }
            if(temp.length()<11)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---DSN args sim number error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case APN_CMD: //设置APN CMNET
        {
            result.args<<args;
            break;
        }
        case LIC_CMD: //设置车牌号码 京A00000
        {
            qWarning("=====================SMS:%s",args.toUtf8().data());
            result.args<<args;
            break;
        }
        case LPC_CMD: //设置车牌颜色 1
        {
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---LPC args car plate color error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case DTM_CMD: //设置缺省回传间隔 30
        {
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---DTM args default report interval error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case ATM_CMD: //设置报警回传间隔
        {
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---ATM args warning report interval error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case STM_CMD: //设置休眠回传间隔
        {
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---STM args sleep report interval error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case MSP_CMD: //设置最大速度
        {
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---MSP args max speed error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case SPM_CMD: //设置速度模式（0:脉冲;1：卫星）
        {
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---SPM args speed model error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            /***********************
            0：vdr 速度；
            1：gps速度
            2：gps速度vdr速度自动切换
            ***********************/
            if((temp == "0") || (temp == "1") || (temp == "2"))
            {
                result.args<<temp;
            }
            else
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---SPM args speed model error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            break;
        }
        case FAC_CMD: //设置车辆特征系数 5000
        {
            return UNKNOW_CMD_PARSE;
            //暂时不用
            QString temp;
            if(getCmdIntArgs(args,temp) == false)
            {
                IBD_PRINTF(DEBUG_INFOMATION, "---SMS---FAC args car factor error:%s!\n",args.toLatin1().data())
                return FORMAT_ERROR_PARSE;
            }
            result.args<<temp;
            break;
        }
        case PSP_CMD: //使用超级密码修改预留号码
        {
            if(isSuperPasswd)
            {
                QString temp="";
                for(int i=0;i<args.length();i++)
                {
                    if(args[i]<QChar('0') || args[i]>QChar('9'))
                    {
                        break;
                    }
                    else
                    {
                        temp.append(args[i]);
                    }
                }
                if(temp.length()<11)
                {
                    IBD_PRINTF(DEBUG_INFOMATION, "---SMS---PSP args sim number error:%s!\n",args.toLatin1().data())
                    return FORMAT_ERROR_PARSE;
                }
                result.args<<temp;
                modifyAuthorizePhone(temp);
                return MODIFY_PHONE_PARSE;
            }
            break;
        }
    }

    return SUCCESS_PARSE;
}

QString SmsCmdProtocol::packageSmsCommand(SMS_CMD_STRUCT send, bool isSet)
{
    QString temp="";
    if(isSet)
    {
        temp="SET ";
    }
    else temp="";
    switch(send.cmd)
    {
        case MDM_CMD: //设置连接方式 0:IP 1：域名
        {
            temp+=cmdArrayList[MDM_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case MIP_CMD: //设置IP和端口 192.168.000.082,6000
        {
            temp+=cmdArrayList[MIP_CMD]+":"+send.args[0]+","+send.args[1];
            if(isSet) temp+=" OK";
            break;
        }
        case DSN_CMD: //13800138000 终端SIM卡号
        {
            temp+=cmdArrayList[DSN_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case APN_CMD: //设置APN CMNET
        {
            temp+=cmdArrayList[APN_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case LIC_CMD: //设置车牌号码 京A00000
        {
            temp+=cmdArrayList[LIC_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case LPC_CMD: //设置车牌颜色 1
        {
            temp+=cmdArrayList[LPC_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case DTM_CMD: //设置缺省回传间隔 30
        {
            temp+=cmdArrayList[DTM_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case ATM_CMD: //设置报警回传间隔
        {
            temp+=cmdArrayList[ATM_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case STM_CMD: //设置休眠回传间隔
        {
            temp+=cmdArrayList[STM_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case MSP_CMD: //设置最大速度
        {
            temp+=cmdArrayList[MSP_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case SPM_CMD: //设置速度模式（0:脉冲;1：卫星）
        {
            temp+=cmdArrayList[SPM_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case FAC_CMD: //设置车辆特征系数 5000
        {
            temp+=cmdArrayList[FAC_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
        case PSP_CMD: //使用超级密码修改预留号码
        {
            temp+=cmdArrayList[PSP_CMD]+":"+send.args[0];
            if(isSet) temp+=" OK";
            break;
        }
    }
    return temp;
}


