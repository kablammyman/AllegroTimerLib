#include <allegro.h>
#include "AllegroTimer.h"


volatile unsigned int speed_counter;
volatile unsigned int milliSec;
volatile int fps;
volatile int frame_counter;    
bool isInit = false;

int decimals  = 0, seconds = 0, minutes = 0;
int dec_factor = 0, sec_factor = 0, min_factor = 0;


void fps_proc() //this counts frame rate
{
	fps = frame_counter;
	frame_counter = 0;
}
END_OF_FUNCTION(fps_proc);
//---------------------------------------------------
void increment_speed_counter() 
{ 
	speed_counter++; 
}
END_OF_FUNCTION(increment_speed_counter);
//---------------------------------------------------
void inc_milliSec()
{
	milliSec++;
}
END_OF_FUNCTION(inc_milliSec);  
//---------------------------------------------------
void initAllegroTimer()
{
	if(isInit)
		return;
	LOCK_VARIABLE(speed_counter);
	LOCK_VARIABLE(milliSec);  
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame_counter);

	LOCK_FUNCTION(inc_milliSec);
	LOCK_FUNCTION(increment_speed_counter); 
	LOCK_FUNCTION(fps_proc);
	
	install_int(inc_milliSec, 10);// every second    //ever 10 of a sec 
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));
	install_int(fps_proc, 1000); 

	isInit = true;
}

void AllegroTimer::startElapsedTimer()
{  
	startMilli = milliSec;
	elapsedTimer = true;
	decimals = 0;
	seconds = 0;
	minutes = 0;
	pause = false;
	timeOver = false;
}
//---------------------------------------------------
void AllegroTimer::updateElapsedTime()
{  
	if(pause)
	{
		startMilli = milliSec;
		return;
	}
	if(milliSec >= (startMilli+99))
	{                             
		int updateAmt = milliSec - (startMilli+99);
		if(updateAmt > 99)//if more than a second passed by
		{
			int min,sec,dec;
			convert_to_standard(updateAmt, min, sec, dec);
			decimals += dec;
			seconds += sec;
			minutes += min;
		}
		else
			decimals = updateAmt;             
		startMilli = milliSec;
		seconds++;
	}
	else
	{
		decimals++;
	}
	if(seconds >= 60)
	{                                                      
		seconds = 0;
		minutes++;                    
	}      
}

//---------------------------------------------------
void AllegroTimer::startCountdown(int min, int sec, int milli)
{  
	startMilli = milliSec;
	elapsedTimer = false;
	decimals = milli;
	seconds = sec;
	minutes = min;

	if(decimals == 0)
	{
		if(seconds > 0)
			seconds--;
		else if(minutes > 0)
		{
			minutes--;
			seconds = 59;
		}
	}

	pause = false;
	timeOver = false;
	
}
//---------------------------------------------------
void AllegroTimer::updateCountdown()
{  
	if(pause)
	{
		startMilli = milliSec;
		return;
	}

	decimals = (startMilli+99) - milliSec;
	
	if(decimals <= 0)
	{                             
		if(minutes == 0 && seconds == 0)
		{
			decimals = 0;
			timeOver = true;
			return;
		}
		int updateAmt = milliSec - (startMilli+99);
		if(updateAmt > 99)//if more than a second passed by
		{
			int min,sec,dec;
			convert_to_standard(updateAmt, min, sec, dec);
			decimals = dec;
			seconds -= sec;
			minutes -= min;
		}
		decimals = 0;
		bool secChange = false;

		if(minutes > 0 && seconds == 0)
		{
			minutes--;
			seconds = 59;
			secChange = true;
		}
		if(seconds > 0)
		{
			if(!secChange)//if we already modified the seconds, dont mod it again
				seconds--;
			startMilli = milliSec; 
		}
		if(seconds <= 0)
			seconds = 0;
		if(minutes <= 0)
			minutes = 0;
		
	}
}
//---------------------------------------------------
void AllegroTimer::updateStopWatch()
{  
	if(elapsedTimer)
		updateElapsedTime();
	else
		updateCountdown();
}
//---------------------------------------------------
bool AllegroTimer::isTimeUp()
{  
	if(decimals <= 0 && seconds <= 0 && minutes <= 0)
		return true;
	return false;
}

int AllegroTimer::convert_to_milliseconds(int min, int sec,int dec)
{
	return (min*60000)+(sec*1000)+dec; 
}
//---------------------------------------------------
void AllegroTimer::convert_to_standard(int time, int &min, int &sec, int &dec)
{
	dec = time%99;
	int x = time/99;
	min = x/60;
	sec = x%60;
}
//---------------------------------------------------
void AllegroTimer::addToMins(int amt)
{
	minutes += amt;
}
//---------------------------------------------------
void AllegroTimer::addToSecs(int amt)
{
	seconds += amt;
}
//---------------------------------------------------
void AllegroTimer::addToMils(int amt)
{
	decimals += amt;
}
