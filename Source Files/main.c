//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"
#include  "ports.h"
#include "wheels.h"
#include "globals.h"
#include "Display.h"
#include "adc_math.h"
#include "dac.h"
#include "bangbang.h"
#include "serial_communication.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void StateMachine(void);
void Init_ADC(void);
void IOT_Process(void);

char stop_saving[] = "AT+SYSSTORE=0\r\n";
char multiple_connections[] = "AT+CIPMUX=1\r\n";
char set_server[] = "AT+CIPSERVER=1,8181\r\n";
char get_ssid[] = "AT+CWJAP?\r\n";
char get_ip[] = "AT+CIFSR\r\n";
char ping_google[] = "AT+PING=\"www.google.com\"\r\n";




//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
   PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers

  // power up sequence to ensure too much current isn't drawn by starting everything up at once

  while (power_flag == FALSE) {
      switch (time_count) {

      case 2:
          // display power sequence
          Init_LCD();                          // Initialize LCD
          P3OUT &= ~IOT_EN_CPU;      // set IOT_Enable low after 200 ms delay for 200 ms


          break;
      case 4:
          // IOT startup
          P3OUT |= IOT_EN_CPU;      // set IOT_Enable high again
          communication_flag = TRUE;        // allow for messages to be sent from the IOT

          break;
      case 6:
          Init_DAC();
          break;

      case 8:
          LCD_BACKLITE_DIMMING = PERCENT_50;

          break;

      case 10:
          // other startup
          Init_ADC();                          // Initialize ADC
          Init_Serial('S');                // Initialize Serial Communication for 115,200 baud

          power_flag = TRUE;
          break;

      default: break;
      }
  }

  strcpy(set_pin, "2367");

//P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
  Display_Update(0,0,0,0);

  state = STARTUP1;
  once = TRUE;
  once2 = TRUE;

  unsigned int i;





