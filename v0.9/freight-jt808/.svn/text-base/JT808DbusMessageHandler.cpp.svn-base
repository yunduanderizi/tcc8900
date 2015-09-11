#include <dbusClient.h>
#include "JT808DbusMessage.h"
#include "JT808MessageHelper.h"
#include "JT808DbusMessage.h"
#include "LogUtility.h"
#include "VDRRunParaTable.h"
#include "VDRDeployParaTable.h"
#include "ConnectionManager.h"
#include "DeviceManagerDbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "NaviDbusMessage.h"
#include "ConfigManager.h"
#include "zlib.h"
#include <stdlib.h>
#include <map>
#define NHB
using namespace std;
extern JT808MessageHandler jt808;

extern DBusClientC     *g_pDbusClient;
extern DBusServerC    *g_pDbusServer;
extern int wire_state;

void JT808RegisterServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{

    string phonenumber = "";
    JT808RegisterDbusReqC   *pReq = (JT808RegisterDbusReqC *)request;
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808RegisterServiceHandler get register request %d ", pReq->result);
    //if(ConnectionManager::Instance()->m_connected == false ) return ;
    if(pReq -> result == 0)//注销
    {
#if 0
        string auth = "";
        VDRDeployParaTable *table = new VDRDeployParaTable();
        if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
            delete table;
            return;
        }
        table->Commit();
        delete table;
#endif

        JT808MessageHelper::SendLogout();
        //sleep(3);
        //ConnectionManager::Instance()->Reconnect();

    }else if(pReq -> result == 1){
    printf("****************** result = 1\n");
	//printf("******************* strlen(pReq->buf) = %d \n",strlen(pReq->buf)); 
	    ConnectionManager::Instance()->Start();	
        JT808MessageHelper::SendRegister(1, pReq->buf);
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }else if(pReq -> result == 2 || pReq -> result == 12){
    printf("***************** result = 2 || 12\n");
        if( ConfigManager::m_control != 3)
        {
            ConfigManager::m_control = 3;
            ConnectionManager::Instance()->Close();
        }
        while(1){
            if (( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_IC )&&( ConnectionManager::Instance()->m_connected == true))
            {
                printf("%s::%d %s trans info to lock before using!!!!\n", __FILE__, __LINE__, pReq->buf);
                JT808MessageHelper::SendRegister(pReq -> result, pReq -> buf);
                break;
            }else
                sleep(1);
        }
    }else if(pReq -> result == 3){
    printf("******************** result = 3\n");
        int count;
        char buf_file[64] = "good.jpg";
        char buf_read[1024] = {0};
        strcat(buf_file, ".gz");
        strcpy((char *)(pReq->buf), buf_file);
        int src_fp = open("good.jpg", O_RDONLY);
        if(src_fp < 0)
        {
            printf("open file error!\n");
            return ;
        }

        gzFile gDesc = gzopen(buf_file, "wb");
        while((count = read(src_fp, buf_read, 512)) > 0 )
        {
            gzwrite(gDesc,buf_read,count);
        }
        gzclose(gDesc);

        close(src_fp);
        JT808MessageHelper::SendGzipPacket( pReq -> buf );
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }else if(pReq -> result == 5){
#if 0
        RebootMachineReqC   rrequest;
        DBusReplyC  *reply=NULL;
        int ret = g_pDbusClient->sendService(&rrequest,reply,5*1000);
        if(ret < 0)
        {
            printf("Send Service Error. Error Code: %d\n",ret);
        }
#endif 
        JT808MessageHelper::SendAck(1, (unsigned short)JT808_PACKET_TERM_CONTROL, (int)ENUM_JT808_COMMON_OK);
        sleep(3);
        LogUtility::Log(LOG_LEVEL_ERROR, "-------------telephone cmd reset factory");
        system("cp /nand2/tables/vdrsetting.db.bak /nand2/tables/vdrsetting.db");
        SendJT808CallBackNotify( 4, phonenumber);
        ConfigManager::Refresh();
        jt808.ResetFactory();
        SendJT808SettingUpdateNotify();
        JT808ParkingDbusNotifyC notify;
        notify.flag = 6;
        g_pDbusServer->sendNotify(&notify);
        ConnectionManager::Instance()->Close();
        LogUtility::Log(LOG_LEVEL_ERROR, "-------------telephone cmd reset factory complete");
    }else if(pReq -> result == 4){
        JT808MessageHelper::SendRegister(2, pReq -> buf);
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }else if(pReq -> result == 6){
    printf("*********************** result = 6\n");
        JT808MessageHelper::SendRegister(1, pReq -> buf);
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }else if(pReq -> result == 7){
     printf("*********************** result = 7\n");
        JT808MessageHelper::SendRegister(2, pReq -> buf);
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }
    else if(pReq -> result == 8)
    {
     printf("*********************** result = 8\n");
        JT808MessageHelper::SendRegister(1, pReq -> buf);
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }
    else if(pReq -> result == 11)
    {
        printf("*********************** result =11\n");
        wire_state =2;
    	ConnectionManager::Instance()->b_zone = true;
      
        ConnectionManager::Instance()->Stop();
        //system("./ppp-off &");
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }
    else if(pReq -> result == 13)
    {
     printf("*********************** result =13\n");
        ConnectionManager::Instance()->b_zone = false;
        //system("./ppp-on &");
        //ConnectionManager::Instance()->Start();
        wire_state =1;
		
    	LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808AuthenticationServiceHandler wire_state  register request %d", wire_state);
        JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
        reply = pReply;
    }
    return;
}

