/*
 * FQuadTXLogging.h
 *
 * Created: 2016-09-12 2:33:25 PM
 *  Author: Felix
 */ 


#ifndef FQUADTXLOGGING_H_
#define FQUADTXLOGGING_H_

#include "require_macros.h"

#define ENABLE_DEBUG_LOGGING 1  // Since this shares UART with XBee, this should be set to 0 when the XBee is used.

#define FQUADTX_LOGGING_MAX_BUFFER_SIZE ( 64 )

#define REMOVE_BRACES( ... ) __VA_ARGS__

#if ENABLE_DEBUG_LOGGING

#include "PlatformUART.h"
#include <string.h>
#include <stdio.h>

#define FQUADTX_DEBUG_LOG( X )                                             \
	do                                                                   \
	{                                                                    \
		char str[ FQUADTX_LOGGING_MAX_BUFFER_SIZE ];                       \
		size_t strLen;                                                   \
	                                                                     \
		snprintf( str, FQUADTX_LOGGING_MAX_BUFFER_SIZE, REMOVE_BRACES X ); \
		strLen = strlen( str );                                          \
		PlatformUART_Transmit(( uint8_t* )str, strLen );                 \
	} while ( 0 )
#else // ENABLE_DEBUG_LOGGING
#define FQUADTX_DEBUG_LOG( X )
#endif // ENABLE_DEBUG_LOGGING

#endif /* FQUADTXLOGGING_H_ */