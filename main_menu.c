#include "main.h"

// External variables declared in other modules
extern unsigned char key;          // Key input from the keypad
unsigned int Index = 0;            // Menu index for tracking menu options
char events_opt[4][20] = {         // Array of menu options
    "VIEW LOG        ",            // Option 0: View log
    "CLEAR LOG       ",            // Option 1: Clear log
    "DOWNLOAD LOG    ",            // Option 2: Download log
    "SET TIME        "             // Option 3: Set time
};

#define MENU_COUNT 2               // Number of menu options to display at a time
unsigned short int star_flag = 0;  // Tracks whether the focus is on the first or second line of the menu

// External variables related to system state and functionality
extern unsigned int state;         // Current state of the system
extern unsigned int flag1;         // Flag for managing specific actions

// Main menu handler function
void mainmenu(void)
{
    // Display the star on the appropriate line (LINE1 or LINE2) to indicate focus
    if (star_flag == 0) 
    {
        clcd_putch('*', LINE1(0)); // Star on first line
        clcd_putch(' ', LINE2(0)); // Clear star on the second line
    } 
    else 
    {
        clcd_print(" ", LINE1(0)); // Clear star on the first line
        clcd_putch('*', LINE2(0)); // Star on the second line
    }

    // Print the menu options on the display
    clcd_print(events_opt[Index], LINE1(1));     // Current menu option on the first line
    clcd_print(events_opt[Index + 1], LINE2(1)); // Next menu option on the second line

    // Handle key presses for navigation
    if (key != ALL_RELEASED) 
    {
        // If SW2 is pressed, move the focus down
        if (key == SW2) 
        {
            if (star_flag == 0) 
            {
                star_flag = 1; // Move focus to the second line
            } 
            else if (Index < MENU_COUNT) 
            {
                Index++;       // Scroll down to the next menu options
            }
        }

        // If SW1 is pressed, move the focus up
        if (key == SW1) 
        {
            if (star_flag == 1) 
            {
                star_flag = 0; // Move focus to the first line
            } 
            else if (Index > 0) 
            {
                Index--;       // Scroll up to the previous menu options
            }
        }

        // If SW4 is pressed, select the current option
        if (key == SW4) 
        {
            // Determine the selected menu option based on the focus and index
            switch (star_flag + Index) 
            {
                case 0:
                    // View log function
                    state = VIEW_LOG;
                    break;

                case 1:
                    // Clear log function
                    clear_log();
                    break;

                case 2:
                    // Download log function
                    state = DOWNLOAD_LOG;
                    break;

                case 3:
                    // Set time function
                    state = SET_TIME;
                    break;

                default:
                    // Handle invalid options (should not occur)
                    break;
            }
        }
    }
}
