/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file femain.c
 *     Front-end desktop and menu system, login screen.
 * @par Purpose:
 *     Implement functions for login screen in front-end desktop.
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
#include "femain.h"

#include "bftext.h"
#include "bfkeybd.h"
#include "bftime.h"
#include "bfmouse.h"
#include "display.h"
#include "guiboxes.h"
#include "guitext.h"
#include "game.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenTextBox heading_box;
extern struct ScreenButton sysmnu_buttons[6];
extern char options_title_text[];

extern struct ScreenButton main_quit_button;
extern struct ScreenButton main_login_button;
extern struct ScreenButton main_map_editor_button;
extern struct ScreenButton main_load_button;

extern struct ScreenBox alert_box;
extern struct ScreenButton alert_OK_button;

extern struct ScreenTextBox loading_INITIATING_box;

ubyte ac_main_do_my_quit(ubyte click);
ubyte ac_main_do_login_1(ubyte click);
ubyte ac_goto_savegame(ubyte click);
ubyte ac_main_do_map_editor(ubyte click);
ubyte ac_alert_OK(ubyte click);
ubyte ac_do_sysmnu_button(ubyte click);

ubyte main_do_map_editor(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_main_do_map_editor\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void show_main_screen(void)
{
    if ((game_projector_speed && (main_quit_button.Flags & 0x01)) ||
      (lbKeyOn[KC_SPACE] && !edit_flag))
    {
        lbKeyOn[KC_SPACE] = 0;
        main_quit_button.Flags |= 0x0002;
        main_load_button.Flags |= 0x0002;
        main_login_button.Flags |= 0x0002;
        main_map_editor_button.Flags |= 0x0002;
    }
    //main_quit_button.DrawFn(&main_quit_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_quit_button), "g" (main_quit_button.DrawFn));
    //main_load_button.DrawFn(&main_load_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_load_button), "g" (main_load_button.DrawFn));
    //main_login_button.DrawFn(&main_login_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_login_button), "g" (main_login_button.DrawFn));
}

void init_main_screen_boxes(void)
{
    short scr_w;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_button(&main_map_editor_button, 260, 387,
      gui_strings[443], 6, med2_font, 1, 0);
    init_screen_button(&main_load_button, 260, 358,
      gui_strings[496], 6, med2_font, 1, 0);
    init_screen_button(&main_quit_button, 260, 329,
      gui_strings[445], 6, med2_font, 1, 0);
    init_screen_button(&main_login_button, 260, 300,
      gui_strings[444], 6, med2_font, 1, 0);

    main_map_editor_button.X = (scr_w - main_map_editor_button.Width) / 2 - 1;
    main_login_button.X = (scr_w - main_login_button.Width) / 2 - 1;
    main_quit_button.X = (scr_w - main_quit_button.Width) / 2 - 1;
    main_load_button.X = (scr_w - main_load_button.Width) / 2 - 1;

    main_map_editor_button.Border = 3;
    main_login_button.Border = 3;
    main_quit_button.Border = 3;
    main_load_button.Border = 3;

    main_map_editor_button.CallBackFn = ac_main_do_map_editor;
    main_login_button.CallBackFn = ac_main_do_login_1;
    main_quit_button.CallBackFn = ac_main_do_my_quit;
    main_load_button.CallBackFn = ac_goto_savegame;

    main_login_button.AccelKey = 28;
    main_quit_button.AccelKey = 1;
}

void set_flag01_main_screen_boxes(void)
{
    main_quit_button.Flags |= 0x0001;
    main_login_button.Flags |= 0x0001;
    main_load_button.Flags |= 0x0001;
    main_map_editor_button.Flags |= 0x0001;
}

ubyte alert_OK(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_alert_OK\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void show_alert_box(void)
{
    asm volatile ("call ASM_show_alert_box\n"
        :  :  : "eax" );
}

void init_alert_screen_boxes(void)
{
    short scr_w;
    int w;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&loading_INITIATING_box, 210u, 230u, 220u, 20,
      6, med_font, 1);
    loading_INITIATING_box.Text = gui_strings[376];

    lbFontPtr = med_font;
    loading_INITIATING_box.Height = font_height('A') + 8;
    w = my_string_width(gui_strings[376]);
    loading_INITIATING_box.Width = w + 9;
    loading_INITIATING_box.X = (scr_w - (w + 9)) / 2 - 1;
    loading_INITIATING_box.Y = 219 - (loading_INITIATING_box.Height >> 1);

    init_screen_box(&alert_box, 219u, 189u, 200u, 100, 6);
    init_screen_button(&alert_OK_button, 10u, 269u,
      gui_strings[458], 6, med2_font, 1, 0);
    alert_OK_button.CallBackFn = ac_alert_OK;

    alert_box.X = (scr_w - alert_box.Width) / 2 - 1;
    alert_OK_button.X = (scr_w - alert_OK_button.Width) / 2 - 1;
    alert_OK_button.AccelKey = 28;
}

void reset_alert_screen_boxes_flags(void)
{
    alert_box.Flags = 1;
}

void set_flag01_alert_screen_boxes(void)
{
    alert_OK_button.Flags |= 0x0001;
}

ubyte show_title_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_title_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte do_sysmnu_button(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_sysmnu_button\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void init_system_menu_boxes(void)
{
    short scr_w, start_x;
    int i, h, val;

    scr_w = 640;//lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&heading_box, 7u, 25, 626, 38, 6, big_font, 1);
    heading_box.DrawTextFn = ac_show_title_box;
    heading_box.Text = options_title_text;

    start_x = (scr_w - heading_box.Width) / 2;
    heading_box.X = start_x;

    init_screen_text_box(&unkn13_SYSTEM_button, 7u, 25u, 197u, 38, 6,
      big_font, 1);
    unkn13_SYSTEM_button.Text = gui_strings[366];
    unkn13_SYSTEM_button.DrawTextFn = ac_show_title_box;

    val = 0;
    h = 72;
    for (i = 0; i < 6; i++)
    {
        init_screen_button(&sysmnu_buttons[i], 7, h,
          gui_strings[378 + val], 6, med2_font, 1, 0);
        sysmnu_buttons[i].Width = 197;
        sysmnu_buttons[i].Height = 21;
        sysmnu_buttons[i].CallBackFn = ac_do_sysmnu_button;
        sysmnu_buttons[i].Flags |= 0x10;
        sysmnu_buttons[i].Border = 3;
        val++;
        h += 30;
    }

    unkn13_SYSTEM_button.X = start_x;
    for (i = 0; i < 6; i++)
    {
        sysmnu_buttons[i].X = start_x;
    }
}

void sysmnu_button_enable(int btnno, int count)
{
    int i;
    for (i = btnno+count; i < 6; i++) {
        sysmnu_buttons[i].Y += 30 * count;
    }
}

void sysmnu_button_disable(int btnno, int count)
{
    int i;
    for (i = btnno+count; i < 6; i++) {
        sysmnu_buttons[i].Y -= 30 * count;
    }
}

void set_heading_box_text(const char *text)
{
    heading_box.Text = text;
}

void reset_heading_screen_boxes_flags(void)
{
    heading_box.Flags = 0x0001;
}

TbBool is_heading_flag01(void)
{
    return (heading_box.Flags & 0x0001) != 0;
}

void set_flag02_heading_screen_boxes(void)
{
    heading_box.Flags |= 0x0002;
}

int draw_heading_box(void)
{
    int ret;
    //ret = heading_box.DrawFn(&heading_box); -- incompatible calling convention
    asm volatile ("call *%2\n"
        : "=r" (ret) : "a" (&heading_box), "g" (heading_box.DrawFn));
    return ret;
}

void reset_system_menu_boxes_flags(void)
{
    int i;

    unkn13_SYSTEM_button.Flags = 0x0001;

    for (i = 0; i < 6; i++) {
        sysmnu_buttons[i].Flags = 0x0011;
    }
}

void clear_someflags_system_menu_screen_boxes(void)
{
    unkn13_SYSTEM_button.Flags &= ~(0x8000|0x2000|0x0004);
}

void show_mission_loading_screen(void)
{
    LbMouseChangeSprite(0);
    reload_background();
    play_sample_using_heap(0, 118, 127, 64, 100, 0, 3);

    DwBool stop;
    TbClockMSec last_loop_time = LbTimerClock();
    do
    {
        memcpy(lbDisplay.WScreen, back_buffer, lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight);
        text_buf_pos = lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight;
        if ((0 != game_projector_speed && (loading_INITIATING_box.Flags & 0x0001))
          || (0 != lbKeyOn[KC_SPACE] && 0 == edit_flag))
            loading_INITIATING_box.Flags |= 0x0002;
        //loading_INITIATING_box.DrawFn(&loading_INITIATING_box); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&loading_INITIATING_box), "g" (loading_INITIATING_box.DrawFn));
        stop = loading_INITIATING_box.Flags & 0x1000;
        draw_purple_screen();
        swap_wscreen();

        game_update();
        TbClockMSec sleep_end = last_loop_time + 1000/GAME_FPS;
        LbSleepUntil(sleep_end);
        last_loop_time = LbTimerClock();
    }
    while (!stop);

    loading_INITIATING_box.Flags = 0x0001;
    wait_for_sound_sample_finish(118);
}

/******************************************************************************/
