/* 
This file is to help the LED_Clock arduino file to control my 
LED display I jacked from that sony clock
*/

#ifndef LED_Clock_Helper_cpp
#define LED_Clock_Helper_cpp

// INCLUDES
	#include "LED_Clock_helper.h"
	#include "Arduino.h"
	#include "LED_pin_converter.h"
	
// Functions
void initializeClockTimers( int T3, int T4){
	// Use Timer 3 to control the clock switching since it has a higher ISR priority to Timer 4
	if (T3 > 0){
		TCCR3A = 0x00; // Want bits 1:0 to be 00, they're WGM3 1:0, and everything else too
		TCCR3B = 0x0C; // Bits 7:6 have to do with input capture, 5 is reserved, 4:3 are waveform generation
		// Bits 4:3 are WGM3 3:2, so we want them to be 0b01, b/c WGMn3:0 = 0b0100, table 17-2 page 145,
		//   because this is CTC mode which clears the counter once it gets to OCR3A
		// Bits 2:0 are the prescalar, p 157 table 17-6
		// I set it to 256 with 0b0100
		OCR3A = 0x0271; // Want the output compare register to be 625 to get 100Hz
		// 16 MHz, 16e6/256 = 62500
		TIMSK3 = 0b0010; // bits 3:1 enable the output compare C:A respectively match interrupt, 0 is overflow
	} // if
	
	// Use Timer 4 as the clock timer
	if (T4>0){
		TCCR4A = 0x00; // Want bits 1:0 to be 00, they're WGM3 1:0, and everything else too
		TCCR4B = 0x0D;  // Bits 7:6 have to do with input capture, 5 is reserved, 4:3 are waveform generation
					  // Bits 4:3 are WGM3 3:2, so we want them to be 01, b/c WGMn3:0 = 0b0100, table 17-2 page 145, 
					  //    because this is CTC mode which clears the counter once it gets to OCR3A
					  // Bits 2:0 are the prescalar, p 157 table 17-6
					  // I set it to 1024 with 0b0101
		OCR4A = 0x3D09-1;   // Want the output compare register to be 15325-1 so it fires once a second
						  // 16 MHz, 16e6/1024 = 15625
		TIMSK4 = 0b0010; // bits 3:1 enable the output compare C:A respectively match interrupt, 0 is overflow
		} // if

	}; // initializeTimers
	
void setupClockOutputs(void){
	// Set up PORT A output pins
	DDRA = 0xFF; // setting a pin to 1 is output, 0 is input.
	// I'm setting A0-A7 as outputs
	DDRC = 0xFF;       // Setting C0-C7 as outputs
	PORTA = 0x00;      // Set Port A as low
	PORTC = 0x01;      // Set Cathode 1 as high 
	}; // setupOutputs

void setClockPins(clockInfo *infop){
	double t; int x;

	infop->t = infop->t%(13*3600); // Reset t if it's over 13 hours
	infop->t = max(infop->t,3600); // Make it reset to 1:00 after midnight/noon, not 0:00
	// Set the 4 clock digits from t in seconds
		t = (double) infop->t;
		infop->d1 = 10 - 9*(t >= 36000); // Tens place of the hours goes in the 1st digit
    		            // ^ is equal to 1 if the number of hours is over 9
		infop->d2 = ((int) floor(t/3600))%10;
		infop->d3 = ((int) floor(t/600))%6;
		infop->d4 = ((int) floor(t/60))%10;
	// Set clock output pins for Cathode 1
		x =   leds2pins(num2leds(infop->d4),4,1) 
            + leds2pins(num2leds(infop->d3),3,1) 
            + leds2pins(num2leds(infop->d2),2,1) 
            + leds2pins(num2leds(infop->d1),1,1); // x contains which pins should be active for Cathode 1
        infop->PortAc1Pins = x&0xFF; // set the pins for port A, which are just the 8 least significant bits of x
    	x = (x&0x3F00)>>6; // Bits 8-13 will become Bits 2-8 on PORTC
    	infop->PortCc1Pins =  (((x&0x80) >> 5) + ((x&0x40) >> 3) + ((x&0x20) >> 1) + 
                        	((x&0x10) << 1)  + ((x&0x08) << 3) + ((x&0x04) << 5)); // reverse order of the Port C, as the pin ordering is reversed    
    // Set clock output pins for Cathode 2
    	x =   leds2pins(num2leds(infop->d4),4,2)
            + leds2pins(num2leds(infop->d3),3,2) 
        	+ leds2pins(num2leds(infop->d2),2,2) 
        	+ leds2pins(num2leds(infop->d1),1,2) + 8192; // x contains which pins should be active for Cathode 2
                                            // ^^ Adding in 8192 into digit 1 on cathode 2 turns on the 
                                            // little dots between the hours and minutes
    infop->PortAc2Pins = x&0xFF;  // set the pins for port A
    x = (x&0x3F00)>>6; // Bits 8-13 will become Bits 2-8 on PORTC
    infop->PortCc2Pins =  (((x&0x80) >> 5) + ((x&0x40) >> 3) + ((x&0x20) >> 1) + 
                         ((x&0x10) << 1) + ((x&0x08) << 3) + ((x&0x04) << 5)); // reverse order of the Port C, as the pin ordering is reversed   
};


int num2leds(int num){
	int out;
	switch(num) {
		case 0:  out = 0x3F;  break;
		case 1:  out = 0x6;   break;
		case 2:  out = 0x5B;  break;
		case 3:  out = 0x4F;  break;
		case 4:  out = 0x66;  break;
		case 5:  out = 0x6D;  break;
		case 6:  out = 0x7D;  break;
		case 7:  out = 0x7;   break;
		case 8:  out = 0x7F;  break;
		case 9:  out = 0x6F;  break;
		case 10: out = 0x0;	  break;	// 10 is display nothing
	} // switch
	return(out);
} // num2leds

int leds2pins(int leds, int digit, int cathode){
	int i = 0;
	int this_pin = 0;
	int pins = 0;
	switch(digit){
		case 1:
			if(cathode==1){
				for (i=0; i<7; i++){
					this_pin = pow2(i);
					pins = pins + ((leds&this_pin)>0)*(d1c1[i]);
					// (1 if leds says yes)*(the correct pin value)
			}//for
			}else{
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d1c2[i]);
					// (1 if leds says yes)*(the correct pin value)
			}//for				
			}//if
			break;
		case 2:
			if(cathode==1){
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d2c1[i]);
					// (1 if leds says yes)*(the correct pin value)
				}//for
			}else{
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d2c2[i]);
					// (1 if leds says yes)*(the correct pin value)
				}//for				
			}//if	
			break;
			
		case 3:
			if(cathode==1){
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d3c1[i]);
					// (1 if leds says yes)*(the correct pin value)
				}//for
			}else{
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d3c2[i]);
					// (1 if leds says yes)*(the correct pin value)
				}//for				
			}//if
			break;
			
		case 4: 
			if(cathode==1){
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d4c1[i]);
					// (1 if leds says yes)*(the correct pin value)
				}//for
			}else{
				for (i=0; i<7; i++){
					this_pin = pow2(i);	
					pins = pins + ((leds&this_pin)>0)*(d4c2[i]);
					// (1 if leds says yes)*(the correct pin value)
				}//for				
			}//if
			break;
	}
	return(pins);
}; //leds2pins
	
int pow2(int power){
	int y = 1;
	for (int i = 1; i <= power; i ++){
		y = y*2;}   // for
	return(y);
}// pow2
	
	
#endif
