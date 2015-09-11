//-----------------------------------------------------------------------------
//
// si475xFMRX.c
//
// Contains the FM radio functions with the exceptions of autoseek and rds.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stddef.h>
#include <stdio.h>
#include "typedefs.h"
#include "commanddefs.h"
#include "propertydefs.h"
#include "si475xFMRX.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define POWERUP_TIME 110    // Powerup delay in milliseconds

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern unsigned char PoweredUp;
static u8  cmd[16];
static u8  rsp[16];
static u16 DC_Type=0;
static u16   calCap=0;
static u16   calFmax=0;

// This variables are used by the status commands.  Make sure to call those
// commands (fmRsqStatus, fmTuneStatus, or fmRdsStatus) prior to access.
static u8  Status=0;
static u8  RsqInts=0;
static u8  BLTF=0;
static u8  AFCRL=0;
static  u8    Valid=0;
static u16   Freq =0;
static u8  RSSI=0;
static u8  RdsInts;
static u8  RdsSync;
static u8  GrpLost;
static u8  RdsFifoUsed;
static u16 BlockA;
static u16 BlockB;
static u16 BlockC;
static u16 BlockD;
static u8  BleA;
static u8  BleB;
static u8  BleC;
static u8  BleD;
static u8  TP;
static u8  PTY;
static u8  PI;


#define FM_SEEKTIMEOUT  200
#define FM_UNITTIMEOUT  200   /*we wait for UNITTIMEOUT*SEEKTIMEOUT MS  */

typedef enum {USA, EUROPE, JAPAN} country_enum; // Could be expanded

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void wait_ms(u16 ms);
int si475x_command(u8 cmd_size, u8 *cmd, u8 reply_size, u8 *reply);
void si475x_reset(void);
u8 getIntStatus(void);
static void fmTuneFreq(u8 quick, u16 frequency);
static void fmSeekStart(u8 seekUp, u8 wrap);
static void fmRsqStatus(u8 attune, u8 cancel, u8 intack);
static u16 g_fmSnr = 3;
static u16 g_fmRssi = 12;
void setFmSnrRssi(u16 rssi,u16 snr)
{
        g_fmSnr = snr;
        g_fmRssi = rssi;
}

//-----------------------------------------------------------------------------
// Take the Si475x out of powerdown mode.
//-----------------------------------------------------------------------------
void si475xFMRX_powerup(void)
{

    // Check if the device is already powered up.
    if (PoweredUp) {
    } else {
		// new datasheet Powerup the device
		cmd[0] = POWER_UP;
		cmd[1] = 0x75;
		cmd[2] = 0x3B;//15pF补电容
		cmd[3] = 0x07;
		cmd[4] = 0x10;
		cmd[5] = 0x01;
		printf("[%s] 0x3B \n",__func__);
		int res =si475x_command(6, cmd, 1, rsp);
		wait_ms(40);               // wait for si475x to powerup
		cmd[0] = POWER_UP;
		cmd[1] = 0x77;
		cmd[2] = 0x3B;
		cmd[3] = 0x03;
		cmd[4] = 0x10;
		cmd[5] = 0x11;
		res =si475x_command(6, cmd, 1, rsp);
		wait_ms(40);               // wait for si475x to powerup
		PoweredUp = 1;
    }
}
//-----------------------------------------------------------------------------
// Configure the output type
//-----------------------------------------------------------------------------
void si475xFMRX_output_config(void)
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

	// *************************************
	// IBOC Blend Mode
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

	// Configure the command array to input I2S through the digital
	// audio pins
	//cmd[0] = DIG_AUDIO_PIN_CFG;
	//cmd[1] = 10;  // DCLK Slave.  Set to 11 for Master
	//cmd[2] = 10;  // DFS  Slave.  Set to 11 for Master
	//cmd[3] = 13;  // Input
	//cmd[4] = 0;
	
	// Send the command
	//si475x_command(5, cmd, 0, NULL);

	// Configure the IC link pin configuration.
	//cmd[0] = IC_LINK_PIN_CFG;
	//cmd[1] = 0;   // Do not modify
	//cmd[2] = 0;   // Do not modify
	//cmd[3] = 10;  // Slave
	//cmd[4] = 14;  // I2S Output Port 2
	
	// Send the command
	//si475x_command(5, cmd, 0, NULL);
}

