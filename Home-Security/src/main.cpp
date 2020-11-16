#include <LiquidCrystal.h>
//#include <Wire.h>
#include <Keypad.h>
#include <Password.h>

int redLed = 32; //cervena 
int greenLed = 15; //zelena
int pir = 33; //pir sensor

Password password = Password( "2345" );

const byte ROWS = 4; // ctyri radky
const byte COLS = 4; // ctyri sloupce

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins [ROWS] = {18, 19, 23, 5};
byte colPins [COLS] = {13, 12, 14};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(1, 26, 25, 17, 16, 27); 

int alarmStatus = 0;
int zone = 0;
int alarmActive = 0;
int passwd_pos = 15;

void setup(){
  lcd.begin(16, 2);
  
  Wire.begin();
  
  displayCodeEntryScreen();

  keypad.addEventListener(keypadEvent);

  pinMode(redLed, OUTPUT); //zelena led
  pinMode(greenLed, OUTPUT); //cervena led
  pinMode(pir, INPUT); //pir
}

void loop(){

  keypad.getKey();
  if (alarmActive == 1){ 
    if (digitalRead(pir) == HIGH){
      alarmTriggered();
    }
   }
}


void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  case PRESSED:
    if (passwd_pos - 15 >= 5) { 
      return ;
    }
    lcd.setCursor((passwd_pos++),0);
    switch (eKey){
    case '#':                 //# overit heslo
      passwd_pos  = 15;
      checkPassword(); 
      break;
    case '*':                 //* resetovani pokusu
      password.reset(); 
      passwd_pos = 15;
      break;
    default: 
      password.append(eKey);
      lcd.print("*");
    }
  }
}

void alarmTriggered(){
  int expected_pos;
  int incr;
  digitalWrite(redLed, HIGH);
  
  password.reset();
  alarmStatus = 1;
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("!Intruder inside!");
  /*
  if (zone == 1)
  { 
    lcd.clear();
    lcd.print("Intruder in house!");
    delay(1000);
  }
   */
}                

// zkontroluje správnost PINu
void checkPassword(){     
  if (password.evaluate())
  {  
    if(alarmActive == 0 && alarmStatus == 0)
    {
      activate();
    } 
    else if( alarmActive == 1 || alarmStatus == 1) {
      deactivate();
    }
  } 
  else {
    invalidCode();
  }
}  

void invalidCode()    // zadal jste spatny PIN
{
  password.reset();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INVALID CODE!");
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
  delay(2000);
  digitalWrite(redLed, LOW);
  delay(1000);
  displayCodeEntryScreen();
}

void activate(){      // po zadani spravneho PINu se aktivuje bezpecnostni system
    if(digitalRead(pir) == LOW){
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(2, HIGH);
    lcd.setCursor(0,0);
    lcd.print("SYSTEM ACTIVE!"); 
    alarmActive = 1;
    password.reset();
    delay(2000);
  }
  else{
    deactivate();   // pokud PIN neni spravne tak se system deaktivuje
  }

}

void deactivate(){
  alarmStatus = 0;
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" SYSTEM DEACTIVATED!");
  alarmActive = 0;
  password.reset();
  delay(5000);

  displayCodeEntryScreen();
}

void displayCodeEntryScreen(){    // pocatecni obrazovka pro zadani PINu a aktivovani systemu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0,2);
  lcd.print("Home Security System");
}
