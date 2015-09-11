//-----------------------------------------------------------------------------
//
// si475xAMRX.c
//
// Contains the AM radio functions with the exceptions of autoseek.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stddef.h>
#include "typedefs.h"
#include "commanddefs.h"
#include "propertydefs.h"
#include "si475xAMRX.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern u8 PoweredUp;
static u8 cmd[16];
static u8 rsp[16];
// This variables are used by the status commands.  Make sure to call those
// commands (amRsqStatus or amTuneStatus) prior to access.
static u8    Status=0;
static u8    RsqInts=0;
static u8  BLTF=0;
static u8    AFCRL=0;
static  u8    Valid=0;
static u16   Freq =0;
static u8    RSSI=0;

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void wait_ms(u16 ms);
int si475x_command(u8 cmd_size, u8  *cmd, u8 reply_size, u8  *reply);
void si475x_reset(void);
u8 getIntStatus(void);
static void amTuneFreq(u16 frequency);
static void amSeekStart(u8 seekUp, u8 wrap);
static void amRsqStatus(u8 attune, u8 cancel, u8 intack);
static u16 g_amSnr = 3;
static u16 g_amRssi = 12;
static u8 * g_am_patch=NULL;
static u32 g_am_patch_line;

void setAmSnrRssi(u16 rssi,u16 snr)
{
        g_amSnr = snr;
        g_amRssi = rssi;
}
void setAMPatch(u8 * p)
{
	g_am_patch = p;
}

void setAMPatchLine(u32 line)
{
	printf("PatchLine %d\n",line);
	g_am_patch_line = line;
}

//-----------------------------------------------------------------------------
// Take the Si475x out of powerdown mode.
//-----------------------------------------------------------------------------
 void si475xAMRX_sendPath(void)
 {
	   u8 * path =g_am_patch;
	   int i=0,j=0;
	   int res;
		//---------------------------------
		//--patch
		cmd[0] = POWER_UP;
		cmd[1] = 0x77;
		cmd[2] = 0x00;
		cmd[3] = 0x03;
		cmd[4] = 0x20;
		cmd[5] = 0x12;
		res =si475x_command(6, cmd, 1, rsp);
		wait_ms(40);               // wait for si475x to powerup
		printf("[%s] [%d]\n",__func__,__LINE__);
		//--发数据
		for(i=0;i<g_am_patch_line;i++){
			for(j=0;j<8;j++)
			{
				cmd[j] = *path;
				path++;
			}
			res =si475x_command(8, cmd, 1, rsp);
		}
		printf("[%s] [%d]\n",__func__,__LINE__);
		memset(cmd,0,sizeof(cmd));
		cmd[0] = POWER_UP;
		cmd[1] = 0x75;
		cmd[2] = 0x3B;
		cmd[3] = 0x07;
		cmd[4] = 0x20;
		cmd[5] = 0x01;
		 res =si475x_command(6, cmd, 1, rsp);
		printf("[%s] [%d]\n",__func__,__LINE__);

		wait_ms(40);               // wait for si475x to powerup
		cmd[0] = POWER_UP;
		cmd[1] = 0x77;
		cmd[2] = 0x3B;
		cmd[3] = 0x03;
		cmd[4] = 0x20;
		cmd[5] = 0x11;
		res =si475x_command(6, cmd, 1, rsp);
		printf("[%s] [%d]\n",__func__,__LINE__);
		cmd[0] = POWER_UP;
		cmd[1] = 0x77;
		cmd[2] = 0x00;
		cmd[3] = 0x03;
		cmd[4] = 0x20;
		cmd[5] = 0x12;
		res =si475x_command(6, cmd, 1, rsp);
		printf("[%s] [%d]\n",__func__,__LINE__);

 }

