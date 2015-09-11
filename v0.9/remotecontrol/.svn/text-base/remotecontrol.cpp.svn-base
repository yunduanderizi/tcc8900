#include <stdio.h>
#include <time.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/rtc.h> 
#include <stdlib.h>

#include "DeviceManagerDbusMessage.h"
#include "dbusApplication.h"
#include "dbusClient.h"
#include "dbusServer.h"

#include "curl.h"
#include "remotecontrol.h"
#include "VDRTrackInfoTable.h"

#include "xml_analy.h"

DBusClientC	*device_Server;

static struct RemoteCtrlInfo ctrl_info;
static struct PostInfo  post_info;

#define AT88SC_IOCTL_BASE    'C'
#define AT88SC_DATA            _IOW (AT88SC_IOCTL_BASE, 0, struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID    _IOW (AT88SC_IOCTL_BASE, 1, struct at88sc_ioctl_data)

#define SLEEP_TIME 10
#define REQUEST_NUM 20

#define REMOTE_CTRL_URL "http://api.21com.com/test"
#define BUFFERSIZE 1024
#define FILE_NAME "/tmp/remotecontrol.xml"
#define VERSION_DIR "/application/data/version.ini"
#define L_BUFFER_SIZE 128

struct at88sc_ioctl_data {
    unsigned char rw;
    unsigned char userzone;
    unsigned char password[13];
};

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{//ptr----->stream 
    size = 1;
    char *buf = (char *)stream; 
    strcat(buf, (char *)ptr);
    return nmemb;
}

int curl_upload(char *url, char *send_buffer, char *receive_buffer)
{
    char *ptr = send_buffer;

    if (url == NULL) {
        fprintf(stderr, "No url!\n");
        return -1;
    }

    fprintf(stderr, "*** ADDR upload: %s\n", url);

    CURL *curlhandle;
    
    CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
    if (ret == CURLE_OK) {
        curlhandle = curl_easy_init();
        if (curlhandle == NULL) {
            fprintf(stderr, "Curlhandle is NULL!\n");
            return -1;
        }
        curl_easy_setopt(curlhandle, CURLOPT_URL, url); 

        if (strlen(send_buffer) == 0) {
            fprintf(stderr, "No data to upload in the host!\n");
            return -1;
        }
        curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, ptr);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, receive_buffer);

        fprintf(stderr, "*** Into curl_easy_perform upload buffer! ***\n");
        ret = curl_easy_perform(curlhandle);
        if (ret != CURLE_OK) {
            fprintf(stderr, "Err: %s\n", curl_easy_strerror(ret));
            return ret;
        }	
        fprintf(stderr, "*** Out curl_easy_perform upload buffer! ***\n");

        curl_easy_cleanup(curlhandle);
        curl_global_cleanup();
    }
    else 
    {
         fprintf(stderr, "Err: %s\n", curl_easy_strerror(ret));
         return ret;
    }
    return 0;
}

