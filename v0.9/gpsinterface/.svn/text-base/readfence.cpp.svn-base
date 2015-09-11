#include "gpsinterface.h"
#include "Utility.h"
#include "LogUtility.h"
#include "ElectronicFenceTable.h"

unsigned char BCDToDec(unsigned char bcd_number)
{
	return((bcd_number>>4)*10 + (bcd_number&0x0f));
}

static int getCircle(int eid,struct ElectronicFenceData *pdata)
{
    VDRCircleFenceTable *table = new VDRCircleFenceTable();
    if(table->Refresh(eid) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getCircle eid query failed(%d)", eid);
        delete table;
        return -1;
    }
    
    int attribute;
    int latitude;
    int longitude;
    vector<unsigned char>  starttime;
    vector<unsigned char>  stoptime;
    int radius;
    table->GetIntValue(VDR_CIRCLE_REGION_ATTRIBUTE, attribute);
    table->GetIntValue(VDR_CIRCLE_CENTER_LATITUDE, latitude);
    table->GetIntValue(VDR_CIRCLE_CENTER_LONGITUDE, longitude);
    table->GetSequenceValue(VDR_CIRCLE_BEGIN_TIME, starttime);
    table->GetSequenceValue(VDR_CIRCLE_END_TIME, stoptime);
    table->GetIntValue(VDR_CIRCLE_RADIUS, radius);
    delete table;
    
    pdata->eid = eid;
    pdata->radius = radius;
    pdata->point_count = 1;
    pdata->tm_start.clear();
    pdata->tm_stop.clear();
    if(attribute & 0x01)
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(BCDToDec(starttime[i]));      
        for(int i = 0;i < (int)stoptime.size();i++)
            pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
    }
    else
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(23);
        pdata->tm_stop.push_back(59);
        pdata->tm_stop.push_back(59);
    }
    pdata->points.clear();
    pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
    return 0;
}

static int getRectangle(int eid,struct ElectronicFenceData *pdata)
{
    VDRRectangleFenceTable *table = new VDRRectangleFenceTable();
    if(table->Refresh(eid) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getRectangle eid query failed(%d)", eid);
        delete table;
        return -1;
    }
    
    int attribute;
    int latitude;
    int longitude;
    int latitude2;
    int longitude2;
    vector<unsigned char>  starttime;
    vector<unsigned char>  stoptime;
    table->GetIntValue(VDR_RECT_REGION_ATTRIBUTE, attribute);
    table->GetIntValue(VDR_RECT_UPLEFT_LATITUDE, latitude);
    table->GetIntValue(VDR_RECT_UPLEFT_LONGITUDE, longitude);
    table->GetIntValue(VDR_RECT_LOWRIGTH_LATITUDE, latitude2);
    table->GetIntValue(VDR_RECT_LOWRIGHT_LONGITUDE, longitude2);
    table->GetSequenceValue(VDR_RECT_BEGIN_TIME, starttime);
    table->GetSequenceValue(VDR_RECT_END_TIME, stoptime);
    delete table;

    pdata->eid = eid;
    pdata->point_count = 2;
    pdata->tm_start.clear();
    pdata->tm_stop.clear();
    if(attribute & 0x01)
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(BCDToDec(starttime[i]));
        for(int i = 0;i < (int)stoptime.size();i++)
            pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
    }
    else
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(23);
        pdata->tm_stop.push_back(59);
        pdata->tm_stop.push_back(59);
    }
    pdata->points.clear();
    pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
    pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude2), POSITION_INT_TO_FLOAT(longitude2), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
    return 0;
}

