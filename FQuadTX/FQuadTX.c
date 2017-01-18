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
#include "FQuadTXPad.h"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

int main( void )
{
	FStatus status;
	bool isButtonPressed; // TEMP
	
	// Initialize power
	status = FQuadTXPower_Init();
	require_noerr( status, exit );
	
	// Hold power enable line to keep the regulator on, even after the user releases the power button
	status = FQuadTXPower_Hold();
	require_noerr( status, exit );
	
	// Initialize LED indicator
	status = FQuadTXLED_Init();
	require_noerr( status, exit );
	
	// Turn on the LED to show the system is on
	status = FQuadTXLED_On();
	require_noerr( status, exit );
	
	// Initialize buttons, joysticks and triggers
	status = FQuadTXPad_Init();
	require_noerr( status, exit );
	
	while(1)
	{
		_delay_ms( 200 );
				
		
		// TEMP turning off power
		status = FQuadTXPad_ReadButtonState( FQuadTXPadButton_Start, &isButtonPressed );
		require_noerr( status, exit );
		
		if ( isButtonPressed )
		{
			status = FQuadTXPower_Release();
			require_noerr( status, exit );
		}	
	}
	
exit:
	while ( 1 )
	{
		FQuadTXLED_Toggle();
		_delay_ms( 40 );
	}
}