void JT808AuthenticationServiceHandler(DBusRequestC *, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808AuthenticationServiceHandler get register request");
    JT808MessageHelper::SendAuth();

    JT808AuthenticationDbusRepC *pReply=new JT808AuthenticationDbusRepC(); 
    reply = pReply;

    return;
}
void JT808LocReportServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    JT808LocReportDbusReqC *pReq = (JT808LocReportDbusReqC *)request;
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808LocReportServiceHandler get loc report request %d\r\n",pReq->m_tableIndex);

    JT808MessageHelper::SendLocation(pReq->m_tableIndex);

    JT808LocReportDbusRepC *pReply = new JT808LocReportDbusRepC();
    reply = pReply;

    return;
}

void JT808AnswerServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808AnswerServiceHandler get answer request");
    JT808AnswerDbusReqC *pReq = (JT808AnswerDbusReqC *)request;
    
    JT808MessageHelper::SendAnswer((unsigned short)pReq->m_seq, (unsigned short)pReq->m_answer_id);

    JT808AnswerDbusRepC *pReply = new JT808AnswerDbusRepC();
    reply = pReply;

    return;
}

void JT808ModifySettingServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    VDRRunParaTable *runtable = new VDRRunParaTable();
    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
    int flag1 = false, flag2 = false;
    int ret = 0;
    int err = 0;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808ModifySettingServiceHandler get modify setting request");
    JT808ModifySettingReqC *pReq = (JT808ModifySettingReqC *)request;

    if(pReq->m_main_server)
    {
        string main_server = pReq->m_main_server;
        string old_main_server;
        runtable->GetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, old_main_server);
        if(main_server != old_main_server)
        {
            runtable->SetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, main_server);
            flag1 = true;
        }
    }

    if(pReq->m_second_server)
    {
        string second_server = pReq->m_second_server;
        string old_second_server;
        runtable->GetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, old_second_server);
        if(second_server != old_second_server)
        {
            runtable->SetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, second_server);
            flag1 = true;
        }
    }

    if(pReq->m_port > 0)
    {
        int port = pReq->m_port;
        int old_port;
        runtable->GetIntValue((int)VDR_PARA_TCP_PORT, old_port);
        if(port != old_port)
        {
            runtable->SetIntValue((int)VDR_PARA_TCP_PORT, port);
            flag1 = true;
        }
    }

    if(pReq->m_port2 > 0)
    {
        int port2 = pReq->m_port2;
        int old_port2;
        runtable->GetIntValue((int)VDR_PARA_UDP_PORT, old_port2);
        if(port2 != old_port2)
        {
            runtable->SetIntValue((int)VDR_PARA_UDP_PORT, port2);
            flag1 = true;
        }
    }
    
    if(flag1)
    {
        ret = runtable->Commit();
        if(ret < 0)
        {
            err = 1;
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808ModifySettingServiceHandler commit runtable failed");
        }
    }

    if(pReq->m_auth)
    {
        string auth = pReq->m_auth;
        string old_auth;
        deploytable->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, old_auth);
        if(auth != old_auth)
        {
            deploytable->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth);
            flag2 = true;
        }
    }

    if(flag2)
    {
        ret = deploytable->Commit();
        if(ret < 0)
        {
            err = 1;
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808ModifySettingServiceHandler commit deploytable failed");
        }
    }

    JT808ModifySettingDbusRepC *pReply = new JT808ModifySettingDbusRepC();
    pReply->m_result = err;
    reply = pReply;

    delete runtable;
    delete deploytable;
    
    if(flag1 || flag2)
    {
        ConnectionManager::Instance()->Reconnect();
    }
    
    return;
}

void JT808DriverInfoServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808DriverInfoServiceHandler get driver info request");
    JT808DriverInfoReqC *pReq = (JT808DriverInfoReqC *)request;
    printf("JT808DriverInfoServiceHandler   ::           pReq->m_time_index===%d \n           ",pReq->m_time_index);
