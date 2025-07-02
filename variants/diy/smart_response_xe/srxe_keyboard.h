/* ************************************************************************************
 * File:    keyboard.h
 * Date:    2021.08.12
 * Author:  Bradan Lane Studio
 *
 * This content may be redistributed and/or modified as outlined under the MIT License
 *
 * ************************************************************************************/

/* ---

## Keyboard
**Keyboard input**

The SRXE keyboard has three mode - **normal**, **shift**, and **symbol**.
The keyboard also includes the ten soft menu keys. These are located five on either side of the LCD screen,
and the is a four-way button in the lower right.

The original keyboard code from **fdufnews** have been extended with **shift** and **symbol** menus as well
as mappings for most available keys. It is strongly recommended to use the key definitions to avoid confusion.
The mapping of ENTER and DEL have been restored.

**FYI:** The ENTER key is not marked on the SRXE keyboard.
It is located in the center of the 4-way navigation pad.
Pressing the center of the NAV pad will generate the ENTER key.

--------------------------------------------------------------------------
--- */

#ifndef __SRXE_KEYBOARD_
#define __SRXE_KEYBOARD_

#include "srxe_common.h"

#define UPPERCASE(key) ((((key) >= 'a') && ((key) <= 'z')) ? ((key) - 'a' + 'A') : (key))
#define ISLETTER(key) (((((key) >= 'a') && ((key) <= 'z')) || (((key) >= 'A') && ((key) <= 'Z'))) ? true : false)
#define ISDIGIT(key) ((((key) >= '0') && ((key) <= '9')) ? true : false)
#define ISPRINTABLE(key) ((((key) >= ' ') && ((key) <= '~')) ? true : false)

// --------------------------------------------------------------------------------------------------------
// Keyboard
// --------------------------------------------------------------------------------------------------------

// Keyboard info
#define ROWS 6
#define COLS 10

/* ---
There are definitions for the various special keys. They are:
```C
*/
#define KEY_NOP 0
#define KEY_SHIFT KEY_NOP
#define KEY_SYM KEY_NOP

#define KEY_DEL 0x08
#define KEY_ENTER 0x0D
#define KEY_ESC 0x1B

#define KEY_MENU1 0x80
#define KEY_MENU2 0x81
#define KEY_MENU3 0x82
#define KEY_MENU4 0x83
#define KEY_MENU5 0x84
#define KEY_MENU6 0x85
#define KEY_MENU7 0x86
#define KEY_MENU8 0x87
#define KEY_MENU9 0x88
#define KEY_MENU10 0x89
#define KEY_LEFT 0x8A
#define KEY_RIGHT 0x8B
#define KEY_UP 0x8C
#define KEY_DOWN 0x8D

#define KEY_MENU1SH 0x90
#define KEY_MENU2SH 0x91
#define KEY_MENU3SH 0x92
#define KEY_MENU4SH 0x93
#define KEY_MENU5SH 0x94
#define KEY_MENU6SH 0x95
#define KEY_MENU7SH 0x96
#define KEY_MENU8SH 0x97
#define KEY_MENU9SH 0x98
#define KEY_MENU10SH 0x99
#define KEY_PGUP 0x9A
#define KEY_PGDN 0x9B
#define KEY_HOME 0x9C
#define KEY_END 0x9D

#define KEY_MENU1SY 0xA0
#define KEY_MENU2SY 0xA1
#define KEY_MENU3SY 0xA2
#define KEY_MENU4SY 0xA3
#define KEY_MENU5SY 0xA4
#define KEY_MENU6SY 0xA5
#define KEY_MENU7SY 0xA6
#define KEY_MENU8SY 0xA7
#define KEY_MENU9SY 0xA8
#define KEY_MENU10SY 0xA9

#define KEY_FRAC 0xB0
#define KEY_ROOT 0xB1
#define KEY_EXPO 0xB2
#define KEY_ROOX 0xB3
#define KEY_BASE 0xB4
#define KEY_PI 0xB5
#define KEY_THETA 0xB6
#define KEY_DEG 0xB7
#define KEY_LE 0xB8
#define KEY_GE 0xB9

