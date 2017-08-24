/*
 * Program to test rotation
 */

// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.
#include <pololu/3pi.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

const char app_banner1[] PROGMEM = " Basics ";
const char app_banner2[] PROGMEM = "Movement";

void welcome_proc()
{
    clear();
    print_from_program_space(app_banner1);
    lcd_goto_xy(0,1);
    print_from_program_space(app_banner2);
    delay_ms(4000);
}

void display_battery() {
    int bat = read_battery_millivolts();

    clear();
    print_long(bat);
    print("mV");
    lcd_goto_xy(0,1);
    print("Press B");

    delay_ms(100);
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
    // This must be called at the beginning of 3pi code, to set up the
    // sensors.  We use a value of 2000 for the timeout, which
    // corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
    pololu_3pi_init(2000);
    
    welcome_proc();

    // Display battery voltage and wait for button press
    while(!button_is_pressed(BUTTON_B))
    {
        display_battery();
    }

    // Always wait for the button to be released so that 3pi doesn't
    // start moving until your hand is away from it.
    wait_for_button_release(BUTTON_B);
    delay_ms(1000);

    set_motors(0, 0);

}

static void get_int(int minval, int maxval, int step, int * p_val) {
    int val = (maxval + minval) / 2;
    if (*p_val >= minval && *p_val <= maxval) {
        val = *p_val;
    }
    lcd_goto_xy(0, 1);
    print_character('v');
    lcd_goto_xy(7, 1);
    print_character('^');
    while(1) {
        lcd_goto_xy(1, 1);
        print("      ");
        lcd_goto_xy(2, 1);
        print_long(val);

        unsigned char button = 0;
        while(!(button = get_single_debounced_button_press(ANY_BUTTON)))
            ;
        while(!get_single_debounced_button_release(button))
            ;
        if (button == BUTTON_A) {
            if (minval + step <= val) {
                val -= step;
            }
        }
        else if (button == BUTTON_C) {
            if (maxval - step >= val) {
                val += step;
            }
        }
        else if (button == BUTTON_B) {
            break;
        }
    }
    *p_val = val;
}

static void get_m1(int * p_m1) {
    clear();
    print("   M1   ");
    get_int(-100, 100, 2, p_m1);
}

static void get_m2(int * p_m2) {
    clear();
    print("   M2   ");
    get_int(-100, 100, 2, p_m2);
}

static void get_delay(int *p_delay) {
    clear();
    print(" DELAY  ");
    get_int(100, 3000, 1, p_delay);
}

int main()
{
    int a_m1 = 100;
    int a_m2 = -100;
    int a_delay = 314;

    // set up the 3pi
    initialize();

    while(1) {
        get_m1(&a_m1);
        get_m2(&a_m2);
        get_delay(&a_delay);

        clear();
        print(">> B <<");
        wait_for_button(BUTTON_B);

        set_motors(a_m1, a_m2);
        delay_ms(a_delay);
        set_motors(0, 0);    
    }
    
    
    while(1) {
        delay_ms(1000);
    }
}
