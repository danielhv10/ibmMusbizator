/*
  PS2Keyboard.cpp - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com> 2010, 2011
  ** Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Version 2.4 (March 2013)
  - Support Teensy 3.0, Arduino Due, Arduino Leonardo & other boards
  - French keyboard layout, David Chochoi, tchoyyfr at yahoo dot fr

  Version 2.3 (October 2011)
  - Minor bugs fixed

  Version 2.2 (August 2011)
  - Support non-US keyboards - thanks to Rainer Bruch for a German keyboard :)

  Version 2.1 (May 2011)
  - timeout to recover from misaligned input
  - compatibility with Arduino "new-extension" branch
  - TODO: send function, proposed by Scott Penrose, scooterda at me dot com

  Version 2.0 (June 2010)
  - Buffering added, many scan codes can be captured without data loss
    if your sketch is busy doing other work
  - Shift keys supported, completely rewritten scan code to ascii
  - Slow linear search replaced with fast indexed table lookups
  - Support for Teensy, Arduino Mega, and Sanguino added

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "PS2Keyboard.h"

#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint8_t DataPin;
static uint8_t CharBuffer[2];
static uint8_t UTF8next[2];
static const uint8_t * keymap=NULL;
static int bareer;

// The ISR for the external interrupt
void ps2interrupt(void)
{
	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 250) {
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11) {
		uint8_t i = head + 1;
		if (i >= BUFFER_SIZE) i = 0;
		if (i != tail) {
			buffer[i] = incoming;
			head = i;
		}
		bitcount = 0;
		incoming = 0;
	}
}

static inline uint8_t get_scan_code(void)
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;
	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return c;
}

// http://www.quadibloc.com/comp/scan.htm
// http://www.computer-engineering.org/ps2keyboard/scancodes2.html

// These arrays provide a simple key map, to turn scan codes into ISO8859
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//

const PROGMEM  uint8_t keymap_ES[PS2_KEYMAP_SIZE] = {
		0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,
		0, KEY_F10, KEY_F8, KEY_F6, KEY_F4, KEY_TAB, KEY_GRAVE, 0,
		0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, KEY_Q, KEY_1, 0,
		0, 0, KEY_Z, KEY_S, KEY_A, KEY_W, KEY_2, 0,
		0, KEY_C, KEY_X, KEY_D, KEY_E, KEY_4, KEY_3, 0,
		0, KEY_SPACE, KEY_V, KEY_F, KEY_T, KEY_R, KEY_5, 0,
		0, KEY_N, KEY_B, KEY_H, KEY_G, KEY_Y, KEY_6, 0,
		0, 0, KEY_M, KEY_J, KEY_U, KEY_7, KEY_8, 0,
		0, KEY_COMMA, KEY_K, KEY_I, KEY_O, KEY_0, KEY_9, 0,
		0, KEY_PERIOD, KEY_SLASH, KEY_L, KEY_SEMICOLON, KEY_P, KEY_MINUS, 0,
		0, 0, KEY_APOSTROPHE, 0, KEY_BRACKET_LEFT, KEY_EQUAL, 0, 0,
		KEY_CAPS_LOCK /*CapsLock*/, 0 /*Rshift*/, KEY_KEYPAD_ENTER /*Enter*/, KEY_BRACKET_RIGHT, 0, KEY_BACKSLASH, 0, 0,
		0, KEY_EUROPE_2, 0, 0, 0, 0, KEY_BACKSPACE, 0,
		0, KEY_1, 0, KEY_4, KEY_7, 0, 0, 0,
		KEY_0, KEY_PERIOD, KEY_2, KEY_5, KEY_6, KEY_8, KEY_ESCAPE, 0 /*NumLock*/,
		KEY_F11, KEY_KEYPAD_ADD, KEY_3, KEY_MINUS, KEY_KEYPAD_MULTIPLY, KEY_9, KEY_SCROLL_LOCK, 0,
		0, 0, 0, KEY_F7
};


#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10

