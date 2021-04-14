#ifndef __ANGLE_POLL__
#define __ANGLE_POLL__

#include "FreeRTOS.h"

typedef struct
{
	uint16_t angle;
	TickType_t  timestamp;
} angle_record_t;

void vAnglePollTask( void* pvParameters );

#endif
