/*#include <xc.h>
#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "ds1307.h"
#include "ext_eeprom.h"
#include "uart.h"
#include "i2c.h"

#define max_event_count 10

extern State_t state;

extern unsigned char key, reg_add;
unsigned int index = 0, log_index = 0, delay = 0, delay1 = 0, delay2 = 0, flag, flag1, hour = 0, min = 0, sec = 0;
unsigned char reg_address = 0;
char menu[4][14] = {max" VIEW LOG     ", " DOWNLOAD LOG ", " CLEAR LOG    ", " SET TIME     "};
char Log[max_event_count][15];
extern unsigned int max_events;
extern unsigned char time[9];

//display_main_menu function
void display_main_menu(void) {
    if (key == MK_SW12 && index < 3) //if swirch12 pressed 
    {
        index++; //increment index to scroll down
    }
    if (key == MK_SW11 && index > 0)//if switch11 pressed
    {
        index--; //decrement index to scroll up
    }

    if (index == 0) //if index value is not changed 
    {
        clcd_print("->", LINE1(0)); //display pointer at LINE1
        clcd_print(menu[0], LINE1(2)); //display option

        clcd_print("  ", LINE2(0)); //display space at LINE2
        clcd_print(menu[1], LINE2(2)); //display option
    } else //else index changed or pointer scrolled
    {
        clcd_print("  ", LINE1(0)); //display space at LINE1
        clcd_print(menu[index - 1], LINE1(2)); //display option
        clcd_print("->", LINE2(0)); //display pointer at LINE2
        clcd_print(menu[index], LINE2(2)); //display option
    }

    if (key == MK_SW2) //switch2 is pressed
    {
        state = e_dashboard; //go back to dashboard
        CLEAR_DISP_SCREEN;
    } else if (key == MK_SW1 && index == 0) //switch 1 is pressed at index 0
    {
        state = e_view_log; //go to view logs
        CLEAR_DISP_SCREEN;
    } else if (key == MK_SW1 && index == 1) //switch 1 is pressed at index 1
    {
        state = e_download_log; //select download logs
    } else if (key == MK_SW1 && index == 2) //switch 1 is pressed at index 2
    {
        state = e_clear_log; //select clear logs
    } else if (key == MK_SW1 && index == 3) //switch 1 is pressed at index 3
    {
        state = e_set_time; //go to set time
    }
}

//event_reader function
void event_reader(void) {
    reg_address = 0;
    //reading every log data from external_EEPROM
    for (int i = 0; i < max_events; i++) {
        for (int j = 0; j < 15; j++) {
            if (j == 8 || j == 11) {
                Log[i][j] = ' ';
            } else if (j == 14) {
                Log[i][j] = '\0';
            } else {
                Log[i][j] = read_eeprom(reg_address++);
            }
        }
    }
}

//view log function
void view_log(void) {
    clcd_print("L   TIME   E  S ", LINE1(0)); //display titles 
    event_reader(); //call event_reader function

    if (key == MK_SW12 && log_index < max_events - 1) //switch 12 for scroll down
    {
        log_index++; //increment log_index 
    }
    if (key == MK_SW11 && log_index > 0) //switch11 for scroll up
    {
        log_index--; //decrement log_index 
    }
    //if log_index exceeds reset to 0
    if (log_index > 9) {
        log_index = 0;
    }
    //display logs in LINE2
    clcd_putch(log_index + '0', LINE2(0)); //convert into character 
    clcd_print(Log[log_index], LINE2(2));

    if (key == MK_SW2) //if switch2 is pressed
    {
        CLEAR_DISP_SCREEN;
        state = e_main_menu; //go back to main_menu
    }
}

//download_log function
void download_log(void) {
    event_reader(); //call event_reader
    //display message
    clcd_print("LOGS DOWNLOADED ", LINE1(0));
    clcd_print(" SUCCESSFULLY   ", LINE2(0));

    while (delay++ == 100) //non blocking delay
    {
        delay = 0;
        state = e_main_menu; //setting to go to main_menu 
        CLEAR_DISP_SCREEN;
        //displaying logs on tera_term
        puts("LOG   TIME   E  SP\n\r");
        for (int i = 0; i < max_events; i++) {
            if (i + 1 >= 10) //if count is 10 
            {
                putch(' ');
                puts("10");
                puts(" ");
                puts(Log[i]);
                puts("\n\r");
            } else //count upto 9
            {
                putch(' ');
                putch((i + 1) + '0');
                puts("  ");
                puts(Log[i]);
                puts("\n\r");
            }
        }
    }
}

//clear_log function
void clear_log(void) {
    //dummy read function to increase delay 
    event_reader();
    //display message 
    clcd_print("  LOGS CLEARED  ", LINE1(0));
    clcd_print("  SUCCESSFULLY  ", LINE2(0));

    while (delay2++ == 100) //non blocking delay
    {
        delay2 = 0;
        state = e_main_menu; //setting to go to main_menu
        CLEAR_DISP_SCREEN;
        //clearing every data replacing with spaces
        for (int i = 0; i < max_events; i++) {
            for (int j = 0; j < 15; j++) {
                if (j == 8 || j == 11) {
                    Log[i][j] = ' ';
                } else if (j == 14) {
                    Log[i][j] = '\0';
                } else {
                    Log[i][j] = ' ';
                }
            }
        }
        max_events = 0; //reset max_events to 0
    }
}

//print_clcd function
void print_clcd(void) {
    //display hour value
    clcd_putch(hour / 10 + '0', LINE2(4));
    clcd_putch(hour % 10 + '0', LINE2(5));
    clcd_putch(':', LINE2(6));
    //display min value
    clcd_putch(min / 10 + '0', LINE2(7));
    clcd_putch(min % 10 + '0', LINE2(8));
    clcd_putch(':', LINE2(9));
    //display sec value
    clcd_putch(sec / 10 + '0', LINE2(10));
    clcd_putch(sec % 10 + '0', LINE2(11));
}

//set_time function 
void set_time(void) {
    clcd_print("    HH:MM:SS    ", LINE1(0)); //display set time title
    //spaces before and after timer
    clcd_print("    ", LINE2(0));
    clcd_print("    ", LINE2(12));
    if (flag1 == 0) //extract the time and convert into numeric values
    {
        hour = ((time[0] - '0')*10) + (time[1] - '0');
        min = ((time[3] - '0')*10) + (time[4] - '0');
        sec = ((time[6] - '0')*10) + (time[7] - '0');
        flag1++;
    }
    if (key == MK_SW11) //switch11 to increment timer
    {
        if (flag1 == 1) { // Increment hours
            if (++hour == 24) {
                hour = 0;
            }
        } else if (flag1 == 2) { // Increment minutes
            if (++min == 60) {
                min = 0;
            }
        } else if (flag1 == 3) { // Increment seconds
            if (++sec == 60) {
                sec = 0;
            }
        }
    }
    if (flag1 == 1) {
        // blinking hour field
        if (delay1++ <= 500) {
            print_clcd();
        } else if (delay1++ <= 1000) {
            clcd_print("  ", LINE2(4));
        } else {
            delay1 = 0;
        }
    }
    if (flag1 == 2) {
        // blinking min field
        if (delay1++ <= 500) {
            print_clcd();
        } else if (delay1++ <= 1000) {
            clcd_print("  ", LINE2(7));
        } else {
            delay1 = 0;
        }
    }
    if (flag1 == 3) {
        // blinking sec field
        if (delay1++ <= 500) {
            print_clcd();
        } else if (delay1++ <= 1000) {
            clcd_print("  ", LINE2(10));
        } else {
            delay1 = 0;
        }
    }

    if (key == MK_SW12) //switch12 to change field
    {
        if (++flag1 == 4) //if flag1 exceeds 
        {
            flag1 = 1; //reset flag1 to 1
        }
    }
    if (key == MK_SW1) //switch1 for setting the time
    {
        write_ds1307(HOUR_ADDR, (((hour / 10) << 4) | (hour % 10)));
        write_ds1307(MIN_ADDR, (((min / 10) << 4) | (min % 10)));
        write_ds1307(SEC_ADDR, (((sec / 10) << 4) | (sec % 10)));

        state = e_main_menu; //goto main menu after resetting
    }
    if (key == MK_SW2) //switch2 to go back to main_menu 
    {
        state = e_main_menu;
    }
}
 * */