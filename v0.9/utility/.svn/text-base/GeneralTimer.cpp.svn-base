#include <errno.h>
#include <signal.h>
#include <ostream>
#include <sys/time.h>
#include "GeneralTimer.h"
#include "LogUtility.h"
#include "boost/timer/timer.hpp"

using boost::timer::cpu_timer;
using boost::timer::cpu_times;
using boost::timer::nanosecond_type;

//#define DBUG_TIMER
#ifdef DBUG_TIMER
#define Printf printf
#else
#define Printf(x,...) 
#endif
GeneralTimer *GeneralTimer::m_instance = NULL;
int GeneralTimer::nums = 0;

GeneralTimerItem *CreateTimer(string name, int timeout, void *data, GeneralTimerHandler handler, bool repeat)
{
    GeneralTimerItem *timer = new GeneralTimerItem();
    timer->name = name;
    timer->timeout = timeout;
    timer->data = data;
    timer->handler = handler;
    timer->repeat = repeat;
    timer->start_interval.tv_sec = 0;
    timer->start_interval.tv_usec = 0;
    
    return timer;
}

int DestroyTimer(GeneralTimerItem *timer)
{
    if(timer == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "DestroyTimer timer is null.");
        return -1;
    }
    
    StopTimer(timer);
    
    delete timer;
    return 0;
}

int StartTimer(GeneralTimerItem *timer)
{
	timer->start_interval.tv_sec = 0;
	timer->start_interval.tv_usec = 0;
	GeneralTimer::Instance()->StartTimer(timer);
	pthread_kill(GeneralTimer::Instance()->m_tid, SIGUSR1);
	return 0;
}

int StopTimer(GeneralTimerItem *timer)
{
	GeneralTimer::Instance()->StopTimer(timer);
	pthread_kill(GeneralTimer::Instance()->m_tid, SIGUSR1);
	return  0;
}

int GetTimerLeft(GeneralTimerItem * timer)
{
    return GeneralTimer::Instance()->TimeLeft(timer);
}

static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

GeneralTimer *GeneralTimer::Instance()
{
    if(!m_instance)
    {
        m_instance = new GeneralTimer();
    }
    
    return m_instance;
}

void GeneralTimer::Destroy()
{
    if(m_instance != NULL)
    {
        delete m_instance;
    }
}

GeneralTimer::GeneralTimer()
{
    m_exit = false;
    m_interval.tv_sec = 1;
    m_interval.tv_usec = 0;
    m_started = false;
    m_startRequest.clear();
    m_stopRequest.clear();
    m_timerlist.clear();

    if(pthread_mutex_init(&m_timermutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GeneralTimer::GeneralTimer: mutex init failed.");
        return;
    }
    
    Start();
}

GeneralTimer::~GeneralTimer()
{
    Stop();
    pthread_mutex_destroy(&m_timermutex);
}

void GeneralTimer::Start()
{
    if(m_started)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "GeneralTimer::Start() already started.");
        return;
    }

    m_exit = false;
   #if 0 
    pthread_attr_t SchedAttr;
    sched_param SchedParam;  
    int MidPriority,MaxPriority,MinPriority;  
    int policy;
    
    pthread_attr_init(&SchedAttr);      
    MaxPriority = sched_get_priority_max(SCHED_RR);     
    MinPriority = sched_get_priority_min(SCHED_RR);     
    MidPriority = (MaxPriority + MinPriority)/2;      
    SchedParam.sched_priority = MidPriority;      
    pthread_attr_setschedparam(&SchedAttr,&SchedParam);      
    pthread_attr_setinheritsched(&SchedAttr,PTHREAD_EXPLICIT_SCHED);      
    pthread_attr_setschedpolicy(&SchedAttr,SCHED_RR);      

    pthread_attr_getschedparam(&SchedAttr,&SchedParam);      
    pthread_attr_getschedpolicy(&SchedAttr,&policy);      
    LogUtility::Log(LOG_LEVEL_DEBUG,"GeneralTimer::Start policy=%d proiority=%d \n",policy,SchedParam.sched_priority);
    #endif 
    pthread_create(&m_tid, NULL, this->ThreadLoop, this);
    
    m_started = true;

    return;
}

