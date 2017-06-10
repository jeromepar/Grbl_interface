/*
 * grbl_protocole.h
 *
 *  Created on: 25 févr. 2017
 *      Author: Jerome
 */

#ifndef GRBL_PROTOCOLE_H_
#define GRBL_PROTOCOLE_H_

#include "serial_management.h"
#include "SimpleFIFO.h"
#include "tools.h"

#define MAX_COMMAND_FIFO_SIZE 20 /* deepf of command FIFO (grbl + internal) */
#define GET_STATUS_PERIOD 500 /* status query period */

/* struct to host the status reports */
typedef struct mystatus{
  char *stat;
  float WposX;
  float WposY;
  float WposZ;
  unsigned int lim_pins;
} s_status;

/* usefull to process status report */
typedef enum seq_status {
  STATUS,
  MPOS_TITLE,
  MPOSX,
  MPOSY,
  MPOSZ,
  WPOS_TITLE,
  WPOSX,
  WPOSY,
  WPOSZ,
  LIM_TITLE,
  LIM
} e_seq_status;


/* Basic type for the command FIFO */
typedef struct mycommand{
  bool  locale; /* i.e. from this arduino */
  bool  response_expected;
  /*char  command[MAX_LINE_LENGTH];*/
} s_command;


void grbl_protocole_init(void);
void execute_grbl_to_computer(void);
void execute_computerAndArduino_to_grbl(void);
void grbl_protocole_periodic_actions(void);
bool grbl_protocole_is_response_needed(char s[]);
void grbl_protocole_send_Grbl_Command(bool isLocale, char s[]);
void grbl_protocole_process_computer_command(char * command);
void grbl_protocole_parseStatus(s_status* s, char *line);
bool grbl_protocole_validate_grbl_response(bool * locale, bool *valid, char s[]);
bool grbl_protocole_is_acq(char s[]);

#endif /* GRBL_PROTOCOLE_H_ */
