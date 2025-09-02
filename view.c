#include "main.h"

// External variables
extern unsigned int Index; // Main menu index
extern unsigned short int star_flag; // Main menu flag
extern unsigned int shift_index;
#define max_event_count 10 // Maximum number of events in the log
extern unsigned char time[9]; // Stores the current time as a string
extern unsigned int over_flag; // Flag for circular log overflow
extern unsigned int state; // Current state of the application
extern unsigned char key; // Key input from the matrix keypad
extern unsigned int event_count; // Number of logged events

// Global variables
unsigned int log_index = 0, delay, flag1, hour = 0, min = 0, sec = 0, delay1 = 0, index_val;
unsigned char ch;
unsigned int view_addr = 0x00; // Address for viewing logs
char Log[max_event_count][15]; // Circular buffer to store logs
unsigned int address = 0x00, icount;

/*
 * Function: view_log
 * ----------------------------
 * Displays stored logs in a scrollable format.
 * Allows the user to navigate between logs or return to the main menu.
 */
void view_log(void) 
{
    if (event_count == 0) // No events to display
    {
        state = MAIN_MENU;
        clcd_print("NO EVENTS STORED", LINE1(0));
        clcd_print("   TO DISPLAY   ", LINE2(0));
        for (unsigned long wait = 500000; wait--; ); // Wait for a brief period
    } 
    else 
    {
        event_reader(); // Read events from EEPROM

        if (over_flag == 1) 
        {
            log_index = event_count % 10; // Adjust for circular buffer overflow
        }

        clcd_print("               ", LINE2(0)); // Clear LINE2

        // Display log details
        clcd_print("# TIME    EV SP", LINE1(0)); // Column titles
        if (key == SW1 && log_index > 0) 
        {
            log_index--; // Scroll up
        } 
        else if (key == SW2 && log_index < event_count - 1) 
        {
            log_index++; // Scroll down
        } 
        else if (key == SW5) 
        {
            log_index = 0;
            state = MAIN_MENU; // Return to main menu
            CLEAR_DISP_SCREEN;
            return;
        }

        clcd_putch(log_index + '0', LINE2(0)); // Display log index
        clcd_putch(' ', LINE2(1));
        clcd_print(Log[log_index], LINE2(2)); // Display the current log
    }
}

/*
 * Function: event_reader
 * ----------------------------
 * Reads stored logs from EEPROM into the circular buffer for display.
 */
void event_reader(void) 
{
    address = 0;

    for (int i = 0; i < event_count; i++) 
    {
        int current_event = (shift_index + i) % 10;
        int view_addr = current_event * 10;

        for (int j = 0; j < 15; j++) 
        {
            if (j == 2 || j == 5) 
            {
                Log[i][j] = ':'; // Insert ':' at appropriate positions
            } 
            else if (j == 8 || j == 11) 
            {
                Log[i][j] = ' '; // Insert space
            } 
            else if (j == 14) 
            {
                Log[i][j] = '\0'; // Null-terminate the string
            } 
            else 
            {
                Log[i][j] = read_external_eeprom(view_addr++); // Read data from EEPROM
            }
        }
    }
}

/*
 * Function: clear_log
 * ----------------------------
 * Clears all logs stored in memory and displays appropriate messages.
 */
void clear_log(void)
{
    if (event_count == 0) // No logs to clear
    {
        clcd_print("NO LOGS STORED! ", LINE1(0));
        clcd_print("    TO CLEAR    ", LINE2(0));
        for (unsigned long int wait = 500000; wait--; );
    }
    else
    {
        event_count = 0; // Reset log count
        clcd_print("CLEARING LOGS...", LINE1(0));
        clcd_print("                ", LINE2(0));
        for (unsigned long int wait = 500000; wait--; );
        clcd_print("    CLEARED!    ", LINE2(0));
        for (unsigned long int wait = 400000; wait--; );
        star_flag = 0;
    }
}

