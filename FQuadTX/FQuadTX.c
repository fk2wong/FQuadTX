/*
 * FQuadTX.c
 *
 * Created: 2016-09-06 11:55:48 AM
 *  Author: Felix
 */ 




#include "Platform_FQuad.h"
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
	
	status = PlatformGPIO_InitAllGPIOs();
	require_noerr_quiet( status, exit );
	
	ringBuf = PlatformRingBuffer_Create( 32 );
	require_quiet( ringBuf, exit );
	
	status = PlatformUART_Init( 19200, ringBuf );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadPWM_MotorNW, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadPWM_MotorNE, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadPWM_MotorSW, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformPWM_Init( FQuadPWM_MotorSE, 250, NULL );
	require_noerr_quiet( status, exit );
	
	status = PlatformI2C_Init();
	require_noerr_quiet( status, exit );
	
	uint8_t MPUAddr = 0;
	status = PlatformI2C_Read( 0x68, 0x75, &MPUAddr, 1 );
	
	if ( status == 0 )
	{
		FQUAD_DEBUG_LOG(( "MPU-6050 ADDR: 0x%x\n", MPUAddr ));
	}
	else
	{
		FQUAD_DEBUG_LOG(( "MPU-6050 Not Found!\n" ));	
	}
		
    while(1)
    {	
		// Test GPIO
		status = PlatformGPIO_Toggle( FQuadGPIO_TestLED );
		require_noerr( status, exit );
		
		// Test UART
		uint8_t rxData[5];
		status = PlatformUART_Receive( rxData, sizeof( rxData ));
		
		if ( status == PlatformStatus_Success )
		{
			FQUAD_DEBUG_LOG(( "UART Received: %s.\n", rxData ));	
		}
		
		// Test ADC
		status = PlatformADC_Init( FQuadADC_PadLeftVertical );
		require_noerr_quiet( status, exit );
		
		uint16_t adcVal;
		status = PlatformADC_Read( FQuadADC_PadLeftVertical, &adcVal );
		require_noerr( status, exit );
		
		FQUAD_DEBUG_LOG(( "ADC Read: %d\n", adcVal ));
		
		status = PlatformADC_Deinit( FQuadADC_PadLeftVertical );
		require_noerr_quiet( status, exit );
		
		// Test PWM outputs
		status = PlatformPWM_Start( FQuadPWM_MotorNW, 40 );
		require_noerr_quiet( status, exit );
		
		status = PlatformPWM_Start( FQuadPWM_MotorNE, 60 );
		require_noerr_quiet( status, exit );
		
		status = PlatformPWM_Start( FQuadPWM_MotorSW, 20 );
		require_noerr_quiet( status, exit );
				
		status = PlatformPWM_Start( FQuadPWM_MotorSE, 80 );
		require_noerr_quiet( status, exit );
		
		_delay_ms( 500 );
		
    }
exit:
	while ( 1 )
	{
		PlatformGPIO_Toggle( FQuadGPIO_TestLED );
		_delay_ms( 100 );
	}
}