/*
 * grbl_protocole.cpp
 *
 *  Created on: 25 f�vr. 2017
 *      Author: Jerome
 */

#include "grbl_interface.h"
#include "grbl_protocole.h"
#include "serial_management.h"


SimpleFIFO<s_command, MAX_COMMAND_FIFO_SIZE> commands_fifo;

void getStatus() {
	grbl_protocole_send_Grbl_Command(true, "?\n");
}
Timer gestatus_timer(&getStatus);


bool grbl_startup_line_picked;


void grbl_protocole_init(){
	commands_fifo.flush();
	serial_management_init();

	grbl_startup_line_picked = false;
}

bool grbl_protocole_is_response_needed(char s[]) {
	bool response_needed  = false;

	switch (s[0]) {
	case '?' :
		response_needed = true;
		break;
	case '$' :
		switch (s[1]) {
		case '\n' :
		case '$' :
		case '#' :
		case 'G' :
		case 'I' :
		case 'N' :
		case 'C' :
			response_needed = true;
			break;
		default:
			response_needed = false;
			break;
		}
		break;
		default:
			response_needed = false;
			break;
	}
	return response_needed;
}

void grbl_protocole_send_Grbl_Command(bool isLocale, char s[]) {


	if(s[0]!=0)
	{
		if(DEBUG_FIFO) {
			Serial.print("** IN : ");
			Serial.println(s); }

		bool response_needed = grbl_protocole_is_response_needed(s);
		s_command cmd = {isLocale, response_needed};
		Serial1.write(s);
		commands_fifo.enqueue(cmd);
	}
}


void grbl_protocole_process_computer_command(char * command) {
	if (command[0] == '?')
		gestatus_timer.start(GET_STATUS_PERIOD);
}

void grbl_protocole_parseStatus(s_status* s, char *line) {
	/*
     Expected : <Idle,MPos:0.000,0.000,0.000,WPos:0.000,0.000,0.000,Lim:000> (last one optionnal)
	 */
	static char stat[10];
	char * pch;
	int e = STATUS;
	pch = strtok (line, "<,>:");
	while (pch != NULL)
	{
		switch (e) {
		case STATUS:
			strncpy(stat, pch, sizeof(stat));
			break;
		case WPOSX:
			s->WposX = ((String)pch).toFloat();
			break;
		case WPOSY:
			s->WposY = ((String)pch).toFloat();
			break;
		case WPOSZ:
			s->WposZ = ((String)pch).toFloat();
			break;
		case LIM:
			s->lim_pins = atoi(pch);
			break;
		default:
			//do nothing
			break;
		}

		//incrementation
		pch = strtok (NULL, "<,>:");
		e = e + 1;
	}

	s->stat = stat;
}

bool grbl_protocole_validate_grbl_response_old(bool * locale, bool *valid, char s[]) {
	*valid = false;
	bool error = false;

	if (commands_fifo.count() > 0) {

		/* If response status */
		if ( ((s[0] == 'o') && (s[1] == 'k')) || ((s[0] == 'e') && (s[1] == 'r')) )
		{
			s_command cmd = commands_fifo.dequeue();
			*locale = cmd.locale; /* response or status */
			*valid = false;
		} else {
			s_command cmd = commands_fifo.peek();
			*locale = cmd.locale; /* response or status */
			*valid = true;
		}

	} else if ( (s[2] == 'G') && (s[3] == 'r') ) { /* Spontaneous init string from grbl */
		*valid = true;
		*locale = false;
	} else
	{
		*locale = true;
		*valid = false;
	}

	return error;
}

typedef enum protocol_state{
	e_prot_idle,
	e_prot_response_received
} t_e_prot;

