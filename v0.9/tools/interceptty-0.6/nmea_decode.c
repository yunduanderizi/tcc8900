/***
** @file: nmea_decode.c
** nmea_decode实现了对NMEA消息的解码
*/

//
#include <stdio.h>
#include "nmea_type.h"

#define A02TOI(x) ((*x - 48)*10 + (*(x+1) - 48))
#define A03TOI(x) ((*x - 48)*100 + (*(x+1) - 48)*10 + (*(x+2) - 48))
// global definition
static char nRxRunMode = 3; // 接收机的工作模式，2=单BD模式，1=单GPS模式，3 = BD+GPS混合模式

void ClearSvData(tSVData *psv)
{
	int i;
	psv->nSvInView = 0;
	psv->nSvsUsed = 0;
	for (i=0;i<NMEA_CHANS;i++)
	{
		psv->aucUsedSvid[i] = 0;
		psv->aSvInView[i].svid = 0;
		psv->aSvInView[i].cno = 0;
		psv->aSvInView[i].elv = 0;
		psv->aSvInView[i].amz = 0;
	}
}
void ClearPartOfNmeaData(tNmea_Msg *pNmea)
{
	pNmea->nMsgMask = 0;
	pNmea->cFixFlag = 0;
	pNmea->cFixQaulityIdx = 0;
	pNmea->cLatIdx = 0;
	pNmea->cLonIdx = 0;
	pNmea->ucFixModeIndex = 0;
	ClearSvData(&pNmea->aSvInfo[0]);
	ClearSvData(&pNmea->aSvInfo[1]);
}
//
/*
 * @brief: 
 * return: 2: BDS MODE; 1: GPS MODE 3:BDS+GPS MODE
*/
unsigned char NmeaGetRxRunMode(void)
{
	return (nRxRunMode>=1&&nRxRunMode<=3)?nRxRunMode:3;
}
//
unsigned int NmeaDecode(const char* pStr,tNmea_Msg *pNmea)
{
	//int len;
	unsigned int errCode;
	if (pStr == NULL)
		return ERR_INTERFAE;

	//len = strlenx(pStr);
	if (pStr[0] != '$')
	{
		errCode |= ERR_BEGIN_CHAR;
		return errCode;
	}
	
	if (pStr[3] == 'G' && pStr[4] == 'G' && pStr[5] == 'A' )
	{
		ClearPartOfNmeaData(pNmea);
		pNmea->nMsgMask |= GGA_DECODE;
		errCode |= DecodeGga(pStr,pNmea);
	}
	else if (pStr[3] == 'R' && pStr[4] == 'M' && pStr[5] == 'C' )
	{
		pNmea->nMsgMask |= RMC_DECODE;
		errCode |= DecodeRmc(pStr,pNmea);
	}
#ifdef DECODE_GSV
	else if (pStr[3] == 'G' && pStr[4] == 'S' && pStr[5] == 'V')
	{
		pNmea->nMsgMask |= GSV_DECODE;
		errCode |= DecodeGsv(pStr,pNmea);
	}
#endif
#ifdef DECODE_GSA
	else if (pStr[3] == 'G' && pStr[4] == 'S' && pStr[5] == 'A')
	{
		pNmea->nMsgMask |= GSA_DECODE;
		errCode |= DecodeGsa(pStr,pNmea);
	}
#endif
	else if (pStr[3] == 'A' && pStr[4] == 'N' && pStr[5] == 'T')
	{
		pNmea->nMsgMask |= ANT_DECODE;
		errCode |= DecodeAntennaChk(pStr,&pNmea->nAnttenaStatus);
	}
    else if (pStr[3] == 'G' && pStr[4] == 'L' && pStr[5] == 'L')
    {
        pNmea->nMsgMask |= GLL_DECODE;
    }
    else if (pStr[3] == 'V' && pStr[4] == 'T' && pStr[5] == 'G')
    {
        pNmea->nMsgMask |= VTG_DECODE;
    }
	else
	{
		errCode |= ERR_MSG_HEADER;
		return errCode;	
	}

	return errCode;
}

unsigned int IntegrityAndCorrectCheck(const char *pStr)
{
	int i;
	char *pos;
	unsigned int errCode = 0;

	// Integrity and correctness check
	unsigned char ucChkSum = CheckSum(pStr,&i);

	if (*(pStr+i) != '*')
		errCode |= ERR_MSG_INCOMPLETE;
	else
	{
		if (ucChkSum != (unsigned char)str2lx(pStr+i+1,&pos,16))
		{
			errCode |= ERR_CHKSUM;
		}
	}

	return errCode;
}

