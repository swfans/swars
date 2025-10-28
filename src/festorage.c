/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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
#include "feshared.h"
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

ubyte show_storage_screen(void)
{
    ubyte drawn;

    //drawn = storage_slots_box.DrawFn(&storage_slots_box); -- incompatible calling convention
    asm volatile ("call *%2\n"
        : "=r" (drawn) : "a" (&storage_slots_box), "g" (storage_slots_box.DrawFn));
    return drawn;
}

void init_storage_screen_boxes(void)
{
    ScrCoord scr_w, scr_h, start_x, start_y;
    short space_w, space_h, border;

    // Border value represents how much the box background goes
    // out of the box area.
    border = 3;
    scr_w = lbDisplay.GraphicsWindowWidth;
#ifdef EXPERIMENTAL_MENU_CENTER_H
    scr_h = global_apps_bar_box.Y;
#else
    scr_h = 432;
#endif

    init_screen_text_box(&storage_slots_box, 213u, 72u, 420u, 354, 6, med2_font, 1);
    storage_slots_box.DrawTextFn = ac_show_menu_storage_slots_box;
    storage_slots_box.ScrollWindowHeight = 208;
    storage_slots_box.Lines = 99;
    storage_slots_box.Flags |= (GBxFlg_RadioBtn | GBxFlg_IsMouseOver);
    storage_slots_box.LineHeight = 26;
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

    // Reposition the components to current resolution

    start_x = unkn13_SYSTEM_button.X + unkn13_SYSTEM_button.Width;
    // On the X axis, we're going for centering on the screen. So subtract the previous
    // button position two times - once for the left, and once to make the same space on
    // the right.
    space_w = scr_w - start_x - unkn13_SYSTEM_button.X - storage_slots_box.Width;

    start_y = system_screen_shared_header_box.Y + system_screen_shared_header_box.Height;
    // On the top, we're aligning to spilled border of previous box; same goes inside.
    // But on the bottom, we're aligning to hard border, without spilling. To compensate
    // for that, add pixels for such border to the space.
    space_h = scr_h - start_y - storage_slots_box.Height + border;

    // There is one box only to position, and no space is needed after it - the whole
    // available empty space goes into one place.
    storage_slots_box.X = start_x + space_w;
    // There is one box only to position, so space goes into two parts - before and after.
    storage_slots_box.Y = start_y + space_h / 2;

    space_w = 5;
    space_h = 5;
    storage_LOAD_button.X = storage_slots_box.X + (space_w + 1);
    storage_LOAD_button.Y = storage_slots_box.Y + storage_slots_box.Height - space_w - storage_LOAD_button.Height;
    storage_SAVE_button.X = storage_LOAD_button.X + storage_LOAD_button.Width + (space_w - 1);
    storage_SAVE_button.Y = storage_LOAD_button.Y;
    storage_NEW_MORTAL_button.X = storage_slots_box.X + storage_slots_box.Width -
      storage_NEW_MORTAL_button.Width - (space_w + 1);
    storage_NEW_MORTAL_button.Y = storage_LOAD_button.Y;
}

void reset_storage_screen_boxes_flags(void)
{
    storage_slots_box.Flags = GBxFlg_Unkn0001;
    storage_slots_box.Flags |= (GBxFlg_RadioBtn | GBxFlg_IsMouseOver);
}

void set_flag01_storage_screen_boxes(void)
{
    storage_LOAD_button.Flags |= GBxFlg_Unkn0001;
    storage_SAVE_button.Flags |= GBxFlg_Unkn0001;
    storage_NEW_MORTAL_button.Flags |= GBxFlg_Unkn0001;
}

void skip_flashy_draw_storage_screen_boxes(void)
{
    storage_slots_box.Flags |= 0x0002;
}

void mark_storage_screen_boxes_redraw(void)
{
    storage_slots_box.Flags &= ~(GBxFlg_BkgndDrawn | GBxFlg_TextRight | GBxFlg_BkCopied);
}

/******************************************************************************/
