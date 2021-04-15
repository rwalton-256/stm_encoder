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

// 50 ticks between angle sample
#define ANGLE_POLL_DELAY 50

void vAnglePollTask( void* pvParameters )
{
	SPI_HandleTypeDef* pxHSPI= (SPI_HandleTypeDef*)pvParameters;
	while( 1 )
	{
		angle_record_t angle_sample =
		{
				.angle  = xEncRead( pxHSPI, EncoderAddr_ANGLEUNC ),
				.timestamp = xTaskGetTickCount()
		};

		HAL_GPIO_WritePin(LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_SET);
		vTaskDelay( (uint32_t)angle_sample.angle * 20 / 0x3fff );
		HAL_GPIO_WritePin(LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_RESET);
		vTaskDelay( ( 0x3fff - (uint32_t)angle_sample.angle ) * 20 / 0x3fff );

//		vTaskDelay( ANGLE_POLL_DELAY );
	}
}
