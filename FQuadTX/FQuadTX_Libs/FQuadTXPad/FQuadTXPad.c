/*
 * FQuadTXPad.c
 *
 * Created: 2017-01-09 10:30:28 PM
 *  Author: Felix
 */ 

#include "FQuadTXPad.h"
#include "Platform_FQuadTX.h"
#include "FQuadTXLogging.h"

#define PAD_LEFT_VERT_MIN     ( 110 )
#define PAD_LEFT_VERT_MID     ( 512 )
#define PAD_LEFT_VERT_MAX     ( 945 )
#define PAD_LEFT_HORZ_MIN     ( 65 )
#define PAD_LEFT_HORZ_MID     ( 512 )
#define PAD_LEFT_HORZ_MAX     ( 960 )
#define PAD_RIGHT_VERT_MIN    ( 130 ) // unused (NC)
#define PAD_RIGHT_VERT_MID    ( 512 ) // unused (NC)
#define PAD_RIGHT_VERT_MAX    ( 920 ) // unused (NC)
#define PAD_RIGHT_HORZ_MIN    ( 75 )
#define PAD_RIGHT_HORZ_MID    ( 510 )
#define PAD_RIGHT_HORZ_MAX    ( 895 )

#define PAD_LEFT_TRIGGER_MIN  ( 210 )
#define PAD_LEFT_TRIGGER_MAX  ( 920 )
#define PAD_RIGHT_TRIGGER_MIN ( 425 )
#define PAD_RIGHT_TRIGGER_MAX ( 1024 )

#define PAD_BUTTONS_ARE_ACTIVE_LOW ( 1 )

#if PAD_BUTTONS_ARE_ACTIVE_LOW
	#define IS_BUTTON_PRESSED( X )  ( !( X ))
#else 
	#define IS_BUTTON_PRESSED( X )  ( X )
#endif

typedef struct FQuadTXPad_ButtonMapUnit 
{
	FQuadTXPadButton name;
	FQuadTXGPIO_t    GPIO;
} FQuadTXPad_ButtonMapUnit_t;

static FQuadTXPad_ButtonMapUnit_t mButtonMapList[PAD_NUM_BUTTONS] =
{
	{
		.name = FQuadTXPadButton_A,
		.GPIO = FQuadTXGPIO_PadA,
	},
	{
		.name = FQuadTXPadButton_B,
		.GPIO = FQuadTXGPIO_PadB,
	},
	{
		.name = FQuadTXPadButton_X,
		.GPIO = FQuadTXGPIO_PadX,
	},
	{
		.name = FQuadTXPadButton_Y,
		.GPIO = FQuadTXGPIO_PadY,
	},
	{
		.name = FQuadTXPadButton_Center,
		.GPIO = FQuadTXGPIO_PadCenter,
	},
	{
		.name = FQuadTXPadButton_Start,
		.GPIO = FQuadTXGPIO_PadStart,
	},
	{
		.name = FQuadTXPadButton_DUp,
		.GPIO = FQuadTXGPIO_PadDUp,
	},
	{
		.name = FQuadTXPadButton_DRight,
		.GPIO = FQuadTXGPIO_PadDRight,
	},
	{
		.name = FQuadTXPadButton_DDown,
		.GPIO = FQuadTXGPIO_PadDDown,
	},
	{
		.name = FQuadTXPadButton_DLeft,
		.GPIO = FQuadTXGPIO_PadDLeft,
	},
};
	

// Static Functions
static uint8_t _FQuadTXPad_ConvertRawToUnsignedPercentage( uint16_t inRawValue, const uint16_t inMinValue, const uint16_t inMaxValue );
static int8_t _FQuadTXPad_ConvertRawToSignedPercentage( const uint16_t inRawValue, const uint16_t inMidValue, const uint16_t inMinValue, const uint16_t inMaxValue );

