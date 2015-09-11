#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "SQLiteTable.h"
#include "VDRDeployParaTable.h"
#include "Utility.h"

#define AT88SC_IOCTL_BASE      'C'
#define AT88SC_DATA            _IOW (AT88SC_IOCTL_BASE, 0, struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID      _IOW (AT88SC_IOCTL_BASE, 1, struct at88sc_ioctl_data)

struct at88sc_ioctl_data {	
	unsigned char rw;
	unsigned char userzone;
	unsigned char password[13];
};

using namespace std;

static unsigned char passbuf[13]={'i','N','N','O','V',8,8,8,9,2,8,7,5};

#define MODEL_TYPE_LEN          20
#define MODEL_ID_LEN            7

string char_to_stirng(char *buf)
{
	return buf;
};

int main(int argc, char **argv)
{
    int fd;
    struct at88sc_ioctl_data ioctldata;
    unsigned char tuid[32];
    int ret;

	string version;

	int default_test = 0;

	FILE* fp = NULL; 
	if(NULL == argv[1])
	{
		default_test = 1;
	}
	else 
	{
		fp = fopen(argv[1], "r");
		if(NULL == fp)
		{
			default_test = 1;
		}
	}
	
	if(default_test)
	{
		printf("try default!\n");
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
    table->SetStringValue(VDR_DEPLOY_PARA_VERSION, char_to_stirng(version_start + 2));
    table->Commit();
    delete table;
	
    
    fd = open("/dev/at88sc", O_RDONLY);
    if(fd < 0)
    {
        cerr << "Failed to open device." << endl;
        return -1;
    }
    
    ioctldata.rw = 1;
    ioctldata.userzone = 1;
    memcpy(ioctldata.password, passbuf, 13);
    
    if(ioctl(fd, AT88SC_DATA, &ioctldata) < 0)
    {
        cerr << "Failed to control device." << endl;
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
    
    string stuid((char *)tuid, 32);
    
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ", tuid[i]);
        if((i + 1) % 16 == 0)
        {
            printf("\r\n");
        }
    }
    
    cout << "STRING: " << stuid << endl;
    
    string modelNo = stuid.substr(16, 9);
    if(modelNo == "0000JL301")
    {
        modelNo = "JL901";
    }
    string modelId = stuid.substr(25, 7);
    vector<unsigned char> sequence;
    sequence.clear();
    VDRDeployParaTable deployTable;
    bool nozero = false;
    for(int i = 0; i < modelNo.length(); i ++)
    {
        if((nozero == false) && (modelNo[i] == '0'))
        {
            continue;
        }
        nozero = true;
        sequence.push_back(modelNo[i]);
    }
    for(int i = sequence.size(); i < MODEL_TYPE_LEN; i ++)
    {
        sequence.push_back(0x00);
    }
    ret = deployTable.SetSequenceValue(VDR_DEPLOY_PARA_MODEL_NO, sequence);
    if(ret < 0)
    {
        cerr << "Set model no failed." << endl;
        return -1;
    }
    sequence.clear();
    for(int i = 0; i < modelId.length(); i ++)
    {
        sequence.push_back(modelId[i]);
    }
    for(int i = sequence.size(); i < MODEL_ID_LEN; i ++)
    {
        sequence.push_back(0x00);
    }
    ret = deployTable.SetSequenceValue(VDR_DEPLOY_PARA_ID, sequence);
    if(ret < 0)
    {
        cerr << "Set model no failed." << endl;
        return -1;
    }
    ret = deployTable.Commit();
    if(ret < 0)
    {
        cerr << "Commit failed." << endl;
        return -1;
    }

    return 0;
}