void GeneralTimer::Stop()
{
    if(!m_started)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "GeneralTimer::Start() already started.");
        return;
    }

    m_exit = true;

    pthread_kill(m_tid, SIGUSR1);

    pthread_join(m_tid, NULL);

    m_started = false;

    return;
}

void *GeneralTimer::ThreadLoop(void *arg)
{
    string serverAddress;
    cpu_timer timer;
    int time_wall;
    int time_wall_all = 0;
    
    timer.stop();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "GeneralTimer::ThreadLoop entry.");
    GeneralTimer *generalTimer = (GeneralTimer *)arg;
    
    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);
    
    while(!generalTimer->m_exit)
    {
        struct timeval interval = generalTimer->m_interval;
        
        if(!timer.is_stopped())
        {
            time_wall = (int)(timer.elapsed().wall / 1000000);
            int diff = (interval.tv_sec * 1000 + interval.tv_usec / 1000) - time_wall;
            
            if(diff > 0)
            {
                if(generalTimer->m_timerlist.begin() != generalTimer->m_timerlist.end())
                {
                    GeneralTimerItem *currentTimer = *(generalTimer->m_timerlist.begin());
                    if(currentTimer != NULL)
                    {
                        if(diff > currentTimer->timeleft)
                        {
                            diff = currentTimer->timeleft;
                        }
                    }
                }
                interval.tv_sec = diff / 1000;
                interval.tv_usec = (diff % 1000) * 1000;
		if(nums == 0)
			select(0, NULL, NULL, NULL, &interval);
            }
        }
        
       
        time_wall = (int)(timer.elapsed().wall / 1000000);
	//Printf("elapsed all is %d\n", time_wall_all += time_wall);
        timer.start();
        generalTimer->HandleTick(time_wall);
        pthread_mutex_lock(&generalTimer->m_timermutex);
	nums = 0;
        time_wall = (int)(timer.elapsed().wall / 1000000);
        generalTimer->HandleStartTimer(time_wall);
        generalTimer->HandleStopTimer();
        pthread_mutex_unlock(&generalTimer->m_timermutex);
        
        //generalTimer->Print();
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::ThreadLoop exit.");
    
    return NULL;
}

void GeneralTimer::HandleTick(int tick)
{
    GeneralTimerList::iterator  it;

    for(it = m_timerlist.begin(); it != m_timerlist.end(); it++)
    {
        GeneralTimerItem *currentTimer = *it;
        
		Printf("^^^^^^^^^^^current timer name is %s, time is %d ,tick is %d, timeleft is %d \n", currentTimer->name.c_str(), time(NULL), tick, currentTimer->timeleft);

        if(tick >= currentTimer->timeleft)
        {
            if(currentTimer->handler)
            {
                currentTimer->handler(currentTimer->name, currentTimer->data, currentTimer->data2);
                if(currentTimer->repeat)
                {
                    StartTimer(currentTimer, true);
                }
            }
            tick -= currentTimer->timeleft;
        }
        else
        {
            currentTimer->timeleft -= tick;
            break;
        }
    }
    
    if(it != m_timerlist.begin())
    {
        m_timerlist.erase(m_timerlist.begin(), it);
    }
}

