#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "curl.h"
#include "xml_analys.h"
#include "uploadUpdateInfo.h"
#include "VDRDeployParaTable.h"

static struct updateInfo post_info;

#define AT88SC_IOCTL_BASE    'C'
#define AT88SC_DATA          _IOW (AT88SC_IOCTL_BASE, 0, struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID    _IOW (AT88SC_IOCTL_BASE, 1, struct at88sc_ioctl_data)

#define UPLODA_URL "http://api.21com.com/service"
#define BUFFERSIZE 1024
#define LITTLE_BUFFER 256
#define RESEND_TIMES 2
#define PRINT_DEBUG_INFO 1

static int file_no = 0;//当前处理的文件编号
char file_dir[LITTLE_BUFFER] = "";

struct at88sc_ioctl_data {
    unsigned char rw;
    unsigned char userzone;
    unsigned char password[13];
};

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{ 
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
#ifdef PRINT_DEBUG_INFO
    fprintf(stdout, "=== ADDR upload: %s ===\n", url);
#endif

    CURL *curlhandle;
    
    CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
    if (ret == CURLE_OK) {
        curlhandle = curl_easy_init();
        if (curlhandle == NULL) {
            fprintf(stderr, "Err: Curlhandle is NULL!\n");
            return -1;
        }
        curl_easy_setopt(curlhandle, CURLOPT_URL, url); 

        if (strlen(send_buffer) == 0) {
            fprintf(stderr, "Err: No data to upload in the host!\n");
            return -1;
        }
        curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, ptr);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, receive_buffer);
#ifdef PRINT_DEBUG_INFO
        fprintf(stdout, "=== Into curl_easy_perform upload buffer! ===\n");
#endif
        ret = curl_easy_perform(curlhandle);
        if (ret != CURLE_OK) {
            fprintf(stderr, "Err: %s\n", curl_easy_strerror(ret));
            return ret;
        }	
#ifdef PRINT_DEBUG_INFO
        fprintf(stdout, "=== Out curl_easy_perform upload buffer! ===\n");
#endif
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

int read_innov_tuid(unsigned char *buf)
{
    int fd;
    static unsigned char passbuf[13]={'i','N','N','O','V',8,8,8,9,2,8,7,5};
    struct at88sc_ioctl_data ioctldata;

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

    if(read(fd, buf, 32) != 32)
    {
        cerr << "Failed to read device." << endl;
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

int get_update_info()
{
    int ret = -1;
    int len = 0;
    char *ptr;
    FILE *fp = NULL;
    unsigned char tuid[32];
    char file_name[LITTLE_BUFFER];
    char tmp_str[LITTLE_BUFFER];
    char match_str[7][32] = {"Old Version = ", "Start = ", "New Version = ", 
        "End = ", "Result = ", "Old Model = ", "New Model = "};
    
    ret = read_innov_tuid(tuid);
    if(ret < 0)
        return -1;
    else
        memcpy(post_info.device_id, tuid, 32);
    
    strcpy(file_name, file_dir);
    sprintf(tmp_str, "updateinfo%d.txt", file_no);
    strcat(file_name, tmp_str);

    if((ret = access (file_name, F_OK)) != 0)
    {
        fprintf(stderr, "Err: %s do not exists!\n", file_name);
        return -1;
    }

    if((fp = fopen(file_name, "r")) == NULL)
    {
        perror("fopen()");
        return -2;
    }

    while(1)
    {
        if(fgets(tmp_str, LITTLE_BUFFER, fp) == NULL)
            break;

        for(int i = 0; i < 7; i++)
        {
            if((ptr = strstr(tmp_str, match_str[i])) != NULL)
            {
                ptr = ptr + strlen(match_str[i]);
                len = strlen(tmp_str) - strlen(match_str[i]) - 1;
                switch(i)
                {
                    case 0:
                        strncpy(post_info.old_version, ptr, len);
                        break;

                    case 1:
                        strncpy(post_info.start, ptr, len);
                        break;
                        
                    case 2:
                        strncpy(post_info.new_version, ptr, len);
                        break;
                        
                    case 3:
                        strncpy(post_info.end, ptr, len);
                        break;
                        
                    case 4:
                        strncpy(post_info.result, ptr, len);
                        break;
                        
                    case 5:
                        strncpy(post_info.old_model, ptr, len);
                        break;
                    case 6:
                        strncpy(post_info.new_model, ptr, len);
                        break;
                }
                break;
            }
        }            
    }    
    fclose(fp);
    
    return 0;
}

int create_send_buffer(char *send_buffer)
{
    char str_tmp[128];

    memset(send_buffer, 0, BUFFERSIZE);
    
    snprintf(send_buffer, 128, "device_id=%s&", post_info.device_id);

    snprintf(str_tmp, 128, "start=%s&", post_info.start);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 128, "end=%s&", post_info.end);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 128, "ex_version=%s %s&", post_info.old_model, post_info.old_version);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 128, "cu_version=%s %s&", post_info.new_model, post_info.new_version);
    strcat(send_buffer, str_tmp);

    snprintf(str_tmp, 128, "status=%s", post_info.result);
    strcat(send_buffer, str_tmp);
