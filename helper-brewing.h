#ifndef helper_brewing_h
#define helper_brewing_h

// INCLUDES
<<<<<<< HEAD
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





=======
#include "Arduino.h"
#include <OneWire.h>            // required for thermistor
#include <DallasTemperature.h>  // required for thermistor

// CLASS AND FUNCTION DEFINITIONS
class Thermometers {
        int pin;
        float temp;
    public:
        void setUp(int thermoPin);
        float getTemp();
};

class Timers{
    int timerValues[10];
    int timerDone[10];
    int buzzerOn = 0;
    int buzzerPin;
public:
    unsigned long t;
    void setUp(int buzzerPinSet);
    void setTimer(int timerID, unsigned long timerValue);
    int checkTimers();
    void clearTimers();
};

class Buttons{
	int pressed[3]; // to record when a button has been pressed	
public:
	int count[3]; // to count how many times a button has been pressed
	void trigger(int buttonID);
	void debounce();	
>>>>>>> 9b926b7100fb11523bbac2393931b53c5e6dd49a

#endif