int GeneralTimer::StartTimer(GeneralTimerItem *timer, bool autoflag)
{
    GeneralTimerList::iterator it;
    
    LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::StartTimer timer %s.", timer->name.c_str());
    
    pthread_mutex_lock(&m_timermutex);

    for(GeneralTimerList::iterator it = m_stopRequest.begin(); it != m_stopRequest.end(); it++)
    {
        if(timer == (*it))
        {
            if(autoflag)
            {
                LogUtility::Log(LOG_LEVEL_INFO, "GeneralTimer::StartTimer timer is in stop list.");
                pthread_mutex_unlock(&m_timermutex);
                return 0;
            }
            else
            {
                LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::StartTimer remove timer from stop list.");
                m_stopRequest.erase(it);
                break;
            }
        }
    }

    for(it = m_startRequest.begin(); it != m_startRequest.end(); it ++)
    {
        if(timer == (*it))
        {
            LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::StartTimer timer already in the list.");
            break;
        }
    }
    
    if(it == m_startRequest.end())
    {
        m_startRequest.push_back(timer);
    }

    if(autoflag == false)
	    nums++;
    pthread_mutex_unlock(&m_timermutex);
    
    return 0;
}

int GeneralTimer::StopTimer(GeneralTimerItem *timer)
{
    GeneralTimerList::iterator it;
    
    LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::StopTimer timer %s.", timer->name.c_str());
    
    pthread_mutex_lock(&m_timermutex);

    for(it = m_stopRequest.begin(); it != m_stopRequest.end(); it ++)
    {
        if(timer == (*it))
        {
            LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::StopTimer timer already in the list.");
            break;
        }
    }
    
    if(it == m_stopRequest.end())
    {
        m_stopRequest.push_back(timer);
    }

    for(GeneralTimerList::iterator it = m_startRequest.begin(); it != m_startRequest.end(); it ++)
    {
        if(timer == (*it))
        {
            LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::StopTimer remove timer from stop list.");
            m_startRequest.erase(it);
            break;
        }
    }
	nums++;
    pthread_mutex_unlock(&m_timermutex);
    
    return 0;
}

void GeneralTimer::HandleStartTimer(int diff)
{
    GeneralTimerList::iterator it, it2;
    bool found = false;
    struct timeval timeval_tmp;
    int start_tmp;
    gettimeofday(&timeval_tmp, NULL);

    for(it = m_startRequest.begin(); it != m_startRequest.end(); it++)
    {
        GeneralTimerItem *timer = *it;

	if(timer->start_interval.tv_sec == 0)
	{
    		gettimeofday(&timer->start_interval, NULL);
		if(timer->start_interval.tv_usec >= 750000)
		{
			timer->start_interval.tv_usec = 750000;
		}else if(timer->start_interval.tv_usec <= 250000)
		{
			timer->start_interval.tv_usec = 250000;
		}

        timer->timeleft = timer->timeout + diff;
		timer->start_time = timer->start_interval.tv_sec*1000+timer->start_interval.tv_usec/1000;
	}
	else
	{
		//start_tmp = ((timeval_tmp.tv_sec*1000 + timeval_tmp.tv_usec/1000) - timer->start_time)% timer->timeout;
		start_tmp = ((timeval_tmp.tv_sec-timer->start_interval.tv_sec)*1000 + (timeval_tmp.tv_usec-timer->start_interval.tv_usec)/1000) % timer->timeout;
		//timer->timeleft = ((timeval_tmp.tv_sec*1000 + timeval_tmp.tv_usec/1000)-(timer->start_interval.tv_sec*1000+timer->start_interval.tv_usec/1000))%timer->timeout;
		//timer->timeleft = start_tmp > 500?start_tmp:(timer->timeout-start_tmp);
		//Printf("start_tmp is %d\n", start_tmp);
        Printf("HandleStartTimer: 3 timeout=%d, start_tmp=%d, diff=%d\n", timer->timeout, start_tmp,diff);      
		if(start_tmp > (timer->timeout >> 1))
			timer->timeleft = (timer->timeout << 1) - start_tmp + diff;
		else
			timer->timeleft = timer->timeout - start_tmp + diff;
		Printf("HandleStartTimer: 7 timeleft::%s is %d, %d, %d, %d, %d, %d\n", timer->name.c_str(), timer->timeleft, timer->timeout, 
          timeval_tmp.tv_sec, timer->start_interval.tv_sec, timeval_tmp.tv_usec, timer->start_interval.tv_usec);
	}
        
        found = false;

        for(it2 = m_timerlist.begin(); it2 != m_timerlist.end();)
        {
            GeneralTimerItem *currentTimer = *it2;
            
            if(timer == (*it2))
            {
                LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::HandleStartTimer timer not stoped yet 1.");
                m_timerlist.erase(it2);
                continue;
            }

            if(found)
            {
                LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::HandleStartTimer timer found next.");
                break;
            }
            
            if(timer->timeleft >= currentTimer->timeleft)
            {
                timer->timeleft -= currentTimer->timeleft;
                it2 ++;
                continue;
            }
            
            m_timerlist.insert(it2, timer);
            currentTimer->timeleft -= timer->timeleft;
            found = true;
            break;
        }

        if(!found)
        {
            m_timerlist.push_back(timer);
        }
    }
    
    m_startRequest.clear();

    return;
}

