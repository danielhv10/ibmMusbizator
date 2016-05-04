#include <PS2Keyboard.h>
#include "usbHIDCharacters.h"



uint8_t buf[8] = { 
  0 }; 	/* Keyboard report buffer */
  
uint8_t  keyboardInput[2];

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
      digitalWrite(ledPin, HIGH);
      delay(20); 
      digitalWrite(ledPin, LOW);
      keyboard.read(keyboardInput);// read the next key  9a  
      writeBuffer(keyboardInput[0], keyboardInput[1]);
      releaseKey();
      keyboard.reset();
        
    
  }
}

 void keyCommand(uint8_t modifiers, uint8_t keycode1) { // modified from the bluefruit tutorial, defaults values fail to compile in the Arduino IDE 
    // only interested in two key commands for now anyway
    //Serial.write(0xFD);       // our command
    Serial.write(modifiers);  // modifier!
    Serial.write((byte)0x00); // 0x00
    Serial.write(keycode1);   // key code #1
    Serial.write((byte)0x00); // key code #2
    Serial.write((byte)0x00); // key code #3
    Serial.write((byte)0x00); // key code #4
    Serial.write((byte)0x00); // key code #5
    Serial.write((byte)0x00); // key code #6
  }


void writeBuffer(uint8_t modifiers, uint8_t keycode1){
  buf[0] = modifiers;
  buf[2] = keycode1;
  Serial.write(buf, 8);
}


void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);	// Release key  
}