#ifdef PRINT_DEBUG_INFO
    fprintf(stdout, "SendBuffer: %s\n", send_buffer);
#endif

    return 0;	
}

int send_update_info(char *receive_buffer)
{
    int ret = -1;
    char send_buffer[BUFFERSIZE];

    if (create_send_buffer(send_buffer) != 0)
    {
        fprintf(stderr, "Err: Create send buffer Failed!\n");
        return -1;
    }    
    memset(receive_buffer, 0, BUFFERSIZE); 
    ret = curl_upload(UPLODA_URL, send_buffer, receive_buffer); 
    if(ret == CURLE_OK)
    {
        fprintf(stdout, "Info: Send upload info successful!\n");
        return 0;
    }

    return ret;
}

int check_file_num()
{
    char file_name[LITTLE_BUFFER];
    char match[32] = "updateinfo";
    int max_no = -1;
    int tmp_no = -1;
    DIR *p_dir;
    struct dirent *p_dirent;
    
    strcpy(file_name, file_dir);
    file_no = 0;
    if((p_dir = opendir(file_name)) == NULL)
    {
        fprintf(stderr, "Err: open %s failed!\n", file_name);
        return -1;
    }
    
    while((p_dirent = readdir(p_dir)))
    {
#ifdef PRINT_DEBUG_INFO
        printf("%s\n", p_dirent->d_name);
#endif
        if(strncmp(p_dirent->d_name, match, strlen(match)) == 0)
        {
            char tmp[10] = "";
            char *ptr = NULL;
            int len = 0;

            ptr = p_dirent->d_name + strlen(match);
            len = strlen(p_dirent->d_name) - strlen(match) - strlen(".txt");
            strncpy(tmp, ptr, len);
#ifdef PRINT_DEBUG_INFO
            fprintf(stdout, "ptr = %s, tmp = %s, len = %d\n", ptr, tmp, len);
#endif
            tmp_no = atoi(tmp);
        }
        else
            continue;
        
        if(tmp_no > max_no)
            max_no = tmp_no;

        if(tmp_no < file_no)
            file_no = tmp_no;
    }
    closedir(p_dir);
#ifdef PRINT_DEBUG_INFO
    printf("file_no = %d, max_no = %d\n", file_no, max_no);
#endif
    return max_no;
}

int rm_has_processed_file()
{
    char tmp_name[LITTLE_BUFFER];
    char rm_str[LITTLE_BUFFER] = "rm -rf ";

    strcat(rm_str, file_dir);
    sprintf(tmp_name, "updateinfo%d.txt", file_no);
    strcat(rm_str, tmp_name);
    system(rm_str);
#ifdef PRINT_DEBUG_INFO
    printf("%s\n", rm_str);
#endif   

    return 0;
}

int main(int argc, char **argv)
{
    int ret = -1;
    int fail_times = 0;
    int max_no = 0; //最大的文件编号
    char receive_buffer[BUFFERSIZE] = "";

    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./uploadUpdateInfo pathname\n    pathname: the path of updateresult.\n");
        return -1;
    }

    if(argv[1][strlen(argv[1]) - 1] != '/')
        strcat(argv[1], "/");

    strcpy(file_dir, argv[1]);
    max_no = check_file_num();
    if(max_no < 0)
    {
        fprintf(stderr, "Err: don't exists update info!\n");
        return -1;
    }
    
    while(file_no <= max_no)
    {
        memset(&post_info, 0, sizeof(struct updateInfo));
        if(get_update_info() != 0)
        {
            file_no++;
            continue;
        }

#ifdef PRINT_DEBUG_INFO
        printf("---------------------\n");
        printf(" device_id = %s\n start = %s\n end = %s\n old_model = %s\n old_version = %s\n"
               " new_model = %s\n new_version = %s\n result = %s\n",
        post_info.device_id, post_info.start, post_info.end, post_info.old_model, 
        post_info.old_version, post_info.new_model, post_info.new_version, post_info.result);
        printf("---------------------\n");
#endif
        fail_times = 0;
        while(fail_times < RESEND_TIMES)
        {
            ret = send_update_info(receive_buffer);
            if(ret != CURLE_OK)
            {
                fprintf(stderr, "Err: Send update info failed!\n");
                fail_times++;
            }
            else
            {
                rm_has_processed_file();
                break;
            }
        }
        file_no++;
    }

    return 0;
}
