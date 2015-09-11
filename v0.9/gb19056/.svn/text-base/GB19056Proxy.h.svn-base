#ifndef _GB19056_PROXY_H_
#define _GB19056_PROXY_H_

#include <unistd.h>
#include "LogUtility.h"
#include "Utility.h"
#include <string>
#include "GB19056Base.h"
#include "GB19056_2012.h"
#include "GB19056CallBackInterface.h"

template<int Ver>
class GB19056Proxy
{
    enum
    {
        GB19056_VERSION = Ver
    };

public:

    ~GB19056Proxy()
    {
        if(p_gb19056)
        {
            delete p_gb19056;
        }
        p_gb19056 = NULL;
    }

    int ProcessRequest(vector<unsigned char> &input, vector<unsigned char> &output)
    {
        if(p_gb19056 == NULL)
        {
            return -1;
        }
        
        LogUtility::Log(LOG_LEVEL_DEBUG, "Input: %s", vectorToString(input).c_str());

        GB19056Buffer buffer(input, output);
        
        int ret = p_gb19056->ProcessRequest(buffer);
        
        LogUtility::Log(LOG_LEVEL_DEBUG, "Output: %s", vectorToString(output).c_str());
        return ret;
    }
    
    int ProcessRequest_JT808(vector<unsigned char> &input, vector<unsigned char> &output)
    {
        if(p_gb19056 == NULL)
        {
            return -1;
        }
        
        LogUtility::Log(LOG_LEVEL_DEBUG, "Input: %s", vectorToString(input).c_str());

        GB19056Buffer buffer(input, output);
        
        int ret = p_gb19056->ProcessRequest_JT808(buffer);
        
        LogUtility::Log(LOG_LEVEL_DEBUG, "Output: %s", vectorToString(output).c_str());
        return ret;
    }
    int DumpData(const string path, GB19056CallBackInterface *notify)
    {
        if(p_gb19056 == NULL)
        {
            return -1;
        }
        
        LogUtility::Log(LOG_LEVEL_DEBUG, "Path: %s", path.c_str());
        
        int ret = p_gb19056->DumpData(path, notify);

        return ret;
    }
    
    static GB19056Proxy<Ver> *Instance()
    {
        if(m_instance == NULL)
        {
            m_instance = new GB19056Proxy<Ver>();
        }
        
        return m_instance;
    }
    
    static void Destroy()
    {
        if(m_instance)
        {
            delete m_instance;
        }
    }

	int set_udisk_stat(int value)
	{
		if(p_gb19056 == NULL)
        {
            return -1;
        }
        
        //LogUtility::Log(LOG_LEVEL_DEBUG, "value: %d", value);
        
        int ret = p_gb19056->set_udisk_stat(value);

        return ret;
	}

	
	int get_udisk_stat()
	{
		if(p_gb19056 == NULL)
        {
            return -1;
        }
        
        int ret = p_gb19056->get_udisk_stat();
		//LogUtility::Log(LOG_LEVEL_DEBUG, "get_udisk_stat: %d", ret);

        return ret;
	}

protected:
    GB19056Proxy()
    {
        if(GB19056_VERSION == 2012)
        {
            p_gb19056 = new GB19056_2012();
        }
        else
        {
            p_gb19056 = NULL;
        }
    }
    
    static GB19056Proxy<Ver> *m_instance;
    GB19056Base *p_gb19056;
};

template <int Ver>
GB19056Proxy<Ver> *GB19056Proxy<Ver>::m_instance = NULL;

#endif
