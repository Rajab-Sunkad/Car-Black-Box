/*
 * NAME: RAJAB J SUNKAD
 * DATE: 16/12/2024
 * PROJECT - CAR BLACK BOX.
 * Description: A car black box, also known as an Event Data Recorder (EDR), is a device installed in vehicles to record critical 
                operational data. 
                Developed a compact and cost-effective car black box designed to accurately record and store essential vehicle data 
                such as speed, time, engine data, and crash information provides invaluable insights for accident investigations and 
                enhancements in vehicle safety.

 */

#include "main.h"

// Variables for key input and menu management
unsigned char key = 0;
extern unsigned int Index; // Main menu index
extern unsigned short int star_flag; // Main menu flag

// State management
unsigned int state;
//unsigned int caseflag = 1;

// Variables for time and clock settings
unsigned char time[9];
unsigned char clock_reg[3];

/*
 * Function: init_configcar
 * ----------------------------
 * Initializes all required peripherals for the car black box system.
 */
static void init_configcar(void)
{
    init_clcd();  // Initialize the LCD display
    init_adc();   // Initialize the ADC for sensor inputs
    init_mkp();   // Initialize the matrix keypad
    init_i2c();   // Initialize the I2C communication
    init_ds1307(); // Initialize the DS1307 RTC
    init_uart();  // Initialize UART for communication
    puts("Welcome"); // Print welcome message
}

/*
 * Function: main
 * ----------------------------
 * Main function that controls the system state and transitions.
 */
void main(void)
{
    // Initialize the car black box system
    init_configcar();

    while (1) // Infinite loop to manage the state machine
    {
        // Read the key pressed on the matrix keypad
        key = read_matrix_keypad(STATE);

        // State machine to handle different functionalities
        switch (state)
        {
            case DASH_BOARD:
                // Manage the dashboard view
                dashb();
                if (key == SW4) // If key SW4 is pressed, go to main menu
                {
                    state = MAIN_MENU;
                }
                break;

            case MAIN_MENU:
                // Manage the main menu display
                mainmenu();
                if (key == SW5) // If key SW5 is pressed, return to dashboard
                {
                    star_flag = 0; // Reset flag
                    Index = 0;     // Reset menu index
                    state = DASH_BOARD;
                }
                break;

            case VIEW_LOG:
                // View logs functionality
                view_log();
                if (key == SW5) // If key SW5 is pressed, go back to main menu
                {
                    state = MAIN_MENU;
                }
                break;

            case DOWNLOAD_LOG:
                // Download logs functionality
                download_log();
                break;

            case SET_TIME:
                // Set time functionality
                set_time();
                break;
        }
    }
}
