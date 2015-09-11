#ifndef _GB19056_2012_H_
#define _GB19056_2012_H_

#include "GB19056Base.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "VDRTrackInfoTable.h"

#define GB19056_2012_MAX_BLOCK_LEN              1000

class GB19056_2012 : public GB19056Base
{
public:
    VDRLocStatus1Table *loc_table;
    virtual int DumpData(string path, GB19056CallBackInterface *notify);
protected:
    virtual int ProcessCmd(GB19056Buffer &buffer);
    virtual int ProcessCmd_JT808(GB19056Buffer &buffer);
private:
    int GetDriverAuthority();
    int ProcessVersionQuery(GB19056Buffer &buffer);
    int ProcessDriverQuery(GB19056Buffer &buffer);
    int ProcessTimeQuery(GB19056Buffer &buffer);
    int ProcessMileageQuery(GB19056Buffer &buffer);
    int ProcessPulseFactorQuery(GB19056Buffer &buffer);
    int ProcessLapPulseFactorQuery(GB19056Buffer &buffer);
    int ProcessCarInfoQuery(GB19056Buffer &buffer);
    int ProcessStatusQuery(GB19056Buffer &buffer);
    int ProcessIdQuery(GB19056Buffer &buffer);
    
    int ProcessCarInfoSet(GB19056Buffer &buffer);
    int ProcessDeployTimeSet(GB19056Buffer &buffer);
    int ProcessStatusConfigSet(GB19056Buffer &buffer);
    int ProcessStatusConfig2Set(GB19056Buffer &buffer);
    int ProcessTimeSet(GB19056Buffer &buffer);
    int ProcessPulseFactorSet(GB19056Buffer &buffer);
    int ProcessLapPulseFactorSet(GB19056Buffer &buffer);
    int ProcessDeployMileageSet(GB19056Buffer &buffer);
    
    int WriteEventLog(GB19056Buffer &buffer, int itime);
    int ProcessRecordQueryRequest(GB19056Buffer &buffer, int &start_time, int &stop_time, int &max_block_number);
    int ProcessDriveRecordQuery(GB19056Buffer &buffer);
    int ProcessLocRecordQuery(GB19056Buffer &buffer);
    int ProcessFileDriveRecordQuery(GB19056Buffer &buffer);
    int ProcessFileLocRecordQuery(GB19056Buffer &buffer);
    int ProcessAccidentRecordQuery(GB19056Buffer &buffer);
    int ProcessTiredRecordQuery(GB19056Buffer &buffer);
    int ProcessDriverInfoRecordQuery(GB19056Buffer &buffer);
    int ProcessPowerRecordQuery(GB19056Buffer &buffer);
    int ProcessParaSettingRecordQuery(GB19056Buffer &buffer);
    int ProcessSpeedInfoRecordQuery(GB19056Buffer &buffer);

    int ProcessFileDriveRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessFileLocRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessAccidentRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessTiredRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessDriverInfoRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessPowerRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessParaSettingRecordQuery_JT808(GB19056Buffer &buffer);
    int ProcessSpeedInfoRecordQuery_JT808(GB19056Buffer &buffer);

    int ProcessInCheck(GB19056Buffer &buffer);
    int ProcessMileageCheck(GB19056Buffer &buffer);
    int ProcessPulseFactorCheck(GB19056Buffer &buffer);
    int ProcessTimeCheck(GB19056Buffer &buffer);
    int ProcessOutCheck(GB19056Buffer &buffer);

    int ProcessQueryModeID(GB19056Buffer &buffer);
    int ProcessSetServerInfo(GB19056Buffer &buffer);
    int ProcessSetDeployPara(GB19056Buffer &buffer);
    int ProcessSetAuthorken(GB19056Buffer &buffer);
    int ProcessSetPulseFactor(GB19056Buffer &buffer);
    int ProcessSetReboot(GB19056Buffer &buffer);

    string GetDataName(unsigned char cmd);

    inline void DumpByte(ofstream &out, unsigned char ch, unsigned char &check_sum)
    {
        out.write((const char *)&ch, 1);
        check_sum ^= ch;
    }

    inline void DumpShort(ofstream &out, unsigned short word, unsigned char &check_sum)
    {
        DumpByte(out, (unsigned char)((word >> 8) & 0xFF), check_sum);
        DumpByte(out, (unsigned char)(word & 0xFF), check_sum);
    }

    inline void DumpLong(ofstream &out, unsigned int word, unsigned char &check_sum)
    {
        DumpShort(out, (unsigned short)((word >> 16) & 0xFFFF), check_sum);
        DumpShort(out, (unsigned short)(word & 0xFFFF), check_sum);
    }

    inline void DumpVector(ofstream &out, vector<unsigned char> &vec, unsigned char &check_sum)
    {
        for(int i = 0; i < (int)vec.size(); i ++)
        {
            unsigned char ch = vec[i];
            DumpByte(out, ch, check_sum);
        }
    }
    
    inline void DumpString(ofstream &out, string value, int len, unsigned char &check_sum)
    {
        for(int i = 0; i < len; i ++)
        {
            unsigned char ch = 0x00;
            if(i < (int)value.length())
            {
                ch = value[i];
            }
            DumpByte(out, ch, check_sum);
        }
    }
    inline void DumpZero(ofstream &outfile, unsigned char cmd, unsigned char &check_sum)
    {
        DumpByte(outfile, cmd, check_sum);
        string cmd_name = GetDataName(cmd);
        DumpString(outfile, cmd_name, 18, check_sum);
        unsigned int length = 0;
        DumpLong(outfile, length, check_sum);
    }

    int DumpFileSpeedRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpFileLocRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpSpeedRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpLocRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpAccidentRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpTiredRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpDriverRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpPowerRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpSettingRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    int DumpSpeedStatusRecord(string &filename, ofstream &outfile, unsigned char &check_sum);
    
    inline string GetTempRecFileName(string &filename)
    {
        return filename + ".TEMP.REC";
    }
    
    inline int PrepareTempRecFile(string &filename, ofstream &record_file)
    {
        string rec_filename = GetTempRecFileName(filename);
        record_file.open(rec_filename.c_str(), ios::out | ios::binary | ios::trunc);
        if(!record_file)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::PrepareTempRecFile fail to open %s to write.", rec_filename.c_str());
            return -1;
        }
        return 0;
    }

    inline int OpenTempRecFile(string &filename, ifstream &record_file)
    {
        string rec_filename = GetTempRecFileName(filename);
        record_file.open(rec_filename.c_str(), ios::in | ios::binary);
        if(!record_file)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::OpenTempRecFile fail to open %s to write.", rec_filename.c_str());
            return -1;
        }
        return 0;
    }
    inline void DeleteTempRecFile(string &filename)
    {
         string rec_filename = GetTempRecFileName(filename);
         unlink(rec_filename.c_str());
         return;
    }
    inline void DumpHeadAndRecord(ofstream &outfile, ifstream &infile, unsigned char cmd, unsigned char &check_sum)
    {
        DumpByte(outfile, cmd, check_sum);
        string cmd_name = GetDataName(cmd);
        DumpString(outfile, cmd_name, 18, check_sum);
        infile.seekg(0, ios::end);
        unsigned int length = (unsigned int)infile.tellg();
        DumpLong(outfile, length, check_sum);
        infile.seekg(0, ios::beg);
        outfile << infile.rdbuf();
        return;
    }
};

#endif
