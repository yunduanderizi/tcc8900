#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camapp.h"

int main(int argv, char* argc[])
{
	int starttimes = 0;	

#if 0
	if(argv < 2 || NULL == argc[1])
	{
		printf("error input param!\n");
		return -1;
	}
	int i = atoi(argc[1]);
#endif	

	int i = 0;
	
	if(0 == i)
	{
		camStart();
		sleep(20);
	}
	//else
	{
		camCap(320, 240);
		sleep(5);
	}
	camStop();

	while(1);

	while(1){
		starttimes++;
		camStart();
		sleep(2);
	
		camStop();
		fprintf(stdout, "finish test...start times = %d\n", starttimes);
		sleep(1);
	}
	camQuit();
	return 0;
}
