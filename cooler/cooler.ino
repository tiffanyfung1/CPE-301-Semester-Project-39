// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <dht.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //library initialized w/ the numbers of interface pins

// Main program constants and variables
#define DHT_ANALOGPIN A0
#define DHTTYPE DHT11
#define LCD_REFRESH 60
#define TEMPERATURE_LIMIT ##
dht DHT;
int state; // 0=disabled, 1=idle, 2=running, 3=error

// I/O Macros
#define LEDS_OFF()     *ledPort &= ~(0x##);
#define LED_ON(pinNum) *ledPort |= (0x01 << pinNum);

void setup(){  
  Serial.begin(9600);
  setState(0); // disabled
  resetTimer(LCD_REFRESH);
  lcd.begin(16, 2); 
}

void loop(){
  // If state != 0 & fan adjust input
    // Adjust fan position
    // Serial port: report vent position change
  
  /*if(state = 1 & getTemperature() > TEMPERATURE_LIMIT){
    setFan(1);   // fan on
    setState(2); // running
  }*/

  /*if(state = 2 & getTemperature() < TEMPERATURE_LIMIT){
    setFan(0);   // fan off
    setState(1); // idle
  }*/
}

// Change cooler state
void setState(int newState){
  state = newState;
  // Serial port: report time and newState
  LEDS_OFF();
  LED_ON(newState);
}

// Resets LCD refresh timer to 1 minute
void resetTimer(int seconds){
  
}

// Reads the current air temperature
int getTemperature(){
  
}

// Turns fan motor on and off
void setFan(int fanOn){
  // fanOn = 0 -> off, fanON = 1 -> on
}

/* Start Button Interrupt:
  // Turn on LCD
  // setState(1); // idle
*/

/* Stop Button Interrupt:
  // Turn off LCD
  // setFan(0);   // fan off
  // setState(0); // disabled
*/

/* Reset Button Interrupt:
  // If waterLevel > threshold
    // setState(1); // idle
*/

// Timer Interrupt:
 // if(state != 0){ 
    // Display humidity and temperature on LCD
    float humidity = dht.readHumidity(); // read humidity
    float tempF = dht.readTemperature(); // read temp in Farenheit
    lcd.setCursor(0, 1);
if (isnan(humidity) || isnan(tempF)) { //check for failed readings
  lcd.println("DHT sensor reading(s) failed");
} else {
  lcd.println("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.println("Temperature: ");
  lcd.print(tempF);
  lcd.print("F");
  }
  // resetTimer(LCD_REFRESH);


// NOTE: Can also be done using ADC
// Comparator Interrupt: // waterLevel < threshold
int threshVal = 0; // value holder
int sensPin = A3; //  sensor pin 

lcd.setCursor(0, 1);

threshVal = analogRead(senspin); // read from analog pin, store in threshVal 

if (thresVal<=300){
  lcd.println("Water level is too low"); // LCD error message
}
delay(1000);
  
  
  // setFan(0);   // fan off
  // setState(3); // error
