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


#endif
