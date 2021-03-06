// -----------------------------------------------------------------------------

/**
 * @file menu.c
 * @brief Handles the user menu.
*/

// -----------------------------------------------------------------------------
#define MENU_C

// ---------------------------------------------------------------------------
// Include files.
#include <stdio.h>
#include <string.h>
#include "menu.h"

// ---------------------------------------------------------------------------
// Function prototypes.
static void pad_string(int used_len, int padding, char pad_char);

// ---------------------------------------------------------------------------
// Module variables.
MENU menu;

/**
   @brief  initialize the menu module.
   @param p_menu - Menu defined in main code to operate on.
**/
int initialize_menu(MENU *p_menu)
{
   memcpy(&menu, p_menu, sizeof(MENU));
   menu.pPrint("Initialized menu\n");
   return(0);
}

/**
   @brief  Pads a string to be an equal amount of characters in between sections.
   @param used_len - length already used.
   @param padding  - How much padding to include.
   @param pad_char - What to pad with... commonly a space char ' '
**/
void pad_string(int used_len, int padding, char pad_char)
{
   char string[] = " ";

   string[0] = pad_char;
   
   for(int i = 0;  i < (padding - used_len); i++)
   {
      menu.pPrint(string);
   }
   return;
}

/**
   @brief  Prints the menu passed in.
   @param pCommands - Table of the commands.
   @param num_commands - number of commands in the table.
**/
void print_menu(const MENU_TABLE *pCommands, int num_commands)
{
   for(int count = 0; count < num_commands; count++)
   {
      if(pCommands[count].pCmd)
      {
         menu.pPrint(pCommands[count].pCmd);
         pad_string(strlen(pCommands[count].pCmd), 30, ' ');
         menu.pPrint("   :   ");
         menu.pPrint(pCommands[count].pDescription);
         menu.pPrint("\n");
      }
   }
   return;
}

/**
   @brief  runs the command on a string match.
   @param pCommands    - pointer to the command table.
   @param num_commands - number of commands in the table.
   @param pString      - String to compare to.
**/
int run_command(const MENU_TABLE *pCommands, int num_commands, char *pString)
{
   for(int count = 0; count < num_commands; count++)
   {
      if(!(strncmp(pCommands[count].pCmd, pString, strlen(pCommands[count].pCmd))))
      {
         (pCommands[count].mcb)((unsigned char*)pString, strlen(pString));
         return count;
      }
   }
   return -1;
}
