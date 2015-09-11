#include "procStatusMonitorInterface.h"

#include <qtimer.h>
#include <QCoreApplication>
#include <QBasicTimer>

#define APP_STATUS_MONITOR_MSG_UPSTATUS         1            ///update the status notice
#define APP_STATUS_MONITOR_MSG_SHUTDOWN         2            ///shutdown notice
#define APP_STATUS_MONITOR_MSG_TOUI             3            ///goto the child function GUI

#define APP_STATUS_MONITOR_DBUS_TIMEOUT         (1*1000)
#define APP_STATUS_MONITOR_SHUT_TIMEOUT         (6*1000)
#define APP_STATUS_MONITOR_SHUT_TIMESALL        (100)
#define APP_STATUS_MONITOR_PROC_SHUT_DELAY      (10)

#define APP_STATUS_MONITOR_TIMER_SHUT           1
#define APP_STATUS_MONITOR_TIMER_NOTE           0




/// Structures Define
typedef pair<ProcStatusMonitorOperC*, unsigned long> ProcStatusMonitorVecType;


/// Static Arguments
PROCSTATUSMONITORMAP ProcStatusMonitorServerC::appStatusList;
map<ProcStatusMonitorOperC*, unsigned long> ProcStatusMonitorClientC::userOperationMap;
ProcStatusMonitorCallbackC  *ProcStatusMonitorClientC::pprocCallback;
ProcStatusMonitorClientC * ProcStatusMonitorClientC::thisObject = NULL;
ProcStatusMonitorServerRepC *  ProcStatusMonitorServerC::pprocCallbackC;
ProcStatusMonitorServerC * ProcStatusMonitorServerC::thisObject = NULL;
pthread_rwlock_t ProcStatusMonitorClientC::rwlock;
pthread_mutex_t ProcStatusMonitorClientC::userOperMapLock;    


/// Local Class define
/// QT timer 

class  ProcStatusMonitorTimerC: public QObject
{
public:
    ProcStatusMonitorTimerC(int flag, ProcStatusMonitorServerC *pProcServer){ tflag = flag; pserver = pProcServer;};
    void timerEvent(QTimerEvent *event);
    void start( int msecond ) { m_ticker.start(msecond, this); }
    void stop() { m_ticker.stop(); }
private:
    QBasicTimer m_ticker;
    int tflag;                // no-zero: exit 0: callback shutdown rate
    ProcStatusMonitorServerC *pserver;

};

void ProcStatusMonitorTimerC::timerEvent(QTimerEvent *event)
{
    if (tflag)
    {
        appSMDbg(" ^^^^^^^^^^^^^^^^  Proc  Now to Exit ! ^^^^^^^^^^^^^^^^^");
        exit(0);
    }
    else
    {
#ifdef DEBUG
        if ( 1 == pserver->procStatusMonitor_timer_rate%20 )
            appSMDbg("-------- TIMER RATE[%d] --------", pserver->procStatusMonitor_timer_rate);

#endif
        pserver->procStatusMonitor_timer_rate++;
        if (pserver->procStatusMonitor_timer_rate > pserver->procStatusMonitor_shutRate)
        {
            long rate = pserver->procStatusMonitor_timer_rate;
            rate = (rate/100 != 0 )*100 + rate * (rate/100 == 0 );            ///not necessary , more reliable
            pserver->pprocCallbackC->handleShutDownRate(rate);
            if ( APP_STATUS_MONITOR_SHUT_TIMESALL == rate )
            {
                stop();
            }
        }
    }
};





/// Dbus class implement
/// Class to handle request message
class ProcStatusMonitorReqC:public DBusRequestC
{
        DBUS_REQUEST_OBJECT(ProcStatusMonitorReqC)

public:
        ProcStatusMonitorReqC():DBusRequestC(REQ_ID_PROC_STATUS_MONITOR_MSG){}
        void print(FILE *fp);

