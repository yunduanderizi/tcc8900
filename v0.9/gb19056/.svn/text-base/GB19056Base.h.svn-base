#ifndef _GB19056_BASE_H_
#define _GB19056_BASE_H_

#include <vector>
#include <string>
#include "LogUtility.h"
#include "GB19056CallBackInterface.h"

using namespace std;

#define GB19056_QUERY_ERROR_FLAG                ((unsigned char)0xFA)
#define GB19056_SET_ERROR_FLAG                  ((unsigned char)0xFB)
#define GB19056_RESERVE_FLAG                    ((unsigned char)0x00)
#define GB19056_IN_HEAD_FLAG                    ((unsigned short)0xAA75)
#define GB19056_OUT_HEAD_FLAG                   ((unsigned short)0x557A)

template <class T>
class GB19056BufferTemplate
{
public:
    GB19056BufferTemplate(T &input, T &output) : m_in_buffer(input), m_out_buffer(output)
    {
        m_input_index = 0;
        m_output_index = 0;
    }
    
    void Clear()
    {
        m_out_buffer.clear();
        m_output_index = 0;
    }
    
    bool Check()
    {
        if(m_in_buffer.size() < 7)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056BufferTemplate::Check buffer len not enough.");
            return false;
        }
        unsigned short head = GetWord();
        if(head != GB19056_IN_HEAD_FLAG)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056BufferTemplate::Check buffer head invalid(%04x).", head);
            return false;
        }
        
        m_cmd = GetByte();
        unsigned short len = GetWord();
        GetByte();
        if(len != m_in_buffer.size() - m_input_index - 1)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056BufferTemplate::Check len not match(%d, %d, %d).", len, m_in_buffer.size(), m_input_index);
            return false;
        }
        
        unsigned char chsm = 0;
        for(int i = 0; i < (int)m_in_buffer.size() - 1; i ++)
        {
            chsm ^= m_in_buffer[i];
        }
        
        m_checksum = m_in_buffer[m_in_buffer.size() - 1];
        
        if(chsm == m_checksum)
        {
            return true;
        }
        else
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056BufferTemplate::Check checksum not match(%02x, %02x).", chsm, m_checksum);
            return false;
        }
    }
    
    void GenerateChecksum()
    {
        unsigned char chsm = 0;
        for(int i = 0; i < (int)m_out_buffer.size(); i ++)
        {
            chsm ^= m_out_buffer[i];
        }
        
        PushByte(chsm);
    }
    
    inline int GetLength()
    {
        return m_in_buffer.size() - m_input_index;
    }
    
    inline unsigned char GetByte()
    {
        unsigned char ch =  m_in_buffer[m_input_index];
        m_input_index ++;
        return ch;
    }

    inline unsigned short GetWord()
    {
        unsigned char ch1 =  GetByte();
        unsigned char ch2 =  GetByte();
        return (unsigned short)((ch1 << 8) | ch2);
    }

    inline unsigned int GetLong()
    {
        unsigned short ch1 =  GetWord();
        unsigned short ch2 =  GetWord();
        return (unsigned int)((ch1 << 16) | ch2);
    }
    
    inline void GetVector(vector<unsigned char> &array, int len)
    {
        for(int i = 0; i < len; i ++)
        {
            if(GetLength() > 0)
            {
                array.push_back(GetByte());
            }
            else
            {
                array.push_back(0x00);
            }
        }
    }

    inline void GetString(string &str_value, int len)
    {
        bool end = false;
        for(int i = 0; i < len; i ++)
        {
            if(GetLength() > 0)
            {
                unsigned ch = GetByte();
                if((ch != '\0') && (!end))
                {
                    str_value.push_back(ch);
                }
                else
                {
                    end = true;
                    continue;
                }
            }
            else
            {
                break;
            }
        }
    }

    inline void PushByte(unsigned char ch)
    {
        m_out_buffer.push_back(ch);
        m_output_index ++;
    }

    inline void PushWord(unsigned short ch)
    {
        PushByte((unsigned char)((ch >> 8) & 0xFF));
        PushByte((unsigned char)(ch & 0xFF));
    }

    inline void PushLong(unsigned int ch)
    {
        PushWord((unsigned short)((ch >> 16) & 0xFFFF));
        PushWord((unsigned short)(ch & 0xFFFF));
    }
    
    inline void PushString(string value, int len)
    {
        for(int i = 0; i < len; i ++)
        {
            if(i < (int)value.length())
            {
                PushByte(value[i]);
            }
            else
            {
                PushByte(0x00);
            }
        }
    }

    inline void PushVector(vector<unsigned char> value, int len)
    {
        for(int i = 0; i < len; i ++)
        {
            if(i < (int)value.size())
            {
                PushByte(value[i]);
            }
            else
            {
                PushByte(0x00);
            }
        }
    }
    
    inline void SetCmd(unsigned char cmd)
    {
        m_cmd = cmd;
    }
    
    inline unsigned char GetCmd()
    {
        return m_cmd;
    }
    
    inline void UpdateLength()
    {
        if(m_out_buffer.size() < 6)
        {
            return;
        }
        
        unsigned short len = m_out_buffer.size() - 6;
        
        m_out_buffer[3] = (unsigned char)((len >> 8) & 0xFF);
        m_out_buffer[4] = (unsigned char)(len & 0xFF);
    }
    