FStatus FQuadTXPad_Init()
{	
	// Set platform status to 0, the error will be accumulated on top
	PlatformStatus platformStatus = 0;
	
	// Configure each button as input pull up, accumulate the error
	for ( uint8_t i = 0; i < PAD_NUM_BUTTONS; i++ )
	{
		platformStatus |= PlatformGPIO_Configure( mButtonMapList[i].GPIO, PlatformGPIOConfig_InputPullUp );
	}
	
	// Initialize joystick ADCs
	platformStatus |= PlatformADC_Init( FQuadTXADC_PadLeftHorz );
	platformStatus |= PlatformADC_Init( FQuadTXADC_PadLeftVert );
	platformStatus |= PlatformADC_Init( FQuadTXADC_PadRightHorz );
	platformStatus |= PlatformADC_Init( FQuadTXADC_PadRightVert );
	
	// Initialize trigger ADCs
	platformStatus |= PlatformADC_Init( FQuadTXADC_PadLeftTrigger );
	platformStatus |= PlatformADC_Init( FQuadTXADC_PadRightTrigger );
	
	return ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
}

FStatus FQuadTXPad_ReadButtonState( const FQuadTXPadButton inButton, bool* const outIsButtonPressed )
{
	FStatus status = FStatus_InvalidArgument;
	PlatformStatus platformStatus = PlatformStatus_InvalidArgument;
	bool buttonState;
	
	require( outIsButtonPressed, exit );
	
	// Loop through button list and find the corresponding GPIO
	for ( uint8_t i = 0; i < PAD_NUM_BUTTONS; i++ )
	{
		if ( inButton == mButtonMapList[i].name )
		{	
			platformStatus = PlatformGPIO_GetInput( mButtonMapList[i].GPIO, &buttonState );
			*outIsButtonPressed = IS_BUTTON_PRESSED( buttonState );
			break;
		}
	}
	
	// Check status
	status = ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
exit:
	return status;
}

FStatus FQuadTXPad_ReadAllButtonStates( uint16_t* const outButtonsPressed )
{
	FStatus status = FStatus_InvalidArgument;
	
	// Set platform status to 0, the error will be accumulated on top
	PlatformStatus platformStatus = 0;
	
	bool buttonState;
	uint16_t buttonsPressedField = 0;
	
	require( outButtonsPressed, exit );
	
	// Get all button presses into a list, and accumulate the error
	for ( uint8_t i = 0; i < PAD_NUM_BUTTONS; i++ )
	{
		platformStatus |= PlatformGPIO_GetInput( mButtonMapList[i].GPIO, &buttonState);
		if ( IS_BUTTON_PRESSED( buttonState ))
		{	
			buttonsPressedField |= mButtonMapList[i].name;
		}
	}
	
	// Output the bit field
	*outButtonsPressed = buttonsPressedField;
	
	// Check the accumulated error
	status = ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
exit:
	return status;
}

FStatus FQuadTXPad_GetJoystickValues( int8_t* const outLeftVert, int8_t* const outLeftHorz, int8_t* const outRightVert, int8_t* const outRightHorz )
{
	FStatus status = FStatus_InvalidArgument;
	PlatformStatus platformStatus = 0; // Set to 0 first, in order to accumulate error
	uint16_t rawADCValue;
	
	// At least one joystick must be specified
	require( outLeftVert || outLeftHorz || outRightVert || outRightHorz , exit );
	
	// Read the ADCs and map them into signed percentages
	if ( outLeftVert )
	{
		platformStatus |= PlatformADC_Read( FQuadTXADC_PadLeftVert, &rawADCValue );
		*outLeftVert = _FQuadTXPad_ConvertRawToSignedPercentage( rawADCValue, PAD_LEFT_VERT_MID, PAD_LEFT_VERT_MIN, PAD_LEFT_VERT_MAX );
	}
	
	if ( outLeftHorz )
	{
		platformStatus |= PlatformADC_Read( FQuadTXADC_PadLeftHorz, &rawADCValue );
		*outLeftHorz = _FQuadTXPad_ConvertRawToSignedPercentage( rawADCValue, PAD_LEFT_HORZ_MID, PAD_LEFT_HORZ_MIN, PAD_LEFT_HORZ_MAX );
	}
	
	if ( outRightVert )
	{
		platformStatus |= PlatformADC_Read( FQuadTXADC_PadRightVert, &rawADCValue );
		*outRightVert = _FQuadTXPad_ConvertRawToSignedPercentage( rawADCValue, PAD_RIGHT_VERT_MID, PAD_RIGHT_VERT_MIN, PAD_RIGHT_VERT_MAX );
	}
	
	if ( outRightHorz )
	{
		platformStatus |= PlatformADC_Read( FQuadTXADC_PadRightHorz, &rawADCValue );
		*outRightHorz = _FQuadTXPad_ConvertRawToSignedPercentage( rawADCValue, PAD_RIGHT_HORZ_MID, PAD_RIGHT_HORZ_MIN, PAD_RIGHT_HORZ_MAX );
	}
	
	// Check accumulated error
	status = ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
exit:
	return status;
}