        int     msgType;
        // mustn't use string data type, it cause the message handle function's segment fualt 
        //string  procName;
        char    cprocName[APP_STATUS_MONITOR_NAME_LEN];
        unsigned long  funcNum;
        StruAppStatusMonitorStatus status;
};

/// For dbus message debug
void ProcStatusMonitorReqC::print(FILE *fp)
{
        fprintf(fp,"\tmsgType = %d\n",msgType);
        fprintf(fp,"\trunStatus = %d\n",status.runStatus);
}

/// Class to handle reply message
class ProcStatusMonitorRepC: public  DBusReplyC
{
        DBUS_REPLY_OBJECT(ProcStatusMonitorRepC)

public:
        ProcStatusMonitorRepC():DBusReplyC(REP_ID_PROC_STATUS_MONITOR_MSG){}
        void print(FILE *fp);

        int     rpMsgType;
        StruAppStatusMonitorStatus rpStatus;
};
/// For dbus message debug
void ProcStatusMonitorRepC::print(FILE *fp)
{
        fprintf(fp,"\tmsgType = %d\n",rpMsgType);
        fprintf(fp,"\trunStatus = %d\n",rpStatus.runStatus);
}
/*
class ProcStatusMonitorNotifyC: public  DBusNotifyC
{
        DBUS_NOTIFY_OBJECT(ProcStatusMonitorNotifyC)

public:
        //ProcStatusMonitorNotifyC(int a);
        //~ProcStatusMonitorNotifyC();

        void print(FILE *fp);

        int testCode;
};
*/

ProcStatusMonitorDbusClientC::ProcStatusMonitorDbusClientC(string procName, DBusClientProxyC*pDbusClient)
{
    appName = procName;
    pdbusClient = pDbusClient;
}



/* local method for ProcStatusMonitorOperC's privilidge, pri bigger ProcStatusMonitorOperC called sooner*/
static int procStatusMonitorComp(const ProcStatusMonitorVecType& x, const ProcStatusMonitorVecType& y)
{
    return x.second >= y.second;

};



/*
 *
 *  implement of class ProcStatusMonitorServerC
 *
 */

ProcStatusMonitorServerC::ProcStatusMonitorServerC(ProcStatusMonitorServerRepC *  pCallbackC,    \
                            DBusServerC *pdbusServer, const PROCSTATUSMONITORDBUSVEC dbusClientVec)
{
    if ( NULL == thisObject )
    {
        thisObject = this;
    }
    else
    {
        appSMDbg("------- ONLY one instance is allowed to create !!!!! --------");
        abort();
    }

    appSMDbg("------- ProcStatusMonitorServerC start --------");
    if ( NULL == pdbusServer )
    {
        appSMDbg("xxxxxxxxxxxx  pdbusServer = NULL xxxxxxxxxxxxxxxx ");
    }


    ///init the process Status map
    StruAppStatusMonitorStatus funcStatus;
    funcStatus.runStatus = APP_STATUS_MONITOR_STATUS_IDLE;
    CHILDFUNCSTATUSMAP childFuncMap;
    PROCSTATUSMONITORDBUSVEC::const_iterator p;

    int seq = 0;
    for ( p = dbusClientVec.begin(); p != dbusClientVec.end(); ++p)
    {    seq++;
        procDbusClientSeqMap.insert(pair<int,DBusClientProxyC*>(seq,p->pdbusClient));
        procDbusClientMap.insert(pair<string,DBusClientProxyC*>(p->appName,p->pdbusClient));
        appStatusList.insert( pair<string,CHILDFUNCSTATUSMAP>(p->appName,childFuncMap) );
        appSMDbg("Add monitored process[%s], Seq[%d]", p->appName.c_str(), seq);
    }

    
    appSMDbg("init process map over, size[%d] ", appStatusList.size());
    
    ///save the callback and DBusServerC pointer
    pprocCallbackC = pCallbackC;
    pprocDbusServer = pdbusServer;
    //procDbusClientMap = dbusClientMap;
    procStatusMonitor_shutRate = 0;
    procStatusMonitor_timer_rate = 0;
    threadID = pthread_self();

    int ret;
    ret = pthread_rwlock_init( &rwlock, NULL );
    if ( ret < 0 )
    {
        appSMDbg("xxxxxxxxxx Error init rwlock error[%d] xxxxxxxxxxx", ret);
    }

    ret = pthread_mutex_init(&userOperMapLock, NULL);
    if ( ret < 0 )
    {
        appSMDbg("xxxxxxxxxx Error init userOperMapLock error[%d] xxxxxxxxxxx", ret);
    }
        
    ptimer = new ProcStatusMonitorTimerC(APP_STATUS_MONITOR_TIMER_NOTE, this);
    /// init DbusServer Reqest and Reply Message
    initDbus();

    return;
};

