/*
	This is my own timer setup and interrupt code for setting timer 21 and timer 2 as a gated frequency counter
	on the STM32L031 Cortex M0+ chip.
	I am using some function from STs low level drivers and my own bit manipulation. The ST HAL drivers are quite 
	bloated for this chip, with it having only 32k of ROM.

	Two timers are needed for the frequency counter, a slave and a master. This is called a 'gated timer'.
	The slave timers clock source is setup to be our input signal, this means that its count register will increment
	every time the input signal goes high. It will only do this when the master timer has triggered.
	By triggering the master timer for one second, we are able to measure the frequency of the input signal.
	This is because the slave timer will only be active for one second so its count register will be the frequency of
	the input signal.

	The master timer is setup to trigger for one second and not trigger for one second (for a period of 2 seconds) by 
	setting its prescaler and a couple of other compare registers.
*/

static void TIM21_Frequency_Counter_Init(void){

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);

	/* TIM21 interrupt Init */
	NVIC_SetPriority(TIM21_IRQn, 0);
	NVIC_EnableIRQ(TIM21_IRQn);

	/* 
	 * Set the prescaler value.
	 * This was calculated as:
	 * (FCPU = 32MHz) / 16000 = 2000 ticks
	 * 1 tick = 500us
	 * 2000 ticks = 1s
	 */
	TIM21->PSC = 0x3E7F; // 0x3E7F = 15999. Prescaler = Actual value - 1.

	/*
	 * Set the timer control register one to all zero.
	 * All of the setting bits I need are zero anyway.
	 * Clock division, count up, ARP...
	 */
	TIM21->CR1 = 0;

	// Set the timers clock source to the internal clock.
	TIM21->SMCR = LL_TIM_CLOCKSOURCE_INTERNAL;
	// Set the timer to master/slave mode. This is the master timer.
	TIM21->SMCR = TIM_SMCR_MSM;
	// Set the output compare 1 reference signal to be used as the trigger output.
	TIM21->CR2 = LL_TIM_TRGO_OC1REF;

	// Sets bits 4-6 in the CCMR1 register to 110. This sets the counter to only be high when CNT < CCR1. CNT is incremented once every tick.
	TIM21->CCMR1 |= (0x6 << 4);
	// Sets the capture compare register to 2000. When CNT reaches this value, the timer trigger is switched low.
	TIM21->CCR1 = 2000;
	// Sets auto reload register to 3999. When the timer hits this value, the interrupt handler is called.
	TIM21->ARR = 3999;
	// Enable timer update interrupt.
	TIM21->DIER |= (1u<<0);
	// Enable the counter.
	TIM21->CR1 |= (1u<<0);
}

// This is the timer 21 interrupt handler. When CNT reaches 3999 this function is called.
void TIM21_IRQHandler(void)
{
  // Disable the counter.
  TIM21->CR1 &= ~(1<<0);
  // Reset the count to 0
  TIM21->CNT = 0;
  // Update the frequency variable from the slave counter.
  inputFrequency = TIM2->CNT;
  // Reset the frequency value.
  TIM2->CNT = 0;
  // This bit is set by the hardware when the interrupt is called and must be reset by software.
  TIM21->SR &= ~(1<<0);
  // Re enable the counter
  TIM21->CR1 |= 1<<0;
}
