#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "wm8731.h"

wm8731_handle wm8731;

// Enable line input to output bypass.
void wm8731_bypass_enable(wm8731_handle w)
{
	wm8731_ctl_set(w, 3, WM8731_AAP_CTL, WM8731_DAC_SELECT, WM8731_BYPASS_ENABLE, WM8731_MIC_MUTE);
	wm8731_ctl_transmit(w->i2cHandle, WM8731_AAP_CTL, w->analogPath);

	wm8731_ctl_transmit(w->i2cHandle, WM8731_POWER_CTL, ALL_PERIPH_OFF);

	wm8731_ctl_set(w, 3, WM8731_POWER_CTL, WM8731_OUTPUTS_ENABLE, WM8731_LINEINPUT_ENABLE, WM8731_POWER_ON);
	wm8731_ctl_transmit(w->i2cHandle, WM8731_POWER_CTL, w->power);
}

wm8731_handle *wm8731_init(I2C_TypeDef I2Cx)
{
	wm8731_handle *temp;

	temp->i2cHandle = I2Cx;

	wm8731_ctl_transmit(I2Cx, WM8731_RESET_CTL, WM8731_RESET_DEVICE);
	wm8731_ctl_transmit(I2Cx, WM8731_POWER_CTL, WM8731_ALL_PERIPH_OFF);

	temp->lineIn = 0;
	temp->lineOut = 0;
	temp->analogPath = 0;
	temp->digitalPath = 0;
	temp->power = WM8731_ALL_PERIPH_OFF;
	temp->digitalFmt = 0;
	temp->sampleRate = 0;


	//wm8731_ctl_transmit(I2Cx, WM8731_AAP_CTL, WM8731_I2S_MODE);
	//wm8731_ctl_transmit(I2Cx, WM8731_DAIFMT, WM8731_MASTER_16BIT_MSBL);

	return temp;
}

/* 
* Edits the codec handle with updated register values.
* The control transmit function uses the values inside the handle.
* ARGS: 
* Codec handle, number of args, register to edit, args to change (...).
*/
void wm8731_ctl_set(wm8731_handle w, uint8_t argNum, uint8_t reg, ...)
{
	va_list argList;
	uint8_t count = 0;
	uint8_t output;

	va_start(argList, reg);

	switch(reg)
	{
		// Right line in and out use same values as left.
		case WM8731_LLINEIN_CTL:
			output = w->lineIn;
			break;
		case WM8731_LHPOUT_CTL:
			output = w->lineOut;
			break;
		case WM8731_AAP_CTL:
			output = w->analogPath;
			break;
		case WM8731_DAP_CTL:
			output = w->digitalPath;
			break;
		case WM8731_POWER_CTL:
			output = w->power;
			break;
		case WM8731_DAIFMT:
			output = w->digitalFmt;
			break;
		case WM8731_SAMPLE_CTL:
			output = w->sampleRate;
			break;
	}

	for(count = 0 ; count < argNum ; count++)
	{
		SET_BIT(output, va_arg(argList, int));
	}

	va_end(argList);
}

// Transmit control data.
void wm8731_ctl_transmit(I2C_TypeDef I2Cx, uint8_t reg, uint8_t data)
{
	uint8_t transmitBuf[2];

	transmitBuf[0] = (reg << 1); // Shift reg address by one
	transmitBuf[1] = data;

	i2cTransmit(I2Cx, WM8731_ADDR, transmitBuf, 2);
}