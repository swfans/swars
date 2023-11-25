/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feworld.c
 *     Front-end desktop and menu system, world view screen.
 * @par Purpose:
 *     Implement functions for world view screen in front-end desktop.
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
#include "feworld.h"

#include "bfkeybd.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenTextBox world_city_info_box;
extern struct ScreenButton world_info_ACCEPT_button;
extern struct ScreenButton world_info_CANCEL_button;
extern struct ScreenBox world_landmap_box;

ubyte ac_show_world_city_info_box(struct ScreenTextBox *box);
ubyte ac_show_world_landmap_box(struct ScreenBox *box);
ubyte ac_do_unkn2_CANCEL(ubyte click);
ubyte ac_do_unkn2_ACCEPT(ubyte click);

ubyte do_unkn2_CANCEL(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_unkn2_CANCEL\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_unkn2_ACCEPT(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_unkn2_ACCEPT\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte show_world_city_info_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_world_city_info_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_world_landmap_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_world_landmap_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void show_worldmap_screen(void)
{
    if ((game_projector_speed && (heading_box.Flags & 0x01)) ||
      (lbKeyOn[KC_SPACE] && !edit_flag))
    {
        lbKeyOn[KC_SPACE] = 0;
        set_flag02_world_screen_boxes();
    }
    int ret = 1;
    if (ret) {
        //ret = heading_box.DrawFn(&heading_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (ret) : "a" (&heading_box), "g" (heading_box.DrawFn));
    }
    if (ret) {
        //ret = world_landmap_box.DrawFn(&world_landmap_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (ret) : "a" (&world_landmap_box), "g" (world_landmap_box.DrawFn));
    }
    if (ret) {
        //ret = world_city_info_box.DrawFn(&world_city_info_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (ret) : "a" (&world_city_info_box), "g" (world_city_info_box.DrawFn));
    }
}

void init_world_screen_boxes(void)
{
    init_screen_box(&world_landmap_box, 7u, 72u, 518u, 354, 6);
    init_screen_text_box(&world_city_info_box, 534u, 72u, 99u, 354, 6, small_med_font, 3);
    init_screen_button(&world_info_ACCEPT_button, 548u, 384u, gui_strings[436], 6,
        med2_font, 1, 0);
    init_screen_button(&world_info_CANCEL_button, 548u, 405u, gui_strings[437], 6,
        med2_font, 1, 0);
    world_info_ACCEPT_button.X = world_city_info_box.X
        + ((world_city_info_box.Width - world_info_ACCEPT_button.Width) >> 1);
    world_info_CANCEL_button.X =
        ((world_city_info_box.Width - world_info_CANCEL_button.Width) >> 1) + world_city_info_box.X;
    world_city_info_box.DrawTextFn = ac_show_world_city_info_box;
    world_city_info_box.Flags |= 0x4000;
    world_info_CANCEL_button.CallBackFn = ac_do_unkn2_CANCEL;
    world_info_ACCEPT_button.CallBackFn = ac_do_unkn2_ACCEPT;
    world_landmap_box.SpecialDrawFn = ac_show_world_landmap_box;
}

void reset_world_screen_boxes_flags(void)
{
    world_landmap_box.Flags = 0x0001;
    world_city_info_box.Flags = 0x0001 | 0x4000;
}

void set_flag01_world_screen_boxes(void)
{
    world_info_ACCEPT_button.Flags |= 0x0001;
    world_info_CANCEL_button.Flags |= 0x0001;
}

void set_flag02_world_screen_boxes(void)
{
    world_landmap_box.Flags |= 0x0002;
    heading_box.Flags |= 0x0002;
    world_city_info_box.Flags |= 0x0002;
}
/******************************************************************************/
