/*
 * FQuadTX.c
 *
 * Created: 2016-09-06 11:55:48 AM
 *  Author: Felix
 */ 




#include "Platform_FQuadTX.h"
#include "FQuadLogging.h"
#include "require_macros.h"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

int main(void)
{
	PlatformStatus status;
	PlatformRingBuffer* ringBuf = NULL;
	
	status = PlatformPowerSave_PowerOffAllPeripherals();
	require_noerr_quiet( status, exit );
	
	status = PlatformGPIO_Configure( FQuadTXGPIO_TestLED, PlatformGPIOConfig_Output );
	require_noerr_quiet( status, exit );
	
	ringBuf = PlatformRingBuffer_Create( 32 );
	require_quiet( ringBuf, exit );
	
	status = PlatformUART_Init( 19200, ringBuf );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadTXPWM_MotorNW, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadTXPWM_MotorNE, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadTXPWM_MotorSW, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadTXPWM_MotorSE, 250, NULL );
	require_noerr_quiet( status, exit );
		
    while(1)
    {	
		// Test GPIO
		status = PlatformGPIO_Toggle( FQuadTXGPIO_TestLED );
		require_noerr( status, exit );
		
		// Test UART
		uint8_t rxData[6];
		status = PlatformUART_Receive( rxData, sizeof( rxData ) - 1 );
		rxData[sizeof( rxData ) - 1] = '\0';
		if ( status == PlatformStatus_Success )
		{
			FQUAD_DEBUG_LOG(( "UART Received: %s.\n", rxData ));	
		}
		
		// Test ADC
		status = PlatformADC_Init( FQuadTXADC_PadLeftVertical );
		require_noerr_quiet( status, exit );
		
		uint16_t adcVal;
		status = PlatformADC_Read( FQuadTXADC_PadLeftVertical, &adcVal );
		require_noerr( status, exit );
		
		FQUAD_DEBUG_LOG(( "ADC Read: %d\n", adcVal ));
		
		status = PlatformADC_Deinit( FQuadTXADC_PadLeftVertical );
		require_noerr_quiet( status, exit );
		
		// Test PWM outputs
		status = PlatformPWM_Start( FQuadTXPWM_MotorNW, 40 );
		require_noerr_quiet( status, exit );
		
		status = PlatformPWM_Start( FQuadTXPWM_MotorNE, 60 );
		require_noerr_quiet( status, exit );
		
		status = PlatformPWM_Start( FQuadTXPWM_MotorSW, 20 );
		require_noerr_quiet( status, exit );
				
		status = PlatformPWM_Start( FQuadTXPWM_MotorSE, 80 );
		require_noerr_quiet( status, exit );
		
		_delay_ms( 500 );
		
    }
exit:
	while ( 1 )
	{
		PlatformGPIO_Toggle( FQuadTXGPIO_TestLED );
		_delay_ms( 100 );
	}
}