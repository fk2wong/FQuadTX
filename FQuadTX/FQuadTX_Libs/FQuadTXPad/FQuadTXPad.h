/*
 * FQuadTXPad.h
 *
 * Created: 2017-01-09 10:30:17 PM
 *  Author: Felix
 */ 


#ifndef FQUADTXPAD_H_
#define FQUADTXPAD_H_

#include "FStatus.h"
#include <stdint.h>
#include <stdbool.h>

#define PAD_MAX_ANALOG_VALUE ( 100 )

#define PAD_NUM_BUTTONS ( 10 )

F_ENUM( uint16_t, FQuadTXPadButton )
{
	FQuadTXPadButton_A		= ( 1 << 0 ),
	FQuadTXPadButton_B		= ( 1 << 1 ),
	FQuadTXPadButton_X		= ( 1 << 2 ),
	FQuadTXPadButton_Y		= ( 1 << 3 ),
	FQuadTXPadButton_Center	= ( 1 << 4 ),
	FQuadTXPadButton_Start	= ( 1 << 5 ),
	FQuadTXPadButton_DUp	= ( 1 << 6 ),
	FQuadTXPadButton_DRight	= ( 1 << 7 ),
	FQuadTXPadButton_DDown	= ( 1 << 8 ),
	FQuadTXPadButton_DLeft	= ( 1 << 9 ),
};

/*!
 *\brief     Inits the pad and all GPIOs used.
 *
 *\return    FStatusSuccess if successful, FStatusFailed if anything failed.
 */
FStatus FQuadTXPad_Init();

/*!
 *\brief     Read the state of a button.
 *
 *\param     inButton           - Button whose state will be read.
 *\param     outIsButtonPressed - True if the button is pressed, false if the button is not pressed.
 *
 *\return    FStatusSuccess if successful, FStatusFailed if anything failed.
 */
FStatus FQuadTXPad_ReadButtonState( const FQuadTXPadButton inButton, bool* const outIsButtonPressed );

/*!
 *\brief     Reads the state of all buttons.
 *
 *\param     outButtonsPressed - Bit field of buttons pressed. '1' indicates that the button is pressed; see enum FQuadTXPadButton for bit assignments.
 *
 *\return    FStatusSuccess if successful, FStatusFailed if anything failed.
 */
FStatus FQuadTXPad_ReadAllButtonStates( uint16_t* const outButtonsPressed );

/*!
 *\brief     Read the horizontal and vertical values of the joysticks.
 *
 *\details   All values are in a range from -100 (-PAD_MAX_ANALOG_VALUE) to 100 (PAD_MAX_ANALOG_VALUE), where 0 is the neutral position. 
 *           For vertical values, a positive value indicates an upward tilt. For horizontal values, a positive value indicates a tilt to the right.
 *
 *\param     outLeftVert  - Value of the left joystick, vertical direction.
 *\param     outLeftHorz  - Value of the left joystick, horizontal direction.
 *\param     outRightVert - Value of the right joystick, vertical direction.
 *\param     outRightHorz - Value of the right joystick, horizontal direction.
 *
 *\return    FStatusSuccess if successful, FStatusFailed if anything failed.
 */
FStatus FQuadTXPad_GetJoystickValues( int8_t* const outLeftVert, int8_t* const outLeftHorz, int8_t* const outRightVert, int8_t* const outRightHorz );

/*!
 *\brief     Read the values of the triggers.
 *
 *\details   All values are in a range from 0 to 100 (PAD_MAX_ANALOG_VALUE), where 0 is the neutral (unpressed) position. 
 *
 *\param     outLeftTrigger  - Value of the left trigger.
 *\param     outRightTrigger - Value of the right trigger.
 *
 *\return    FStatusSuccess if successful, FStatusFailed if anything failed.
 */
FStatus FQuadTXPad_GetTriggerValues( uint8_t* const outLeftTrigger, uint8_t* const outRightTrigger );


#endif /* FQUADTXPAD_H_ */