/*
$GPGGA,<UTC hhmmss.sss>,<纬度>,<纬度半球>,<经度>,<经度半球>,<定位质量指示>,<使用卫星数量>,
       <水平精确度>,<海拔高度>,<高度单位>,<大地水准面高度>,<高度单位>,<差分GPS数据期限>,
	   <差分参考基站标号>*<校验和><CR><LF>
*/
//$GNGGA,100533.000,4000.0010,N,11600.0026,E,1,16,0.777,53.850,M,0.0,M,,*4A
unsigned int DecodeGga(const char* pStr,tNmea_Msg *pNmea)
{
	char *pos;
	unsigned int errCode = 0;
	double temp;
	// Integrity and correctness check
	if (errCode = IntegrityAndCorrectCheck(pStr))
	{
		return errCode;
	}

	if (pStr[1] == 'G' && pStr[2] == 'N')
	{
		nRxRunMode = 3;
	}
	else if(pStr[1] == 'B' && pStr[2] == 'D')
	{
		nRxRunMode = 2;
	}
	else if(pStr[1] == 'G' && pStr[2] == 'P')
	{
		nRxRunMode = 1;
	}

	pNmea->nMsgMask |= GGA_DECODE;

	pStr += 7;
	// get utc time
	temp = str2dx(pStr,&pos);
	pNmea->usHours = (unsigned short)(temp/10000);
	temp -= pNmea->usHours*10000;
	pNmea->usMinutes = (unsigned short)(temp/100);
	pNmea->fSeconds = (float)(temp - pNmea->usMinutes*100);

	// get latitude and format to degree
	pStr = pos+1;
	if (*pStr == ',')
	{
		pNmea->fLatitude = 0.0;
		pNmea->fLatInDegree = 0.0;
		pStr++; 
	}
	else
	{
		temp = str2dx(pStr,&pos);
		pNmea->fLatitude = temp;
		pNmea->fLatInDegree = format2deg(temp);
		pStr = pos+1;
	}
	if (*pStr == ',')
	{
		pNmea->cLatIdx = ' ';
		pStr++;
	}
	else
	{
		pNmea->cLatIdx = *pStr;
		pStr += 2;
	}

	// get longitude and format to degree
	if (*pStr == ',')
	{
		pNmea->fLongitude = 0.0;
		pNmea->fLonInDegree = 0.0f;
		pStr++; //指向方向标识
	}
	else
	{
		temp = str2dx(pStr,&pos);
		pNmea->fLongitude = temp;
		pNmea->fLonInDegree = format2deg(temp);
		pStr = pos+1;
	}
	if (*pStr == ',')
	{
		pNmea->cLonIdx = ' ';
		pStr++;
	}
	else
	{
		pNmea->cLonIdx = *pStr;
		pStr += 2;
	}

	// get fix index
	if (*pStr == ',')
	{
		pNmea->cFixQaulityIdx = 0;
		pStr++;
	}
	else
	{
		pNmea->cFixQaulityIdx = (char)(*pStr - 48);
		pStr += 2;
	}

	// get svs in used
	if (*pStr == ',')
	{
		pNmea->nUsedSvCount = 0;
		pStr++;
	}
	else
	{
		pNmea->nUsedSvCount = (char)A02TOI(pStr);
		pStr += 3;
	}

	// get hdop
	if (*pStr == ',')
	{
		pNmea->fHdop = 0.0f;
		pStr++;
	}
	else
	{
		pNmea->fHdop = (float)str2dx(pStr,&pos);
		pStr = pos + 1;
	}

	// get altitude
	if (*pStr == ',')
	{
		pNmea->fAltitude = 0.0f;
		pStr++;
	}
	else
	{
		/*
		while (*pStr != ',')
		{
			pStr++;
		}*/
		pNmea->fAltitude = (float)str2dx(pStr,&pos);
		pStr = pos + 1;
	}

	// get geoid height
	if (*pStr == ',')
	{
		pNmea->fGeoidHeight = 0.0f;
		pStr++;
	}
	else
	{
		pNmea->fGeoidHeight = (float)str2dx(pStr,&pos);
		pStr = pos + 1;
	}

	return errCode;
}

