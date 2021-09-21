#ifndef __CAN_WRAPPER__
#define __CAN_WRAPPER__

#include "main.h"

typedef struct
{
	uint8_t data[8];
	CAN_RxHeaderTypeDef header;
} CAN_Packet_t;

typedef union
{
	uint32_t raw;
	struct
	{
		uint32_t res:1;
		uint32_t RTR:1;
		uint32_t IDE:1;
		uint32_t EXID:18;
		uint32_t STID:11;
	} bit;
} CAN_Filter32_t;

void vCanTask( void* pvParam );
void vSendCANMsg( CAN_Packet_t* pxPacket );

#endif
