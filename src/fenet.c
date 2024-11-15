/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fenet.c
 *     Front-end desktop and menu system, multiplayer screen.
 * @par Purpose:
 *     Implement functions for multiplayer screen in front-end desktop.
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
#include "fenet.h"

#include "bfscrcopy.h"
#include "bfsprite.h"
#include "bftext.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "femain.h"
#include "game_sprts.h"
#include "game.h"
#include "purpldrw.h"
#include "purpldrwlst.h"
#include "swlog.h"
/******************************************************************************/

extern struct ScreenButton net_INITIATE_button;
extern struct ScreenButton net_groups_LOGON_button;
extern struct ScreenButton unkn8_EJECT_button;
extern struct ScreenButton net_protocol_select_button;
extern struct ScreenButton net_unkn40_button;
extern struct ScreenButton net_SET2_button;
extern struct ScreenButton net_SET_button;
extern struct ScreenBox net_groups_box;
extern struct ScreenBox net_users_box;
extern struct ScreenBox net_faction_box;
extern struct ScreenBox net_team_box;
extern struct ScreenBox net_benefits_box;
extern struct ScreenBox net_comms_box;
extern struct ScreenBox net_unkn21;
extern struct ScreenBox net_protocol_box;
extern struct ScreenButton net_protocol_option_button;

extern char net_unkn40_text[];
extern char unkn_opt_number_text[];
extern ulong dword_155750[];
extern ubyte byte_155174; // = 166;
extern ubyte byte_155175[];
extern ubyte byte_155180; // = 109;
extern ubyte byte_155181[];
extern struct TbSprite *sprites_Icons0_0;

ubyte ac_do_net_protocol_option(ubyte click);
ubyte ac_do_net_unkn40(ubyte click);
ubyte ac_do_serial_speed_switch(ubyte click);
ubyte ac_do_net_SET2(ubyte click);
ubyte ac_do_net_SET(ubyte click);
ubyte ac_do_net_INITIATE(ubyte click);
ubyte ac_do_net_groups_LOGON(ubyte click);
ubyte ac_do_unkn8_EJECT(ubyte click);
ubyte ac_show_net_benefits_box(struct ScreenBox *box);
ubyte ac_show_net_unkn21(struct ScreenBox *box);
ubyte ac_show_net_comms_box(struct ScreenBox *box);
ubyte ac_do_net_protocol_select(ubyte click);
ubyte ac_show_net_protocol_box(struct ScreenBox *box);
ubyte ac_show_net_faction_box(struct ScreenBox *box);
ubyte ac_show_net_team_box(struct ScreenBox *box);
ubyte ac_show_net_groups_box(struct ScreenBox *box);
ubyte ac_show_net_users_box(struct ScreenBox *box);

ubyte do_net_protocol_option(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_protocol_option\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_unkn40(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_unkn40\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_serial_speed_switch(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_serial_speed_switch\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_SET2(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_SET2\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_SET(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_SET\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_INITIATE(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_INITIATE\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_groups_LOGON(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_groups_LOGON\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

TbBool mouse_down_over_unkn1(short x, short y, short width, short height)
{
    short ms_x, ms_y;

    ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
    ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
    if ((ms_y >= y) && (ms_y <= y + height))
    {
        short sum_min, sum_max;

        sum_min = x + y + height;
        sum_max = x + y + width;
        return (ms_x + ms_y >= sum_min) && (ms_x + ms_y <= sum_max);
    }
    return false;
}

void show_net_benefits_sub1(short x0, short y0, TbPixel colour)
{
    ushort spridx;
    short dx, dy;
    short i;

    dx = x0;
    dy = y0;
    spridx = 114;
    for (i = 0; i < 8; i++)
    {
        struct TbSprite *p_sprite;
        short delta;

        p_sprite = &sprites_Icons0_0[spridx];
        lbDisplay.DrawColour = colour;
        draw_sprite_purple_list(dx, dy, p_sprite);
        if (i < 2)
            delta = p_sprite->SWidth - 11;
        else if (i < 5)
            delta = p_sprite->SWidth - 25;
        else
            delta = p_sprite->SWidth - 5;
        dx += delta;
        if (i == 0)
            delta = 2;
        else if (i == 1)
            delta = -14;
        else
            delta = 0;
        dy += delta;
        spridx++;
    }
}

void show_net_benefits_sub2(short x0, short y0, TbPixel *colours)
{
    ushort spridx;
    short dx, dy;
    short i;

    dx = x0;
    dy = y0;
    spridx = 114;
    for (i = 0; i < 8; i++)
    {
        struct TbSprite *p_sprite;
        short delta;

        p_sprite = &sprites_Icons0_0[spridx];
        if (i < login_control__TechLevel)
        {
            lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
            lbDisplay.DrawColour = colours[i];
            draw_sprite_purple_list(dx, dy, p_sprite);
            lbDisplay.DrawFlags = 0;
        }
        if (mouse_down_over_unkn1(dx, dy, p_sprite->SWidth, p_sprite->SHeight))
        {
            if (lbDisplay.LeftButton)
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 2) == 0)
                  && (login_control__State == 5))
                    login_control__TechLevel = i + 1;
            }
        }
        if (i < 2)
            delta = p_sprite->SWidth - 11;
        else if (i < 5)
            delta = p_sprite->SWidth - 25;
        else
            delta = p_sprite->SWidth - 5;
        dx += delta;
        if (i == 0)
            delta = 2;
        else if (i == 1)
            delta = -14;
        else
            delta = 0;
        dy += delta;
        spridx++;
    }
}

void show_net_benefits_sub3(struct ScreenBox *box)
{
    struct ScreenBoxBase box1 = {box->X + 5, box->Y + 16, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box1))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box1))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 2) == 0)
                    && (login_control__State == 5))
                {
                    login_control__TechLevel--;
                    if (login_control__TechLevel < 1)
                        login_control__TechLevel = 1;
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    draw_sprite_purple_list(box1.X, box1.Y, &sprites_Icons0_0[108]);
}

