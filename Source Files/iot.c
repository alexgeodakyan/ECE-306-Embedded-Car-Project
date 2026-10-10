/*
 * iot.c
 *
 *  Created on: Nov 27, 2025
 *      Author: ageod
 */


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

void IOT_Process(void){ // Process IOT messages
    unsigned int i;
    unsigned int j;
    unsigned int period_counter;
    period_counter = 0;

    unsigned int ssid_length;

    unsigned int start_index3;

    char valid_command;     // flag used to discriminate between valid and invalid commands

    unsigned int iot_rx_wr_temp;
    iot_rx_wr_temp = iot_rx_wr;     // save current index

    if(iot_rx_wr_temp != iot_rx_rd){ // Determine if IOT is available
        IOT_Data[line][character] = IOT_Ring_Rx[iot_rx_rd++];   // write current bit to process buffer and increment
        if(iot_rx_rd >= sizeof(IOT_Ring_Rx)){
            iot_rx_rd = BEGINNING;      // reset ring buffer index if above limit
        }

        // INPUT PARSING------------

        switch(character) {       // switch-case statement based on index of IOT message. checks for a character given a case
        case 1:
            if (IOT_Data[line][character] == 'K') {     // looks for the 'K' in "OK"
                iot_ready_flag = TRUE;
            }
            break;

        case 2:
            if(IOT_Data[line][character] == 'W') {      // looks for 'W' in "+CWJAP..."
                ssid_possible = TRUE;
            }
            break;

        case 3:
            if (IOT_Data[line][character] == 'D') {     // looks for 'D' in "+IPD..."
                phone_message_flag = TRUE;
            }
            break;

        case 4:
            if(IOT_Data[line][character] == 'y') {      // looks for the 'y' in "ready"
                iot_ready_flag = TRUE;
            }

            break;

        case 5:
            if(IOT_Data[line][character] == 'G') {      // looks for the 'G' in "WIFI GOT IP"
                state = PUSHING_IP;
            }

            if((IOT_Data[line][character] == 'P') && (ssid_possible)) {      // looks for the 'P' in "+CWJAP..."
                ssid_flag = TRUE;
            }

            break;

        case 11:
            if(IOT_Data[line][character] == 'P') {      // looks for the second 'P' in "+CIFSR:APIP..." (IP address indicator)
                ip_flag = TRUE;
            }

            break;



        default: break;
        }


        //----------------



        if(IOT_Data[line][character++] == 0x0A){      // if read character is linefeed
            IOT_Data[line][character] = 0x00;       // append null terminator to string

            // whole input received, input processing-----

            if (ip_flag) {
                i = IP_START;
                ip_length1 = 0;
                ip_length2 = 0;

                while (period_counter < 2) {        // reads the ip address until the appearance of the second period, store into first ip line
                    ip_line1[i - IP_START] = IOT_Data[line][i];
                    ip_length1++;
                    if (IOT_Data[line][i++] == '.') {
                        period_counter++;
                    }
                }
                while (IOT_Data[line][i] != '"') {      // read until the appearance of a quote, which indicates the end of the ip address
                    ip_line2[i - (IP_START + ip_length1 + 1)] = IOT_Data[line][i++];
                    ip_length2++;
                }

                DisplayIPAddress(2);

                ip_flag = FALSE;        // done getting IP
            }

            if (ssid_flag) {
                i = SSID_START;
                ssid_length = 0;

                while ((IOT_Data[line][i] != '"') && (i < SSID_START + DISPLAY_SIZE)) {
                    ssid[i - SSID_START] = IOT_Data[line][i];
                    i++;
                    ssid_length++;
                }

                start_index3 = ((DISPLAY_SIZE - ssid_length) / 2);

                for (i = 0; i < ssid_length; i++) {
                    display_line[0][start_index3 + i] = ssid[i];
                }

                ssid_possible = FALSE;
                ssid_flag = FALSE;      // done getting SSID
            }


            if(phone_message_flag) {        // valid phone command follows a format of /<PIN[4]><Movement_Character[1]><Movement_Time[3]>
                i = MESSAGE_START;
                char received_pin[5];
                if (IOT_Data[line][i] == '/') {     // '/' indicates the start of a command

                    for (i = MESSAGE_START + 1; i < MESSAGE_START + sizeof(received_pin); i++) {
                        received_pin[i - (MESSAGE_START + 1)] = IOT_Data[line][i];
                    }

                    command_queue[command_write].command[0] = IOT_Data[line][i];

                    if (!strcmp(set_pin, received_pin)) {       // if true, the PIN is correct.

                        switch(IOT_Data[line][i++]) {     // evaluates movement character
                        case 'F':
                            command_queue[command_write].pattern = 'F';
                            valid_command = TRUE;
                            break;
                        case 'L':
                            command_queue[command_write].pattern = 'L';
                            valid_command = TRUE;
                            break;
                        case 'R':
                            command_queue[command_write].pattern = 'R';
                            valid_command = TRUE;
                            break;
                        case 'B':
                            command_queue[command_write].pattern = 'B';
                            valid_command = TRUE;
                            break;

                        case 'A':       // automatic (black line detection)
                            command_queue[command_write].pattern = 'A';
                            valid_command = TRUE;
                            break;

                        case 'P':       // pad (display "arrived..." message), command will take the format P00X
                            command_queue[command_write].pattern = 'P';
                            valid_command = TRUE;
                            break;

                        case 'C':       // clear message line
                            command_queue[command_write].pattern = 'C';
                            valid_command = TRUE;
                            break;

                        case 'E':
                            command_queue[command_write].pattern = 'E';
                            valid_command = TRUE;
                            break;

                        default:
                            valid_command = FALSE;
                            break;
                        }

                        if (valid_command) {        // if typed letter corresponds to an actual command, evaluate the numbers
                            j = 1;

                            for (; i < MESSAGE_START + 9; i++) {
                                command_queue[command_write].command[j] = IOT_Data[line][i];
                                j++;
                            }

                            i = MESSAGE_START + 6;

                            command_queue[command_write++].duration = String_To_MS(i, line);       // converts typed string to movement time in counts of 10 ms and increments index

                            if (command_write >= QUEUE_SIZE) {
                                command_write = BEGINNING;
                            }

                            if (!first_command) {
                                first_command = TRUE;
                                time_count = 0;
                            }

                        }

                    }
                }

                phone_message_flag = FALSE;     // done processing message, reset flag
            }

            //-----------------

            character = 0;
            prev_line = line;
            line++;             // move to start of new line
            if(line >= 4){
                line = 0;       // reset line index if above limit
            }

            // take some action here if needed
        }
    }

}
