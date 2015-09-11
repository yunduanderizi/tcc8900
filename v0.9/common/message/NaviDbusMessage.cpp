
#include "NaviDbusMessage.h"

// ======================================================================

CNaviAlertInfoNotifyC::CNaviAlertInfoNotifyC():DBusNotifyC(NOTF_ID_NAVI_ALERTINFO)
{
	// 全赋为0
	memset( &iDistAlert, 0, 52 + sizeof(int) );
}

CNaviAlertInfoNotifyC::~CNaviAlertInfoNotifyC()
{
}

void CNaviAlertInfoNotifyC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviAlertInfoNotifyC %d, %d, %d, %s\n\n", iDistAlert, (int)cTipType, (int)cBeAlert, cAlertTip );
}

// ======================================================================

CNaviCarLocationNotifyC::CNaviCarLocationNotifyC():DBusNotifyC(NOTF_ID_NAVI_CARLOCATION)
{
	// 全赋为0
	memset( &cCurType, 0, 154 );
}

CNaviCarLocationNotifyC::~CNaviCarLocationNotifyC()
{
}

void CNaviCarLocationNotifyC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviCarLocationNotifyC %d, %s, %s, %s, %d, %d, %d, %d \n\n", (int)cCurType, cRoadName, cAreaName,cityName,cPositioning,cMode,cSatellitenum,cBDSatellitenum);
}

// ======================================================================

void CNaviCarLocationRepC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviCarLocationRepC result=[%d] %d, %s, %s, %s\n\n", result, (int)cCurType, cRoadName, cAreaName, cityName );
}

// ======================================================================

void CNaviCarLocationReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviCarLocationReqC\n\n" );
}

// ======================================================================

void CNaviCommonRepC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviCommonRepC result=[%d]\n\n", result );
}

// ======================================================================

void CNaviDestinatonRecordReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviDestinatonRecordReqC\n\n" );
}

// ======================================================================

void CNaviDestinatonReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviDestinatonReqC\n\n" );
}
// ======================================================================

void CNaviAddressBookReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviAddressBookReqC\n\n" );
}
// ======================================================================

void CNaviSurroundingSearchReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviSurroundingSearchReqC\n\n" );
}
// ======================================================================

void CNaviRoadBook2ReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviRoadBook2ReqC\n\n" );
}
// ======================================================================

void CNaviGasStationReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviGasStationReqC\n\n" );
}

// ======================================================================

void CNaviGPSInfoRepC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviGPSInfoRepC result=[%d] %f, %f, %d, %d, %d, %d\n\n",
				result, fX, fY, iTime, iSysTime, iOrientation, iVelocity );
}

// ======================================================================

void CNaviGPSInfoReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviGPSInfoReqC\n\n" );
}

// ======================================================================

// 查询GPS信息（请求）
void CNaviGPSInfoReportCycleReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviGPSInfoReportCycleReqC %d\n\n", iReportCycle );
}

// ======================================================================

CNaviGPSInfoReportNotifyC::CNaviGPSInfoReportNotifyC():DBusNotifyC(NOTF_ID_NAVI_GPSINFO_REPORT)
{
}

CNaviGPSInfoReportNotifyC::~CNaviGPSInfoReportNotifyC()
{
}

// 车辆当前位置信息通知
void CNaviGPSInfoReportNotifyC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviGPSInfoReportNotifyC (%f, %f), (%d, %d), %d, %d, (%d, %d, %d), %d, %d, %d, %d \n\n",
				fX, fY, iTime, iSysTime, iOrientation, iVelocity,ilatitude,ilongitude,ialtitude,cPositioning,cMode,cSatellitenum,cBDSatellitenum);
}

// ======================================================================

void CNaviLocateCarReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviLocateCarReqC %d\n\n", iShowMode );
}

// ======================================================================

void CNaviLocateHomeReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviLocateHomeReqC %d\n\n", iShowMode );
}

// ======================================================================

CNaviNavigationInfoNotifyC::CNaviNavigationInfoNotifyC():DBusNotifyC(NOTF_ID_NAVI_INFO)
{
	// 全赋为0
	memset( &cStatus, 0, 52 + sizeof(int) * 2 );
}