FStatus FQuadTXPad_GetTriggerValues( uint8_t* const outLeftTrigger, uint8_t* const outRightTrigger )
{
	FStatus status = FStatus_InvalidArgument;
	PlatformStatus platformStatus = 0; // Set to zero first, in order to accumulate error
	uint16_t rawADCValue;
	
	// At least one trigger must be specified
	require( outLeftTrigger || outRightTrigger, exit );
	
	// Read the ADCs and map them into percentages
	if ( outLeftTrigger )
	{
		platformStatus |= PlatformADC_Read( FQuadTXADC_PadLeftTrigger, &rawADCValue );
		*outLeftTrigger = _FQuadTXPad_ConvertRawToUnsignedPercentage( rawADCValue, PAD_LEFT_TRIGGER_MIN, PAD_LEFT_TRIGGER_MAX );	
	}
	
	if ( outRightTrigger )
	{
		// Read the ADCs and map them into percentages
		platformStatus |= PlatformADC_Read( FQuadTXADC_PadRightTrigger, &rawADCValue );
		*outRightTrigger = _FQuadTXPad_ConvertRawToUnsignedPercentage( rawADCValue, PAD_RIGHT_TRIGGER_MIN, PAD_RIGHT_TRIGGER_MAX );		
	}

	// Check accumulated error
	status = ( platformStatus == PlatformStatus_Success ) ? FStatus_Success : FStatus_Failed;
exit:
	return status;
}

static int8_t _FQuadTXPad_ConvertRawToSignedPercentage( const uint16_t inRawValue, const uint16_t inMidValue, const uint16_t inMinValue, const uint16_t inMaxValue )
{
	int8_t signedPercentage;
	int32_t difference;
	
	difference = ( int32_t )inRawValue - inMidValue;
	
	if ( difference >= 0 )
	{
		signedPercentage = ( int8_t )(( difference * PAD_MAX_ANALOG_VALUE ) / (( int32_t )inMaxValue - inMidValue ));
	}
	else 
	{
		signedPercentage = ( int8_t )(( difference * PAD_MAX_ANALOG_VALUE ) / ( int32_t )( inMidValue - inMinValue ));
	}
	
	return signedPercentage;
}

static uint8_t _FQuadTXPad_ConvertRawToUnsignedPercentage( uint16_t inRawValue, const uint16_t inMinValue, const uint16_t inMaxValue )
{
	uint8_t percentage;
	
	if ( inRawValue > inMaxValue )
	{
		inRawValue = inMaxValue;
	}
	else if ( inRawValue < inMinValue )
	{
		inRawValue = inMinValue;
	}
	percentage = ( uint8_t )((( uint32_t )( inRawValue - inMinValue ) * PAD_MAX_ANALOG_VALUE ) / ( inMaxValue - inMinValue ));
	return percentage;
}