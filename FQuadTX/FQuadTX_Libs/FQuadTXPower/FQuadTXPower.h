/*
 * FQuadTXPower.h
 *
 * Created: 2017-01-09 8:09:24 PM
 *  Author: Felix
 */ 


#ifndef FQUADTXPOWER_H_
#define FQUADTXPOWER_H_

#include "FStatus.h"

/*!
 *\brief  Initializes the module that controls the power.
 */
FStatus FQuadTXPower_Init();

/*!
 *\brief   Holds power on for the system. Should be called immediately on power up, since the power is currently only being held by manual button press.
 */
FStatus FQuadTXPower_Hold();

/*!
 *\brief   Releases the line that holds power to the system. This function will not return.
 *
 *\details The system will not shut down until the user releases the manual power button. Regardless, this function will not return.
 */
FStatus FQuadTXPower_Release();

/*!
 *\brief     Determines if the user has held the button for at least FQUADTX_POWER_RELEASE_REQUEST_WAIT_MS, 
  *          and releases the power line if so. Should be called frequently since this is polled, not interrupt-driven.
  *
  *\details  If a power off is requested, then the power will be released and this function will not return. 
  *          The system will not shut down until the user releases the manual power button.
 */
FStatus FQuadTXPower_CheckPowerOffRequest();


#endif /* FQUADTXPOWER_H_ */