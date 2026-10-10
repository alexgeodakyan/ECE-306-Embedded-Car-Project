/*
 * globals.h
 *
 *  Created on: Sep 22, 2025
 *      Author: ageod
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_


  // Global Variables
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;
volatile unsigned int time_count;       // variable that counts up every 100 ms. used for displaying elapsed time
volatile unsigned int time_loop;        // variable that counts up in step with time_count, but it resets
volatile char once;
volatile char once2;            // second flag to ensure an event happens once
volatile char white_set;        // TRUE if white calibration is complete
volatile char black_set;        // TRUE if black calibration is complete

volatile char state;     // state within movement state machine
volatile char previous_state;

volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_THUMB_WHEEL;

volatile unsigned int debounce_count1;
volatile unsigned int debounce_count2;

char adc_char[4];
char time_char[5];


volatile unsigned int delay_counter;
volatile unsigned int brakes_off;
volatile unsigned int left_flag;
volatile unsigned int right_flag;

volatile unsigned int DAC_data;

volatile unsigned int white_threshold_left;
volatile unsigned int white_threshold_right;
volatile unsigned int black_threshold_left;
volatile unsigned int black_threshold_right;
volatile int black_detector_difference;

volatile unsigned int adc_cycle_count;
volatile unsigned int adjustment;
volatile unsigned int overflow;
volatile signed int integral;
volatile signed int derivative;
volatile signed int errors[4];      // SAMPLE_WINDOW
volatile unsigned char new_value;   // indicates if a new value has been read from the ADC. set after second reading, reset after value is saved in errors array
volatile unsigned int cycles;   // tracks number of data points from ADC during CIRCLING

volatile char pb_index; // Index for process_buffer
char process_buffer[25]; // Size for appropriate Command Length

volatile unsigned int usb_rx_wr;
volatile unsigned int usb_rx_rd;
volatile unsigned int usb_tx_wr;
volatile unsigned int usb_tx_rd;

volatile unsigned int iot_tx;
volatile unsigned int direct_iot;
volatile unsigned int iot_rx_wr;
volatile unsigned int iot_rx_rd;
volatile char IOT_Ring_Rx[16];
char iot_TX_buf[27];
volatile char USB_Ring_Rx[16];

volatile unsigned char mode;
unsigned int display_index;
unsigned int baud;  // state variable to track baud rate
char IOT_Data[4][60];   // process buffer for IOT
char line;  // holds index for line of process buffer
char prev_line;
char character; // holds index for character of process buffer

volatile char communication_flag;
volatile char command_flag;         // high indicates that a command is actively being sent
char power_flag;        // false while board is not fully powered up
char iot_ready_flag;    // indicates whether the IOT is ready to receive a command
char ip_flag;       // true if the message is for an IP address
char ssid_flag;     // true if the message is for SSID
volatile char exit_flag;        // true if the car is exiting
char first_command;     // set true when the first command is sent
char ssid_possible;     // flag to identify whether message could pertain to ssid
char phone_message_flag;     // flag to identify whether message is sent from Magic Smoke app
volatile unsigned int delay_counter_movement;       // timer delay loop variable for iot movement
unsigned int future_time;   // variable used to set time based states
typedef struct {
    char pattern;           // 'F', 'L', 'R', 'B'
    unsigned int duration;  // movement time in ms or time counts
    char command[5];        // base command sent to iot
} MovementCommand;
unsigned int command_read;      // pointer to the command to be executed next
unsigned int command_write;     // pointer to the last position in the command queue

char ssid[11];          // holds ssid of connected network
char ip_line1[11];       // holds first 2 sets of numbers for ip
char ip_line2[11];       // holds last 2 sets of numbers for ip
char set_pin[5];    // pin used to enable messaging
MovementCommand command_queue[5];

unsigned int ip_length1;
unsigned int ip_length2;
unsigned int start_index1;
unsigned int start_index2;

char last_move;     // tracks last move during bang bang algorithm
char reverse_flag;
char white_flag;


#endif /* GLOBALS_H_ */
