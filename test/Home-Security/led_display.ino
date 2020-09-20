#include <LiquidCrystal.h>
#include <Arduino.h>

LiquidCrystal lcd(1, 26, 27, 16, 17, 25);

void setup() {
  pinMode(14, INPUT);
  lcd.print("SENSOR ACTIVE");
  delay(2000);
  lcd.clear();
}

void loop() {
  if(digitalRead(14)==HIGH){
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
