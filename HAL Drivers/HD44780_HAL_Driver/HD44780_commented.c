/*
	This driver is used to interface an STM32 with a generic Arduino 16x2 LCD. 
	The LCD uses the Hitachi hd44780 controller with a PCF8574 I2C IO expander backpack.
*/

#define I2C_LCD_ADDRESS 0x4E // Default address.


void i2c_lcd_sendDataByte(char input){
	uint8_t i2cBuf[3]; // I2C transmission buffer, each element will hold a 4 bit nibble.

	
	/*
		These are used to split the byte into high and low nibbles.
		For example, if input was 01010011, byteLOW would hold 00110000 (after bit shifting) and byteHigh would hold 01010000.
	*/
	char byteLow;
	char byteHigh;

	/*
		Bitwise AND to get the high bits.
		0xf0 == 11110000
		Bitwise AND will copy a bit to the result if that bit exists in BOTH operands.
		
		input  = 01010011
		0xf0   = 11110000
			---------- &
		Result = 01010000
	*/
	byteHigh = (input & 0xf0);

	/*
		Same again for the low bits, except input is bitwise shifted 4 bits to the left.
		
		input  = 01010011
		<< 4   = 00110000
	
		Same again...

		input  = 00110000
		0xf0   = 11110000
			---------- &
		Result = 00110000
	*/
	byteLow = ((input<<4) & 0xf0);


	/*
		The reason for this bit shifting to move the data into the high bits is because the lower 4 bits contain the control bits (r/w, en etc)
		So a typical 8 bit write to the IO chip would look something like this:

		bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0
		data  | data  | data  | data  | N/C   | en    | R/W   | RS 

		This is just because of how the PCF is connected to the display.
		Looking at the TI PCF8574 datasheet (page 12 section 8.3.2), the I2C data bytes are interpreted as:

		bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0	
		P7    | P6    | P5    | P4    | P3    | P2    | P1    | P0

		As you can see this lines up with how we are formatting the bytes and the physical pin connections.

		PCF8574 <-> hd4470
		P4-P7		D4-D7   // Data lines
		P2			en  	// Enable
		P1          R/W     // Read/Write
		P0			RS 		// Register select (RS=0: Command, RS=1: Data)   
	*/
	i2cBuf[0] = byteHigh | 0x0D; // Bitwise OR with enable high and register select high (0x0D = 1101)
	i2cBuf[1] = byteHigh | 0x09; // Same but with enable low (0x09 = 1001)
	i2cBuf[2] = byteLow | 0x0D;
	i2cBuf[3] = byteLow | 0x09;

	// ARGS = I2C device handle, device address, data buffer, sizeof data buffer, timeout. HAL_MAX_DELAY is essentially no timeout.
	HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2cBuf, 3, HAL_MAX_DELAY);
}


// This is the exact same function as data send except register select will be LOW.
void i2c_lcd_sendCmdByte(char input){
	uint8_t i2cBuf[3]; 
	char byteLow;
	char byteHigh;

	byteHigh = (input & 0xf0);
	byteLow = ((input<<4) & 0xf0);

	i2cBuf[0] = byteHigh | 0x0C; // Bitwise OR with enable high and register select low (0x0C = 1100)
	i2cBuf[1] = byteHigh | 0x08; // Same but with enable low (0x08 = 1000)
	i2cBuf[2] = byteLow | 0x0C;
	i2cBuf[3] = byteLow | 0x08;

	HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2cBuf, 3, HAL_MAX_DELAY);
}

// This is taken from the wikipedia page and the datasheet of the HD44780.
void i2c_lcd_init(void){

	// This first part sets the LCD to 4 bit mode, this is described on the wiki page.
	HAL_Delay(50);
	i2c_lcd_sendCmdByte(0x30);
	HAL_Delay(5);
	i2c_lcd_sendCmdByte(0x30);
	HAL_Delay(5);
	i2c_lcd_sendCmdByte(0x30);
	HAL_Delay(5);
	i2c_lcd_sendCmdByte(0x20);
	HAL_Delay(5);

	// Display initialisation, taken from datasheet and examples online.
	i2c_lcd_sendCmdByte(0x28);
	HAL_Delay(1);
	i2c_lcd_sendCmdByte(0x08);
	HAL_Delay(1);
	i2c_lcd_sendCmdByte(0x01);
	HAL_Delay(1);
	i2c_lcd_sendCmdByte(0x06);
	HAL_Delay(1);
	i2c_lcd_sendCmdByte(0x0C);
}

// Loops through the string and sends it using the data transmission function.
void i2c_lcd_sendString(char *str){
	while (*str) i2c_lcd_sendDataByte(*str++);
}

// 0x80 is the command to clear the display
void i2c_lcd_clear(void){
	i2c_lcd_sendCmdByte(0x80);
}


void i2c_lcd_putCursor(uint8_t row, uint8_t col){
	if(row == 0){
		col |= 0x80;
		i2c_lcd_sendCmdByte(col);
		return;
	} else if(row == 1){
	 	col |= 0xC0;
		i2c_lcd_sendCmdByte(col);
		return;
	} else {
		return;
	}
}
