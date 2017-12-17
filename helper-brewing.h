#ifndef helper_brewing_h
#define helper_brewing_h

// INCLUDES
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

#endif