CNaviNavigationInfoNotifyC::~CNaviNavigationInfoNotifyC()
{
}

void CNaviNavigationInfoNotifyC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviNavigationInfoNotifyC %d, %d, %s, %d, %d\n\n", (int)cStatus, (int)cTipType, cRoadName, iDistTurn, iDistEnd );
}

// ======================================================================

void CNaviNavigationReqC::print(FILE *fp)
{
        fprintf( fp, "\tCNaviNavigationReqC %f %f %d %s\n\n", fX, fY, iRuleSelect, cDistName );
}
// ======================================================================

void CNaviParkReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviParkReqC\n\n" );
}

// ======================================================================

void CNaviWindowDisplayControlReqC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviWindowDisplayControlReqC %d, %d, %d, %d, %d, %d, %d\n\n",
				(int)cShowMode, (int)cHideMode, (int)cOther, iX, iY, iWidth, iHeight );
}

// ======================================================================
//路书请求
void CNaviRoadBookReqC::print(FILE *fp)
{
	struct RoadBookItems *rtmp;
	
	rtmp= rBookItems;
	fprintf( fp, "\trBookTitle=%s\n", rBookTitle);
	fprintf( fp, "\trDescription=%s\n", rDescription);
	fprintf( fp, "\trItemNum=%d\n", rItemNum);
	while(rtmp)
	{
		fprintf(fp,"\tCDistName=%s\n",rtmp->cDistName);
		fprintf(fp,"\tfx=%f\n",rtmp->fX);
		fprintf(fp,"\tfy=%f\n\n",rtmp->fY);
		rtmp = rtmp->next;
	}
	
}
CNaviRoadBookReqC::CNaviRoadBookReqC():DBusRequestC(REQ_ID_NAVI_ROADBOOK)
{
	rItemNum=0;
	rBookItems=NULL;
}
CNaviRoadBookReqC::~CNaviRoadBookReqC()
{
	struct RoadBookItems  *item, *rtmp;

	rtmp= rBookItems;
	while(rtmp)
	{
		item= rtmp->next;
		delete rtmp;
		rtmp = item;
	}

	rBookItems = NULL;
}

void CNaviRoadBookReqC:: SetBookTitle(char * buffer)
{
	memset(rBookTitle, 0, 128);
	if(strlen(buffer) > 128)
		strncpy(rBookTitle, buffer, 128);
	else
		strcpy(rBookTitle, buffer);
}

void CNaviRoadBookReqC:: SetDescription(char * buffer)
{
	memset(rDescription, 0, 256);

	if(strlen(buffer) > 256)
		strncpy(rDescription, buffer, 256);
	else
		strcpy(rDescription, buffer);
}
 void  CNaviRoadBookReqC::AddBookItem(char *name, float fx, float fy)
 {
 	struct RoadBookItems  *item, *rtmp;
	
	item=new(struct RoadBookItems);
	memset(item->cDistName, 0, 50);
	item->fX = 0.0;
	item->fY = 0.0;
	
	if(strlen(name) > 50)
		strncpy(item->cDistName, name, 50);
	else
		strcpy(item->cDistName, name);
	item->fX = fx;
	item->fY= fy;
	item->next = NULL;

	rtmp = rBookItems;
	while(1)
	{
	    if(rtmp == NULL)
	    {
	        rBookItems = item;
		 rItemNum = 1;
	        break;
	    }
	    if(rtmp->next == NULL)
	    {
	        rtmp->next = item;
		 rItemNum++;
	        break;
	    }
	    rtmp = rtmp->next;
	}	
 }

 char * CNaviRoadBookReqC:: GetBookTitle()
{
	return rBookTitle;
}
char * CNaviRoadBookReqC:: GetDescription()
{
	return rDescription;
}
int  CNaviRoadBookReqC:: GetItemNum()
{
	return rItemNum;
}
struct RoadBookItems * CNaviRoadBookReqC::GetBookItems()
{
	return rBookItems;
}

 struct RoadBookItems * CNaviRoadBookReqC::operator[ ](int index)
{
 	struct RoadBookItems  *rtmp;
	int i=0;
	