ProcStatusMonitorServerC::~ProcStatusMonitorServerC()
{
    //delete appStatusList;
    if ( NULL != ptimer )
    {
        delete(ptimer);
    }

    thisObject = NULL;

    return;
};

int ProcStatusMonitorServerC::initDbus()
{
    ///register dbus server function handler
    pprocDbusServer->registerService(REQ_ID_PROC_STATUS_MONITOR_MSG, ProcStatusMonitorServerC::dbusServerHandle, \
                                        ProcStatusMonitorReqC::factory);
    
    ///init dbus client
    
    PROCSTATUSMONITORDBUSMAP::const_iterator it;
    for ( it = procDbusClientMap.begin(); it != procDbusClientMap.end(); ++it)
    {
        if ( NULL != it->second )
        {
            (it->second)->registerReply( REP_ID_PROC_STATUS_MONITOR_MSG, ProcStatusMonitorRepC::factory );
            /*
            (it->second)->registerNotify( NOTF_ID_PROC_STATUS_MONITOR_MSG, ProcStatusMonitorServerC::dbusNotifyHandle,    \
                            ProcStatusMonitorNotifyC::factory );
                                */

        }
    }
    return APP_STATUS_MONITOR_E_SUCCESS;
};

void ProcStatusMonitorServerC::dbusServerHandle(DBusRequestC *request, DBusReplyC *&reply)
{
    ProcStatusMonitorReqC * localReq = (ProcStatusMonitorReqC *)request;
    string strProcName = localReq->cprocName;

    appSMDbg( "recv func[%lu] Dbus request type[%d] status[%d]", localReq->funcNum,localReq->msgType, localReq->status.runStatus );
    if ( REQ_ID_PROC_STATUS_MONITOR_MSG != localReq->getIdentifier() )
    {
        appSMDbg("xxxxxxxxxxxxxx Recv wrong dbus request ID [%d] ", localReq->getIdentifier() );
        return;
    }
    
    /// handle different message type
    ProcStatusMonitorRepC * rep = new ProcStatusMonitorRepC();
    rep->result = APP_STATUS_MONITOR_E_INVALID_FUNCNAME;
    switch(localReq->msgType)
    {
    case APP_STATUS_MONITOR_MSG_UPSTATUS:
    {

        PROCSTATUSMONITORMAP::iterator it;
        for ( it = appStatusList.begin(); it != appStatusList.end(); ++it)
        {

            if ( it != appStatusList.end() && it->first == strProcName)
            {
                CHILDFUNCSTATUSMAP::const_iterator p = (it->second).find(localReq->funcNum);
                if ( p != (it->second).end() )
                {
                    (it->second).erase(localReq->funcNum);
                    (it->second)[localReq->funcNum] = localReq->status;
                    ///Callback
                    pprocCallbackC->handleChildFuncStatusChange((it->second).find(localReq->funcNum));
                    rep->result = APP_STATUS_MONITOR_E_SUCCESS;
                    appSMDbg("Update Proc Status funNum[%lu], status[%d]", localReq->funcNum, localReq->status.runStatus);
                }
                else
                {
                    (it->second)[localReq->funcNum] = localReq->status;
                    pprocCallbackC->handleChildFuncStatusChange((it->second).find(localReq->funcNum));
                    rep->result = APP_STATUS_MONITOR_E_SUCCESS;
                    appSMDbg("Add proc[%s] funcName[%lu], status[%d] !", localReq->cprocName, localReq->funcNum, localReq->status.runStatus);
                }
                                            
            }
            else
            {
                //appSMDbg("Wrong function Number[%lu], don't exit !", localReq->funcNum);
                //
            }
        }

        break;
    }
    case APP_STATUS_MONITOR_MSG_SHUTDOWN:
    {
        /// shutdown return; not support now
        appSMDbg("xxxxxxxxxxx shutdown return; not support now");
        break;
    }
    default:
    {
        appSMDbg("xxxxxxxxxxx wrong request ");
        rep->result = APP_STATUS_MONITOR_E_FAIL;
        break;
    }

    }

    reply = rep;
    return;
};

