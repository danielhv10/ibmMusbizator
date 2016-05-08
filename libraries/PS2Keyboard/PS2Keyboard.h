/*
  PS2Keyboard.h - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com>, June 2010
  ** Modified for use with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

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


#ifndef PS2Keyboard_h
#define PS2Keyboard_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h" // for attachInterrupt, FALLING
#else
#include "WProgram.h"
#endif

#include "utility/int_pins.h"

// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB				9
#define PS2_ENTER			13
#define PS2_BACKSPACE			127
#define PS2_ESC				27
#define PS2_INSERT			0
#define PS2_DELETE			127
#define PS2_HOME			0
#define PS2_END				0
#define PS2_PAGEUP			25
#define PS2_PAGEDOWN			26
#define PS2_UPARROW			11
#define PS2_LEFTARROW			8
#define PS2_DOWNARROW			10
#define PS2_RIGHTARROW			21
#define PS2_F1				0
#define PS2_F2				0
#define PS2_F3				0
#define PS2_F4				0
#define PS2_F5				0
#define PS2_F6				0
#define PS2_F7				0
#define PS2_F8				0
#define PS2_F9				0
#define PS2_F10				0
#define PS2_F11				0
#define PS2_F12				0
#define PS2_SCROLL			0

#define PS2_INVERTED_EXCLAMATION	161 // ¡
#define PS2_CENT_SIGN			162 // ¢
#define PS2_POUND_SIGN			163 // £
#define PS2_CURRENCY_SIGN		164 // ¤
#define PS2_YEN_SIGN			165 // ¥
#define PS2_BROKEN_BAR                 	166 // ¦
#define PS2_SECTION_SIGN		167 // §
#define PS2_DIAERESIS			168 // ¨
#define PS2_COPYRIGHT_SIGN		169 // ©
#define PS2_FEMININE_ORDINAL		170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE	171 // «
#define PS2_NOT_SIGN			172 // ¬
#define PS2_HYPHEN			173
#define PS2_REGISTERED_SIGN		174 // ®
#define PS2_MACRON			175 // ¯
#define PS2_DEGREE_SIGN			176 // °
#define PS2_PLUS_MINUS_SIGN		177 // ±
#define PS2_SUPERSCRIPT_TWO		178 // ²
#define PS2_SUPERSCRIPT_THREE		179 // ³
#define PS2_ACUTE_ACCENT		180 // ´
#define PS2_MICRO_SIGN			181 // µ
#define PS2_PILCROW_SIGN		182 // ¶
#define PS2_MIDDLE_DOT			183 // ·
#define PS2_CEDILLA			184 // ¸
#define PS2_SUPERSCRIPT_ONE		185 // ¹
#define PS2_MASCULINE_ORDINAL		186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE	187 // »
#define PS2_FRACTION_ONE_QUARTER	188 // ¼
#define PS2_FRACTION_ONE_HALF		189 // ½
#define PS2_FRACTION_THREE_QUARTERS	190 // ¾
#define PS2_INVERTED_QUESTION MARK	191 // ¿
#define PS2_A_GRAVE			192 // À
#define PS2_A_ACUTE			193 // Á
#define PS2_A_CIRCUMFLEX		194 // Â
#define PS2_A_TILDE			195 // Ã
#define PS2_A_DIAERESIS			196 // Ä
#define PS2_A_RING_ABOVE		197 // Å
#define PS2_AE				198 // Æ
#define PS2_C_CEDILLA			199 // Ç
#define PS2_E_GRAVE			200 // È
#define PS2_E_ACUTE			201 // É
#define PS2_E_CIRCUMFLEX		202 // Ê
#define PS2_E_DIAERESIS			203 // Ë
#define PS2_I_GRAVE			204 // Ì
#define PS2_I_ACUTE			205 // Í
#define PS2_I_CIRCUMFLEX		206 // Î
#define PS2_I_DIAERESIS			207 // Ï
#define PS2_ETH				208 // Ð
#define PS2_N_TILDE			209 // Ñ
#define PS2_O_GRAVE			210 // Ò
#define PS2_O_ACUTE			211 // Ó
#define PS2_O_CIRCUMFLEX		212 // Ô
#define PS2_O_TILDE			213 // Õ
#define PS2_O_DIAERESIS			214 // Ö
#define PS2_MULTIPLICATION		215 // ×
#define PS2_O_STROKE			216 // Ø
#define PS2_U_GRAVE			217 // Ù
#define PS2_U_ACUTE			218 // Ú
#define PS2_U_CIRCUMFLEX		219 // Û
#define PS2_U_DIAERESIS			220 // Ü
#define PS2_Y_ACUTE			221 // Ý
#define PS2_THORN			222 // Þ
#define PS2_SHARP_S			223 // ß
#define PS2_a_GRAVE			224 // à
#define PS2_a_ACUTE			225 // á
#define PS2_a_CIRCUMFLEX		226 // â
#define PS2_a_TILDE			227 // ã
#define PS2_a_DIAERESIS			228 // ä
#define PS2_a_RING_ABOVE		229 // å
#define PS2_ae				230 // æ
#define PS2_c_CEDILLA			231 // ç
#define PS2_e_GRAVE			232 // è
#define PS2_e_ACUTE			233 // é
#define PS2_e_CIRCUMFLEX		234 // ê
#define PS2_e_DIAERESIS			235 // ë
#define PS2_i_GRAVE			236 // ì
#define PS2_i_ACUTE			237 // í
#define PS2_i_CIRCUMFLEX		238 // î
#define PS2_i_DIAERESIS			239 // ï
#define PS2_eth				240 // ð
#define PS2_n_TILDE			241 // ñ
#define PS2_o_GRAVE			242 // ò
#define PS2_o_ACUTE			243 // ó
#define PS2_o_CIRCUMFLEX		244 // ô
#define PS2_o_TILDE			245 // õ
#define PS2_o_DIAERESIS			246 // ö
#define PS2_DIVISION			247 // ÷
#define PS2_o_STROKE			248 // ø
#define PS2_u_GRAVE			249 // ù
#define PS2_u_ACUTE			250 // ú
#define PS2_u_CIRCUMFLEX		251 // û
#define PS2_u_DIAERESIS			252 // ü
#define PS2_y_ACUTE			253 // ý
#define PS2_thorn			254 // þ
#define PS2_y_DIAERESIS			255 // ÿ


#define PS2_KEYMAP_SIZE 136





///////////////////////////HID Values:////////////////////////////////////////////////////////////////////

#define MODIFIER_NONE          byte((0))
#define MODIFIER_CONTROL_LEFT  byte((1<<0))
#define MODIFIER_SHIFT_LEFT    byte((1<<1))
#define MODIFIER_ALT_LEFT      byte((1<<2))
#define MODIFIER_GUI_LEFT      byte((1<<3))
#define MODIFIER_CONTROL_RIGHT byte((1<<4))
#define MODIFIER_SHIFT_RIGHT   byte((1<<5))
#define MODIFIER_ALT_RIGHT     byte((1<<6))
#define MODIFIER_GUI_RIGHT     byte((1<<7))

//Keys
#define KEY_NONE               byte(0x00)
#define KEY_A                  byte(0x04)
#define KEY_B                  byte(0x05)
#define KEY_C                  byte(0x06)
#define KEY_D                  byte(0x07)
#define KEY_E                  byte(0x08)
#define KEY_F                  byte(0x09)
#define KEY_G                  byte(0x0A)
#define KEY_H                  byte(0x0B)
#define KEY_I                  byte(0x0C)
#define KEY_J                  byte(0x0D)
#define KEY_K                  byte(0x0E)
#define KEY_L                  byte(0x0F)
#define KEY_M                  byte(0x10)
#define KEY_N                  byte(0x11)
#define KEY_O                  byte(0x12)
#define KEY_P                  byte(0x13)
#define KEY_Q                  byte(0x14)
#define KEY_R                  byte(0x15)
#define KEY_S                  byte(0x16)
#define KEY_T                  byte(0x17)
#define KEY_U                  byte(0x18)
#define KEY_V                  byte(0x19)
#define KEY_W                  byte(0x1A)
#define KEY_X                  byte(0x1B)
#define KEY_Y                  byte(0x1C)
#define KEY_Z                  byte(0x1D)
#define KEY_1                  byte(0x1E)
#define KEY_2                  byte(0x1F)
#define KEY_3                  byte(0x20)
#define KEY_4                  byte(0x21)
#define KEY_5                  byte(0x22)
#define KEY_6                  byte(0x23)
#define KEY_7                  byte(0x24)
#define KEY_8                  byte(0x25)
#define KEY_9                  byte(0x26)
#define KEY_0                  byte(0x27)
#define KEY_RETURN             byte(0x28)
#define KEY_ESCAPE             byte(0x29)
#define KEY_BACKSPACE          byte(0x2A)
#define KEY_TAB                byte(0x2B)
#define KEY_SPACE              byte(0x2C)
#define KEY_MINUS              byte(0x2D)
#define KEY_EQUAL              byte(0x2E)
#define KEY_BRACKET_LEFT       byte(0x2F)
#define KEY_BRACKET_RIGHT      byte(0x30)
#define KEY_BACKSLASH          byte(0x31)
#define KEY_EUROPE_1           byte(0x32)
#define KEY_SEMICOLON          byte(0x33)
#define KEY_APOSTROPHE         byte(0x34)
#define KEY_GRAVE              byte(0x35)
#define KEY_COMMA              byte(0x36)
#define KEY_PERIOD             byte(0x37)
#define KEY_SLASH              byte(0x38)
#define KEY_CAPS_LOCK          byte(0x39)
#define KEY_F1                 byte(0x3A)
#define KEY_F2                 byte(0x3B)
#define KEY_F3                 byte(0x3C)
#define KEY_F4                 byte(0x3D)
#define KEY_F5                 byte(0x3E)
#define KEY_F6                 byte(0x3F)
#define KEY_F7                 byte(0x40)
#define KEY_F8                 byte(0x41)
#define KEY_F9                 byte(0x42)
#define KEY_F10                byte(0x43)
#define KEY_F11                byte(0x44)
#define KEY_F12                byte(0x45)
#define KEY_PRINT_SCREEN       byte(0x46)
#define KEY_SCROLL_LOCK        byte(0x47)
#define KEY_PAUSE              byte(0x48)
#define KEY_INSERT             byte(0x49)
#define KEY_HOME               byte(0x4A)
#define KEY_PAGE_UP            byte(0x4B)
#define KEY_DELETE             byte(0x4C)
#define KEY_END                byte(0x4D)
#define KEY_PAGE_DOWN          byte(0x4E)
#define KEY_ARROW_RIGHT        byte(0x4F)
#define KEY_ARROW_LEFT         byte(0x50)
#define KEY_ARROW_DOWN         byte(0x51)
#define KEY_ARROW_UP           byte(0x52)
#define KEY_NUM_LOCK           byte(0x53)
#define KEY_KEYPAD_DIVIDE      byte(0x54)
#define KEY_KEYPAD_MULTIPLY    byte(0x55)
#define KEY_KEYPAD_SUBTRACT    byte(0x56)
#define KEY_KEYPAD_ADD         byte(0x57)
#define KEY_KEYPAD_ENTER       byte(0x58)
#define KEY_KEYPAD_1           byte(0x59)
#define KEY_KEYPAD_2           byte(0x5A)
#define KEY_KEYPAD_3           byte(0x5B)
#define KEY_KEYPAD_4           byte(0x5C)
#define KEY_KEYPAD_5           byte(0x5D)
#define KEY_KEYPAD_6           byte(0x5E)
#define KEY_KEYPAD_7           byte(0x5F)
#define KEY_KEYPAD_8           byte(0x60)
#define KEY_KEYPAD_9           byte(0x61)
#define KEY_KEYPAD_0           byte(0x62)
#define KEY_KEYPAD_DECIMAL     byte(0x63)
#define KEY_EUROPE_2           byte(0x64)
#define KEY_APPLICATION        byte(0x65)
#define KEY_POWER              byte(0x66)
#define KEY_KEYPAD_EQUAL       byte(0x67)
#define KEY_F13                byte(0x68)
#define KEY_F14                byte(0x69)
#define KEY_F15                byte(0x6A)
#define KEY_CONTROL_LEFT       byte(0xE0)
#define KEY_SHIFT_LEFT         byte(0xE1)
#define KEY_ALT_LEFT           byte(0xE2)
#define KEY_GUI_LEFT           byte(0xE3)
#define KEY_CONTROL_RIGHT      byte(0xE4)
#define KEY_SHIFT_RIGHT        byte(0xE5)
#define KEY_ALT_RIGHT          byte(0xE6)
#define KEY_GUI_RIGHT          byte(0xE7)

//////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	uint8_t noshift[PS2_KEYMAP_SIZE];
	uint8_t shift[PS2_KEYMAP_SIZE];
	uint8_t uses_altgr;
	uint8_t altgr[PS2_KEYMAP_SIZE];
} PS2Keymap_t;


typedef struct {
	uint8_t noshift[PS2_KEYMAP_SIZE][2];
	uint8_t shift[PS2_KEYMAP_SIZE][2];
	uint8_t uses_altgr;
	uint8_t altgr[PS2_KEYMAP_SIZE][2];
} PS2Keymap_t_Buffer;


extern const PROGMEM PS2Keymap_t PS2Keymap_US;
extern const PROGMEM PS2Keymap_t PS2Keymap_German;
extern const PROGMEM PS2Keymap_t PS2Keymap_French;
extern const PROGMEM PS2Keymap_t PS2Keymap_ES;
extern const PROGMEM PS2Keymap_t PS2Keymap_ES_Buffer;

extern const  uint8_t keymap_ES[PS2_KEYMAP_SIZE];

/**
 * Purpose: Provides an easy access to PS2 keyboards
 * Author:  Christian Weichel
 */
class PS2Keyboard {
  public:
  	/**
  	 * This constructor does basically nothing. Please call the begin(int,int)
  	 * method before using any other method of this class.
  	 */
    PS2Keyboard();
    
    /**
     * Starts the keyboard "service" by registering the external interrupt.
     * setting the pin modes correctly and driving those needed to high.
     * The propably best place to call this method is in the setup routine.
     */
    static void begin(uint8_t dataPin, uint8_t irq_pin, const uint8_t * map = keymap_ES);

	static void reset();
    /**
     * Returns true if there is a char to be read, false if not.
     */
    static bool available();

    /**
     * Returns the char last read from the keyboard.
     * If there is no char availble, -1 is returned.
     */
    static void read(uint8_t * result);
};

#endif
