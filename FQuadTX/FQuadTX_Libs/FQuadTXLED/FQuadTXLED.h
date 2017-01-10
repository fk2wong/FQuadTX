/*
 * FQuadTXLED.h
 *
 * Created: 2017-01-09 9:31:27 PM
 *  Author: Felix
 */ 


#ifndef FQUADTXLED_H_
#define FQUADTXLED_H_

#include "FStatus.h"

/*!
 * \brief   Initializes LED functionality.
 *
 * \return  FStatus_Success if successful, FStatus_Failed otherwise.
 */
FStatus FQuadTXLED_Init();

/*!
 * \brief   Turns on the LED.
 *
 * \return  FStatus_Success if successful, FStatus_Failed otherwise.
 */
FStatus FQuadTXLED_On();

/*!
 * \brief   Turns off the LED.
 *
 * \return  FStatus_Success if successful, FStatus_Failed otherwise.
 */
FStatus FQuadTXLED_Off();

/*!
 * \brief   Toggles the LED.
 *
 * \return  FStatus_Success if successful, FStatus_Failed otherwise.
 */
FStatus FQuadTXLED_Toggle();


#endif /* FQUADTXLED_H_ */