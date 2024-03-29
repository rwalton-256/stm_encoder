/*
 * app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: waltor
 */

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"
#include "angle_poll.h"
#include "can_wrapper.h"

void vMainTask(void *pvArgs)
{
	main_task_args* args = (main_task_args*)pvArgs;

	TaskHandle_t pxAnglePollTaskHndl, pxCanTaskHndl;

	xTaskCreate( vAnglePollTask,
			     "vAnglePollTask",
				 64,
				 args->pxSPI_Hdl,
				 3,
				 &pxAnglePollTaskHndl );

	xTaskCreate( vCanTask,
			     "vCanTask",
				 64,
				 args->pxCAN_Hdl,
				 3,
				 &pxCanTaskHndl );

	while(1)
	{
//		HAL_GPIO_WritePin(LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		vTaskDelay(1000);
//		HAL_GPIO_WritePin(LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
		vTaskDelay(1000);
	}
}

