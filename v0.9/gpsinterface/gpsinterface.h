#ifndef _GPS_INTERFACE_H_
#define _GPS_INTERFACE_H_

#include <stdio.h>
#include <map>
#include <vector>
#include <time.h>
#include <cstring>

using namespace std;

#define POSITION_INT_TO_FLOAT(x)          ((double)(x)) / ((double)(10*10*10*10*10*10))

// Read tuid from hardware.
// Output: tuid vector
// Return: 0 success; other fail
int InnovReadTuid(vector<unsigned char> &tuid);

typedef enum FenceType
{
    FENCE_TYPE_CIRCLE = 0,
    FENCE_TYPE_RECTANGLE,
    FENCE_TYPE_POLYGON,
    FENCE_TYPE_PATH,
    FENCE_TYPE_BUTT
} EnumFenceType;

class Point
{
public:
	Point(double x,double y, int lat, int lon)
	{
		px = x;
		py = y;
        latitude = lat;
        longitude = lon;
	}
	double getx(){return px;}
	double gety(){return py;}
	int getlatitude() {return latitude;}
	int getlongitude() {return longitude;}
private:
	int latitude;//纬度标示 北纬---0   南纬---1
	int longitude;//经度标示  东经---0   西经---1
	double px;//点的x坐标，纬度
	double py;//点的x坐标，经度
};

/****ElectronicFenceData结构体返回值说明========
===类型：圆
point_count = 1
radius = 圆的半径，单位米
points中只有一个点，即圆心的坐标
===类型：矩形
point_count = 2
radius（值无效）
points中有两个点，即左上角坐标，右下角坐标
===类型：多边形
point_count = N （多边形的顶点数）
radius（值无效）
points中有N个点，即顶点坐标
===类型：线段
point_count = N （线段的起始，拐点以及结束点个数）
radius = 线段的宽度，单位米
points中有N个点，即线段的起始点，拐点，结束点坐标
====时间
起始显示时间与结束显示时间确定了一个时间段，在该时间内地图上显示围栏*/
typedef struct ElectronicFenceData{
        int eid;
        long radius;//半径
        int point_count;//点的个数
        vector<unsigned char> tm_start;//起始显示时间
        vector<unsigned char> tm_stop;//结束显示时间
        vector<Point> points;
}StruElectronicFenceData;

//返回值0读取成功，非0读取失败
int getElectronicFenceByType(EnumFenceType type,int eid,struct ElectronicFenceData *pdata);//type围栏的类型，eid围栏在数据库中的id号，围栏的数据保存在pdata中

int getAllElectronicFenceByType(EnumFenceType type, vector<struct ElectronicFenceData> &data);

#endif
