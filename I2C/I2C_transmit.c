void i2cTransmit(I2C_TypeDef I2Cx, uint8_t devAddress, uint8_t *data, uint16_t size)
{	
	uint8_t *dataPtr = data;
	volatile uint16_t XferCount = size;
		
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx) == 1){} // Wait for the bus to be free.

	LL_I2C_GenerateStartCondition(I2Cx);

	while(LL_I2C_IsActiveFlag_SB(I2Cx) != 1){} // Wait for the SB flag to be set.
	
	// Put the address into the data register.
	// Automatically transmits it.
	I2Cx->DR = devAddress; 

	while(LL_I2C_IsActiveFlag_ADDR(I2Cx) != 1){} // Wait for the ADDR flag to set.

	LL_I2C_ClearFlag_ADDR(I2Cx); // Clear the address flag

	while(XferCount > 0U)
	{
		while(LL_I2C_IsActiveFlag_TXE(I2Cx) != 1){} // Wait for the data register to be empty	
		
		// Fill the data register. Device automatically transmits it.
		I2Cx->DR = *dataPtr; 
		dataPtr++;
		XferCount--;
		
		// Wait for the byte transfer complete flag to be set.
		while(LL_I2C_IsActiveFlag_BTF(I2Cx) != 1){}
	}
	
	LL_I2C_GenerateStopCondition(I2Cx);
}