void si475xAMRX_powerup(void)
{

    // Check if the device is already powered up.
    if (PoweredUp) {
    } else {
		// new datasheet Powerup the device
		cmd[0] = POWER_UP;
		cmd[1] = 0x75;
		cmd[2] = 0x3B;
		cmd[3] = 0x07;
		cmd[4] = 0x20;
		cmd[5] = 0x01;
		int res =si475x_command(6, cmd, 1, rsp);
		wait_ms(40);               // wait for si475x to powerup
		cmd[0] = POWER_UP;
		cmd[1] = 0x77;
		cmd[2] = 0x3B;
		cmd[3] = 0x03;
		cmd[4] = 0x20;
		cmd[5] = 0x11;
		res =si475x_command(6, cmd, 1, rsp);
		wait_ms(40);               // wait for si475x to powerup
		si475xAMRX_sendPath();
		PoweredUp = 1;
    }
}

//-----------------------------------------------------------------------------
// Configure the output type
//-----------------------------------------------------------------------------
void si475xAMRX_output_config(void)
{
	// Configure the output.  Depending on the output
	// type uncomment the necessary code.

	// *************************************
	// Analog Output
	// *************************************
	// Configure the command array to output analog audio through the analog
	// audio pins
	cmd[0] = ANA_AUDIO_PIN_CFG;
	cmd[1] = 2;  // Audio Out
	
	// Send the command
	si475x_command(2, cmd, 0, NULL);

	// *************************************
	// MPX Output
	// *************************************
	// Configure the command array to output MPX through the analog
	// audio pins
	//cmd[0] = ANA_AUDIO_PIN_CFG;
	//cmd[1] = 3;  // Audio Out
	
	// Send the command
	//si475x_command(2, cmd, 0, NULL);


	// *************************************
	// I2S Output
	// *************************************
	// Configure the command array to output I2S through the digital
	// audio pins
	//cmd[0] = DIG_AUDIO_PIN_CFG;
	//cmd[1] = 10;  // DCLK Slave.  Set to 11 for Master
	//cmd[2] = 10;  // DFS  Slave.  Set to 11 for Master
	//cmd[3] = 12;  // Output
	//cmd[4] = 0;
	
	// Send the command
	//si475x_command(5, cmd, 0, NULL);

	// *************************************
	// I/Q (ZIF) Output
	// *************************************
	// Configure the command array to output I/Q (ZIF) through the ZIF
	// audio pins
	//cmd[0] = ZIF_PIN_CFG;
	//cmd[1] = 21;  // Master
	//cmd[2] = 21;  // Master
	//cmd[3] = 22;  // Output
	//cmd[4] = 22;  // Output
	
	// Send the command
	//si475x_command(5, cmd, 0, NULL);
}

//-----------------------------------------------------------------------------
// Place the Si47xx into powerdown mode.
//-----------------------------------------------------------------------------
void si475xAMRX_powerdown(void)
{
	// Check to see if the device is powered up.  If not do not do anything.
    if(PoweredUp)
    {   
        // Set the powered up variable to 0
        PoweredUp = 0;

	    // Put the ID for the command in the first byte.
	    cmd[0] = POWER_DOWN;

	    // Invoke the command
		si475x_command(1, cmd, 1, rsp);
    }
}

//-----------------------------------------------------------------------------
// This function will set up some general items on the hardware like
// initializing the STC interrupts.
//-----------------------------------------------------------------------------
static void si475xAMRX_hardware_cfg(void)
{
	// Enable the STC interrupt here
    si475x_set_property(INT_CTL_ENABLE, 0x0001);
	printf("[%s] [%d]\n",__func__,__LINE__);
	si475x_set_property(AM_NB_DETECT_THRESHOLD, 0x0010);
	si475x_set_property(AM_SECONDARY_NB_DETECT_THRESHOLD, 0x000E);
	si475x_set_property(AM_SECONDARY_NB_INTERVAL, 0x000A);
	si475x_set_property(AM_NB_MODE_CONTROL, 0x0002);
	si475x_set_property(AM_NB_SCHEME, 0x0002);

	printf("[%s] [%d]\n",__func__,__LINE__);

}