void ProcStatusMonitorServerC::dbusNotifyHandle(DBusNotifyC *notify)
{

    appSMDbg("xxxxxxxxxxxxxx Error: in dbusNotifyHandle xxxxxxxxxxxxxxx");
    return;
};

const PROCSTATUSMONITORMAP * ProcStatusMonitorServerC::getAppStatusList( )
{
    return &appStatusList;

};

int    ProcStatusMonitorServerC::gotoAppFunc( string  sAppName, unsigned long funcName)
{
    PROCSTATUSMONITORMAP::const_iterator it = appStatusList.find(sAppName);
    if ( it != appStatusList.end() )
    {
        //CHILDFUNCSTATUSMAP::const_iterator p = (it->second).find(funcName);
        //if ( p != (it->second).end() )
        //{
            ///find this child function now to find it's dbus client
            ///    DBusClientC * pdbusClient = procDbusClientMap[sAppName];
            PROCSTATUSMONITORDBUSMAP::const_iterator it_busClient = procDbusClientMap.find(sAppName);
            if ( it_busClient != procDbusClientMap.end() )
            {
                /// prepare a dbus request and phase the reply
                ProcStatusMonitorReqC dbusReq;
                DBusReplyC *pdbusRep = NULL;

                /// check the current status
                /*
                if ( APP_STATUS_MONITOR_STATUS_EXIT == (p->second).runStatus )
                {
                    appSMDbg("xxxxxxxxx Error the Process[%s] had exited ! xxxxxxxxxx", sAppName.c_str());
                    return APP_STATUS_MONITOR_E_WRONG_STATUS;
                }
                */
                dbusReq.funcNum = funcName;
                dbusReq.msgType = APP_STATUS_MONITOR_MSG_TOUI;
                int result = (it_busClient->second)->sendService(&dbusReq, pdbusRep, APP_STATUS_MONITOR_DBUS_TIMEOUT );
                if ( result < 0 )
                {
                    appSMDbg("Dbus sendService:goto UI Error:[%d] ", result);
                }
                else
                {
                    appSMDbg("Dbus sendService:goto UI return:[%d] ", pdbusRep->result);
                    if ( NULL != pdbusRep )
                    {
                        delete pdbusRep;
                    }
                }
                
            }
            else
            {
                appSMDbg("Wrong process Name[%s], don't exit !", sAppName.c_str());
                return APP_STATUS_MONITOR_E_INVALID_APPNAME;
            }
        //}
        /*else
        {
            appSMDbg("Wrong function Number[%lu], don't exit !", funcName);
            return APP_STATUS_MONITOR_E_INVALID_FUNCNAME;
        }*/
    }
    else
    {
        appSMDbg("Wrong process Name[%s], don't exit !", sAppName.c_str());
        return APP_STATUS_MONITOR_E_INVALID_APPNAME;
    }

    return APP_STATUS_MONITOR_E_SUCCESS;

}

