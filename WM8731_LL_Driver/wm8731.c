// Simple codec setup.
void wm8731_init(void)
{
	 wm8731_ctl_transmit(RESET_CTL, RESET_DEVICE);
	 wm8731_ctl_transmit(POWER_CTL, ALL_PERIPH_OFF);
	 wm8731_ctl_transmit(AAP_CTL, I2S_MODE);
	 LL_mDelay(1);
	 wm8731_ctl_transmit(DAIFMT, MASTER_16BIT_MSBL);

}

// Enable line input to output bypass.
void wm8731_bypass_enable(void)
{
	 wm8731_ctl_transmit(AAP_CTL, BYPASS_ENABLE);
	 wm8731_ctl_transmit(POWER_CTL, OUTPUTS_ENABLE);
	 wm8731_ctl_transmit(POWER_CTL, LINE_INPUTS_ENABLE);
}

// Transmit control data to the codec over I2C
void wm8731_ctl_transmit(uint8_t reg, uint8_t data)
{
	uint8_t transmitBuf[2];

	transmitBuf[0] = (reg << 1); // Shift reg address by one
	transmitBuf[1] = data;

	i2cTransmit(I2C1, WM8731_ADDR, transmitBuf, 2);
}