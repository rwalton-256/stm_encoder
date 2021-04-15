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

void vCanTask( void* pvParam )
{
	CAN_HandleTypeDef* pxHCAN = (CAN_HandleTypeDef*)pvParam;

	HAL_CAN_Start( pxHCAN );

	while( 1 )
	{
		CAN_TxHeaderTypeDef header =
		{
				.StdId = 0xaa,
				.DLC = 8,
				.IDE = CAN_ID_STD,
				.RTR = CAN_RTR_DATA
		};

		uint8_t data[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

		uint32_t mailbox;

		HAL_CAN_AddTxMessage( pxHCAN, &header, data, &mailbox );

		vTaskDelay( 1000 );
	}
}
