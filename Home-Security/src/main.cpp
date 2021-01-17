#include <LiquidCrystal.h>
#include <Wire.h>
#include <Keypad.h>
#include <Password.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
//#include <WiFi.h>

/*
  const char* ssid = "";
  const char* password = "";

  const char* http_username = "admin";
  const char* http_password = "admin";

  const char* PARAM_INPUT_1 = "state";
*/

int redLed = 32; //cervena 
int greenLed = 15; //zelena
int pir = 33; //pir sensor
//int processor;
//int ledPin;

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

void displayCodeEntryScreen(){    // pocatecni obrazovka pro zadani PINu a aktivovani systemu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
}

void deactivate(){
  alarmStatus = 0;
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SYSTEM ");
  lcd.setCursor(0,2);
  lcd.print("DEACTIVATED");
  alarmActive = 0;
  password.reset();
  delay(5000);

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

void checkPassword(){   // zkontroluje správnost PINu
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
  digitalWrite(redLed, HIGH);
  
  password.reset();
  alarmStatus = 1;
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("!Intruder inside!");
}                

void setup(){
  lcd.begin(16, 2);
  
  Wire.begin();
  
  displayCodeEntryScreen();

  keypad.addEventListener(keypadEvent);

  pinMode(redLed, OUTPUT); //zelena led
  pinMode(greenLed, OUTPUT); //cervena led
  pinMode(pir, INPUT); //pir

  /*
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.begin();
  */
 
}

void loop(){

  keypad.getKey();
  if (alarmActive == 1){ 
    if (digitalRead(pir) == HIGH){
      alarmTriggered();
    }
   }
}