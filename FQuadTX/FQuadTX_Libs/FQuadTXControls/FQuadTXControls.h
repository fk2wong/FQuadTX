/*
 * FQuadTXControls.h
 *
 * Created: 2017-03-13 4:16:37 PM
 *  Author: Felix
 */ 

#pragma once

#include "Platform_FQuadTX.h"
#include "FStatus.h"
#include "FQuadDefs.h"

/*!
 *\brief   Initializes the module that reads the controller and maps it to desired flight controls.
 *
 *\details Here is a summary of how the controls are interpreted: 
 *         - D-pad buttons change the base pitch/roll values, while the Y/A buttons change the base thrust value.
 *         - Left analog stick adjusts pitch/roll values on top of the base pitch/roll values. Triggers adjust the thrust on top of the base thrust.
 *         - Right trigger adds thrust, left trigger subtracts.
 *         - Right analog (horizontal) changes yaw rate change.
 *         - X button is a shortcut used to change the baseline of the thrust from the triggers. Ex: on startup, press RT until the quad lifts off, then press X.
 *           The base thrust will now be this value after the triggers are released, and can be adjusted using Y/A or the triggers once more.
 *         - Start button resets base pitch/roll values to 0.
 *         - Center button resets base pitch/roll/thrust values to 0. Good for emergency turn off.
 *         - Unused: B button, Right Vertical stick
 *         - Disconnected: Both bumpers, Select
 *
 */
FStatus FQuadTXControls_Init( void );

/*!
 *\brief   Updates the status of button presses. Should be called frequently in order to capture short button presses, as they are polled (not interrupt-driven).
 */
FStatus FQuadTXControls_ReadUserInput( void );

/*!
 *\brief   Gets the most recent controls read from the controller. 
 */
FStatus FQuadTXControls_GetUpdatedControls( FQuadAxisValue *const outPitch, 
											FQuadAxisValue *const outRoll, 
											FQuadAxisValue *const outYaw, 
											FQuadThrustValue *const outThrust );

