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

unsigned char lbExtendedKeyPress;

unsigned char lbKeyOn_UNUSED[256];

TbKeyCode lbInkey_UNUSED;

unsigned char lbInkeyFlags;

unsigned char lbIInkey;

unsigned char lbIInkeyFlags;

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