//$--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a,a*hh<CR><LF>
//$GNRMC,100533.000,A,4000.0010,N,11600.0026,E,0.053,82.745,050313,,E,A*08
unsigned int DecodeRmc(const char* pStr,tNmea_Msg *pNmea)
{
	char *pos;
	unsigned int errCode = 0;
	double temp;
	// Integrity and correctness check
	if (errCode = IntegrityAndCorrectCheck(pStr))
	{
		return errCode;
	}

	if (pStr[1] == 'G' && pStr[2] == 'N')
	{
		nRxRunMode = 3;
	}
	else if(pStr[1] == 'B' && pStr[2] == 'D')
	{
		nRxRunMode = 2;
	}
	else if(pStr[1] == 'G' && pStr[2] == 'P')
	{
		nRxRunMode = 1;
	}

	pNmea->nMsgMask |= GGA_DECODE;

	pStr += 7;
	// get time, hh:mm:ss.sss
	temp = str2dx(pStr,&pos);
	pNmea->usHours = (unsigned short)(temp/10000);
	temp -= pNmea->usHours*10000;
	pNmea->usMinutes = (unsigned short)(temp/100);
	pNmea->fSeconds = (float)(temp - pNmea->usMinutes*100);

	// get fix valid flag
	pStr = pos+1;
	pNmea->cFixFlag = *pStr;

	// get latitude and format to degree
	pStr += 2;
	if (*pStr == ',')
	{
		pNmea->fLatitude = 0.0;
		pNmea->fLatInDegree = 0.0;
		pStr++; //指向方向标识
	}
	else
	{
		temp = str2dx(pStr,&pos);
		pNmea->fLatitude = temp;
		pNmea->fLatInDegree = format2deg(temp);
		pStr = pos+1;
	}
	if (*pStr == ',')
	{
		pNmea->cLatIdx = ' ';
		pStr++;
	}
	else
	{
		pNmea->cLatIdx = *pStr;
		pStr += 2;
	}

	// get longitude and format to degree
	if (*pStr == ',')
	{
		pNmea->fLongitude = 0.0;
		pNmea->fLatInDegree = 0.0;
		pStr++; //指向方向标识
	}
	else
	{
		temp = str2dx(pStr,&pos);
		pNmea->fLongitude = temp;
		pNmea->fLonInDegree = format2deg(temp);
		pStr = pos+1;
	}
	if (*pStr == ',')
	{
		pNmea->cLonIdx = ' ';
		pStr++;
	}
	else
	{
		pNmea->cLonIdx = *pStr;
		pStr += 2;
	}

	// get ground velocity
	if (*pStr == ',')
	{
		pNmea->fGroundVel = 0.0f;
		pStr++;
	}
	else
	{
		pNmea->fGroundVel = (float)str2dx(pStr,&pos);
		pStr = pos + 1;
	}

	// get heading
	if (*pStr == ',')
	{
		pNmea->fHeading = 0.0;
		pStr++;
	}
	else
	{
		pNmea->fHeading = (float)str2dx(pStr,&pos);
		pStr = pos + 1;
	}

	// get year,month and day
	if (*pStr == ',')
	{
		pNmea->usYear = 6;
		pNmea->ucMonth = 1;
		pNmea->ucDay = 1;
		pStr++;
	}
	else
	{
		unsigned long ymd;
		ymd = (unsigned long)str2lx(pStr,&pos,10);
		pNmea->usYear = (unsigned short)(ymd%100);// + 2000 ?;
		pNmea->ucMonth = (unsigned char)((ymd%10000)/100);
		pNmea->ucDay = (unsigned char)(ymd/10000);
		pStr = pos + 1;
	}

	// 跳过磁偏角
	if (*pStr == ',')
	{
		pStr++;
	}
	else
	{
		while (*pStr != ',')
		{
			pStr++;
		}
		pStr++;
	}

	// 跳过磁偏角指示
	if (*pStr == ',')
	{
		pStr++;
	}
	else
	{
		pStr += 2;
	}

	// get fix mode index
	if (*pStr == ',')
	{
		pStr++;
	}
	else
	{
		pNmea->ucFixModeIndex = *pStr;
		pStr += 2;
	}

	return errCode;
}

// GPS:$GNGSA,A,3,34,60,59,55,35,41,53,49,48,,,,1.181,0.833,0.837*28
// BD :$GNGSA,A,3,05,11,10,06,08,12,01,,,,,,1.181,0.833,0.837*29
// GPS:$GPGSA,A,3,02,28,27,23,03,09,21,17,16,,,,1.181,0.833,0.837*28
// BD :$BDGSA,A,3,05,11,10,06,08,12,01,,,,,,1.181,0.833,0.837*29
unsigned int DecodeGsa(const char* pStr,tNmea_Msg *pNmea)
{
	int i;
	int sysIdx = -1;
	char offset = 0;
	char *pos;
	unsigned int errCode = 0;
	// Integrity and correctness check
	if (errCode = IntegrityAndCorrectCheck(pStr))
	{
		return errCode;
	}

	if (pStr[1] == 'B' && pStr[2] == 'D')
	{
		sysIdx = 0;
	}
	else if (pStr[1] == 'G' && pStr[2] == 'P')
	{
		sysIdx = 1;
	}

	pStr += 7;

	if (*pStr != 'A')
	{
		int j;
		pNmea->nUsedSvCount = 0;
		for(j=0;j<MAX_SYS;j++)
		{
			if (!(pNmea->nMsgMask | GGA_DECODE))
			{
				if (pNmea->aSvInfo[j].nSvsUsed == 0)
				{
					ClearSvData(&pNmea->aSvInfo[j]);	
				}
			}
		}
	}
	else
	{
		pStr += 4;
		if (sysIdx == -1)
		{
			if (A02TOI(pStr) > 32)
			{
				sysIdx = 1;
				offset = 32;
			}
			else
				sysIdx = 0;
		}
		pNmea->aSvInfo[sysIdx].nSvsUsed = 0;
		for (i=0;i<NMEA_CHANS;i++)
		{
			if (*pStr != ',')
			{
				pNmea->aSvInfo[sysIdx].nSvsUsed++;
				pNmea->aSvInfo[sysIdx].aucUsedSvid[i] = (unsigned char)A02TOI(pStr)-offset;
				pStr += 3;
			}
			else
			{
				pNmea->aSvInfo[sysIdx].aucUsedSvid[i] = 0;
				pStr++;
			}
		}
		
		if (!(pNmea->nMsgMask | GGA_DECODE))
		{
			// 跳过PDOP
			if (*pStr == ',')
			{
				pStr++;
			}
			else
			{
				while (*pStr != ',')
				{
					pStr++;
				}
				pStr++;
				//pNmea->fPdop = (float)strtod(pStr,&pos);
			}

			if (*pStr == ',')
			{
				pStr++;
			}
			else
			{
				pNmea->fHdop = (float)str2dx(pStr,&pos);
			}
		}
	}

	return errCode;
}