int  read_tuid_from_at88sc(unsigned char *tuid)
{
    int fd;
    unsigned char passbuf[13]={'i','N','N','O','V',8,8,8,9,2,8,7,5};
    struct at88sc_ioctl_data ioctldata;
    printf("read tuid from at88sc ...\n");
    fd = open("/dev/at88sc", O_RDONLY);
    if(fd < 0)
    {
        cerr << "Failed to open tuid device." << endl;
        return -1;
    }

    ioctldata.rw = 1;
    ioctldata.userzone = 1;
    memcpy(ioctldata.password, passbuf, 13);

    if(ioctl(fd, AT88SC_DATA, &ioctldata) < 0)
    {
        cerr << "Failed to control tuid device." << endl;
        close(fd);
        return -1;
    }

    if(read(fd, tuid, 32) != 32)
    {
        cerr << "Failed to read device." << endl;
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int read_tuid_from_emmc(char * tuid)
{
	FILE* fp = NULL; 
    int ret;
    
    fp = fopen("/nand2/tuid.conf", "r");
    if(NULL == fp)
    {
        cerr << "Failed to open nor flash tuid.conf ." << endl;
        return -1;
    }
    printf("read tuid from nor flash ...\n");
    fgets(tuid, 33, fp);
    fclose(fp);
    return 0;
}

int read_innov_tuid(unsigned char *buf)
{
    int ret= 0;
    
    ret = read_tuid_from_emmc((char *)buf);
    if(ret < 0)
    {
        ret = read_tuid_from_at88sc(buf);
        if(ret < 0)
        {
            cout <<"read tuid from emmc at88sc error.\n" <<endl;
            return -1;
        }
    }
    return 0;
}

int get_version_model()
{
    int ret = -1;
    int len = 0;
    char *ptr;
    FILE *fp = NULL;
    char file_name[L_BUFFER_SIZE];
    char tmp_str[L_BUFFER_SIZE];
    char match_str[2][32] = {"Version = ", "Model = "};
    
    strcpy(file_name, VERSION_DIR);
    if((fp = fopen(file_name, "r")) == NULL)
    {
        perror("fopen()");
        return -1;
    }

    while(1)
    {
        if(fgets(tmp_str, L_BUFFER_SIZE, fp) == NULL)
            break;

        for(int i = 0; i < 2; i++)
        {
            if((ptr = strstr(tmp_str, match_str[i])) != NULL)
            {
                ptr = ptr + strlen(match_str[i]);
                len = strlen(tmp_str) - strlen(match_str[i]) - 1;
                switch(i)
                {
                    case 0:
                        strncpy(post_info.version, ptr, len);
                        break;
                    case 1:
                        strncpy(post_info.vdr_model, ptr, len);
                        break;
                }
                break;
            }
        }            
    }    
    fclose(fp);
   
    return 0;
}

int read_locstatus_and_version()
{
    int status = 0;
    VDRLocStatus1Table *loc_table = new VDRLocStatus1Table();
    int index = loc_table->RefreshBottom();
    if(index < 0)
    {
        delete loc_table;
        return -1;
    }
    else
    {
        loc_table->GetIntValue(VDR_LOCS_STATUS_FLAG, status);
        loc_table->GetIntValue(VDR_LOCS_OCCUR_TIME, post_info.itime);
        loc_table->GetIntValue(VDR_LOCS_LATITUDE, post_info.ilat);
        loc_table->GetIntValue(VDR_LOCS_LONGITUDE, post_info.ilng);
        loc_table->GetIntValue(VDR_LOCS_ALTITUDE, post_info.ialti);
        delete loc_table; 
		
        if(status & 0x02)
            post_info.position = 1;
        else
            post_info.position = 0;
		
        if(status & 0x04)
            post_info.lat_flag = 1;
        else
            post_info.lat_flag = 0;
		
        if(status & 0x08)
            post_info.lng_flag = 1;
        else
            post_info.lng_flag = 0;
    }	

    if(get_version_model() < 0)
        return -1;
    
    return 0;
}

int  send_shutdown_dbus_request()
{
    ShutdownMachineReqC srequest;
    DBusReplyC  *reply=NULL;
    int ret = -1;

    ret = device_Server->sendService(&srequest,reply,5*1000);
    if(ret < 0)
    {
        printf("Send Service Error. Error Code: %d\n",ret);
    }
    return ret;
}

void init_remote_control_info(struct RemoteCtrlInfo *info)
{
    info->sendpolicy = 0;
    info->sendnum = 1;
    info->interval = 300;
    info->startpolicy = 0;
}

int create_send_buffer(char *send_buffer)
{
    char str_tmp[100];

    memset(send_buffer, 0, BUFFERSIZE);
    
    snprintf(send_buffer, 64+1, "device_id=%s&", post_info.device_id );

    snprintf(str_tmp, 3*sizeof(int)+1, "lng=%d&", post_info.ilng);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 3*sizeof(int)+1, "lat=%d&", post_info.ilat);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 3*sizeof(int)+1, "alti=%d&", post_info.ialti);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 5*sizeof(int)+1, "time=%d&", post_info.itime);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 3*sizeof(int)+1, "lng_flag=%d&", post_info.lng_flag);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 3*sizeof(int)+1, "lat_flag=%d&", post_info.lat_flag);
    strcat(send_buffer, str_tmp);	

    snprintf(str_tmp, strlen(post_info.vdr_model) + strlen(post_info.version) + 10, 
        "version=%s %s", post_info.vdr_model, post_info.version);
    strcat(send_buffer, str_tmp);

    printf("Sendbuffer:%s\n", send_buffer);

    return 0;	
}

