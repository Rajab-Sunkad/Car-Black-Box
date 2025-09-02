#ifndef MAIN_H
#define MAIN_H

// Include necessary libraries and module headers
#include <xc.h>                 // Header for microcontroller-specific definitions
#include "clcd.h"               // Custom header for CLCD operations
#include "adc.h"                // Custom header for ADC operations
#include "mkp.h"                // Custom header for Matrix Keypad operations
#include "uart.h"               // Custom header for UART communication
#include "i2c.h"                // Custom header for I2C communication
#include "external_eeprom.h"    // Custom header for external EEPROM operations
#include "ds1307.h"             // Custom header for DS1307 RTC operations

// Function declarations
void dashb(void);               // Displays the dashboard screen
void mainmenu(void);            // Displays the main menu
void write_event(void);         // Logs an event to external storage
void view_log(void);            // Displays stored event logs
void event_reader(void);        // Reads stored events from EEPROM
void clear_log(void);           // Clears all stored logs
void download_log(void);        // Downloads logs to a serial terminal
void display_time(void);        // Displays the current time
static void get_time(void);     // Fetches the current time from RTC
void set_time(void);            // Allows the user to set the current time
void print_clcd(void);          // Prints the formatted time on the CLCD

// Enum for application states
typedef enum
{
    DASH_BOARD,                 // State for the dashboard display
    MAIN_MENU,                  // State for the main menu display
    VIEW_LOG,                   // State for viewing logs
    DOWNLOAD_LOG,               // State for downloading logs
    CLEAR_LOG,                  // State for clearing logs
    SET_TIME                    // State for setting the time
} state_t;

#endif /* MAIN_H */
