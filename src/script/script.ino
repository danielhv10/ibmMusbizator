#include <PS2Keyboard.h>
#include "usbHIDCharacters.h"



uint8_t buf[8] = { 
  0 }; 	/* Keyboard report buffer */
  
uint8_t  keyboardInput[2];
uint8_t  lastkeyboardPressed;

const int DataPin = 2;
const int IRQpin =  3;
const int ledPin = 13;
const int ledPinPressed = 12;

PS2Keyboard keyboard;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinPressed, OUTPUT);

  //Serial.println("Keyboard Test:");
}

void loop() {
    
  if (keyboard.available()) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin, LOW);
      keyboard.read(keyboardInput);// read the next key  9a  
      writeBuffer(keyboardInput[0], keyboardInput[1]);
      lastkeyboardPressed = keyboardInput[1];
      delay(94);
      while(keyboard.available()){
        writeBuffer(keyboardInput[0], keyboardInput[1]);
        digitalWrite(ledPinPressed, HIGH);
                
        keyboard.read(keyboardInput);
        
        if(keyboardInput[1] == lastkeyboardPressed){
          delay(94);  
          continue;
        }
        else{
          writeBuffer(keyboardInput[0], keyboardInput[1]);
          break;
        }  
      } 
     digitalWrite(ledPinPressed, LOW); 
     digitalWrite(ledPin, LOW); 
      releaseKey();
      
  }
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