void show_net_benefits_sub4(struct ScreenBox *box)
{
    struct ScreenBoxBase box2 = {box->X + 242, box->Y + 5, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box2))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box2))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 2) == 0)
                    && (login_control__State == 5))
                {
                    login_control__TechLevel++;
                    if (login_control__TechLevel > 8)
                        login_control__TechLevel = 8;
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    draw_sprite_purple_list(box2.X - 7, box2.Y, &sprites_Icons0_0[109]);
}

ulong sub_CCE8C(sbyte change)
{
    ulong ret;
    asm volatile ("call ASM_sub_CCE8C\n"
        : "=r" (ret) : "a" (change));
    return ret;
}

void show_net_benefits_sub5(short x0, short y0, TbPixel *colours)
{
    ushort spridx;
    short dx, dy;
    short i;

    dx = x0;
    dy = y0;
    spridx = 114;
    for (i = 0; i < 8; i++)
    {
        struct TbSprite *p_sprite;
        short delta;

        p_sprite = &sprites_Icons0_0[spridx];
        if (login_control__Money >= dword_155750[i])
        {
            lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
            lbDisplay.DrawColour = colours[i];
            draw_sprite_purple_list(dx, dy, p_sprite);
            lbDisplay.DrawFlags = 0;
        }
        if (mouse_down_over_unkn1(dx, dy, p_sprite->SWidth, p_sprite->SHeight))
        {
            if (lbDisplay.LeftButton)
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 1) == 0)
                  && (login_control__State == 5))
                {
                    login_control__Money = dword_155750[i];
                    ingame.Credits = login_control__Money;
                }
            }
        }
        if (i < 2)
            delta = p_sprite->SWidth - 11;
        else if (i < 5)
            delta = p_sprite->SWidth - 25;
        else
            delta = p_sprite->SWidth - 5;
        dx += delta;
        if (i == 0)
            delta = 2;
        else if (i == 1)
            delta = -14;
        else
            delta = 0;
        dy += delta;
        spridx++;
    }
}

void show_net_benefits_sub6(struct ScreenBox *box)
{
    struct ScreenBoxBase box1 = {box->X + 5, box->Y + 47, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box1))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box1))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 1) == 0)
                    && (login_control__State == 5))
                {
                    sub_CCE8C(-1);
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
    lbDisplay.DrawColour = 87;
    draw_sprite_purple_list(box1.X, box1.Y, &sprites_Icons0_0[108]);
}

void show_net_benefits_sub7(struct ScreenBox *box)
{
    struct ScreenBoxBase box2 = {box->X + 242, box->Y + 36, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box2))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box2))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 1) == 0)
                    && (login_control__State == 5))
                {
                    sub_CCE8C(1);
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
    lbDisplay.DrawColour = 87;
    draw_sprite_purple_list(box2.X - 7, box2.Y, &sprites_Icons0_0[109]);
}


