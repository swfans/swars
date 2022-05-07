/******************************************************************************/
// Free implementation of Bullfrog's Syndicate Wars strategy game.
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

ulong buffered_keys[KEYBOARD_BUFFER_SIZE];
ulong buffered_keys_read_index;
ulong buffered_keys_write_index;

#pragma pack()

static void add_key_to_buffer(uint8_t key)
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
