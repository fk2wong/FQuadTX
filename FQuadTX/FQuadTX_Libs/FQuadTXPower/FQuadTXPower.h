/*
 * FQuadTXPower.h
 *
 * Created: 2017-01-09 8:09:24 PM
 *  Author: Felix
 */ 


#ifndef FQUADTXPOWER_H_
#define FQUADTXPOWER_H_

#include "FStatus.h"

FStatus FQuadTXPower_Init();

FStatus FQuadTXPower_Hold();

FStatus FQuadTXPower_Release();

FStatus FQuadTXPower_CheckPowerOffRequest();


#endif /* FQUADTXPOWER_H_ */