ubyte show_net_benefits_box(struct ScreenBox *box)
{
    ubyte drawn = true;

    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);
    if ((box->Flags & GBxFlg_Unkn1000) == 0)
    {
        lbFontPtr = med2_font;
        draw_text_purple_list2(30, 1, gui_strings[401], 0);
        draw_text_purple_list2(27, 32, gui_strings[402], 0);
        lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
        show_net_benefits_sub1(box->X + 8, box->Y + 16, byte_155174);
        show_net_benefits_sub1(box->X + 8, box->Y + 47, byte_155180);
        lbDisplay.DrawFlags = 0;
        copy_box_purple_list(box->X - 3, box->Y - 3,
            box->Width + 6, box->Height + 6);
        box->Flags |= GBxFlg_Unkn1000;
    }

    show_net_benefits_sub2(box->X + 8, box->Y + 16, byte_155175);
    show_net_benefits_sub3(box);
    show_net_benefits_sub4(box);

    lbDisplay.DrawFlags = 0;

    show_net_benefits_sub5(box->X + 8, box->Y + 47, byte_155181);
    show_net_benefits_sub6(box);
    show_net_benefits_sub7(box);

    lbDisplay.DrawFlags = 0;
    if (is_unkn_current_player() && (login_control__State == 5))
    {
        //net_SET2_button.DrawFn(&net_SET2_button); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&net_SET2_button), "g" (net_SET2_button.DrawFn));
        //net_SET_button.DrawFn(&net_SET_button); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&net_SET_button), "g" (net_SET_button.DrawFn));
    }
    return drawn;
}

void purple_unkn3_data_to_screen(void)
{
    LbScreenSetGraphicsWindow(net_unkn21.X + 4, net_unkn21.Y + 4,
      255, 96);
    LbScreenCopy(dword_1C6DE8, lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsWindowHeight);
    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
}

void purple_unkn4_data_to_screen(void)
{
    LbScreenSetGraphicsWindow(net_unkn21.X + 4, net_unkn21.Y + 4,
      255, 96);
    LbScreenCopy(dword_1C6DE4, lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsWindowHeight);
    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
}