void GeneralTimer::HandleStopTimer()
{
    GeneralTimerList::iterator it, it2;

    for(it = m_stopRequest.begin(); it != m_stopRequest.end(); it ++)
    {
        GeneralTimerItem *timer = *it;
        int time_found = 0;
        for(it2 = m_timerlist.begin(); it2 != m_timerlist.end();)
        {
            if(timer == (*it2))
            {
                m_timerlist.erase(it2);
                time_found = timer->timeleft;
                continue;
            }
            
            if(time_found)
            {
                GeneralTimerItem *tmptimer = *it2;
                tmptimer->timeleft += time_found;
                time_found = 0;
		break;
            }
            
            it2 ++;
        }
    }
    
    m_stopRequest.clear();
    
    return;
}

int GeneralTimer::TimeLeft(GeneralTimerItem * timer)
{
    int time_left = 0;
    bool found = false;
    GeneralTimerList::iterator it;

    pthread_mutex_lock(&m_timermutex);

    for(it = m_timerlist.begin(); it != m_timerlist.end(); it ++)
    {
        GeneralTimerItem *currentTimer = *it;
        
        time_left += currentTimer->timeleft;
        
        if(timer == currentTimer)
        {
            LogUtility::Log(LOG_LEVEL_INFO, "GeneralTimer::TimeLeft find timer.");
            found = true;
            break;
        }
    }
    
    pthread_mutex_unlock(&m_timermutex);
    
    if(!found)
    {
        LogUtility::Log(LOG_LEVEL_WARN, "GeneralTimer::TimeLeft failed to find timer.");
        time_left = -1;
    }

    return time_left;
}

void GeneralTimer::PrintTimer(GeneralTimerItem *timer)
{
    cout << "Name: " << timer->name.c_str() << endl;
    cout << "Timeout: " << timer->timeout << endl;
    cout << "Timeleft: " << timer->timeleft << endl;
    cout << "Repeat: " << timer->repeat << endl;
}

void GeneralTimer::Print()
{
#if 0
    cout << "TimerList: " << endl;
    for(GeneralTimerList::iterator it = m_timerlist.begin(); it != m_timerlist.end(); it ++)
    {
        GeneralTimerItem *timer = *it;
        PrintTimer(timer);
    }

    cout << "StartRequest: " << endl;
    for(GeneralTimerList::iterator it = m_startRequest.begin(); it != m_startRequest.end(); it ++)
    {
        GeneralTimerItem *timer = *it;
        PrintTimer(timer);
    }

    cout << "StopRequest: " << endl;
    for(GeneralTimerList::iterator it = m_stopRequest.begin(); it != m_stopRequest.end(); it ++)
    {
        GeneralTimerItem *timer = *it;
        PrintTimer(timer);
    }
#endif
}
