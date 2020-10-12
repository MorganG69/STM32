// Status codes for the I2C interface.
typedef enum
{
	I2C_FAULT_BUSY = 0,
	I2C_FAULT_ADDR,
	I2C_FAULT_SB,
	I2C_FAULT_TXE,
	I2C_FAULT_BTF,
	I2CTX_SUCCESS,
	I2CRX_SUCCESS

}_i2c_status;

typedef _i2c_status i2c_status_t;



// Imporved polling transmit function with timeouts and specific fail/success codes.
i2c_status_t i2cTransmit(I2C_TypeDef I2Cx, uint8_t devAddress, uint8_t *data, uint16_t size)
{	

	uint8_t *dataPtr = data;
	volatile uint16_t XferCount = size;
	uint32_t timeOut = 1000;
	uint32_t retryCount = 0;

	// Wait for bus to be free.
	while((LL_I2C_IsActiveFlag_BUSY(I2Cx) == 1) && (retryCount < timeOut))retryCount++;
	if(retryCount >= timeOut) return I2C_FAULT_BUSY;
	retryCount = 0;
	
	LL_I2C_GenerateStartCondition(I2Cx);

	// Wait for the SB flag to be set.
	while((LL_I2C_IsActiveFlag_SB(I2Cx) != 1) && (retryCount < timeOut))retryCount++;
	if(retryCount >= timeOut) return I2C_FAULT_SB;
	retryCount = 0;
	
	// Put the address into the data register.
	// Automatically transmits it.
	I2Cx->DR = devAddress; 

	// Wait for the ADDR flag to set.
	while((LL_I2C_IsActiveFlag_ADDR(I2Cx) != 1) && (retryCount < timeOut))retryCount++;
	if(retryCount >= timeOut) return I2C_FAULT_ADDR;
	retryCount = 0;

	LL_I2C_ClearFlag_ADDR(I2Cx); // Clear the address flag

	while(XferCount > 0U)
	{
		// Wait for the data register to be empty	
		while((LL_I2C_IsActiveFlag_TXE(I2Cx) != 1) && (retryCount < timeOut))retryCount++;
 		if(retryCount >= timeOut) return I2C_FAULT_TXE;
		retryCount = 0;
		
		// Fill the data register. Device automatically transmits it.
		I2Cx->DR = *dataPtr; 
		dataPtr++; // Increment the data pointer
		XferCount--; // Decrease the packet count
		
		// Wait for the byte transfer complete flag to be set.
		while((LL_I2C_IsActiveFlag_BTF(I2Cx) != 1) && (retryCount < timeOut))retryCount++;
		if(retryCount >= timeOut) return I2C_FAULT_BTF;
		retryCount = 0;
	}
	
	LL_I2C_GenerateStopCondition(I2Cx);
	return I2CTX_SUCCESS;
}