//-----------------------------------------------------------------------------
// Place the Si47xx into powerdown mode.
//-----------------------------------------------------------------------------
void si475xFMRX_powerdown(void)
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
// initializing the RDS and STC interrupts.
//
// Note:
//     * RDS is only available on certain parts.  Please refer to the data
//       sheet for your part to determine if your part supports RDS.
//-----------------------------------------------------------------------------
static void si475xFMRX_hardware_cfg(void)
{
	// Enable the RDS and STC interrupt here
    si475x_set_property(INT_CTL_ENABLE, 0x0005);
}

//-----------------------------------------------------------------------------
// Set up general configuration properties:
//      Soft Mute Rate, Soft Mute Max Attenuation, Soft Mute SNR Threshold,
//      Blend Mono Threshold, Blend Stereo Threshold, Max Tune Error,
//      Seek Tune SNR Threshold, Seek Tune RSSI Threshold
//
// Note:
//     * RDS is only available on certain parts.  Please refer to the data
//       sheet for your part to determine if your part supports RDS.
//-----------------------------------------------------------------------------
static void si475xFMRX_general_cfg(void)
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
    si475x_set_property(FM_VALID_SNR_THRESHOLD, g_fmSnr);
    si475x_set_property(FM_VALID_RSSI_THRESHOLD, g_fmRssi);
}

//-----------------------------------------------------------------------------
// Set up regional configuration properties including:
//      Seek Band Bottom, Seek Band Top, Seek Freq Spacing, Deemphasis
//
// Inputs:
//     country
//
// Note:
//     * RDS is only available on certain parts.  Please see the part's
//       datasheet for more information.
//-----------------------------------------------------------------------------
static void si475xFMRX_regional_cfg(country_enum country)
{
    // Typically the settings used for stereo blend are determined by the 
    // designer and not exposed to the end user. They should be adjusted here.
    // If the user wishes to force mono set the stereo separation max and min to
	// zero.
    // si475x_set_property(FM_BLEND_SNR_STEREO_THRESHOLD, 25);
    // si475x_set_property(FM_BLEND_MULTIPATH_STEREO_THRESHOLD, 50);
    // si475x_set_property(FM_BLEND_MULTIPATH_MONO_THRESHOLD, 90);
	// si475x_set_property(FM_BLEND_STEREO_SEPARATION, 0x4000);  // 0 for mono

    // Depending on the country, set the de-emphasis, band, and space settings
    // Also optionally enable RDS for countries that support it
    switch (country) {
    case USA :
        // This interrupt will be used to determine when RDS is available.
        si475x_set_property(FM_RDS_INTERRUPT_SOURCE, 0x0002); // RDS Sync Interrupt

		// Enable the RDS and allow all blocks so we can compute the error
        // rate later.
        si475x_set_property(FM_RDS_CONFIG, 0x00F1);

		// Set the De-emphasis
        si475x_set_property(AUDIO_DE_EMPHASIS, 0); // 0 = 75us, 1 = 50us
        // Band is already set to 87.5-107.9MHz (US)
        // Space is already set to 200kHz (US)
        si475x_set_property(FM_SEEK_FREQUENCY_SPACING, 10); // 100 kHz Spacing

        break;
    case JAPAN :
        si475x_set_property(FM_RDS_CONFIG, 0);              // Disable RDS
        si475x_set_property(AUDIO_DE_EMPHASIS, 1);          // 0 = 75us, 1 = 50us
        si475x_set_property(FM_SEEK_BAND_BOTTOM, 7600);     // 76 MHz Bottom
        si475x_set_property(FM_SEEK_BAND_TOP, 9000);        // 90 MHz Top
        si475x_set_property(FM_SEEK_FREQUENCY_SPACING, 10); // 100 kHz Spacing
        break;
    case EUROPE :
    default:
        // This interrupt will be used to determine when RDS is available.
        si475x_set_property(FM_RDS_INTERRUPT_SOURCE, 0x0002); // RDS Sync Interrupt

		// Enable the RDS and allow all blocks so we can compute the error
        // rate later.
        si475x_set_property(FM_RDS_CONFIG, 0x00F1);

        si475x_set_property(AUDIO_DE_EMPHASIS, 1); // 0 = 75us, 1 = 50us
        // Band is already set to 87.5-107.9MHz (Europe)
        si475x_set_property(FM_SEEK_FREQUENCY_SPACING, 10); // 100 kHz Spacing
        break;
    }
}

