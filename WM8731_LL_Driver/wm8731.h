#define WM8731_ADDR 0x34 // Actual address is 0x1A but has to be left shifted for the R/W bit.

// Inputs and outputs
//#define WM8731_BYPASS_ENABLE 0x1A // Enables the DAC and bypass, mutes the ADC mic input.
//#define WM8731_OUTPUTS_ENABLE 0x6F
//#define WM8731_LINE_INPUTS_ENABLE 0x6E
//#define WM8731_LINE_INPUTS_MUTE 0x80

// Line input control
#define WM8731_LINEINPUT_MUTE 0x80
#define WM8731_LINEINPUT_UNMUTE 0x00

// Headphone output control
#define WM8731_HP_ZEROCROSS_ENABLE 0x80
#define WM8731_HP_ZEROCROSS_DISABLE 0x00

// Analogue audio path control
#define WM8731_MIC_BOOST_ENABLE 0x01
#define WM8731_MIC_BOOST_DISABLE 0x00
#define WM8731_MIC_MUTE 0x02
#define WM8731_MIC_UNMUTE 0x00
#define WM8731_ADC_INPUT_MIC 0x04
#define WM8731_ADC_INPUT_LINE 0x00
#define WM8731_BYPASS_ENABLE 0x08
#define WM8731_BYPASS_DISABLE 0x00
#define WM8731_DAC_SELECT 0x10
#define WM8731_DAC_DESELECT 0x00
#define WM8731_SIDETONE_15DB 0xC0
#define WM8731_SIDETONE_12DB 0x80
#define WM8731_SIDETONE_9DB 0x40
#define WM8731_SIDETONE_6DB 0x00

// Digital audio path control
#define WM8731_ADC_HPF_ENABLE 0x01
#define WM8731_ADC_HPF_DISABLE 0x00
#define WM8731_DEEMP_48K 0x06
#define WM8731_DEEMP_44K 0x04
#define WM8731_DEEMP_32K 0x02
#define WM8731_DEEMP_DISABLE 0x00
#define WM8731_DAC_MUTE 0x80
#define WM8731_DAC_UNMUTE 0x00
#define WM8731_HPF_DCSTORE 0x10
#define WM8731_HPF_DCCLEAR 0x00

// Power down control
#define WM8731_ALL_PERIPH_OFF 0x7F
#define WM8731_ALL_PERIPH_ON 0x00
#define WM8731_LINEINPUT_DISABLE 0x01
#define WM8731_LINEINPUT_ENABLE 0x00
#define WM8731_MICINPUT_DISABLE 0x02
#define WM8731_MICINPUT_ENABLE 0x00
#define WM8731_ADC_DISABLE 0x04
#define WM8731_ADC_ENABLE 0x00
#define WM8731_DAC_DISABLE 0x08
#define WM8731_DAC_ENABLE 0x00
#define WM8731_OUTPUTS_DISABLE 0x10
#define WM8731_OUTPUTS_ENABLE 0x00
#define WM8731_OSC_DISABLE 0x20
#define WM8731_OSC_ENABLE 0x00
#define WM8731_CLKOUT_DISABLE 0x40
#define WM8731_CLKOUT_ENABLE 0x00
#define WM8731_POWER_OFF 0x80
#define WM8731_POWER_ON 0x00

// Digital audio interface format
#define WM8731_FMT_DSP 0x03
#define WM8731_FMT_I2S 0x02
#define WM8731_FMT_MSBL 0x01
#define WM8731_FMT_MSBR 0x00
#define WM8731_FMT_32BIT 0x0C
#define WM8731_FMT_24BIT 0x08
#define WM8731_FMT_20BIT 0x04
#define WM8731_FMT_16BIT 0x00
#define WM8731_DACLRC_PHASE_HIGH 0x10
#define WM8731_DACLRC_PHASE_LOW 0x00
#define WM8731_DAC_CLKSWAP_ENABLE 0x20
#define WM8731_DAC_CLKSWAP_DISABLE 0x00
#define WM8731_MASTER_MODE 0x40
#define WM8731_SLAVE_MODE 0x00
#define WM8731_BITCLK_INVERT 0x80
#define WM8731_BITCLK_NONINVERT 0x00

// Sampling control
#define WM8731_USB_MODE 0x01
#define WM8731_NORMAL_MODE 0x00
#define WM8731_BOSR_256FS 0x00
#define WM8731_BOSR_384FS 0x02
#define WM8731_CORECLK_DIV2 0x40
#define WM8731_CORECLK_NODIV 0x00
#define WM8731_CLKOUT_DIV2 0x80
#define WM8731_CLKOUT_NODIV 0x00

// Active control
#define WM8731_ACTIVE_ENABLE 0x01
#define WM8731_ACTIVE_DISABLE 0x00

// Reset control
#define WM8731_RESET 0x00

// Register addresses.
#define WM8731_LLINEIN_CTL 0x00
//#define WM8731_RLINEIN_CTL 0x01
#define WM8731_LHPOUT_CTL 0x02
//#define WM8731_RHPOUT_CTL 0x03
#define WM8731_AAP_CTL 0x04
#define WM8731_DAP_CTL 0x05
#define WM8731_POWER_CTL 0x06
#define WM8731_DAIFMT 0x07
#define WM8731_SAMPLE_CTL 0x08
#define WM8731_ACTIVE_CTL 0x09
#define WM8731_RESET_CTL 0x0F

// Device handle
typedef struct 
{
	// Handle to the I2C peripheral controlling the codec.
	I2C_TypeDef i2cHandle;
	/*
	* These hold the current register values. 
	* So individual bits can be changed without ORing the whole thing together everytime.	
	*/
	uint8_t lineIn;
	uint8_t lineOut;
	uint8_t analogPath;
	uint8_t digitalPath;
	uint8_t power;
	uint8_t digitalFmt;
	uint8_t sampleRate;
}wm8731_handle_t;

typedef wm8731_handle_t *wm8731_handle;

// Control functions
void wm8731_ctl_transmit(uint8_t reg, uint8_t data);
wm8731_handle *wm8731_init(void);
void wm8731_bypass_enable(wm8731_handle w);

