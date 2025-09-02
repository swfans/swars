/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecntrls.c
 *     Front-end desktop and menu system, research screen.
 * @par Purpose:
 *     Implement functions for research screen in front-end desktop.
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
#include "fecntrls.h"

#include "bftext.h"
#include "bflib_joyst.h"

#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game_save.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "network.h"
#include "player.h"
#include "purpldrw.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenBox controls_keylist_box;
extern struct ScreenBox controls_joystick_box;
extern struct ScreenButton controls_defaults_button;
extern struct ScreenButton controls_save_button;
extern struct ScreenButton controls_calibrate_button;

ubyte ac_do_controls_defaults(ubyte click);
ubyte ac_do_controls_save(ubyte click);
ubyte ac_do_controls_calibrate(ubyte click);

ubyte do_controls_defaults(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_controls_defaults\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    set_default_user_settings();
    return 1;
}

ubyte do_controls_save(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_controls_save\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    const char *msg_str;
    if (save_user_settings())
        msg_str = gui_strings[578];
    else
        msg_str = gui_strings[577];
    alert_box_text_fmt("%s", msg_str);
    return 1;
}

ubyte do_controls_calibrate(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_controls_calibrate\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    net_unkn_pos_02 = 1;
    alert_box_text_fmt("%s", gui_strings[574]);
    return 1;
}

ubyte show_controls_joystick_box(struct ScreenBox *p_box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_show_controls_joystick_box\n"
        : "=r" (ret) : "a" (p_box));
    return ret;
#endif
    char locstr[52];
    struct ScreenRect active_rect;
    PlayerInfo *p_locplayer;
    const char *text;
    short wpos_x, wpos_y;
    short ln_height;
    short tx_width;
    ubyte dmuser;
    int i;

    lbFontPtr = small_med_font;
    my_set_text_window(p_box->X + 4, p_box->Y + 4, p_box->Width - 8, p_box->Height - 8);
    ln_height = font_height('A');

    if ((p_box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        lbFontPtr = med_font;
        lbDisplay.DrawFlags |= Lb_TEXT_HALIGN_CENTER;
        text = gui_strings[489];
        draw_text_purple_list2(0, 110, text, 0);
        lbDisplay.DrawFlags &= ~Lb_TEXT_HALIGN_CENTER;

        lbFontPtr = small_med_font;
        wpos_x = 6;
        wpos_y = 10;
        text = gui_strings[459];
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        wpos_y += ln_height + 8;

        for (i = 0; i < 4; i++)
        {
            sprintf(locstr, "%s %d", gui_strings[460], i + 1);
            text = loctext_to_gtext(locstr);
            draw_text_purple_list2(wpos_x, wpos_y, text, 0);
            wpos_y += ln_height + 4;
        }

        copy_box_purple_list(p_box->X + 4, p_box->Y + 4,
          p_box->Width - 8, p_box->Height - 8);

        lbFontPtr = small_med_font;
        p_box->Flags |= GBxFlg_BkgndDrawn;
        ln_height = font_height('A');
    }
    wpos_y = 126;

    lbDisplay.DrawFlags |= 0x8000;
    if (byte_1C4A9F == 17)
    {
      if (joy_func_063(locstr) != -1)
      {
        text = loctext_to_gtext(locstr);
        tx_width = my_string_width(locstr);
        wpos_x = (p_box->Width - tx_width) / 2;
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
      }
    }
    else
    {
        text = gui_strings[539 + byte_1C4A9F];
        tx_width = my_string_width(text);
        wpos_x = (p_box->Width - tx_width) / 2;
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
    }
    lbDisplay.DrawFlags &= ~0x8000;

    active_rect.X = text_window_x1 + wpos_x;
    active_rect.Width = tx_width;
    active_rect.Y = text_window_y1 + wpos_y;
    active_rect.Height = ln_height;
    if (mouse_down_over_box(&active_rect))
    {
        if (lbDisplay.LeftButton)
        {
            sbyte v23;
            ubyte v24;

            if (byte_1C4A9F)
                joy_func_066(&joy);
            lbDisplay.LeftButton = 0;

            v23 = -1;
            v24 = byte_1C4A9F;
            while (v23 != 1)
            {
                if (++byte_1C4A9F > 24)
                    byte_1C4A9F = 1;
                if (v24 == byte_1C4A9F)
                {
                    v23 = 1;
                    byte_1C4A9F = 0;
                }
                if (unkn01_maskarr[byte_1C4A9F])
                    v23 = joy_func_067(&joy, byte_1C4A9F);
                if (!v24)
                    v24 = 1;
            }
        }
    }
    p_locplayer = &players[local_player_no];

    wpos_x = 140;
    wpos_y = 10;

    sprintf(locstr, "%d", p_locplayer->DoubleMode + 1);
    lbDisplay.DrawFlags |= 0x8000;
    text = loctext_to_gtext(locstr);
    draw_text_purple_list2(wpos_x, wpos_y, text, 0);
    tx_width = LbTextStringWidth(locstr);
    lbDisplay.DrawFlags &= ~0x8000;

    active_rect.X = text_window_x1 + wpos_x;
    active_rect.Width = tx_width;
    active_rect.Y = text_window_y1 + wpos_y;
    active_rect.Height = ln_height;
    if (mouse_down_over_box(&active_rect))
    {
        if (lbDisplay.LeftButton)
        {
            lbDisplay.LeftButton = 0;
            if (login_control__State != 5 || nsvc.I.Type == 1)
            {
                p_locplayer->DoubleMode++;
                if (p_locplayer->DoubleMode > 3)
                    p_locplayer->DoubleMode = 0;
            }
            else
            {
                p_locplayer->DoubleMode = 0;
            }

            for (dmuser = p_locplayer->DoubleMode + 1; dmuser < 4; dmuser++)
            {
                p_locplayer->UserInput[dmuser].ControlMode = 1;
            }
        }
    }

    wpos_x = 100;
    wpos_y = ln_height + 18;
    lbDisplay.DrawFlags |= 0x8000;

    active_rect.Y = text_window_y1 + wpos_y;
    active_rect.Height = ln_height;
    for (dmuser = 0; dmuser < p_locplayer->DoubleMode + 1; dmuser++)
    {
        ushort ctlmode;

        ctlmode = p_locplayer->UserInput[dmuser].ControlMode;
        if (ctlmode >= 2)
        {
            int n_found;

            i = 0;
            n_found = 0;
            while (n_found < ctlmode - 1)
            {
                if (joy.Init[i])
                    n_found++;
                i++;
            }
            sprintf(locstr, "%s %d", gui_strings[463], i);
            text = loctext_to_gtext(locstr);
            draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        }
        else
        {
            sprintf(locstr, "%s", gui_strings[461 + ctlmode]);
            text = loctext_to_gtext(locstr);
            draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        }
        tx_width = LbTextStringWidth(locstr);

        active_rect.X = text_window_x1 + wpos_x;
        active_rect.Width = tx_width;
        if (mouse_down_over_box(&active_rect))
        {
            if (lbDisplay.LeftButton)
            {
                lbDisplay.LeftButton = 0;

                ctlmode = p_locplayer->UserInput[dmuser].ControlMode + 1;
                p_locplayer->UserInput[dmuser].ControlMode = ctlmode;
                if (!byte_1C4A9F && ctlmode == 2)
                    p_locplayer->UserInput[dmuser].ControlMode = 0;
                if (p_locplayer->UserInput[dmuser].ControlMode > joy.NumberOfDevices + 1)
                    p_locplayer->UserInput[dmuser].ControlMode = 0;
            }
        }
        wpos_y += ln_height + 4;
    }
    lbDisplay.DrawFlags &= ~0x8000;

    //controls_calibrate_button.DrawFn(&controls_calibrate_button); -- incompatible calling convention
    {
        ubyte drawn;
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&controls_calibrate_button), "g" (controls_calibrate_button.DrawFn));
    }
    return 0;
}