//-----------------------------------------------------------------------------
// Sets the tracking filter properties to the values read from the daugthercard
// EEPROM.  These values should be calibrated using the chip calibration 
// procedure and stored into memory so they can be set prior to the first tune 
// after powerup.
//-----------------------------------------------------------------------------
static void si475xFMRX_front_end_cfg(void)
{
	// Depending on the front end supported by the EVB daughtercard configure
	// the device for the board configuration
	
	// Single Ended LNA
	if(DC_Type == 5 || DC_Type == 7)
    {
        si475x_set_property(FM_AGC_FE_CONFIG, 0x0080);
		si475x_set_property(FM_AGC_PD_CONFIG, 0x0270);
	}
    else if(DC_Type == 6) // LNA Balun
    {
        si475x_set_property(FM_AGC_FE_CONFIG, 0x0000);
		si475x_set_property(FM_AGC_PD_CONFIG, 0x0270);
	} 
	else  // Tracking Filter
    {
		si475x_set_property(FM_AGC_FE_CONFIG, 0x0001);
		si475x_set_property(FM_AGC_PD_CONFIG, 0x0230);
   	
        // If the calibration coefficients are stored correctly then write
	    // them to the appropriate properties.
	    if(calCap != 0xFFFF && calFmax != 0xFFFF)
	    {
		    si475x_set_property(FM_CAL_CAPACITOR, calCap);
		    si475x_set_property(FM_CAL_FMAX, calFmax);
	    }
    }  
}

//-----------------------------------------------------------------------------
// Configures the device for normal operation
//-----------------------------------------------------------------------------
void si475xFMRX_configure(void)
{

    si475xFMRX_output_config(); // Configure the output mode
	// Configure all other registers
    si475xFMRX_hardware_cfg();
    si475xFMRX_general_cfg();
    si475xFMRX_regional_cfg(USA);
	// Configure the tracking filter
	si475xFMRX_front_end_cfg();

}

