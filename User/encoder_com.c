/*
 *
 * Created by: Ryan Walton
 * 4/10/2021
 */

#include "encoder_com.h"

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
  ) ? 0 : 1                \
)

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

	// Enable SPI chip select for the encoder UC
	HAL_GPIO_WritePin(ENC_SPI_SEL_GPIO_Port, ENC_SPI_SEL_Pin, GPIO_PIN_RESET);

	HAL_StatusTypeDef status;

	status = HAL_SPI_Transmit( pxHSPI, (uint8_t*)&msg, 1, 100 );

	// Reset SPI chip select
	HAL_GPIO_WritePin(ENC_SPI_SEL_GPIO_Port, ENC_SPI_SEL_Pin, GPIO_PIN_SET);

	return status;
}

HAL_StatusTypeDef xSendReceiveMsg( SPI_HandleTypeDef* pxHSPI, uint16_t txMsg, uint16_t* rxMsg )
{
	HAL_StatusTypeDef status;

	// Clear top bit
	txMsg &= 0x7fff;
	if( !CHECKSUM( txMsg ) )
	{
		// Checksum failed, we need to set the checksum bit so the
		// checksum will pass on the encoder uc
		txMsg |= 0x8000;
	}

	// Enable SPI chip select for the encoder UC
	HAL_GPIO_WritePin(ENC_SPI_SEL_GPIO_Port, ENC_SPI_SEL_Pin, GPIO_PIN_RESET);

	status = HAL_SPI_TransmitReceive( pxHSPI,
			                          (uint8_t*)&txMsg,
									  (uint8_t*)rxMsg,
									  1,
									  100 );

	// Reset SPI chip select
	HAL_GPIO_WritePin(ENC_SPI_SEL_GPIO_Port, ENC_SPI_SEL_Pin, GPIO_PIN_SET);
	return status;
}

HAL_StatusTypeDef xReceiveMsg( SPI_HandleTypeDef* pxHSPI, uint16_t* rx_msg )
{
	return xSendReceiveMsg( pxHSPI, EncoderAddr_NOP | 0x4000, rx_msg );
}

uint16_t xEncRead( SPI_HandleTypeDef* pxHSPI , uint16_t addr )
{
	uint16_t resp;
	HAL_StatusTypeDef status;

	status = xSendMsg( pxHSPI, addr | 0x4000 );

	if( status != HAL_OK )
	{
		// Error
	}

	status = xReceiveMsg( pxHSPI, &resp );

	if( status != HAL_OK )
	{
		// Error
	}

	if( resp & 0x4000 )
	{
		// Error bit of receive transmission high
	}
	else if( !CHECKSUM( resp ) )
	{
		// Checksum doesn't match
	}

	return resp & 0x3fff;
}
