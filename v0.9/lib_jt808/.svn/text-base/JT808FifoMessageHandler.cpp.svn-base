#include<iostream>
#include<stdlib.h>
#include<string.h>
#include"JT808FifoMessageHandler.h"
#include "JT808MessageHelper.h"
#include "ConnectionManager.h"
#include "ConfigManager.h"
#include "LogUtility.h"


using namespace std;


int analyze_data_to_808(unsigned char* buff)
{
	int id=buff[0];
	switch(id)
	{
		case JT808_FIFO_IC_CARD_INFO:
			handle_ic_card_info(buff);
			break;
		case JT808_FIFO_POSITION_INFO:
			handle_position_info(buff);
			break;
		case JT808_FIFO_TRANSMISSION_INFO:
			handle_transmissionData_info(buff);
		default:
			break;
	}
	return 1;
}
int handle_ic_card_info(unsigned char *buff)
{
	JT808DriverInfoReqC notify;
	memcpy(&notify,buff,sizeof(notify));
	int time =notify.time;
	JT808MessageHelper::SendDriverInfo(time);
	return 1;
}
int handle_position_info(unsigned char *buff)
{
	LocReportNotify notify;
	memcpy(&notify,buff,sizeof(notify));
	JT808MessageHelper::SendLocation(notify.index);
	return 1;
}
int handle_transmissionData_info(unsigned char *buff)
{
	JT808DataTransmitReqC notify;
	memcpy(&notify,buff,sizeof(notify));
	if(notify.type!=0x0B)
	{
		JT808MessageHelper::SendTransmissionData(notify.type, notify.total, notify.buf);
	}
	else
	{
		if( ConfigManager::m_control != 2)
		{
			ConfigManager::m_control = 2;
			ConnectionManager::Instance()->Close();
		}
		while(1){
		if (( ConnectionManager::Instance()->m_connId == ENUM_CONNECTION_ID_IC )&&( ConnectionManager::Instance()->m_connected == true))
		{
			printf("%s::%d trans info to ic server!!!!\n",__FILE__,__LINE__);
			JT808MessageHelper::SendTransmissionData(0x0B,  notify.total, notify.buf);
			break;
		}
		else
			sleep(1);
		}
	}
	return 1;
}