/*
$GPGSV,3,1,12,27,75,336,41,03,64,294,41,31,58,306,32,02,48,188,42*75
$GPGSV,3,2,12,32,32,090,33,17,30,061,41,29,30,090,32,28,26,116,41*78
$GPGSV,3,3,12,21,22,313,41,23,02,166,41,16,02,287,41,09,01,030,41*79
$BDGSV,2,1,08,08,72,221,45,06,64,148,45,11,50,047,45,01,37,145,44*64
$BDGSV,2,2,08,10,31,202,45,12,19,072,45,05,16,248,46,03,,,46*52
*/
unsigned int DecodeGsv(const char* pStr,tNmea_Msg *pNmea)
{
	int i;
	unsigned int errCode = 0;
	char nChanCnts,nMsgCnt,nMsgIdx;
	tSVData *pSvd;
	// Integrity and correctness check
	if (errCode = IntegrityAndCorrectCheck(pStr))
	{
		return errCode;
	}

	// BD or GPS
	if (pStr[1] == 'B' && pStr[2] == 'D')
	{
		pSvd = &pNmea->aSvInfo[0];
	}
	else if (pStr[1] == 'G' && pStr[2] == 'P')
	{
		pSvd = &pNmea->aSvInfo[1];
	}

	pStr += 7;

	nMsgCnt = (char)(*pStr - 48);
	pStr += 2;

	nMsgIdx = (char)(*pStr - 48);
	pStr += 2;

	nChanCnts = (char)A02TOI(pStr);
	pStr += 3;
	pSvd->nSvInView = nChanCnts;

	for (i=(nMsgIdx-1)*4;i<nChanCnts && i < nMsgIdx*4;i++)
	{
		// get svid
		if (*pStr == ',')
		{
			pSvd->aSvInView[i].svid = 0;
			pStr++;
		}
		else
		{
			pSvd->aSvInView[i].svid = (unsigned char)A02TOI(pStr);
			pStr += 3;
		}

		// get elevation
		if (*pStr == ',')
		{
			pSvd->aSvInView[i].elv = 0;
			pStr++;
		}
		else
		{
			pSvd->aSvInView[i].elv = (unsigned char)A02TOI(pStr);
			pStr += 3;
		}

		// get amazition
		if (*pStr == ',')
		{
			pSvd->aSvInView[i].amz = 0;
			pStr++;
		}
		else
		{
			pSvd->aSvInView[i].amz = (unsigned char)A03TOI(pStr);
			pStr += 4;
		}

		// get cno
		if (*pStr == ',')
		{
			pSvd->aSvInView[i].cno = 0;
			pStr++;
		}
		else
		{
			pSvd->aSvInView[i].cno = (unsigned char)A02TOI(pStr);
			pStr += 3;
		}
	}

	return errCode;
}

unsigned int DecodeAntennaChk(const char* pStr,char *AntSts)
{
	unsigned int errCode = 0;

	// Integrity and correctness check
	if (errCode = IntegrityAndCorrectCheck(pStr))
	{
		return errCode;
	}

	pStr += 7;
	*AntSts = (unsigned char)(*pStr);

	return errCode;
}

// 把经纬度的度分格式转换为度，
// ddmm.mmmm -> dd.ddddddd or dddmm.mmmm -> ddd.ddddddddd
double format2deg(double x)
{
	long deg;
	double frac;

	deg = (long)(x/100);
	frac = x - deg*100;

	return (deg+frac/60.0);
}
