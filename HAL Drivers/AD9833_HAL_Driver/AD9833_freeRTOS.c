SemaphoreHandle_t AD9833_writeSemaphore; // Create a semaphore handle. This will be global.

// AD9833 task for writing the frequency.
void AD9833_setFrequency(void *params){
	AD9833_writeSemaphore = xSemaphoreCreateBinary(); // Create the semaphore using the handle.
	
	while(1){

		/*
		 * This will block until the semaphore is given by the button press ISR.
		 * After taking the semaphore, there is no need to give it back. Explained:
		 * Think of binary semaphores as a queue with a capacity of 1 item.
		 * When the semaphore is given by the ISR after pushing the button, 1 item is added to the queue making it full.
		 * When the semaphore is taken by this function, it pops the item off the queue leaving it empty.
		 * After the function is complete, the code returns to take the semaphore to find that it is not available (because it just took it, meaning the queue is empty) and blocks.
		 * If it is not available, the task is suspended for 100 ticks.
		 */

		if(xSemaphoreTake(AD9833_writeSemaphore, portMAX_DELAY) == pdTRUE){
			AD9833_Set_Output_DMA(n);
		} else {
			// If the semaphore is not available then block.
			vTaskDelay(100);
		}
	}
}