ubyte show_net_unkn21(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_unkn21\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_comms_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_comms_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte do_net_protocol_select(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_protocol_select\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte show_net_protocol_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_protocol_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_faction_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_faction_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_team_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_team_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_groups_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_groups_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_users_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_users_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte do_unkn8_EJECT(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_unkn8_EJECT\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void init_net_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_box(&net_groups_box, 213u, 72u, 171u, 155, 6);
    init_screen_box(&net_users_box, 393u, 72u, 240u, 155, 6);
    init_screen_box(&net_faction_box, 213u, 236u, 73u, 67, 6);
    init_screen_box(&net_team_box, 295u, 236u, 72u, 67, 6);
    init_screen_box(&net_benefits_box, 376u, 236u, 257u, 67, 6);
    init_screen_box(&net_comms_box, 295u, 312u, 336u, 104, 6);
    init_screen_box(&net_unkn21, 7u, 312u, 279u, 104, 6);
    init_screen_box(&net_protocol_box, 7u, 252u, 197u, 51, 6);

    init_screen_button(&net_INITIATE_button, 218u, 185u, gui_strings[385], 6,
        med2_font, 1, 0);
    init_screen_button(&net_groups_LOGON_button, 218u, 206u, gui_strings[386],
        6, med2_font, 1, 0);
    init_screen_button(&unkn8_EJECT_button, 308u, 206u, gui_strings[403], 6,
        med2_font, 1, 0);
    init_screen_button(&net_SET2_button, 562u, 251u, gui_strings[440], 6,
        med2_font, 1, 0);
    init_screen_button(&net_SET_button, 562u, 284u, gui_strings[440], 6,
        med2_font, 1, 0);
    init_screen_button(&net_protocol_select_button, 37u, 256u, gui_strings[498],
        6, med2_font, 1, 0);
    init_screen_button(&net_unkn40_button, 37u, 256u, net_unkn40_text, 6,
        med2_font, 1, 0);
    init_screen_button(&net_protocol_option_button, 7u, 275u,
        unkn_opt_number_text, 6, med2_font, 1, 0);

    net_groups_LOGON_button.Width = 85;
    net_INITIATE_button.Width = 85;
    net_unkn40_button.Width = 21;
    net_protocol_select_button.Width = 157;
    net_protocol_option_button.Width = net_protocol_select_button.Width;
    net_protocol_option_button.CallBackFn = ac_do_net_protocol_option;
    net_INITIATE_button.CallBackFn = ac_do_net_INITIATE;
    net_faction_box.SpecialDrawFn = ac_show_net_faction_box;
    net_team_box.SpecialDrawFn = ac_show_net_team_box;
    net_groups_box.Flags |= GBxFlg_RadioBtn|GBxFlg_IsMouseOver;

    net_groups_box.SpecialDrawFn = ac_show_net_groups_box;
    net_users_box.SpecialDrawFn = ac_show_net_users_box;
    net_benefits_box.SpecialDrawFn = ac_show_net_benefits_box;
    net_unkn40_button.CallBackFn = ac_do_net_unkn40;
    net_SET_button.CallBackFn = ac_do_net_SET;
    net_protocol_select_button.CallBackFn = ac_do_net_protocol_select;
    unkn8_EJECT_button.CallBackFn = ac_do_unkn8_EJECT;
    net_comms_box.SpecialDrawFn = ac_show_net_comms_box;
    net_users_box.Flags |= GBxFlg_RadioBtn|GBxFlg_IsMouseOver;
    net_groups_LOGON_button.CallBackFn = ac_do_net_groups_LOGON;
    net_unkn21.SpecialDrawFn = ac_show_net_unkn21;
    net_SET2_button.CallBackFn = ac_do_net_SET2;
    net_protocol_box.SpecialDrawFn = ac_show_net_protocol_box;

    start_x = (scr_w - unkn13_SYSTEM_button.Width - 16 - net_groups_box.Width - 9 - net_users_box.Width - 7) / 2;

    net_groups_box.X = start_x + unkn13_SYSTEM_button.Width + 16;
    net_users_box.X = net_groups_box.X + net_groups_box.Width + 9;
    net_faction_box.X = start_x + unkn13_SYSTEM_button.Width + 16;;
    net_team_box.X = net_faction_box.X + net_faction_box.Width + 9;
    net_benefits_box.X = net_team_box.X + net_team_box.Width + 9;
    net_protocol_box.X = start_x + 7;
    net_unkn21.X = start_x + 7;
    net_comms_box.X = net_unkn21.X + net_unkn21.Width + 9;

    // Two buttons on top of each other
    net_protocol_select_button.X = net_protocol_box.X +
      (net_protocol_box.Width - net_protocol_select_button.Width) / 2;
    net_unkn40_button.X = net_protocol_box.X +
      (net_protocol_box.Width - net_unkn40_button.Width) / 2;

    net_protocol_option_button.X = net_protocol_box.X +
      (net_protocol_box.Width - net_protocol_option_button.Width) / 2;

    net_INITIATE_button.X = net_groups_box.X + 5;
    net_groups_LOGON_button.X = net_groups_box.X + 5;

    unkn8_EJECT_button.X = net_groups_box.X + net_groups_box.Width - 76;
    net_SET2_button.X = net_benefits_box.X + net_benefits_box.Width - 71;
    net_SET_button.X = net_benefits_box.X + net_benefits_box.Width - 71;
}

void reset_net_screen_boxes_flags(void)
{
    net_users_box.Flags = GBxFlg_Unkn0001;
    net_groups_box.Flags = GBxFlg_Unkn0001;
    net_benefits_box.Flags = GBxFlg_Unkn0001;
    net_team_box.Flags = GBxFlg_Unkn0001;
    net_faction_box.Flags = GBxFlg_Unkn0001;
    net_comms_box.Flags = GBxFlg_Unkn0001;
    net_unkn21.Flags = GBxFlg_Unkn0001;
    net_protocol_box.Flags = GBxFlg_Unkn0001;
}

void reset_net_screen_EJECT_flags(void)
{
    unkn8_EJECT_button.Flags = GBxFlg_Unkn0001;
}

void set_flag01_net_screen_boxes(void)
{
    net_SET2_button.Flags |= GBxFlg_Unkn0001;
    net_SET_button.Flags |= GBxFlg_Unkn0001;
    net_INITIATE_button.Flags |= GBxFlg_Unkn0001;
    net_protocol_option_button.Flags |= GBxFlg_Unkn0001;
    net_protocol_select_button.Flags |= GBxFlg_Unkn0001;
    net_unkn40_button.Flags |= GBxFlg_Unkn0001;
    unkn8_EJECT_button.Flags |= GBxFlg_Unkn0001;
    net_groups_LOGON_button.Flags |= GBxFlg_Unkn0001;
}

void switch_net_screen_boxes_to_initiate(void)
{
    net_INITIATE_button.Flags = GBxFlg_Unkn0001;
    net_INITIATE_button.Text = gui_strings[385];
    net_groups_LOGON_button.Text = gui_strings[386];
}

void switch_net_screen_boxes_to_execute(void)
{
    net_INITIATE_button.Text = gui_strings[387];
    if (byte_1C4A6F)
        net_groups_LOGON_button.Text = gui_strings[520];
    else
        net_groups_LOGON_button.Text = gui_strings[388];
}

/******************************************************************************/
