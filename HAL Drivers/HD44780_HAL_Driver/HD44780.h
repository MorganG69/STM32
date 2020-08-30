#define I2C_LCD_ADDRESS 0x4E

void i2c_lcd_sendDataByte(char input);
void i2c_lcd_sendCmdByte(char input);
void i2c_lcd_init(void);
void i2c_lcd_sendString(char *str);
void i2c_lcd_clear(void);
void i2c_lcd_putCursor(uint8_t row, uint8_t col);