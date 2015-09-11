#include <stdio.h>
#include <time.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "LogUtility.h"
#include "VDRUpdateTable.h"
#include "VDRDeployParaTable.h"

#define UPDATE_STATUS_SUCCESSFUL	0x10
#define UPDATE_STATUS_FAILED		0x11
#define UPDATE_STATUS_CANCLED		2
#define UPDATE_STATUS_NOTDONE		3

#define UPDATE_SCRIPT		"app_update.sh"
#define UPDATE_RET			"/tmp/updateret"
#define UPDATE_TEMP_TAR_DIR "/application/tmp_update_patch"
#define DEFAULT_DIR			"/application/work"

#define DEV_APP 			"/dev/mmcblk0p2"
#define DIR_APP 			"/application/"

#define UPDATE_SUCCESFUL 			48
#define UPDATE_FAILED				45

int update_process(const char *filepath)
{
	if(NULL == filepath)	
	{
		printf("NULL filepath!\n");
		return -1;
	}

	chmod(filepath, 777);

	char cmd[1024] = {0};
	if(access(UPDATE_TEMP_TAR_DIR, F_OK) < 0)
	{
	     mkdir(UPDATE_TEMP_TAR_DIR, 0777);
    }
	sprintf(cmd, "tar zxvf %s -C %s", filepath, UPDATE_TEMP_TAR_DIR);
	printf("cmd:%s\n", cmd);
	system(cmd);

	chdir(UPDATE_TEMP_TAR_DIR);

	char update_script[1024] = {0};
	sprintf(update_script, "./%s", UPDATE_SCRIPT);
	printf("update_script:%s\n", update_script);
	chmod(update_script, 777);
	system(update_script);

	FILE *fp = fopen(UPDATE_RET, "r");
    if(NULL == fp)
    {
		printf("can not open result file!\n");
		return -1;
	}

	int ret = fgetc(fp);
	printf("ret:%d\n", ret);
	if(UPDATE_SUCCESFUL != ret)
	{
	    fclose(fp);
		return -1;
	}
	
    fclose(fp);

	chdir(DEFAULT_DIR);

	return 0;
}

int update_dev_version()
{
    FILE* fp = NULL; 
	string version;

    printf("update software version !\n");
    fp = fopen("/application/data/version.ini", "r");
    if(NULL == fp)
    {
        fp = fopen("/nand3/data/version.ini", "r");
        if(NULL == fp)
        {
            cerr << "Failed to open version.ini." << endl;
            return -1;
        }
    }
	int i = 0;
	char buf[1024] = {0};
	for(i = 0; i < 2; i++)
	{
		fgets(buf, 1024, fp);
	}

	char *version_start = strstr(buf, "=");

	printf("version:%s\n", version_start + 2);
	
	VDRDeployParaTable *table = new VDRDeployParaTable();
    table->SetStringValue(VDR_DEPLOY_PARA_VERSION, version_start + 2);
    table->Commit();
    delete table;

    return 0;
}
int main(int argc, char **argv)
{
	int temp = -1;

	if(mount(DEV_APP, DIR_APP, "ext2", MS_REMOUNT, NULL) < 0)
    {
        printf("mount error!\n");
        return -1;
    }
		
	VDRUpdateTable *table = new VDRUpdateTable();
	int num_cnt = 	table->GetRowCount();
	if(num_cnt <= 0)
	{
		printf("no recoder!\n");
		return -1;
	}

	int update_work = -1;

	int i = 0;
	int ret = table->RefreshTop();
	if(ret < 0)
	{
		printf("refresh failed!\n");
		return -1;
	}
	
	for(i = 0; i < num_cnt; i++)
	{
		if(ret != -1)
		{
			table->GetIntValue(VDR_UPDATE_RESULT, temp);
			string p_file_path;
			table->GetStringValue(VDR_UPDATE_PACKAGE_URL, p_file_path);
			printf("url:%s\n", p_file_path.c_str());

			printf("tmp:%d\n", temp);
			if(UPDATE_STATUS_NOTDONE == temp)
			{
			    string file_path;
				table->GetStringValue(VDR_UPDATE_PACKAGE_URL, file_path); //读出文件路径和文件名?


				time_t update_tm;

				//升级
				int ret_update = update_process(file_path.c_str());//升级;
				if(ret_update < 0)
				{
					time(&update_tm);
					table->SetIntValue(VDR_UPDATE_FINISH_TIME, update_tm); //记录升级结束时间。
					table->SetIntValue(VDR_UPDATE_RESULT, UPDATE_STATUS_FAILED); 
					table->Commit();
					delete table;
					return -1;
				}

				time(&update_tm); //取当前时间。
				table->SetIntValue(VDR_UPDATE_FINISH_TIME, update_tm); //记录升级结束时间。
	    		table->SetIntValue(VDR_UPDATE_RESULT, UPDATE_STATUS_SUCCESSFUL);  //升级成功!
				table->Commit();
                update_dev_version();
				update_work = 1;
			}
			ret = table->RefreshNext();
		}
	
		
	}

	delete table;

	sync();
	if(1 == update_work)
        system("accoff.sh 2 &");
		
	return 0;
}

