// ---------------------------------------------------------------------------
/**
 *
 * @file main.c
 *
 *  @brief This will do minimum startup and call the userInterface
 *
 *
 **/
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Include files.
#include <asf.h>
#include "led.h"
#include "user_interface.h"

/**
   @brief This will do minimum startup and call the userInterface
   **/
int main (void)
{
	system_init();
	delay_init();
	udc_start();
	
	// Start the LED
	initAppLED();
	initUserInterface();
	
    system_interrupt_enable_global();
	runUserInterface();
	/* Insert application code here, after the board has been initialized. */

	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {
		delay_cycles_ms(3000);
	}
}

