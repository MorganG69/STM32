// My WM8731 driver for the STM32

#define WM8731_ADDR 0x34

#define I2S_MODE 0x02

#define DATA_LENGTH_16 0x00
#define DATA_LENGTH_20 0x01
#define DATA_LENGTH_24 0x02
#define DATA_LENGTH_32 0x03

#define STANDBY 0x7F
#define POWER_OFF 0xE0

// Register map
#define LLINEIN_CTL 0x00 
#define RLINEIN_CTL 0x01
#define LHPOUT_CTL 0x02
#define RHPOUT_CTL 0x03
#define AAP_CTL 0x04 
#define DAP_CTL 0x05
#define POWER_CTL 0x06
#define DAIFMT 0x07
#define SAMPLE_CTL 0x08
#define ACTIVE_CTL 0x09
#define RESET_CTL 0x0F

void wm8731_ctl_transmit(uint8_t reg, uint8_t data);
void i2cTransmit(I2C_TypeDef *I2Cx, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void i2cPrepare(I2C_TypeDef *I2Cx, uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request);
void i2cInit(void);
void periphClockInit(void);

// Taken from the ST HAL and reworked slightly to work with the LL.
#define I2C_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__FLAG__) == I2C_ISR_TXE) ? ((__HANDLE__)->ISR |= (__FLAG__)) \
                                                    : ((__HANDLE__)->ICR = (__FLAG__)))

#define I2C_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->ISR) & \
                                                    (__FLAG__)) == (__FLAG__)) ? SET : RESET)

