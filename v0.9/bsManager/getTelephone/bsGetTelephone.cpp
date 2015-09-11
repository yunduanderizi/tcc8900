/**
 * @file   bsLocationUpload.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110704
 * 
 * @brief  
 *      Implement for getting telephone of sevice or helping
 * 
 */
 
#include <QDir>
#include <QMutex>
#include <QWaitCondition>
#include <QXmlStreamReader>
#include <QTextCodec>
#include <curl.h>
#include "bsGetTelephone.h"
#include "bsClient.h"
#include <sqlite3.h>
#include "common_config.h"
#include "bsCommunalLock.h"
#define TELEPHONE_XMLFEIL  "Telephone/Download/OEMTelephone.xml"

#define  BS_NEWS_E_CALLBACK	42



static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{		
	if ( (*m_bExit) == true)
	{
		return -1;
	}	
	return 0;
}

BsGetTelephoneC *BsGetTelephoneC::thisObject = NULL;
BsGetTelephoneC *BsGetTelephoneC::object()
{
	BsGetTelephoneC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsGetTelephoneC();
	}

	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

void BsGetTelephoneC::releaseObject()
{
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
}

BsGetTelephoneC *BsGetTelephoneC::getObject()
{
	BsGetTelephoneC*pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	QIODevice *pFile = (QIODevice *)stream;

	qint64 writeen = pFile->write((char *)ptr, (qint64)(size*nmemb));
	if(writeen < nmemb*size)
	{
		return -1;
	}
	return writeen;
}

static int _emptyTelephoneFromDatabase()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from BsTelephone";

	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))	
		return -1;
	}

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return -1;
	}

	while (1)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	sqlite3_finalize(pStmt);

	sqlite3_close(pDB);


	return 0;
}

static int _readTelephoneFromDatabase(BsTelephoneList &telephoneList, bool bFromDefault=false)
{
	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pQueryTablesSel[128];
	if( bFromDefault )
	{
		strcpy(pQueryTablesSel, "SELECT * FROM BsTelephoneDefault");
	}
	else
	{
		strcpy(pQueryTablesSel, "SELECT * FROM BsTelephone");
	}
	
	retval = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database: %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);
		return -1;
	}

	retval = sqlite3_busy_timeout(pDB, 1000);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	retval = sqlite3_prepare(pDB, pQueryTablesSel, strlen(pQueryTablesSel), &pStmt, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);
		return -1;
	}

	while(1)
	{
		int ret = sqlite3_step(pStmt);
		if( ret != SQLITE_ROW )
		{
			//It is completed to read table
			break;
		}
		else
		{
			BsTelephoneC node;
			node.id = (int)sqlite3_column_int(pStmt, 0);
			node.phoneNumber = (char*)sqlite3_column_text(pStmt, 1);
			node.callType = (Telephone_Call_Type_E)sqlite3_column_int(pStmt, 2);
			node.callBackNumber.append((char*)sqlite3_column_text(pStmt, 3));

			bool bFind = false;
			for(int i=0; i<telephoneList.count(); i++)
			{
				if( node.id == telephoneList[i].id )
				{
					bFind = true;
					if( TCT_PSTN_CALL_BACK == telephoneList[i].callType && TCT_PSTN_CALL_BACK == node.callType )
					{
						telephoneList[i].callBackNumber.append(node.callBackNumber[0]);
					}
				}
			}

			if( !bFind )
			{
				telephoneList.append(node);
			}
		}
	}
	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}

