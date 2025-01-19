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
#include "bfmemut.h"
#include "bfscrcopy.h"
#include "campaign.h"
#include "femail.h"
#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "purpldrw.h"
#include "display.h"
#include "game_sprts.h"
#include "game.h"
#include "keyboard.h"
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

extern sbyte selected_netscan_objective;// = -1;
extern char unkn39_text[];

extern ubyte brief_state_city_selected;
extern ubyte byte_1C47D9;

/** Amount of cities available in brief of a selected mail message.
 */
extern long mail_num_active_cities;

extern long dword_1C47E0;
extern ubyte byte_1C47E4;
extern short word_1C47E6;
extern short word_1C47E8;

ubyte ac_brief_do_netscan_enhance(ubyte click);
ubyte ac_show_brief_netscan_box(struct ScreenTextBox *box);
ubyte ac_show_citymap_box(struct ScreenBox *box);
ubyte ac_accept_mission(ubyte click);
ubyte ac_do_unkn1_CANCEL(ubyte click);
void ac_purple_unkn2_data_to_screen(void);
void ac_SCANNER_data_to_screen(void);

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

void reveal_netscan_objective(short nsobv)
{
    struct NetscanObjective *p_nsobv;

    p_nsobv = &netscan_objectives[nsobv];
    brief_netscan_box.Lines += p_nsobv->TextLines;
}

void brief_citymap_readd_scanner_signals(void)
{
    struct NetscanObjective *p_nsobv;

    p_nsobv = &netscan_objectives[selected_netscan_objective];
    add_netscan_signal_to_scanner(p_nsobv, 1);
}

ubyte brief_do_netscan_enhance(ubyte click)
{
    short nsobv;
    struct NetscanObjective *p_nsobv;

    nsobv = cities[selected_city_id].Info;
    p_nsobv = &netscan_objectives[nsobv];

    if (!player_try_spend_money(100 * p_nsobv->CreditCost))
        return 0;

    cities[selected_city_id].Info++;
    update_netscan_cost_button(selected_city_id);
    selected_netscan_objective = nsobv;
    reveal_netscan_objective(selected_netscan_objective);
    brief_netscan_box.Flags |= GBxFlg_Unkn0080;
    recount_city_credit_reward(selected_city_id);
    brief_citymap_readd_scanner_signals();
    return 1;
}

TbBool mouse_over_text_window_item(short tx_height, short margin, short start_shift, short n_lines)
{
    short ln_height;
    short lines_y1, lines_y2;

    ln_height = tx_height + margin;
    // Allow one pixel higher or lower
    lines_y1 = text_window_y1 + start_shift - 1;
    lines_y2 = lines_y1 + n_lines * ln_height - margin + 1;

    return mouse_down_over_box_coords(text_window_x1, lines_y1, text_window_x2, lines_y2);
}

