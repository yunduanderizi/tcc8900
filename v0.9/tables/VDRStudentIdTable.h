#ifndef _VDR_STUDENT_ID_TABLE_H_
#define _VDR_STUDENT_ID_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
//#include "TableNameDef.h"

using namespace std;

#define VDR_STUDENT_ID_TABLE_MAX_ROWS					200
#define VDR_SCHOOL_STUDENTS_DB_NAME					("studentid.db")

#define VDR_SCHOOL_STUDENTS_CNT_DB_NAME				("studentcnt.db")
#define VDR_STUDENTS_ID_CNT_TABLE_NAME				("count")

#define VDR_STUDENTS_ID_UP_TABLE_NAME			("idup")
#define VDR_STUDENTS_ID_DOWN_TABLE_NAME			("iddown")

typedef enum
{
    VDR_STUDENT_ID_NUMBER = 0,
    VDR_STUDENT_ID_READ_TIME,    
	VDR_STUDENT_ID_INDEX,
    VDR_STUDENT_ID_DETAIL_BUTT
}ENUM_VDR_STUDENT_ID;

class VDRStudentIdUpTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRStudentIdUpTable();
	
public:
		int UpRefreshByEqualToStr(int colId,string str);
};


typedef enum
{
    VDR_STUDENT_ID_DAYTIME = 0,
	VDR_STUDENT_ID_COUNT,
	VDR_STUDENT_ID_COUNT_BUTT
}ENUM_VDR_STUDENT_ID_COUNT;

class VDRIdCountTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRIdCountTable();
};



#endif
