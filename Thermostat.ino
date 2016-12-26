// INCLUDES
    #include <LiquidCrystal.h>
    #include <OneWire.h>    // required for thermistor
    #include <DallasTemperature.h>    // required for thermistor
    #include <TimerOne.h>    // required for timer
// DEFINES
    #define THERMO_DATA_PIN 11   // this goes to the thermistor 
    #define DESIRED_TEMP_PIN A0  // this goes to the potentiometer to get the 
    #define FURNACE_PIN 12

    unsigned int t;        // time in seconds
    char amPm;             // 0 if am, 1 if pm
  // This function calls out the pins for the display: LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
  LiquidCrystal lcd(5,6,7,8,9,10);
  // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  OneWire oneWire(THERMO_DATA_PIN);
  // Pass our oneWire reference to Dallas Temperature. 
  DallasTemperature sensors(&oneWire);
  
void setup(){
  // LCD Setup
      lcd.begin(16,2); // This function sets up the lcd screen. The two numbers are the number of columns and rows
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(13,0); 
      lcd.print("OFF");
  // Start up the thermometer library
      sensors.begin();
  // Set up pins
      pinMode(THERMO_DATA_PIN,OUTPUT);
      pinMode(FURNACE_PIN,OUTPUT);
      Serial.begin(9600);
// Set Up Timer 1
    TCCR1A = 0x00; // Want bits 1:0 to be 00, they're WGM3 1:0, and everything else too
    TCCR1B = 0x0C; // Bits 7:6 have to do with input capture, 5 is reserved, 4:3 are waveform generation
    // Bits 4:3 are WGM3 3:2, so we want them to be 0b01, b/c WGMn3:0 = 0b0100, table 20-6 page 171,
    //   because this is CTC mode which clears the counter once it gets to OCR3A
    // Bits 2:0 are the prescalar, p 173 table 20-7
    // I set it to 256 with 0b0100
    OCR1A = (0x0271)*50; // Want the output compare register to be 625*50 to be 1Hz at oscillator of 8MHz
    // 16 MHz, 16e6/256 = 62500
    TIMSK1 = 0b0010; // bits 3:1 enable the output compare C:A respectively match interrupt, 0 is overflow
      t = 12*3600 + 40*60;// set time
      amPm = 1;
  } //setup
  
// declare variables
  float Troom;
  int tooCold, tooHot, Tdes, furnOn;
  
void loop() {
  
  
  // Read in the temperature
  sensors.requestTemperatures(); // Send the command to get temperatures
  Troom = sensors.getTempCByIndex(0)*9/5+32; 
  // read in the desired temperature
  Tdes = (int) analogRead(DESIRED_TEMP_PIN)*30/1023+50;

  // print out both temperatures
  lcd.setCursor(0,0); // print the word want at 0,0
  lcd.print("Want: ");
  lcd.print(Tdes);
  lcd.setCursor(0,1); // print the word is at 0,1
  lcd.print("Is: ");
  lcd.print(round(Troom));
  
  if(Troom>(Tdes+2)){tooCold = 0; tooHot = tooHot+1; }
    else if (Troom < (Tdes-2)) { tooHot = 0; tooCold = tooCold + 1;}
    else { tooHot = 0; tooCold = 0;}
  // Display the time
  lcd.setCursor(9+((t/3600)<10),1); 
  lcd.print( (int) floor(t/3600));
  lcd.print(":");
  if(((t/60)%60)<10){lcd.print(" ");}
  lcd.print( (int) floor((t/60)%60)); // print minutes
  if(amPm){lcd.print("pm");}else{lcd.print("am");}
  if(t>(13*3600-1)){amPm = -amPm+1; // switch AM/PM
    t = t%(12*3600)+3600; // reset after 12 hours, add an hour
    } // if
  
      
  if(tooCold>20) {
    digitalWrite(FURNACE_PIN,HIGH);
    lcd.setCursor(13,0); // print the desired temperature at 9,0
    lcd.print("   ");
    lcd.setCursor(13,0); // print the desired temperature at 9,0
    lcd.print(" ON");
    furnOn = 1;
    tooCold = 21;
    }
    else if((tooCold<20) & (furnOn = 0)) {
      digitalWrite(FURNACE_PIN,LOW);
      lcd.setCursor(13,0); // print the desired temperature at 9,0
      lcd.print("   ");
      lcd.setCursor(13,0); // print the desired temperature at 9,0
      lcd.print("OFF");
      furnOn = 0;
    }
    else if((tooHot>20) & (furnOn = 1)) {
      digitalWrite(FURNACE_PIN,LOW);
      lcd.setCursor(13,0); // print the desired temperature at 9,0
      lcd.print("   ");
      lcd.setCursor(13,0); // print the desired temperature at 9,0
      lcd.print("OFF");
      furnOn = 0;
    }
  } //loop


ISR(TIMER1_COMPA_vect){
    t++;
}