static int _saveTelephoneToDatabase(BsTelephoneList &telephoneList)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from BsTelephone";
	char pInsertTablesSel[128] = {0};

	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))	
		return -1;
	}

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return -1;
	}

	while (1)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	sqlite3_finalize(pStmt);

	for(int loop=0; loop<telephoneList.count(); loop++)
	{
		if( TCT_PSTN_CALL_BACK == telephoneList[loop].callType )
		{
			for(int i=0; i<telephoneList[loop].callBackNumber.count(); i++)
			{
				sprintf(pInsertTablesSel, "insert into BsTelephone values('%d', '%s', '%d', '%s')", \
						telephoneList[loop].id, telephoneList[loop].phoneNumber.toLocal8Bit().data(), \
						telephoneList[loop].callType, telephoneList[loop].callBackNumber[i].toLocal8Bit().data());

				ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
				if (ret != SQLITE_OK)
				{
		                   IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 3: %s\n", sqlite3_errmsg(pDB));
					sqlite3_close(pDB);
					return -1;
				}

				ret = sqlite3_step(pStmt);
				if (ret != SQLITE_DONE)
				{
					IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
					sqlite3_close(pDB);
					return -1;
				}
				
				sqlite3_finalize(pStmt);
			}
		}
		else
		{
			sprintf(pInsertTablesSel, "insert into BsTelephone values('%d', '%s', '%d', '%s')", \
					telephoneList[loop].id, telephoneList[loop].phoneNumber.toLocal8Bit().data(), \
					telephoneList[loop].callType, "");

			ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
			if (ret != SQLITE_OK)
			{
	                   IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 3: %s\n", sqlite3_errmsg(pDB));
				sqlite3_close(pDB);
				return -1;
			}

			ret = sqlite3_step(pStmt);
			if (ret != SQLITE_DONE)
			{
				IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
				sqlite3_close(pDB);
				return -1;
			}
			
			sqlite3_finalize(pStmt);
		}
	}

	sqlite3_close(pDB);

	return 0;
}
BsGetTelephoneC::BsGetTelephoneC()
	:m_bExit(false),
	m_bStartAgain(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
}

BsGetTelephoneC::~BsGetTelephoneC()
{
	if( isRunning() )
	{
		cancelOperation();
	
		wait();
	}
}

void	BsGetTelephoneC::cancelOperation()
{
	m_mutexForThreadSafe.lock();

	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;
		m_bStartAgain = false;	

		m_mutexWaitCondition.lock();
		m_condExit.wakeAll();
		m_mutexWaitCondition.unlock();
	}

	m_mutexForThreadSafe.unlock();
}

void BsGetTelephoneC::slotFinished()
{
	m_mutexForThreadSafe.lock();
	if( m_bStartAgain && !isRunning() )
	{
		m_bStartAgain = false;
		m_bExit = false;

		start();
	}	
	m_mutexForThreadSafe.unlock();
}

void BsGetTelephoneC::startGettingTelephone()
{
	m_mutexForThreadSafe.lock();

	if(!isRunning() )
	{
		m_bExit = false;
		start();
	}
	else
	{
		m_bExit = true;
		m_bStartAgain = true;	
	}

	m_mutexForThreadSafe.unlock();
}

bool BsGetTelephoneC::isFinish()
{
	bool bFinished = false;
	
	m_mutexForThreadSafe.lock();

	if( !isRunning() )
	{
		bFinished = true;
	}

	m_mutexForThreadSafe.unlock();

	return bFinished;
}

QString BsGetTelephoneC::getOEMTelephone(TELEPHONE_TYPE typeNum)
{
	BsTelephoneList phoneList;
	int retval = BS_E_OK;
	QString	phoneNumber = "";
	
	phoneList.clear();
	
	if( _readTelephoneFromDatabase(phoneList) < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read telephone from database error!\n")
		return phoneNumber;
	}

	if( phoneList.count() == 0 )
	{
		if( _readTelephoneFromDatabase(phoneList, true) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read telephone from database error!\n")
			retval = BS_E_DATABASE_ERROR;
		}
	}
	
	for(int loop=0; loop<phoneList.count(); loop++)
	{
		if( (int)(phoneList[loop].id) == (int)(typeNum) )
		{
			phoneNumber = phoneList[loop].phoneNumber;
			break;
		}
	}
	
	return phoneNumber;
}