	if(index >= rItemNum || index<0)
		return NULL;
	
	rtmp= rBookItems;
	while(i<index)
	{
		if(rtmp == NULL)
		{
		    break;
		}
		if(rtmp->next == NULL)
		{
		    break;
		}
		rtmp = rtmp->next;
		i++;
	}
	
	return rtmp;
}
int CNaviRoadBookReqC::encode(void *&buffer,int  &len,DBusMessageFreeFunc &freeFunc)
{
	struct RoadBookItems *item;
		
	buffer = malloc(384+sizeof(struct RoadBookItems)*rItemNum);	
	memset(buffer,0 , 384+sizeof(struct RoadBookItems)*rItemNum);

	int  index=0;

	strcpy((char *)buffer+index,rBookTitle);
	index+= strlen(rBookTitle)+1;
	strcpy((char *)buffer+index,rDescription);
	index+= strlen(rDescription)+1;

	item = rBookItems;
	while(item)
	{
		strcpy((char *)buffer+index,item->cDistName);
		index+= strlen(item->cDistName)+1;

		memcpy((char *)buffer+index,(void*)&(item->fX),sizeof(float));
		index +=sizeof(float);
		memcpy((unsigned char  *)buffer+index,(void*)&(item->fY),sizeof(float));
		index +=sizeof(float);
		
		item = item->next;
	}
	
	len =index;
	freeFunc=free;
	return 0;
}

int CNaviRoadBookReqC::decode(void *buffer,int len)
{
	struct RoadBookItems  *item, *rtmp;
       int  index=0;
	   
	strcpy(rBookTitle,(char  *)buffer+index);
	index +=strlen(rBookTitle)+1;

	strcpy(rDescription,(char  *)buffer+index);
	index +=strlen(rDescription)+1;

	rtmp = rBookItems;
       while(index<len)
       	{
		item=new(struct RoadBookItems);
		memset(item->cDistName, 0, 50);
		item->fX = 0.0;
		item->fY = 0.0;

		strcpy(item->cDistName,(char  *)buffer+index);
		index +=strlen(item->cDistName)+1;

		memcpy((void*)&(item->fX),(unsigned char *)buffer+index,sizeof(float));
		index +=sizeof(float);
		memcpy((void*)&(item->fY),(unsigned char *)buffer+index,sizeof(float));
		index +=sizeof(float);
		item->next =NULL;
		
		rtmp = rBookItems;
		while(1)
		{
			if(rtmp == NULL)
			{
				rBookItems = item;
				rItemNum = 1;
				break;
			}
			if(rtmp->next == NULL)
			{
				rtmp->next = item;
				rItemNum ++;
				break;
			}
			rtmp = rtmp->next;
		}	

       	}
	return 0;
}

// ======================================================================
//查找附近医院请求 
void CNaviHospitalReqC::print(FILE *fp)
{
        fprintf( fp, "\tCNaviHospitalReqC \n\n" );
}


// ======================================================================
//实时路况请求 
void CNaviRealTimeRoadReqC::print(FILE *fp)
{
        fprintf( fp, "\tCNaviRealTimeRoadReqC \n\n" );
}

// ======================================================================
//路况早报请求
void CNaviRoadReportReqC::print(FILE *fp)
{
        fprintf( fp, "\t CNaviRoadReportReqC \n" );
	fprintf( fp, "\t src_fx=%f src_fy=%f\n", src_fX, src_fY);
	fprintf( fp, "\t dst_fx=%f dst_fy=%f \n", dst_fX, dst_fY);
	fprintf( fp, "\t starttime=%d, endtime=%d\n", starttime, endtime);
	fprintf( fp, "\t interval=%d, flag=%d\n\n", interval, flag);
}
// ======================================================================
//通用请求
void CNaviCommonReqC::print(FILE *fp)
{
        fprintf( fp, "\t CNaviCommonReqC reqid=%d \n\n", reqid );
}
// ======================================================================
//变更白天黑夜模式请求
void CNavDayNightModeReqC::print(FILE *fp)
{
        fprintf( fp, "\t CNavDayNightModeReqC dayNightMode=%d \n\n", dayNightMode );
}
// ======================================================================
//白天黑夜模式查询请求
void CGuiDayNightModeReqC::print(FILE *fp)
{
        fprintf( fp, "\t CGuiDayNightModeReqC \n\n" );
}
// ======================================================================