protected:
    GB19056BufferTemplate() {};
    T &m_in_buffer;
    T &m_out_buffer;
    int m_input_index;
    int m_output_index;
    unsigned char m_cmd;
    unsigned char m_checksum;
};

typedef GB19056BufferTemplate< vector<unsigned char> > GB19056Buffer;

typedef enum
{
    ENUM_GB19056_CMD_QUERY_VER = 0x00,
    ENUM_GB19056_CMD_QUERY_DRIVER = 0x01,
    ENUM_GB19056_CMD_QUERY_TIME = 0x02,
    ENUM_GB19056_CMD_QUERY_MILEAGE = 0x03,
    ENUM_GB19056_CMD_QUERY_PULSE_FACTOR = 0x04,
    ENUM_GB19056_CMD_QUERY_CAR_INFO = 0x05,
    ENUM_GB19056_CMD_QUERY_STATUS = 0x06,
    ENUM_GB19056_CMD_QUERY_ID = 0x07,
    ENUM_GB19056_CMD_QUERY_SPEED_RECORD = 0x08,
    ENUM_GB19056_CMD_QUERY_LOC_RECORD = 0x09,
    ENUM_GB19056_CMD_QUERY_ACCIDENT_RECORD = 0x10,
    ENUM_GB19056_CMD_QUERY_OVERTIME_RECORD = 0x11,
    ENUM_GB19056_CMD_QUERY_DRIVER_RECORD = 0x12,
    ENUM_GB19056_CMD_QUERY_POWER_RECORD = 0x13,
    ENUM_GB19056_CMD_QUERY_PARA_MODIFY_RECORD = 0x14,
    ENUM_GB19056_CMD_QUERY_SPEED_STATUS_RECORD = 0x15,
    ENUM_GB19056_CMD_QUERY_LAP_PULSE_FACTOR = 0x20,
    ENUM_GB19056_CMD_SET_CAR_INFO = 0x82,
    ENUM_GB19056_CMD_SET_DEPLOY_TIME = 0x83,
    ENUM_GB19056_CMD_SET_STATUS_CONFIG = 0x84,
    ENUM_GB19056_CMD_SET_TIME = 0xC2,
    ENUM_GB19056_CMD_SET_PULSE_RATE = 0xC3,
    ENUM_GB19056_CMD_SET_DEPLOY_MILEAGE = 0xC4,
    ENUM_GB19056_CMD_SET_STATUS_CONFIG_2 = 0xD0,
    ENUM_GB19056_CMD_SET_LAP_PULSE_FACTOR = 0xD1,
    ENUM_GB19056_CMD_CHECK_IN = 0xE0,
    ENUM_GB19056_CMD_CHECK_MILEAGE = 0xE1,
    ENUM_GB19056_CMD_CHECK_PULSE_FACTOR = 0xE2,
    ENUM_GB19056_CMD_CHECK_TIME = 0xE3,
    ENUM_GB19056_CMD_CHECK_OUT = 0xE4,

    ENUM_PC_VDR_CMD_QUERY_MODE_ID = 0x60,
    ENUM_PC_VDR_CMD_SET_SERVER_INFO = 0xD2,
    ENUM_PC_VDR_CMD_SET_DEPLOY_PARA = 0xD3,
    ENUM_PC_VDR_CMD_SET_AUTHTOKEN = 0xD4,
    ENUM_PC_VDR_CMD_SET_PULSE_FACTOR = 0xD5,
    ENUM_PC_VDR_CMD_SET_REBOOT = 0xD6
    
}ENUM_GB19056_CMD;

#define IS_CMD_QUERY(a)         ((a) <= 0x60)
#define IS_CMD_SET(a)           (((a) >= 0x82) && ((a) <= 0xCF))

#define GB19056_CHAR_SET        "GBK"

class GB19056Base
{
public:
    int ProcessRequest(GB19056Buffer &buffer);
    int ProcessRequest_JT808(GB19056Buffer &buffer);

    virtual int DumpData(string path, GB19056CallBackInterface *notify) = 0;

	int udisk_on;
	int set_udisk_stat(int value)
	{
		udisk_on = value;
		return 0;
	}

	int get_udisk_stat()
	{
		return udisk_on;
	}
	
protected:
    virtual int ProcessCmd(GB19056Buffer &buffer) = 0;
    virtual int ProcessCmd_JT808(GB19056Buffer &buffer) = 0;
    void GenerateError(GB19056Buffer &buffer)
    {
        buffer.Clear();
        PacketHead(buffer);
        if(IS_CMD_QUERY(buffer.GetCmd()))
        {
            GenerateQueryErrorResponse(buffer);
        }
        else
        {
            GenerateSetErrorResponse(buffer);
        }
    }
    void GenerateQueryErrorResponse(GB19056Buffer &buffer);
    void GenerateSetErrorResponse(GB19056Buffer &buffer);
    inline void PacketHead(GB19056Buffer &buffer)
    {
        buffer.PushWord(GB19056_OUT_HEAD_FLAG);
    }
    inline void PacketReserve(GB19056Buffer &buffer)
    {
        buffer.PushByte(GB19056_RESERVE_FLAG);
    }
};

#endif
