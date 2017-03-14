/*
 * FQuadTXLED.c
 *
 * Created: 2017-01-09 9:33:03 PM
 *  Author: Felix
 */ 

#include "FQuadTXLED.h"
#include "require_macros.h"
#include "PlatformStatus.h"
#include "Platform_FQuadTX.h"
#include <util/delay.h>

FStatus FQuadTXLED_Init()
{
	PlatformStatus platformStatus = PlatformGPIO_Configure( FQuadTXGPIO_GreenLED, PlatformGPIOConfig_Output );

	return ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
}

FStatus FQuadTXLED_On()
{
	PlatformStatus platformStatus = PlatformGPIO_OutputHigh( FQuadTXGPIO_GreenLED );

	return ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
}

FStatus FQuadTXLED_Off()
{
	PlatformStatus platformStatus = PlatformGPIO_OutputLow( FQuadTXGPIO_GreenLED );

	return ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
}

FStatus FQuadTXLED_Toggle()
{
	PlatformStatus platformStatus = PlatformGPIO_Toggle( FQuadTXGPIO_GreenLED );

	return ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
}

FStatus FQuadTXLED_Flash( uint8_t inCycles )
{
	PlatformStatus platformStatus = FStatus_Success;
	
	for ( uint8_t i = 0; i < inCycles; i++ )
	{
		platformStatus |= PlatformGPIO_Toggle( FQuadTXGPIO_GreenLED );
		_delay_ms( 50 );
		
		platformStatus |= PlatformGPIO_Toggle( FQuadTXGPIO_GreenLED );
		_delay_ms( 50 );
	}
	
	return ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
}