static int getPolygon(int eid,struct ElectronicFenceData *pdata)
{
    VDRPolygonFenceTable *table = new VDRPolygonFenceTable();
    if(table->Refresh(eid) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getPolygon eid query failed(%d)", eid);
        delete table;
        return -1;
    }
    
    int attribute;
    vector<unsigned char> latlong;
    vector<unsigned char>  starttime;
    vector<unsigned char>  stoptime;
    int count;
    table->GetIntValue(VDR_POL_REGION_ATTRIBUTE, attribute);
    table->GetIntValue(VDR_POL_POINT_COUNT, count);
    table->GetSequenceValue(VDR_POL_POINT_LATLONG, latlong);
    table->GetSequenceValue(VDR_POL_BEGIN_TIME, starttime);
    table->GetSequenceValue(VDR_POL_END_TIME, stoptime);
    delete table;
    
    if((int)latlong.size() < count * 8)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getPolygon latlong invalid(%d, %d)", count, latlong.size());
        return -1;
    }

    pdata->eid = eid;
    pdata->point_count = count;
    pdata->tm_start.clear();
    pdata->tm_stop.clear();
    if(attribute & 0x01)
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(BCDToDec(starttime[i]));
        for(int i = 0;i < (int)stoptime.size();i++)
            pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
    }
    else
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(23);
        pdata->tm_stop.push_back(59);
        pdata->tm_stop.push_back(59);
    }
    pdata->points.clear();
    for(int i = 0; i < count; i ++)
    {
        int latitude = GetLongFromVector(latlong, i * 8);
        int longitude = GetLongFromVector(latlong, i * 8 + 4);
        pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
    }
    return 0;
}

static int getLine(int eid,struct ElectronicFenceData *pdata)
{
    VDRLineFenceTable *table = new VDRLineFenceTable();
    if(table->Refresh(eid) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getLine eid query failed(%d)", eid);
        delete table;
        return -1;
    }
    
    vector<unsigned char> latlong;
    vector<unsigned char>  starttime;
    vector<unsigned char>  stoptime;
    int count;
    int m_attribute;
    table->GetIntValue(VDR_LINE_INFLEXION_COUNT, count);
    table->GetIntValue(VDR_LINE_REGION_ATTRIBUTE,m_attribute);
    table->GetSequenceValue(VDR_LINE_INFLEXION_INFO, latlong);
    table->GetSequenceValue(VDR_LINE_BEGIN_TIME, starttime);
    table->GetSequenceValue(VDR_LINE_END_TIME, stoptime);
    delete table;

    pdata->eid = eid;
    pdata->point_count = count;
    pdata->tm_start.clear();
    pdata->tm_stop.clear();
    if(m_attribute & 0x01)
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(BCDToDec(starttime[i]));
        for(int i = 0;i < (int)stoptime.size();i++)
            pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
    }
    else
    {
        for(int i = 0;i < (int)starttime.size();i++)
            pdata->tm_start.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(0);
        pdata->tm_stop.push_back(23);
        pdata->tm_stop.push_back(59);
        pdata->tm_stop.push_back(59);
    }
    pdata->points.clear();
    int index = 0;
    for(int i = 0; i < count; i ++)
    {
        if((latlong.size() - index) < 18)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "getLine latlong invalid(%d, %d)", index, latlong.size());
            return -1;
        }
        int latitude = GetLongFromVector(latlong, index + 8);
        int longitude = GetLongFromVector(latlong, index + 12);
        int width = latlong[index + 16];
        int attribute = latlong[index + 17];
        pdata->radius = width;
        pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
        index += 18;
        if(attribute & 0x01)
        {
            index += 4;
        }
        if(attribute & 0x02)
        {
            index += 3;
        }
    }
    return 0;
}

int getElectronicFenceByType(EnumFenceType type,int eid,struct ElectronicFenceData *pdata)
{
    if(pdata == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getElectronicFenceByType pdata is NULL");
        return -1;
    }
    
    switch(type)
    {
        case FENCE_TYPE_CIRCLE:
            return getCircle(eid, pdata);
        case FENCE_TYPE_RECTANGLE:
            return getRectangle(eid, pdata);
        case FENCE_TYPE_POLYGON:
            return getPolygon(eid, pdata);
        case FENCE_TYPE_PATH:
            return getLine(eid, pdata);
        default:
            LogUtility::Log(LOG_LEVEL_ERROR, "getElectronicFenceByType invalid type(%d)", (int)type);
            return -1;
    }
}

