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

FStatus FQuadTXControls_Init( void );

FStatus FQuadTXControls_ReadUserInput( void );

FStatus FQuadTXControls_GetUpdatedControls( FQuadAxisValue *const outPitch, 
											FQuadAxisValue *const outRoll, 
											FQuadAxisValue *const outYaw, 
											FQuadThrustValue *const outThrust );