void ProcStatusMonitorServerC::shutDown()
{

    /// start a timer for shut down rate 
    ptimer->start(APP_STATUS_MONITOR_SHUT_TIMEOUT/APP_STATUS_MONITOR_SHUT_TIMESALL);
    
    

    /// note every process shut down message
    int count = 0;
    int process_num = procDbusClientSeqMap.size();
    
    map<int, DBusClientProxyC*>::const_iterator p;
    while(1)
    {
        /// count the shut down speed, and note the monitor
        procStatusMonitor_shutRate = (count*APP_STATUS_MONITOR_SHUT_TIMESALL)/process_num;
        if ( procStatusMonitor_shutRate > procStatusMonitor_timer_rate )
        {
            pprocCallbackC->handleShutDownRate(procStatusMonitor_shutRate);
        }
        if ( procDbusClientSeqMap.size() == 0 )
        {
            break;
        }
    
        for ( p = procDbusClientSeqMap.begin(); p != procDbusClientSeqMap.end(); ++p)
        {
            /// prepare a dbus request and phase the reply
            ProcStatusMonitorReqC dbusReq;
            DBusReplyC *pdbusRep;

            /// check the current status
            //appSMDbg("Note processSeq[%d] pclient[%x] shutdown start", p->first, p->second);
            
            if ( NULL != p->second )        /// monitored process is remote, need dbus to note
            {
                dbusReq.msgType = APP_STATUS_MONITOR_MSG_SHUTDOWN;
                DBusClientProxyC *pclient = p->second;
                DBusClientC*pcli = (DBusClientC*)pclient;
                int result = pcli->sendService(&dbusReq, pdbusRep, 20 );
                
                if ( result < 0 )
                {
                    //appSMDbg("Dbus shut down message reply Error:[%d] procSeq[%d] timeout[%d]", result,p->first, DBUS_RESULT_ERROR_TIMEOUT);
                    /// if process don't exit
                    if ( DBUS_RESULT_ERROR_NO_SERVER == result )
                    {
                        count++;
                        appSMDbg("Dbus shut down message OK return:[%d] shutNum[%d] current[%d] ", result,count,p->first);
                        procDbusClientSeqMap.erase(p->first);
                        break;
                    
                    }
                    else if ( DBUS_RESULT_ERROR_TIMEOUT != result )
                    {
                        appSMDbg("Dbus shut down message reply Error:[%d] shutNum[%d][%d] Mapsize[%d]", result,count,process_num,procDbusClientSeqMap.size());
                        
                    }
                }
                else
                {    
                    count++;
                    appSMDbg("Dbus shut down message OK return:[%d] shutNum[%d]current[%d] ", pdbusRep->result,count,p->first);
                    if ( NULL != pdbusRep )
                    {
                        delete pdbusRep;
                    }
                    procDbusClientSeqMap.erase(p->first);
                    break;
                }
                    
                
            }
            else    /// handle shut task of the ProcStatusMonitorServerC process itself
            {
                pthread_rwlock_wrlock(&rwlock);
                
                pthread_mutex_lock(&userOperMapLock);
                vector<ProcStatusMonitorVecType> vec;
                map<ProcStatusMonitorOperC*, unsigned long>::iterator curr;
                for ( curr = userOperationMap.begin(); curr != userOperationMap.end(); ++curr )
                {
                    vec.push_back(make_pair(curr->first, curr->second));
                }
                qSort( vec.begin(), vec.end(), procStatusMonitorComp );
                vector<ProcStatusMonitorVecType>::iterator vec_it;
                for ( vec_it = vec.begin(); vec_it != vec.end(); ++vec_it)
                {
                    if ( userOperationMap.end() != userOperationMap.find(vec_it->first))
                    {
                        ///
                        (vec_it->first)->reclaimer();
                        //appSMDbg("Callback Pri[%lu] ", vec_it->second);
                    }
                }

                /// count the shutdown speed again, add the appManager itself
                count++;
                appSMDbg("AppMananger itself shut down over shutNum[%d] current[%d] ", count, p->first);
                procDbusClientSeqMap.erase(p->first);
                break;
            }
            

            //appSMDbg("Note procSeqNum[%d] shutdown over", p->first);
        
        } /// end of for
    }
    
    

    appSMDbg("^^^^^^^ Note Proc over and wait for time over ^^^^^^^^^ ");
    return;
}

