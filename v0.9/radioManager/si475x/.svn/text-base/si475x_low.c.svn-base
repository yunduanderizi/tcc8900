//-----------------------------------------------------------------------------
//
// si475x_low.c
//
// Contains the low level, hardware functions
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stddef.h>
#include "typedefs.h"
#include "commanddefs.h"
#include "io.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
unsigned char PoweredUp = 0;

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void wait_us(u16 us);
void wait_ms(u16 ms);


//-----------------------------------------------------------------------------
// Reset the Si475x and select the appropriate bus mode.
//-----------------------------------------------------------------------------
void si475x_reset(void)
{
    io2w_init();
    return;

}
//-----------------------------------------------------------------------------
// This command returns the status
//-----------------------------------------------------------------------------
u8 si475x_readStatus()
{
    u8 status;
   
    io2w_read(1, &status);

    return status;
}
//-----------------------------------------------------------------------------
// Command that will wait for CTS before returning
//-----------------------------------------------------------------------------
void si475x_waitForCTS()
{
	u16 i=100;

    // Loop until CTS is found or stop due to the counter running out.
    while (--i && !(si475x_readStatus() & CTS))
    {
        wait_us(1000);
    }

    // If the i is equal to 0 then something must have happened.
    // It is recommended that the controller do some type of error
    // handling in this case.
}

//-----------------------------------------------------------------------------
// Sends a command to the part and returns the reply bytes
//-----------------------------------------------------------------------------
int si475x_command(u8 cmd_size, u8 *cmd, u8 reply_size, u8 *reply)
{
	int res = -1;
    // It is always a good idea to check for cts prior to sending a command to
    // the part.
    si475x_waitForCTS();
    // Write the command to the part
	res = io2w_write(cmd_size, cmd);
	if(res  <0 )
		return res;

    // Wait for CTS after sending the command
    si475x_waitForCTS();

    // If the calling function would like to have results then read them.
    if(reply_size)
    {
	   res = io2w_read(reply_size, reply);
    }
	return res;
}
//-----------------------------------------------------------------------------
// Helper function that sends the GET_INT_STATUS command to the part
//
// Returns:
//   The status byte from the part.
//-----------------------------------------------------------------------------
u8 getIntStatus(void)
{
    u8 cmd[1] = {0};
    u8 rsp[1] = {0};

    // Put the ID for the command in the first byte.
    cmd[0] = GET_INT_STATUS;

    // Invoke the command
	si475x_command(1, cmd, 1, rsp);

	// Return the status
	return rsp[0];
}

//-----------------------------------------------------------------------------
// Set the passed property number to the passed value.
//
// Inputs:
//      propNumber:  The number identifying the property to set
//      propValue:   The value of the property.
//-----------------------------------------------------------------------------
void si475x_set_property(u16 propNumber, u16 propValue)
{
     u8   cmd[6]={0};

    // Put the ID for the command in the first byte.
    cmd[0] = SET_PROPERTY;

	// Initialize the reserved section to 0
    cmd[1] = 0;

	// Put the property number in the third and fourth bytes.
    cmd[2] = (u8)(propNumber >> 8);
	cmd[3] = (u8)(propNumber & 0x00FF);

	// Put the property value in the fifth and sixth bytes.
    cmd[4] = (u8)(propValue >> 8);
    cmd[5] = (u8)(propValue & 0x00FF);

    // Invoke the command
	si475x_command(6, cmd, 0, NULL);
}

int si475x_getPartInformation(int size,char * buf)
{
    char revision;
    u8 partNumber;
    char partMajor;
    char partMinor;
    char partBuild;
    u8   romID;
    u8 rsp[9] = {0,0,0,0,0,0,0,0,0};
    u8   cmd[1]={0};

    // NOTE:  This routine can be called either when the part is in the boot loader
    //        mode or fully operational.

    // Put the ID for the command in the first byte.
    cmd[0] = PART_INFO;

    // Invoke the command
    si475x_command(1, cmd, 9, rsp);

    // Now take the result and put in the variables we have declared
    // Status is in the first element of the array so skip that.
    revision   = (char)rsp[1];
    partNumber = rsp[2];
    partMajor  = (char)rsp[3];
    partMinor  = (char)rsp[4];
    partBuild  = (char)rsp[5];
    romID      = rsp[8];
    printf("------>[%s]:[%d.%d.%d.%d.%d.%d] \n",__func__,revision,partNumber,partMajor,partMinor,partBuild,romID);
    if(0==revision &&
         0==partNumber  &&
            0==partMajor  &&
            0==partMinor  &&
            0==partBuild  &&
            0==romID)
        return -1;
    if(size>=9)
    {
        memcpy(buf,rsp,9);
    }
    else
    {
        memcpy(buf,rsp,size);
    }
    return 0;
}
