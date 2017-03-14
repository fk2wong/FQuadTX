/*
 * FQuadTXControls.c
 *
 * Created: 2017-03-13 4:22:24 PM
 *  Author: Felix
 */ 

#include "FQuadTXControls.h"
#include "FQuadTXPad.h"
#include "require_macros.h"
#include <stdbool.h>

#define FQUADTXCONTROLS_PITCH_ROLL_INCREMENT ( 5 )
#define FQUADTXCONTROLS_THRUST_INCREMENT     ( 1 )

#define FQUADTXCONTROLS_MAX_AXIS_VALUE       ( 0x7F )
#define FQUADTXCONTROLS_MIN_AXIS_VALUE       ( -FQUADTXCONTROLS_MAX_AXIS_VALUE )

#define FQUADTXCONTROLS_MIN_THRUST_VALUE     ( 0 )
#define FQUADTXCONTROLS_MAX_THRUST_VALUE     ( 0xFF )

typedef struct FQuadTXControlsStruct
{
	FQuadAxisValue basePitch;
	FQuadAxisValue baseRoll;
	FQuadAxisValue baseYaw;
	FQuadThrustValue baseThrust;
	
	uint16_t lastButtonStates;
	bool waitingForTriggersToRelease;
	
} FQuadTXControlsStruct_t;

static FQuadTXControlsStruct_t mControlsInfo;


static FStatus _FQuadTXControls_GetControls( FQuadAxisValue *const outPitch,
											 FQuadAxisValue *const outRoll,
											 FQuadAxisValue *const outYaw,
											 FQuadThrustValue *const outThrust );
											 
static FStatus _FQuadTXControls_GetNewPressesAndUpdateState( uint16_t *const newPresses );

FStatus FQuadTXControls_Init( void )
{
	FStatus status;
	
	// Initialize the pad (may already be initialized)
	status = FQuadTXPad_Init();
	require( status == FStatus_Success || status == FStatus_AlreadyInitialized, exit );
	
	status = FStatus_Success;
exit:
	return status;
}

