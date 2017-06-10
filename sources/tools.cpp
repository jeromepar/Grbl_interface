/*
 * tools.cpp
 *
 *  Created on: 25 févr. 2017
 *      Author: Jerome
 */


#include "tools.h"

LiquidCrystal_I2C *lcd;

void tools_init_lcd(LiquidCrystal_I2C *item){
	lcd = item;

	lcd->backlight();
	lcd->printCentered(0, "Ready!");
}

void tools_disp_lcd_status(struct mystatus *s){

	static float x = -1000, y = -1000, z = -1000;
	static int l = -1;
	static char status_prec = '?';

	if (status_prec != s->stat[0]) {
		status_prec = s->stat[0];
		lcd->clearLine(0);
		lcd->setCursor(0, 0);
		lcd->myPrint(s->stat);
	}
	if (l != s->lim_pins) {
		l = s->lim_pins;
		lcd->setCursor(9, 0);
		lcd->myPrint("lim:");
		lcd->myPrintLong((long)s->lim_pins,10);
	}

	if (x != s->WposX) {
		x = s->WposX;
		lcd->setCursor(0, 1);
		lcd->myPrintFloat(s->WposX, 1,4);
	}
	if (y != s->WposY) {
		y = s->WposY;
		lcd->setCursor(5, 1);
		lcd->myPrint("|");
		lcd->myPrintFloat(s->WposY, 1,4);
	}
	if (z != s->WposZ) {
		z = s->WposZ;
		lcd->setCursor(11, 1);
		lcd->myPrint("|");
		lcd->myPrintFloat(s->WposZ, 1,4);
	}


	/*
  Serial.print("ok ");
  Serial.print(status.stat);
  Serial.print("/");
  Serial.print(status.WposX);
  Serial.print("/");
  Serial.println(status.lim_pins);
	 */
}