/*
 * Function: download_log
 * ----------------------------
 * Downloads stored logs to a serial terminal (e.g., Tera Term) for viewing.
 */
void download_log(void) 
{
    if (event_count == 0) // No logs to download
    {
        clcd_print("NO LOGS STORED! ", LINE1(0));
        clcd_print("  TO DOWNLOAD   ", LINE2(0));
        for (unsigned long int wait = 500000; wait--; );
    }
    else
    {
        event_reader(); // Fetch logs from EEPROM
        clcd_print("LOGS DOWNLOADED ", LINE1(0));
        clcd_print("  SUCCESSFULLY  ", LINE2(0));
        for (unsigned long int wait = 500000; wait--; );

        // Display logs on serial terminal
        puts("LOG   TIME   E  SP\n\r");
        for (int j = 0; j < event_count; j++)
        {
            putch(j+48);
            puts("   ");
            puts(Log[j]);
            // puts(Log[j]);
            puts("\n\r");
        }
    }
    CLEAR_DISP_SCREEN;
    state = MAIN_MENU; // Return to main menu
}

/*
 * Function: set_time
 * ----------------------------
 * Allows the user to set the current time (HH:MM:SS) using the keypad.
 */
void set_time(void) 
{
    clcd_print("    HH:MM:SS    ", LINE1(0)); // Display set time title
    clcd_print("    ", LINE2(0)); // Spaces before the timer
    clcd_print("    ", LINE2(12)); // Spaces after the timer

    if (flag1 == 0) // Extract time and convert to numeric values
    {
        hour = ((time[0] - '0') * 10) + (time[1] - '0');
        min = ((time[3] - '0') * 10) + (time[4] - '0');
        sec = ((time[6] - '0') * 10) + (time[7] - '0');
        flag1++;
    }

    if (key == SW1) // Increment time fields
    {
        if (flag1 == 1) // Increment hours
        {
            if (++hour == 24) 
                hour = 0;
        } 
        else if (flag1 == 2) // Increment minutes
        {
            if (++min == 60) 
                min = 0;
        } 
        else if (flag1 == 3) // Increment seconds
        {
            if (++sec == 60) 
                sec = 0;
        }
    }

    // Handle blinking for selected field
    if (flag1 == 1 || flag1 == 2 || flag1 == 3) 
    {
        if (delay1++ <= 500) 
        {
            print_clcd();
        } 
        else if (delay1++ <= 1000) 
        {
            clcd_print("  ", LINE2(4 + (flag1 - 1) * 3));
        } 
        else 
        {
            delay1 = 0;
        }
    }

    if (key == SW2) // Switch to next field
    {
        if (++flag1 == 4) 
            flag1 = 1;
    }

    if (key == SW4) // Set time and return to dashboard
    {
        write_ds1307(HOUR_ADDR, (((hour / 10) << 4) | (hour % 10)));
        write_ds1307(MIN_ADDR, (((min / 10) << 4) | (min % 10)));
        write_ds1307(SEC_ADDR, (((sec / 10) << 4) | (sec % 10)));
        star_flag = 0;
        Index = 0;
        state = DASH_BOARD;
    }

    if (key == SW5) // Return to main menu without setting time
    {
        star_flag = 0;
        Index = 0;
        state = MAIN_MENU;
    }
}

/*
 * Function: print_clcd
 * ----------------------------
 * Displays the time being set on the CLCD.
 */
void print_clcd(void) 
{
    clcd_putch(hour / 10 + '0', LINE2(4));
    clcd_putch(hour % 10 + '0', LINE2(5));
    clcd_putch(':', LINE2(6));
    clcd_putch(min / 10 + '0', LINE2(7));
    clcd_putch(min % 10 + '0', LINE2(8));
    clcd_putch(':', LINE2(9));
    clcd_putch(sec / 10 + '0', LINE2(10));
    clcd_putch(sec % 10 + '0', LINE2(11));
}
