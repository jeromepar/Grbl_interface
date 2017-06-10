/*
 * tools.h
 *
 *  Created on: 25 févr. 2017
 *      Author: Jerome
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include "grbl_interface.h"
#include "grbl_protocole.h"

void tools_disp_lcd_status(struct mystatus *s);

void tools_init_lcd(LiquidCrystal_I2C *item);

#endif /* TOOLS_H_ */