ubyte show_brief_netscan_box(struct ScreenTextBox *box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_show_brief_netscan_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
#endif
    int nlines;
    short start_shift;
    short nsobv;
    short tx_height, ln_height;
    short border, margin, scrollbar_width;
    ubyte drawn = true;

    border = 4;
    margin = 4;
    scrollbar_width = 12;
    if (open_brief < 0)
    {
        draw_noise_box_purple_list(box->X, box->Y, box->Width, box->Height);
        return 0;
    }

    my_set_text_window(box->X + border, box->ScrollWindowOffset + box->Y + border,
      box->Width - 2 * border - scrollbar_width, box->ScrollWindowHeight);
    lbFontPtr = small_med_font;
    tx_height = font_height('A');
    ln_height = tx_height + margin;
    nlines = 0;
    start_shift = border - ln_height * box->field_38;
    lbDisplay.DrawColour = 87;
    lbDisplay.DrawFlags = 0;
    if (selected_city_id != -1)
    {
        for (nsobv = 0; nsobv < cities[selected_city_id].Info; nsobv++)
        {
            struct NetscanObjective *p_nsobv;

            p_nsobv = &netscan_objectives[nsobv];
            if (nlines + p_nsobv->TextLines >= box->field_38)
            {
                if (lbDisplay.LeftButton)
                {
                    if (mouse_over_text_window_item(tx_height, margin, start_shift, p_nsobv->TextLines))
                    {
                        selected_netscan_objective = nsobv;
                        brief_citymap_readd_scanner_signals();
                    }
                }
                if (selected_netscan_objective == nsobv)
                    lbDisplay.DrawFlags |= 0x0040;
                draw_text_purple_list2(0, start_shift, netscan_text + p_nsobv->TextOffset, 0);
                lbDisplay.DrawFlags = 0;
                start_shift += ln_height * p_nsobv->TextLines;
                if (start_shift + tx_height >= text_window_y2 - text_window_y1)
                    break;
            }
            nlines += p_nsobv->TextLines;
        }
        if (cities[selected_city_id].Info < netscan_objectives_count) {
            //drawn = brief_NETSCAN_button.DrawFn(&brief_NETSCAN_button); -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (drawn) : "a" (&brief_NETSCAN_button), "g" (brief_NETSCAN_button.DrawFn));
        }
    }
    if (drawn) {
        //brief_NETSCAN_COST_box.DrawFn(&brief_NETSCAN_COST_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
          : "=r" (drawn) : "a" (&brief_NETSCAN_COST_box), "g" (brief_NETSCAN_COST_box.DrawFn));
    }
    return 0;
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
    LbScreenSetGraphicsWindow(brief_graphical_box.X + 1, brief_graphical_box.Y + 1,
      brief_graphical_box.Width - 2, brief_graphical_box.Height - 2);
    LbScreenCopy(unkn_buffer_05, lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsWindowHeight);
    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
}

void load_netscan_map(ushort mapno)
{
    TbFileHandle fh;
    char locstr[52];

    sprintf(locstr, "maps/map%03d.scn", mapno);
    fh = LbFileOpen(locstr, Lb_FILE_MODE_READ_ONLY);
    if (fh != INVALID_FILE) {
        LbFileRead(fh, SCANNER_data, 0x10000);
        LbFileClose(fh);
    } else {
        LbMemorySet(SCANNER_data, 0, 0x10000);
    }
}

/** Counts amount of selectable cities, sets selected if single found.
 */
void count_selectable_cities(void)
{
    short city_id, last_city_id;

    mail_num_active_cities = 0;
    for (city_id = 0; city_id < num_cities; city_id++)
    {
        if (cities[city_id].Flags & CitF_Unkn01) {
            last_city_id = city_id;
            mail_num_active_cities++;
        }
    }
    // If found only one city, set it as selected
    if (mail_num_active_cities == 1)
        selected_city_id = last_city_id;
}

void show_citymap_city_selection(struct ScreenBox *box)
{
    short city_id;
    short text_h;
    short dy;
    ulong bufpos;

    text_h = font_height('A');
    dy = text_h + 4;
    for (city_id = 0; city_id < num_cities; city_id++)
    {
        if ((cities[city_id].Flags & CitF_Unkn01) == 0)
            continue;

        dy += text_h + 4;
        lbDisplay.DrawFlags |= 0x8000;
        bufpos = cities[city_id].TextIndex[0];
        draw_text_purple_list2(0, dy, (char*) &memload[bufpos], 0);
        lbDisplay.DrawFlags &= ~0x8000;
    }
}

ubyte input_citymap_city_selection(struct ScreenBox *box)
{
    short city_id;
    short text_h;
    short dy;

    text_h = font_height('A');
    dy = text_h + 4;
    for (city_id = 0; city_id < num_cities; city_id++)
    {
        if ((cities[city_id].Flags & CitF_Unkn01) == 0)
            continue;

        dy += text_h + 4;
        if (lbDisplay.LeftButton)
        {
            if (mouse_move_over_box_coords(text_window_x1, text_window_y1 + dy,
              text_window_x2, text_window_y1 + dy + text_h))
            {
                lbDisplay.LeftButton = 0;
                selected_city_id = city_id;
                play_sample_using_heap(0, 111, 127, 64, 100, 0, 2u);
                return 1;
            }
        }
    }
    return 0;
}

