#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <sstream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <pthread.h>

using namespace std;

template <class Type>
inline Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;    
};

inline string numToString(const unsigned char& value)
{
    stringstream strStream;
    strStream << std::setw(2) << std::setfill('0') << (int)value; 
    return strStream.str();
};

template <class Type>
inline string numToString(const Type& value)
{
    stringstream strStream;
    strStream << value; 
    return strStream.str();
};

inline string hexToString(int value)
{
    stringstream strStream;
    strStream << std::hex << std::setw(2) << std::setfill('0') << value; 
    return strStream.str();
};

inline string seqToString(vector<unsigned char> &seq)
{
    string str;
    for(int i = 0; i < (int)seq.size(); i ++)
    {
        str += hexToString(seq[i]);
    }
    return str;
}

inline string vectorToString(vector<unsigned char> &seq)
{
    string str;
    for(int i = 0; i < (int)seq.size(); i ++)
    {
        if(i % 16 == 0)
        {
            str += "\r\n";
        }
        str += hexToString(seq[i]);
    }
    return str;
}

inline int GetLongFromVector(vector<unsigned char> &vec, int index)
{
    unsigned char ch1 = vec[index];
    unsigned char ch2 = vec[index+1];
    unsigned char ch3 = vec[index+2];
    unsigned char ch4 = vec[index+3];
    
    return ((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | (ch4));
}

inline unsigned char CharToBCD(char ch)
{
    if((ch >= '0') & (ch <= '9'))
    {
        return (unsigned char)(ch - '0');
    }
    else if((ch >= 'A') & (ch <= 'F'))
    {
        return (unsigned char)(ch - 'A' + 0xA);
    }
    else if((ch >= 'a') & (ch <= 'f'))
    {
        return (unsigned char)(ch - 'a' + 0xA);
    }
    else
    {
        return 0x0;
    }
}

#define BCD_TO_VALUE(c) ((((c) >> 4) & 0xF) * 10 + ((c) & 0xF))
#define VALUE_TO_BCD(c)  (((((c) / 10) & 0xF) << 4) | ((c) % 10))

inline void IntToBcd(int value, vector<unsigned char> &bcd, int len)
{
    int current_value = value;
    
    bcd.clear();
    bcd.insert(bcd.begin(), len, 0);
    for(int i = 0; i < len; i ++)
    {
        unsigned char ch = current_value % 100;
        current_value /= 100;
        bcd[len - i - 1] = VALUE_TO_BCD(ch);
    }
    return;
}

inline int BcdToInt(vector<unsigned char> &bcd)
{
    int value = 0;
    
    for(int i = 0; i < (int)bcd.size(); i ++)
    {
        unsigned char ch = BCD_TO_VALUE(bcd[i]);
        value = value * 100 + ch;
    }
    return value;
}

inline unsigned int GetTime(vector<unsigned char> &bcd)
{
    struct tm lt;
    if(bcd.size() != 6)
    {
        return -1;
    }
    
    lt.tm_year = BCD_TO_VALUE(bcd[0]) + 100;
    lt.tm_mon = BCD_TO_VALUE(bcd[1]) - 1;
    lt.tm_mday = BCD_TO_VALUE(bcd[2]);
    lt.tm_hour = BCD_TO_VALUE(bcd[3]);
    lt.tm_min = BCD_TO_VALUE(bcd[4]);
    lt.tm_sec = BCD_TO_VALUE(bcd[5]);
    return (unsigned int)mktime(&lt);
}

inline void GetTime(long long itime, vector<unsigned char> &bcd)
{
    struct tm *lt, p;
    const time_t ttime = itime;
    
    localtime_r(&ttime, &p);
    lt = &p;
    bcd.clear();
    bcd.push_back(VALUE_TO_BCD(lt->tm_year % 100));
    bcd.push_back(VALUE_TO_BCD(lt->tm_mon + 1));
    bcd.push_back(VALUE_TO_BCD(lt->tm_mday));
    bcd.push_back(VALUE_TO_BCD(lt->tm_hour));
    bcd.push_back(VALUE_TO_BCD(lt->tm_min));
    bcd.push_back(VALUE_TO_BCD(lt->tm_sec));
    return;
}

inline string GetTime(long long itime, char slash, bool with_sec)
{
    struct tm *lt, p;
    string strtime;
    const time_t ttime = itime;
    
    localtime_r(&ttime, &p);
    lt = &p;
    strtime = numToString((unsigned char)(lt->tm_year % 100));
    strtime += numToString((unsigned char)(lt->tm_mon + 1));
    strtime += numToString((unsigned char)(lt->tm_mday));
    if(slash)
    {
        strtime += slash;
    }
    strtime += numToString((unsigned char)(lt->tm_hour));
    strtime += numToString((unsigned char)(lt->tm_min));
    if(with_sec)
    {
        strtime += numToString((unsigned char)(lt->tm_sec));
    }
    return strtime;
}

#define TIMESTRING_TO_VALUE(ch1, ch2)     (((ch1) - 0x30) * 10 + (ch2) - 0x30)

inline unsigned int GetTime(string stime)
{
    struct tm lt;
    if(stime.length() < 17)
    {
        return -1;
    }
    
    lt.tm_year = TIMESTRING_TO_VALUE(stime[0], stime[1]) + 100;
    lt.tm_mon =  TIMESTRING_TO_VALUE(stime[3], stime[4]) - 1;
    lt.tm_mday = TIMESTRING_TO_VALUE(stime[6], stime[7]);
    lt.tm_hour = TIMESTRING_TO_VALUE(stime[9], stime[10]);
    lt.tm_min = TIMESTRING_TO_VALUE(stime[12], stime[13]);
    lt.tm_sec = TIMESTRING_TO_VALUE(stime[15], stime[16]);
    return (unsigned int)mktime(&lt);
}

class CommonThreadLock
{
public:
    CommonThreadLock(pthread_mutex_t *mutex)
    {
        m_mutex = mutex;
        pthread_mutex_lock(m_mutex);
    }
    
    ~CommonThreadLock()
    {
        pthread_mutex_unlock(m_mutex);
    }
private:
    CommonThreadLock() {}
    pthread_mutex_t *m_mutex;
};

#define MINUTE_ALINE(t)               ((t) / 60 * 60)
#define HOUR_ALINE(t)                 ((t) / 3600 * 3600)

#endif
