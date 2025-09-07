/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feresearch.c
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
#include "feresearch.h"

#include "bftext.h"
#include "bfsprite.h"
#include "femain.h"
#include "guiboxes.h"
#include "guigraph.h"
#include "guitext.h"
#include "cybmod.h"
#include "purpldrw.h"
#include "weapon.h"
#include "research.h"
#include "display.h"
#include "game_sprts.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenTextBox research_unkn21_box;
extern struct ScreenButton research_submit_button;
extern struct ScreenButton unkn12_WEAPONS_MODS_button;
extern struct ScreenTextBox research_progress_button;
extern struct ScreenBox research_graph_box;
extern struct ScreenButton research_list_buttons[2];

extern ubyte research_on_weapons;// = true;
extern ubyte research_unkn_var_01;

ubyte ac_do_research_submit(ubyte click);
ubyte ac_do_research_suspend(ubyte click);
ubyte ac_do_unkn12_WEAPONS_MODS(ubyte click);
ubyte ac_show_unkn21_box(struct ScreenTextBox *box);

TbBool research_weapon_daily_progress(void)
{
    short prev, lost;

    prev = research.CurrentWeapon;
    lost = research_daily_progress_for_type(0);
    scientists_lost += lost;
    if (research.CurrentWeapon != prev)
        new_weapons_researched |= 1 << prev;

    return (lost != 0) || (research.CurrentWeapon != prev);
}

TbBool research_cybmod_daily_progress(void)
{
    short prev, lost;

    prev = research.CurrentMod;
    lost = research_daily_progress_for_type(1);
    scientists_lost += lost;
    if (research.CurrentMod != prev)
        new_mods_researched |= 1 << prev;

    return (lost != 0) || (research.CurrentWeapon != prev);
}

/** If an agent in Cryo Chamber owns a weapon, allow its research.
 */
void research_allow_weapons_in_cryo(void)
{
    asm volatile ("call ASM_research_allow_weapons_in_cryo\n"
        :  :  : "eax" );
}

void forward_research_progress_after_mission(int num_days)
{
    int i;

    // TODO clear the data after filling research report, not here - there may be items accumulated
    // by time progress while waiting in menu; also clear on game load and new game
    new_mods_researched = 0;
    new_weapons_researched = 0;
    scientists_lost = 0;
    for (i = 0; i < num_days; i++)
    {
        research_weapon_daily_progress();
        research_cybmod_daily_progress();
    }
    research_allow_weapons_in_cryo();
}

