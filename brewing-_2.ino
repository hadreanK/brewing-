// DEFINES
	#define T1PIN 10
	#define T2PIN 11
	#define T3PIN 12
	#define BUZZPIN 13


// LIBRARIES AND HELPER FILES
	#include "helper-brewing.h"
	#include <avrTimers.h>
	#include <LiquidCrystal.h>

// GLOBAL  VARIABLES AND OBJECTS
	Timers Timer;
	Buttons Button;
	int errAh;
	LiquidCrystal Lcd(0,1,2,3,4,5); // This needs to be changed

	// SET UP THERMOMETERS 
	T1_OW oneWire(T1PIN); 	// Set up oenWire instance to the thermometer
	T1_DT sensors(&T1_OW); 	// Pass the oneWire reference to Dallas Temperature.
	T2_OW oneWire(T2PIN); 	// Set up oenWire instance to the thermometer
	T2_DT sensors(&T2_OW); 	// Pass the oneWire reference to Dallas Temperature.
	T3_OW oneWire(T3PIN); 	// Set up oenWire instance to the thermometer
	T3_DT sensors(&T3_OW); 	// Pass the oneWire reference to Dallas Temperature.

    
void setup() {
// TIMERS
  	atMega328_T1(0b101, (15625 - 1)); // 16MHz/1024, count to 15625, 1Hz
	atMega328_T2(0b101, 1500); // 16MHz/1024, count to 1500, 10Hz
  	Timer.setUp(12);
  	Timer.setTimer(0, 15);
  	Timer.setTimer(1, 8);
  
// LCD
	Lcd.begin(16, 2);
// THERMOMETERS
    sensors.begin(); 
// INTERRUPT
    EICRA = 0xF0;   // Set INT2 and INT3 to rising edge interrupt, set to 0b11, 15.2.2, page 111
    EICRB = 0x03;   // Set INT4 to rising edge interrupt, set to 0b11, 15.2.2, page 111
    EIMSK = 0x1C;   // Enable interrupts INT2, INT3 and INT4 page 111
}

void loop() {
// LCD
  	Timer.checkTimers();
  	Lcd.setCursor(0, 0); Lcd.print(Timer.t);
  	Lcd.setCursor(0, 1); Lcd.print(Timer.t);
  	Lcd.setCursor(9, 0); Lcd.print(OCR4A);
  	Lcd.setCursor(9, 1); Lcd.print(errAh);

// MENU
	// Check to see if menu navigation buttons have been pushed
  	// Navigate accordingly
	// Check to see if action button has been pushed
	// Make the action, based on where we are in the Menu
}

// TIMER ISR 1Hz
ISR(TIMER4_COMPA_vect) {
  	Timer.t++; // Increment timer
  	
} 

// TIMER ISR 10Hz
ISR(TIMER3_COMPA_vect) {
	// This interrupt should have higher priority than the button interrupts. I sure hope it does...
	// disable debounce interrupt
  	Button.debounce();// debounce
	
} 

// Interrupt ISR for button 0, pin 2, INT 4
ISR(INT4_vect) { // Interrupt vectors on page 101, this is pin 2
    Button.trigger(0);
	// Enable debounce interrupt   
}

// Interrupt ISR for button 0, w/e
// Button.trigger(1);