bool grbl_protocole_validate_grbl_response(bool * locale, bool *valid, char s[]) {

	*valid = false;
	bool error = false;

	if(DEBUG_FIFO) { Serial.println("-- Enter"); }

	if (commands_fifo.count() > 0) {
		if(DEBUG_FIFO) { Serial.println("-- Cmd enqeued"); }

		s_command cmd = commands_fifo.peek();
		*locale = cmd.locale; /* response or status */

		bool acq = grbl_protocole_is_acq(s);

		static t_e_prot state=e_prot_idle;

		switch (state) {
		case e_prot_idle:
			if (acq && (cmd.response_expected==false) ){
				if(DEBUG_FIFO) { Serial.println("-- Idle without resp -DEQUEUE"); }
				commands_fifo.dequeue();
				*valid = false;
			} else {
				if(DEBUG_FIFO) { Serial.println("-- Resp OK --> to 2nd state"); }
				state = e_prot_response_received;
				*valid = true;
			}
			break;
		case e_prot_response_received:
			if (acq){
				if(DEBUG_FIFO) { Serial.println("-- acq --> to END"); }
				commands_fifo.dequeue();
				*valid = false;
				state = e_prot_idle;
			} else {
				if(DEBUG_FIFO) { Serial.println("-- response continuing"); }
				*valid = true;
			}
			break;
		}


	} else if ( (s[2] == 'G') && (s[3] == 'r') ) { /* Spontaneous init string from grbl */
		if(DEBUG_FIFO) { Serial.println("-- spontaneous startup"); }
		*valid = true;
		*locale = false;
	} else
	{
		if(DEBUG_FIFO) { Serial.println("-- ignored"); }
		*locale = true;
		*valid = false;
	}
	if(DEBUG_FIFO) { Serial.println("-- END"); }
	return error;
}

bool grbl_protocole_is_acq(char s[]){
	bool acq = false;
	/* If response status */
	if ( ((s[0] == 'o') && (s[1] == 'k')) || ((s[0] == 'e') && (s[1] == 'r')) )
	{
		acq = true;
	}
	return acq;
}

void grbl_protocole_process_grbl_response(char * response) {

	/* memorisation of startup line in order to feed it to the computer
      as soon as the connection starts
	 */
	if (grbl_startup_line_picked == false) {
		grbl_startup_line_picked = true;

		/* save startup line */
		serial_management_save_grbl_startup_line();
		//gestatus_timer.start(GET_STATUS_PERIOD);

		//ask for limit pins
		if (ENABLE_LIMIT_PINS_REPORT) {
			grbl_protocole_send_Grbl_Command(true, "$10=19\n"); //1 machine pos +2 work pos + 16 lim pins
		}
	}

	if (response[0] == '<') // status line
	{
		static s_status status = {"None", 0, 0, 0, 0xffff};
		grbl_protocole_parseStatus(&status, response);

		tools_disp_lcd_status(&status);

	}


}



/* Serial from GRBL to COMPUTER */
void execute_grbl_to_computer(){
	if (serial_management_is_serial1_available()) {

		char * response = serial_management_get_serial1_buffer();

		bool locale = false;
		bool valid = false;
		bool error = grbl_protocole_validate_grbl_response(&locale, &valid, response);

		if(DEBUG_FIFO) {
			Serial.println("--------");
			Serial.write("locale:");
			Serial.print(locale);
			Serial.write("|valid:");
			Serial.print(valid);
			Serial.write("|resp:");
			Serial.write(response);
			Serial.println("=======");
		}

		if (!locale) /* if the command comes from the computer */
			Serial.write(response);

		if (valid) /* not ok or error */
			grbl_protocole_process_grbl_response(response);

		serial_management_free_serial1();
	}

} /* END OF Serial from GRBL to COMPUTER */

/* Serial from COMPUTER to GRBL */
void execute_computerAndArduino_to_grbl(){

	if (serial_management_is_serial_available()) {

		char * cmd = serial_management_get_serial_buffer();

		/* transmit command to grbl without additional computing */
		grbl_protocole_send_Grbl_Command(false /* not locale */, cmd);

		grbl_protocole_process_computer_command(cmd);

		serial_management_free_serial();
	}

}/* END OF Serial from GRBL to COMPUTER */

void grbl_protocole_periodic_actions(){

	/* Timer management for status*/
	if (gestatus_timer.watch() ) {
		gestatus_timer.start(GET_STATUS_PERIOD);
	}
	/* END OF Timer management */
}
