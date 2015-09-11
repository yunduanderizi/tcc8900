#ifndef _PROCSTATUSMONITORINTERFACE_H_
#define _PROCSTATUSMONITORINTERFACE_H_


#include <map>
#include <string>
#include <vector>

#include "dbusMessage.h"
#include "dbusServer.h"
#include "dbusClient.h"
#include "dbusApplication.h"
#include "dbusClientProxy.h"


using namespace std;
class ProcStatusMonitorTimerC;

//@{ DEBUG Macro define for debug
#define DEBUG
#ifdef DEBUG
#define appSMDbg( fmt, arg... )  do{                        \
        fprintf(stderr, "[AppStatusMonitor]: [ %s ] %4d:"fmt"\n", __FILE__, __LINE__, ##arg ); \
    }while( 0 )
#else
#define appSMDbg( fmt, arg... )
#endif    //@}

/// Dbus Message ID define
#define REQ_ID_PROC_STATUS_MONITOR_MSG              61
#define REP_ID_PROC_STATUS_MONITOR_MSG              62
#define NOTF_ID_PROC_STATUS_MONITOR_MSG             63


//@{ Process Name and child function number define
#define APP_STATUS_MONITOR_NAME_LEN                 64

#define APP_STATUS_MONITOR_PROC_PHONE               "phone"
#define APP_STATUS_MONITOR_PROC_NAVI                "Navi"
#define APP_STATUS_MONITOR_PROC_MULTIMEDIA          "Multimedia"
#define APP_STATUS_MONITOR_PROC_INFOMATION          "information"
//#define APP_STATUS_MONITOR_PROC_LIVESERVICE         "liveService"
#define APP_STATUS_MONITOR_PROC_SECURITY            "security"
#define APP_STATUS_MONITOR_PROC_ANNOUNCE            "announce"
#define APP_STATUS_MONITOR_PROC_FILEBROWSER         "filebrowser"
#define APP_STATUS_MONITOR_PROC_VOICE               "voice"


///phone
#define APP_STATUS_MONITOR_FUNC_TELEPHONE           11
#define APP_STATUS_MONITOR_FUNC_PHONEHISTORY        12
#define APP_STATUS_MONITOR_FUNC_ADDRESSBOOK         13
#define APP_STATUS_MONITOR_FUNC_SMS                 14
//#define APP_STATUS_MONITOR_FUNC_FACETIME            15
#define APP_STATUS_MONITOR_FUNC_DIAL_SOS            15
#define APP_STATUS_MONITOR_FUNC_WIFI                16
#define APP_STATUS_MONITOR_FUNC_DIAL_SHUTDOWN		(17)
#define APP_STATUS_MONITOR_FUNC_DIAL_SERVICE		(18)
#define APP_STATUS_MONITOR_FUNC_PHONE_NEW_CALL          (19)

///navi
#define APP_STATUS_MONITOR_FUNC_NAVI                20
#define APP_STATUS_MONITOR_FUNC_WHEREYOU            21
#define APP_STATUS_MONITOR_FUNC_WHEREME             22
#define APP_STATUS_MONITOR_FUNC_DYNAMICNAVI         23
#define APP_STATUS_MONITOR_FUNC_MORNINGROADPAPER    24
#define APP_STATUS_MONITOR_FUNC_ROADBOOK            25
#define APP_STATUS_MONITOR_FUNC_TRAFFICNOTE         26

///multimedia
#define APP_STATUS_MONITOR_FUNC_MUSIC               31
#define APP_STATUS_MONITOR_FUNC_MOVIE               32
#define APP_STATUS_MONITOR_FUNC_RADIO               33
#define APP_STATUS_MONITOR_FUNC_PICTURE             34
//#define APP_STATUS_MONITOR_FUNC_MUSICDOWNLOAD       35
//#define APP_STATUS_MONITOR_FUNC_NETRADIO            36

///infomation
#define APP_STATUS_MONITOR_FUNC_NEWS                41
#define APP_STATUS_MONITOR_FUNC_STOCK               42
#define APP_STATUS_MONITOR_FUNC_EBOOK               43

///liveserice
#define APP_STATUS_MONITOR_FUNC_LIVEADVISE          51
#define APP_STATUS_MONITOR_FUNC_BOOKRESTAURANT      52
#define APP_STATUS_MONITOR_FUNC_BOOKHOTEL           53
#define APP_STATUS_MONITOR_FUNC_BOOKTICKETS         54

///security
#define APP_STATUS_MONITOR_FUNC_VEHICLETIPS         61
#define APP_STATUS_MONITOR_FUNC_ROADASSISTANCE      62
#define APP_STATUS_MONITOR_FUNC_TAFFICREPORT        63
#define APP_STATUS_MONITOR_FUNC_REMOTEDIAGNOSIS     64
#define APP_STATUS_MONITOR_FUNC_PECCANCYTIPS        65
#define APP_STATUS_MONITOR_FUNC_MAINTENANCE         66
#define APP_STATUS_MONITOR_FUNC_DCUDISPLAY          67


///filebrowser
#define APP_STATUS_MONITOR_FUNC_SYSTEMSETING        70

///voice
#define APP_STATUS_MONITOR_FUNC_VOICE               80

// ANNOUNCE
#define APP_STATUS_MONITOR_FUNC_ANNOUNCE            90    



//@}

///Macro Define for Error
#define APP_STATUS_MONITOR_E_SUCCESS                0
#define APP_STATUS_MONITOR_E_FAIL                   -1
#define APP_STATUS_MONITOR_E_WRONG_STATUS           -2
#define APP_STATUS_MONITOR_E_INVALID_PRI            -3
#define APP_STATUS_MONITOR_E_INVALID_APPNAME        -4
#define APP_STATUS_MONITOR_E_INVALID_FUNCNAME       -5
#define APP_STATUS_MONITOR_E_INVALID_ARGUMENT       -6


