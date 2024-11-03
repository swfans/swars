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

#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game_sprts.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenButton storage_LOAD_button;
extern struct ScreenButton storage_SAVE_button;
extern struct ScreenButton storage_NEW_MORTAL_button;
extern struct ScreenTextBox storage_slots_box;
extern struct ScreenTextBox storage_header_box;

ubyte ac_do_storage_NEW_MORTAL(ubyte click);
ubyte ac_load_game_slot(ubyte click);
ubyte ac_save_game_slot(ubyte click);
ubyte ac_show_menu_storage_slots_box(struct ScreenTextBox *box);

ubyte show_menu_storage_slots_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_menu_storage_slots_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void init_storage_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&storage_header_box, 213u, 25u, 420u, 38, 6, big_font, 1);
    storage_header_box.DrawTextFn = ac_show_title_box;

    init_screen_text_box(&storage_slots_box, 213u, 72u, 420u, 354, 6, med2_font, 1);
    storage_slots_box.DrawTextFn = ac_show_menu_storage_slots_box;
    storage_slots_box.ScrollWindowHeight = 208;
    storage_slots_box.Lines = 99;
    storage_slots_box.Flags |= GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
    storage_slots_box.BGColour = 26;
    storage_slots_box.ScrollWindowOffset += 27;

    init_screen_button(&storage_LOAD_button, 219u, 405u,
      gui_strings[438], 6, med2_font, 1, 0);
    init_screen_button(&storage_SAVE_button, 219 + storage_LOAD_button.Width + 4, 405u,
      gui_strings[439], 6, med2_font, 1, 0);
    init_screen_button(&storage_NEW_MORTAL_button, 627u, 405u,
      gui_strings[482], 6, med2_font, 1, 128);
    storage_LOAD_button.CallBackFn = ac_load_game_slot;
    storage_SAVE_button.CallBackFn = ac_save_game_slot;
    storage_NEW_MORTAL_button.CallBackFn = ac_do_storage_NEW_MORTAL;

    start_x = (scr_w - unkn13_SYSTEM_button.Width - 16 - storage_slots_box.Width - 7) / 2;

    storage_header_box.X = start_x + 7 + unkn13_SYSTEM_button.Width + 9;
    storage_slots_box.X = start_x + 7 + unkn13_SYSTEM_button.Width + 9;

    storage_LOAD_button.X = storage_slots_box.X + 6;
    storage_SAVE_button.X = storage_LOAD_button.X + storage_LOAD_button.Width + 4;
    storage_NEW_MORTAL_button.X = storage_slots_box.X + storage_slots_box.Width -
      storage_NEW_MORTAL_button.Width - 6;
}

void reset_storage_screen_boxes_flags(void)
{
    storage_header_box.Flags = GBxFlg_Unkn0001;
    storage_slots_box.Flags = GBxFlg_Unkn0001 | GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
}

void set_flag01_storage_screen_boxes(void)
{
    storage_LOAD_button.Flags |= GBxFlg_Unkn0001;
    storage_SAVE_button.Flags |= GBxFlg_Unkn0001;
    storage_NEW_MORTAL_button.Flags |= GBxFlg_Unkn0001;
}

void clear_someflags_storage_screen_boxes(void)
{
    storage_header_box.Flags &= ~(GBxFlg_BkgndDrawn | GBxFlg_TextRight | GBxFlg_Unkn0004);
    storage_slots_box.Flags &= ~(GBxFlg_BkgndDrawn | GBxFlg_TextRight | GBxFlg_Unkn0004);
}

/******************************************************************************/
