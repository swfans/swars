/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gkeybd.cpp
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfkeybd.h"

char lbInkeyToAscii[] = {
  '\x00','\x00',   '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   '0',   '-',   '=','\x08',  '\t',
     'q',   'w',   'e',   'r',   't',   'y',   'u',   'i',   'o',   'p',   '[',   ']','\x00','\x00',   'a',   's',
     'd',   'f',   'g',   'h',   'j',   'k',   'l',   ';',  '\'',   '`','\x00',   '#',   'z',   'x',   'c',   'v',
     'b',   'n',   'm',   ',',   '.',   '/','\x00',   '*','\x00',   ' ','\x00','\x00','\x00','\x00','\x00','\x00',
  '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',   '-','\x00','\x00','\x00',   '+','\x00',
  '\x00','\x00','\x00','\x00','\x00','\x00',  '\\','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',
     '/','\x00','\x00',   '(',   ')',   '/',   '*','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',
  '\x00',   '.','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',
};

char lbInkeyToAsciiShift[] = {
  '\x00','\x00',   '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   '0',   '-',   '=','\x08',  '\t',
     'Q',   'W',   'E',   'R',   'T',   'Y',   'U',   'I',   'O',   'P',   '{',   '}','\x00','\x00',   'A',   'S',
     'D',   'F',   'G',   'H',   'J',   'K',   'L',   ':',   '@',   '~','\x00',   '~',   'Z',   'X',   'C',   'V',
     'B',   'N',   'M',   '<',   '>',   '?','\x00',   '*','\x00',   ' ','\x00','\x00','\x00','\x00','\x00','\x00',
  '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',   '-','\x00','\x00','\x00',   '+','\x00',
  '\x00','\x00','\x00','\x00','\x00','\x00',   '|','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',
     '/','\x00','\x00',   '(',   ')',   '/',   '*','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',
  '\x00',   '.','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00',
};

const ubyte lbAsciiToInkey[] = {
  KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,
  KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,
  KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,
  KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,KC_UNASSIGNED,
       KC_SPACE,         KC_1,         KC_2, KC_BACKSLASH,         KC_4,         KC_5,         KC_7,KC_APOSTROPHE,
           KC_9,         KC_0,         KC_8,       KC_ADD,     KC_COMMA,     KC_MINUS,    KC_PERIOD,     KC_SLASH,
           KC_0,         KC_1,         KC_2,         KC_3,         KC_4,         KC_5,         KC_6,         KC_7,
           KC_8,         KC_9, KC_SEMICOLON, KC_SEMICOLON,     KC_COMMA,    KC_EQUALS,    KC_PERIOD,     KC_SLASH,
  KC_APOSTROPHE,         KC_A,         KC_B,         KC_C,         KC_D,         KC_E,         KC_F,         KC_G,
           KC_H,         KC_I,         KC_J,         KC_K,         KC_L,         KC_M,         KC_N,         KC_O,
           KC_P,         KC_Q,         KC_R,         KC_S,         KC_T,         KC_U,         KC_V,         KC_W,
           KC_X,         KC_Y,         KC_Z,  KC_LBRACKET,   KC_OEM_102,  KC_RBRACKET,         KC_6,     KC_MINUS,
       KC_GRAVE,         KC_A,         KC_B,         KC_C,         KC_D,         KC_E,         KC_F,         KC_G,
           KC_H,         KC_I,         KC_J,         KC_K,         KC_L,         KC_M,         KC_N,         KC_O,
           KC_P,         KC_Q,         KC_R,         KC_S,         KC_T,         KC_U,         KC_V,         KC_W,
           KC_X,         KC_Y,         KC_Z,  KC_LBRACKET,   KC_OEM_102,  KC_RBRACKET, KC_BACKSLASH,KC_UNASSIGNED,
};

const char *lbKeyNames[] = {
       "ESCAPE",          "1",          "2",          "3",          "4",          "5",          "6",          "7",
            "8",          "9",          "0",          "-",          "=",  "BACKSPACE",        "TAB",          "Q",
            "W",          "E",          "R",          "T",          "Y",          "U",          "I",          "O",
            "P",          "[",          "]",     "RETURN",  "LEFT CTRL",          "A",          "S",          "D",
            "F",          "G",          "H",          "J",          "K",          "L",          ";",          "'",
            "`", "LEFT SHIFT",          "#",          "Z",          "X",          "C",          "V",          "B",
            "N",          "M",          ",",          ".",          "/","RIGHT SHIFT",       "KP *",   "LEFT ALT",
        "SPACE",   "CAPSLOCK",         "F1",         "F2",         "F3",         "F4",         "F5",         "F6",
           "F7",         "F8",         "F9",        "F10",    "NUMLOCK",    "SCRLOCK",       "KP 7",       "KP 8",
         "KP 9",       "KP -",       "KP 4",       "KP 5",       "KP 6",       "KP +",       "KP 1",       "KP 2",
         "KP 3",       "KP 0",       "KP .",         NULL,         NULL,         "\\",        "F11",        "F12",
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,      "ENTER", "RIGHT CTRL",         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,       "KP /",         NULL,  "PRINT SCR",  "RIGHT ALT",
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,
           NULL,         NULL,         NULL,         NULL,         NULL,         NULL,       "HOME",         "UP",
         "PGUP",         NULL,       "LEFT",         NULL,      "RIGHT",         NULL,        "END",       "DOWN",
         "PGDN",     "INSERT",     "DELETE",         NULL,         NULL,         NULL,         NULL,         NULL,
};

unsigned char lbExtendedKeyPress;

unsigned char lbKeyOn[256];

TbKeyCode lbInkey;

unsigned char lbInkeyFlags;

unsigned char lbShift;

unsigned char lbIInkey;

unsigned char lbIInkeyFlags;

ulong lbInkey_prefixed;

int lbKeyboardLang;

void LbKeyboardSetLanguage(int lngnum)
{
    lbKeyboardLang = lngnum;
}

TbBool LbKeyCodeValid(TbKeyCode key)
{
    if (key <= KC_UNASSIGNED)
        return false;
    if (key > KC_WAKE) // last key in enumeration - update if enumeration is changed
        return false;
    return true;
}

/******************************************************************************/
