#include <boost/program_options.hpp>
#include <iostream>
#include "LogUtility.h"
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "SQLiteTable.h"
#include "VDRRunParaTable.h"
#include "VDRTrackInfoTable.h"
#include "MultiRowWithUniqueIndexTable.h"
#include "GeneralTimer.h"
#include "VDRMonitorTable.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "JT808MessageHandler.h"
#include "Utility.h"
#include "dbusClient.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

string g_testItem;
string g_tableName;
int g_count = 0;
GeneralTimerItem *timer;
GeneralTimerItem *timer2;
GeneralTimerItem *timer3;
GeneralTimerItem *timer4;
GeneralTimerItem *timer5;

static int time_count = (int)time(NULL);

VDRRunStatusTable g_runStatusTable;
VDRLocStatus1Table g_locStatus1table;

void AddColumn(SQLiteTable &table)
{
    SQLiteColumn col1("Title1", ENUM_SQLite_COLUMN_STRING);
    col1.SetUnique(true);
    col1.SetDefaultValue("DEFAULT");
    table.AddColumn(col1);
    SQLiteColumn col2("Title2", ENUM_SQLite_COLUMN_INTEGER);
    col2.SetDefaultValue(10);
    table.AddColumn(col2);
    SQLiteColumn col3("Title3", ENUM_SQLite_COLUMN_FLOAT);
    col3.SetDefaultValue(11.0f);
    table.AddColumn(col3);
    SQLiteColumn col4("Title4", ENUM_SQLite_COLUMN_SEQUENCE);
    SQLiteSequence seq;
    seq.push_back(0x01);
    seq.push_back(0x0A);
    seq.push_back(0x0B);
    seq.push_back(0x02);
    seq.push_back(0x5C);
    col4.SetDefaultValue(seq);
    table.AddColumn(col4);
}

void SetIndex(SQLiteTable &table)
{
    set<string> index1;
    index1.insert("Title1");
    table.AddIndex("Index1", index1, true);
}

