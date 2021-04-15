#ifndef __ENCODER_COM__
#define __ENCODER_COM__

#include "main.h"

typedef enum
{
	// Volatile
	EncoderAddr_NOP = 0x0000,
	EncoderAddr_ERRFL = 0x0001,
	EncoderAddr_PROG = 0x0003,
	// Non-Volatile
	EncoderAddr_ZPOSM = 0x0016,
	EncoderAddr_ZPOSL = 0x0017,
	EncoderAddr_SETTINGS1 = 0x0018,
	EncoderAddr_SETTINGS2 = 0x0019,
	// Volatile
	EncoderAddr_DIAAGC = 0x3FFC,
	EncoderAddr_MAG = 0x3FFD,
	EncoderAddr_ANGLEUNC = 0x3FFE,
	EncoderAddr_ANGLECOM = 0x3FFF
} EncoderAddr_t;

uint16_t xEncRead( SPI_HandleTypeDef* pxHSPI , uint16_t addr );

HAL_StatusTypeDef xSendMsg( SPI_HandleTypeDef* pxHSPI, uint16_t msg );
HAL_StatusTypeDef xSendReceiveMsg( SPI_HandleTypeDef* pxHSPI, uint16_t txMsg, uint16_t* rxMsg );
HAL_StatusTypeDef xReceiveMsg( SPI_HandleTypeDef* pxHSPI, uint16_t* rx_msg );

#endif
