#ifndef helper-brewing_h
#define helper-brewing_h

// INCLUDES
#include <AVR-Timers>

// CLASS AND FUNCTION DEFINITIONS

// BeerThermometer will set up a thermometer using the oneWire, read the thermometer and print out the Temperature in °F

class BeerThermometer{
		int pin;
		int temp;
	public:
		void setup();
		void getTemp();
		float printTemp();
};

// BeerTimer will set timers to go off after a certain amount of time has passed
class BeerTimer{
		int setTime; // The time the alarm is set for
	public:
		void setTimer(); 	// This will set the timer
		int timeLeft(); 	// returns the time left in seconds
		
		
	
};






#endif