void init_controls_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_box(&controls_keylist_box, 213u, 72u, 420u, 354, 6);
    init_screen_button(&controls_defaults_button, 219u, 405u,
      gui_strings[484], 6, med2_font, 1, 0);
    controls_defaults_button.CallBackFn = ac_do_controls_defaults;
    init_screen_button(&controls_save_button, 627u, 405u,
      gui_strings[439], 6, med2_font, 1, 0x80);
    controls_save_button.CallBackFn = ac_do_controls_save;

    init_screen_box(&controls_joystick_box, 7u, 252u, 197u, 174, 6);
    controls_joystick_box.SpecialDrawFn = show_controls_joystick_box;
    init_screen_button(&controls_calibrate_button, 57u, 405u,
      gui_strings[485], 6, med2_font, 1, 0);
    controls_calibrate_button.CallBackFn = ac_do_controls_calibrate;

    start_x = (scr_w - controls_joystick_box.Width - controls_keylist_box.Width - 23) / 2;

    controls_joystick_box.X = start_x + 7;
    controls_calibrate_button.X = controls_joystick_box.X + 50;

    controls_keylist_box.X = controls_joystick_box.X + controls_joystick_box.Width + 9;
    controls_defaults_button.X = controls_keylist_box.X + 6;
    controls_save_button.X = controls_keylist_box.X + controls_keylist_box.Width - controls_save_button.Width - 6;
}

void reset_controls_screen_boxes_flags(void)
{
    controls_keylist_box.Flags = GBxFlg_Unkn0001;
    controls_joystick_box.Flags = GBxFlg_Unkn0001;
}

void set_flag01_controls_screen_boxes(void)
{
    controls_save_button.Flags |= GBxFlg_Unkn0001;
    controls_defaults_button.Flags |= GBxFlg_Unkn0001;
    controls_calibrate_button.Flags |= GBxFlg_Unkn0001;
}

void clear_someflags_controls_screen_boxes(void)
{
    controls_keylist_box.Flags &= ~(GBxFlg_BkgndDrawn|GBxFlg_TextRight|GBxFlg_BkCopied);
}

/******************************************************************************/
