#ifndef TIMERS_H
#define TIMERS_H
#include <allegro.h>
#include <winalleg.h>

extern volatile unsigned int speed_counter;
extern volatile unsigned int milliSec;
extern volatile int fps;
extern volatile int frame_counter;
extern bool isInit;

void fps_proc(); //this counts frame rate
void increment_speed_counter(); 
void inc_milliSec();
void initAllegroTimer();

//we can use this to verify the setup was done. right now, no checks are in place

//to make sure the allegro timer stuff gets done, we will make the user call this method


class AllegroTimer
{
public:
	int decimals, seconds, minutes;
	int dec_factor, sec_factor, min_factor;
	bool elapsedTimer;
	unsigned int startMilli;
	bool pause;
	bool timeOver;

	AllegroTimer()
	{
		decimals = seconds = minutes = 0;
		dec_factor = sec_factor = min_factor = 0;
		elapsedTimer = true;
		startMilli = milliSec;
	}

	void startElapsedTimer();
	void startCountdown(int min, int sec, int milli);
	void updateElapsedTime();
	void updateCountdown();
	void updateStopWatch();
	bool isTimeUp();
	void convert_to_standard(int time, int &min, int &sec, int &dec);
	int convert_to_milliseconds(int min, int sec,int dec);
	void addToMins(int amt);
	void addToSecs(int amt);
	void addToMils(int amt);
};


#endif //TIMERS_H

