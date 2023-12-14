/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file febrief.c
 *     Front-end desktop and menu system, e-mail mission brief screen.
 * @par Purpose:
 *     Implement functions for e-mail mission brief screen in front-end desktop.
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
#include "febrief.h"

#include "bftext.h"
#include "campaign.h"
#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "lvobjctv.h"
#include "scanner.h"
#include "wadfile.h"
#include "wrcities.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenButton brief_NETSCAN_button;
extern struct ScreenInfoBox brief_NETSCAN_COST_box;
extern struct ScreenTextBox brief_netscan_box;

extern struct ScreenTextBox brief_mission_text_box;
extern struct ScreenButton unkn1_ACCEPT_button;
extern struct ScreenButton unkn1_CANCEL_button;

extern struct ScreenBox brief_graphical_box;

extern sbyte unkstruct04_id;// = -1;
extern char unkn39_text[];

ubyte ac_brief_do_netscan_enhance(ubyte click);
ubyte ac_show_brief_netscan_box(struct ScreenTextBox *box);
ubyte ac_show_citymap_box(struct ScreenBox *box);
ubyte ac_accept_mission(ubyte click);
ubyte ac_do_unkn1_CANCEL(ubyte click);

void update_netscan_cost_button(ubyte city_id)
{
    int k;
    ushort ninfo;
    char *text;

    ninfo = cities[city_id].Info;
    if (ninfo >= netscan_objectives_count) {
        brief_NETSCAN_COST_box.Text2[0] = '\0';
        text = gui_strings[495];
    } else {
        k = 100 * netscan_objectives[ninfo].CreditCost;
        sprintf(brief_NETSCAN_COST_box.Text2, "%d", k);
        text = gui_strings[442];
    }
    brief_NETSCAN_COST_box.Text1 = text;
}

void reveal_netscan_objective(ushort info)
{
    brief_netscan_box.Lines += netscan_objectives[info].TextLines;
}

ubyte brief_do_netscan_enhance(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_brief_do_netscan_enhance\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    ushort nsobv;
    ushort n;
    struct NetscanObjective *p_nsobv;

    nsobv = cities[city_id].Info;
    p_nsobv = &netscan_objectives[nsobv];

    if (!player_try_spend_money(100 * p_nsobv->CreditCost))
        return 0;

    cities[city_id].Info++;
    update_netscan_cost_button(city_id);
    unkstruct04_id = nsobv;
    reveal_netscan_objective(unkstruct04_id);
    brief_netscan_box.Flags |= 0x80;
    recount_city_credit_reward(city_id);

    clear_all_scanner_signals();
    for (n = 0; n < 5; n++)
    {
        if (p_nsobv->Z[n] || p_nsobv->X[n]) {
            add_blippoint_to_scanner(p_nsobv->X[n] << 15, p_nsobv->Z[n] << 15, 87);
        }
    }
    return 1;
}

ubyte show_brief_netscan_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_brief_netscan_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_citymap_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_citymap_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte load_mail_text(const char *filename)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_mail_text\n"
        : "=r" (ret) : "a" (filename));
    return ret;
#endif
    int totlen;
    char *p;

    p = mission_briefing_text;
    p[0] = '\\';
    p[1] = 'c';
    p[2] = '3';

    totlen = load_file_alltext(filename, p + 3);
    if (totlen == Lb_FAIL) {
        return 0;
    }
    if (3+totlen >= mission_briefing_text_len) {
        LOGERR("Insufficient memory for mission_briefing_text - %d instead of %d", mission_briefing_text_len, 3+totlen);
        totlen = mission_briefing_text_len - 3 - 1;
    }
    p = mission_briefing_text;
    p[3 + totlen] = '\0';
    my_preprocess_text(p);
    return Lb_SUCCESS;
}

void brief_load_mission_info(void)
{
    char fname[FILENAME_MAX];

    set_heading_box_text(gui_strings[372]);
    data_1c4aa2 = 0;
    brief_netscan_box.Lines = 0;

    if (open_brief != 0)
    {
        if (open_brief < 0) {
            short email;
            email = -open_brief - 1;
            sprintf(fname, "%s/mail%03d.txt", "textdata", email_store[email].Mission);
        } else if (open_brief > 0) {
            ushort missi;
            missi = brief_store[open_brief - 1].Mission;
            sprintf(fname, "%s/miss%03d.txt", "textdata", mission_list[missi].SourceID);
        }
        load_mail_text(fname);
        brief_mission_text_box.Lines = 0;
    }
}

