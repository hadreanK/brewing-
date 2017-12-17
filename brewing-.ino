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
LiquidCrystal Lcd(48, 49, 50, 51, 52, 53);
OneWire oneWire(thermoPin); // Set up oenWire instance to the thermometer
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature.
    
    
void setup() {
// TIMERS
  	atMega2560_T4(0b101, (15625 - 1)); // 16MHz/1024, count to 15625
	atMega2560_T3(0b101, 1500); // 16MHz/1024, count to 1500
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
  
}

// TIMER ISR
ISR(TIMER4_COMPA_vect) {
  	Timer.t++; // Increment timer
  	
} 

// TIMER ISR
ISR(TIMER3_COMPA_vect) {
	// This interrupt should have higher priority than the button interrupts. I sure hope it does...
	// disable debounce interrupt
  	Button.debounce();// debounce
	
} 

// Interrupt ISR for button 1, pin 2, INT 4
ISR(INT4_vect) { // Interrupt vectors on page 101, this is pin 2
    Button.trigger(0);
	// Enable debounce interrupt   
}
