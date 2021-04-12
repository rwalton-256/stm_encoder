/*
 *
 * Created by: Ryan Walton
 * 4/10/2021
 */

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"

// Returns true if there is an even number
// of '1's in bits 15:0
#define CHECKSUM( data )   \
(                          \
  (                        \
    (                      \
      ( data & 0x0001 ) +  \
      ( data & 0x0002 ) +  \
      ( data & 0x0004 ) +  \
      ( data & 0x0008 ) +  \
      ( data & 0x0010 ) +  \
      ( data & 0x0020 ) +  \
      ( data & 0x0040 ) +  \
      ( data & 0x0080 ) +  \
      ( data & 0x0100 ) +  \
      ( data & 0x0200 ) +  \
      ( data & 0x0400 ) +  \
      ( data & 0x0800 ) +  \
      ( data & 0x1000 ) +  \
      ( data & 0x2000 ) +  \
      ( data & 0x4000 ) +  \
      ( data & 0x8000 )    \
    ) % 2                  \
  ) ? 1 : 0                \
)

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

HAL_StatusTypeDef xSendMsg( SPI_HandleTypeDef* pxHSPI, uint16_t msg )
{
	// Clear top bit
	msg &= 0x7fff;
	if( !CHECKSUM( msg ) )
	{
		// Checksum failed, we need to set the checksum bit so the
		// checksum will pass on the encoder uc
		msg |= 0x8000;
	}

	return HAL_SPI_Transmit( pxHSPI, (uint8_t*)&msg, 1, 100 );
}

HAL_StatusTypeDef xSendReceiveMsg( SPI_HandleTypeDef* pxHSPI, uint16_t txMsg, uint16_t* rxMsg )
{
	HAL_StatusTypeDef status;

	volatile uint16_t foo = txMsg;

	// Clear top bit
	foo &= 0x7fff;
	if( !CHECKSUM( foo ) )
	{
		// Checksum failed, we need to set the checksum bit so the
		// checksum will pass on the encoder uc
		foo = foo | 0x8000;
	}

	status = HAL_SPI_TransmitReceive( pxHSPI,
			                          (uint8_t*)&foo,
									  (uint8_t*)rxMsg,
									  1,
									  100 );

	if( *rxMsg & 0x4000 )
	{
		// Error bit of receive transmission high
		status = HAL_ERROR;
	}
	else if( !CHECKSUM( *rxMsg ) )
	{
		// Checksum doesn't match
		status = HAL_ERROR;
	}
	return status;
}

HAL_StatusTypeDef xReceiveMsg( SPI_HandleTypeDef* pxHSPI, uint16_t* rx_msg )
{
	HAL_StatusTypeDef status;
	uint16_t tx_msg = EncoderAddr_NOP | 0x4000;
	status = HAL_SPI_TransmitReceive( pxHSPI,
			                          (uint8_t*)&tx_msg,
			                          (uint8_t*)rx_msg,
									  1,
									  100 );

	if( *rx_msg & 0x4000 )
	{
		// Error bit of receive transmission high
		status = HAL_ERROR;
	}
	else if( !CHECKSUM( *rx_msg ) )
	{
		// Checksum doesn't match
		status = HAL_ERROR;
	}
	return status;
}

uint16_t xEncRead( SPI_HandleTypeDef* pxHSPI , uint16_t addr )
{
	uint16_t ret;

	// Enable SPI chip select for the encoder UC
	HAL_GPIO_WritePin(ENC_SPI_SEL_GPIO_Port, ENC_SPI_SEL_Pin, GPIO_PIN_RESET);

	xSendReceiveMsg( pxHSPI, addr | 0x4000, &ret );
	vTaskDelay( 500 );
	xSendReceiveMsg( pxHSPI, EncoderAddr_NOP | 0x4000, &ret );

	// Reset SPI chip select
	HAL_GPIO_WritePin(ENC_SPI_SEL_GPIO_Port, ENC_SPI_SEL_Pin, GPIO_PIN_SET);

	return ret;
}

void vAnglePollTask( void* pvParameters )
{
	SPI_HandleTypeDef* pxHSPI= (SPI_HandleTypeDef*)pvParameters;
	while( 1 )
	{
		xEncRead( pxHSPI, EncoderAddr_DIAAGC );
		vTaskDelay( 500 );
	}
}