// #if 1
#if 0

    //turn on when go to beisanhuan
    //20130802


    while(1){
        if ((jt808.m_authorized == true)&&( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_MASTER ))
        {
            printf("JT808DriverInfoServiceHandler::    enter   while (1) \n");
            JT808MessageHelper::SendDriverInfo(pReq->m_time_index);
            return;
        }
        else
            sleep(1);
    }
#endif
    JT808MessageHelper::SendDriverInfo(pReq->m_time_index);
    
    JT808DriverInfoDbusRepC *pReply = new JT808DriverInfoDbusRepC();
    reply = pReply;
    
    return;
}

void JT808CameraServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808CameraServiceHandler get camera request");
    JT808CameraDbusReqC *pReq = (JT808CameraDbusReqC *)request;

    JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_PHOTO, pReq->m_tableIndex, pReq->trans_flag,pReq->stop_time);

    JT808CameraDbusRepC *pReply = new JT808CameraDbusRepC();
    reply = pReply;
    
    return;
}

void JT808EventServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    static unsigned char up_event[]={'1','2','3','4'}; 
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808EventServiceHandler request");
    JT808CameraDbusReqC *pReq = (JT808CameraDbusReqC *)request;

    JT808MessageHelper::SendEvents(up_event, 4);
    
    return;
}

void JT808VideoServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808CameraServiceHandler get camera request");
    JT808VideoDbusReqC *pReq = (JT808VideoDbusReqC *)request;

    JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_VIDEO, pReq->m_tableIndex,1,0);
    
    return;
}

void JT808RecordServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808RecordServiceHandler get record request");
    JT808RecordDbusReqC *pReq = (JT808RecordDbusReqC *)request;

    JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_AUDIO, pReq->m_tableIndex, 1,0);
    
    JT808RecordDbusRepC *pReply = new JT808RecordDbusRepC();
    reply = pReply;
    
    return;
}

void JT808TransmissionDataServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    JT808DataTransmitReqC *pReq = (JT808DataTransmitReqC *)request;
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808TransmitServiceHandler get record request::%d ", pReq->type);
    if( pReq->type == 0xF1 ){
        if(pReq->total == 0){
            //wire_state =1;
        }else if(pReq->total == 1){
            //wire_state =2;
        }
    }else if( pReq->type == 0xF0 ){
        //JT808MessageHelper::SendElecBillInfo();
    }else if( pReq->type == 0x41 ){
        JT808MessageHelper::SendTransmissionData(pReq->type, pReq->total, pReq->buf);
    }else if( pReq->type != 0x0B ){
        JT808MessageHelper::SendTransmissionData(pReq->type, pReq->total, pReq->buf);
    }else{
#if 1
        if( ConfigManager::m_control != 2)
        {
            ConfigManager::m_control = 2;
            ConnectionManager::Instance()->Close();
        }
        while(1){
            if (( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_IC )&&( ConnectionManager::Instance()->m_connected == true))
            {
                printf("%s::%d trans info to ic server!!!!\n",__FILE__,__LINE__);
                JT808MessageHelper::SendTransmissionData(0x0B,  pReq->total, pReq->buf);
                break;
            }else
                sleep(1);
        }

#endif
#if 0
        try{
            int i;
            TCPSocket sock(ConfigManager::m_icmainServerAddress.c_str(),ConfigManager::m_ictcpPort);
            StruJT808Packet *thepacket = jt808.CreatePacket();
            JT808Packet &packet = thepacket->packet;
            jt808.PushByte(packet, 0x0B);
            for(i=0;i<pReq->total;i++)
                jt808.PushByte(packet, pReq->buf[i]);
            jt808.PacketHead(thepacket, (unsigned short)JT808_PACKET_UP_TRANSPARENT_TRANSMISSION, JT808_CRYTO_NONE);
            jt808.PacketCheckSumFlag(thepacket);
            JT808DataTransmitNotifyC notify;
            sock.send(pReq->buf, pReq->total);
            notify.type = 0x0B;
            notify.total= sock.recv(notify.buf,sizeof(notify.buf)-1);
            if(notify.total > 0)
                g_pDbusServer->sendNotify(&notify);
        }catch(SocketException &e){
            cerr << e.what() <<endl;
        }
#endif
    }
    return;
}

