#include <LiquidCrystal.h>
#include <Keypad.h> 
#include <Arduino.h>

#define greenLED 15

char* password = "2345";
int pozice = 0;

const byte rows = 4; 
const byte cols = 3;

char keyMap [rows] [cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins [rows] = {18, 19, 23, 5};
byte colPins [cols] = {13, 12, 14};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);

LiquidCrystal lcd (1, 26, 25, 17, 16, 27);

int pin = 33;
int pirState = LOW;

void setup(){

  pinMode(pin, INPUT);
  
  lcd.begin(16, 2);
   //set the LED as an output
  pinMode(greenLED, OUTPUT);
  
}

void loop(){

  if(digitalRead(pin)==LOW){
    lcd.setCursor(0, 0);
    lcd.print("   ...Working...");
  }

  else{


  char whichKey = myKeypad.getKey();

  lcd.setCursor(0, 0);
  lcd.print("    Welcome");
  lcd.setCursor(0, 1);
  lcd.print(" Enter Password");

  if(whichKey == '*' || whichKey == '#'){
    pozice=0;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Invalid Key!");
    delay(1000);
    lcd.clear();
  }

  if(whichKey == password [pozice]){
    pozice ++;
  }

  if(pozice == 4){
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** Verified ***");
    delay(7000);
    lcd.clear();
  }
  delay(100);
  }
}