//-----------------------------------------------------------------------------
// Set up general configuration properties:
//      Soft Mute Rate, Soft Mute Max Attenuation, Soft Mute SNR Threshold,
//      Seek Tune SNR Threshold, Seek Tune RSSI Threshold
//-----------------------------------------------------------------------------
static void si475xAMRX_general_cfg(void)
{
	// Here is an example of how to configure a parameter on the 5x/6x products
	// using properties configure the parameter to the desired values as displayed
    // below for softmute.  See the propertydefs.h file for all the
	// configuration properties available.

    // The softmute feature can be disabled by setting the attenuation property
    // to zero.
    //  si475x_set_property(SOFT_MUTE_MAX_ATTENUATION, 8);
    //  si475x_set_property(SOFT_MUTE_TRIGGER_THRESHOLD, 4);
    //  si475x_set_property(SOFT_MUTE_END_THRESHOLD, 0);
    //  si475x_set_property(SOFT_MUTE_RELEASE_TIME, 4000);
    //  si475x_set_property(SOFT_MUTE_ATTACK_TIME, 120);

    // Typically the settings used for seek are determined by the designer
    // and not exposed to the end user. They should be adjusted here.
    si475x_set_property(AM_VALID_SNR_THRESHOLD, g_amSnr);
    si475x_set_property(AM_VALID_RSSI_THRESHOLD, g_amRssi);
}

//AM (Medium Wave), SW (Short Wave), and LW (Long Wave) use the same AM_SW_LW 
//components, thus the commands and properties for these functions are the same.
//For simplicity reasons, the commands and properties only have a prefix AM 
//instead of AM_SW_LW. The main difference among AM, SW, and LW is only the
//frequency range.

//-----------------------------------------------------------------------------
// Set up regional configuration properties including:
//      Seek Band Bottom, Seek Band Top, Seek Freq Spacing
//-----------------------------------------------------------------------------
static void si475xAMRX_regional_cfg(void)
{
	// Change the following properties if desired for the AM settings appropriate
    // for your region.
    si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 9); // Set spacing to 10kHz
    si475x_set_property(AM_SEEK_BAND_BOTTOM, 531); // Set the band bottom to 520kHz
    si475x_set_property(AM_SEEK_BAND_TOP, 1629);   // Set the band top to 1710kHz
}
//-----------------------------------------------------------------------------
// Set up regional configuration properties including:
//      Seek Band Bottom, Seek Band Top, Seek Freq Spacing
//-----------------------------------------------------------------------------
static void si475xAMSWLWRX_regional_cfg(void)
{
	// Change the following properties if desired for the AM/SW/LW settings
	// appropriate for your region.

	//AM band
 //   si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 10); // Set spacing to 10kHz
 //   si475x_set_property(AM_SEEK_BAND_BOTTOM, 520); // Set the band bottom to 520kHz
 //   si475x_set_property(AM_SEEK_BAND_TOP, 1710);   // Set the band top to 1710kHz

	//LW band
 //   si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 9); // Set spacing to 9kHz
 //   si475x_set_property(AM_SEEK_BAND_BOTTOM, 153); // Set the band bottom to 153kHz
 //   si475x_set_property(AM_SEEK_BAND_TOP, 279);   // Set the band top to 279kHz

	//SW band (entire band)
 //   si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 5); // Set spacing to 5kHz
 //   si475x_set_property(AM_SEEK_BAND_BOTTOM, 2300); // Set the band bottom to 2300kHz
 //   si475x_set_property(AM_SEEK_BAND_TOP, 23000);   // Set the band top to 23000kHz

	//SW band (120m band)
 //   si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 5); // Set spacing to 5kHz
 //   si475x_set_property(AM_SEEK_BAND_BOTTOM, 2300); // Set the band bottom to 2300kHz
 //   si475x_set_property(AM_SEEK_BAND_TOP, 2495);   // Set the band top to 2495kHz

	//SW band (60m band)
    si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 5); // Set spacing to 5kHz
    si475x_set_property(AM_SEEK_BAND_BOTTOM, 4750); // Set the band bottom to 4750kHz
    si475x_set_property(AM_SEEK_BAND_TOP, 5060);   // Set the band top to 5060kHz
}



//-----------------------------------------------------------------------------
// Configures the device for normal operation
//-----------------------------------------------------------------------------
void si475xAMRX_configure(void)
{
    si475xAMRX_output_config(); // Configure the output mode

    // Configure all other registers
    si475xAMRX_hardware_cfg();
    si475xAMRX_general_cfg();
    si475xAMRX_regional_cfg();
}

