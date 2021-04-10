/*
 * app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: waltor
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"

void vMainTask(void *argument)
{
	while(1)
	{
		HAL_GPIO_WritePin(LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		vTaskDelay(1000);
		HAL_GPIO_WritePin(LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
		vTaskDelay(1000);
		printf("Hello World!\n");
	}
}

