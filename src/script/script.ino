#include <PS2Keyboard.h>
#include "usbHIDCharacters.h"



uint8_t buf[8] = { 
  0 }; 	/* Keyboard report buffer */
  

const int DataPin = 2;
const int IRQpin =  3;
const int ledPin = 13;

PS2Keyboard keyboard;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  //Serial.println("Keyboard Test:");
}

void loop() {
    
  if (keyboard.available()) {
    
    // read the next key  
    char c = keyboard.read();
    
    digitalWrite(ledPin, HIGH);
    delay(20); 
    digitalWrite(ledPin, LOW);
    
    buf[2] = 0x0F;
    
    Serial.write(buf, 8);
    releaseKey();
    /*
    // check for some of the special key
    if (c == PS2_ENTER) {
      
      Serial.println();
    } else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } else {
    */
    
    // otherwise, just print all normal characters
    //Serial.print(c);
    
  }
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);	// Release key  
}