//@{ Data structure and type define
typedef enum _AppStatusMonitorRunStaus    
{
    APP_STATUS_MONITOR_STATUS_EXIT ,
    APP_STATUS_MONITOR_STATUS_IDLE ,
    APP_STATUS_MONITOR_STATUS_USING
} AppStatusMonitorRunStaus;

//功能单元的状态
typedef struct _StruAppStatusMonitorStatus    
{
    AppStatusMonitorRunStaus      runStatus;
} StruAppStatusMonitorStatus;

/*
typedef struct _StruAppStatusMonitorDbusClient    
{
    string appName;
    DBusClientC* pdbusClient;
} StruAppStatusMonitorDbusClient;
*/
class ProcStatusMonitorDbusClientC
{
public:
    ProcStatusMonitorDbusClientC(string procName, DBusClientProxyC*pDbusClient);
    string appName;
    DBusClientProxyC* pdbusClient;
};



//管理整个系统进程运行状态的数据结构


typedef map<unsigned long,StruAppStatusMonitorStatus> CHILDFUNCSTATUSMAP;
typedef map<string,CHILDFUNCSTATUSMAP>     PROCSTATUSMONITORMAP;
typedef map<string,DBusClientProxyC*>     PROCSTATUSMONITORDBUSMAP;
typedef vector<ProcStatusMonitorDbusClientC>     PROCSTATUSMONITORDBUSVEC;


class ProcStatusMonitorOperC
{
public:
    virtual void reclaimer () = 0;
};



class ProcStatusMonitorServerRepC
{
public:
    virtual void handleChildFuncStatusChange (CHILDFUNCSTATUSMAP::const_iterator it_childFuncMap) = 0;
    
    virtual void handleShutDownRate ( int rate ) = 0;
};

class ProcStatusMonitorServerC
{
public:
    ProcStatusMonitorServerC( ProcStatusMonitorServerRepC *  pCallbackC, DBusServerC *pdbusServer, const PROCSTATUSMONITORDBUSVEC dbusClientVec);
    ~ProcStatusMonitorServerC();

    const PROCSTATUSMONITORMAP * getAppStatusList( );

    int    gotoAppFunc( string  sAppName, unsigned long funcName);
    
    int stopProcess( string  sAppName );
    
    int startProcess( string  sAppPathName, const char * sargv[] );
    
    void shutDown();

    int registerOperation( ProcStatusMonitorOperC * pOperC, unsigned long pri );

    int    realseOperation( ProcStatusMonitorOperC * pOperC );

private:

    int initDbus();
    static void dbusServerHandle(DBusRequestC *request, DBusReplyC *&reply);
    static void dbusNotifyHandle(DBusNotifyC *notify);
    
    static PROCSTATUSMONITORMAP appStatusList;
    static ProcStatusMonitorServerRepC *  pprocCallbackC;
    static ProcStatusMonitorServerC * thisObject;
    
    DBusServerC *pprocDbusServer;
    PROCSTATUSMONITORDBUSMAP procDbusClientMap;
    map<int, DBusClientProxyC*> procDbusClientSeqMap;
    map<ProcStatusMonitorOperC*, unsigned long> userOperationMap;
    pthread_t threadID;
    pthread_rwlock_t rwlock;
    int procStatusMonitor_shutRate;
    int procStatusMonitor_timer_rate;
    ProcStatusMonitorTimerC * ptimer;
    pthread_mutex_t userOperMapLock;
    
friend class ProcStatusMonitorTimerC;
friend class ProcStatusMonitorServerLockC;

};

class ProcStatusMonitorCallbackC
{
public:
    virtual void handleGotoAppStatus(unsigned long funcName) = 0;
};

class ProcStatusMonitorClientC
{
public:

    ProcStatusMonitorClientC(string appName, DBusServerC *pdbusServer, DBusClientProxyC * pdbusClient, \
                    ProcStatusMonitorCallbackC *pcallbackC);

    ~ProcStatusMonitorClientC();

    int    changeAppFuncStatus( unsigned long funcName, StruAppStatusMonitorStatus status);

    int registerOperation( ProcStatusMonitorOperC * pOperC, unsigned long pri );

    int    realseOperation( ProcStatusMonitorOperC * pOperC );
    
private:
    int initDbus();
    static void dbusServerHandle(DBusRequestC *request, DBusReplyC *&reply);
    static void dbusNotifyHandle(DBusNotifyC *notify);

    static map<ProcStatusMonitorOperC*, unsigned long> userOperationMap;
    static ProcStatusMonitorCallbackC  *pprocCallback;
    static pthread_rwlock_t rwlock;    
    static pthread_mutex_t userOperMapLock;
    static ProcStatusMonitorClientC * thisObject;
    
    DBusServerC *pprocDbusServer;
    DBusClientProxyC *pprocDbusClient;
    string sprocName;
    pthread_t threadID;
    int shut_flag;

friend class ProcStatusMonitorLockC;
};

class ProcStatusMonitorLockC
{
public:
    ProcStatusMonitorLockC();
    ~ProcStatusMonitorLockC();

private:
    bool lockValid;
    ProcStatusMonitorClientC * ppsmcObject;

};

class ProcStatusMonitorServerLockC
{
public:
    ProcStatusMonitorServerLockC();
    ~ProcStatusMonitorServerLockC();

private:
    bool lockValid;
    ProcStatusMonitorServerC * ppsmcObject;

};


#endif
