/*
 * serial_management.h
 *
 *  Created on: 25 févr. 2017
 *      Author: Jerome
 */

#ifndef SERIAL_MANAGEMENT_H_
#define SERIAL_MANAGEMENT_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define MAX_LINE_LENGTH 200 /* max command length */

void serial_management_monitor_PC_state(void);
void serial_management_init(void);
void serial_management_save_buffer1(char output_s[], int length);
void serial_management_save_grbl_startup_line();

void serial_management_free_serial();
void serial_management_free_serial1();
bool serial_management_is_serial_available();
bool serial_management_is_serial1_available();

char * serial_management_get_serial_buffer();
char * serial_management_get_serial1_buffer();

#endif /* SERIAL_MANAGEMENT_H_ */
