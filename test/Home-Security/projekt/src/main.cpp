#include <LiquidCrystal.h>
#include <Arduino.h>
#include <EEPROM.h>

LiquidCrystal lcd(1, 26, 27, 16, 17, 25);

void setup() {
  EEPROM.write(0, 1);
}

void loop() {
  if(digitalRead(15)==HIGH){
    lcd.print("PANIK!!!");
    delay(1000);
    lcd.clear();
  }
  else{
    lcd.print("Kalm");
    delay(1000);
    lcd.clear();
  }
  
}