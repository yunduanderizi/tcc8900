#include "dataBasePlugin.h"
#include "common_config.h"


#define  CAN_NOT_REVERT_BASE  	-1
using namespace DataManager;
static DataBasePluginC *gspMyObject = NULL;
PluginDataC *GetObject(void)
{
	if( NULL == gspMyObject )
	{
 		gspMyObject = new DataBasePluginC();
	}

	return gspMyObject;
}

void ReleaseObject(void)
{
	if( gspMyObject )
	{
		delete gspMyObject;
		gspMyObject = NULL;
	}
}
DataBasePluginC::DataBasePluginC()
{
	
}
DataBasePluginC::	~DataBasePluginC()
{
	
}
int  DataBasePluginC::_readBaseToStruct(sqlite3 *pdb,char *tableName,ColumnInfoList *columnInfoList,int &maxLine)
{
	int retval;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char tablesDataSel[512] = "SELECT * FROM ";
	strcat(tablesDataSel,tableName);
	sqlite3_stmt *pStmt_data = NULL;
	retval = sqlite3_prepare(pdb, tablesDataSel, strlen(tablesDataSel), &pStmt_data, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		fprintf(stderr, "list table count error: %s!\n", sqlite3_errmsg(pdb));
		return -1;
	}
	char countTableSel[512] = "SELECT count(*) FROM ";
	sqlite3_stmt *pStmt_line = NULL;
	strcat(countTableSel, tableName);
	retval = sqlite3_prepare(pdb, countTableSel, strlen(countTableSel), &pStmt_line, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		fprintf(stderr, "list table count error: %s!\n", sqlite3_errmsg(pdb));
		return -1;
	}
	retval = sqlite3_step(pStmt_line);
	if (retval!= SQLITE_ROW)
		return -1;
	maxLine = sqlite3_column_int(pStmt_line,0);
	for(int i = 0;i < maxLine;i++)
	{
		retval = sqlite3_step(pStmt_data);
		if (retval != SQLITE_ROW)
			break;
		for(int j = 0;j<sqlite3_column_count(pStmt_data);j++)
		{
			ColumnInfo mColumnInfo;	
			switch(sqlite3_column_type(pStmt_data,j))
			{
				case FIELDS_DATA_TYPE_INT:
					mColumnInfo.Data_Int= sqlite3_column_int(pStmt_data,j);
					break;
				case FIELDS_DATA_TYPE_DOUBLE:
					mColumnInfo.Data_Float= sqlite3_column_double(pStmt_data,j);	
					break;
				case FIELDS_DATA_TYPE_TEXT:
					if(strlen((char *)sqlite3_column_text(pStmt_data,j))>DATA_SIZE-3)
					{
						strcat(mColumnInfo.Data_Text,"'");
						strncpy(mColumnInfo.Data_Text,(char *)sqlite3_column_text(pStmt_data,j),DATA_SIZE-3);
						strcat(mColumnInfo.Data_Text,"\0");
						strcat(mColumnInfo.Data_Text,"'");
					}
					sprintf(mColumnInfo.Data_Text,"'%s'",(char *)sqlite3_column_text(pStmt_data,j));
					break;
				case FIELDS_DATA_TYPE_BLOB:
					if(strlen((char *)sqlite3_column_text(pStmt_data,j))>DATA_SIZE-1)
					{
						strcat(mColumnInfo.Data_Blob,"'");
						strncpy(mColumnInfo.Data_Blob,(char *)sqlite3_column_blob(pStmt_data,j),DATA_SIZE-1);
						strcat(mColumnInfo.Data_Blob,"\0");
						strcat(mColumnInfo.Data_Blob,"'");
					}
					sprintf(mColumnInfo.Data_Blob,"'%s'",(char *)sqlite3_column_blob(pStmt_data,j));
					break;
				default:
					break;
		
			}
			mColumnInfo.dataType = (FIELDS_DATA_TYPE)sqlite3_column_type(pStmt_data,j);
			mColumnInfo.declType = sqlite3_column_decltype(pStmt_data,j);
			mColumnInfo.name = (char *)sqlite3_column_name(pStmt_data,j);
			mColumnInfo.lineID = i+1;
			
			columnInfoList->append(mColumnInfo);
			
		}
		
	}
	
	sqlite3_finalize(pStmt_line);
	sqlite3_finalize(pStmt_data);
	
return 0;
}
bool  DataBasePluginC::_findDataFromStruct(int lineID,string name,string declType,ColumnInfoList *columnInfoList,ColumnInfo*columnInfo)
{
	for(int i = 0;i <columnInfoList->size();i++)
	{
		if(columnInfoList->at(i).lineID == lineID) 
		{
			if(columnInfoList->at(i).name.compare(name.c_str()) == 0)
			{
				if(columnInfoList->at(i).declType.compare(declType.c_str()) == 0)
				{
					switch(columnInfoList->at(i).dataType)
					{
						case FIELDS_DATA_TYPE_INT:
							columnInfo->Data_Int = columnInfoList->at(i).Data_Int;
							break;
						case FIELDS_DATA_TYPE_DOUBLE:
							columnInfo->Data_Float = columnInfoList->at(i).Data_Float;	
							break;
						case FIELDS_DATA_TYPE_TEXT:
							strcpy(columnInfo->Data_Text,columnInfoList->at(i).Data_Text);
							break;
						case FIELDS_DATA_TYPE_BLOB:
							strcpy(columnInfo->Data_Blob,columnInfoList->at(i).Data_Blob);
							break;
						default:
							break;
			
					}
					
					columnInfo->lineID = columnInfoList->at(i).lineID;
					columnInfo->name = columnInfoList->at(i).name;
					columnInfo->dataType = columnInfoList->at(i).dataType;
					return true;
				}
				
			}
		}
	}
	

return false;
}

