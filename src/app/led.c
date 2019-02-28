/**
 * @file led.c
 * @brief Handles LED operations.
*/

// ---------------------------------------------------------------------------
// Include files.
#include <asf.h>
#include "led.h"

// ---------------------------------------------------------------------------
// Macros and structures.
#define TC_COUNT_VALUE 16000
#define CONF_TC_INSTANCE TC3

// ---------------------------------------------------------------------------
// Module variables.
static APP_LED_STATE appLEDstate = APP_LED_OFF;
static struct tc_module tc_instance;

// ---------------------------------------------------------------------------
// Function prototypes.
static void configure_tc(void);
static void configure_tc_callbacks(void);
static void tc_callback_to_counter(struct tc_module *const module_inst);

/**
   @brief This will Initialize the timer to control the LED.
**/
void initAppLED(void)
{
	configure_tc();
	configure_tc_callbacks();
}

/**
   @brief This will set the state of the LED.
   @param state - state of the led.
**/void setStateLED(APP_LED_STATE state)
{
	appLEDstate = state;
}

/** @brief TC Callback function. Once a millisecond. 
	Toggle the LED at 200msec.
	@param module_inst instance of the timer.
 */
static void tc_callback_to_counter(struct tc_module *const module_inst)
{
	static uint16_t count = 0;
	count ++;
	if(appLEDstate == APP_LED_FLASH)
	{
		if(!(count % 200)){
			port_pin_toggle_output_level(LED_0_PIN);
			count = 0;
		}
	}
	if(appLEDstate == APP_LED_ON)
	{
		port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	}
	
	if(appLEDstate == APP_LED_OFF)
	{
		port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
	}

	tc_set_count_value(module_inst,TC_COUNT_VALUE);
}

/** @brief Configures  TC function with the  driver.
 */
static void configure_tc(void)
{
	struct tc_config config_tc;

	tc_get_config_defaults(&config_tc);
	config_tc.counter_size    = TC_COUNTER_SIZE_16BIT;
	config_tc.counter_16_bit.value = TC_COUNT_VALUE;
	tc_init(&tc_instance, CONF_TC_INSTANCE, &config_tc);
	tc_enable(&tc_instance);
}

/** @brief Registers TC callback function with the  driver.
 */
static void configure_tc_callbacks(void)
{
	tc_register_callback(
			&tc_instance,
			tc_callback_to_counter,
			TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
}