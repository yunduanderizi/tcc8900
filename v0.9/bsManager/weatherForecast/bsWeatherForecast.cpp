#include "bsWeatherForecast.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QMutex>
#include <QDebug>
#include "common_config.h"
#include "bsCommunalLock.h"
#define  TODAY                                                                      1
#define  TOMORROW                                                              2
#define  AFTERTOMORROW                                                    3

#define WEATHER_FORECAST_XMLFEIL  "weatherForecast/Download/weatherForecast.xml"
#define WEATHER_FORECAST_ICON_PATH  "/nand3/tmpdata/weatherForecast/icon/"



static QString getWeatherName(QString weatherType)
{
	QString pTmp;
	if(weatherType == "-1")
		pTmp = "unknown";
	else if(weatherType == "0")
		pTmp = "fine";
	else if(weatherType == "1")
		pTmp = "cloudy";
	else if (weatherType == "2")
		pTmp = "overcast";
	else if(weatherType == "3")
		pTmp = "shower";
	else if(weatherType == "4")
		pTmp = "thunder_shower";
	else if(weatherType == "5")
		pTmp = "thunder_shower_and_accompanied_by_hail";
	else if(weatherType == "6")
		pTmp = "sleet";
	else if(weatherType == "7")
		pTmp = "light_rain";
	else if(weatherType == "8")
		pTmp = "moderate_rain";
	else if(weatherType == "9")
		pTmp ="heavy_rain";
	else if(weatherType == "10")
		pTmp = "torrential_rain";
	else if(weatherType == "11")
		pTmp = "downpour";
	else if(weatherType == "12")
		pTmp = "extraordinary_rainstorm";
	else if(weatherType == "13")
		pTmp = "snow_shower";
	else if(weatherType == "14")
		pTmp ="light_snow";
	else if(weatherType == "15")
		pTmp = "moderate_snow";
	else if(weatherType == "16")
		pTmp ="heavy_snow";
	else if(weatherType == "17")
		pTmp ="severe_snow";
	else if(weatherType == "18")
		pTmp = "fog";
	else if(weatherType == "19")
		pTmp = "ice_rain";
	else if(weatherType == "20")
		pTmp = "sand_storm";
	else if(weatherType == "21")
		pTmp = "small_to_costarring";
	else if(weatherType == "22")
		pTmp = "moderate_to_heavy_rain";
	else if(weatherType == "23")
		pTmp = "heavy_rains";
	else if(weatherType == "24")
		pTmp ="heavy_rains_to_heavy_rain_storm_to_the_storm";
	else if(weatherType == "25")
		pTmp = "heavy_rain_to_severe_storms";
	else if(weatherType == "26")
		pTmp = "small_to_moderate_snow";
	else if(weatherType == "27")
		pTmp = "moderate_to_heavy_snow";
	else if(weatherType == "28")
		pTmp = "big_to_blizzard_snow";
	else if(weatherType == "29")
		pTmp = "floating_dust";
	else if(weatherType == "30")
		pTmp = "dust_blowing";
	else if(weatherType == "31")
		pTmp = "severe_dust_devil";	
	return pTmp;
}
static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{	
	if ( *m_bExit == true)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "in  m_bExit_func  m_bExit = %d\n",m_bExit);
		return -1;
	}	
	return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	QIODevice *pFile = (QIODevice *)stream;

	qint64 writeen = pFile->write((char *)ptr, (qint64)(size*nmemb));
	if(writeen < nmemb*size)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:write_data fail\n");
		return -1;
	}
	return writeen;
}

BsWeatherForecastC  *BsWeatherForecastC::thisObject = NULL;
BsWeatherForecastC  *BsWeatherForecastC::object()
{
	BsWeatherForecastC *pTmp = NULL;
	initBsManager();	
	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsWeatherForecastC ();
	}

	pTmp = thisObject;

	bsManagerObjectUnLock();
	
	return pTmp;
}

BsWeatherForecastC  *BsWeatherForecastC::thisObject1 = NULL;
BsWeatherForecastC  *BsWeatherForecastC::object1()
{
	BsWeatherForecastC *pTmp = NULL;
		
	bsManagerObjectLock();
	
	if( !thisObject1 )
	{
		thisObject1 = new BsWeatherForecastC ();
	}

	pTmp = thisObject1;
	bsManagerObjectUnLock();
	
	return pTmp;
}

void BsWeatherForecastC::releaseObject()
{
	bsManagerObjectLock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
	
}

void BsWeatherForecastC::releaseObject1()
{
	bsManagerObjectLock();

	if( thisObject1 )
	{
		delete thisObject1;
		thisObject1 = NULL;
	}
	bsManagerObjectUnLock();
	
}