bool DataBasePluginC::inquireSurporType(USER_DATA_SURPOR_TYPE surpor_type)
{
	printf("*************surpor_type([%d])\n",surpor_type);
	if(TYPE_DATA_BASE == surpor_type)
		return true;
	else
		return false;
}
bool DataBasePluginC::isRepairUserSubarea()
{
	printf("************* in  isRepairUserSubarea()\n");
	return false;
}
int DataBasePluginC::checkRervertUserData(int currentVersion,int backVersion)
{
	if(currentVersion<backVersion)
		return -1;
	return 0;
}
int DataBasePluginC::backupUserData(QString sAddress)
{
	sAddress += SERVICE_BACKNAME;
	char cmd[512] = "";
	sprintf(cmd,"cp %s %s",INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY,sAddress.toAscii().data());
	if(system(cmd)<0)
	{
		printf("ERROR:backup User Data error\n");
		return DATA_BASE_BACK_ERROR;
	}
	return DATA_BASE_PLUGIN_OK;
}
int DataBasePluginC:: revertUserData(QString sAddress)
{
	sAddress += SERVICE_BACKNAME;
	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pListTablesNameSel = (char*)"SELECT name FROM sqlite_master";
	retval = sqlite3_open(sAddress.toAscii().data(), &pDB);
	if (retval != SQLITE_OK)
	{
		fprintf(stderr, "can't open database: %s!\n",sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return DATA_BASE_OPEN_ERROR;
	}
	retval = sqlite3_busy_timeout(pDB, 1000);
	if (retval != SQLITE_OK)
	{
		fprintf(stderr, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return DATA_BASE_OUT_TIME;
	}
	
	retval = sqlite3_prepare(pDB, pListTablesNameSel, strlen(pListTablesNameSel), &pStmt, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		fprintf(stderr, "list table count error: %s!\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return DATA_BASE_PREPARE_ERROR;
	}
	while (1)
	{
		retval = sqlite3_step(pStmt);
		if (retval != SQLITE_ROW)
			break;
		char mTableName[200] = {""};

		sqlite3 *pDB_base = NULL;
		sqlite3_stmt *pStmt_base = NULL;
		retval = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB_base);
		if (retval != SQLITE_OK)
		{
			fprintf(stderr, "can't open database: %s!\n",sqlite3_errmsg(pDB_base));
			break;
		}
		if(strcmp((char *)sqlite3_column_text(pStmt,0),"sqlite_sequence"))
		{
			strcat(mTableName,(char *)sqlite3_column_text(pStmt,0));
			ColumnInfoList mColumnInfoList;
			int state;
			int max_Line;
			state = _readBaseToStruct(pDB, mTableName,&mColumnInfoList,max_Line);
			if(state<0)
			{
				fprintf(stderr, "ERROR:  _readBaseToList error [%d]\n",state);
				return DATA_BASE_READ_ERROR;
			}
			char pSelectTablesSel[512]= "SELECT * FROM ";
			strcat(pSelectTablesSel,mTableName);
			retval = sqlite3_prepare(pDB_base, pSelectTablesSel, strlen(pSelectTablesSel), &pStmt_base, (const char**)&pError);
			if (retval != SQLITE_OK)
			{
				fprintf(stderr, "list table count error: %s!\n", sqlite3_errmsg(pDB_base));
				goto NEXT_TABLE;
			}	
			ColumnInfoList inster_ColumnInfoList[max_Line];
			for(int i = 0;i<max_Line;i++)
			{
				retval = sqlite3_step(pStmt_base);
				for(int j = 0;j<sqlite3_column_count(pStmt_base);j++)
				{
					ColumnInfo columnInfo_add;
					if(!_findDataFromStruct(i+1,sqlite3_column_name(pStmt_base,j), sqlite3_column_decltype(pStmt_base,j), &mColumnInfoList,&columnInfo_add))
					{
						columnInfo_add.name = sqlite3_column_name(pStmt_base,j);
						columnInfo_add.declType = sqlite3_column_decltype(pStmt_base,j);
						switch(sqlite3_column_type(pStmt_base,j))
						{
							case FIELDS_DATA_TYPE_INT:
								columnInfo_add.Data_Int = 0;
								break;
							case FIELDS_DATA_TYPE_DOUBLE:
								columnInfo_add.Data_Float = 0.0;	
								break;
							case FIELDS_DATA_TYPE_TEXT:
								strcpy(columnInfo_add.Data_Text,"''");
								break;
							case FIELDS_DATA_TYPE_BLOB:
								strcpy(columnInfo_add.Data_Blob,"''");
								break;
							default:
								break;
						}
						columnInfo_add.dataType = (FIELDS_DATA_TYPE)sqlite3_column_type(pStmt_base,j);
						columnInfo_add.lineID = i+1;
	
					}
					inster_ColumnInfoList[i].append(columnInfo_add);			
				}							
			}
			sqlite3_finalize(pStmt_base);
			char deleteTableSel[512] ="DELETE FROM ";
			strcat(deleteTableSel ,mTableName);
			retval = sqlite3_exec(pDB_base,deleteTableSel,0,0,&pError);
			if (retval != SQLITE_OK)
			{
				fprintf(stderr, "list table count error: %s!\n", sqlite3_errmsg(pDB_base));
				break;
			}
		
			for(int i = 0;i<max_Line;i++)
			{
				QString insterTableSel = "";
				insterTableSel += "INSERT INTO ";
				insterTableSel += mTableName;
				insterTableSel += "(";
				for(int t = 0;t<inster_ColumnInfoList[i].size();t++)
				{
					insterTableSel += "'";
					insterTableSel += inster_ColumnInfoList[i].at(t).name.c_str();
					insterTableSel += "'";
					if(t < inster_ColumnInfoList[i].size()-1 )
						insterTableSel += ",";
				}
				insterTableSel+= ") values(";
				for(int t = 0;t<inster_ColumnInfoList[i].size();t++)
				{
					switch(inster_ColumnInfoList[i].at(t).dataType)
					{
						case FIELDS_DATA_TYPE_INT:
							insterTableSel += insterTableSel.number(inster_ColumnInfoList[i].at(t).Data_Int);
							break;
						case FIELDS_DATA_TYPE_DOUBLE:
							char dataFloat[500];
							sprintf(dataFloat,"%f",inster_ColumnInfoList[i].at(t).Data_Float);
							insterTableSel += dataFloat;
							break;
						case FIELDS_DATA_TYPE_TEXT:
							insterTableSel += inster_ColumnInfoList[i].at(t).Data_Text;
							break;
						case FIELDS_DATA_TYPE_BLOB:
							insterTableSel +=inster_ColumnInfoList[i].at(t).Data_Blob;
						
							break;
						default:
							insterTableSel += "NULL";
							break;
					}
					if(t < inster_ColumnInfoList[i].size()-1 )
						insterTableSel += ",";
					}
				insterTableSel+= ")";
				retval = sqlite3_exec(pDB_base,insterTableSel.toAscii().data(),0,0,&pError);
				if (retval != SQLITE_OK)
				{
					fprintf(stderr, "list table count error: %s!\n", sqlite3_errmsg(pDB_base));
					break;
				}
			}

			
		}
NEXT_TABLE:
			sqlite3_close(pDB_base);
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return DATA_BASE_PLUGIN_OK;
}