int ProcStatusMonitorServerC::registerOperation( ProcStatusMonitorOperC * pOperC, unsigned long pri )
{
    if ( NULL != pOperC )
    {
        pthread_mutex_lock(&userOperMapLock);
        userOperationMap.insert( pair<ProcStatusMonitorOperC*,unsigned long>(pOperC,pri)  );
        pthread_mutex_unlock(&userOperMapLock);
    }
    else
    {
        return APP_STATUS_MONITOR_E_INVALID_ARGUMENT;
    }
    return APP_STATUS_MONITOR_E_SUCCESS;
}

int ProcStatusMonitorServerC::realseOperation( ProcStatusMonitorOperC * pOperC )
{
    if ( NULL != pOperC )
    {
        pthread_mutex_lock(&userOperMapLock);
        userOperationMap.erase(userOperationMap.find(pOperC));
        pthread_mutex_unlock(&userOperMapLock);
    }
    else
    {
        return APP_STATUS_MONITOR_E_INVALID_ARGUMENT;
    }
    return APP_STATUS_MONITOR_E_SUCCESS;

}



/*
 *
 *    implement of class ProcStatusMonitorClientC
 *
 */
ProcStatusMonitorClientC::ProcStatusMonitorClientC(string appName, DBusServerC *pdbusServer, \
                                DBusClientProxyC * pdbusClient, ProcStatusMonitorCallbackC *pcallbackC )
{

    if ( NULL == thisObject )
    {
        thisObject = this;
    }
    else
    {
        appSMDbg("------- ONLY one instance is allowed to create !!!!! --------");
        abort();
    }

    appSMDbg("------- ProcStatusMonitorClientC start NNWWWW--------");
    if ( appName.length() < 1 )
    {
        appSMDbg("xxxxxxxxxxxx Error Wrong AppName xxxxxxxxxxxxxxxxxx");
    }
    sprocName= appName;
    pprocDbusServer = pdbusServer;
    pprocDbusClient = pdbusClient;
    pprocCallback = pcallbackC;
    threadID = pthread_self();
    shut_flag = 0;

    ///init rwlock
    int ret;
    ret = pthread_rwlock_init( &rwlock, NULL );
    if ( ret < 0 )
    {
        appSMDbg("xxxxxxxxxx Error init rwlock xxxxxxxxxxx");
    }
    ret = pthread_mutex_init(&userOperMapLock, NULL);
    if ( ret < 0 )
    {
        appSMDbg("xxxxxxxxxx Error init userOperMapLock error[%d] xxxxxxxxxxx", ret);
    }
    
    ///init Dbus
    initDbus();
    
    return;
}

ProcStatusMonitorClientC::~ProcStatusMonitorClientC()
{
        thisObject = NULL;
}

int ProcStatusMonitorClientC::initDbus()
{
    ///register dbus server function handler
    pprocDbusServer->registerService(REQ_ID_PROC_STATUS_MONITOR_MSG, ProcStatusMonitorClientC::dbusServerHandle, \
                                        ProcStatusMonitorReqC::factory);
    
    ///init dbus client
    pprocDbusClient->registerReply( REP_ID_PROC_STATUS_MONITOR_MSG, ProcStatusMonitorRepC::factory );
    /*
    pprocDbusClient->registerNotify( NOTF_ID_PROC_STATUS_MONITOR_MSG, ProcStatusMonitorClientC::dbusNotifyHandle,    \
                                ProcStatusMonitorNotifyC::factory );
    */        
    return APP_STATUS_MONITOR_E_SUCCESS;
};


void static procShutDownDelay(void)
{
    appSMDbg(" ^^^^^^^^^^^^^^^^  Proc  Now to Exit ! ^^^^^^^^^^^^^^^^^");
    exit(0);

}

