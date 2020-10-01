// My WM8731 driver for the STM32
#include "wm8731.h"

void i2cInit(void)
{
  // THE GPIO MUST BE CONFIGURED IN THIS ORDER TO WORK!!!!!
  LL_GPIO_SetPinSpeed(GPIOA, I2C_SDA, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, I2C_SDA, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOA, I2C_SDA, LL_GPIO_PULL_UP);
  LL_GPIO_SetAFPin_8_15(GPIOA, I2C_SDA, LL_GPIO_AF_1);
  LL_GPIO_SetPinMode(GPIOA, I2C_SDA, LL_GPIO_MODE_ALTERNATE);

  // THE GPIO MUST BE CONFIGURED IN THIS ORDER TO WORK!!!!!
  LL_GPIO_SetPinSpeed(GPIOA, I2C_SCL, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, I2C_SCL, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOA, I2C_SCL, LL_GPIO_PULL_UP);
  LL_GPIO_SetAFPin_8_15(GPIOA, I2C_SCL, LL_GPIO_AF_1);
  LL_GPIO_SetPinMode(GPIOA, I2C_SCL, LL_GPIO_MODE_ALTERNATE);
		
  LL_I2C_Disable(I2C1); // The interface must be disabled to configure the filters and timing.
  LL_I2C_ConfigFilters(I2C1, LL_I2C_ANALOGFILTER_ENABLE, 0); // Second arg is the digital filter.
  LL_I2C_SetTiming(I2C1, 0x00707CBB); // This is calculated by CubeMX.
  LL_I2C_Enable(I2C1); // Re enable
  LL_I2C_SetOwnAddress1(I2C1, 0, LL_I2C_OWNADDRESS1_7BIT); // Own address of 0.
  LL_I2C_EnableOwnAddress1(I2C1);
  LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK); // Generates the acknowledge after a receive byte.
  LL_I2C_EnableAutoEndMode(I2C1); // Automatically generates the stop condition.
  LL_I2C_EnableClockStretching(I2C1); // Allows a slave to make us wait if it needs to slow down. It holds SCL low.
}

void periphClockInit(void)
{
	// System
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	
	// GPIO
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	
	// I2C
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
}

/*
* Transmit a command to the codec.
* Format: Address...ACK...Register address...ACK...data
*/
void wm8731_ctl_transmit(uint8_t reg, uint8_t data)
{
	uint8_t transmitBuf[2];

	transmitBuf[0] = reg;
	transmitBuf[1] = data;

	i2cTransmit(I2C1, WM8731_ADDR, transmitBuf, 2);
}

// Based around the HAL function but smaller.
void i2cTransmit(I2C_TypeDef *I2Cx, uint16_t DevAddress, uint8_t *pData, uint16_t Size){
	uint8_t *dataPtr = pData;
	volatile uint16_t XferCount = Size;
	uint16_t XferSize = XferCount;

  // This initialises the peripheral for master transmission and sends the slave address.
	i2cPrepare(I2Cx, DevAddress, (uint8_t)XferSize, I2C_CR2_AUTOEND, LL_I2C_GENERATE_START_WRITE);

	while(XferCount > 0U){

		while(I2C_GET_FLAG(I2Cx, I2C_ISR_TXIS) != 1){} // This will be set when the transmit data register (TXDR) is empty.

		/* 
		* Fill The transmit data register with one byte.
		* When the TXDR is full (1 byte), the STM32 will automatically send the data onto the line.
		*/
		I2Cx->TXDR = *dataPtr;

		dataPtr++; // Increment the data pointer.
		XferCount--; /// Keeps track of where we are in the buffer.
	}
    I2C_CLEAR_FLAG(I2Cx, I2C_ISR_STOPF); // Clear the stop condition flag for next transfer.
}

/* 
* This function is taken from HAL and modified.
* Before transmission the CR2 register must be configured with information about the transmission and how to generate start and stop bits.
*/
void i2cPrepare(I2C_TypeDef *I2Cx, uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{

  /* update CR2 register */
  MODIFY_REG(I2Cx->CR2,((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | \
               (I2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), \
             (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) |
                        (((uint32_t)Size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)Mode | (uint32_t)Request));
}
