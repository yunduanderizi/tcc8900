/***
**
**
*/
/*
使用说明：
1. 在nmea_type.h中实现了接口的声明，使用时请包含它，例如：#include "nmea_type.h".
2. 定义一个tNmea_Msg的结构体，用于存储解码后的NMEA信息；
3. 调用NmeaDecode(const char* pStr,tNmea_Msg *pNmea)方法，调用要求：
(1) pStr是一个完整的语句，例如 pStr = "$GNGGA,100533.000,4000.0010,N,11600.0026,E,1,16,0.777,53.850,M,0.0,M,,*4A";
(2) pNmea是解码参数存储的地方。
(3) 一旦有一组完整的语句时，就需要调用NmeaDecode（），调用后查看pNmea->nMsgMask的状态，就可以知道一组NMEA消息（GGA,GSA,GSV,RMC,ANT）是否完全被解码；
(4) 由于一组NMEA语句的顺序总是GGA开始，ANT结束的，所以再一次调用NmeaDecode（）传入的参数pStr是GGA语句时，pNmea指向的空间被清空，所以当pNmea->nMsgMask的状态满足条件时，
一定要把解析完成的数据传出去。
*/ 
#include <stdio.h>
#include <stdlib.h>
#include "nmea_type.h"

//test code:

void main(void)
{
	int i;
	int ret;
	tNmea_Msg nmeamsg ; //用于存储解码信息的结构体

	// 实际运行时，pstr是从串口的缓存中读取的，假设已经读取了一组数据，并且被处理成一条一条完整的语句
	const char *pstr[]={
		"$GNGGA,025746.000,3957.5027,N,11616.9777,E,0,,,78.961,M,0.0,M,,*69",
		"$GNGSA,A,1,,,,,,,,,,,,,,,*00",
		"$GNGGA,101039.000,4000.0003,N,11600.0009,E,1,18,0.735,54.004,M,0.0,M,,*4D",
		"$GNGSA,A,1,,,,,,,,,,,,,,,*00",	
		"$GNGSA,A,3,55,34,49,41,53,35,60,48,40,59,,,1.102,0.735,0.821*29",
		"$GNGSA,A,3,03,05,01,11,06,08,10,12,,,,,1.102,0.735,0.821*2F",
		"$GPGSV,3,1,12,27,75,338,40,03,65,295,40,31,58,306,35,02,49,188,40*7F",
		"$GPGSV,3,2,12,32,32,089,34,17,29,061,40,28,25,117,40,21,23,313,40*7A",
		"$GPGSV,3,3,12,08,19,094,40,16,02,286,40,23,02,166,40,09,01,030,40*77",
		"$BDGSV,2,1,08,08,72,222,48,06,63,148,48,11,49,048,48,03,43,189,49*66",
		"$BDGSV,2,2,08,01,37,145,49,10,32,202,48,12,18,073,48,05,16,248,49*67",
		"$GNRMC,101039.000,A,4000.0003,N,11600.0009,E,0.024,90.975,050313,,E,A*07",
		"$CFANT,0*42"};

	for (i=0;i<10;i++)
	{
		printf("decode %s\n",pstr[i]);
		memset(&nmeamsg,0,sizeof(tNmea_Msg));
		ret = NmeaDecode(pstr[i],&nmeamsg);
		// 监控是否解码一组消息
		printf("ret = %x\n",ret);
//		if (nmeamsg.nMsgMask == 0x8F)
		{
				fprintf(stdout,"nAnttenaStatus = %d fLatitude = %f fLongitude = %f (%d:%d:%f)\n",
						nmeamsg.nAnttenaStatus,
						nmeamsg.fLatitude,
						nmeamsg.fLongitude,
						nmeamsg.usHours,	
						nmeamsg.usMinutes,
						nmeamsg.fSeconds);
				
			// 已经完整解码一组NMEA消息，调用上传的函数，将数据传出去
			//xxxxx();
		}
	}
}
