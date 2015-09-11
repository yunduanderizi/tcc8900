#include <expat.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <map>
#include "SQLiteTable.h"
#include "VDRRunParaTable.h"
#include "VDRDeployParaTable.h"
#include "Utility.h"

using namespace std;

#define SETTING_FILE_BUF_LEN        256

#define XML_TAG_PROVINCE                "province"
#define XML_TAG_CITY                    "city"
#define XML_TAG_MAKER                   "maker"
#define XML_TAG_MODEL                   "model"
#define XML_TAG_TERMID                  "termid" 
#define XML_TAG_VIN                     "vin"    
#define XML_TAG_COLOR                   "platecolor"
#define XML_TAG_PLATE                   "plate"
#define XML_TAG_PLATE_TYPE              "platetype"
#define XML_TAG_TIME                    "time"
#define XML_TAG_MILEAGE                 "mileage"
#define XML_TAG_RATE                    "rate"
#define XML_TAG_LAP						"sensingelement"
#define XML_TAG_PHONE                   "phone"
#define XML_TAG_AUTH                    "auth"
#define XML_TAG_MAIN_SERVER             "main"
#define XML_TAG_SECOND_SERVER           "second"
#define XML_TAG_PORT                    "port"
#define XML_TAG_MAXSPEED				"speed"
#define XML_TAG_DRIVE_THRES				"drivedura"
#define XML_TAG_TERM_TYPE				"termtype"
#define XML_TAG_HAD_VERSION				"hadversion"

typedef struct JT808Para
{
    JT808Para()
    {
        colid = -1;
        len = 0;
        type = ENUM_SQLite_COLUMN_INTEGER;
    }
    JT808Para(int col, EnumSQLiteColumnType t, int l)
    {
        colid = col;
        type = t;
        len = l;
    }
    int colid;
    EnumSQLiteColumnType type;
    int len;
} StruJT808Para;

static map<string, StruJT808Para> JT808RunParas;
static map<string, StruJT808Para> JT808DeployParas;
static string currentTag = "";
static VDRDeployParaTable deployTable;
static VDRRunParaTable runTable;
static bool deployTableFlag = false;
static bool runTableFlag = false;
static bool errorFlag = false;
static string storevalue = "";

void ElementStart(void *, const char *el, const char **)
{
    currentTag = el;
    storevalue = "";
}

void ElementEnd(void *, const char *)
{
    string value = storevalue;
    StruJT808Para para;
    SingleRowTable *table;
    bool *table_flag;
    if(JT808DeployParas.count(currentTag))
    {
        cout << "Deploy Tag " << currentTag << " : " << value << endl;
        para = JT808DeployParas[currentTag];
        table = &deployTable;
        table_flag = &deployTableFlag;
     }
     else if(JT808RunParas.count(currentTag))
     {
        cout << "Run Tag " << currentTag << " : " << value << endl;
        para = JT808RunParas[currentTag];
        table = &runTable;
        table_flag = &runTableFlag;
     }
     else
     {
        cerr << "Unknown para " << currentTag << " : " << value << endl;
        return;
     }
     
    if(para.type == ENUM_SQLite_COLUMN_INTEGER)
    {
        int intvalue = 0;
        if(currentTag == XML_TAG_TIME)
        {
            intvalue = GetTime(value);
            if(intvalue < 0)
            {
                cerr << "Invalid time value" << endl;
                errorFlag = true;
                return;
            }
        }
        else
        {
            intvalue = stringToNum<int>(value);
        }
        int ret = table->SetIntValue(para.colid, intvalue);
        if(ret < 0)
        {
            cerr << "Failed to set value " << value << "  for " << currentTag << endl;
            errorFlag = true;
        }
    }
    else if(para.type == ENUM_SQLite_COLUMN_STRING)
    {
        int ret = table->SetStringValue(para.colid, value);
        if(ret < 0)
        {
            cerr << "Failed to set value " << value << "  for " << currentTag << endl;
            errorFlag = true;
        }
    }
    else if(para.type == ENUM_SQLite_COLUMN_SEQUENCE)
    {
        vector<unsigned char> seqValue;
        int seqlen = para.len;
        int stringlen = value.length();
        if(seqlen < value.length())
        {
            stringlen = seqlen;
        }
        for(int i = 0; i < stringlen; i ++)
        {
            seqValue.push_back(value[i]);
        }
        for(int i = stringlen; i < seqlen; i ++)
        {
            seqValue.push_back(0x0);
        }
        int ret = table->SetSequenceValue(para.colid, seqValue);
        if(ret < 0)
        {
            cerr << "Failed to set value " << value << "  for " << currentTag << endl;
            errorFlag = true;
        }
    }
    *table_flag = true;
    currentTag = "";
    storevalue = "";
    return;
}

void ElementData(void *, const char*el, int len)
{
    const char *ptr = el;
    const char *ptr_end = el + len - 1;

    if(currentTag.length() <= 0)
    {
        return;
    }

    while(ptr <= ptr_end)
    {
        if((*ptr != ' ') && (*ptr != '\t') && (*ptr != '\r') && (*ptr != '\n'))
        {
            break;
        }
        ptr ++;
    }
    while (ptr <= ptr_end)
    {
        if((*ptr_end != ' ') && (*ptr_end != '\t') && (*ptr_end != '\r') && (*ptr_end != '\n'))
        {
            break;
        }
        ptr_end --;
    }
    if(ptr_end < ptr)
    {
        return;
    }

    string value(ptr, ptr_end-ptr+1);
    storevalue += value;
}

