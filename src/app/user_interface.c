// ---------------------------------------------------------------------------
/** 
 *
 * @user_interface.c
 * 
 *  @brief Handles the communications to terminal and button actions.
 *  This will Change the state of the led on a button press.
 *  This will also initialize and control the menu system.
 *  
 **/ 
// ---------------------------------------------------------------------------
#define HW_USER_INTERFACE_C ///< C file definition 

// ---------------------------------------------------------------------------
// Include files.
#include <string.h>
#include "asf.h"
#include "led.h"
#include "user_interface.h"
#include "buffer.h"
#include "menu.h"

//------------------------------------------------------------------------------
// Macros
#define BUFFER_SIZE_UART (BUFFER_MEDIUM)

//------------------------------------------------------------------------------
// Module Variables
static T_BUFFER bufferUartRx;             ///< Used for receive buffering.
static uint8_t rxdata[BUFFER_SIZE_UART];  ///< Used for holding for ::T_BUFFER.
static uint8_t gotSomething = 0;          ///< Indicates that we received full byte.
static uint8_t readDataBuffer[BUFFER_SIZE_UART]; ///< Used for reading the bytes once received.
static bool my_flag_autorize_cdc_transfert = false;

// ---------------------------------------------------------------------------
// Function prototypes.
static void userInterfacePrint(const char *pData);
static int toggleLED(unsigned char *pData, int length);
static int printHelp(unsigned char *pData, int length);bool my_callback_cdc_enable(void);
void startMenu(void);
void my_callback_cdc_disable(void);
void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_detection_callback(void);

///! Command table used to display the help menu
static const   MENU_TABLE commandTable[] = 
{ 
   { "?",     "Print help",                          printHelp },
   { "led",   "Toggle the [LED / ON / OFF / FLASH]", toggleLED }
};       
#define CMD_TBL_SIZE (sizeof(commandTable) / sizeof(MENU_TABLE))

/**
   @brief initUserInterface will init the external interrupt for the button,
   init the buffer for communications, and start the menu system.
**/
void initUserInterface(void)
{
	configure_extint_channel();
	configure_extint_callbacks();
	delay_ms(1000);
	userInterfacePrint((char *)"Testing the xplained board\n\r");
	userInterfacePrint((char *)"Press button to change state of the LED (ON OFF FLASH)\n\r");
	
   // Initialize the UART receive buffer
   memset(&bufferUartRx, 0, sizeof(T_BUFFER));
   bufferUartRx.size = BUFFER_SIZE_UART;
   bufferUartRx.pBuf = rxdata;
   initBuffer(&bufferUartRx);
   startMenu();
   return;
}

/**
   @brief runUserInterface will be the main loop of the program.
   It will launch the requested commands from the user.
**/
void runUserInterface(void)
{
	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {
	   delay_cycles_ms(100);
	   if(gotSomething)
       {
         int rVal;
         memset(readDataBuffer, 0, BUFFER_SIZE_UART);
         (void)readBuffer(&bufferUartRx, readDataBuffer, BUFFER_SIZE_UART);
         rVal = run_command(commandTable, CMD_TBL_SIZE, (char*)readDataBuffer);
         if(rVal < 0)
         {
            userInterfacePrint("Command not found: ");
            userInterfacePrint((char*)readDataBuffer);
            userInterfacePrint("\n");
         }
         gotSomething = 0;
      }
	}
}

/**
   @brief This is called on enable of USB.
**/
bool my_callback_cdc_enable(void)
{
	my_flag_autorize_cdc_transfert = true;
	return true;
}

/**
   @brief This is called on Disable of USB.
**/
void my_callback_cdc_disable(void)
{
	my_flag_autorize_cdc_transfert = false;
}

/**
   @brief This is called on receiving of USB.
**/
void my_callback_rx_notify(uint8_t port)
{
	char rchar;
	T_BUFFER *pBuffer = &bufferUartRx;
	
	//	port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	rchar = (char)udi_cdc_getc();
    if(rchar == '\n' || rchar == '\r')
	{
      gotSomething = 1;
    }else
    {
      buff_write_char(pBuffer, rchar);
    }

	udi_cdc_putc(rchar);
}

/**
   @brief This will print to the USB console.
   @param pData - Pointer to the message to print.
**/
void userInterfacePrint(const char *pData)
{
	size_t len = strlen(pData);
	for(size_t count = 0; count < len; count++)
	{
		if(*pData == '\n')
		{
			char send = '\r';
			udi_cdc_putc((int)send);
		}
		udi_cdc_putc((int)*pData);
		pData++;
	}
}

/**
   @brief This will configure the button interrupt.
**/
void configure_extint_channel(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	
	config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
}

/**
   @brief This will configure the button interrupt callback.
**/
void configure_extint_callbacks(void)
{
    extint_register_callback(extint_detection_callback,
       BUTTON_0_EIC_LINE,
       EXTINT_CALLBACK_TYPE_DETECT);
       extint_chan_enable_callback(BUTTON_0_EIC_LINE,
       EXTINT_CALLBACK_TYPE_DETECT);
}

/**
   @brief This will Handle the button being pressed.
   It will cycle through the LED states.
**/
void extint_detection_callback(void)
{
	static APP_LED_STATE state = APP_LED_OFF;
	bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
    port_pin_set_output_level(LED_0_PIN, pin_state);
	if(pin_state)
	{
		if(state++ >= MAX_LED_STATE)
		{
			state = APP_LED_OFF;
		}
		setStateLED(state);
	}
}

/**
   @brief This will initialize and start the menu.
**/
void startMenu(void)
{
   MENU newMenu;
   
   memset(&newMenu, 0, sizeof(MENU));
   
   newMenu.pPrint = userInterfacePrint;
   
   initialize_menu(&newMenu);
   print_menu(commandTable, (sizeof(commandTable) / sizeof(MENU_TABLE)));

}

/**
   @brief This is the callback for changing the state of the led.
   @param pData - The command is passed in for further examination.
   @param length - Length of the string passed in.
   @TODO probably rename
**/
int toggleLED(unsigned char *pData, int length)
{
   userInterfacePrint("Todo Toggle the LED\n");

   if(!strncmp((char *)pData, "led ON", strlen("led ON")))
   {
      userInterfacePrint("::ON\n");
	  setStateLED(APP_LED_ON);
   }
   if(!strncmp((char *)pData, "led OFF", strlen("led OFF")))
   {
	  setStateLED(APP_LED_OFF);
      userInterfacePrint("::OFF\n");
   }
   if(!strncmp((char *)pData, "led FLASH", strlen("led FLASH")))
   {
	  setStateLED(APP_LED_FLASH);
      userInterfacePrint("::FLASH\n");
   }
   return(0);
}

/**
   @brief This will print the help menu.
   @param pData - Not used.
   @param length - Not used.
**/
int printHelp(unsigned char *pData, int length)
{
	print_menu(commandTable, (sizeof(commandTable) / sizeof(MENU_TABLE)));
	return(0);
}