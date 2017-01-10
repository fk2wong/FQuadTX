/*
 * FQuadTXPower.c
 *
 * Created: 2017-01-09 8:15:30 PM
 *  Author: Felix
 */ 

#include "FQuadTXPower.h"
#include "FQuadTXLogging.h"
#include "Platform_FQuadTX.h"

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
		
	status = FStatus_Success;
exit:
	return status;
}