//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
//    StateMachine();            // Run a Time Based State Machine
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF


    IOT_Process();      // Save values from IOT into process buffer

    switch (time_loop) {
    case PING_INTERVAL:        // every 5s
        // ping google
        strcpy(iot_TX_buf, ping_google);
        iot_tx = 0;
        UCA0IE |=UCTXIE;
        UCA0IFG |= UCTXIFG;   // force trigger

        time_loop = 0;
        break;
    default:
        break;
    }



    //------------------------- STATE MACHINE




    switch (state) {

    case CALIBRATION:       // car enters calibration with sw1 press. this sets white and black values for BL detection
        if (once) {
            P2OUT |= IR_LED;
            once = FALSE;
            previous_state = CALIBRATION;
        }
        switch(ADC_THUMB_WHEEL) {
        case 0:
            if (!white_set && once2) {
                strcpy(display_line[0], "SET WHITE ");
                strcpy(display_line[3], "          ");
            }

            else if (once2) {
                strcpy(display_line[0], "WHITE SET");
                strcpy(display_line[3], "SW2 TO RST");
            }


            break;
        case 1:
            if (!black_set) {
                strcpy(display_line[0], "SET BLACK ");
                strcpy(display_line[3], "          ");
            }

            else if (once2) {
                strcpy(display_line[0], "BLACK SET");
                strcpy(display_line[3], "SW2 TO RST");
            }

            break;
        default:
            break;
        }

        break;
    case STARTUP1:              // startup1, 2, and 3 send necessary IOT commands for startup
        if (iot_ready_flag) {
            strcpy(iot_TX_buf, stop_saving);
            iot_tx = 0;
            UCA0IE |=UCTXIE;
            UCA0IFG |= UCTXIFG;   // force trigger
            iot_ready_flag = FALSE;
            state = STARTUP2;
        }
        break;

    case STARTUP2:
        if (iot_ready_flag) {
            strcpy(iot_TX_buf, multiple_connections);
            iot_tx = 0;
            UCA0IE |=UCTXIE;
            UCA0IFG |= UCTXIFG;   // force trigger
            iot_ready_flag = FALSE;
            state = STARTUP3;
        }
    break;

    case STARTUP3:
        if (iot_ready_flag) {
            strcpy(iot_TX_buf, set_server);
            iot_tx = 0;
            UCA0IE |=UCTXIE;
            UCA0IFG |= UCTXIFG;   // force trigger
            iot_ready_flag = FALSE;
            strcpy(display_line[1], "IP address");
            state = CHILLING;
        }
    break;

    case CHILLING:          // main idle state for the car. commands from the iot are acted on in this state
        if (once && !white_set && !black_set) {     // if calibration has not been done
            strcpy(display_line[1], "IP address");
            once = FALSE;
        }

        else if (once && (previous_state == CALIBRATION)) {        // after calibration, default text
            P2OUT &= ~IR_LED;       // turn off LED after calibration

            strcpy(display_line[0], "Waiting   ");
            strcpy(display_line[1], "for input ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");

            DisplayIPAddress(2);        // show IP address on lines 2 and 3 of the display (bottom)
            once = FALSE;
            previous_state = CHILLING;      // save state
        }

        else if (once) {
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], "          ");
            display_line[3][9] = 's';
            DisplayIPAddress(1);                // clear display, show IP on middle lines, and show timer
            once = FALSE;
        }

        if (command_read != command_write) {        // evaluate commands in the queue to switch to a move
            switch(command_queue[command_read].pattern) {
            case 'F':
                state = FORWARD_SLOW;

                break;
            case 'L':
                state = TURNING_LEFT;

                break;
            case 'R':
                state = TURNING_RIGHT;

                break;
            case 'B':
                state = REVERSE_SLOW;

                break;

            case 'A':       // autonomous mode command
                command_write = command_read + 1;  // command_write now points to the next element in command_queue. it WILL overwrite commands if more are queued after the A000 command

                if (command_write >= QUEUE_SIZE) {
                    command_write = BEGINNING;
                }

                command_queue[command_write].pattern = 'F';
                command_queue[command_write].duration = BL_INITIAL_FORWARD_DURATION;     // 4 s
                strcpy(command_queue[command_write++].command, command_queue[command_read].command); // ensure that the command displayed does not change

                if (command_write >= QUEUE_SIZE) {
                    command_write = BEGINNING;
                }

               switch(command_queue[command_read].duration) {
               case LEFT_LOOP:
                   command_queue[command_write].pattern = 'L';
                   command_queue[command_write].duration = BL_INITIAL_TURN_DURATION;
                   strcpy(command_queue[command_write++].command, command_queue[command_read].command); // ensure that the command displayed does not change

                   break;

               case RIGHT_LOOP:
                   command_queue[command_write].pattern = 'R';
                   command_queue[command_write].duration = BL_INITIAL_TURN_DURATION;
                   strcpy(command_queue[command_write++].command, command_queue[command_read].command); // ensure that the command displayed does not change

                   break;

               default:
                   break;
               }

               if (command_write >= QUEUE_SIZE) {
                   command_write = BEGINNING;
               }

               command_queue[command_write].pattern = 'I';
               command_queue[command_write].duration = 0;       // queue up command that begins BL interception
               strcpy(command_queue[command_write++].command, command_queue[command_read].command); // ensure that the command displayed does not change

               break;

            case 'P':       // pad arrived command
                strcpy(display_line[0], "Arrived 0 ");
                display_line[0][9] = command_queue[command_read].duration + 0x30;
                break;

            case 'C':       // clear display command
                strcpy(display_line[0], "          ");
                break;

            case 'I':       // initiate black line detection command
                state = BL_START;
                once = TRUE;
                break;

            case 'E':       // E000, exit/end command

                command_write = command_read + 1;
                if (command_write >= QUEUE_SIZE) {
                    command_write = BEGINNING;
                }

                command_queue[command_write].pattern = 'F';
                command_queue[command_write].duration = BL_EXIT_FORWARD_DURATION;     // 5 s
                strcpy(command_queue[command_write++].command, command_queue[command_read].command); // ensure that the command displayed does not change

                strcpy(display_line[0], " BL Exit  ");
                exit_flag = TRUE;

            default:
                break;
            }


            strcpy(display_line[3], command_queue[command_read].command);       // show command on display


            for (i = 4; i < 7; i++) {
                display_line[3][i] = ' ';           // fill the rest of the display line with spaces
            }


            while (command_queue[command_read].duration > 200) {
                delay_counter_movement++;
                command_queue[command_read].duration -= 200;
            }

            TB2CCTL2 &= ~CCIFG;
            TB2CCR2 = TB2R + (command_queue[command_read++].duration * MOVEMENT_UNIT_INTERVAL);
            TB2CCTL2 |= CCIE;       // set up movement timer for given time length

            if (command_read >= QUEUE_SIZE) {
                command_read = BEGINNING;
            }
            once = TRUE;

        }
        break;

    case PUSHING_IP:        // send get_ip command to iot and display IP address
        if (iot_ready_flag) {
            strcpy(iot_TX_buf, get_ip);
            iot_tx = 0;
            UCA0IE |=UCTXIE;
            UCA0IFG |= UCTXIFG;   // force trigger
            iot_ready_flag = FALSE;
            state = PUSHING_SSID;
        }
        break;

    case PUSHING_SSID:      // same for ssid
        if (iot_ready_flag) {
            strcpy(iot_TX_buf, get_ssid);
            iot_tx = 0;
            UCA0IE |=UCTXIE;
            UCA0IFG |= UCTXIFG;   // force trigger
            iot_ready_flag = FALSE;
            state = CHILLING;
        }

        break;

    case FORWARD_SLOW:
        if (once) {
            Forward_On_Slow();
            once = FALSE;
        }
        break;

    case TURNING_LEFT:
        if (once) {
            Turn_CCW_Slow();
            once = FALSE;
        }
        break;

    case TURNING_RIGHT:
        if (once) {
            Turn_CW_Slow();
            once = FALSE;
        }
        break;

    case REVERSE_SLOW:
        if (once) {
            Reverse_On_Slow();
            once = FALSE;
        }
        break;

    case FORWARD_OFF:
        Forward_Off();
        state = DELAYING;
        break;

    case TURNING_LEFT_OFF:
        Turn_CCW_Off();
        state = DELAYING;

        break;
    case TURNING_RIGHT_OFF:
        Turn_CW_Off();
        state = DELAYING;

        break;
    case REVERSE_OFF:
        Reverse_Off();
        state = DELAYING;

        break;
    case DELAYING:      // there is a safety delay after each movement to protect the FETs
        if (once) {
            // start delay timer, delay timer sets state back to CHILLING
            future_time = time_count + SAFETY_DELAY;
            once = FALSE;
            }

        if (time_count >= future_time) {

            switch (exit_flag) {
            case FALSE:
                state = CHILLING;
                break;
            case TRUE:
                state = END;
                break;
            default:
                break;
            }

            future_time = 0;
            once = TRUE;
        }

        break;

    case BL_START:
        if (once) {
            P2OUT |= IR_LED;        // turn on IR emitter
            strcpy(display_line[0], " BL Start ");
            future_time = time_count + DISPLAY_DELAY;
            once = FALSE;
        }

        if (time_count >= future_time) {
            state = INTERCEPTING;
            once = TRUE;
        }
        break;

    case INTERCEPTING:
        if (once) {
            Forward_On_Slow();
            previous_state = state;     // save current state
            once = FALSE;
        }

        if ((ADC_Left_Detect <= white_threshold_left) && (ADC_Right_Detect <= white_threshold_right)) {
            white_flag = TRUE;
        }
        if ((ADC_Left_Detect >= black_threshold_left) && (ADC_Right_Detect >= black_threshold_right) && white_flag) {      // if both detectors see black
            Forward_Off();
            state = BRAKING;
            once = TRUE;

            if (ADC_Left_Detect > ADC_Right_Detect + black_detector_difference) {   // compares detector values while accounting for natural differences due to physical setup
                right_flag = TRUE;
            }
            else {      // right has a greater value
                left_flag = TRUE;
            }

        }

        break;

    case BRAKING:
        if (once) {
            Brake();
            once = FALSE;
        }
        break;

    case STOP_BRAKING:

        switch(previous_state) {
        case INTERCEPTING:
            Reverse_Off();
            state = FOUND;
            once = TRUE;
            break;

        case TURNING:
            if (right_flag) {
                Turn_CCW_Off();
            }
            else {
                Turn_CW_Off();
            }

            state = STOPPED;
            once = TRUE;

        }

        break;

        case FOUND:
            if (once) {
                strcpy(display_line[0], "Intercept ");
                future_time = time_count + DISPLAY_DELAY;
                once = FALSE;
            }

            if (time_count >= future_time) {
                state = TURNING;
                once = TRUE;
            }

            break;

        case TURNING:
            if (once) {
                strcpy(display_line[0], " BL Turn  ");

                if (right_flag) {
                    Turn_CW_Slow();
                }

                else if (left_flag) {
                    Turn_CCW_Slow();            // turn the car to the point where it's on top of the black line, with direction based on which emitter saw black first
                }

                once = FALSE;
            }

            if (((left_flag) && (ADC_Right_Detect <= black_threshold_right - TURNING_ADJUSTMENT)) || ((right_flag) && (ADC_Left_Detect <= black_threshold_left - TURNING_ADJUSTMENT))) {
                // turned to the point where a detector sees black again
                once = TRUE;
                previous_state = state;     // save current state

                if(right_flag) {
                    Turn_CW_Off();
                }

                else if(left_flag) {
                    Turn_CCW_Off();
                }

                state = BRAKING;
                once = TRUE;
            }

            break;

        case STOPPED:       // delay to show state on display

            if (once) {
                future_time = time_count + DISPLAY_DELAY;
                once = FALSE;
            }

            if (time_count >= future_time) {
                state = TRAVELING;
                once = TRUE;
            }

            break;

        case TRAVELING:

            if (once) {
                     strcpy(display_line[0], "BL Travel ");
                     Forward_On_Slower();
                     once = FALSE;
                     previous_state = state;

                     future_time = time_count + CIRCLE_DELAY;       // show BL Travel on display initially, switch to BL CIRCLE after time-based delay
                 }

            if (once2 && (time_count >= future_time)) {
                strcpy(display_line[0], "BL Circle ");
                once2 = FALSE;
            }

            bangbang();

            if (command_read != command_write) {        // if a new command is received during the TRAVELING state, turn all wheels off and return to command processing state
                Forward_Off();
                Reverse_Off();
                state = CHILLING;                   // allow interrupts from IOT to receive exit command
            }

             break;

        case END:
            if (once) {
                strcpy(display_line[0], " BL Stop  ");
                strcpy(display_line[1], "  COURSE  ");
                strcpy(display_line[2], " COMPLETE ");
                display_line[3][0] = 'T';
                display_line[3][1] = 'i';
                display_line[3][2] = 'm';
                display_line[3][3] = 'e';
                display_line[3][4] = ':';
            }           // final display text includes message and time


    default:
        break;
    }




    //-------------------------



    Check_Wheels();     // check wheels to ensure that forward and reverse movement aren't enabled at the same time

  }
//------------------------------------------------------------------------------

}