int BsGetTelephoneC::_getTelephoneFromBS(QFile *file, BsTelephoneList& phoneList)
{
	int retval = -1;
	char TelephoneUrl[URL_MAX_LENGTH];
	char apHost[URL_MAX_LENGTH];

	char tuid[BK_TUID_LENGTH_MAX+1];
	char uuid[BK_UUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;

	phoneList.clear();

	retval = BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword);
	if( retval < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getLoginResult fail\n")
		return retval;
	}

	if( !bLogin )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"not get telephone when not login\n")
		return BS_E_USER_NOT_LOGIN;
	}
	
	if( 0 == strcmp(tuid, "") )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"tuid is void\n")
		return BS_E_GET_TUID_ERROR;
	}

	retval = BsBaseC::getApApiHost(Ap_telephone, apHost);
	if( BS_E_OK != retval )	
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getApApiInfo fail\n")
		return retval;
	}
	
	sprintf(TelephoneUrl, "http://%s/ap005/api/1.0/tu/phone/?tuid=%s&v=2", apHost, tuid);
	CURL *curl = curl_easy_init();

	CURLcode res;
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, TelephoneUrl);		
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
				
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	      	curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)file);
     		
		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK)
		{
			if (res ==  BS_NEWS_E_CALLBACK)
				IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
			else
				IBD_PRINTF(DEBUG_INFOMATION, "Error: curl error: %s\n",curl_easy_strerror(res));
			
			curl_easy_cleanup(curl);
		}	
		else
		{
			bool				xml_ok = false;
			bool				bCallbackPhone = false;
			BsTelephoneC		phoneNode;
			QString			phoneId;
			QString			callType;
			QIODevice *device = (QIODevice *)file;
			device->seek(0);
		     
			QXmlStreamReader xml;
			xml.setDevice(device);
			while(!xml.atEnd())
			{
				xml.readNext();
				if( xml.isStartElement() && xml.name()=="status" )
				{
					if( xml.readElementText().compare("OK") == 0 )
					{
						xml_ok = true;
					}
				}
				
				if( xml.isStartElement() && xml.name()=="id" )
				{
					phoneId = xml.readElementText();
					bool				ok;
					phoneNode.id = phoneId.toInt(&ok);
					if( !ok )
					{
						phoneNode.id = -1;
					}
				}

				if( xml.isStartElement() && xml.name()=="phone" )
				{
					if( bCallbackPhone )
					{
						phoneNode.callBackNumber.append(xml.readElementText());
					}
					else
					{
						phoneNode.phoneNumber = xml.readElementText();
					}
				}
				
				if( xml.isStartElement() && xml.name()=="call_type" )
				{
					callType= xml.readElementText();
					if( 0 == callType.compare("PSTN") )
					{
						phoneNode.callType = TCT_PSTN;
					}
					else if( 0 == callType.compare("PSTN_CALL_BACK") )
					{
						phoneNode.callType = TCT_PSTN_CALL_BACK;
					}
					else if( 0 == callType.compare("SKYPE") )
					{
						phoneNode.callType = TCT_SKYPE;
					}
					else
					{
						IBD_PRINTF(DEBUG_INFOMATION, "It is impossible to come here\n");
					}
				}
				
				if( xml.isStartElement() && xml.name()=="call_back_phone" )
				{
					bCallbackPhone = true;
				}

				if( xml.isEndElement() && xml.name()=="call_back_phone" )
				{
					bCallbackPhone = false;
				}

				if( xml.isEndElement() && xml.name()=="data" )
				{
					if( xml_ok && phoneNode.id >= 0 && !(phoneNode.phoneNumber.isEmpty()) )
					{
						phoneList.append(phoneNode);
					}
					phoneNode.clear();
				}
			}

			curl_easy_cleanup(curl);
		}
	}

	file->close();

	if( phoneList.count() > 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Success: get phone number from %s\n", TelephoneUrl);
		return BS_E_OK;
	}
	else		
	{
		if (res == CURLE_OK) 
		{
			IBD_PRINTF(DEBUG_INFOMATION, "parse xml error!\n")
			return BS_E_URL_XML_PARSE_ERROR;
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "curl_easy_perform fail res[%d]!\n", res)
			return BS_E_URL_HTTP_ERROR;
		}
	}
}

int BsGetTelephoneC::getTelephone(BsTelephoneList& phoneList)
{
	int retval = BS_E_OK;
	
	phoneList.clear();
	
	if( _readTelephoneFromDatabase(phoneList) < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read telephone from database error!\n")
		retval = BS_E_DATABASE_ERROR;
		return retval;
	}

	if( phoneList.count() == 0 )
	{
		if( _readTelephoneFromDatabase(phoneList, true) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read telephone from database error!\n")
			retval = BS_E_DATABASE_ERROR;
			return retval;
		}
	}
	
	return retval;
}