FStatus FQuadTXControls_ReadUserInput( void )
{
	FStatus status;
	uint16_t buttonsNewlyPressed;
	
	FQuadThrustValue thrust;
	
	int16_t temp;
	
	status = _FQuadTXControls_GetNewPressesAndUpdateState( &buttonsNewlyPressed );
	require_noerr( status, exit );
	
	// Set the base pitch/roll/thrust
	if ( buttonsNewlyPressed & FQuadTXPadButton_DUp )
	{
		temp = mControlsInfo.basePitch - FQUADTXCONTROLS_PITCH_ROLL_INCREMENT;
		mControlsInfo.basePitch = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( buttonsNewlyPressed & FQuadTXPadButton_DDown )
	{
		temp = mControlsInfo.basePitch + FQUADTXCONTROLS_PITCH_ROLL_INCREMENT;
		mControlsInfo.basePitch = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( buttonsNewlyPressed & FQuadTXPadButton_DRight )
	{
		temp = mControlsInfo.baseRoll + FQUADTXCONTROLS_PITCH_ROLL_INCREMENT;
		mControlsInfo.baseRoll = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( buttonsNewlyPressed & FQuadTXPadButton_DLeft )
	{
		temp = mControlsInfo.baseRoll - FQUADTXCONTROLS_PITCH_ROLL_INCREMENT;
		mControlsInfo.baseRoll = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( buttonsNewlyPressed & FQuadTXPadButton_Y )
	{
		temp = mControlsInfo.baseThrust + FQUADTXCONTROLS_THRUST_INCREMENT;
		mControlsInfo.baseThrust = ( FQuadThrustValue ) CLIP( temp, FQUADTXCONTROLS_MIN_THRUST_VALUE, FQUADTXCONTROLS_MAX_THRUST_VALUE );
	}
	if ( buttonsNewlyPressed & FQuadTXPadButton_A )
	{
		temp = mControlsInfo.baseThrust - FQUADTXCONTROLS_THRUST_INCREMENT;
		mControlsInfo.baseThrust = ( FQuadThrustValue ) CLIP( temp, FQUADTXCONTROLS_MIN_THRUST_VALUE, FQUADTXCONTROLS_MAX_THRUST_VALUE );
	}
	
	// Set the base thrust (shortcut)
	if ( buttonsNewlyPressed & FQuadTXPadButton_X )
	{
		status = _FQuadTXControls_GetControls( NULL, NULL, NULL, &thrust );
		require_noerr( status, exit );
		
		mControlsInfo.baseThrust = thrust;
		mControlsInfo.waitingForTriggersToRelease = true;
	}
	
	// If start is pressed, reset the pitch/roll
	if ( buttonsNewlyPressed & FQuadTXPadButton_Start )
	{
		mControlsInfo.basePitch = 0;
		mControlsInfo.baseRoll = 0;
	}
	
	// If center button is pressed, set pitch/roll/thrust to 0
	if ( buttonsNewlyPressed & FQuadTXPadButton_Center )
	{
		mControlsInfo.basePitch = 0;
		mControlsInfo.baseRoll = 0;
		mControlsInfo.baseThrust = 0;
	}
	
	status = FStatus_Success;
exit:
	return status;
}

FStatus FQuadTXControls_GetUpdatedControls( FQuadAxisValue *const outPitch,
											FQuadAxisValue *const outRoll,
											FQuadAxisValue *const outYaw,
											FQuadThrustValue *const outThrust )
{
	FStatus status;
	
	status = _FQuadTXControls_GetControls( outPitch, outRoll, outYaw, outThrust );
	require_noerr( status, exit );
	
	status = FStatus_Success;
exit:
	return status;		
}

static FStatus _FQuadTXControls_GetControls( FQuadAxisValue *const outPitch,
											 FQuadAxisValue *const outRoll,
											 FQuadAxisValue *const outYaw,
											 FQuadThrustValue *const outThrust )
{
	FStatus status;
	int8_t leftVert;
	int8_t leftHorz;
	int8_t rightHorz;
	
	uint8_t leftTrig;
	uint8_t rightTrig;
	
	int16_t temp;
	
	status = FQuadTXPad_GetJoystickValues( &leftVert, &leftHorz, NULL, &rightHorz );
	require_noerr( status, exit );
	
	status = FQuadTXPad_GetTriggerValues( &leftTrig, &rightTrig );
	require_noerr( status, exit );
	
	if ( outPitch )
	{
		// Subtract mapped ADC value, because positive ADC indicates a negative pitch
		temp = mControlsInfo.basePitch - ((( int16_t )leftVert * FQUADTXCONTROLS_MAX_AXIS_VALUE ) / PAD_MAX_ANALOG_VALUE );
		*outPitch = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( outRoll )
	{
		temp = mControlsInfo.baseRoll + ((( int16_t )leftHorz * FQUADTXCONTROLS_MAX_AXIS_VALUE ) / PAD_MAX_ANALOG_VALUE );
		*outRoll = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( outYaw )
	{
		temp = mControlsInfo.baseYaw + ((( int16_t )rightHorz * FQUADTXCONTROLS_MAX_AXIS_VALUE ) / PAD_MAX_ANALOG_VALUE );
		*outYaw = ( FQuadAxisValue ) CLIP( temp, FQUADTXCONTROLS_MIN_AXIS_VALUE, FQUADTXCONTROLS_MAX_AXIS_VALUE );
	}
	if ( outThrust )
	{
		// If waiting for triggers to release (caused by setting the base thrust using X shortcut), then only use base thrust
		if ( mControlsInfo.waitingForTriggersToRelease )
		{
			// Reset the flag once triggers are released
			if ( !leftTrig && !rightTrig )
			{
				mControlsInfo.waitingForTriggersToRelease = false;
			}
			
			// Use only base thrust for now
			*outThrust = mControlsInfo.baseThrust;	
		}
		else
		{
			temp = mControlsInfo.baseThrust + (((( int16_t )rightTrig - leftTrig ) * FQUADTXCONTROLS_MAX_THRUST_VALUE ) / PAD_MAX_ANALOG_VALUE );
			*outThrust = ( FQuadThrustValue ) CLIP( temp, FQUADTXCONTROLS_MIN_THRUST_VALUE, FQUADTXCONTROLS_MAX_THRUST_VALUE );
		}
	}
	
	status = FStatus_Success;
exit:
	return status;
}

static FStatus _FQuadTXControls_GetNewPressesAndUpdateState( uint16_t *const newPresses )
{
	FStatus status;
	uint16_t currentButtonStates;
	
	status = FQuadTXPad_ReadAllButtonStates( &currentButtonStates );
	require_noerr( status, exit );
	
	// XOR isolates changed bits, AND isolates changes from 0 to 1
	*newPresses = ( currentButtonStates ^ mControlsInfo.lastButtonStates ) & currentButtonStates;
	
	mControlsInfo.lastButtonStates = currentButtonStates;

	status = FStatus_Success;
exit:
	return status;
}