#include "serial_management.h"


static char buffer [MAX_LINE_LENGTH];
static char buffer1 [MAX_LINE_LENGTH];
static int cnt = 0;
static int cnt1 = 0;
static bool serial_ready = false;
static bool serial1_ready = false;

bool PC_serial_connected = false;
char grbl_startup_line[30];


void serialEvent() {
	if (Serial.available())
	{
		char c = Serial.read();
		buffer[cnt++] = c;

		if (c == 10 /* line feed */ && cnt > 1 /* ie different from \n\r\0 */)
		{
			buffer[cnt] = '\0';
			cnt = 0;
			serial_ready = true;
		}
	}
}

void serialEvent1() {
	if (Serial1.available())
	{
		char c = Serial1.read();
		buffer1[cnt1++] = c;

		if (c == 10 /* line feed */ && cnt1 > 2 /* ie different from \n\r\0 */)
		{
			buffer1[cnt1] = '\0';
			cnt1 = 0;
			serial1_ready = true;
		}
	}

}

/* Monitor Serial (COMPUTER) state */
void serial_management_monitor_PC_state(){

	if ( (PC_serial_connected == false) && Serial )
	{
		Serial.write(grbl_startup_line);
		PC_serial_connected = true;
	} else if ( !Serial )
	{
		PC_serial_connected = false;
	}
}

void serial_management_init(){

	// Open serial communications
	Serial.begin(115200);
	Serial1.begin(115200);

	cnt = 0;
	cnt1 = 0;
	serial_ready = false;
	serial1_ready = false;

	PC_serial_connected = false;
	grbl_startup_line[0] = 0;
}

void serial_management_save_buffer1(char output_s[], int length){
	strncpy ( output_s, buffer1, length );
}

void serial_management_free_serial() {
	serial_ready = false;
}
void serial_management_free_serial1() {
	serial1_ready = false;
}

bool serial_management_is_serial_available() {
	return serial_ready;
}
bool serial_management_is_serial1_available(){
	return serial1_ready;
}

char * serial_management_get_serial_buffer() {
	return &buffer[0];
}
char * serial_management_get_serial1_buffer() {
	return &buffer1[0];
}

void serial_management_save_grbl_startup_line(){
	serial_management_save_buffer1(grbl_startup_line, sizeof(grbl_startup_line));
}
