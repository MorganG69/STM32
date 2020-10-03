#define WM8731_ADDR 0x34 // Actual address is 0x1A but has to be left shifted for the R/W bit.

#define I2S_MODE 0x02

#define DATA_LENGTH_16 0x00
#define DATA_LENGTH_20 0x01
#define DATA_LENGTH_24 0x02
#define DATA_LENGTH_32 0x03

#define BYPASS_ENABLE 0x1A // Enables the DAC and bypass, mutes the ADC mic input.
#define OUTPUTS_ENABLE 0x6F
#define LINE_INPUTS_ENABLE 0x6E

#define STANDBY 0x7F
#define POWER_OFF 0xE0
#define RESET_DEVICE 0x00
#define ALL_PERIPH_OFF 0x7F

#define MASTER_16BIT_MSBL 0x42 // MSBL m.s.b first left justified

// Register addresses.
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
void wm8731_init(void);
void wm8731_bypass_enable(void);