// ======================================================================
//右旋钮旋转消息请求
void CNavRightButtonRotateReqC::print(FILE *fp)
{
        fprintf( fp, "\t CNavRightButtonRotateReqC \n\n" );
}
// ======================================================================

void CNaviMultipointReqC::print(FILE *fp)
{
	struct MultipointItems *rtmp;
	
	rtmp= rPointItems;
    fprintf( fp, "\trPointTitle=%s\n", rTitle);
	fprintf( fp, "\trDescription=%s\n", rDescription);
	fprintf( fp, "\trItemNum=%d\n", rItemNum);
	while(rtmp)
	{
		fprintf(fp,"\tCDistName=%s\n",rtmp->cDistName);
		fprintf(fp,"\tfx=%f\n",rtmp->fX);
		fprintf(fp,"\tfy=%f\n\n",rtmp->fY);
		rtmp = rtmp->next;
	}
	
}
CNaviMultipointReqC::CNaviMultipointReqC():DBusRequestC(REQ_ID_NAVI_MULTIPOINTGPSINFO)
{
	rItemNum=0;
	rPointItems=NULL;
}
CNaviMultipointReqC::~CNaviMultipointReqC()
{
	struct MultipointItems  *item, *rtmp;

	rtmp= rPointItems;
	while(rtmp)
	{
		item= rtmp->next;
		delete rtmp;
		rtmp = item;
	}

	rPointItems = NULL;
}


void  CNaviMultipointReqC::AddPointItem(char *name, float fx, float fy)
 {
 	struct MultipointItems  *item, *rtmp;
	
	item=new(struct MultipointItems);
	memset(item->cDistName, 0, 50);
	item->fX = 0.0;
	item->fY = 0.0;
	
	if(strlen(name) > 50)
		strncpy(item->cDistName, name, 50);
	else
		strcpy(item->cDistName, name);
	item->fX = fx;
	item->fY= fy;
	item->next = NULL;

	rtmp = rPointItems;
	while(1)
	{
	    if(rtmp == NULL)
	    {
	        rPointItems = item;
		 rItemNum = 1;
	        break;
	    }
	    if(rtmp->next == NULL)
	    {
	        rtmp->next = item;
		 rItemNum++;
	        break;
	    }
	    rtmp = rtmp->next;
	}	
 }

 
int  CNaviMultipointReqC:: GetItemNum()
{
	return rItemNum;
}

void CNaviMultipointReqC:: SetTitle(char * buffer)
{
	memset(rTitle, 0, 128);
	if(strlen(buffer) > 128)
		strncpy(rTitle, buffer, 128);
	else
		strcpy(rTitle, buffer);
}

void CNaviMultipointReqC:: SetDescription(char * buffer)
{
	memset(rDescription, 0, 256);

	if(strlen(buffer) > 256)
		strncpy(rDescription, buffer, 256);
	else
		strcpy(rDescription, buffer);
}

 char * CNaviMultipointReqC:: GetTitle()
{
	return rTitle;
}
char * CNaviMultipointReqC:: GetDescription()
{
	return rDescription;
}

struct MultipointItems * CNaviMultipointReqC::operator[ ](int index)
{
 	struct MultipointItems  *rtmp;
	int i=0;
	
	if(index >= rItemNum || index<0)
		return NULL;
	
	rtmp= rPointItems;
	while(i<index)
	{
		if(rtmp == NULL)
		{
		    break;
		}
		if(rtmp->next == NULL)
		{
		    break;
		}
		rtmp = rtmp->next;
		i++;
	}
	
	return rtmp;
}
struct MultipointItems * CNaviMultipointReqC::GetPointItems()
{
	return rPointItems;
}