#define KEY_GRAB 0xFC    // when SCREEN_GRABBER has been defined
#define KEY_GRABON 0xFD  // when SCREEN_GRABBER has been defined
#define KEY_GRABOFF 0xFE // when SCREEN_GRABBER has been defined

#define KEY_MENU 0xFF
/*
```
--- */

// Logical Layout:
//                ROW1|ROW2|ROW3|ROW4|ROW5|ROW6|ROW7|ROW8|ROW9|ROW10
//           COL1    1|   2|   3|   4|   5|   6|   7|   8|   9|    0
//           COL2    Q|   W|   E|   R|   T|   Y|   U|   I|   O|    P
//           COL3    A|   S|   D|   F|   G|   H|   J|   K|   L|  Del
//           COL4 Shft|   Z|   X|   C|   V|   B|   N|Down|Entr|   Up
//           COL5  Sym|Frac|Root| Exp| Spc|   ,|   .|   M|Left|Right
//           COL6  SK1| SK2| SK3| SK4| SK5| SK6| SK7| SK8| SK9| SK10
//
// SK# are screen keys with top to bottom 1-5 on left and 6-10 on right

/* ---
There are keyboard maps for normal, with-shift, and with-symbol keys. They are:
```C
*/
uint8_t _kbd_normal_keys[] = {'1',       '2',       '3',       '4',       '5',       '6',       '7',       '8',       '9',
                              '0',       'q',       'w',       'e',       'r',       't',       'y',       'u',       'i',
                              'o',       'p',       'a',       's',       'd',       'f',       'g',       'h',       'j',
                              'k',       'l',       KEY_DEL,   KEY_SHIFT, 'z',       'x',       'c',       'v',       'b',
                              'n',       KEY_DOWN,  KEY_ENTER, KEY_UP,    KEY_SYM,   KEY_FRAC,  KEY_ROOT,  KEY_EXPO,  ' ',
                              ',',       '.',       'm',       KEY_LEFT,  KEY_RIGHT, KEY_MENU1, KEY_MENU2, KEY_MENU3, KEY_MENU4,
                              KEY_MENU5, KEY_MENU6, KEY_MENU7, KEY_MENU8, KEY_MENU9, KEY_MENU10};

uint8_t _kbd_shift_keys[] = {
    '1',         '2',         '3',         '4',         '5',         '6',         '7',         '8',         '9',
    '0',         'Q',         'W',         'E',         'R',         'T',         'Y',         'U',         'I',
    'O',         'P',         'A',         'S',         'D',         'F',         'G',         'H',         'J',
    'K',         'L',         KEY_ESC,     KEY_SHIFT,   'Z',         'X',         'C',         'V',         'B',
    'N',         KEY_PGDN,    KEY_ENTER,   KEY_PGUP,    KEY_SYM,     KEY_GRABOFF, KEY_GRAB,    KEY_GRABON,  '_',
    ',',         '.',         'M',         KEY_HOME,    KEY_END,     KEY_MENU1SH, KEY_MENU2SH, KEY_MENU3SH, KEY_MENU4SH,
    KEY_MENU5SH, KEY_MENU6SH, KEY_MENU7SH, KEY_MENU8SH, KEY_MENU9SH, KEY_MENU10SH};

uint8_t _kbd_symbol_keys[] = {
    '!',         KEY_PI,      KEY_THETA,   '$',         '%',         KEY_DEG,     '\'',        '\"',        '(',
    ')',         'Q',         'W',         'E',         'R',         'T',         'Y',         'U',         ';',
    '[',         ']',         '=',         '+',         '-',         'F',         'G',         'H',         'J',
    ':',         '?',         KEY_ESC,     KEY_SHIFT,   '*',         '/',         'C',         'V',         'B',
    KEY_LE,      KEY_PGDN,    KEY_ENTER,   KEY_PGUP,    KEY_SYM,     KEY_FRAC,    KEY_ROOX,    KEY_BASE,    KEY_MENU,
    '<',         '>',         KEY_GE,      KEY_HOME,    KEY_END,     KEY_MENU1SY, KEY_MENU2SY, KEY_MENU3SY, KEY_MENU4SY,
    KEY_MENU5SY, KEY_MENU6SY, KEY_MENU7SY, KEY_MENU8SY, KEY_MENU9SY, KEY_MENU10SY};

#endif