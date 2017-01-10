/*
 * Platform_FQuadTX.h
 *
 * Created: 2016-09-06 4:29:33 PM
 *  Author: Felix
 */ 


#ifndef PLATFORM_FQUADTX_H_
#define PLATFORM_FQUADTX_H_

#include "PlatformInterrupt.h"
#include "PlatformPowerSave.h"
#include "PlatformClock.h"
#include "PlatformGPIO.h"
#include "PlatformRingBuffer.h"
#include "PlatformUART.h"
#include "PlatformADC.h"
#include "PlatformPWM.h"
#include "PlatformI2C.h"


typedef enum 
{	
	FQuadTXGPIO_PadX		= PlatformGPIO_PTB0,
	FQuadTXGPIO_PadA		= PlatformGPIO_PTB1,
	FQuadTXGPIO_GreenLED	= PlatformGPIO_PTB2,
	FQuadTXGPIO_PowerHold	= PlatformGPIO_PTB3,
	FQuadTXGPIO_PadB		= PlatformGPIO_PTB4,
	
	FQuadTXADC_PadDDown		= PlatformGPIO_PTC4,
	FQuadTXADC_PadDLeft		= PlatformGPIO_PTC5,
	
	FQuadTXGPIO_PadDRight   = PlatformGPIO_PTD2,
	FQuadTXGPIO_PadDUp		= PlatformGPIO_PTD3,
	FQuadTXGPIO_XBeeSleep   = PlatformGPIO_PTD4,
	FQuadTXGPIO_PadCenter	= PlatformGPIO_PTD5,
	FQuadTXGPIO_PadStart    = PlatformGPIO_PTD6,
	FQuadTXGPIO_PadY		= PlatformGPIO_PTD7,
	
} FQuadTXGPIO_t;

typedef enum 
{
	FQuadTXADC_PadLeftVert  = PlatformADC_ADC0,
	FQuadTXADC_PadLeftHorz  = PlatformADC_ADC1,
	FQuadTXADC_PadRightHorz = PlatformADC_ADC2,
	FQuadTXADC_PadRightVert	= PlatformADC_ADC3,
} FQuadTXADC_t;

#endif /* PLATFORM_FQUADTX_H_ */