static int getAllCircle(vector<struct ElectronicFenceData> &data)
{
    VDRCircleFenceTable *table = new VDRCircleFenceTable();
    if(table->RefreshAll(VDR_CIRCLE_REGION_ID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getAllCircle RefreshAll query failed");
        delete table;
        return -1;
    }
    
    int totalnum = table->GetResultsCount();
    for(int i = 0; i < totalnum; i ++)
    {
        int attribute;
        int latitude;
        int longitude;
        vector<unsigned char>  starttime;
        vector<unsigned char>  stoptime;
        int radius;
        int eid;
        table->GetIntValue(i, VDR_CIRCLE_REGION_ID, eid);
        table->GetIntValue(i, VDR_CIRCLE_REGION_ATTRIBUTE, attribute);
        table->GetIntValue(i, VDR_CIRCLE_CENTER_LATITUDE, latitude);
        table->GetIntValue(i, VDR_CIRCLE_CENTER_LONGITUDE, longitude);
        table->GetSequenceValue(i, VDR_CIRCLE_BEGIN_TIME, starttime);
        table->GetSequenceValue(i, VDR_CIRCLE_END_TIME, stoptime);
        table->GetIntValue(i, VDR_CIRCLE_RADIUS, radius);
        
        StruElectronicFenceData newdata;
        StruElectronicFenceData *pdata = &newdata;
        pdata->eid = eid;
        pdata->radius = radius;
        pdata->point_count = 1;
        pdata->tm_start.clear();
        pdata->tm_stop.clear();
        if(attribute & 0x01)
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(BCDToDec(starttime[i]));      
            for(int i = 0;i < (int)stoptime.size();i++)
                pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
        }
        else
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(23);
            pdata->tm_stop.push_back(59);
            pdata->tm_stop.push_back(59);
        }
        pdata->points.clear();
        pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
        data.push_back(newdata);
    }
    delete table;
    return 0;
}

static int getAllRectangle(vector<struct ElectronicFenceData> &data)
{
    VDRRectangleFenceTable *table = new VDRRectangleFenceTable();
    if(table->RefreshAll(VDR_RECT_REGION_ID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getAllRectangle RefreshAll query failed");
        delete table;
        return -1;
    }

    int totalnum = table->GetResultsCount();
    for(int i = 0; i < totalnum; i ++)
    {
        int attribute;
        int latitude;
        int longitude;
        int latitude2;
        int longitude2;
        vector<unsigned char>  starttime;
        vector<unsigned char>  stoptime;
        int eid;
        table->GetIntValue(i, VDR_RECT_REGION_ID, eid);
        table->GetIntValue(i, VDR_RECT_REGION_ATTRIBUTE, attribute);
        table->GetIntValue(i, VDR_RECT_UPLEFT_LATITUDE, latitude);
        table->GetIntValue(i, VDR_RECT_UPLEFT_LONGITUDE, longitude);
        table->GetIntValue(i, VDR_RECT_LOWRIGTH_LATITUDE, latitude2);
        table->GetIntValue(i, VDR_RECT_LOWRIGHT_LONGITUDE, longitude2);
        table->GetSequenceValue(i, VDR_RECT_BEGIN_TIME, starttime);
        table->GetSequenceValue(i, VDR_RECT_END_TIME, stoptime);
        
        StruElectronicFenceData newdata;
        StruElectronicFenceData *pdata = &newdata;
        pdata->eid = eid;
        pdata->point_count = 2;
        pdata->tm_start.clear();
        pdata->tm_stop.clear();
        if(attribute & 0x01)
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(BCDToDec(starttime[i]));      
            for(int i = 0;i < (int)stoptime.size();i++)
                pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
        }
        else
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(23);
            pdata->tm_stop.push_back(59);
            pdata->tm_stop.push_back(59);
        }
        pdata->points.clear();
        pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
        pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude2), POSITION_INT_TO_FLOAT(longitude2), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
        data.push_back(newdata);
    }
    delete table;
    return 0;
}

static int getAllPolygon(vector<struct ElectronicFenceData> &data)
{
    VDRPolygonFenceTable *table = new VDRPolygonFenceTable();
    if(table->RefreshAll(VDR_POL_REGION_ID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getPolygon RefreshAll query failed");
        delete table;
        return -1;
    }

    int totalnum = table->GetResultsCount();
    for(int i = 0; i < totalnum; i ++)
    {
        int attribute;
        vector<unsigned char> latlong;
        vector<unsigned char>  starttime;
        vector<unsigned char>  stoptime;
        int count;
        int eid;
        table->GetIntValue(i, VDR_POL_REGION_ID, eid);
        table->GetIntValue(i, VDR_POL_REGION_ATTRIBUTE, attribute);
        table->GetIntValue(i, VDR_POL_POINT_COUNT, count);
        table->GetSequenceValue(i, VDR_POL_POINT_LATLONG, latlong);
        table->GetSequenceValue(i, VDR_POL_BEGIN_TIME, starttime);
        table->GetSequenceValue(i, VDR_POL_END_TIME, stoptime);
        
        if((int)latlong.size() < count * 8)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "getPolygon latlong invalid(%d, %d)", count, latlong.size());
            delete table;
            return -1;
        }

        StruElectronicFenceData newdata;
        StruElectronicFenceData *pdata = &newdata;
        pdata->eid = eid;
        pdata->point_count = count;
        pdata->tm_start.clear();
        pdata->tm_stop.clear();
        if(attribute & 0x01)
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(BCDToDec(starttime[i]));      
            for(int i = 0;i < (int)stoptime.size();i++)
                pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
        }
        else
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(23);
            pdata->tm_stop.push_back(59);
            pdata->tm_stop.push_back(59);
        }
        pdata->points.clear();
        for(int i = 0; i < count; i ++)
        {
            int latitude = GetLongFromVector(latlong, i * 8);
            int longitude = GetLongFromVector(latlong, i * 8 + 4);
            pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
        }
        data.push_back(newdata);
    }
    delete table;
    return 0;
}