BsWeatherForecastC *BsWeatherForecastC::getObject()
{
	BsWeatherForecastC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

BsWeatherForecastC *BsWeatherForecastC::getObject1()
{
	BsWeatherForecastC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
BsWeatherForecastC ::BsWeatherForecastC ()
	:m_bExit(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
	char buf[500];
	urlEncode((unsigned char *)"北京", (unsigned char *)buf);
	m_city = buf;
}

BsWeatherForecastC ::~BsWeatherForecastC ()
{
	if( isRunning() )
	{
		cancelOperation();

		wait();
	}
}

void BsWeatherForecastC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( (!m_list.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

int BsWeatherForecastC::setCityPinyin(const char *pCity)
{
	m_mutexForThreadSafe.lock();

	if( !pCity )
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_ARG_INVALID;
	}
	m_city = pCity;

	IBD_PRINTF(DEBUG_INFOMATION,"set city name is :[%s]\n",pCity)

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

int BsWeatherForecastC::setCityUtf8(const char *pCity)
{
	m_mutexForThreadSafe.lock();

	if(NULL == pCity)
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_ARG_INVALID;
	}

	if(strlen(pCity)*3>500)
	{
		IBD_PRINTF(DEBUG_INFOMATION,"set city name [%s] is  too long\n",pCity)
		m_mutexForThreadSafe.unlock();
		return BS_WEATHER_CITY_NAME_LONGER;
	}
	char buf[500];
	urlEncode((unsigned char *)pCity, (unsigned char *)buf);

	m_city = buf;

	IBD_PRINTF(DEBUG_INFOMATION,"set city name is :[%s]\n",pCity)

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

int BsWeatherForecastC::startGetWeatherForecast()
{
	m_mutexForThreadSafe.lock();
	
	DownloadTask myTask;
	m_city += "&v=2";
	myTask.city = m_city;
	m_list.append(myTask);
	
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}

	m_mutexForThreadSafe.unlock();
		
	return BS_E_OK;
}

void	BsWeatherForecastC ::cancelOperation()
{
	m_mutexForThreadSafe.lock();

	m_list.clear();
	
	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;
	}

	m_mutexForThreadSafe.unlock();
}

int BsWeatherForecastC::parseXmlWeather(QFile *file, BsWeatherForecastInfo *pWeather)
{
	if(file == NULL || pWeather == NULL)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:the parseXmlWeahter fun no has parameter\n");
		return -1;
	}
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
     
	QString tmp ;
	QString tmpName;
	QXmlStreamReader xml;
	xml.setDevice(device);
	int currentDay = 0;
	
	while(!xml.atEnd())
	{
		xml.readNext();
		
		if(xml.isStartElement() && xml.name() == "weather")
		{
		      pWeather->clear();
		}
		if(xml.isStartElement() && xml.name() == "city_name" )
		{
			pWeather->city_name= xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement())
		{
			if ( xml.name() == "today_conditions" )
			{
				 currentDay = TODAY;
			}
			else if( xml.name() == "tomorrow_conditions")
			{
				currentDay = TOMORROW;
			}
			else if( xml.name() == "after_tomorrow_conditions")
			{
				currentDay = AFTERTOMORROW;
			}	
			
			if(currentDay == TODAY)
			{
				if (xml.name() == "condition"  )
	                    {
					tmp = xml.attributes().value("data").toString();
					pWeather->condition_data[0] = tmp.toUtf8().data();
					QString weatherType = xml.attributes().value("weather_type").toString();

					if( !weatherType.isEmpty() )
					{
						if( weatherType.toInt() >= 0 && weatherType.toInt() <= 31 )
						{
					
							tmp=WEATHER_FORECAST_ICON_PATH+weatherType+".png";
							pWeather->weather_icon[0] = tmp;
							tmpName = getWeatherName(weatherType);
							pWeather->weather_name[0] = tmpName;
						}
					}
					else
					{
						//tmp = WEATHER_FORECAST_ICON_PATH+QString("nothing.png");
				
						tmp= WEATHER_FORECAST_ICON_PATH+QString("");
						pWeather->weather_icon[0] = tmp;
						tmpName = getWeatherName("-1");
						pWeather->weather_name[0] = tmpName;
					}
	  		       }
				else if(xml.name() == "temp_c")
				{
					tmp = xml.attributes().value("low").toString();
					pWeather->low[0] = tmp.toUtf8().data();

					tmp = xml.attributes().value("high").toString();
					pWeather->high[0] = tmp.toUtf8().data();

					tmp = xml.attributes().value("now").toString();
					pWeather->now = tmp.toUtf8().data();
				}
				else if(xml.name() == "forecast_date")
				{
					tmp = xml.attributes().value("data").toString();
					pWeather->forecast_date = tmp.toUtf8().data();
				}
				else if(xml.name() == "current_date_time")
				{
					tmp = xml.attributes().value("data").toString();
					pWeather->current_time = tmp.toUtf8().data();
				}
				else if(xml.name() == "humidity")
				{
					tmp = xml.attributes().value("data").toString();
					pWeather->humidity = tmp.toUtf8().data();
				}
				else if(xml.name() == "wind_condition")
				{
					tmp = xml.attributes().value("direction").toString();
					pWeather->wind_direction[0] = tmp.toUtf8().data();

					tmp = xml.attributes().value("power").toString();
					pWeather->wind_power[0] = tmp.toUtf8().data();
				}
				else if(xml.name() == "washcar_condition")
				{
					tmp = xml.attributes().value("data").toString();
					pWeather->wash_data[0] = tmp.toUtf8().data();

					tmp = xml.attributes().value("desc").toString();
					pWeather->wash_desc[0] = tmp.toUtf8().data();
				}
			}
			else if(currentDay == TOMORROW)
			{
				if (xml.name() == "condition"  )
	                    {
					tmp = xml.attributes().value("data").toString();
					pWeather->condition_data[1] = tmp.toUtf8().data();
					QString weatherType = xml.attributes().value("weather_type").toString();

					if( !weatherType.isEmpty() )
					{
						if( weatherType.toInt() >= 0 && weatherType.toInt() <= 31 )
						{
		
							tmp= WEATHER_FORECAST_ICON_PATH+weatherType+".png";
							pWeather->weather_icon[1] = tmp;
								tmpName = getWeatherName(weatherType);
							pWeather->weather_name[1] = tmpName;
							
						}
					}
					else
					{
						//tmp = WEATHER_FORECAST_ICON_PATH+QString("nothing.png");
						tmp= WEATHER_FORECAST_ICON_PATH+QString("");
						pWeather->weather_icon[1] = tmp;
						tmpName = getWeatherName("-1");
						pWeather->weather_name[1] = tmpName;
					}
	  		       }
				else if(xml.name() == "temp_c")
				{
					tmp = xml.attributes().value("low").toString();
					pWeather->low[1] = tmp.toUtf8().data();

					tmp = xml.attributes().value("high").toString();
					pWeather->high[1] = tmp.toUtf8().data();
				}
				else if(xml.name() == "wind_condition")
				{
					tmp = xml.attributes().value("direction").toString();
					pWeather->wind_direction[1] = tmp.toUtf8().data();

					tmp = xml.attributes().value("power").toString();
					pWeather->wind_power[1] = tmp.toUtf8().data();
				}
				else if(xml.name() == "washcar_condition")
				{
					tmp = xml.attributes().value("data").toString();
					pWeather->wash_data[1] = tmp.toUtf8().data();

					tmp = xml.attributes().value("desc").toString();
					pWeather->wash_desc[1] = tmp.toUtf8().data();
				}
			}
			else  if(currentDay == AFTERTOMORROW)
			{
			       if (xml.name() == "condition"  )
	                    {
					tmp = xml.attributes().value("data").toString();
					pWeather->condition_data[2] = tmp.toUtf8().data();
					QString weatherType = xml.attributes().value("weather_type").toString();

					if( !weatherType.isEmpty() )
					{
						if( weatherType.toInt() >= 0 && weatherType.toInt() <= 31 )
						{
							tmp= WEATHER_FORECAST_ICON_PATH+weatherType+".png";
							pWeather->weather_icon[2] = tmp;
							tmpName = getWeatherName(weatherType);
							pWeather->weather_name[2] = tmpName;
							
						}
					}
					else
					{
						//tmp = WEATHER_FORECAST_ICON_PATH+QString("nothing.png");
						tmp= WEATHER_FORECAST_ICON_PATH+QString("");
						pWeather->weather_icon[2] = tmp;
						tmpName = getWeatherName("-1");
						pWeather->weather_name[2] = tmpName;
					}
				}
				   else if(xml.name() == "temp_c")
				{
					tmp = xml.attributes().value("low").toString();
					pWeather->low[2] = tmp.toUtf8().data();

					tmp = xml.attributes().value("high").toString();
					pWeather->high[2] = tmp.toUtf8().data();
				}
				   else if(xml.name() == "wind_condition")
				{
					tmp = xml.attributes().value("direction").toString();
					pWeather->wind_direction[2] = tmp.toUtf8().data();

					tmp = xml.attributes().value("power").toString();
					pWeather->wind_power[2] = tmp.toUtf8().data();
				}
				else if(xml.name() == "washcar_condition")
				{
					tmp = xml.attributes().value("data").toString();
					pWeather->wash_data[2] = tmp.toUtf8().data();

					tmp = xml.attributes().value("desc").toString();
					pWeather->wash_desc[2] = tmp.toUtf8().data();
				}
			}
		}
	
        }
	if(currentDay == 0)
	{
		return -1;
	}
	else
		return 0;
}
void BsWeatherForecastC ::run()
{
	int retval = BS_E_OK;
	
	
	BsWeatherForecastInfo weatherInfo;
	DownloadTask		myTask;

	char weatherUrl[URL_MAX_LENGTH];
	char weatherUrlUsed[URL_MAX_LENGTH];
	char apHost[URL_MAX_LENGTH]="app.innov.21com.com";
#if 0
	retval = BsBaseC::getApApiHost(Ap_Weather, apHost);
	if( BS_E_OK != retval )
	{
		printf("--------------------------------%d\n",__LINE__);
		IBD_PRINTF(DEBUG_INFOMATION,"getApApiInfo fail\n")
		m_mutexForThreadSafe.lock();
		m_list.clear();
		m_mutexForThreadSafe.unlock();
		emit sigetWeatherForecastResult(retval, weatherInfo);
		return;
	}
#endif
	//printf("http://%s/ap004/api/1.0/tu/getweather?city=", apHost);
	sprintf(weatherUrl, "http://%s/ap004/api/1.0/tu/getweather?city=", apHost);
	QDir weatherForecastPath;
	char weatherForecastDir[512] = {""};
	sprintf(weatherForecastDir,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,"weatherForecast/Download");
	if(false == weatherForecastPath.mkpath(weatherForecastDir))
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:mkdir path fial\n");
		emit sigetWeatherForecastResult(BS_E_URL_FILE_ERROR, weatherInfo);
		return;
	}
	
	while(1)
	{
		if( m_bExit )
			break;
			
		m_mutexForThreadSafe.lock();
		if( !m_list.isEmpty() )
		{
			myTask = m_list.takeFirst();
		}
		else
		{
			m_mutexForThreadSafe.unlock();
			break;
		}
		m_mutexForThreadSafe.unlock();
		
		CURL *curl = curl_easy_init();
		char weatherFilePath[512] = {""};
		sprintf(weatherFilePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,WEATHER_FORECAST_XMLFEIL);
		QFile weatherFile(weatherFilePath); 
		if (!weatherFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
		{
			emit sigetWeatherForecastResult(BS_E_URL_FILE_ERROR, weatherInfo);
	        	return;
		}

		weatherInfo.clear();
		
		if(curl)
		{
			strcpy(weatherUrlUsed, weatherUrl);
			strcat(weatherUrlUsed, myTask.city.toAscii().data());
			//strcpy(weatherUrlUsed, "http://ap.chinatsp.com/ap004/api/1.0/tu/getweather?city=%E5%8C%97%E4%BA%AC");
			//strcpy(weatherUrlUsed, "http://app.innov.21com.com/ap004/api/1.0/tu/getweather?city=%E5%8C%97%E4%BA%AC");
			//printf("Connecting to %s --> LIne is %d\n", weatherUrlUsed,__LINE__);
			curl_easy_setopt(curl,CURLOPT_URL, weatherUrlUsed);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		   	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
		   	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		   	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	      	 	curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)&weatherFile);
	     		
			CURLcode res = curl_easy_perform(curl);
			
			if (res != CURLE_OK)
			{
				IBD_PRINTF(DEBUG_INFOMATION, "Error: curl error: %s\n",curl_easy_strerror(res));
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(parseXmlWeather(&weatherFile,&weatherInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION, "Error: parse xml error\n");
					retval = BS_E_URL_XML_PARSE_ERROR;
				}
				else
				{
					IBD_PRINTF(DEBUG_INFOMATION, "Success: download weather info from %s\n", weatherUrlUsed);
					retval = BS_E_OK;
				}
			}   
		}
		else
		{
			//printf("error -------------------Connecting to %d -->\n", curl);
			IBD_PRINTF(DEBUG_INFOMATION, "Error: curl_easy_init error\n");
			retval = BS_E_URL_HTTP_ERROR;
		}
		
		emit sigetWeatherForecastResult(retval, weatherInfo);			

		curl_easy_cleanup(curl);

		weatherFile.close();
	}

}

