/*
 *
 * Created by: Ryan Walton
 * 4/13/2021
 */


#include "FreeRTOS.h"
#include "task.h"

#include "main.h"

#include "angle_poll.h"
#include "encoder_com.h"

#include "can_wrapper.h"
#include "string.h"

// 50 ticks between angle sample
#define ANGLE_POLL_DELAY 50

void vAnglePollTask( void* pvParameters )
{
	SPI_HandleTypeDef* pxHSPI= (SPI_HandleTypeDef*)pvParameters;

	angle_record_t angle_sample;

	CAN_Packet_t packet =
	{
			.header =
			{
					.StdId = 0xc,
					.DLC = sizeof( angle_sample.timestamp ) + sizeof( angle_sample.angle ),
					.IDE = CAN_ID_STD,
					.RTR = CAN_RTR_DATA
			}
	};

	while( 1 )
	{
		angle_sample.angle  = xEncRead( pxHSPI, EncoderAddr_ANGLEUNC );
		angle_sample.timestamp = xTaskGetTickCount();

		memcpy( &packet.data[0], &angle_sample.timestamp, sizeof( angle_sample.timestamp ) );
		memcpy( &packet.data[sizeof( angle_sample.timestamp )], &angle_sample.angle, sizeof( angle_sample.angle ) );

		vSendCANMsg( &packet );

		vTaskDelay( 1 );
	}
}
