/*
 * app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: waltor
 */

#include "FreeRTOS.h"
#include "task.h"

#include "angle_poll.h"
#include "can_wrapper.h"

#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_spi.h"

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
		vTaskDelay(1000);
		vTaskDelay(1000);
	}
}

