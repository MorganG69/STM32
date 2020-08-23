#define I2C_LCD_ADDRESS 0x4E

void i2c_lcd_sendDataByte(char input){
	uint8_t i2cBuf[4];
	char byteLow;
	char byteHigh;

	byteHigh = (input&0xf0);
	byteLow = ((input<<4)&0xf0);

	i2cBuf[0] = byteHigh|0x0D;
	i2cBuf[1] = byteHigh|0x09;
	i2cBuf[2] = byteLow|0x0D;
	i2cBuf[3] = byteLow|0x09;

	HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, (uint8_t *)i2cBuf, 4, 100);
}

void i2c_lcd_sendCmdByte(char input){
	uint8_t i2cBuf[4];
	char byteLow;
	char byteHigh;

	byteHigh = (input&0xf0);
	byteLow = ((input<<4)&0xf0);

	i2cBuf[0] = byteHigh|0x0C;
	i2cBuf[1] = byteHigh|0x08;
	i2cBuf[2] = byteLow|0x0C;
	i2cBuf[3] = byteLow|0x08;

	HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, (uint8_t *)i2cBuf, 4, 100);
}

void i2c_lcd_init(void){
	HAL_Delay(50);
	i2c_lcd_sendCmdByte(0x30);
	HAL_Delay(5);
	i2c_lcd_sendCmdByte(0x30);
	HAL_Delay(5);
	i2c_lcd_sendCmdByte(0x30);
	HAL_Delay(5);
	i2c_lcd_sendCmdByte(0x20);
	HAL_Delay(5);

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

void i2c_lcd_sendString(char *str){
	while (*str) i2c_lcd_sendDataByte(*str++);
}

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
