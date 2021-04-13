/*
 * app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: waltor
 */

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"
#include "encoder_communication.h"

void vMainTask(void *pvArgs)
{
	main_task_args* args = (main_task_args*)pvArgs;

	TaskHandle_t pxAnglePollTaskHndl;

	xTaskCreate( vAnglePollTask,
			     "vAnglePollTask",
				 64,
				 NULL,//args->pxSPI_Hdl,
				 3,
				 &pxAnglePollTaskHndl );


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