//-----------------------------------------------------------------------------
// Resets the part and initializes registers to the point of being ready for
// the first tune or seek.
//-----------------------------------------------------------------------------
void si475xFMRX_initialize(void)
{
    // Zero status registers.
	PoweredUp = 0;

    // Perform a hardware reset, power up the device, and then perform the
    // initial configuration.
    si475x_reset();             // Reset the device
    si475xFMRX_powerup();       // Issue the power up command
	si475xFMRX_mute(1);
    si475xFMRX_configure();
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
void si475xFMRX_set_volume(u8 volume)
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
void si475xFMRX_mute(u8 mute)
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
//      quick:  when set to high the tune will quick tune and return to the
//              previous station in order to check the other stations metrics.
//      frequency:  frequency in 10kHz steps
//
// Returns:
//      The RSSI level found during the tune.
//-----------------------------------------------------------------------------
u8 si475xFMRX_tune(u8 quick, u16 frequency)
{
	u8 SeekTimeOut =0;
	// Enable the bit used for the interrupt of STC.
    ////SeekTuneInProc = 1;
	// Call the tune command to start the tune.
    fmTuneFreq(quick, frequency);

    // wait for the interrupt before continuing
    // If you do not wish to use interrupts but wish to poll the part
    // then comment out this line.
//    while (WaitSTCInterrupt); // Wait for interrupt to clear the bit

    while (!(getIntStatus() & STCINT)){
             SeekTimeOut++;
             wait_ms(100);
			 if(SeekTimeOut>5){
                    break;
              }
    }
	if ( SeekTimeOut>5)
      {
           printf("-----FM wait for tune STCINT [%d] ---------\n",SeekTimeOut );
     }

// Wait for stc bit to be set
//    while (!(getIntStatus() & STCINT));

	// Clear the STC bit and get the results of the tune.
    fmRsqStatus(1, 0, 1);

	// Disable the bit used for the interrupt of STC.
    //SeekTuneInProc = 0;

//printf("--->line:%d RSSI %d\n",RSSI,__LINE__);
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
u8 si475xFMRX_seek(u8 seekup, u8 seekmode)
{
	// Enable the bit used for the interrupt of STC.
    //SeekTuneInProc = 1;

	// Call the tune command to start the seek.
    fmSeekStart(seekup, !seekmode);

    // wait for the interrupt before continuing
    // If you do not wish to use interrupts but wish to poll the part
    // then comment out these two lines.
//    while (WaitSTCInterrupt); // Wait for interrupt to clear the bit

    // Wait for stc bit to be set
    // If there is a display to update seek progress, then you could
    // call fmTuneStatus in this loop to get the current frequency.
    // When calling fmTuneStatus here make sure intack is zero.
//    while (!(getIntStatus() & STCINT));
     u8 SeekTimeOut =0;
     while (!(getIntStatus() & STCINT)){
    SeekTimeOut++;
    wait_ms(FM_UNITTIMEOUT);
    if(SeekTimeOut>FM_SEEKTIMEOUT){
    break;
    }
    
   }

if ( SeekTimeOut>FM_SEEKTIMEOUT )
{
printf("-----FM wait for seek STCINT[%d]---------\n", SeekTimeOut );
}

	// Clear the STC bit and get the results of the tune.
    fmRsqStatus(1, 0, 1);

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
//      frequency in 10kHz steps
//-----------------------------------------------------------------------------
u16 si475xFMRX_get_frequency()
{
	// Get the tune status which contains the current frequency
    fmRsqStatus(1, 0, 0);

    // Return the frequency
    return Freq;
}

//-----------------------------------------------------------------------------
// Returns the current RSSI value returned from the part
//
// Returns:
//      frequency in 10kHz steps
//-----------------------------------------------------------------------------
u8 si475xFMRX_get_rssi()
{
	// When attune is set to 0 the RSSI returned will be the RSSI at the time
	// the RSQ status command was called.  If 1 is set to attune then the RSSI
	// measured during the last tune or seek will be returned.
	fmRsqStatus(0, 0, 0); 

    // Return the RSSI level
    return RSSI;
}
//-----------------------------------------------------------------------------
// Helper function that sends the FM_TUNE_FREQ command to the part
//
// Inputs:
// 	frequency in 10kHz steps
//-----------------------------------------------------------------------------
static void fmTuneFreq(u8 quick, u16 frequency)
{
    // Put the ID for the command in the first byte.
    cmd[0] = FM_TUNE_FREQ;

	// Depending on what tune functionality is needed or in the appropriate bits
    cmd[1] = 0;

	// Determine the injection side
	cmd[1] |= 0; // 0 = Automatic, 1 = Low-side, 2 = High-side

	// Determine if the metrics should be smoothly transitioned after a tune
	cmd[1] |= 0; // 0 = Metrics match new channel, 4 = trans. from prev. channel

	// Determine the tune mode
	if(quick)
	{
		cmd[1] |= 0x30; // 0 = Validated normal tune, 0x10 = unvalidated fast tune
					    // 0x20 = Validated AF tune, 0x30 = Validated AF check
	}
	else
	{
		cmd[1] |= 0x00; // 0 = Validated normal tune, 0x10 = unvalidated fast tune
					    // 0x20 = Validated AF tune, 0x30 = Validated AF check
	}

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
// Helper function that sends the FM_SEEK_START command to the part
//
// Inputs:
// 	seekUp: If non-zero seek will increment otherwise decrement
//  wrap:   If non-zero seek will wrap around band limits when hitting the end
//          of the band limit.
//-----------------------------------------------------------------------------
static void fmSeekStart(u8 seekUp, u8 wrap)
{
    // Put the ID for the command in the first byte.
    cmd[0] = FM_SEEK_START;

	// Put the flags if the bit was set for the input parameters.
	cmd[1] = 0;
    if(seekUp) cmd[1] |= 0x08;
	if(wrap)   cmd[1] |= 0x04;

    // Invoke the command
	si475x_command(2, cmd, 1, rsp);
}
//-----------------------------------------------------------------------------
// Helper function that sends the FM_RSQ_STATUS command to the part
//
// Inputs:
// 	attune: If non-zero the metrics will be the current values of the metrics
//          for the station.  If zero the metrics will be the value found at
//          the last seek/tune command.
//  cancel: If non-zero the currently running tune/seek will be cancelled
//  intack: If set the STC interrupt will be cleared.
//
//  Status:  Contains bits about the status returned from the part.
//  RsqInts: Contains bits about the interrupts that have fired related to RSQ.
//  Valid:   The station is valid if this is non-zero
//  BLTF:    If the tune/seek failed this bit will be non-zero
//  Freq:    The currently tuned frequency
//  RSSI:    The current signal strength of the station
//  ASNR:    The current audio SNR level of the station
//  ASSI:    The current adjacent channel strength for the largest channel
//           above or below the currently tuned station.
//  MULT:    The current multipath value of the station
//  FreqOff: The frequency offset in kHz of the current station from the tuned 
//           frequency.
//  DEV:     The current deviation of the station
//-----------------------------------------------------------------------------
static void fmRsqStatus(u8 attune, u8 cancel, u8 intack)
{
    u8    DEV;
    u8    MULT;
    u8    ASSI;
    u8    FreqOff;
    i8    ASNR;

    // Put the ID for the command in the first byte.
    cmd[0] = FM_RSQ_STATUS;

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
	ASSI   = rsp[9];
	MULT   = rsp[11];
	DEV    = rsp[12];
    printf("[%s] RSSI %d ASNR %d Freq %d Valid %d ASSI %d\n",__func__,RSSI,ASNR,Freq,Valid, ASSI);
}

//-----------------------------------------------------------------------------
// Helper function that sends the FM_RDS_STATUS command to the part
//
// Inputs:
//  intack: If non-zero the interrupt for STCINT will be cleared.
//  mtfifo: If non-zero the fifo will be cleared.
//
// Outputs:
//  Status:      Contains bits about the status returned from the part.
//  RdsInts:     Contains bits about the interrupts that have fired related to RDS.
//  RdsSync:     If non-zero the RDS is currently synced.
//  GrpLost:     If non-zero some RDS groups were lost.
//  TP:          Set if there is a traffic program present
//  PTY:         The program type
//  PI:          The PI code for the tuned station
//  RdsFifoUsed: The amount of groups currently remaining in the RDS fifo.
//  BleA:        Block A corrected error information.
//  BleB:        Block B corrected error information.
//  BleC:        Block C corrected error information.
//  BleD:        Block D corrected error information.
//  BlockA:      Block A group data from the oldest FIFO entry.
//  BlockB:      Block B group data from the oldest FIFO entry.
//  BlockC:      Block C group data from the oldest FIFO entry.
//  BlockD:      Block D group data from the oldest FIFO entry.
//-----------------------------------------------------------------------------
void fmRdsStatus(u8 intack, u8 mtfifo)
{
    // Put the ID for the command in the first byte.
    cmd[0] = FM_RDS_STATUS;

	// Put the flags if the bit was set for the input parameters.
	cmd[1] = 0;
	if(intack) cmd[1] |= 0x1;
    if(mtfifo) cmd[1] |= 0x2;

    // Invoke the command
	si475x_command(2, cmd, 16, rsp);

    // Parse the results
	Status      = rsp[0];
    RdsInts     = rsp[1];
	RdsSync     = !!(rsp[2] & 0x02);
    GrpLost     = !!(rsp[2] & 0x02);
	TP			= !!(rsp[3] & 0x20);
	PTY			= rsp[3] & 0x1F;
    PI          = ((u16)rsp[4]  << 8) | (u16)rsp[5];
    RdsFifoUsed = rsp[6];
    BleA        = (rsp[7] & 0xC0) >> 6;
    BleB        = (rsp[7] & 0x30) >> 4;
    BleC        = (rsp[7] & 0x0C) >> 2;
    BleD        = (rsp[7] & 0x03) >> 0;
    BlockA      = ((u16)rsp[8]  << 8) | (u16)rsp[9];
    BlockB      = ((u16)rsp[10] << 8) | (u16)rsp[11];
    BlockC      = ((u16)rsp[12] << 8) | (u16)rsp[13];
    BlockD      = ((u16)rsp[14] << 8) | (u16)rsp[15];
}


#ifdef TEST
int main(int argc, char *argv[])
{

    u8 seek_fail;
    u16 current_frequency;
    int num_found = 0;
    char buf[32]; 

   if(argc == 1)
   {
	printf("usage: ./fm <seek> <rssi> <snr>\n");
	printf("usage: ./fm <tune> <station>\n");
	return -1;
   }   
   if(!strcmp(argv[1],"seek"))
   {

   setFmSnrRssi(atoi(argv[3]),atoi(argv[2]));
   printf("g_fmRssi = %d g_fmSnr = %d\n",g_fmSnr, g_fmRssi);
   si475xFMRX_initialize();
   si475xFMRX_set_volume(63);
   si475xFMRX_mute(0);
   si475x_getPartInformation(sizeof(buf),buf);

	   si475xFMRX_tune(0,8750);

		   sleep(3);
	   seek_fail = si475xFMRX_seek(1, 1);  // seek up, stop at limits
		   while((seek_fail == 0)) 
		   {
			   printf("seek_fail = %d\n",seek_fail);
	   		current_frequency = si475xFMRX_get_frequency();
			   num_found++;
			   printf("%d found\n",current_frequency);
			   sleep(3);
			   
			seek_fail = si475xFMRX_seek(1, 1); // seek up, stop at limts
		   }
	   printf("num_found %d\n",num_found);
   }else if(!strcmp(argv[1],"tune"))
  {
		
   setFmSnrRssi(12,8);
   si475xFMRX_initialize();
   si475xFMRX_set_volume(63);
   si475xFMRX_mute(0);
   si475x_getPartInformation(sizeof(buf),buf);

	   si475xFMRX_tune(0,atoi(argv[2]));
	   sleep(-1);
  }
   si475xFMRX_mute(1);
   si475xFMRX_powerdown();
}

#endif
