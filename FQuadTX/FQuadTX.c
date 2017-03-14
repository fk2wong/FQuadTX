/*
 * FQuadTX.c
 *
 * Created: 2016-09-06 11:55:48 AM
 *  Author: Felix
 */ 


#include "Platform_FQuadTX.h"
#include "FQuadTXLogging.h"
#include "require_macros.h"
#include "FQuadTXPower.h"
#include "FQuadTXLED.h"
#include "FQuadComms.h"
#include "FQuadTXControls.h"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#define LOOP_PERIOD_MS ( 100 )
#define LED_FLASH_PERIOD_MS ( 500 )
#define NUM_LOOPS_PER_TOGGLE ( LED_FLASH_PERIOD_MS / LOOP_PERIOD_MS )

int main( void )
{
	FStatus status;
	
	uint32_t loopCount = 0;
	
	FQuadAxisValue pitch;
	FQuadAxisValue roll;
	FQuadAxisValue yaw;
	FQuadThrustValue thrust;
	
	// Initialize power
	status = FQuadTXPower_Init();
	require_noerr( status, exit );
	
	// Hold power enable line to keep the regulator on, even after the user releases the power button
	status = FQuadTXPower_Hold();
	require_noerr( status, exit );
	
	// Initialize comms
	status = FQuadComms_Init( FQuadTXGPIO_XBeeSleep );
	require_noerr( status, exit );
	
	// Initialize controls
	status = FQuadTXControls_Init();
	require_noerr( status, exit );
	
	while(1)
	{	
		_delay_ms( LOOP_PERIOD_MS );
		
		// Read the user input buttons, should be called frequently to read quick button presses
		status = FQuadTXControls_ReadUserInput();
		require_noerr( status, exit );
		
		// Get updated controls
		status = FQuadTXControls_GetUpdatedControls( &pitch, &roll, &yaw, &thrust );
		require_noerr( status, exit );
		
		// Send the updated controls
		status = FQuadComms_SendControls( pitch, roll, yaw, thrust );
		
		// If the send timed out, then flash the LED quickly
		if ( status == FStatus_Timeout )
		{
			FQuadTXLED_Flash( 5 );
		}
		else
		{
			// Flash LED once to show that we're still doing something
			if ( loopCount % NUM_LOOPS_PER_TOGGLE == 0 )
			FQuadTXLED_Toggle();
		}
		
		// TODO: Low RSSI check
		
		// Power off if the user has requested
		status = FQuadTXPower_CheckPowerOffRequest();
		require_noerr( status, exit );
		
		loopCount++;
	}
	
exit:
	while ( 1 )
	{
		FQuadTXLED_Toggle();
		_delay_ms( 50 );
	}
}