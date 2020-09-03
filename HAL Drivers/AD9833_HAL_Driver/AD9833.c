/*
	AD9833 DDS function generator driver for STM32.
	This IC can create frequencies from 0hz to 12.5MHz with 28 bits of resolution.
	It is programmed using a 3 wire SPI interface. 

	The AD9833 requires two 16 bit writes in order to set the frequency.
	It takes the actual frequency data value as a 28 bit word, the other 4 bits are used to address the 
	frequency value register.
	The users chosen frequency is plugged into a formula from the datasheet and manipulated to include 
	the address and the high / low data.

	Still work in progress.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "AD9833.h"

// Creates the correct frequency word and transmits it to the AD9833.
// Can create sine, triangle and square waves.
void AD9833_Set_Output(int freq, int wave){
	uint32_t outputFreq;
	uint16_t outputFreqLOW;
	uint16_t outputFreqHIGH;
	uint16_t data[10];
	HAL_StatusTypeDef err;

	// Taken from application note AN-1070 page 3.
	switch(wave){
		case 0:
			data[0] = 0x2100;
			data[4] = 0x2000;
			break;
		case 1:
			data[0] = 0x2102;
			data[4] = 0x2002;
			break;
		case 2:
			data[0] = 0x2128;
			data[4] = 0x2028;
			break;
		default:
			return;
	}

  	data[3] = 0xC000;

	// This formula is given in the datasheet to create a 32 bit number. This needs to be manipulated to make the 28 bit word.
	outputFreq = (freq * pow(2, 28)) / AD9833_REF_FREQ;

	// Bitmask the 32 bit value to get the lower 14 data bits.
	outputFreqLOW = outputFreq & 0x3FFF;
	// OR this result to add on the register address to the MSB.
	outputFreqLOW = outputFreqLOW | 0x4000;

	// Bitwise shift the high bits over and do the same process.
	outputFreqHIGH = (outputFreq >> 14) & 0x3FFF;
	outputFreqHIGH = outputFreqHIGH | 0x4000;

	data[1] = outputFreqLOW;
	data[2] = outputFreqHIGH;

	// Chip select must go low to begin transmission.
	// I configured the GPIO pin to be default high.
	HAL_GPIO_WritePin(AD9833_CS_GPIO_Port, AD9833_CS_Pin, GPIO_PIN_RESET);

	// Transmit 5 16 bit words from the data buffer.
	// ARGS: SPI device handle, uint8 pointer to data buffer, number of words to transmit, timeout.
  	err = HAL_SPI_Transmit(&hspi2, (uint8_t *)(data), 5, 100);
  	if(err != HAL_OK){
  		printString("SPI tx error\r\n");
  	}

  	// Chip select off.
  	HAL_GPIO_WritePin(AD9833_CS_GPIO_Port, AD9833_CS_Pin, GPIO_PIN_SET);
}

void AD9833_Set_Waveform(int wave){
	uint16_t data[0];
	HAL_StatusTypeDef err;

	switch(wave){
		case 0:
			data[0] = 0x2000;
			break;
		case 1:
			data[0] = 0x2002;
			break;
		case 2:
			data[0] = 0x2028;
			break;
		default:
			return;
	}

	HAL_GPIO_WritePin(AD9833_CS_GPIO_Port, AD9833_CS_Pin, GPIO_PIN_RESET);
	err = HAL_SPI_Transmit(&hspi2, (uint8_t *)(data), 1, 100);
	if(err != HAL_OK){
		printString("SPI tx error\r\n");
	}
  	HAL_GPIO_WritePin(AD9833_CS_GPIO_Port, AD9833_CS_Pin, GPIO_PIN_SET);
}

// Example usage.
int main(void){
	AD9833_Set_Output(1000, SIN);
}