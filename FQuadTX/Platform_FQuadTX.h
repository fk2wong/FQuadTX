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
	FQuadTXGPIO_PadDUp         = PlatformGPIO_PTD2,
	FQuadTXGPIO_PadDDown       = PlatformGPIO_PTD3,
	FQuadTXGPIO_PadDLeft       = PlatformGPIO_PTD4,
	FQuadTXGPIO_PadDRight      = PlatformGPIO_PTD5,
	FQuadTXGPIO_PadA           = PlatformGPIO_PTD6,
	FQuadTXGPIO_PadB           = PlatformGPIO_PTD7,
	FQuadTXGPIO_PadX           = PlatformGPIO_PTC0,
	FQuadTXGPIO_PadY           = PlatformGPIO_PTC1,
	FQuadTXGPIO_PadStart       = PlatformGPIO_PTB0,
	FQuadTXGPIO_PadSelect      = PlatformGPIO_PTB1,
	FQuadTXGPIO_PadCenter      = PlatformGPIO_PTB2,
	FQuadTXGPIO_PadLeftBumper  = PlatformGPIO_PTB3,
	FQuadTXGPIO_PadRightBumper = PlatformGPIO_PTB4,
	FQuadTXGPIO_TestLED        = PlatformGPIO_PTB5,
} FQuadTXGPIO_t;

typedef enum 
{
	FQuadTXADC_PadLeftVertical    = PlatformADC_ADC2,
	FQuadTXADC_PadLeftHorizontal  = PlatformADC_ADC3,
	FQuadTXADC_PadRightVertical   = PlatformADC_ADC4,
	FQuadTXADC_PadRightHorizontal = PlatformADC_ADC5,
} FQuadTXADC_t;

typedef enum
{
	FQuadTXPWM_MotorNW = PlatformPWM_0A,
	FQuadTXPWM_MotorNE = PlatformPWM_0B,
	FQuadTXPWM_MotorSE = PlatformPWM_2A,
	FQuadTXPWM_MotorSW = PlatformPWM_2B,
} FQuadTXPWM_t;

#endif /* PLATFORM_FQUADTX_H_ */