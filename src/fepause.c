/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fepause.c
 *     Front-end desktop and menu system, in-game pause screen.
 * @par Purpose:
 *     Implement functions for pause screen displayed in-game.
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
#include "fepause.h"

#include "bfkeybd.h"
#include "bfutility.h"
#include "guiboxes.h"
#include "display.h"
#include "feoptions.h"
#include "game.h"
#include "sound.h"
#include "util.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenButton pause_continue_button;
extern struct ScreenButton pause_abort_button;

ubyte ac_do_abort_2(ubyte click);
ubyte ac_do_login_2(ubyte click);

ubyte do_login_2(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_login_2\n"
        : "=r" (ret) : "a" (click));
    return ret;
#else
    if (strlen(login_name) == 0)
        return 0;
    strtocapwords(login_name);

    read_user_settings();

    update_options_screen_state();

    if (in_network_game)
    {
        screentype = 7;
        game_system_screen = 3;
        reload_background_flag = 1;
        edit_flag = 0;
        return 1;
    }

    if ((ingame.Flags & GamF_Unkn0010) != 0)
    {
        sysmnu_button_enable(1,2);
        ingame.Flags &= ~GamF_Unkn0010;
    }

    campaign_new_game_prepare();

    if (new_mail)
        play_sample_using_heap(0, 119 + (LbRandomAnyShort() % 3), 127, 64, 100, 0, 3u);

    return 1;
#endif
}

ubyte do_abort_2(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_abort_2\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void show_pause_screen(void)
{
    if ((game_projector_speed && (pause_unkn12_box.Flags & 0x01)) ||
      (lbKeyOn[KC_SPACE] && !edit_flag))
    {
        lbKeyOn[KC_SPACE] = 0;
        pause_unkn11_box.Flags |= 0x0002;
        pause_unkn12_box.Flags |= 0x0002;
        pause_abort_button.Flags |= 0x0002;
        pause_continue_button.Flags |= 0x0002;
    }
    //pause_unkn12_box.DrawFn(&pause_unkn12_box); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&pause_unkn12_box), "g" (pause_unkn12_box.DrawFn));
    //pause_unkn11_box.DrawFn(&pause_unkn11_box); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&pause_unkn11_box), "g" (pause_unkn11_box.DrawFn));
    //pause_continue_button.DrawFn(&pause_continue_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&pause_continue_button), "g" (pause_continue_button.DrawFn));
    //pause_abort_button.DrawFn(&pause_abort_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&pause_abort_button), "g" (pause_abort_button.DrawFn));
}

void init_pause_screen_boxes(void)
{
    init_screen_button(&pause_continue_button, 260u, 300u,
      gui_strings[455], 6, med2_font, 1, 0);
    init_screen_button(&pause_abort_button, 260u, 329u,
      gui_strings[388], 6, med2_font, 1, 0);
    pause_continue_button.X = 319 - (pause_continue_button.Width >> 1);
    pause_abort_button.X = 319 - (pause_abort_button.Width >> 1);
    pause_abort_button.Border = 3;
    pause_continue_button.Border = 3;
    pause_continue_button.AccelKey = 28;
    pause_abort_button.AccelKey = 1;
    pause_continue_button.CallBackFn = ac_do_login_2;
    pause_abort_button.CallBackFn = ac_do_abort_2;
}

void set_flag01_pause_screen_boxes(void)
{
    pause_continue_button.Flags |= 0x0001;
    pause_abort_button.Flags |= 0x0001;
}

/******************************************************************************/
