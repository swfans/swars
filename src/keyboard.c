/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file keyboard.c
 *     Buffered handler for keystrokes.
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
#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "bfkeybd.h"
#include "game.h"

#define KEYBOARD_BUFFER_SIZE 16

#pragma pack(1)

ushort kbkeys[GKey_KEYS_COUNT] = {
    KC_UNASSIGNED, KC_LCONTROL, KC_LALT, KC_DELETE,
    KC_PGDOWN, KC_TAB, KC_RCONTROL, KC_OEM_102,
    KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,
    KC_HOME, KC_END, KC_D, KC_Z,
    KC_INSERT, KC_PGUP, KC_P, KC_K,
    KC_1, KC_2, KC_3, KC_4,
};

ushort jskeys[GKey_KEYS_COUNT] = {
    0, 1, 2, 0x10,
    0x20, 4, 8, 0,
    0, 0, 0, 0,
    0, 0, 0x0F, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

ulong buffered_keys[KEYBOARD_BUFFER_SIZE];
ulong buffered_keys_read_index;
ulong buffered_keys_write_index;

#pragma pack()

ubyte is_key_pressed(TbKeyCode key, TbKeyMods kmodif)
{
    if ((kmodif == KMod_DONTCARE) || (kmodif == lbShift))
        return lbKeyOn[key];
    return 0;
}

void clear_key_pressed(TbKeyCode key)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
    if (key >= sizeof(lbKeyOn))
        return;
#pragma GCC diagnostic pop
    lbKeyOn[key] = 0;
    if (key == lbInkey) {
        lbInkey = KC_UNASSIGNED;
    }
}

static void add_key_to_buffer(ubyte key)
{
    ulong new_write_index;

    buffered_keys[buffered_keys_write_index] = key;

    new_write_index = (buffered_keys_write_index + 1) % KEYBOARD_BUFFER_SIZE;

    if (new_write_index != buffered_keys_read_index)
        buffered_keys_write_index = new_write_index;
}

ulong next_buffered_key(void)
{
    ulong key;

    if (buffered_keys_read_index == buffered_keys_write_index)
        return 0;

    key = buffered_keys[buffered_keys_read_index];
    buffered_keys_read_index
      = (buffered_keys_read_index + 1) % KEYBOARD_BUFFER_SIZE;

  return key;
}

void reset_buffered_keys(void)
{
    buffered_keys_read_index  = 0;
    buffered_keys_write_index = 0;
}

TbResult handle_custom_key_press(TbKeyAction action, TbKeyCode code)
{
    if (action != KActn_KEYDOWN)
        return false;

    if (lbKeyOn[KC_RCONTROL] || lbKeyOn[KC_LCONTROL])
    {
        if (code == KC_Q)
        {
            game_quit();
            return Lb_SUCCESS;
        }
    }

    return Lb_OK;
}

TbResult KEventBufferedKeysUpdate(TbKeyAction action, TbKeyCode code)
{
    if (action == KActn_KEYDOWN)
    {
        if (!lbInkey_prefixed)
            add_key_to_buffer(code);
    }

    return handle_custom_key_press(action, code);
}

void init_buffered_keys(void)
{
    LbKeyboardCustomHandler(KEventBufferedKeysUpdate);
}
/******************************************************************************/