static int get_iso8859_code(uint8_t * c)
{
	static uint8_t state=0;
	uint8_t s;

	while (1) {
		s = get_scan_code();
		if (!s) return 1;
		if (s == 0xF0) {
			state |= BREAK;
		} else if (s == 0xE0) {
			state |= MODIFIER;
		} else {
			if (state & BREAK) {
				if (s == 0x12) {
					state &= ~SHIFT_L;
				} else if (s == 0x59) {
					state &= ~SHIFT_R;
				} else if (s == 0x11 /*&& (state & MODIFIER)*/) {
					state &= ~ALTGR;

				}
				// CTRL, ALT & WIN keys could be added
				// but is that really worth the overhead?
				state &= ~(BREAK | MODIFIER);
				continue;
			}
			if (s == 0x12) {
				state |= SHIFT_L;
				continue;
			} else if (s == 0x59) {
				state |= SHIFT_R;
				continue;
			} else if (s == 0x11 && (state & MODIFIER)) {
				state |= ALTGR;
			}

			if (state & MODIFIER) {
				c[0] = 0;
				switch (s) {
				  case 0x70: c[1] = KEY_INSERT;      break;
				  case 0x6C: c[1] = KEY_HOME;        break;
				  case 0x7D: c[1] = KEY_PAGE_UP;     break;
				  case 0x71: c[1] = KEY_DELETE;      break;
				  case 0x69: c[1] = KEY_END;         break;
				  case 0x7A: c[1] = KEY_PAGE_DOWN;    break;
				  case 0x75: c[1] = KEY_ARROW_UP;     break;
				  case 0x6B: c[1] = KEY_ARROW_LEFT;   break;
				  case 0x72: c[1] = KEY_ARROW_DOWN;   break;
				  case 0x74: c[1] = KEY_ARROW_RIGHT;  break;
				  //case 0x4A: c[1] = '/';             break;
				  case 0x5A: c[1] = KEY_KEYPAD_ENTER;       break;
				  default: break;
				}
			} else if ((state & ALTGR)) {
				if (s < PS2_KEYMAP_SIZE){
					c[0] = MODIFIER_ALT_RIGHT;
					c[1] = pgm_read_byte(keymap + s);
					break;
				}

			} else if (state & (SHIFT_L | SHIFT_R)) {
				if (s < PS2_KEYMAP_SIZE){
					c[0] = MODIFIER_SHIFT_LEFT;
					c[1] = pgm_read_byte(keymap + s);
					break;
				}

			} else {
				if (s < PS2_KEYMAP_SIZE){
					c[1] = pgm_read_byte(keymap + s);
					break;
				}

			}
			state &= ~(BREAK | MODIFIER);
			//if (c) return 0;
		}
	}
}

bool PS2Keyboard::available() {
	if (CharBuffer[1] || UTF8next[1]) return true;
		get_iso8859_code(CharBuffer);
	if (CharBuffer[1]) return true;
	return false;
}

void PS2Keyboard::read(uint8_t * result) {

	result[0] = UTF8next[0];
	result[1] = UTF8next[1];

	if (result[1]) {
		UTF8next[0] = 0;
		UTF8next[1] = 0;
	}
	else {
		result[0] = CharBuffer[0];
		result[1] = CharBuffer[1];
	}
	if (result[1]) {
		CharBuffer[0] = 0;
		CharBuffer[1] = 0;
	}
	else {
		get_iso8859_code(result);
	}
	if (result[1] >= 128) {
		UTF8next[0] = 0;
		UTF8next[1] = (result[1] & 0x3F) | 0x80;
		result[0] = 0;
		result[1] = ((result[1] >> 6) & 0x1F) | 0xC0;
	}

	if (result[1]){
		CharBuffer[0] = 0;
		CharBuffer[1] = 0;
		UTF8next[0] = 0;
		UTF8next[1] = 0;
	}
	//return result;

}

PS2Keyboard::PS2Keyboard() {
  // nothing to do here, begin() does it all
}

void  PS2Keyboard::reset(){
	CharBuffer[0] = 0;
	CharBuffer[1] = 0;
}

void PS2Keyboard::begin(uint8_t data_pin, uint8_t irq_pin, const uint8_t * map ) {
  uint8_t irq_num=255;

  DataPin = data_pin;
  keymap = map;

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(irq_pin, INPUT_PULLUP);
  pinMode(data_pin, INPUT_PULLUP);
#else
  pinMode(irq_pin, INPUT);
  digitalWrite(irq_pin, HIGH);
  pinMode(data_pin, INPUT);
  digitalWrite(data_pin, HIGH);
#endif

#ifdef CORE_INT_EVERY_PIN
  irq_num = irq_pin;

#else
  switch(irq_pin) {
    #ifdef CORE_INT0_PIN
    case CORE_INT0_PIN:
      irq_num = 0;
      break;
    #endif
    #ifdef CORE_INT1_PIN
    case CORE_INT1_PIN:
      irq_num = 1;
      break;
    #endif
    #ifdef CORE_INT2_PIN
    case CORE_INT2_PIN:
      irq_num = 2;
      break;
    #endif
    #ifdef CORE_INT3_PIN
    case CORE_INT3_PIN:
      irq_num = 3;
      break;
    #endif
    #ifdef CORE_INT4_PIN
    case CORE_INT4_PIN:
      irq_num = 4;
      break;
    #endif
    #ifdef CORE_INT5_PIN
    case CORE_INT5_PIN:
      irq_num = 5;
      break;
    #endif
    #ifdef CORE_INT6_PIN
    case CORE_INT6_PIN:
      irq_num = 6;
      break;
    #endif
    #ifdef CORE_INT7_PIN
    case CORE_INT7_PIN:
      irq_num = 7;
      break;
    #endif
  }
#endif

  head = 0;
  tail = 0;
  if (irq_num < 255) {
    attachInterrupt(irq_num, ps2interrupt, FALLING);
  }
}