void load_netscan_data(ubyte city_id, ubyte level)
{
#if 0
    asm volatile ("call ASM_load_netscan_data\n"
        : : "a" (city_id), "d" (a2));
#else
    int i;

    my_set_text_window(brief_netscan_box.X + 4, brief_netscan_box.ScrollWindowOffset + brief_netscan_box.Y + 4,
      brief_netscan_box.Width - 20, brief_netscan_box.ScrollWindowHeight);
    lbFontPtr = small_med_font;
    brief_netscan_box.Lines = 0;
    load_netscan_text_data(cities[city_id].MapID, level);

    for (i = 0; i < netscan_objectives_count; i++)
    {
        if (netscan_objectives[i].CreditCost == 0)
            reveal_netscan_objective(i);
    }

    if (cities[city_id].Info == 0)
    {
        for (i = 0; i < netscan_objectives_count; i++)
        {
            if (netscan_objectives[i].CreditCost)
                break;
        }
        cities[city_id].Info = i;
    }
    recount_city_credit_reward(city_id);
    update_netscan_cost_button(city_id);
#endif
}

void init_brief_screen_boxes(void)
{
    init_screen_text_box(&brief_netscan_box, 7u, 281u, 322u, 145, 6, small_med_font, 3);
    init_screen_button(&brief_NETSCAN_button, 312u, 405u,
      gui_strings[441], 6, med2_font, 1, 128);
    init_screen_info_box(&brief_NETSCAN_COST_box, 12u, 405u, 213u,
      gui_strings[442], unkn39_text, 6, med_font, small_med_font, 1);
    brief_NETSCAN_COST_box.Width = 312 - brief_NETSCAN_button.Width - 17;
    brief_NETSCAN_COST_box.Text2 = brief_netscan_cost_text;
    brief_NETSCAN_button.CallBackFn = ac_brief_do_netscan_enhance;
    brief_netscan_box.Flags |= 0x0300;
    brief_netscan_box.DrawTextFn = ac_show_brief_netscan_box;

    init_screen_text_box(&brief_mission_text_box, 338u, 72u, 295u, 354, 6, small_font, 3);
    init_screen_button(&unkn1_ACCEPT_button, 343u, 405u,
      gui_strings[436], 6, med2_font, 1, 0);
    init_screen_button(&unkn1_CANCEL_button, 616u, 405u,
      gui_strings[437], 6, med2_font, 1, 128);
    brief_mission_text_box.Buttons[0] = &unkn1_ACCEPT_button;
    brief_mission_text_box.Buttons[1] = &unkn1_CANCEL_button;
    brief_mission_text_box.Flags |= 0x0300;
    brief_mission_text_box.Text = mission_briefing_text;
    unkn1_ACCEPT_button.CallBackFn = ac_accept_mission;
    unkn1_CANCEL_button.CallBackFn = ac_do_unkn1_CANCEL;

    init_screen_box(&brief_graphical_box, 7u, 72u, 322u, 200, 6);
    brief_graphical_box.SpecialDrawFn = ac_show_citymap_box;
}

void update_brief_screen_netscan_button(ushort text_id)
{
    const char *text;

    text = gui_strings[text_id];
    init_screen_button(&brief_NETSCAN_button, 312, 405,
      text, 6, med2_font, 1, 128);
    brief_NETSCAN_COST_box.Width = 312 - brief_NETSCAN_button.Width - 17;
    brief_NETSCAN_button.CallBackFn = ac_brief_do_netscan_enhance;
}

void reset_brief_screen_boxes_flags(void)
{
    brief_NETSCAN_COST_box.Flags = 0x0001;
    brief_netscan_box.Flags = 0x0001 | 0x0100 | 0x0200;
    brief_graphical_box.Flags = 0x0001;
    brief_mission_text_box.Flags = 0x0001 | 0x0100 | 0x0200;
}

void set_flag01_brief_screen_boxes(void)
{
    brief_NETSCAN_button.Flags |= 0x0001;
}

/******************************************************************************/
