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
	
	// Initialize power
	status = FQuadTXPower_Init();
	require_noerr( status, exit );
	
	// Hold power enable line to keep the regulator on, even after the user releases the power button
	status = FQuadTXPower_Hold();
	require_noerr( status, exit );
	
	// Initialize buttons, joysticks and triggers
	status = FQuadTXPad_Init();
	require_noerr( status, exit );
	
	while(1)
	{
		
		status = FQuadTXPower_CheckPowerOffRequest();	
		require_noerr( status, exit );
		
		// 		_delay_ms( 200 );
	}
	
exit:
	while ( 1 )
	{
		FQuadTXLED_Toggle();
		_delay_ms( 100 );
	}
}