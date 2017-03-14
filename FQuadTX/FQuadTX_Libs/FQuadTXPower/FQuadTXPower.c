/*
 * FQuadTXPower.c
 *
 * Created: 2017-01-09 8:15:30 PM
 *  Author: Felix
 */ 

#include "FQuadTXPower.h"
#include "FQuadTXLED.h"
#include "FQuadTXPad.h"
#include "FQuadTXLogging.h"
#include "Platform_FQuadTX.h"
#include <stdbool.h>
#include <util/delay.h>

#define FQUADTX_POWER_RELEASE_REQUEST_WAIT_MS ( 2000 )
#define FQUADTXPOWER_NEVER_RETURN()  do \
                                    { \
										_delay_ms( 100 ); \
									}\
                                    while(1);

static uint32_t mLastPressedTime;
static uint32_t mShouldResetTimerOnNextPress;

FStatus FQuadTXPower_Init()
{
	FStatus status = FStatus_Failed;
	PlatformStatus platformStatus;
	
	// Power off all peripherals, they will be powered on when needed
	platformStatus = PlatformPowerSave_PowerOffAllPeripherals();
	require_noerr( platformStatus, exit );
	
	// Configure the power hold line, which is connected to the regulator enable pin through an OR gate
	platformStatus = PlatformGPIO_Configure( FQuadTXGPIO_PowerHold, PlatformGPIOConfig_Output );
	require_noerr( platformStatus, exit );
	
	// Configure the LED that indicates the power is on
	status = FQuadTXLED_Init();
	require_noerr( status, exit );
	
	// Init the pad buttons, since the center button is used to request a power off
	// Note the pad may already be init by other modules
	status = FQuadTXPad_Init();
	require(( status == FStatus_Success ) || ( status = FStatus_AlreadyInitialized ) , exit );
	
	// Initialize the timer, which is used for the soft power off
	platformStatus = PlatformTimer_Init();
	require_noerr( platformStatus, exit );
		
	platformStatus = PlatformTimer_Reset();
	require_noerr( platformStatus, exit );
	
	status = FStatus_Success;
exit:
	return status;
}

FStatus FQuadTXPower_Hold()
{
	FStatus status = FStatus_Failed;
	PlatformStatus platformStatus;
	
	// Hold power on in order to stay alive
	platformStatus = PlatformGPIO_OutputHigh( FQuadTXGPIO_PowerHold );
	require_noerr( platformStatus, exit );
	
	// Turn on the LED to show the system is on
	status = FQuadTXLED_On();
	require_noerr( status, exit );
		
	status = FStatus_Success;
exit:
	return status;
}

FStatus FQuadTXPower_Release()
{
	FStatus status = FStatus_Failed;
	PlatformStatus platformStatus;
		
	// Release power hold, which will turn this MCU off when the user releases the on button
	platformStatus = PlatformGPIO_OutputLow( FQuadTXGPIO_PowerHold );
	require_noerr( platformStatus, exit );
		
	// Stop execution
	FQUADTXPOWER_NEVER_RETURN();	
	
	status = FStatus_Success;
exit:
	return status;
}

FStatus FQuadTXPower_CheckPowerOffRequest()
{
	FStatus status = FStatus_Failed;
	uint32_t currentTime;
	bool isButtonPressed;
	
	status = FQuadTXPad_ReadButtonState( FQuadTXPadButton_Center, &isButtonPressed );
	require_noerr( status, exit );
	
	// If the button is released, then the next press should reset mLastPressedTime
	if ( !isButtonPressed )
	{
		mShouldResetTimerOnNextPress = true;
	}
	else // Button is being held
	{	
		// Reset mLastPressedTime if the button was just pressed
		if ( mShouldResetTimerOnNextPress )
		{
			status = PlatformTimer_GetTime( &mLastPressedTime );
			require_noerr( status, exit );
			mShouldResetTimerOnNextPress = false;
		}
		
		// Get the current time, and check if the button has been held for at least FQUADTX_POWER_RELEASE_REQUEST_WAIT_MS
		status = PlatformTimer_GetTime( &currentTime );
		require_noerr( status, exit );
		
		if (( currentTime - mLastPressedTime ) > FQUADTX_POWER_RELEASE_REQUEST_WAIT_MS )
		{	
			// Turn off the LED to indicate the system is shutting down
			status = FQuadTXLED_Off();
			require_noerr( status, exit );
						
			// Release the power line, don't return
			status = FQuadTXPower_Release();
			require_noerr( status, exit );
		}
	}
		
	status = FStatus_Success;
exit:
	return status;
}