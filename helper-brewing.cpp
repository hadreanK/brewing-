/*
This file has the helper functions and classes for main-brewing.ino
*/

#ifndef helper_brewing_cpp
#define helper_brewing_cpp

#include "helper-brewing.h"

#include "Arduino.h"



// THERMOMETER DEFINITIONS
Thermometers:setUp(int thermoPin){

    pinMode(thermoPin,OUTPUT);


}

// TIMER DEFINITIONS
void Timers::setUp(int buzzerPinSet){
    t = 0; // zero out the time
    buzzerPin = buzzerPinSet; // store the buzzer pin
    pinMode(buzzerPinSet,OUTPUT);
}

void Timers::setTimer(int timerID, unsigned long timerValue){
    timerValues[timerID] = t+timerValue;
}

int Timers::checkTimers(){
    for (int i=0; i<11; i++){
         if((timerValues[i] < t) & timerValues[i]>0){
            timerDone[i] = 1;
            buzzerOn = 1;
        } // if
    } // for
    digitalWrite(buzzerPin,buzzerOn);// TURN ON buzzer if buzzerOn
    return(buzzerOn);
}

void Timers::clearTimers(){
    for (int i=0; i<11; i++){
         if(timerDone[i]){
             timerValues[i] = 0;
             timerDone[i] = 0;
         } // if
    } // for
}

// BUTTON DEFINITIONS
void Buttons::trigger(int buttonID){
	pressed[buttonID] = 1;
}

void Buttons::debounce(){
// The big question here is, would a for loop be faster, or would 
// adding each element together be faster? I think adding each element
// might be faster...

count[0] = count[0] + pressed[0]; 
count[1] = count[1] + pressed[1];
count[2] = count[2] + pressed[2];
count[3] = count[3] + pressed[3];
}




#endif
