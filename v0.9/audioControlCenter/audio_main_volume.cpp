#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <alsa/asoundlib.h>
#include "audio_main_volume.h"

#define IBAC_VOLUME_FULL_SCALE          30  // 30
#define IBAC_VOLUME_MAIN_MIN            86  //50  80
#define IBAC_VOLUME_MAIN_MAX           127  //140 //127  //112

#define IBAC_DEV_ID_MAX 31

#ifdef _8925_VERSION
#define DEV_ID_MAIN_VOLUME         4
#else
#define DEV_ID_MAIN_VOLUME         1
#endif

#define IBAC_VOLUME_DAI_MIN 100
#define IBAC_VOLUME_DAI_MAX 255

#define IBAC_MUTE_GF_TIME       (1000)

#define check_range(val, min, max) \
          ((val < min) ? (min) : (val > max) ? (max) : (val)) 
#define convert_prange1(val, min, max) \
         ceil((val) * ((max) - (min)) / (IBAC_VOLUME_FULL_SCALE) + (min))

#define MAIN_VOLUME_FILE ((char*)"/nand3/applicationdata/main_volume.cfg")
#define MUTE_VOLUME_FILE ((char*)"/nand3/applicationdata/mute_volume.cfg")

double Main_Volume::getLogBaics(void)
{
    static double res = 0.0;
    if(res <0.001)
    {
        res =log(IBAC_VOLUME_FULL_SCALE+1);
    }
    return res;
}

int Main_Volume::percent2val_2(int val, int min, int max)
{
    int ret = -1;
    if(val < 0)
        return -1;
        
    ret = (max-min) * (log(val+1)) / (getLogBaics()) + min;
    
    printf("*[percent2val_2]*percent2val_vol = %d; val = %d\n", ret, val);
    return ret;
}

int Main_Volume::percent2val_3(int val, int min, int max) 
{
    int retval = -1;
    if(val < 0) 
        return -1;
    if(val == 0)    
        return 0; 

    retval = ((IBAC_VOLUME_FULL_SCALE-val)*min + (val-1)*max)/(IBAC_VOLUME_FULL_SCALE - 1); 

    printf("------------>after_map_vol = %d\n",retval);
    return retval;  
}

long Main_Volume::get_integer(long x, long min, long max)
{
         long val = min;
         val = (long)convert_prange1(x, min, max);
         val = check_range(val, min, max);
         return val;
}


