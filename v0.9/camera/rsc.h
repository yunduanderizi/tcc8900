/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               Camera    I n t e r f a c e    M O D U L E

                        EDIT HISTORY FOR MODULE

when        who       what, where, why
--------    ---       -------------------------------------------------------
10/xx/08   ZzaU      Created file.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef __CAM_RSC_H__
#define __CAM_RSC_H__

/*===========================================================================
                           LCD InterFace
===========================================================================*/
extern void clear_fb(CameraDevice* self);
extern int bak_lcd_init(CameraDevice* self);
extern int recovery_lcd_init(CameraDevice* self);
extern void rsc_init_lcd(CameraDevice* self, unsigned int view_mode);
extern void rsc_set_lcd_ch0(CameraDevice* self, unsigned char en);
#if 0
extern void rsc_set_lcd_addr(CameraDevice* self, unsigned int addr);
#else
extern void rsc_set_lcd_addr(int flagyuv420, CameraDevice* self, unsigned int addr);
#endif
extern void rsc_draw_lcd(CameraDevice* self);


/*===========================================================================
                           G2D InterFace
===========================================================================*/
extern void rsc_process_image(CameraDevice *self, uint8_t *source);

  
/*===========================================================================
                           JPEG InterFace
===========================================================================*/
extern void  rsc_encode_jpeg(CameraDevice *self);


/*===========================================================================
                           FS InterFace
===========================================================================*/
extern void  rsc_save_file(CameraDevice *self, unsigned short* filename);


/*===========================================================================
                           Kernel Resource
===========================================================================*/
extern void rsc_sched_delay(int ms);

extern void Set_DisplayPosition(CameraDevice *self, unsigned int display_scr_width, unsigned int display_scr_height,unsigned int display_dst_width, unsigned int display_dst_height,
						unsigned int check_multiple,unsigned int SizediffofW,unsigned int SizediffofH, unsigned int src_addr );

#endif /* __CAM_RSC_H__ */
