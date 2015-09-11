#ifndef _GENERAL_TIMER_H_
#define _GENERAL_TIMER_H_

#include <sys/select.h>
#include <pthread.h>
#include <string>
#include <vector>

using namespace std;

#define  TIMER_SEC_TO_MSEC(x)             ((x) * 1000)

typedef void (*GeneralTimerHandler)(string &, void *, void *);

typedef struct
{
    string name;
    int timeout; // microseconds
    int timeleft;
    void *data;
    void *data2;
    GeneralTimerHandler handler;
    bool repeat;
    struct timeval start_interval;
    int start_time;
}GeneralTimerItem;

GeneralTimerItem *CreateTimer(string name, int timeout, void *data, GeneralTimerHandler handler, bool repeat);
int DestroyTimer(GeneralTimerItem *timer);
int StartTimer(GeneralTimerItem * timer);
int StopTimer(GeneralTimerItem * timer);
int GetTimerLeft(GeneralTimerItem * timer);

typedef vector<GeneralTimerItem *> GeneralTimerList;

class GeneralTimer
{
public:
    ~GeneralTimer();
    int StartTimer(GeneralTimerItem * timer, bool autoflag = false);
    int StopTimer(GeneralTimerItem * timer);
    int TimeLeft(GeneralTimerItem * timer);

    void SetInterval(int interval) //microseconds
    {
        m_interval.tv_sec = interval / 1000;
        m_interval.tv_usec = (interval % 1000) * 1000;
    }
    vector<GeneralTimerItem *> &GetTimerList()
    {
        return m_timerlist;
    }

    static GeneralTimer *Instance();
    static void Destroy();
    static int nums;
    pthread_t m_tid;
    
protected:
    GeneralTimer();
    void Start();
    void Stop();
    void HandleStartTimer(int );
    void HandleStopTimer();
    void Print();
    void PrintTimer(GeneralTimerItem *timer);

private:
    static void *ThreadLoop(void *arg);
    void HandleTick(int ticks);

    bool m_exit;
    bool m_started;
    struct timeval m_interval;
    static GeneralTimer *m_instance;
    GeneralTimerList m_timerlist;
    GeneralTimerList m_startRequest;
    GeneralTimerList m_stopRequest;
    pthread_mutex_t m_timermutex;
};

#endif
