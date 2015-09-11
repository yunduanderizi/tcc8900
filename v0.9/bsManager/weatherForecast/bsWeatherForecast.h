#ifndef  WEATHER_H_20110221
#define  WEATHER_H_20110221

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"



//data struct used by BsWeatherForecastC
class BsWeatherForecastInfo
{
public:
	QString  city_name;                 // weatherforecast  of  city_name 
	QString  condition_data[3];    //condition_data of weather
	QString  weather_icon[3];     //icon of weather
	QString  weather_name[3];     //name of weather
	QString  low[3];                     //low of weather  temperature
	QString  high[3];                    //high of weather  temperature
	QString  now ;                        //now of today  temperature
	QString  forecast_date ;         //forecast_date of  today weather
	QString  current_time ;           //current_time of  today weather
	QString  humidity ;                  //humidity of  today weathers
	QString  wind_direction[3];     //wind_direction of weather
	QString  wind_power[3];        //wind_power of weather
	QString  wash_data[3];          //wash_data of weather
	QString  wash_desc[3];          //wash_desc of weather

	void clear()
		{
			city_name = "";		
			now ="";
			forecast_date ="";
			current_time ="";
			humidity ="";
	             for(int  i = 0;i<3;i++ )
		      {
			  condition_data[i] = QString();
			  weather_icon[i] = QString();
			  weather_name[i] = QString();
			  low[i] = QString();
			  high[i] = QString();
			  wind_direction[i] = QString();
			  wind_power[i] = QString();
			  wash_data[i] = QString();
			  wash_desc[i] = QString();
		      }	
		 }
};

Q_DECLARE_METATYPE(BsWeatherForecastInfo);

class BsWeatherForecastC : public QThread,  public BsBaseC
{
	Q_OBJECT
	public:
	       //get the object of "BsWeatherForecastC", notice: you can not get the object from "new"
		 static BsWeatherForecastC *object();
	        static BsWeatherForecastC *object1();
	        //release the object of "BsWeatherForecastC"
	         static void releaseObject();
	         static void releaseObject1();
		static BsWeatherForecastC *getObject();
		static BsWeatherForecastC *getObject1();
		  int startGetWeatherForecast();
		  int setCityPinyin(const char *pcity);
	         int setCityUtf8(const char *pcity);
		  void  cancelOperation();
	signals:
		 void sigetWeatherForecastResult(int result, BsWeatherForecastInfo weatherinfo);

	private:
		static BsWeatherForecastC *thisObject;
		static BsWeatherForecastC *thisObject1;
		BsWeatherForecastC();
		~BsWeatherForecastC();
		void run();
		volatile bool m_bExit;
	       QString m_city;
	private slots:
		void slotFinished();	 
	private:
		class DownloadTask
		{
		public:
			QString city;
		};
		typedef QList<DownloadTask> DownloadTaskList;
		DownloadTaskList m_list;
		
		int parseXmlWeather(QFile *file, BsWeatherForecastInfo *pWeather);
		QMutex m_mutexForThreadSafe;
};

#endif