int Main_Volume::csetAudio(int numid, char *name, int volume)
{
//	name = "tccvol";// tccvol
#ifdef _8925_VERSION
    if(volume > 0)
        system("amixer cset numid=5 on");
    else if(volume == 0)
        system("amixer cset numid=5 off");
	
#endif
	printf("=== [Main_Volume::csetAudio]*** numid = %d; name = %s; volume = %d ===\n", numid, name, volume);
	long vol = (long)volume;
	int err;
	char card[20];
	snd_ctl_t *handle = NULL;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_id_t *id;
	snd_ctl_elem_value_t *control;
	unsigned int idx, count;
	long tmp = 0;
	snd_ctl_elem_type_t type;
	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);
	
	strcpy(card, "default");

	// if numid is 0 or name is NULL, device is not available, return
	if (numid <= 0 && !strcmp(name, ""))
	{		
		printf("[Audio Control]: Set Audio --> Wrong control identifier: %d\n", numid);
		return -1;
	}
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);	/* default */

	if (numid > 0 && numid <= IBAC_DEV_ID_MAX)
	{
		snd_ctl_elem_id_set_numid(id, numid);		/* set the numid to id->numid */
	}
	else if (strcmp(name, "") > 0)
	{
		snd_ctl_elem_id_set_name(id, name);
	}
	
	if (handle == NULL && (err = snd_ctl_open(&handle, card, 0)) < 0) 
	{		/* open the control */
		printf("[Audio Control]: Set Audio --> Control %s open error: %s\n", card, snd_strerror(err));
		return err;
	}
	
	snd_ctl_elem_info_set_id(info, id);				/* info->id = id */
	
	if ((err = snd_ctl_elem_info(handle, info)) < 0) 		// get info details from drivers
	{		/* handle->ops->element_info(handle, info); */
		printf("[Audio Control]: Set Audio --> Cannot find the given element from control %s, name = %s, id = %d\n", card, name, numid);
		printf("*** pb_error: %s\n", snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return err;
	}
	
	snd_ctl_elem_info_get_id(info, id); /* FIXME: Remove it when hctl find works ok !!! */ 

	type = snd_ctl_elem_info_get_type(info);	/* type = info->type */
	count = snd_ctl_elem_info_get_count(info);	/* count = info->count*/
	snd_ctl_elem_value_set_id(control, id);		/* control->id = id */
	

	for (idx = 0; idx < count && idx < 128; idx++) 
	{
		switch (type) 
		{
			case SND_CTL_ELEM_TYPE_BOOLEAN:
				tmp = vol > 0 ? 1 : 0;
				snd_ctl_elem_value_set_boolean(control, idx, tmp);		/* control->value.integer.value[idx] = tmp; */
				break;
			case SND_CTL_ELEM_TYPE_INTEGER:
                                tmp = vol;
				snd_ctl_elem_value_set_integer(control, idx, tmp);	/* control->value.integer.value[idx] = tmp; */
				break;
			case SND_CTL_ELEM_TYPE_INTEGER64:
                                tmp = vol;
				snd_ctl_elem_value_set_integer64(control, idx, tmp);	/* control->value.integer64.value[idx] = tmp; */
				break;
			case SND_CTL_ELEM_TYPE_ENUMERATED:
				vol = (unsigned)(vol < 0  ? 0 : vol);				
				tmp =  check_range(vol, 0, (int)snd_ctl_elem_info_get_items(info) - 1);
				snd_ctl_elem_value_set_enumerated(control, idx, tmp);	/* control->value.enumerated.item[idx] = val; */
				break;
			case SND_CTL_ELEM_TYPE_BYTES:
				tmp = get_integer(vol, 0, 255);
				snd_ctl_elem_value_set_byte(control, idx, tmp);	/* control->value.bytes.data[idx] = val; */
				break;
			default:
				break;
		}
	}

	if ((err = snd_ctl_elem_write(handle, control)) < 0) 	/* handle->ops->element_write(handle, control) */
	{		
		printf("[Audio Control]: Set Audio --> Control %s element write error: %s\n", card, snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return err;
	}

	snd_ctl_close(handle);
	handle = NULL;

	return 0;
}

int Main_Volume::set_main_volume(int volume)
{
#ifndef _8925_VERSION
	int vol;
	if((vol = percent2val_3(volume, IBAC_VOLUME_MAIN_MIN, IBAC_VOLUME_MAIN_MAX)) == -1){
		printf("set_main_volume/percent2val_3 error\n");
		return -1;
	}  
	printf("set_main_volume=%d\n", vol);

	if(csetAudio(DEV_ID_MAIN_VOLUME, NULL, vol) < 0 ){
		printf("set_main_volume() error\n");
		return -1;
	}
#else
    //printf("----- volume = %d -----\n", volume);
	if(csetAudio(DEV_ID_MAIN_VOLUME, NULL, volume) < 0 ){
		printf("set_main_volume() error\n");
		return -1;
	}
#endif

	return 0;
}

int Main_Volume::get_main_volume()
{
	int vol;
	char volume[3];
	FILE *fd = fopen(MAIN_VOLUME_FILE, "r+");

         if (!fd) {      
                 printf("[main_volume.cfg]: save_main_volume() open file failed !!!\n");
                 fclose(fd);
                 return -1;
         }
	
	if(fread(volume, sizeof(char), 2, fd) != 2) {
                 printf("[main_volume.cfg]: save_main_volume() read file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	vol = atoi(volume);

         fclose(fd);
         return vol;
}

int Main_Volume::save_main_volume(int volume)
{
	char vol[3];
	sprintf(vol,"%d",volume);

         FILE *fd = fopen(MAIN_VOLUME_FILE, "w+");

         if (!fd) {      
                 printf("[main_volume.cfg]: save_main_volume() open file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	if(fwrite(vol, sizeof(char), 2, fd) != 2 ) {
                 printf("[main_volume.cfg]: save_main_volume() write file failed !!!\n");
                 fclose(fd);
                 return -1;
         }
 
	 fflush(fd);
         fclose(fd);
         return 0;
}

int Main_Volume::set_mute(void)
{
	char vol[3];
	sprintf(vol,"%d",1);

	csetAudio(DEV_ID_MAIN_VOLUME, NULL, 0); 

         FILE *fd = fopen(MUTE_VOLUME_FILE, "w+");

         if (!fd) {      
                 printf("[mute_volume.cfg]: set_mute() open file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	if(fwrite(vol, sizeof(char), 2, fd) != 2 ) {
                 printf("[mute_volume.cfg]: set_mute() write file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	 fflush(fd);
         fclose(fd);

         return 0;
}

int Main_Volume::set_unmute(void)
{

	char vol[3];
	sprintf(vol,"%d",0);

	int get_vol = get_main_volume();

	set_main_volume(get_vol);
	
         FILE *fd = fopen(MUTE_VOLUME_FILE, "w+");

         if (!fd) {      
                 printf("[mute_volume.cfg]: set_unmute() open file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	if(fwrite(vol, sizeof(char), 2, fd) != 2 ) {
                 printf("[mute_volume.cfg]: set_unmute() write file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	 fflush(fd);
         fclose(fd);

         return 0;
}      

int Main_Volume::get_mute_status(void)
{
	int vol;
	char volume[3];
	FILE *fd = fopen(MUTE_VOLUME_FILE, "r+");

         if (!fd) {      
                 printf("[mute_volume.cfg]: get_mute_volume() open file failed !!!\n");
                 fclose(fd);
                 return -1;
         }
	
	if(fread(volume, sizeof(char), 2, fd) != 2) {
                 printf("[mute_volume.cfg]: get_mute_volume() read file failed !!!\n");
                 fclose(fd);
                 return -1;
         }

	vol = atoi(volume);

         fclose(fd);
         return vol;
}
