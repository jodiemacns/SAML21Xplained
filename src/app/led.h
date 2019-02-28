// ---------------------------------------------------------------------------
/** 
 *
 * @file led.h
 * 
 *  @brief This is the header for the led.c file.
 *  
 *
 **/ 
// ---------------------------------------------------------------------------

#ifndef LED_H_
#define LED_H_

// ---------------------------------------------------------------------------
// Macros and structures.

//! brief - These are the different states of the LED.
typedef enum
{
	APP_LED_OFF,	///< OFF
	APP_LED_ON,     ///< ON
	APP_LED_FLASH,  ///< Flash 200 msec.
	MAX_LED_STATE   ///< Used to check limits.
}APP_LED_STATE;

// ---------------------------------------------------------------------------
// Function prototypes.
void initAppLED(void);
void setStateLED(APP_LED_STATE);

#endif /* LED_H_ */