void QueryAll(SQLiteTable &table)
{
    SQLiteValuePair constrain;
    set<string> results;
    results.insert("Title1");
    results.insert("Title2");
    results.insert("Title3");
    results.insert("Title4");
    set<SQLiteValuePair> returnResults;
    SQLiteValuePair returnResult;
    table.Query(constrain, results, returnResults);
    for(set<SQLiteValuePair>::iterator it = returnResults.begin(); it != returnResults.end(); it ++)
    {
        SQLiteValuePair pair = *it;
        LogUtility::Log(LOG_LEVEL_INFO, "Title1: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_STRING, pair["Title1"]).c_str());
        LogUtility::Log(LOG_LEVEL_INFO, "Title2: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, pair["Title2"]).c_str());
        LogUtility::Log(LOG_LEVEL_INFO, "Title3: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_FLOAT, pair["Title3"]).c_str());
        LogUtility::Log(LOG_LEVEL_INFO, "Title4: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_SEQUENCE, pair["Title4"]).c_str());
    }
}

int testDB()
{
    SQLiteValuePair constrain;
    set<string> results;
    set<SQLiteValuePair> returnResults;
    SQLiteSequence seq;
    SQLiteValuePair returnResult;
    
   
    SQLiteTable table("test.db", "test", 2);
    table.SetOrderTitle("Title2");

    AddColumn(table);
    
    SetIndex(table);

    table.Open();
    
    SQLiteValuePair pairs;
    pairs["Title1"] = SQLiteValue("First record");
    table.Add(pairs);
    
    pairs.clear();
    pairs["Title1"] = SQLiteValue("Second record");
    pairs["Title2"] = SQLiteValue(-15);
    pairs["Title3"] = SQLiteValue(-10.0f);
    seq.clear();
    seq.push_back(0xAA);
    seq.push_back(0xAB);
    seq.push_back(0xAC);
    seq.push_back(0xAD);
    pairs["Title4"] = SQLiteValue(seq);
    table.Add(pairs);

    results.clear();
    results.insert("Title1");
    results.insert("Title2");
    results.insert("Title3");
    results.insert("Title4");
    returnResult.clear();
    table.QueryTopBottom("Title2", results, returnResult, true);
    LogUtility::Log(LOG_LEVEL_INFO, "Title1: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_STRING, returnResult["Title1"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title2: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, returnResult["Title2"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title3: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_FLOAT, returnResult["Title3"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title4: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_SEQUENCE, returnResult["Title4"]).c_str());
    table.QueryTopBottom("Title2", results, returnResult, false);
    LogUtility::Log(LOG_LEVEL_INFO, "Title1: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_STRING, returnResult["Title1"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title2: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, returnResult["Title2"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title3: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_FLOAT, returnResult["Title3"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title4: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_SEQUENCE, returnResult["Title4"]).c_str());


    QueryAll(table);

    constrain.clear();
    constrain["Title1"] = SQLiteValue("First record");
    pairs.clear();
    pairs["Title2"] = SQLiteValue(-16);
    pairs["Title3"] = SQLiteValue(-11.0f);
    seq.clear();
    seq.push_back(0xBA);
    seq.push_back(0xBB);
    seq.push_back(0xBC);
    pairs["Title4"] = SQLiteValue(seq);
    table.Modify(constrain, pairs);

    QueryAll(table);

    constrain.clear();
    pairs.clear();
    pairs["Title2"] = SQLiteValue(-17);
    pairs["Title3"] = SQLiteValue(-12.0f);
    seq.clear();
    seq.push_back(0xCA);
    seq.push_back(0xCB);
    seq.push_back(0xCC);
    pairs["Title4"] = SQLiteValue(seq);
    table.Modify(constrain, pairs);

    QueryAll(table);
    
    constrain.clear();
    constrain["Title1"] = SQLiteValue("First record");
    table.Delete(constrain);
    
    QueryAll(table);
    
    constrain.clear();
    constrain["Title1"] = SQLiteValue("First record");
    results.clear();
    results.insert("Title1");
    results.insert("Title2");
    results.insert("Title3");
    results.insert("Title4");
    returnResults.clear();
    table.Query(constrain, results, returnResults);
    for(set<SQLiteValuePair>::iterator it = returnResults.begin(); it != returnResults.end(); it ++)
    {
        SQLiteValuePair pair = *it;
        LogUtility::Log(LOG_LEVEL_INFO, "Title1: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_STRING, pair["Title1"]).c_str());
        LogUtility::Log(LOG_LEVEL_INFO, "Title2: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, pair["Title2"]).c_str());
        LogUtility::Log(LOG_LEVEL_INFO, "Title3: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_FLOAT, pair["Title3"]).c_str());
        LogUtility::Log(LOG_LEVEL_INFO, "Title4: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_SEQUENCE, pair["Title4"]).c_str());
    }

    results.clear();
    results.insert("Title1");
    results.insert("Title2");
    results.insert("Title3");
    results.insert("Title4");
    returnResult.clear();
    table.QueryLatest(results, returnResult);
    LogUtility::Log(LOG_LEVEL_INFO, "Title1: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_STRING, returnResult["Title1"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title2: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, returnResult["Title2"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title3: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_FLOAT, returnResult["Title3"]).c_str());
    LogUtility::Log(LOG_LEVEL_INFO, "Title4: %s", SQLiteValueToString(ENUM_SQLite_COLUMN_SEQUENCE, returnResult["Title4"]).c_str());

    
    VDRRunParaTable settingTable;
    int heartbeat;
    string serveraddress;
    settingTable.GetIntValue(VDR_PARA_HEART_BEAT_INTERVAL, heartbeat);
    settingTable.GetStringValue(VDR_PARA_MAIN_SERVER_ADDRESS, serveraddress);
    cout << settingTable;
    cout << "Heart Beat: " << heartbeat;
    cout << "Main Server Address: " << serveraddress;
    settingTable.SetIntValue(VDR_PARA_HEART_BEAT_INTERVAL, 60);
    settingTable.SetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, "127.0.0.1");
    settingTable.Commit();
    settingTable.Refresh();
    cout << settingTable;
    
    //ConnectionManager::Instance()->Stop();
    
    //VDRRunParaTable paraTable;
    //ConcreteTableBase<1> ctable;
    
    getchar();
    return 0;
}

void TimerHandler(string &, void *data, void *)
{
    int value = (int)data;

    cout << "Timer Hanlder " << value << endl;
    
    if(value == 1)
    {
        StartTimer(timer2);
    }
    if(value == 2)
    {
        cout << "Time " << time(NULL);
        if(g_tableName == "runstatus")
        {
            vector<unsigned char> speed;
            vector<unsigned char> status;
            for(int i = 0; i < 16; i ++)
            {
                speed.push_back(0);
                status.push_back(0);
            }
            g_runStatusTable.SetIntValue(VDR_RUNS_OCCUR_TIME, time_count);
            g_runStatusTable.SetSequenceValue(VDR_RUNS_SPEED, speed);
            g_runStatusTable.SetSequenceValue(VDR_RUNS_STATUS, status);
            g_runStatusTable.Add();
        }
        else if(g_tableName == "locstatus1")
        {
            g_locStatus1table.SetIntValue(VDR_LOCS_OCCUR_TIME, time_count);
            g_locStatus1table.SetIntValue(VDR_LOCS_ALARM_SIGNAL, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_STATUS_FLAG, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_LATITUDE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_LONGITUDE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_ALTITUDE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_ORIENTATION, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_GPS_SPEED, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_TOTAL_MILEAGE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_OIL_MASS, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_VDR_SPEED, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_VDR_DISTANCE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_OVERSPEED_ALARM, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_OVERSPEED_REGIONID, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_INOUT_ALARM, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_INOUT_REGIONID, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_INOUT_DIRECTION, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_SHORT_LONG_LINEID, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_SHORT_LONG_TIME, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_SHORT_LONG_RESULT, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_EMERGENCY_ALARM_ID, 0);
            g_locStatus1table.Add();
        }

        time_count ++;
        //StartTimer(timer3);
    }
    if(value == 3)
    {
        StopTimer(timer);
        StartTimer(timer4);
    }
    if(value == 4)
    {
        StopTimer(timer2);
        StartTimer(timer5);
    }
    if(value == 5)
    {
        StopTimer(timer2);
    }
}

void testTimer()
{
    timer = CreateTimer("Timer 1", 5000, (void *)1, TimerHandler, true);
    timer2 = CreateTimer("Timer 2", 1000, (void *)2, TimerHandler, true);
    timer3 = CreateTimer("Timer 3", 10000, (void *)3, TimerHandler, true);
    timer4 = CreateTimer("Timer 4", 20000, (void *)4, TimerHandler, false);
    timer5 = CreateTimer("Timer 5", 60000, (void *)5, TimerHandler, false);
    StartTimer(timer2);
    while(1)
    {
        sleep(1);
    }
}

void PrintTable(MultiRowWithUniqueIndexTable &table)
{
    set<SQLiteValuePair> results;
    results.clear();
    table.QueryAll(results);
    
    for(set<SQLiteValuePair>::iterator it = results.begin(); it != results.end(); it ++)
    {
        SQLiteValuePair pair = *it;
        for(SQLiteValuePair::iterator it2 = pair.begin(); it2 != pair.end(); it2 ++)
        {
            string name = it2->first;
            SQLiteValue value = it2->second;
            cout << name;
            cout << " = ";
            cout << SQLiteValueToString(value);
            cout << endl;
        }
    }
}

void testMultiRowTable()
{
    VDRTiredTable tiredTable;
    tiredTable.RefreshBottom();
    
    VDRRecordTable table;
    table.SetIntValue(VDR_REC_FILE_TIME, (int)time(NULL));
    table.SetStringValue(VDR_REC_FILE_PATH, "/tmp/file1.wav");
    table.SetIntValue(VDR_REC_RECORD_DURA, 120);
    table.Commit();
    sleep(1);
    int timenow = (int)time(NULL);
    table.SetIntValue(VDR_REC_FILE_TIME, timenow);
    table.SetStringValue(VDR_REC_FILE_PATH, "/tmp/file2.wav");
    table.SetIntValue(VDR_REC_FILE_TYPE, 1);
    table.SetIntValue(VDR_REC_RECORD_DURA, 130);
    table.Commit();
    sleep(1);
    table.SetIntValue(VDR_REC_FILE_TIME, (int)time(NULL));
    table.SetStringValue(VDR_REC_FILE_PATH, "/tmp/file3.wav");
    table.SetIntValue(VDR_REC_RECORD_DURA, 140);
    table.Commit();
    sleep(1);
    table.SetIntValue(VDR_REC_FILE_TIME, (int)time(NULL));
    table.SetStringValue(VDR_REC_FILE_PATH, "/tmp/file4.wav");
    table.SetIntValue(VDR_REC_RECORD_DURA, 150);
    table.Commit();
    PrintTable(table);
    table.RefreshBottom();
    cout << table;
    table.RefreshTop();
    cout << table;
    table.RefreshNext();
    cout << table;
    table.Delete(timenow);
    PrintTable(table);
}

void testLargeTable()
{
    int intvalue;
    vector<unsigned char> seqvalue;

    if(g_tableName == "runstatus")
    {
    /*
        for(int i = 0; i < 10; i ++)
        {
            vector<unsigned char> speed;
            vector<unsigned char> status;
            for(int i = 0; i < 16; i ++)
            {
                speed.push_back(0x30 + i);
                status.push_back(0x31 + i);
            }
            g_runStatusTable.SetIntValue(VDR_RUNS_OCCUR_TIME, time_count);
            g_runStatusTable.SetSequenceValue(VDR_RUNS_SPEED, speed);
            g_runStatusTable.SetSequenceValue(VDR_RUNS_STATUS, status);
            g_runStatusTable.Add();
            time_count ++;  
        }
    */
        int ret = g_runStatusTable.RefreshNearest(VDR_RUNS_OCCUR_TIME, g_count, true);
        if(ret < 0)
        {
            cerr << "Failed to refresh nearest" << endl;
            return;
        }
        cout << "Query result " << numToString<int>(ret) << endl;
        g_runStatusTable.GetIntValue(VDR_RUNS_OCCUR_TIME, intvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
        g_runStatusTable.GetSequenceValue(VDR_RUNS_SPEED, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
        g_runStatusTable.GetSequenceValue(VDR_RUNS_STATUS, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());
        ret = g_runStatusTable.RefreshPrevious(ret);
        cout << "Query result " << numToString<int>(ret) << endl;
        g_runStatusTable.GetIntValue(VDR_RUNS_OCCUR_TIME, intvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
        g_runStatusTable.GetSequenceValue(VDR_RUNS_SPEED, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
        g_runStatusTable.GetSequenceValue(VDR_RUNS_STATUS, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());

        ret = g_runStatusTable.RefreshNearest(VDR_RUNS_OCCUR_TIME, g_count, false);
        if(ret < 0)
        {
            cerr << "Failed to refresh nearest" << endl;
            return;
        }
        cout << "Query result " << numToString<int>(ret) << endl;
        g_runStatusTable.GetIntValue(VDR_RUNS_OCCUR_TIME, intvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
        g_runStatusTable.GetSequenceValue(VDR_RUNS_SPEED, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
        g_runStatusTable.GetSequenceValue(VDR_RUNS_STATUS, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());
        ret = g_runStatusTable.RefreshNext(ret);
        cout << "Query result " << numToString<int>(ret) << endl;
        g_runStatusTable.GetIntValue(VDR_RUNS_OCCUR_TIME, intvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
        g_runStatusTable.GetSequenceValue(VDR_RUNS_SPEED, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
        g_runStatusTable.GetSequenceValue(VDR_RUNS_STATUS, seqvalue);
        LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());
        
    /*
        int ret = g_runStatusTable.RefreshBottom(30);
        if(ret < 0)
        {
            cerr << "Failed to refresh bottom" << endl;
            return;
        }
        cout << "We get " << g_runStatusTable.GetResultsCount() << endl;
        for(int i = 0; i < g_runStatusTable.GetResultsCount() ; i ++)
        {
            g_runStatusTable.GetIntValue(i, VDR_RUNS_OCCUR_TIME, intvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
            g_runStatusTable.GetSequenceValue(i, VDR_RUNS_SPEED, seqvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
            g_runStatusTable.GetSequenceValue(i, VDR_RUNS_STATUS, seqvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());
        }
    */
    /*
        index = g_runStatusTable.RefreshBottom();
       
        while(index >= 0)
        {
            g_runStatusTable.GetIntValue(VDR_RUNS_OCCUR_TIME, intvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
            g_runStatusTable.GetSequenceValue(VDR_RUNS_SPEED, seqvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
            g_runStatusTable.GetSequenceValue(VDR_RUNS_STATUS, seqvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());
            index = g_runStatusTable.RefreshPrevious(index);
        }

        index = g_runStatusTable.RefreshTop();
        
        while(index >= 0)
        {
            g_runStatusTable.GetIntValue(VDR_RUNS_OCCUR_TIME, intvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Time: %d.", intvalue);
            g_runStatusTable.GetSequenceValue(VDR_RUNS_SPEED, seqvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Speed: %s.", seqToString(seqvalue).c_str());
            g_runStatusTable.GetSequenceValue(VDR_RUNS_STATUS, seqvalue);
            LogUtility::Log(LOG_LEVEL_INFO, "Run Status Table Status: %s.", seqToString(seqvalue).c_str());
            index = g_runStatusTable.RefreshNext(index);
        }
    */
    }
}

void testConnectionManager()
{
    JT808MessageHandler jt808;
    ConnectionManager::Instance()->Start();
    getchar();
    ConnectionManager::Instance()->Stop();
}

DBusServerC    *g_pDbusServer;

void *SendDbusThread(void *arg)
{
    char command[256];
    bool flag = false;
    
    DBusClientC *client = (DBusClientC *)arg;
    
    while(!flag)
    {
        cin.getline(command, 256);
        
        char *ptr = command + strlen(command);
        
        while((ptr > command) && ((*ptr == '\n') || (*ptr == '\r') || (*ptr == '\0') || (*ptr == ' ')))
        {
            *ptr = '\0';
            ptr --;
        }
        
        if(ptr == command)
        {
            continue;
        }
        
        string dbusCommand = command;

        if(dbusCommand == "exit")
        {
            flag = true;
        }
        else if(dbusCommand == "register")
        {
			JT808RegisterDbusReqC request;
			DBusReplyC *pReply = NULL;
			
			client->sendService(&request, pReply, 5000);
			if(pReply)
			{
				cout << "Get register reply." << endl;
				delete pReply;
			}
			else
			{
				cerr << "Failed to get register reply." << endl;
			}
        }
        else if(dbusCommand == "locstatus")
        {
			JT808LocReportDbusReqC request;
			DBusReplyC *pReply = NULL;
			
			client->sendService(&request, pReply, 5000);
			if(pReply)
			{
				cout << "Get locstatus reply." << endl;
				delete pReply;
			}
			else
			{
				cerr << "Failed to get locstatus reply." << endl;
			}
        }
        else if(dbusCommand == "modify")
        {
			JT808ModifySettingReqC request;
			DBusReplyC *pReply = NULL;
			
            request.m_main_server = (char *)malloc(100);
            request.m_second_server = (char *)malloc(100);
            request.m_auth = (char *)malloc(100);
            strcpy(request.m_main_server, "1.1.1.1");
            strcpy(request.m_second_server, "1.1.1.1");
            strcpy(request.m_auth, "1234");
            request.m_port = 1020;
            
			client->sendService(&request, pReply, 5000);
			if(pReply)
			{
				cout << "Get modify reply." << endl;
				delete pReply;
			}
			else
			{
				cerr << "Failed to get modify reply." << endl;
			}
        }    }
    
    return NULL;
}

void testDbus(int argc, char **argv)
{

    DBusApplicationC app(argc, argv);

    DBusClientC *client  =new DBusClientC((char*)"com.innov.jt808", (char*)"/jt808");
    client->registerReply(JT808_DBUS_REP_ID_REGISTER,JT808RegisterDbusRepC::factory);
    client->registerReply(JT808_DBUS_REP_ID_LOC_REPORT,JT808LocReportDbusRepC::factory);
    client->registerReply(JT808_DBUS_REP_ID_SETTING_MODIFY,JT808ModifySettingDbusRepC::factory);
    client->connect();

    pthread_t threadId;
    pthread_create(&threadId, NULL, SendDbusThread, client);

    app.loop();
}

void testGenerateDB()
{
    if(g_tableName == "runstatus")
    {
        LogUtility::Log(LOG_LEVEL_INFO, "Begin to generate run status table.");
        vector<unsigned char> speed;
        vector<unsigned char> status;
        for(int i = 0; i < g_count; i ++)
        {
            g_runStatusTable.SetIntValue(VDR_RUNS_OCCUR_TIME, time_count);
            speed.clear();
            status.clear();
            for(int i = 0; i < 16; i ++)
            {
                speed.push_back(0x30 + i % 10);
                status.push_back(0x30 + i % 10);
            }
            g_runStatusTable.SetSequenceValue(VDR_RUNS_SPEED, speed);
            g_runStatusTable.SetSequenceValue(VDR_RUNS_STATUS, status);
            g_runStatusTable.Add();
            time_count ++;  
        }
        LogUtility::Log(LOG_LEVEL_INFO, "Over.");
    }
    else if(g_tableName == "locstatus1")
    {
        LogUtility::Log(LOG_LEVEL_INFO, "Begin to generate loc status 1 table.");
        for(int i = 0; i < g_count; i ++)
        {
            g_locStatus1table.SetIntValue(VDR_LOCS_OCCUR_TIME, time_count);
            g_locStatus1table.SetIntValue(VDR_LOCS_ALARM_SIGNAL, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_STATUS_FLAG, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_LATITUDE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_LONGITUDE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_ALTITUDE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_ORIENTATION, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_GPS_SPEED, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_TOTAL_MILEAGE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_OIL_MASS, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_VDR_SPEED, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_VDR_DISTANCE, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_OVERSPEED_ALARM, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_OVERSPEED_REGIONID, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_INOUT_ALARM, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_INOUT_REGIONID, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_INOUT_DIRECTION, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_SHORT_LONG_LINEID, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_SHORT_LONG_TIME, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_SHORT_LONG_RESULT, 0);
            g_locStatus1table.SetIntValue(VDR_LOCS_EMERGENCY_ALARM_ID, 0);
            g_locStatus1table.Add();
            time_count ++;
        }
        LogUtility::Log(LOG_LEVEL_INFO, "Over.");
    }
}

namespace po = boost::program_options;

static int ProcessOptions(int argc, char *argv[])
{
    try {

        po::options_description options("Allowed options");
        options.add_options()
            ("help", "produce help message")
            ("debug", po::value<int>(), "set debug level")
            ("log", po::value<int>(), "set log level")
            ("item", po::value<string>(), "set test item")
            ("table", po::value<string>(), "set table name")
            ("count", po::value<int>(), "set count")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << options << "\r\n";
            return 1;
        }

        if (vm.count("debug")) {
            cout << "debug level was set to " 
                 << vm["debug"].as<int>() << ".\r\n";
            LogUtility::SetStdoutLevel(vm["debug"].as<int>());
        }

        if (vm.count("log")) {
            cout << "log level was set to " 
                 << vm["log"].as<int>() << ".\r\n";
            LogUtility::SetLogLevel(vm["log"].as<int>());
        }

        if (vm.count("item")) {
            cout << "test item was set to " 
                 << vm["item"].as<string>() << ".\r\n";
            g_testItem = vm["item"].as<string>();
        }

        if (vm.count("table")) {
            cout << "table item was set to " 
                 << vm["table"].as<string>() << ".\r\n";
            g_tableName = vm["table"].as<string>();
        } else {
            cout << "table item was not set.\r\n";
        }

        if (vm.count("count")) {
            cout << "count was set to " 
                 << vm["count"].as<int>() << ".\r\n";
            g_count = vm["count"].as<int>();
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\r\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\r\n";
    }

    return 0;
}

int main(int argc, char **argv)
{
    if(ProcessOptions(argc, argv) != 0)
    {
        return 1;
    }
    
    if(g_testItem == "database")
    {
        testDB();
    }
    else if(g_testItem == "multirow")
    {
        testMultiRowTable();
    }
    else if(g_testItem == "largetable")
    {
        testLargeTable();
    }
    else if(g_testItem == "generatedb")
    {
        testGenerateDB();
    }
    else if(g_testItem == "timer")
    {
        testTimer();
    }
    else if(g_testItem == "config")
    {
        ConfigManager::Refresh();
        cout << ConfigManager();
    }
    else if(g_testItem == "connection")
    {
        testConnectionManager();
    }
    else if(g_testItem == "dbus")
    {
        testDbus(argc, argv);
    }
    else
    {
        cerr << "Unknown item.\r\n";
        return -1;
    }
}
