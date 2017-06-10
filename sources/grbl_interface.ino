#include "grbl_interface.h"

LiquidCrystal_I2C Lcd(0x27, 18, 2); // set the LCD address to 0x27 for a 18 chars and 2 line display

void setup()
{

	//inits
	grbl_protocole_init();
	Lcd.init();
	tools_init_lcd(&Lcd);


	delay(500);

}


// The loop function is called in an endless loop
void loop()
{


	/* Serial from GRBL to COMPUTER */
	execute_grbl_to_computer();

	/* Serial from COMPUTER to GRBL */
	execute_computerAndArduino_to_grbl();

	/* Monitor Serial (COMPUTER) state */
	serial_management_monitor_PC_state();

	/* Periodic actions */
	grbl_protocole_periodic_actions();

}