void BsGetTelephoneC::emptyTelephone()
{
	BsTelephoneList phoneList;
	
	phoneList.clear();
	
	if( _readTelephoneFromDatabase(phoneList) < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read telephone from database error!\n")
		return;
	}

	_emptyTelephoneFromDatabase();

	if( phoneList.count() > 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "telephone have changed!\n")

		BsClientC *pClient = BsClientC::getObject();
		if( pClient )
		{
			pClient->internalSendCommonCommand(BM_COMMAND_TELEPHONE);
		}
		emit sigTelephoneChanged();		
	}

}

void BsGetTelephoneC::run()
{
	int loop = 0;
	int retval = 0;
	BsTelephoneList telephoneListFromBs;
	BsTelephoneList telephoneListFromDatabase;
	QDir telephonePath;
	char telephoneFileDir[512] = {""};
	sprintf(telephoneFileDir,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,"Telephone/Download");
	
	if(false == telephonePath.mkpath(telephoneFileDir))
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:mkdir path fial\n");
		return;
	}
	char telephoneFilePath[512] = {""};
	sprintf(telephoneFilePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,TELEPHONE_XMLFEIL);
	QFile TelephoneFile(telephoneFilePath); 
	if (!TelephoneFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:open file %s faill\n",telephoneFilePath);
        	return;
	}
	
	while(1)
	{
		if( m_bExit )
			break;

		//when fail, need wait some time
		if( loop > 0 )
		{
			m_mutexWaitCondition.lock();
			m_condExit.wait(&m_mutexWaitCondition, 10000);
			m_mutexWaitCondition.unlock();
		}
		
		if( (retval=_getTelephoneFromBS(&TelephoneFile, telephoneListFromBs)) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "_getTelephoneFromBS error!\n")
			if( BS_E_URL_HTTP_ERROR == retval )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "because http error, so try it again!\n")
				loop++;
				continue;
			}
		}

		if( retval >= 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "get telephone from bs success!\n")

			m_mutexForThreadSafe.lock();

			if( _readTelephoneFromDatabase(telephoneListFromDatabase) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "Error: get telephone from database error\n");
			}
			
			if( 0 != _comparePhoneList(telephoneListFromBs, telephoneListFromDatabase) )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "telephone have changed!\n")

				if(_saveTelephoneToDatabase(telephoneListFromBs)<0)
				{
					IBD_PRINTF(DEBUG_INFOMATION, "save telephone to database error!\n")
				}

				BsClientC *pClient = BsClientC::getObject();
				if( pClient )
				{
					pClient->internalSendCommonCommand(BM_COMMAND_TELEPHONE);
				}
				emit sigTelephoneChanged();
			}
		
			m_mutexForThreadSafe.unlock();
		}
		
		break;
	}
}

static int _compareQStringList(QStringList& list1, QStringList& list2)
{
	if( list1.count() != list2.count() )
	{
		return -1;
	}
	
	for(int i=0; i<list1.count(); i++)
	{
		if( 0 != list1[i].compare(list2[i]) )
		{
			return -1;
		}
	}

	return 0;
}

int BsGetTelephoneC::_comparePhoneList(BsTelephoneList& list1, BsTelephoneList& list2)
{
	if( list1.count() != list2.count() )
	{
		return -1;
	}
	
	for(int i=0; i<list1.count(); i++)
	{
		if( list1[i].id != list2[i].id || \
			0 != list1[i].phoneNumber.compare(list2[i].phoneNumber) || \
			list1[i].callType != list2[i].callType || \
			0 != _compareQStringList(list1[i].callBackNumber, list2[i].callBackNumber) )
		{
			return -1;
		}
	}

	return 0;
}

void BsGetTelephoneC::internalNotify(BsManager_Command_Id id)
{
	if( BM_COMMAND_TELEPHONE == id )
	{
		emit sigTelephoneChanged();
	}
}

