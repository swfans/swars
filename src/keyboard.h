/******************************************************************************/
// Free implementation of Bullfrog's Syndicate Wars strategy game.
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

#include <SDL.h>
#include "bftypes.h"

enum GameKeys
{
  GKey_UNKN0 = 0x0,
  GKey_FIRE = 0x1,
  GKey_CHANGE_MD_WP = 0x2,
  GKey_VIEW_SPIN_L = 0x3,
  GKey_VIEW_SPIN_R = 0x4,
  GKey_CHANGE_AGENT = 0x5,
  GKey_GOTO_POINT = 0x6,
  GKey_GROUP = 0x7,
  GKey_UP = 0x8,
  GKey_DOWN = 0x9,
  GKey_LEFT = 0xA,
  GKey_RIGHT = 0xB,
  GKey_ZOOM_IN = 0xC,
  GKey_ZOOM_OUT = 0xD,
  GKey_SELF_DESTRUCT = 0xE,
  GKey_DROP_WEAPON = 0xF,
  GKey_VIEW_TILT_U = 0x10,
  GKey_VIEW_TILT_D = 0x11,
  GKey_PAUSE = 0x12,
  GKey_KEY_CONTROL = 0x13,
  GKey_SEL_AGENT_1 = 0x14,
  GKey_SEL_AGENT_2 = 0x15,
  GKey_SEL_AGENT_3 = 0x16,
  GKey_SEL_AGENT_4 = 0x17,
};

/******************************************************************************/
extern ushort kbkeys[];
extern ushort jskeys[];

void init_buffered_keys(void);
void reset_buffered_keys(void);
ulong next_buffered_key(void);

#endif
/******************************************************************************/
