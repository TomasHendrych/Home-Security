#include <Arduino.h>
#include <LiquidMenu.h>
#include <LiquidCrystal.h>

const byte LCD_RS = 1;
const byte LCD_E = 26;
const byte LCD_D4 = 25;
const byte LCD_D5 = 17;
const byte LCD_D6 = 16;
const byte LCD_D7 = 27;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

const byte analogPin = 34;
unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

unsigned int period_check = 1000;
unsigned long lastMs_check = 0;

unsigned int period_nextScreen = 5000;
unsigned long lastMs_nextScreen = 0;

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(3, 1, "Hello Menu");

LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
LiquidScreen secondary_screen(analogReading_line);

LiquidMenu menu(lcd);

void setup() {
  Serial.begin(250000);

  pinMode(analogPin, INPUT);

  lcd.begin(16, 2);

  menu.add_screen(welcome_screen);
  menu.add_screen(secondary_screen);
}

void loop() {
  if (millis() - lastMs_check > period_check) {
    lastMs_check = millis();
    analogReading = analogRead(analogPin);

    if (analogReading != lastAnalogReading) {
      lastAnalogReading = analogReading;
      menu.update();
    }
  }
  
  if (millis() - lastMs_nextScreen > period_nextScreen) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
