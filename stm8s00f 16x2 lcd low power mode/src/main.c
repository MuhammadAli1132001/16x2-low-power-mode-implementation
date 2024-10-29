#include "Arduino.h"
#include "LiquidCrystal.h"
#define POWERCONTROLEPIN PA2
LiquidCrystal (lcd,PD3,PD2,PD1,PC7,PC6,PC5,PC4);

uint8_t RemainingCapacity = 100; //Making it global so that it can access by Buzzer.
static uint8_t receivedBuffer[15];
uint8_t km_range  = 100;

void processBMSdata(uint8_t *data);

void setup()  {
  
  Serial_begin(9600);
  pinMode(POWERCONTROLEPIN, OUTPUT);
    lcd_begin(16,2);
  lcd_print_s(" Implementing ");
  lcd_setCursor(0,1);
  lcd_print_s(" low power");
  delay(5000);
  lcd_clear();
} 

void loop()  {
  receivedBuffer[0] = 0x19;
  receivedBuffer[1] = 0xfb;
  receivedBuffer[2] = 0x3A;
  receivedBuffer[3] = 0xBB;
  receivedBuffer[4] = 0x64;   
  processBMSdata(receivedBuffer);

}

void processBMSdata(uint8_t *data)
{ 
  float   current = 0;
  float   voltage = 0;
  float   power   = 0;

  /*Temporary for display*/
  uint16_t tpvoltage = (uint16_t)data[0] << 8 | data[1]; //volatge two combine byte 
  uint16_t tpcurrent = (uint16_t)data[2] << 8 | data[3]; //current two combine byte
  RemainingCapacity  = data[4];                           //Capacity one byte

  voltage = tpvoltage * 0.01f;


  current = current * 0.01f;
  power   =  voltage * current;

  /*Print Remaining Capacity*/
  lcd_print_u(RemainingCapacity);
  lcd_print_c('%');

  /*Calculate the Remaining Range in Kms*/
  lcd_setCursor(5,0);
  lcd_print_u(RemainingCapacity * ((float)km_range/100.0f));
  lcd_print_s("KM");

  lcd_setCursor(10,0);
  lcd_print_i(current);
  lcd_print_c('A');

  /*Print Voltage on the lcd*/
  lcd_setCursor(0,1);
  lcd_print_u(voltage);
  lcd_print_c('V');

  lcd_setCursor(10,1);
  lcd_print_i(power);
  lcd_print_c('W');

  digitalWrite(POWERCONTROLEPIN, !(digitalRead(POWERCONTROLEPIN)));
  delay(5000);

}