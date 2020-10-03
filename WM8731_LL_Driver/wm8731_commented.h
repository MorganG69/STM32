// My WM8731 driver for the STM32

// Register addresses and description..
// All registers are 16 bits wide, 7 bit address and 9 bit data.

// CSB is low on the BLA54 board.
#define WM8731_ADDR 0x1A

#define I2S_MODE 0x02

#define DATA_LENGTH_16 0x00
#define DATA_LENGTH_20 0x01
#define DATA_LENGTH_24 0x02
#define DATA_LENGTH_32 0x03

// Write this to the power down control register to put the device into standby or power off.
// The control interface remains active in these modes.
#define STANDBY 0x7F
#define POWER_OFF 0xE0

/* 
* Left/Right Line in registers.
* Controls line input volume and mute function to the ADC.
* Bit[0:4] = Line input volume. 
* Bit 7 = Line input mute to ADC. 
* Bit 8 = enables simultaneous control of these registers.
*/
#define LLINEIN_CTL 0x00 
#define RLINEIN_CTL 0x01

/*
* Left/Right Headphone out registers.
* Controls the on board audio amplifier.
* Bit[0:6] = Volume control. Write < 0110000 to these registers to mute the outputs individually.
* Bit 7 = Channel zero cross detect. This means a volume control update will only occur when the analogue ouput is close to 0v.
* Bit 8 = Allows channel control to be locked together. Enable either the left or right register bit and it will control the other.
*/ 
#define LHPOUT_CTL 0x02
#define RHPOUT_CTL 0x03

/* 
* Analogue audio path control register. 
* Controls mic input level boost, mute to ADC, ADC input select and line out control.
* Bit 0 = mic boost enable/disable. 
* Bit 1 = Mic input mute to ADC.
* Bit 3 = Line input bypass.
* Bit 4 = Dac select.
* Bit 5 = Mic input bypass (sidetone).
* Bit[6:7] = Side tone attenuation control.
*/
#define AAP_CTL 0x04 

/*
* Digital audio path control register.
* Contains ADC and DAC filters that control the onboard signal processing.
* Bit 0 = ADC HPF enable/disable.
* Bit 4 = Store DC offset when HPF disabled. The will be the last DC offset stored before the HPF was disabled.
*		  This will continue to be subtracted from the input and wont change until the HPF is enabled.
* Bit[2:1] = Digital de-emphasis control.
* Bit 3 = DAC soft mute control.
*/
#define DAP_CTL 0x05

/*
* Power down control register.
* Bit 0 = Line input power down.
* Bit 1 = Mic input bias power down.
* Bit 2 = ADC power down.
* Bit 3 = DAC power down.
* Bit 4 = Line output power down.
* Bit 5 = Oscillator power down.
* Bit 6 = Clock output power down.
* Bit 7 = Power off the device.
*/
#define POWER_CTL 0x06

/*
* Digital audio interface format register.
* Digital format configuration such as mode, data length and phase control.
* Bit[0:1] = Audio data format select.
* Bit[2:3] = Input data length.
* Bit 4 = DAC left right clock phase control
* Bit 5 = DAC left right clock swap. 
* Bit 6 = Master slave control.
* Bit 7 = Bit Clock invert.
*/ 
#define DAIFMT 0x07

/*
* Sampling control register.
* Bit 0 = USB or normal mode select.
* Bit 1 = Base Over-Sampling rate (BOSR). This is the rate that the digital signal processing is carried out.
* Bit[2:5] = ADC and DAC sample rate control.
* Bit 6 = Core clock divide by 2 select.
* Bit 7 = Clock output divide by 2 select.
*
* For DAC and ADC sample rates of 48Khz, the device should be programmed as BOSR = 0 and bits 2-5 as 0.
*/
#define SAMPLE_CTL 0x08

/*
* Active control register.
* Once the audio interface and sampling control is programmed the device can be activated here.
* It is recommended to disable the interface when making changes to the sample contol or digital interface register.
* Bit 0 = Active enable/disable.
*/
#define ACTIVE_CTL 0x09

/*
* Device reset register.
* Write bits 0:8 to 0 to reset the device
*/
#define RESET_CTL 0x0F
