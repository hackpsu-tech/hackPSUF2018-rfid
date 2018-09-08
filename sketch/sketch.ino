//#include "hackPSUrfid.h"
#include "hackPSUstatemachine.h"
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
using namespace hackPSU;

#define clearSR() shiftOut(KPD_SIG, KPD_CLK, MSBFIRST, B00000000);

// RFID Pins
#define RFID_RST 2          // SDD2 Configurable, see typical pin layout above
#define RFID_SS  15         // SDD3 Configurable, see typical pin layout above

// LCD Pins (I2C pins)
#define LCD_SCL  5
#define LCD_SDA  4

// Keypad pins
#define KPD_SRC  A0
#define KPD_CLK  D0
#define KPD_SIG  D3

char getKeyPress(int, long, int);
char getUniqueKey(int);
char readKeypad(int);
void toggle(bool, int = 10);

char keys[4][4] = {  {'1', '2', '3', 'A'},
                     {'4', '5', '6', 'B'},
                     {'7', '8', '9', 'C'},
                     {'*', '0', '#', 'D'} };

const byte I2C_ADDRESSES[] = {0x27, 0x3f};
LiquidCrystal_I2C *lcd;
MFRC522 mfrc522(RFID_SS, RFID_RST);  // Create MFRC522 instance

//Scanner scanner(RFID_SS, RFID_RST);
StateMachine fsm();

void setup() {

  Serial.begin(9600);
  while(!Serial);
  Serial.println("Begin of program");
  
  pinMode(KPD_CLK, OUTPUT);
  pinMode(KPD_SIG, OUTPUT);
  pinMode(KPD_SRC, INPUT);
  clearSR();

  Wire.begin();
  for(byte i : I2C_ADDRESSES){
    Wire.beginTransmission(i);
    if(Wire.endTransmission() == 0){
      lcd = new LiquidCrystal_I2C(i, 16, 2);
//      Serial.print("I2C LCD found at 0x" + i < 16 ? "0":"");
//      Serial.println(i, HEX);
      break;
    }
  }
  
  lcd->begin();
  lcd->clear();
  lcd->backlight();
  lcd->setCursor(0,0);
  lcd->print("Hello!");
  lcd->setCursor(0,1);
  lcd->print("Scan wristband");

  
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  
  
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("UID:");
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  
  lcd->setCursor(0,1);
  for(int i = 0; i < 8; i++){
    char key = getUniqueKey(A0);
    if(key != 'u') lcd->print(key);
    else i--;
  }
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("Hello!");
  lcd->setCursor(0,1);
  lcd->print("Scan wristband");
}

void changeState(){
  
}
/**
 * Input: Analog pin to read value
 */
char readKeypad(int pin){
  char key = 'x'; // x is the default return value, only returned if no keypress is detected
  int i = 0;

  for(int i = 0; i < 4 && key == 'x'; i++){
    toggle(i == 0, 4);
    int tmp = analogRead(pin);
    if( tmp > 800 ) key = keys[0][3-i];
    else if( tmp > 450 ) key = keys[1][3-i];
    else if( tmp > 320 ) key = keys[2][3-i];
    else if( tmp > 50)  key = keys[3][3-i];
    /*
    if( tmp > 800 ) key = keys[0][4-i];      // 1024
    else if( tmp > 450 ) key = keys[1][4-i]; // 535, 440
    else if( tmp > 320 ) key = keys[2][4-i]; // 355, 275
    else if( tmp > 50 )  key = keys[3][4-i]; // 368, 200
    */
    //Serial.println(String(tmp) + " - " + key);
  }
  clearSR();
  return key;
}

void toggle(bool data, int space) {
  digitalWrite(KPD_SIG, data ? HIGH : LOW);
  digitalWrite(KPD_CLK, HIGH);
  delay(space / 2);
  digitalWrite(KPD_CLK, LOW);
  delay(space / 2);
}

char getUniqueKey(int pin = A0){
  char val = getKeyPress(1, 500, pin);
  if(val != 'u'){
    Serial.print(val);
    while(getKeyPress(1, 250, pin) == val) Serial.print('.');
    Serial.print('\n');
  }
  return(val);
}

#define SPACE 5
char getKeyPress(int stable = 3, long timeout = 1500, int pin = A0) {
  char val;
  unsigned long start = millis();
  
  val = readKeypad(pin);
  delay(SPACE);
  int i;
  for( i = 0; i <= stable && start + timeout > millis(); i++) {
    char temp = readKeypad(pin);
    //if(temp != 'x') Serial.print(temp);
    if( i == stable && val != 'x') {
      //Serial.print("Time = " + String(millis() - start));
      return(val);   
    } else if( temp != val ) {
      val = temp;
      i = 0;
    }  
    delay(SPACE);
  }
  return('u');
}



// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    lcd->print(buffer[i] < 0x10 ? " 0" : " ");
    lcd->print(buffer[i], HEX);
  }
}
