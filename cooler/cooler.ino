// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Stepper.h>
#include "RTClib.h"
#include <Wire.h>

// Cooler state
int state; // 0=disabled, 1=idle, 2=running, 3=error
volatile int newState = 0; // start state as disabled
const char stateNames[4][9] = {"DISABLED", "IDLE", "RUNNING", "ERROR"};

// Buttons
#define DEBOUNCE_TIME 500
volatile unsigned long lastButtonTime;
#define RESET_BUTTON(); // check for reset button press // TODO

// LEDs
#define LED_ON(pinNum) PORTC |= (0x01 << state);
#define LEDS_OFF()     PORTC &= ~(0x0F);

// LCD
#define LCD_REFRESH 60000
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // initialized w/ #s of interface pins
unsigned long nextLCDRefresh;

// DHT
#define DHT_PIN 27
#define TEMPERATURE_LIMIT 27
DHT dht(DHT_PIN, DHT11);
unsigned int temperature;

// ADC
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;

// Stepper motor
#define STEPS 100
Stepper stepper(STEPS,32,33,30,31); 
#define STEPPER_PIN 999999 //TODO 
int ventPosition = 0;

// RTC
RTC_DS1307 rtc;
DateTime now; // Current Date & Time

// DC fan motor
#define FAN_ON()  PORTA |= (1 << DD1);
#define FAN_OFF() PORTA &= (0 << DD1);

// Water sensor
#define WATER_MINIMUM 999999 // TODO
#define WATER_PIN 999999 // TODO
unsigned int waterLevel; // holds current water level

// *******MAIN FUNCTIONS*******

void setup(){  
  // Interrupts
  PCICR |= (1 << PCIE1) | (1 << PCIE2); // enable interrupts on vectors 1,2
  PCMSK2 |= 1 << PCINT17; // enable on pin A9
  PCMSK1 |= 1 << PCINT10; // enable on pin 14
  DDRK &= 0 << DD1; // set A9 to input with pullup enabled
  PORTK |= 1 << DD1;
  DDRJ &= 0 << DD1; // set 14 to input with pullup enabled
  PORTJ |= 1 << DD1;
  
  // Serial Monitor
  Serial.begin(9600);

  // LEDs
  DDRC |= 0x0F; // set pins 34-37 to output

  // Stepper motor
  stepper.setSpeed(30);

  // LCD
  lcd.begin(16, 2);  

  // RTC
  Wire.begin();
  rtc.begin();

  // DHT
  dht.begin();  

  // DC fan motor
  DDRA |= 0x06; // set pins 23,24 to output
  FAN_OFF(); // fan off

  // ADC
  //adcInit();
  
  // Initialize state
  setState(); // set starting state
}

void loop(){
  // Display temp and humidity to LCD every minute if not disabled
  if(millis() >= nextLCDRefresh){
    nextLCDRefresh += LCD_REFRESH; // set next display time
    if(state != 0){
      displayTempAndHumidity();
    }
  }

  temperature = dht.readTemperature(); // read current temperature
 
  // Temperature > threshold: turn on fan
  if(state == 1 && temperature > TEMPERATURE_LIMIT){
    newState = 2;
  }

  // Temperature < threshold: turn off fan
  else if(state == 2 && temperature < TEMPERATURE_LIMIT){
    newState = 1;
  }

  // TODO
  // Water level low: display error
  /*if(getWaterLevel() < WATER_MINIMUM){
    newState = 3;
  }*/

  // TODO
  // Reset button: begin idle
  /*bool reset = RESET_BUTTON();
  if(state == 3 && reset && getWaterLevel() > WATER_MINIMUM){
    newState = 1;
  }*/
 
  // Allow adjustments to fan position if not disabled
  if(state != 0){
    //adjustFan();
  }

  // Sets state for beginning of next iteration of the main loop
  setState();
}

// *******INTERRUPTS*******

// Start button interrupt
ISR(PCINT2_vect){
  unsigned long currentTime = millis();
  if(currentTime - lastButtonTime > DEBOUNCE_TIME){
    newState = 1;
  }
  lastButtonTime = currentTime;
}

// Stop button interrupt
ISR(PCINT1_vect){
  unsigned long currentTime = millis();
  if(currentTime - lastButtonTime > DEBOUNCE_TIME){
    newState = 0;
  }
  lastButtonTime = currentTime;
}

// *******ADDITIONAL FUNCTIONS*******

// Sets state for beginning of next iteration of the main loop
void setState(){
  // Handle state transition
  switch(newState){
    case 0: 
      lcd.clear();
      FAN_OFF() // fan off
      break;
    case 1: 
      displayTempAndHumidity();
      FAN_OFF(); // fan off
      break;
    case 2: 
      FAN_ON(); // fan on
    case 3:
      // TODO
    default: 
      break;
  }
  
  // Change cooler state
  if(newState != -1){
    changeState();
  }
}

// General state transition procedure
void changeState(){
  state = newState; // update current state
  printTime();
  Serial.print(" state is now ");
  Serial.println(stateNames[state]); 
  LEDS_OFF();
  LED_ON(state);
  newState = -1; // reset to no state transition
}

// Prints time to Serial monitor from RTC
void printTime(){
  now = rtc.now(); // get current time
  Serial.print(now.year(), DEC); 
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" at ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(":");
}

// Display temperature and humidity to LCD screen
void displayTempAndHumidity(){
  int humidity = dht.readHumidity(); // read humidity
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // Check for failed readings
  if(isnan(humidity) || isnan(temperature)){
    lcd.print("DHT sensor reading(s) failed");
  }
  
  // Display humidity and temperature on LCD 
  else{
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }
}

// TODO
// Adjust vent position
/*void adjustFan(){
  int newVentPosition = adcRead(STEPPER_PIN);
  stepper.step(newVentPosition - ventPosition);
  ventPosition = newVentPosition;
}*/

// TODO
// Read current water level
/*void getWaterLevel(){
  
}*/

// TODO
/*void adcInit()
{
  // setup the A register
  // set bit   7 to 1 to enable the ADC
  // clear bit 5 to 0 to disable the ADC trigger mode
  // clear bit 3 to 0 to disable the ADC interrupt
  // clear bit 2-0 to 0 to set prescaler selection to slow reading
  *my_ADCSRA |= 0x80;
  *my_ADCSRA &= ~(0x2F);
  
  // setup the B register
  // clear bit 3 to 0 to reset the channel and gain bits
  // clear bit 2-0 to 0 to set free running mode
  *my_ADCSRB &= 0xF0;
  
  // setup the MUX Register
  // clear bit 7 to 0 for AVCC analog reference
  // set bit   6 to 1 for AVCC analog reference
  // clear bit 5 to 0 for right adjust result
  // clear bit 4-0 to 0 to reset the channel and gain bits
  *my_ADMUX = 0x40;
}*/

//TODO
/*unsigned int adcRead(unsigned char adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX &= 0xE0;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0xF7;
  // set the channel number
  *my_ADMUX |= adc_channel_num;
  // set the channel selection bits, but remove the most significant bit (bit 3)
  *my_ADMUX = (*myADMUX && 0xF8) || (adc_channel_num && 0x07);
  // set MUX bit 5
  *my_ADCSRB |= 0x08;
 // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*myADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}*/