int CNaviMultipointReqC::encode(void *&buffer,int  &len,DBusMessageFreeFunc &freeFunc)
{
	struct MultipointItems *item;
		
	buffer = malloc(384+sizeof(struct MultipointItems)*rItemNum);	
	memset(buffer,0 , 384+sizeof(struct MultipointItems)*rItemNum);

	int  index=0;

	strcpy((char *)buffer+index,rTitle);
	index+= strlen(rTitle)+1;
	strcpy((char *)buffer+index,rDescription);
	index+= strlen(rDescription)+1;

	item = rPointItems;
	while(item)
	{
		strcpy((char *)buffer+index,item->cDistName);
		index+= strlen(item->cDistName)+1;

		memcpy((char *)buffer+index,(void*)&(item->fX),sizeof(float));
		index +=sizeof(float);
		memcpy((unsigned char  *)buffer+index,(void*)&(item->fY),sizeof(float));
		index +=sizeof(float);
		
		item = item->next;
	}
	
	len =index;
	freeFunc=free;
	return 0;
}

int CNaviMultipointReqC::decode(void *buffer,int len)
{
	struct MultipointItems  *item, *rtmp;
       int  index=0;
	   
	strcpy(rTitle,(char  *)buffer+index);
	index +=strlen(rTitle)+1;

	strcpy(rDescription,(char  *)buffer+index);
	index +=strlen(rDescription)+1;

	rtmp = rPointItems;
       while(index<len)
       	{
		item=new(struct MultipointItems);
		memset(item->cDistName, 0, 50);
		item->fX = 0.0;
		item->fY = 0.0;

		strcpy(item->cDistName,(char  *)buffer+index);
		index +=strlen(item->cDistName)+1;

		memcpy((void*)&(item->fX),(unsigned char *)buffer+index,sizeof(float));
		index +=sizeof(float);
		memcpy((void*)&(item->fY),(unsigned char *)buffer+index,sizeof(float));
		index +=sizeof(float);
		item->next =NULL;
		
		rtmp = rPointItems;
		while(1)
		{
			if(rtmp == NULL)
			{
				rPointItems = item;
				rItemNum = 1;
				break;
			}
			if(rtmp->next == NULL)
			{
				rtmp->next = item;
				rItemNum ++;
				break;
			}
			rtmp = rtmp->next;
		}	

       	}
	return 0;
}

//显示gps菜单 请求（请求）gui to navi
void CNaviShowMenuC::print(FILE *fp)
{
        fprintf( fp, "\t CNaviShowMenuC %d\n\n",m_mode);
}

CNaviMapMenuNotifyC::CNaviMapMenuNotifyC():DBusNotifyC(NOTF_ID_NAVI_MAP_MENU)
{

}

CNaviMapMenuNotifyC::~CNaviMapMenuNotifyC()
{

}

void CNaviMapMenuNotifyC::print(FILE *fp)
{
	fprintf( fp, "\t CNaviMapMenuNotifyC %d",m_mode);
}

CNaviElectronicFenceNotifyC::CNaviElectronicFenceNotifyC():DBusNotifyC(NOTF_ID_NAVI_ELECTRONIC_FENCE)
{

}
CNaviElectronicFenceNotifyC::~CNaviElectronicFenceNotifyC()
{
}

void CNaviElectronicFenceNotifyC::print(FILE *fp)
{
	fprintf( fp, "\tCNaviElectronicFenceNotifyC %d, %d, %d, %d\n\n",eflag, eid, etype, estate);
}

CNaviElectronicFenceStateNotifyC::CNaviElectronicFenceStateNotifyC():DBusNotifyC(NOTF_ID_NAVI_ELECTRONIC_FENCE_STATE)
{

}
CNaviElectronicFenceStateNotifyC::~CNaviElectronicFenceStateNotifyC()
{
}

void CNaviElectronicFenceStateNotifyC::print(FILE *fp)
{
        	fprintf( fp, "\tCNaviElectronicFenceStateNotifyC %d %d, %d, %d, %d ,%d\n\n",iTime,etype, eid, estate,epoint,edeviation);
}

//显示卫星定位页面请求（请求）gui to navi
void CNaviSatelliteUIC::print(FILE *fp)
{
        fprintf( fp, "\t CNaviSatelliteUIC %d\n\n",m_mode);
}