ubyte do_research_submit(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_research_submit\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_research_suspend(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_research_suspend\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_unkn12_WEAPONS_MODS(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_unkn12_WEAPONS_MODS\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte show_unkn21_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_unkn21_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void draw_unkn20_subfunc_01(int x, int y, char *text, ubyte a4)
{
    asm volatile (
      "call ASM_draw_unkn20_subfunc_01\n"
        : : "a" (x), "d" (y), "b" (text), "c" (a4));
}

void show_research_screen(void)
{
    asm volatile ("call ASM_show_research_screen\n"
        :  :  : "eax" );
}

ubyte show_research_graph(struct ScreenBox *box)
{
    char *text;
    ushort *y_vals;
    ushort y_trend_delta;
    int n_y_vals;
    int graph_days, done_days;
    int x, y, w, h;

    // Make actual graph grid not larger than x=80%/y=75% of the box,
    // and with exact same grid squares
    w = box->Width * 80 / 100;
    w -= w % 10; // for original resolution it's 320
    h = box->Height * 75 / 100;
    h -= h % 10; //  // for original resolution it's 240
    // Prepare position where the graph area starts (the actual graph grid)
    x = box->X + (box->Width - w) * 65 / 100; // for original res 7 + 57
    y = box->Y + (box->Height - h) * 31 / 100; // for original res 103 + 25

    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        int twidth;

        lbFontPtr = small_med_font;
        my_set_text_window(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

        draw_chartxy_axis_y_values(x, y, h, 0, 100, 10);

        draw_chartxy_axis_y_grid(x, y, w, h, 10);
        draw_chartxy_axis_x_grid(x, y, w, h, 10);

        draw_text_purple_list2(x - 12, y + h + 6, misc_text[0], 0);
        lbFontPtr = med_font;
        my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8,  box->Height - 8);
        text = gui_strings[453];
        twidth = my_string_width(text);
        draw_text_purple_list2((box->Width - 8 - twidth) >> 1, 290, text, 0);
        text = gui_strings[452];
        draw_unkn20_subfunc_01(10, 31, text, 2);

        box->Flags |= GBxFlg_BkgndDrawn;
        copy_box_purple_list(box->X, box->Y, box->Width, box->Height);
    }

    graph_days = 0;
    if (research_on_weapons)
        done_days = research.WeaponDaysDone[research.CurrentWeapon];
    else
        done_days = research.ModDaysDone[research.CurrentMod];
    if (done_days >= 10) {
        graph_days = done_days - 9;
        n_y_vals = 10;
    } else {
        n_y_vals = done_days + 1;
    }
    lbFontPtr = small_med_font;
    my_set_text_window(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
    draw_chartxy_axis_x_values(x, y + h, w, graph_days+1, graph_days+11, 10);

    LbScreenSetGraphicsWindow(x - 1, y, w + 3, h + 2);
    if (research_on_weapons)
    {
        if (research.CurrentWeapon != -1)
        {
            struct WeaponDef *wdef;

            y_vals = &research.WeaponProgress[research.CurrentWeapon][0];
            wdef = &weapon_defs[research.CurrentWeapon + 1];
            y_trend_delta = research_unkn_func_004(wdef->PercentPerDay, wdef->Funding, research.WeaponFunding);

            draw_chartxy_curve(1, 0, w, h, y_vals, n_y_vals, RESEARCH_COMPLETE_POINTS, y_trend_delta, 10);
        }
    }
    else
    {
        if (research.CurrentMod != -1)
        {
            struct ModDef *mdef;

            y_vals = &research.ModProgress[research.CurrentMod][0];
            mdef = &mod_defs[research.CurrentMod + 1];
            y_trend_delta = research_unkn_func_004(mdef->PercentPerDay, mdef->Funding, research.ModFunding);

            draw_chartxy_curve(1, 0, w, h, y_vals, n_y_vals, RESEARCH_COMPLETE_POINTS, y_trend_delta, 10);
        }
    }

    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
    draw_chartxy_axis_x_main(x, y + h, w);
    draw_chartxy_axis_y_main(x, y, h);
    return 0;
}

void init_research_screen_boxes(void)
{
    int i, val;
    const char *s;
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_box(&research_graph_box, 7u, 103u, 409u, 322, 6);
    init_screen_text_box(&research_progress_button, 7u, 72u, 409u, 23,
      6, med_font, 1);
    init_screen_text_box(&research_unkn21_box, 425u, 72u, 208u, 353,
      6, small_med_font, 3);
    init_screen_button(&research_submit_button, 430u, 302u,
      gui_strings[418], 6, med2_font, 1, 0);

    init_screen_button(&unkn12_WEAPONS_MODS_button, 616u, 302u,
        gui_strings[450], 6, med2_font, 1, 0x80);
    init_screen_button(&research_list_buttons[0], 425u, 404u,
     gui_strings[478], 6, med2_font, 1, 0);
    init_screen_button(&research_list_buttons[1], 425u, 404u,
      gui_strings[479], 6, med2_font, 1, 0);

    val = 0;
    for (i = 0; i < 2; i++)
    {
        research_list_buttons[i].Radio = &ingame.AutoResearch;
        research_list_buttons[i].RadioValue = val;
        research_list_buttons[i].Flags |= GBxFlg_RadioBtn;
        val++;
    }

    unkn12_WEAPONS_MODS_button.CallBackFn = ac_do_unkn12_WEAPONS_MODS;
    unkn12_WEAPONS_MODS_button.Text = gui_strings[451];

    research_unkn21_box.DrawTextFn = ac_show_unkn21_box;
    research_unkn21_box.ScrollWindowHeight = 180;
    research_unkn21_box.Buttons[0] = &research_submit_button;
    research_unkn21_box.Buttons[1] = &unkn12_WEAPONS_MODS_button;
    research_unkn21_box.ScrollWindowOffset += 41;
    research_submit_button.CallBackFn = ac_do_research_submit;
    research_progress_button.DrawTextFn = ac_show_title_box;
    research_submit_button.Text = gui_strings[417];
    research_progress_button.Text = gui_strings[449];
    research_unkn21_box.Flags |= GBxFlg_RadioBtn|GBxFlg_IsMouseOver;

    lbFontPtr = med2_font;
    research_graph_box.SpecialDrawFn = show_research_graph;

    if (my_string_width(gui_strings[418]) <= my_string_width(gui_strings[417]))
        s = gui_strings[417];
    else
        s = gui_strings[418];
    research_submit_button.Width = my_string_width(s) + 4;

    if (my_string_width(gui_strings[451]) <= my_string_width(gui_strings[450]))
        s = gui_strings[450];
    else
        s = gui_strings[451];
    unkn12_WEAPONS_MODS_button.Width = my_string_width(s) + 4;

    start_x = (scr_w - research_graph_box.Width - research_unkn21_box.Width - 23) / 2;

    research_graph_box.X = start_x + 7;
    research_progress_button.X = start_x + 7;
    research_unkn21_box.X = research_graph_box.X + research_graph_box.Width + 9;
    research_submit_button.X = research_unkn21_box.X + 5;
    unkn12_WEAPONS_MODS_button.X = research_unkn21_box.X + research_unkn21_box.Width -
      17 - unkn12_WEAPONS_MODS_button.Width;

    // Middle of first half od the panel
    research_list_buttons[0].X = research_unkn21_box.X +
      (research_unkn21_box.Width / 2 - research_list_buttons[0].Width) / 2;
    // Middle of 2nd half od the panel
    research_list_buttons[1].X = research_unkn21_box.X + research_unkn21_box.Width -
      research_unkn21_box.Width / 2 + (research_unkn21_box.Width / 2 - research_list_buttons[1].Width) / 2;
}

void reset_research_screen_boxes_flags(void)
{
    research_unkn21_box.Flags = GBxFlg_Unkn0001 | GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
    research_graph_box.Flags = GBxFlg_Unkn0001;
    research_progress_button.Flags = GBxFlg_Unkn0001;
}

void set_flag01_research_screen_boxes(void)
{
    research_submit_button.Flags |= GBxFlg_Unkn0001;
    research_list_buttons[1].Flags |= GBxFlg_Unkn0001;
    research_list_buttons[0].Flags |= GBxFlg_Unkn0001;
    unkn12_WEAPONS_MODS_button.Flags |= GBxFlg_Unkn0001;
}

void clear_research_screen(void)
{
    research_unkn21_box.Lines = 0;
}

/******************************************************************************/