int analy_return_ctrl_info(char *receive_buffer) 
{

    int data[4] = {0};
    FILE *fp;
    char *ptr = receive_buffer;
    int i = 0;
    int count_tmp = 0;
    fp = fopen(FILE_NAME,"w");

    /* receive_buffer------>standard xml */
    for(i = 0; ptr[i] != '\0'; i++)
    {
        if(ptr[i] == '<' )
        {
            count_tmp++;
            if(count_tmp == 2)
                break;
        }
    }
    fwrite(ptr, 1, i, fp);
    fwrite("\n<setting>\n", 1, 11, fp);
    fwrite(ptr+i, 1, strlen(ptr)-i-1, fp);
    fwrite("\n</setting>\r\n", 1, 13, fp);
    fclose(fp);

    xml_analy( data, FILE_NAME, 4);
    ctrl_info.interval = data[2];
    ctrl_info.sendnum = data[1];
    ctrl_info.sendpolicy = data[0];
    ctrl_info.startpolicy = data[3];

    return 0;
}

int send_first_request (char *receive_buffer) 
{
    int count_fr = 0;
    int ret = -1;
    char send_buffer[BUFFERSIZE];
    char *ptr = send_buffer;
		
    do
    {
        count_fr++;
        if ( read_locstatus_and_version() != 0 )
        {
            fprintf(stderr, "Read track locstatus table failed !\n");
            return -1;
        }
        if ( create_send_buffer(send_buffer) != 0)
        {
            fprintf(stderr, "Create send buffer Failed!\n");
            return -1;
        } 
        memset(receive_buffer, 0, BUFFERSIZE); 
        ret = curl_upload(REMOTE_CTRL_URL, send_buffer, receive_buffer); 
        if(ret == CURLE_OK)
        {			
            ptr = receive_buffer;
            analy_return_ctrl_info(ptr);
            return 0;
        }
        
        sleep(SLEEP_TIME);
        if(count_fr == REQUEST_NUM)
        {
            fprintf(stderr, "First request send %d times!All failed!\n", count_fr);
            return ret;
        }			
    }while(1);
}

int send_vdr_request(char *receive_buffer)
{
    int ret = -1;
    char send_buffer[BUFFERSIZE];

    if ( create_send_buffer(send_buffer) != 0)
    {
        fprintf(stderr, "Create send buffer Failed!\n");
        return -1;
    }    
    memset(receive_buffer, 0, BUFFERSIZE); 
    ret = curl_upload(REMOTE_CTRL_URL, send_buffer, receive_buffer); 
    if( ret == CURLE_OK )
    {
        analy_return_ctrl_info(receive_buffer);
        sleep(ctrl_info.interval);
        return 0;
    }
    sleep(ctrl_info.interval);

    return ret;
}

int main(int argc, char **argv)
{
    DBusApplicationC app(argc, argv);

    int ret = -1;
    unsigned char tuid[33];

    memset(&ctrl_info,0,sizeof(struct RemoteCtrlInfo));
    memset(&post_info,0,sizeof(struct PostInfo));
    memset(tuid, 0, 33);

    device_Server= new DBusClientC(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
    if(device_Server->connect() < 0)
    {
        printf("Cann't connect to device_Server DBUS server.\n");
        return -1;

    }

    init_remote_control_info(&ctrl_info);
    ret = read_innov_tuid(tuid);
    if(ret<0)
        return -1;
    else
        memcpy(post_info.device_id,tuid,32);

    fprintf( stdout,"device_id = %s\n",post_info.device_id );
    
    char receive_buffer[BUFFERSIZE] = ""; 
    int icount = 0; 
    int vdr_fail_times = 0;
    
    ret = send_first_request(receive_buffer);
    if( ret != 0 )
        return -1;

    do
    {                   
        if( ctrl_info.startpolicy == 1 )
        {                       
            send_shutdown_dbus_request(); 
            return 0;	
        }
        if( ctrl_info.sendpolicy == 1 )
        {
            return -1;
        }	
        if ( read_locstatus_and_version() != 0 )
        {
            fprintf(stderr, "Read track locstatus table failed !\n");
            return -1;
        }
      if(post_info.position == 0)
        {
            fprintf(stderr, "Locate failed! Try again! \n");
            if(ctrl_info.interval < 3)
                sleep(3);
            else
                sleep( ctrl_info.interval );
            
            continue;
        }
                               
        ret = send_vdr_request(receive_buffer);
        if( ret != CURLE_OK )
        {
            fprintf(stderr, "Send vdr request failed!\n");
            vdr_fail_times++;
            if(vdr_fail_times == REQUEST_NUM)
                return -1;
        }
        else
        {
            icount++;             
        }
        
        if( ctrl_info.sendnum == 0 )
        {
            icount = 0; 
            continue;
        }
    }while( icount <= ctrl_info.sendnum );

    //return app.loop();
    return 0;
}