void ProcStatusMonitorClientC::dbusServerHandle(DBusRequestC *request, DBusReplyC *&reply)
{
    ProcStatusMonitorReqC * localReq = (ProcStatusMonitorReqC *)request;

    //appSMDbg( "recv Dbus request type[%d] status[%d]",localReq->msgType, localReq->status.runStatus );
    
    if ( REQ_ID_PROC_STATUS_MONITOR_MSG != localReq->getIdentifier() )
    {
        appSMDbg("xxxxxxxxxxxxxx Recv wrong dbus request ID [%d] ", localReq->getIdentifier() );
        return;
    }
    
    /// handle different message type
    ProcStatusMonitorRepC * rep = new ProcStatusMonitorRepC();
    rep->rpMsgType = APP_STATUS_MONITOR_MSG_SHUTDOWN;
    rep->result = APP_STATUS_MONITOR_E_SUCCESS;
    reply = rep;
    
    switch(localReq->msgType)
    {
    case APP_STATUS_MONITOR_MSG_UPSTATUS:
    {
        appSMDbg("xxxxxxxxxxx wrong request");
        rep->result = APP_STATUS_MONITOR_E_FAIL;
        break;
    }
    case APP_STATUS_MONITOR_MSG_SHUTDOWN:
    {
        //appSMDbg("recv APP_STATUS_MONITOR_MSG_SHUTDOWN ");
        
        pthread_rwlock_wrlock(&rwlock);
        //appSMDbg("recv APP_STATUS_MONITOR_MSG_SHUTDOWN  to handle  callback11");
        /// lock the userOperationMap and handle the content
        if ( 0 == pthread_mutex_trylock(&userOperMapLock) )
        {
            //appSMDbg("recv APP_STATUS_MONITOR_MSG_SHUTDOWN  to handle  callback22");
            vector<ProcStatusMonitorVecType> vec;
            map<ProcStatusMonitorOperC*, unsigned long>::iterator curr;
            for ( curr = userOperationMap.begin(); curr != userOperationMap.end(); ++curr )
            {
                vec.push_back(make_pair(curr->first, curr->second));
            }
            qSort( vec.begin(), vec.end(), procStatusMonitorComp );
            vector<ProcStatusMonitorVecType>::iterator vec_it;
            for ( vec_it = vec.begin(); vec_it != vec.end(); ++vec_it)
            {
                if ( userOperationMap.end() != userOperationMap.find(vec_it->first))
                {
                    ///
                    (vec_it->first)->reclaimer();
                    //appSMDbg("Callback Pri[%lu] ", vec_it->second);
                }
            }
            
            
            appSMDbg("handle APP_STATUS_MONITOR_MSG_SHUTDOWN now start timer[%d]", APP_STATUS_MONITOR_PROC_SHUT_DELAY );
            /// set a timer and exit later
            /// attion: this timer must be use in QT Main thread£¬I don's release the timer object,because process will exit soon
            ProcStatusMonitorTimerC * ptimer = new ProcStatusMonitorTimerC(APP_STATUS_MONITOR_TIMER_SHUT, NULL);
            ptimer->start(APP_STATUS_MONITOR_PROC_SHUT_DELAY);
        }
        else
        {
            appSMDbg("handle shut down busy" );
        }
        //exit(0);

        break;
    }
    case APP_STATUS_MONITOR_MSG_TOUI:
    {
        ///
        pprocCallback->handleGotoAppStatus(localReq->funcNum);
        rep->result = APP_STATUS_MONITOR_E_SUCCESS;
        break;
    }
    default:
    {
        appSMDbg("xxxxxxxxxxx wrong requestxxxxxxxxxxxx");
        rep->result = APP_STATUS_MONITOR_E_FAIL;
        break;
    }

    }
    
    //appSMDbg("handle  Dbus request over");
};

void ProcStatusMonitorClientC::dbusNotifyHandle(DBusNotifyC *notify)
{

    appSMDbg("xxxxxxxxxxxxxx Error: in dbusNotifyHandle xxxxxxxxxxxxxxx");
    return;
};

