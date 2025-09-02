#include "main.h"

// External variables
extern unsigned char time[9]; // Stores the current time as a string
unsigned char clock_reg[3];   // Stores raw clock register data

// Global variables
extern unsigned char key; // Key input from the matrix keypad
unsigned int event_index = 0, event_count, over_flag, shift_index;
unsigned int flag = 0; // Flag for event selection
unsigned short adc_reg_val; // ADC register value for speed calculation
unsigned char speed; // Current speed
char event[8][3] = {"ON", "GN", "GR", "G1", "G2", "G3", "G4", "C "}; // Event types
unsigned char arr[10];
unsigned char ch;
unsigned int address = 0x00; // EEPROM address for storing events

/*
 * Function: dashb
 * ----------------------------
 * Displays the dashboard with time, event, and speed.
 */
void dashb(void)
{
    get_time(); // Fetch the current time from the RTC

    // Display the dashboard layout
    clcd_print("  Time   EV   SP", LINE1(0));

    // Read the speed from ADC (Channel 4) and calculate 0-99 speed range
    adc_reg_val = read_adc(CHANNEL4);
    speed = (adc_reg_val / 10.33); // Convert ADC value to speed
    clcd_putch((speed / 10) + 48, LINE2(14)); // Display tens place of speed
    clcd_putch((speed % 10) + 48, LINE2(15)); // Display units place of speed

    // Handle key inputs
    if (key != ALL_RELEASED)
    {
        if (key == SW1) // Key SW1 triggers event "C "
        {
            event_index = 7;
            flag = 1;
            write_event(); // Log the event
        }
        else if (key == SW2) // Key SW2 increments the event index
        {
            if (event_index < 6 && flag == 0)
            {
                event_index++; // Increment event index
            }
            else if (flag == 1)
            {
                event_index = 1; // Reset event index
                flag = 0;
            }
            write_event(); // Log the event
        }
        else if (key == SW3) // Key SW3 decrements the event index
        {
            if (event_index > 1 && flag == 0)
            {
                event_index--; // Decrement event index
            }
            else if (flag == 1)
            {
                event_index = 1; // Reset event index
                flag = 0;
            }
            write_event(); // Log the event
        }
    }

    // Display the current event
    clcd_print(event[event_index], LINE2(9));
    clcd_print(" ", LINE2(8));
    clcd_print("  ", LINE2(11));

    // Display the current time on the dashboard
    display_time();
}

/*
 * Function: write_event
 * ----------------------------
 * Logs the current event with time and speed to the EEPROM.
 */
void write_event(void)
{
    if (address >= 100) // Check if the maximum EEPROM storage is reached
    {
        address = 0; // Reset EEPROM address
        over_flag = 1; // Set overflow flag
    }

    // Write time, event, and speed to the EEPROM
    write_external_eeprom(address++, time[0]);
    write_external_eeprom(address++, time[1]);
    write_external_eeprom(address++, time[3]);
    write_external_eeprom(address++, time[4]);
    write_external_eeprom(address++, time[6]);
    write_external_eeprom(address++, time[7]);
    write_external_eeprom(address++, event[event_index][0]);
    write_external_eeprom(address++, event[event_index][1]);
    write_external_eeprom(address++, (speed / 10) + 48);
    write_external_eeprom(address++, (speed % 10) + 48);

    if (event_count < 10) // Maintain a circular buffer of 10 events
    {
        event_count++;
    }
    else
    {
        shift_index = (shift_index + 1) % 10; // Shift index for overwriting
    }
}

/*
 * Function: get_time
 * ----------------------------
 * Fetches the current time from the RTC (DS1307) and formats it as a string.
 */
static void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // Read hours
    clock_reg[1] = read_ds1307(MIN_ADDR); // Read minutes
    clock_reg[2] = read_ds1307(SEC_ADDR); // Read seconds

    // Convert the raw RTC data into human-readable format
    if (clock_reg[0] & 0x40) // 12-hour format
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    else // 24-hour format
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0'; // Null-terminate the string
}

/*
 * Function: display_time
 * ----------------------------
 * Displays the current time on the CLCD.
 */
void display_time(void)
{
    clcd_print(time, LINE2(0)); // Print time on line 2 of the display
}
