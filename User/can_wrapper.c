/*
 *
 * Created by: Ryan Walton
 * 4/13/2021
 */


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "main.h"

#include "angle_poll.h"
#include "encoder_com.h"
#include "can_wrapper.h"

QueueHandle_t RX_Queue_Hndl = NULL, TX_Queue_Hndl = NULL;

void setup_can_wrapper( CAN_HandleTypeDef* pxHCAN );
void vCANRxCallback( CAN_HandleTypeDef* pxHCAN );

void vCanTask( void* pvParam )
{
	CAN_HandleTypeDef* pxHCAN = (CAN_HandleTypeDef*)pvParam;

	setup_can_wrapper( pxHCAN );

	while( 1 )
	{
		CAN_Packet_t packet;
		BaseType_t ret;

		ret = xQueueReceive( TX_Queue_Hndl, &packet, portMAX_DELAY );

		uint32_t mailbox;

		HAL_CAN_AddTxMessage( pxHCAN, &packet.header, packet.data, &mailbox );
	}

	vTaskDelete( NULL );
}

void setup_can_wrapper( CAN_HandleTypeDef* pxHCAN )
{
	// Setup and receive and tranmsit FreeRTOS Queues
	RX_Queue_Hndl = xQueueCreate( 20, sizeof(CAN_Packet_t) );
	TX_Queue_Hndl = xQueueCreate( 20, sizeof(CAN_Packet_t) );

	// Setup RX FIFO 0 MSG Pending interrupt and handler
	HAL_CAN_RegisterCallback( pxHCAN, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, vCANRxCallback );
	HAL_CAN_ActivateNotification( pxHCAN, CAN_IT_RX_FIFO0_MSG_PENDING );

	// Setup RX filtering
	volatile CAN_Filter32_t filter_id =
	{
			.bit.res = 0,
			.bit.RTR = CAN_RTR_DATA,
			.bit.IDE = CAN_ID_STD,
			.bit.EXID = 0x0,
			.bit.STID = 0xaa
	};

	CAN_Filter32_t filter_mask =
	{
			.bit.res = 0,
			.bit.RTR = 0,
			.bit.IDE = 0,
			.bit.EXID = 0x0,
			.bit.STID = 0x0
	};

	CAN_FilterTypeDef filter =
			{
					.FilterIdHigh = ( filter_id.raw >> 16 ),
					.FilterIdLow = ( filter_id.raw & 0xffff ),
					.FilterMaskIdHigh = ( filter_mask.raw >> 16 ),
					.FilterMaskIdLow = ( filter_mask.raw & 0xffff ),
					.FilterActivation = CAN_FILTER_ENABLE,
					.FilterFIFOAssignment = CAN_FILTER_FIFO0,
					.FilterBank = 0,
					.FilterMode = CAN_FILTERMODE_IDMASK,
					.FilterScale = CAN_FILTERSCALE_32BIT,
			};

	HAL_CAN_ConfigFilter( pxHCAN, &filter );

	// Start the CAN module
	HAL_CAN_Start( pxHCAN );
}

void vCANRxCallback( CAN_HandleTypeDef* pxHCAN )
{
	CAN_Packet_t packet;

	HAL_CAN_GetRxMessage( pxHCAN, 0, &packet.header, packet.data );

	BaseType_t ret, xTaskWoken = pdFALSE;
	ret = xQueueSendFromISR( RX_Queue_Hndl, &packet, &xTaskWoken );

	if( ret == pdFALSE )
	{

	}

	if( xTaskWoken == pdFALSE )
	{
		taskYIELD();
	}
}

void vSendCANMsg( CAN_Packet_t* pxPacket )
{
	BaseType_t ret;

	if( TX_Queue_Hndl == NULL )
		return;

	ret = xQueueSend( TX_Queue_Hndl, pxPacket, portMAX_DELAY );

	if( ret == pdFAIL )
	{

	}
}