int ProcStatusMonitorClientC::registerOperation( ProcStatusMonitorOperC * pOperC, unsigned long pri )
{
    if ( NULL != pOperC )
    {
        pthread_mutex_lock(&userOperMapLock);
        userOperationMap.insert( pair<ProcStatusMonitorOperC*,unsigned long>(pOperC,pri)  );
        pthread_mutex_unlock(&userOperMapLock);
    }
    else
    {
        return APP_STATUS_MONITOR_E_INVALID_ARGUMENT;
    }
    return APP_STATUS_MONITOR_E_SUCCESS;
}

int ProcStatusMonitorClientC::realseOperation( ProcStatusMonitorOperC * pOperC )
{
    if ( NULL != pOperC )
    {
        pthread_mutex_lock(&userOperMapLock);
        userOperationMap.erase(userOperationMap.find(pOperC));
        pthread_mutex_unlock(&userOperMapLock);
    }
    else
    {
        return APP_STATUS_MONITOR_E_INVALID_ARGUMENT;
    }
    return APP_STATUS_MONITOR_E_SUCCESS;

}

int ProcStatusMonitorClientC::changeAppFuncStatus( unsigned long funcName, StruAppStatusMonitorStatus status)
{
    ProcStatusMonitorReqC dbusReq;
    DBusReplyC *pdbusRep = NULL;
    dbusReq.msgType = APP_STATUS_MONITOR_MSG_UPSTATUS;
    dbusReq.funcNum = funcName;
    dbusReq.status = status;
    strcpy( dbusReq.cprocName, sprocName.c_str());
    
    int result = pprocDbusClient->sendService(&dbusReq, pdbusRep, APP_STATUS_MONITOR_DBUS_TIMEOUT );

    if ( result < 0 )
    {
        appSMDbg("Dbus msg change Status reply Error:[%d] ", result);
    }
    else
    {    
        //appSMDbg("Dbus msg change Status OK return:[%d]", pdbusRep->result);
        if ( NULL != pdbusRep )
        {
            delete pdbusRep;
        }
    }
    return APP_STATUS_MONITOR_E_SUCCESS;
}




/*
 *
 *    implement of class ProcStatusMonitorLockC
 *
 */
ProcStatusMonitorLockC::ProcStatusMonitorLockC()
{
    if ( NULL != ProcStatusMonitorClientC::thisObject )
    {
        //assert(NULL != ppsmcObject);
        ppsmcObject= ProcStatusMonitorClientC::thisObject;
        lockValid = false;
        
        if ( pthread_self() != ppsmcObject->threadID)
        {
            lockValid = ( 0 == pthread_rwlock_rdlock( &(ppsmcObject->rwlock)) );
        }
    }

    return;
}
ProcStatusMonitorLockC::~ProcStatusMonitorLockC()
{
    if ( NULL != ProcStatusMonitorClientC::thisObject )
    {
        ppsmcObject = ProcStatusMonitorClientC::thisObject;
        if ( lockValid )
        {
            pthread_rwlock_unlock( &(ppsmcObject->rwlock) );
        }
    }
    
    return;
}

ProcStatusMonitorServerLockC::ProcStatusMonitorServerLockC( )
{

    if ( NULL != ProcStatusMonitorServerC::thisObject )
    {
        //assert(NULL != ppsmcObject);
        ppsmcObject= ProcStatusMonitorServerC::thisObject;
        lockValid = false;
        
        if ( pthread_self() != ppsmcObject->threadID)
        {
            lockValid = ( 0 == pthread_rwlock_rdlock( &(ppsmcObject->rwlock)) );
        }
    }

    return;
}
ProcStatusMonitorServerLockC::~ProcStatusMonitorServerLockC()
{

    if ( NULL != ProcStatusMonitorServerC::thisObject )
    {
        ppsmcObject = ProcStatusMonitorServerC::thisObject;
        if ( lockValid )
        {
            pthread_rwlock_unlock( &(ppsmcObject->rwlock) );
        }
    }
    return;
}