//-----------------------------------------------------------------------------
// Configures the device for normal operation
//-----------------------------------------------------------------------------
void si475xAMSWLWRX_configure(void)
{
    si475xAMRX_output_config(); // Configure the output mode

    // Configure all other registers
    si475xAMRX_hardware_cfg();
    si475xAMRX_general_cfg();
    si475xAMSWLWRX_regional_cfg();
}

//-----------------------------------------------------------------------------
// Resets the part and initializes registers to the point of being ready for
// the first tune or seek.
//-----------------------------------------------------------------------------
void si475xAMRX_initialize(void)
{
    // Zero status registers.
	PoweredUp = 0;

    // Perform a hardware reset, power up the device, and then perform the
    // initial configuration.
    si475x_reset();
    si475xAMRX_powerup();
	printf("[%s] [%d]\n",__func__,__LINE__);
	si475xAMRX_mute(1);
	printf("[%s] [%d]\n",__func__,__LINE__);
    si475xAMRX_configure();
	printf("[%s] [%d]\n",__func__,__LINE__);

}

void si475xAMSWLWRX_initialize(void)
{
    // Zero status registers.
	PoweredUp = 0;

    // Perform a hardware reset, power up the device, and then perform the
    // initial configuration.
    si475x_reset();
    si475xAMRX_powerup();
    si475xAMSWLWRX_configure();
}

//-----------------------------------------------------------------------------
// Set the volume and mute/unmute status
//
// Inputs:
//      volume:    a 6-bit volume value
//
// Note: It is assumed that if the volume is being adjusted, the device should
// not be muted.
//-----------------------------------------------------------------------------
void si475xAMRX_set_volume(u8 volume)
{
    // Turn off the mute
    si475x_set_property(AUDIO_MUTE, 0);

    // Set the volume to the passed value
    si475x_set_property(AUDIO_ANALOG_VOLUME, volume);
}

//-----------------------------------------------------------------------------
// Mute/unmute audio
//
// Inputs:
//      mute:  0 = output enabled (mute disabled)
//             1 = output muted
//-----------------------------------------------------------------------------
void si475xAMRX_mute(u8 mute)
{
    if(mute)
		// 1 = Left muted, 2 = right muted, 3 = both muted
    	si475x_set_property(AUDIO_MUTE, 3);
    else
    	si475x_set_property(AUDIO_MUTE, 0);
}

//-----------------------------------------------------------------------------
// Tunes to a station number using the current band and spacing settings.
//
// Inputs:
//      frequency:  frequency in 1kHz steps
//
// Returns:
//      The RSSI level found during the tune.
//-----------------------------------------------------------------------------
u8 si475xAMRX_tune(u16 frequency)
{
	// Enable the bit used for the interrupt of STC.
    //SeekTuneInProc = 1;

	// Call the tune command to start the tune.
    amTuneFreq(frequency);

    // wait for the interrupt before continuing
    // If you do not wish to use interrupts but wish to poll the part
    // then comment out this line.
//    while (WaitSTCInterrupt); // Wait for interrupt to clear the bit

    // Wait for stc bit to be set
    while (!(getIntStatus() & STCINT));

	// Clear the STC bit and get the results of the tune.
    amRsqStatus(1, 0, 1);


	// Disable the bit used for the interrupt of STC.
    //SeekTuneInProc = 0;

    // Return the RSSI level
    return RSSI;
}

//-----------------------------------------------------------------------------
// Inputs:
//      seekup:  0 = seek down
//               1 = seek up
//      seekmode: 0 = wrap at band limits
//                1 = stop at band limits
// Outputs:
//      zero = seek found a station
//      nonzero = seek did not find a station
//-----------------------------------------------------------------------------
u8 si475xAMRX_seek(u8 seekup, u8 seekmode)
{
	// Enable the bit used for the interrupt of STC.
    //SeekTuneInProc = 1;

	// Call the tune command to start the seek.
    amSeekStart(seekup, !seekmode);

    // wait for the interrupt before continuing
    // If you do not wish to use interrupts but wish to poll the part
    // then comment out these two lines.
  //  while (WaitSTCInterrupt); // Wait for interrupt to clear the bit

    // Wait for stc bit to be set
    // If there is a display to update seek progress, then you could
    // call fmTuneStatus in this loop to get the current frequency.
    // When calling fmTuneStatus here make sure intack is zero.
    while (!(getIntStatus() & STCINT));

	// Clear the STC bit and get the results of the tune.
    amRsqStatus(1, 0, 1);

	// Disable the bit used for the interrupt of STC.
    //SeekTuneInProc = 0;

    // The tuner is now set to the newly found channel if one was available
    // as indicated by the seek-fail bit.
    return BLTF; //return seek fail indicator
}