ubyte input_citymap_scanner(struct ScreenBox *box)
{
    int dx, dy;
    short sdx, sdy;
    ubyte ret;

    ret = 0;
    if (lbDisplay.RightButton)
    {
        short ms_x, ms_y;
        lbDisplay.RightButton = 0;
        ms_x = (lbDisplay.GraphicsScreenHeight < 400) ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
        ms_y = (lbDisplay.GraphicsScreenHeight < 400) ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
        word_1C47E6 = ms_x;
        word_1C47E8 = ms_y;
        ret = 1;
    }
    if (lbKeyOn[KC_PGDOWN])
    {
        if (dword_1C47E0 > 0)
            dword_1C47E0 = -dword_1C47E0;
        dword_1C47E0--;
        dword_1C47E0 += dword_1C47E0 >> 2;
        if (dword_1C47E0 < -256)
            dword_1C47E0 = -256;
        ret = 1;
    }
    else if (lbKeyOn[KC_DELETE])
    {
        if (dword_1C47E0 < 0)
            dword_1C47E0 = -dword_1C47E0;
        dword_1C47E0++;
        dword_1C47E0 += dword_1C47E0 >> 2;
        if (dword_1C47E0 > 256)
            dword_1C47E0 = 256;
        ret = 1;
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
    if (lbKeyOn[KC_RIGHT]) {
        dx++;
        ret = 1;
    }
    if (lbKeyOn[KC_LEFT]) {
        dx--;
        ret = 1;
    }
    if (lbKeyOn[KC_UP]) {
        dy--;
        ret = 1;
    }
    if (lbKeyOn[KC_DOWN]) {
        dy++;
        ret = 1;
    }
    ingame.Scanner.MX += dx * lbSinTable[ingame.Scanner.Angle + LbFPMath_PI/2] >> 13;
    ingame.Scanner.MX += dy * lbSinTable[ingame.Scanner.Angle] >> 13;
    sdx = dx * lbSinTable[ingame.Scanner.Angle] >> 13;
    sdy = dy * lbSinTable[ingame.Scanner.Angle + LbFPMath_PI/2] >> 13;
    ingame.Scanner.MZ += sdx - sdy;
    if (ingame.Scanner.MX < 0)
        ingame.Scanner.MX = 0;
    if (ingame.Scanner.MZ < 0)
        ingame.Scanner.MZ = 0;
    if (ingame.Scanner.MX > 256)
        ingame.Scanner.MX = 256;
    if (ingame.Scanner.MZ > 256)
        ingame.Scanner.MZ = 256;
    return ret;
}

ubyte show_citymap_box(struct ScreenBox *box)
{
    ubyte anim_no;

    if (selected_city_id == -1)
    {
        brief_state_city_selected = 0;
        byte_1C47D9 = 0;
        selected_netscan_objective = -1;
    }
    if (selected_netscan_objective != byte_1C47E4)
        byte_1C47D9 = 0;
    byte_1C47E4 = selected_netscan_objective;
    if (!byte_1C47D9 && (selected_netscan_objective != -1))
    {
        anim_no = netscan_objectives[selected_netscan_objective].AnimNo;
        if (anim_no) {
            byte_1C47D9 = 1;
            flic_netscan_open_anim(anim_no);
        }
    }
    if (!brief_state_city_selected)
    {
        lbFontPtr = small_med_font;
        my_set_text_window(box->X + 4, box->Y + 4,
            box->Width - 8, box->Height - 8);
        lbDisplay.DrawFlags = Lb_TEXT_HALIGN_CENTER;
        draw_text_purple_list2(0, 0, gui_strings[483], 0);
        *brief_NETSCAN_COST_box.Text2 = 0;
        brief_NETSCAN_COST_box.Text1 = gui_strings[495];
        mail_num_active_cities = 0;
        selected_city_id = -1;
        count_selectable_cities();
        if (mail_num_active_cities != 1)
        {
            show_citymap_city_selection(box);
            input_citymap_city_selection(box);
        }
        if (selected_city_id != -1)
        {
            selected_netscan_objective = -1;
            load_netscan_map(cities[selected_city_id].MapID);
            load_netscan_objectives(cities[selected_city_id].MapID, cities[selected_city_id].Level);
            load_netscan_data(selected_city_id, cities[selected_city_id].Level);
            if (cities[selected_city_id].Info != 0)
            {
                selected_netscan_objective = 0;
                brief_citymap_readd_scanner_signals();
            }
            brief_state_city_selected = 1;
        }
    }

    if (byte_1C47D9)
    {
        if (!netscan_objectives[selected_netscan_objective].AnimNo)
            byte_1C47D9 = 0;
        if (xdo_next_frame(9))
            byte_1C47D9 = 0;
        draw_flic_purple_list(ac_purple_unkn2_data_to_screen);
    }
    else if (brief_state_city_selected)
    {
        if (mouse_move_over_box(box))
        {
            input_citymap_scanner(box);
        }
        draw_flic_purple_list(ac_SCANNER_data_to_screen);
        if (mail_num_active_cities != 1)
            draw_hotspot_purple_list(box->X + (box->Width >> 1), box->Y + (box->Height >> 1));
        if (mouse_move_over_box(box))
        {
            if (lbDisplay.LeftButton && (mail_num_active_cities != 1))
            {
                brief_state_city_selected = 0;
            }
        }
    }
    return 0;
}

ubyte load_mail_text(const char *filename)
{
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
        LOGERR("Insufficient memory for mission_briefing_text - %d instead of %d",
          mission_briefing_text_len, 3+totlen);
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
}

ubyte show_mission_screen(void)
{
#if 0
    asm volatile ("call ASM_show_mission_screen\n"
        :  :  : "eax" );
#endif
    ubyte drawn = true;

    if (((game_projector_speed != 0) && is_heading_flag01()) ||
      (is_key_pressed(KC_SPACE, KMod_DONTCARE) && !edit_flag))
    {
        clear_key_pressed(KC_SPACE);
        set_flag02_heading_screen_boxes();
        brief_graphical_box.Flags |= GBxFlg_Unkn0002;
        brief_NETSCAN_button.Flags |= GBxFlg_Unkn0002;
        brief_mission_text_box.Flags |= GBxFlg_Unkn0002;
        brief_netscan_box.Flags |= GBxFlg_Unkn0002;
        brief_NETSCAN_COST_box.Flags |= GBxFlg_Unkn0002;
    }
    // Draw sequentially
    if (drawn)
        drawn = draw_heading_box();

    if (drawn)
    {
        //drawn = brief_mission_text_box.DrawFn(&brief_mission_text_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&brief_mission_text_box), "g" (brief_mission_text_box.DrawFn));
    }

    if (drawn)
    {
        if (lbKeyOn[KC_F])
        {
          if ( mouse_move_over_rect(
                 brief_mission_text_box.X,
                 brief_mission_text_box.Width + brief_mission_text_box.X,
                 brief_mission_text_box.Y,
                 brief_mission_text_box.Height + brief_mission_text_box.Y) )
          {
            lbKeyOn[KC_F] = 0;
            if ((brief_mission_text_box.Flags & 0x1000) == 0)
            {
              if (brief_mission_text_box.Font == small_font)
              {
                brief_mission_text_box.Font = small_med_font;
              }
              else if (brief_mission_text_box.Font == small_med_font)
              {
                brief_mission_text_box.Font = med_font;
              }
              else
              {
                brief_mission_text_box.Font = small_font;
              }
              brief_mission_text_box.Lines = 0;
              brief_mission_text_box.BGColour = 0;
              brief_mission_text_box.Flags |= GBxFlg_Unkn0080;
            }
          }
        }
        //drawn = brief_graphical_box.DrawFn(&brief_graphical_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&brief_graphical_box), "g" (brief_graphical_box.DrawFn));
    }

    if (drawn)
    {
        //drawn = brief_netscan_box.DrawFn(&brief_netscan_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&brief_netscan_box), "g" (brief_netscan_box.DrawFn));
    }

    return drawn;
}

void init_brief_screen_scanner(void)
{
    ingame.Scanner.MX = 127;
    ingame.Scanner.MZ = 127;
    ingame.Scanner.Angle = 0;
    ingame.Scanner.Zoom = 256;

    SCANNER_set_screen_box(brief_graphical_box.X + 1, brief_graphical_box.Y + 1,
      brief_graphical_box.Width - 4, brief_graphical_box.Height - 3, 0);
    clear_all_scanner_signals();
    SCANNER_width = ingame.Scanner.Width;
    SCANNER_init();
}

void init_brief_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&brief_netscan_box, 7, 281, 322, 145,
      6, small_med_font, 3);
    init_screen_button(&brief_NETSCAN_button, 312u, 405u,
      gui_strings[441], 6, med2_font, 1, 0x80);
    init_screen_info_box(&brief_NETSCAN_COST_box, 12u, 405u, 213u,
      gui_strings[442], unkn39_text, 6, med_font, small_med_font, 1);
    brief_NETSCAN_COST_box.Text2 = brief_netscan_cost_text;
    brief_NETSCAN_button.CallBackFn = ac_brief_do_netscan_enhance;
    brief_netscan_box.Flags |= (GBxFlg_RadioBtn|GBxFlg_IsMouseOver);
    brief_netscan_box.DrawTextFn = ac_show_brief_netscan_box;

    init_screen_text_box(&brief_mission_text_box, 338u, 72u, 295u, 354, 6, small_font, 3);
    init_screen_button(&unkn1_ACCEPT_button, 343u, 405u,
      gui_strings[436], 6, med2_font, 1, 0);
    init_screen_button(&unkn1_CANCEL_button, 616u, 405u,
      gui_strings[437], 6, med2_font, 1, 0x80);
    brief_mission_text_box.Buttons[0] = &unkn1_ACCEPT_button;
    brief_mission_text_box.Buttons[1] = &unkn1_CANCEL_button;
    brief_mission_text_box.Flags |= (GBxFlg_RadioBtn|GBxFlg_IsMouseOver);
    brief_mission_text_box.Text = mission_briefing_text;
    unkn1_ACCEPT_button.CallBackFn = ac_accept_mission;
    unkn1_CANCEL_button.CallBackFn = ac_do_unkn1_CANCEL;

    init_screen_box(&brief_graphical_box, 7u, 72u, 322u, 200, 6);
    brief_graphical_box.SpecialDrawFn = ac_show_citymap_box;

    start_x = (scr_w - brief_graphical_box.Width - brief_mission_text_box.Width - 23) / 2;
    brief_graphical_box.X = start_x + 7;

    brief_netscan_box.X = start_x + 7;
    brief_NETSCAN_COST_box.X = brief_netscan_box.X + 5;
    //no need to update brief_NETSCAN_button.X - it will happen on the update below
    update_brief_screen_netscan_button(441);

    brief_mission_text_box.X = brief_graphical_box.X + brief_graphical_box.Width + 9;
    unkn1_ACCEPT_button.X = brief_mission_text_box.X + 5;
    unkn1_CANCEL_button.X = brief_mission_text_box.X + brief_mission_text_box.Width - unkn1_CANCEL_button.Width - 17;
}

void update_brief_screen_netscan_button(ushort text_id)
{
    const char *text;

    text = gui_strings[text_id];
    init_screen_button(&brief_NETSCAN_button, brief_netscan_box.X + brief_netscan_box.Width - 17, 405,
      text, 6, med2_font, 1, 0x80);
    brief_NETSCAN_COST_box.Width = brief_netscan_box.Width - 10 - brief_NETSCAN_button.Width - 17;
    brief_NETSCAN_button.CallBackFn = ac_brief_do_netscan_enhance;
}

void reset_brief_screen_boxes_flags(void)
{
    brief_NETSCAN_COST_box.Flags = GBxFlg_Unkn0001;
    brief_netscan_box.Flags = GBxFlg_Unkn0001 | GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
    brief_graphical_box.Flags = GBxFlg_Unkn0001;
    brief_mission_text_box.Flags = GBxFlg_Unkn0001 | GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
}

void set_flag01_brief_screen_boxes(void)
{
    brief_NETSCAN_button.Flags |= GBxFlg_Unkn0001;
}

/******************************************************************************/
