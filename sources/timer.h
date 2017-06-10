/*
 * timer.h
 *
 *  Created on: 20 janv. 2017
 *      Author: Jerome
 */

#ifndef TIMER_H_
#define TIMER_H_


class Timer
{
public:
	Timer(void (*function)());

	void start(unsigned long delay);
	void stop();
	bool watch();
private:
	bool is_activated;
	unsigned long EndDate;
  void (*f)();

};



#endif /* TIMER_H_ */