//-----------------------------------------------------------------------------
// Returns the current tuned frequency of the part
//
// Returns:
//      frequency in 1kHz steps
//-----------------------------------------------------------------------------
u16 si475xAMRX_get_frequency()
{
	// Get the tune status which contains the current frequency
    amRsqStatus(1, 0, 0);

    // Return the frequency
    return Freq;
}

//-----------------------------------------------------------------------------
// Returns the current RSSI value returned from the part
//
// Returns:
//      RSSI in dBuV
//-----------------------------------------------------------------------------
u8 si475xAMRX_get_rssi()
{
	// When attune is set to 0 the RSSI returned will be the RSSI at the time
	// the RSQ status command was called.  If 1 is set to attune then the RSSI
	// measured during the last tune or seek will be returned.
	amRsqStatus(0, 0, 0); 

    // Return the RSSI level
    return RSSI;
}

//-----------------------------------------------------------------------------
// Helper function that sends the AM_TUNE_FREQ command to the part
//
// Inputs:
// 	frequency in 1kHz steps
//-----------------------------------------------------------------------------
static void amTuneFreq(u16 frequency)
{
    // Put the ID for the command in the first byte.
    cmd[0] = AM_TUNE_FREQ;

	// Depending on what tune functionality is needed or in the appropriate bits
    cmd[1] = 0;

	// Determine the injection side
	cmd[1] |= 0; // 0 = Automatic, 1 = Low-side, 2 = High-side

	// Determine if the metrics should be smoothly transitioned after a tune
	cmd[1] |= 0; // 0 = Metrics match new channel, 4 = trans. from prev. channel

	// Determine the tune mode
	cmd[1] |= 0; // 0 = Validated normal tune, 0x10 = unvalidated fast tune
				 // 0x20 = Validated AF tune, 0x30 = Validated AF check

	// Determine if HD ready mode is needed
	cmd[1] |= 0; // 0 = Normal bandwidth, 0x40 = wide bandwidth/HD mode

	// Put the frequency in the second and third bytes.
    cmd[2] = (u8)(frequency >> 8);
	cmd[3] = (u8)(frequency & 0x00FF);

	// Set the antenna calibration value.
    cmd[4] = (u8)0;  // Auto
	cmd[5] = (u8)0;

    // Invoke the command
	si475x_command(6, cmd, 1, rsp);
}

//-----------------------------------------------------------------------------
// Helper function that sends the AM_SEEK_START command to the part
//
// Inputs:
// 	seekUp: If non-zero seek will increment otherwise decrement
//  wrap:   If non-zero seek will wrap around band limits when hitting the end
//          of the band limit.
//-----------------------------------------------------------------------------
static void amSeekStart(u8 seekUp, u8 wrap)
{
    // Put the ID for the command in the first byte.
    cmd[0] = AM_SEEK_START;

	// Put the flags if the bit was set for the input parameters.
	cmd[1] = 0;
    if(seekUp) cmd[1] |= 0x08;
	if(wrap)   cmd[1] |= 0x04;

    // Invoke the command
	si475x_command(2, cmd, 1, rsp);
}