int main(int argc, char **argv)
{
    int len;
    char buf[SETTING_FILE_BUF_LEN];
    int ret;

    cout << "Enter importsettings." << endl;
    
    if(argc != 2)
    {
        cerr << "Usage: importsetting settingfile" << endl;
        return 1;
    }

    JT808DeployParas.clear();
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_PROVINCE,    StruJT808Para((int)VDR_DEPLOY_PARA_PROVINCE_ID,        ENUM_SQLite_COLUMN_INTEGER,     0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_CITY,        StruJT808Para((int)VDR_DEPLOY_PARA_CITY_ID,            ENUM_SQLite_COLUMN_INTEGER,     0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_MAKER,       StruJT808Para((int)VDR_DEPLOY_PARA_MANUFACTORY_ID,     ENUM_SQLite_COLUMN_SEQUENCE,    5)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_MODEL,       StruJT808Para((int)VDR_DEPLOY_PARA_MODEL_NO,           ENUM_SQLite_COLUMN_SEQUENCE,    20)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_TERMID,      StruJT808Para((int)VDR_DEPLOY_PARA_ID,                 ENUM_SQLite_COLUMN_SEQUENCE,    7)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_VIN,         StruJT808Para((int)VDR_DEPLOY_PARA_CAR_VIN,            ENUM_SQLite_COLUMN_STRING,      0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_COLOR,       StruJT808Para((int)VDR_DEPLOY_PARA_PLATE_COLOR,        ENUM_SQLite_COLUMN_INTEGER,     0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_PLATE,       StruJT808Para((int)VDR_DEPLOY_PARA_PLATE_ID,           ENUM_SQLite_COLUMN_STRING,      0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_PLATE_TYPE,  StruJT808Para((int)VDR_DEPLOY_PARA_PLATE_SORT,         ENUM_SQLite_COLUMN_STRING,      0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_TIME,        StruJT808Para((int)VDR_DEPLOY_PARA_DEPLOY_TIME,        ENUM_SQLite_COLUMN_INTEGER,     0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_MILEAGE,     StruJT808Para((int)VDR_DEPLOY_PARA_DEPLOY_MILEAGE,     ENUM_SQLite_COLUMN_INTEGER,     0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_RATE,        StruJT808Para((int)VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, ENUM_SQLite_COLUMN_INTEGER,     0)));
	JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_LAP,         StruJT808Para((int)VDR_DEPLOY_PARA_LAP_PULSE_FACTOR, 	ENUM_SQLite_COLUMN_INTEGER,     0)));
	JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_PHONE,       StruJT808Para((int)VDR_DEPLOY_PARA_PHONE_NUMBER,       ENUM_SQLite_COLUMN_STRING,      0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_AUTH,        StruJT808Para((int)VDR_DEPLOY_PARA_AUTH_TOKEN,         ENUM_SQLite_COLUMN_STRING,      0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_TERM_TYPE,   StruJT808Para((int)VDR_DEPLOY_PARA_VDR_TYPE,         	ENUM_SQLite_COLUMN_INTEGER,      0)));
    JT808DeployParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_HAD_VERSION, StruJT808Para((int)VDR_DEPLOY_PARA_HARDWARE_VERSION,        ENUM_SQLite_COLUMN_STRING,      0)));


    JT808RunParas.clear();
    JT808RunParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_MAIN_SERVER,    StruJT808Para((int)VDR_PARA_MAIN_SERVER_ADDRESS,       ENUM_SQLite_COLUMN_STRING,     0)));
    JT808RunParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_SECOND_SERVER,  StruJT808Para((int)VDR_PARA_SECOND_SERVER_ADDRESS,     ENUM_SQLite_COLUMN_STRING,     0)));
    JT808RunParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_PORT,           StruJT808Para((int)VDR_PARA_TCP_PORT,                  ENUM_SQLite_COLUMN_INTEGER,    5)));	
    JT808RunParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_MAXSPEED,       StruJT808Para((int)VDR_PARA_MAX_SPEED,                 ENUM_SQLite_COLUMN_INTEGER,    0)));
    JT808RunParas.insert(map<string, StruJT808Para>::value_type(XML_TAG_DRIVE_THRES,    StruJT808Para((int)VDR_PARA_CONT_DRIVE_THRESHOLD,      ENUM_SQLite_COLUMN_INTEGER,    0)));
    
    string settingfile = argv[1];
    XML_Parser parser = XML_ParserCreate("UTF-8");
    int fd = open(settingfile.c_str(), O_RDONLY);
    if(fd < 0)
    {
        cerr << "Failed to open file " << settingfile.c_str() << endl;
        return -1;
    }

    XML_SetUserData(parser, NULL);
    XML_SetElementHandler(parser, ElementStart, ElementEnd);
    XML_SetCharacterDataHandler(parser, ElementData);
    
    while((len = read(fd, buf, SETTING_FILE_BUF_LEN)) > 0)
    {
        ret = XML_Parse(parser, (char *)buf, len, 0);
        if(!ret)
        {
            cerr << "ParseCfg XML_Parse failed." << endl;
            break;
        }
    }

    if(ret)
    {
        XML_Parse(parser, (char *)buf, 0, 1);
    }

    XML_ParserFree(parser);
    
    if(deployTableFlag)
    {
        ret = deployTable.Commit();
        if(ret < 0)
        {
            errorFlag = true;
        }
    }

    if(runTableFlag)
    {
        ret = runTable.Commit();
        if(ret < 0)
        {
            errorFlag = true;
        }
    }

    if(errorFlag)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
