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

void init_buffered_keys(void);
void reset_buffered_keys(void);
ulong next_buffered_key(void);

#endif
/******************************************************************************/