//-----------------------------------------------------------------------------
// Helper function that sends the AM_RSQ_STATUS command to the part
//
// Inputs:
// 	attune: If non-zero the metrics will be the current values of the metrics
//          for the station.  If zero the metrics will be the value found at
//          the last seek/tune command.
//  cancel: If non-zero the currently running tune/seek will be cancelled
//  intack: If set the STC interrupt will be cleared.
//
// Outputs:
//  Status:  Contains bits about the status returned from the part.
//  RsqInts: Contains bits about the interrupts that have fired related to RSQ.
//  Valid:   The station is valid if this is non-zero
//  BLTF:    If the tune/seek failed this bit will be non-zero
//  Freq:    The currently tuned frequency
//  RSSI:    The current signal strength of the station
//  ASNR:    The current audio SNR level of the station
//  LASSI:   The current adjacent channel strength for the channel below the
//           the currently tuned station.
//  HASSI:   The current adjacent channel strength for the channel above the
//           the currently tuned station.
//  FreqOff: The frequency offset in kHz of the current station from the tuned 
//           frequency.
//  MOD:     The current modulation depth of the current station.
//-----------------------------------------------------------------------------
static void amRsqStatus(u8 attune, u8 cancel, u8 intack)
{
    u8    MOD;
    u8    HASSI;
    u8    LASSI;
    u8    FreqOff;
    i8    ASNR;

    // Put the ID for the command in the first byte.
    cmd[0] = AM_RSQ_STATUS;

	// Depending on the passed parameters set the second argument
	cmd[1] = 0;
	
	if(intack) cmd[1] |= 0x01;
	if(cancel) cmd[1] |= 0x02;
	if(attune) cmd[1] |= 0x04;

	// Invoke the command
	si475x_command(2, cmd, 13, rsp);

    // Parse the results
	Status  = rsp[0];
    RsqInts = rsp[1];
    Valid   = !!(rsp[2] & 0x01);
	AFCRL   = !!(rsp[2] & 0x02);
    BLTF    = !!(rsp[2] & 0x80);
    Freq   = ((u16)rsp[3] << 8) | (u16)rsp[4];
    FreqOff = rsp[5];
    RSSI   = rsp[6];
    ASNR   = rsp[7];
	LASSI  = rsp[9];
	HASSI  = rsp[10];
	MOD    = rsp[12];
    printf("[%s] RSSI %d ASNR %d Freq %d Valid %d LASSI %d HASSI %d\n",__func__,RSSI,ASNR,Freq,Valid, LASSI,HASSI);
}

#ifdef TEST
#include <stdio.h>
#include <stdlib.h>
#include "fix_amrx_patch.h"

int main(int argc, char *argv[])
{

    u8 seek_fail;
    u16 current_frequency;
    int num_found = 0;
    char buf[32]; 

   if(argc == 1)
   {
	printf("usage: ./am <seek> <rssi> <snr>\n");
	printf("usage: ./am <tune> <station>\n");
	return -1;
   }   
   setAMPatch(am_patch);
   setAMPatchLine(sizeof(am_patch)/8);

   if(!strcmp(argv[1],"seek"))
   {

   setAmSnrRssi(atoi(argv[3]),atoi(argv[2]));
   printf("g_amRssi = %d g_amSnr = %d\n",g_amSnr, g_amRssi);
   si475xAMRX_initialize();
   si475xAMRX_set_volume(63);
   si475xAMRX_mute(0);
   si475x_getPartInformation(sizeof(buf),buf);

	   si475xAMRX_tune(520);

		   sleep(3);
	   seek_fail = si475xAMRX_seek(1, 1);  // seek up, stop at limits
		   while((seek_fail == 0)) 
		   {
			   printf("seek_fail = %d\n",seek_fail);
	   		current_frequency = si475xAMRX_get_frequency();
			   num_found++;
			   printf("%d found\n",current_frequency);
			   sleep(3);
			   
			seek_fail = si475xAMRX_seek(1, 1); // seek up, stop at limts
		   }
	   printf("num_found %d\n",num_found);
   }else if(!strcmp(argv[1],"tune"))
  {
		
   setAmSnrRssi(12,8);
   si475xAMRX_initialize();
   si475xAMRX_set_volume(63);
   si475xAMRX_mute(0);
   si475x_getPartInformation(sizeof(buf),buf);

	   si475xAMRX_tune(atoi(argv[2]));
	   sleep(-1);
  }
   si475xAMRX_mute(1);
   si475xAMRX_powerdown();
}

#endif
