/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file keyboard.c
 *     Header file for buffered handler for keystrokes.
 * @par Purpose:
 *     Taps keyboard event handler and stores each key pressed.
 * @par Comment:
 *     When keys are handled outside the event, it is possible that some
 *     keypresses will not be registered, making typing text frustrating.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 13 Oct 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "bftypes.h"
#include "bfkeybd.h"

enum GameKeys
{
  GKey_NONE = 0,
  GKey_FIRE = 1,
  GKey_CHANGE_MD_WP = 2,
  GKey_VIEW_SPIN_L = 3,
  GKey_VIEW_SPIN_R = 4,
  GKey_CHANGE_AGENT = 5,
  GKey_GOTO_POINT = 6,
  GKey_GROUP = 7,
  GKey_UP = 8,
  GKey_DOWN = 9,
  GKey_LEFT = 10,
  GKey_RIGHT = 11,
  GKey_ZOOM_IN = 12,
  GKey_ZOOM_OUT = 13,
  GKey_SELF_DESTRUCT = 14,
  GKey_DROP_WEAPON = 15,
  GKey_VIEW_TILT_U = 16,
  GKey_VIEW_TILT_D = 17,
  GKey_PAUSE = 18,
  GKey_KEY_CONTROL = 19,
  GKey_SEL_AGENT_1 = 20,
  GKey_SEL_AGENT_2 = 21,
  GKey_SEL_AGENT_3 = 22,
  GKey_SEL_AGENT_4 = 23,
  GKey_KEYS_COUNT,
};

/******************************************************************************/
/** Array of keyboard keys bindings; uses GKey_* enum members as index.
 */
extern ushort kbkeys[GKey_KEYS_COUNT];
/** Array of joystick button bindings; uses GKey_* enum members as index.
 */
extern ushort jskeys[GKey_KEYS_COUNT];

void init_buffered_keys(void);
void reset_buffered_keys(void);
ulong next_buffered_key(void);

/**
 * Checks if a specific key is pressed.
 *
 * @param key Code of the key to check.
 * @param kmodif Key modifier flags required.
 */
ubyte is_key_pressed(TbKeyCode key, TbKeyMods kmodif);

/**
 * Clears the marking that a specific key was pressed.
 */
void clear_key_pressed(TbKeyCode key);

#endif
/******************************************************************************/
