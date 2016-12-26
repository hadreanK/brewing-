					// INCLUDES and DEFINES
#ifndef LED_Clock_Helper_h
#define LED_Clock_Helper_h

					// NEW DATA TYPES
typedef struct{
	int d1; 		// digit to put on 1st display
	int d2;			// 		"		"  2nd 		"
	int d3;			// 		"		"  3rd 		"
	int d4;			// 		"		"  4th 		"
	int PortAc1Pins;		// Anode pins on PORT A that need to be on while 1st cathode is on
	int PortAc2Pins;		// 		"			"			"	   	"		 2nd 	 	"
	int PortCc1Pins;		// Anode pins on PORT B that need to be on while 1st cathode is on
	int PortCc2Pins;		// 		"			"			"		"	     2nd 	 	"
	unsigned int t;			// time in seconds
	int Cathode;	// Which cathode is currently LOW, or active. 0 or 1.
} clockInfo; 
//extern clockInfo cInfo;
					// FUNCTION PROTOTYPES
void initializeClockTimers( int T3, int T4);
// input 1's to initialize timers T2 and T3 respectively

void setupClockOutputs(void);
// Sets up Port A and C as outputs for controlling the LEDs

void setClockPins(clockInfo *infop);
// Sets PortAc1Pins, PortAc2Pins, PortCc1Pins and PortCc2Pins to display
// the time given t in seconds. 
// Pass it the pointer to the clockInfo structure

int num2leds(int num);
// input: the number you would like to print on the screen
	// input 10 to display a blank
// output: a binary representation of the LEDs that need to be turned on, A-F
// ex: to print 4, output is 0b1100110

int leds2pins(int leds, int digit, int cathode);
// input: LEDs to turn on (leds), the digit to turn on,
	   // and which cathode is currently connected
// output: a binary representation of which pins (1-14) that need to be turned on
// for digit 1, the pins are: AM, PM, 1C, 1B, UC/LC. AM is most significant in 0b1011

int pow2(int power);
// Output is 2 raised to the power of the input
// Only works for integer powers greater than or equal to 1




#endif