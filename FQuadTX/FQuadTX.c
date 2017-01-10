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
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

int main(void)
{
	FStatus status;
	
	// TEMP
	status = PlatformGPIO_Configure(FQuadTXGPIO_GreenLED, PlatformGPIOConfig_Output );
	require_noerr( status, exit );
	//
	
	// Initialize power
	status = FQuadTXPower_Init();
	require_noerr( status, exit );
	
	// Hold power enable line to keep the regulator on, even after the user releases the power button
	status = FQuadTXPower_Hold();
	require_noerr( status, exit );
	
    while(1)
    {	
		// Test GPIO
		status = PlatformGPIO_Toggle( FQuadTXGPIO_GreenLED );
		require_noerr( status, exit );
		
		_delay_ms( 500 );
    }
	
exit:
	while ( 1 )
	{
		PlatformGPIO_Toggle( FQuadTXGPIO_GreenLED );
		_delay_ms( 100 );
	}
}