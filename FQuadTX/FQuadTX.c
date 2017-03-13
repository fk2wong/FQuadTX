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
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

int main( void )
{
	FStatus status;
	
	// Initialize power
	status = FQuadTXPower_Init();
	require_noerr( status, exit );
	
	// Hold power enable line to keep the regulator on, even after the user releases the power button
	status = FQuadTXPower_Hold();
	require_noerr( status, exit );
	
	// Initialize comms
	status = FQuadComms_Init( FQuadTXGPIO_XBeeSleep );
	require_noerr( status, exit );
	
	
	while(1)
	{	
		_delay_ms( 1000 );
		
		// Power off if the user has requested
		status = FQuadTXPower_CheckPowerOffRequest();
		require_noerr( status, exit );
		
		FQuadTXLED_Toggle();
		_delay_ms( 50 );
		FQuadTXLED_Toggle();
	}
	
exit:
	while ( 1 )
	{
		FQuadTXLED_Toggle();
		_delay_ms( 100 );
	}
}