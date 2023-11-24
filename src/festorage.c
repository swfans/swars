/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file festorage.c
 *     Front-end desktop and menu system, file storage screen.
 * @par Purpose:
 *     Implement functions for file storage screen in front-end desktop.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "festorage.h"

#include "guiboxes.h"
#include "display.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenButton storage_LOAD_button;
extern struct ScreenButton storage_SAVE_button;
extern struct ScreenButton storage_NEW_MORTAL_button;

ubyte ac_do_storage_NEW_MORTAL(ubyte click);
ubyte ac_load_game_slot(ubyte click);
ubyte ac_save_game_slot(ubyte click);

void init_storage_screen_boxes(void)
{
    init_screen_button(&storage_LOAD_button, 219u, 405u,
      gui_strings[438], 6, med2_font, 1, 0);
    init_screen_button(&storage_SAVE_button, storage_LOAD_button.Width + 223, 405u,
      gui_strings[439], 6, med2_font, 1, 0);
    init_screen_button(&storage_NEW_MORTAL_button, 627u, 405u,
      gui_strings[482], 6, med2_font, 1, 128);
    storage_LOAD_button.CallBackFn = ac_load_game_slot;
    storage_SAVE_button.CallBackFn = ac_save_game_slot;
    storage_NEW_MORTAL_button.CallBackFn = ac_do_storage_NEW_MORTAL;
}

void set_flag01_storage_screen_boxes(void)
{
    storage_LOAD_button.Flags |= 0x0001;
    storage_SAVE_button.Flags |= 0x0001;
    storage_NEW_MORTAL_button.Flags |= 0x0001;
}

/******************************************************************************/