void SendJT808RegisterNotify(int result)
{
    JT808RegisterDbusNotifyC notify;
    
    notify.result = result;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808AuthenticationNotify(int result)
{
    JT808AuthenticationDbusNotifyC notify;
    
    notify.result = result;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808SettingUpdateNotify()
{
    JT808SettingUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808GnssUpdateNotify(int mode,int baud,int out,int fre)
{
    CGNSSAttrChangeNotifyC notify;
    notify.mode=mode;
    notify.baudrate=baud;
    notify.outfrequency=out;
    notify.samplerate=fre;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808LocReportNotify(int confirm)
{
    JT808LocReportDbusNotifyC notify;
    notify.confirm_alarm=confirm;
    g_pDbusServer->sendNotify(&notify);
    return;
}

void SendJT808TempLocReportNotify(int interval,int validity)
{
    JT808TempLocReportDbusNotifyC notify;

    notify.interval=interval;
    notify.validity=validity;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808TextNotify(int emergency, int lcd, int tts, int bigscreen, int naviorcan, string &text)
{
    JT808TextDbusNotifyC notify;
    
    notify.m_emergency = emergency;
    notify.m_lcd = lcd;
    notify.m_tts = tts;
    notify.m_bigscreen = bigscreen;
    notify.m_naviorcan= naviorcan; 
    notify.m_text = (char *)malloc(text.length() + 1);
    
    text.copy(notify.m_text, text.length());
    notify.m_text[text.length()] = '\0';
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808QuestionNotify(int seq, int emergency, int lcd, int tts, int bigscreen, string &text, map<int, string> &answers)
{
    JT808QuestionDbusNotifyC notify;
    
    notify.m_seq = seq;
    notify.m_emergency = emergency;
    notify.m_lcd = lcd;
    notify.m_tts = tts;
    notify.m_bigscreen = bigscreen;
    notify.m_text = (char *)malloc(text.length() + 1);
    text.copy(notify.m_text, text.length());
    notify.m_text[text.length()] = '\0';
    
    int answerNumber = answers.size();
    if(answerNumber > JT808_MAX_QUESTION_NUMBER)
    {
        answerNumber = JT808_MAX_QUESTION_NUMBER;
    }
    
    notify.m_answerNumber = answerNumber;
    int count = 0;
    
    for(map<int, string>::iterator it = answers.begin(); it != answers.end(); it ++, count ++)
    {
        if(count == 0)
        {
            notify.m_answer1_id = it->first;
            string &answer = it->second;
            notify.m_answer1 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer1, answer.length());
            notify.m_answer1[answer.length()] = '\0';
        }
        else if(count == 1)
        {
            notify.m_answer2_id = it->first;
            string &answer = it->second;
            notify.m_answer2 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer2, answer.length());
            notify.m_answer2[answer.length()] = '\0';
        }
        else if(count == 2)
        {
            notify.m_answer3_id = it->first;
            string &answer = it->second;
            notify.m_answer3 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer3, answer.length());
            notify.m_answer3[answer.length()] = '\0';
        }
        else if(count == 3)
        {
            notify.m_answer4_id = it->first;
            string &answer = it->second;
            notify.m_answer4 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer4, answer.length());
            notify.m_answer4[answer.length()] = '\0';
        }
    }
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808CallBackNotify(int monitor, string &phone)
{
    JT808CallBackDbusNotifyC notify;
    
    notify.m_monitor = monitor;
    notify.m_phone = (char *)malloc(phone.length() + 1);
    
    phone.copy(notify.m_phone, phone.length());
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808ContactsUpdateNotify()
{
    JT808ContactsUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808FenceUpdateNotify(int flag,int type,int id,int state)
{
    CNaviElectronicFenceNotifyC notify;
    
    notify.eflag=flag;
    notify.etype=type;
    notify.eid=id;
    notify.estate=state;

    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808CircleFenceUpdateNotify()
{
    JT808CircleFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808RectangleFenceUpdateNotify()
{
    JT808RectangleFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808PolygonFenceUpdateNotify()
{
    JT808PolygonFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808LinesFenceUpdateNotify()
{
    JT808LinesFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808ConncetionStatusNotify(int main_status, int second_status)
{
    JT808ConnStatusDbusNotifyC notify;
    
    notify.m_main_status = main_status;
    notify.m_second_status = second_status;
    
    g_pDbusServer->sendNotify(&notify);

    return;
}

void SendJT808CameraStartNotify(StruCameraCommand &command)
{
    printf("camera :::::::::::::::: flag is %d\n ",command.flag);
    JT808CameraDbusNotifyC notify;
    
    notify.m_channel = command.chan;
    notify.m_command = command.command;
    notify.m_interval = command.interval;
    notify.m_flag = command.flag;
    notify.m_resolution = command.resolution;
    notify.m_quality = command.quality;
    notify.m_brightness = command.brightness;
    notify.m_saturation = command.saturation;
    notify.m_chroma = command.chroma;
    
    g_pDbusServer->sendNotify(&notify);

    return;
}

void SendJT808RecordStartNotify(StruRecordCommand &command)
{
    JT808RecordDbusNotifyC notify;
    
    notify.m_command = command.command;
    notify.m_time = command.duration;
    notify.m_flag = command.flag;
    notify.m_sample = command.sample;
    
    g_pDbusServer->sendNotify(&notify);

    return;
}
