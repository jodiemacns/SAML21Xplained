// ---------------------------------------------------------------------------
/** 
 *
 * @file menu.h
 * 
 *  @brief This is the header for the menu.c file.
 *  
 *
 **/ 
// ---------------------------------------------------------------------------

#ifndef MENU_H_
#define MENU_H_

// ---------------------------------------------------------------------------
// Macros typedefs and structures.
typedef int (*menu_callback)(unsigned char *pData, int length);
typedef void (*menu_print)(const char *pData);
typedef struct MENU_TABLE MENU_TABLE;

///! @brief Menu table for describing your menus.
struct MENU_TABLE 
{
   const char *pCmd;			///< Pointer to the command to be typed.
   const char *pDescription;    ///< Pointer to the description for help.
   menu_callback mcb;           ///< Pointer to the callback function.
};

///! @brief Module structure for the menu system.
typedef struct
{
   menu_print pPrint; ///< Pointer the the print function.
}MENU;
   
 
#define M_NUM_CMDS(x) (sizeof(x) / sizeof(MENU_TABLE))
#define MAX_PRE_STRING 50                      

// ---------------------------------------------------------------------------
// Function prototypes.
int initialize_menu(MENU *p_menu);
void print_menu(const MENU_TABLE *pCommands, int num_commands);
int run_command(const MENU_TABLE *pCommands, int num_commands, char *pString);

#endif