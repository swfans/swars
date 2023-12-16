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

#include "bfkeybd.h"
#include "bftext.h"
#include "bfmath.h"
#include "campaign.h"
#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "lvobjctv.h"
#include "scanner.h"
#include "sound.h"
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

extern ubyte byte_1C47D8;
extern ubyte byte_1C47D9;
extern long dword_1C47DC;
extern long dword_1C47E0;
extern ubyte byte_1C47E4;
extern short word_1C47E6;
extern short word_1C47E8;

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

void flic_netscan_open_anim(ubyte anim_no)
{
    ulong k;

    k = anim_slots[9];
    sprintf(animations[k].Filename, "data/equip/net%02d.fli", anim_no);
    flic_unkn03(9);
}

void purple_unkn2_data_to_screen(void)
{
    ubyte *o;
    ubyte *inp;
    ubyte dy;

    o = &lbDisplay.WScreen[73 * lbDisplay.PhysicalScreenWidth + 8];
    inp = unkn_buffer_05;
    for (dy = 0; dy < 198; dy++)
    {
        memcpy(o, inp, 320);
        o += lbDisplay.PhysicalScreenWidth;
        inp += 320;
    }
}

ubyte show_citymap_box(struct ScreenBox *box)
{
#if 1
    ubyte ret;
    asm volatile ("call ASM_show_citymap_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
#else
    short text_h;
    short ms_x, ms_y;
    ubyte anim_no;
    ushort i;
    int n;

    if (city_id == -1)
    {
        byte_1C47D8 = 0;
        byte_1C47D9 = 0;
        unkstruct04_id = -1;
    }
    if (unkstruct04_id != byte_1C47E4)
        byte_1C47D9 = 0;
    byte_1C47E4 = unkstruct04_id;
    if (!byte_1C47D9 && (unkstruct04_id != -1))
    {
        anim_no = netscan_objectives[unkstruct04_id].AnimNo;
        if (anim_no) {
            byte_1C47D9 = 1;
            flic_netscan_open_anim(anim_no);
        }
    }
    if (!byte_1C47D8)
    {
        short dy;
        lbFontPtr = small_med_font;
        text_h = font_height('A');
        my_set_text_window(box->X + 4, box->Y + 4,
            box->Width - 8, box->Height - 8);
        lbDisplay.DrawFlags = 0x0100;
        draw_text_purple_list2(0, 0, gui_strings[483], 0);
        dy = text_h + 4;
        city_id = -1;
        dword_1C47DC = 0;
        *brief_NETSCAN_COST_box.Text2 = 0;
        brief_NETSCAN_COST_box.Text1 = gui_strings[495];
        for (i = 0; i < num_cities; i++)
        {
            if (cities[i].Flags & 0x01) {
                city_id = i;
                ++dword_1C47DC;
            }
        }
        if (dword_1C47DC != 1)
        {
            city_id = -1;
            for (i = 0; i < num_cities; i++)
            {
                if ((cities[i].Flags & 0x01) == 0)
                    continue;

                dy += text_h + 4;
                lbDisplay.DrawFlags |= 0x8000;
                draw_text_purple_list2(0, dy, (char*) &memload[cities[i].TextIndex[0]], 0);
                lbDisplay.DrawFlags &= ~0x8000;
                if (lbDisplay.LeftButton)
                {
                    ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
                    ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
                    if ((ms_x >= text_window_x1) && (ms_x < text_window_x2) &&
                      (ms_y >= dy + text_window_y1) && (ms_y < text_h + text_window_y1 + dy))
                    {
                        lbDisplay.LeftButton = 0;
                        city_id = i;
                        play_sample_using_heap(0, 111, 127, 64, 100, 0, 2u);
                    }
                }
            }
        }
        if (city_id != -1)
        {
            TbFileHandle fh;
            char locstr[52];

            unkstruct04_id = -1;
            for (i = 0; i < 16; i++)
            {
                ingame.Scanner.BigBlip[i].Period = 0;
            }
            sprintf(locstr, "maps/map%03d.scn", cities[city_id].MapID);
            fh = LbFileOpen(locstr, Lb_FILE_MODE_READ_ONLY);
            if (fh != INVALID_FILE)
            {
                LbFileRead(fh, SCANNER_data, 0x10000);
                LbFileClose(fh);
                load_netscan_objectives(cities[city_id].MapID, cities[city_id].Level);
                load_netscan_text_data(city_id, cities[city_id].Level);
                if (cities[city_id].Info)
                {
                    ushort nsobv = 0;
                    n = 0;
                    unkstruct04_id = 0;
                    for (i = 0; i < 5; i++)
                    {
                        if (netscan_objectives[nsobv].Z[i] || netscan_objectives[nsobv].X[i])
                        {
                            short x, z;
                            ingame.Scanner.BigBlip[n].Colour = 87;
                            x = netscan_objectives[nsobv].X[i];
                            ingame.Scanner.BigBlip[n].Period = 32;
                            ingame.Scanner.BigBlip[n].X = x << 15;
                            ingame.Scanner.BigBlip[n].Counter = 32;
                            z = netscan_objectives[nsobv].Z[i];
                            ingame.Scanner.BigBlip[n].Speed = 4;
                            ingame.Scanner.BigBlip[n].Z = z << 15;
                            n++;
                        }
                    }
                }
            }
            byte_1C47D8 = 1;
        }
    }

    if (byte_1C47D9)
    {
        if (!netscan_objectives[unkstruct04_id].AnimNo)
            byte_1C47D9 = 0;
        if (xdo_next_frame(9))
            byte_1C47D9 = 0;
        draw_flic_purple_list(purple_unkn2_data_to_screen);
    }
    else if (byte_1C47D8)
    {
        if (mouse_move_over_rect(box->X, box->X + box->Width, box->Y, box->Y + box->Height))
        {
            int dx, dy;
            short sdx, sdy;

            if (lbDisplay.RightButton)
            {
                lbDisplay.RightButton = 0;
                ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
                ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
                word_1C47E6 = ms_x;
                word_1C47E8 = ms_y;
            }
            if (lbKeyOn[KC_PGDOWN])
            {
                if (dword_1C47E0 > 0)
                    dword_1C47E0 = -dword_1C47E0;
                dword_1C47E0--;
                dword_1C47E0 += dword_1C47E0 >> 2;
                if (dword_1C47E0 < -256)
                    dword_1C47E0 = -256;
            }
            else if (lbKeyOn[KC_DELETE])
            {
                if (dword_1C47E0 < 0)
                    dword_1C47E0 = -dword_1C47E0;
                dword_1C47E0++;
                dword_1C47E0 += dword_1C47E0 >> 2;
                if (dword_1C47E0 > 256)
                    dword_1C47E0 = 256;
            }
            else
            {
                dword_1C47E0 -= dword_1C47E0 >> 2;
                if ((dword_1C47E0 >= 0) && (dword_1C47E0 <= 4))
                    dword_1C47E0--;
                if ((dword_1C47E0 <= 0) && (dword_1C47E0 >= -4))
                    dword_1C47E0++;
            }
            dx = 0;
            dy = 0;
            ingame.Scanner.Angle = ((dword_1C47E0 >> 2) + ingame.Scanner.Angle) & 0x7FF;
            if (lbKeyOn[KC_RIGHT])
                dx++;
            if (lbKeyOn[KC_LEFT])
                dx--;
            if (lbKeyOn[KC_UP])
                dy--;
            if (lbKeyOn[KC_DOWN])
                dy++;
            ingame.Scanner.MX += dx * lbSinTable[ingame.Scanner.Angle + 512] >> 13;
            ingame.Scanner.MX += dy * lbSinTable[ingame.Scanner.Angle] >> 13;
            sdx = dx * lbSinTable[ingame.Scanner.Angle] >> 13;
            sdy = dy * lbSinTable[ingame.Scanner.Angle + 512] >> 13;
            ingame.Scanner.MZ += sdx - sdy;
            if (ingame.Scanner.MX < 0)
                ingame.Scanner.MX = 0;
            if (ingame.Scanner.MZ < 0)
                ingame.Scanner.MZ = 0;
            if (ingame.Scanner.MX > 256)
                ingame.Scanner.MX = 256;
            if (ingame.Scanner.MZ > 256)
                ingame.Scanner.MZ = 256;
        }
        draw_flic_purple_list(SCANNER_data_to_screen);
        if (dword_1C47DC != 1)
            draw_hotspot_purple_list(box->X + (box->Width >> 1), box->Y + (box->Height >> 1));
        ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
        ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
        if ((ms_x >= box->X) && (ms_x < box->X + box->Width) &&
          (ms_y >= box->Y) && (ms_y < box->Height + box->Y))
        {
            if (lbDisplay.LeftButton && (dword_1C47DC != 1))
            {
                byte_1C47D8 = 0;
            }
        }
    }
    return 0;
#endif
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
    short scr_w, start_x;

    scr_w = 640;//lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&brief_netscan_box, 7u, 281u, 322u, 145, 6, small_med_font, 3);
    init_screen_button(&brief_NETSCAN_button, 312u, 405u,
      gui_strings[441], 6, med2_font, 1, 0x80);
    init_screen_info_box(&brief_NETSCAN_COST_box, 12u, 405u, 213u,
      gui_strings[442], unkn39_text, 6, med_font, small_med_font, 1);
    brief_NETSCAN_COST_box.Text2 = brief_netscan_cost_text;
    brief_NETSCAN_button.CallBackFn = ac_brief_do_netscan_enhance;
    brief_netscan_box.Flags |= 0x0300;
    brief_netscan_box.DrawTextFn = ac_show_brief_netscan_box;

    init_screen_text_box(&brief_mission_text_box, 338u, 72u, 295u, 354, 6, small_font, 3);
    init_screen_button(&unkn1_ACCEPT_button, 343u, 405u,
      gui_strings[436], 6, med2_font, 1, 0);
    init_screen_button(&unkn1_CANCEL_button, 616u, 405u,
      gui_strings[437], 6, med2_font, 1, 0x80);
    brief_mission_text_box.Buttons[0] = &unkn1_ACCEPT_button;
    brief_mission_text_box.Buttons[1] = &unkn1_CANCEL_button;
    brief_mission_text_box.Flags |= 0x0300;
    brief_mission_text_box.Text = mission_briefing_text;
    unkn1_ACCEPT_button.CallBackFn = ac_accept_mission;
    unkn1_CANCEL_button.CallBackFn = ac_do_unkn1_CANCEL;

    init_screen_box(&brief_graphical_box, 7u, 72u, 322u, 200, 6);
    brief_graphical_box.SpecialDrawFn = ac_show_citymap_box;

    start_x = (scr_w - brief_graphical_box.Width - brief_mission_text_box.Width - 23) / 2;
    brief_graphical_box.X = start_x + 7;

    brief_netscan_box.X = start_x + 7;
    brief_NETSCAN_button.X = brief_netscan_box.X + brief_netscan_box.Width - brief_NETSCAN_button.Width - 17;
    brief_NETSCAN_COST_box.X = brief_netscan_box.X + 5;

    brief_mission_text_box.X = brief_graphical_box.X + brief_graphical_box.Width + 9;
    unkn1_ACCEPT_button.X = brief_mission_text_box.X + 5;
    unkn1_CANCEL_button.X = brief_mission_text_box.X + brief_mission_text_box.Width - unkn1_CANCEL_button.Width - 17;
}

void update_brief_screen_netscan_button(ushort text_id)
{
    const char *text;

    text = gui_strings[text_id];
    init_screen_button(&brief_NETSCAN_button, brief_netscan_box.X + brief_netscan_box.Width - 17, 405,
      text, 6, med2_font, 1, 128);
    brief_NETSCAN_COST_box.Width = brief_netscan_box.Width - 10 - brief_NETSCAN_button.Width - 17;
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
