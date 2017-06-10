/*
 * timer.cpp
 *
 *  Created on: 20 janv. 2017
 *      Author: Jerome
 */


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "timer.h"

Timer::Timer(void (*function)())
{
	is_activated = false;
	EndDate = 0;
	f=function;
}

void Timer::start(unsigned long delay_ms)
{
	is_activated = true;
	EndDate=millis()+delay_ms;
	
}

void Timer::stop()
{
	is_activated = false;
	EndDate = 0;
}

bool Timer::watch()
{
  bool output = false;

	if (is_activated && (millis()>EndDate) )
 {
		f();
    output=true;
    this->stop();
 }

 return(output);
}