static int getAllLine(vector<struct ElectronicFenceData> &data)
{
    VDRLineFenceTable *table = new VDRLineFenceTable();
    if(table->RefreshAll(VDR_LINE_REGION_ID) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "getLine eid query failed");
        delete table;
        return -1;
    }

    int totalnum = table->GetResultsCount();
    for(int i = 0; i < totalnum; i ++)
    {
        vector<unsigned char> latlong;
        vector<unsigned char>  starttime;
        vector<unsigned char>  stoptime;
        int count;
        int m_attribute;
        int eid;
        table->GetIntValue(i, VDR_LINE_REGION_ID, eid);
        table->GetIntValue(i, VDR_LINE_INFLEXION_COUNT, count);
        table->GetIntValue(i,VDR_LINE_REGION_ATTRIBUTE,m_attribute);
        table->GetSequenceValue(i, VDR_LINE_INFLEXION_INFO, latlong);
        table->GetSequenceValue(i, VDR_LINE_BEGIN_TIME, starttime);
        table->GetSequenceValue(i, VDR_LINE_END_TIME, stoptime);
        
        StruElectronicFenceData newdata;
        StruElectronicFenceData *pdata = &newdata;
        pdata->eid = eid;
        pdata->tm_start.clear();
        pdata->tm_stop.clear();
        if(m_attribute & 0x01)
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(BCDToDec(starttime[i]));      
            for(int i = 0;i < (int)stoptime.size();i++)
                pdata->tm_stop.push_back(BCDToDec(stoptime[i]));
        }
        else
        {
            for(int i = 0;i < (int)starttime.size();i++)
                pdata->tm_start.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(0);
            pdata->tm_stop.push_back(23);
            pdata->tm_stop.push_back(59);
            pdata->tm_stop.push_back(59);
        }
        pdata->point_count = count;
        pdata->points.clear();
        int index = 0;
        for(int j = 0; j < count; j ++)
        {
            if((latlong.size() - index) < 18)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "getLine latlong invalid(%d, %d)", index, latlong.size());
                delete table;
                return -1;
            }
            int latitude = GetLongFromVector(latlong, index + 8);
            int longitude = GetLongFromVector(latlong, index + 12);
            int width = latlong[index + 16];
            int attribute = latlong[index + 17];
            pdata->radius = width;
            pdata->points.push_back(Point(POSITION_INT_TO_FLOAT(latitude), POSITION_INT_TO_FLOAT(longitude), (attribute >> 6) & 0x01, (attribute >> 7) & 0x01));
            index += 18;
            if(attribute & 0x01)
            {
                index += 4;
            }
            if(attribute & 0x02)
            {
                index += 3;
            }
        }
        data.push_back(newdata);     
    }
    delete table;
    return 0;
}

int getAllElectronicFenceByType(EnumFenceType type, vector<struct ElectronicFenceData> &data)
{
    switch(type)
    {
        case FENCE_TYPE_CIRCLE:
            return getAllCircle(data);
        case FENCE_TYPE_RECTANGLE:
            return getAllRectangle(data);
        case FENCE_TYPE_POLYGON:
            return getAllPolygon(data);
        case FENCE_TYPE_PATH:
            return getAllLine(data);
        default:
            LogUtility::Log(LOG_LEVEL_ERROR, "getAllElectronicFenceByType invalid type(%d)", (int)type);
            return -1;
    }
}
