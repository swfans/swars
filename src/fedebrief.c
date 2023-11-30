/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fedebrief.c
 *     Front-end desktop and menu system, mission debrief screen.
 * @par Purpose:
 *     Implement functions for mission debrief screen in front-end desktop.
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
#include "fedebrief.h"

#include "bftext.h"
#include "bffont.h"
#include "bfutility.h"
#include "display.h"
#include "guiboxes.h"
#include "guitext.h"
#include "game.h"
#include "thing.h"
#include "player.h"
#include "research.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenBox debrief_mission_box;
extern struct ScreenBox debrief_people_box;

const ushort mod_group_type_strid[] = {74, 71, 72, 70, 73, };

extern ushort word_1C4846[8];
extern ushort word_1C4856[8];

ubyte ac_show_mission_stats(struct ScreenBox *box);
ubyte ac_show_mission_people_stats(struct ScreenBox *box);

ubyte show_mission_stats(struct ScreenBox *box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_show_mission_stats\n"
        : "=r" (ret) : "a" (box));
    return ret;
#endif
    char *text;
    char locstr[40];
    int fheight, lnheight;
    int x, y;

    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;

    if ((box->Flags & 0x8000) == 0)
    {
        lbDisplay.DrawFlags = 0;

        // Row with names
        x = 20;
        y = lnheight;

        // Reference no
        draw_text_purple_list2(x, y, gui_strings[611], 0);
        y += lnheight;

        // Status
        draw_text_purple_list2(x, y, gui_strings[612], 0);
        y += lnheight;

        // City time
        draw_text_purple_list2(x, y, gui_strings[614], 0);
        y += lnheight;

        // Mission time
        draw_text_purple_list2(x, y, gui_strings[615], 0);
        y += lnheight;
        y += lnheight;

        // Income
        draw_text_purple_list2(x, y, gui_strings[633], 0);
        y += lnheight;

        // Expenditure
        draw_text_purple_list2(x, y, gui_strings[632], 0);
        y += lnheight;

        if (new_weapons_researched || new_mods_researched)
        {
            // Research completed
            draw_text_purple_list2(x, y, gui_strings[631], 0);
            y += lnheight;
        }
        if (scientists_lost > 0)
        {
            // Scientists lost
            draw_text_purple_list2(x, y, gui_strings[537], 0);
            y += lnheight;
            load_scientist_lost_reason(LbRandomAnyShort() % 20);
        }

        // Row with values
        x = 300;
        y = lnheight;

        // Reference no
        snprintf(locstr, sizeof(locstr), "%d", (int)byte_1C4AA3);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%s", gui_strings[635 + ingame.MissionStatus]);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprint_dh_time_duration(locstr, sizeof(locstr),
          mission_status[open_brief].CityDays, mission_status[open_brief].CityHours);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprint_dh_time_duration(locstr, sizeof(locstr),
          mission_status[open_brief].Days, mission_status[open_brief].Hours);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;
        y += lnheight;

        // Income
        snprintf(locstr, sizeof(locstr), "%ld C",
          (ingame.Credits + ingame.Expenditure) - ingame.CashAtStart);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        // Expenditure
        snprintf(locstr, sizeof(locstr), "%ld C", ingame.Expenditure);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        ingame.CashAtStart = ingame.Credits;
        ingame.Expenditure = 0;
        box->Flags |= 0x8000;
        copy_box_purple_list(box->X, box->Y, box->Width, box->Height);
    }

    x = 300;
    y = 8 * lnheight;
    locstr[0] = '\0';
    if (new_weapons_researched)
    {
        ushort wtype;
        ushort strid;
        ushort pos;

        wtype = WEP_TYPES_COUNT;
        while (1)
        {
            wtype = weapons_prev_weapon(new_weapons_researched, wtype);
            if (wtype == 0)
                break;

            if (strlen(locstr) > sizeof(locstr) - 4)
                break;

            if (background_type == 1)
                strid = 30 + wtype;
            else
                strid = 0 + wtype;

            pos = strlen(locstr);
            if (pos == 0)
                snprintf(locstr, sizeof(locstr), "%s", gui_strings[strid]);
            else
                snprintf(locstr + pos, sizeof(locstr) - pos, ", %s", gui_strings[strid]);
        }
    }
    if (new_mods_researched)
    {
        ushort mtype, mgrouptype;
        ushort gt_strid;
        ushort mv_strid;
        ushort pos;

        mtype = MOD_TYPES_COUNT;
        while (1)
        {
            mtype = cybmodflags_prev_mod(new_mods_researched, mtype);
            if (mtype == 0)
                break;

            if (strlen(locstr) > sizeof(locstr) - 4)
                break;

            mgrouptype = cybmod_group_type(mtype);
            gt_strid = mod_group_type_strid[mgrouptype];
            if (mgrouptype == 4)
                mv_strid = 75;
            else
                mv_strid = 76;

            pos = strlen(locstr);
            if (pos == 0)
                snprintf(locstr, sizeof(locstr), "%s %s %d",
                  gui_strings[gt_strid], gui_strings[mv_strid], cybmod_version(mtype));
            else
                snprintf(locstr + pos, sizeof(locstr) - pos, ", %s %s %d",
                  gui_strings[gt_strid], gui_strings[mv_strid], cybmod_version(mtype));
        }
    }
    if (locstr[0] != '\0')
    {
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;
    }

    if (scientists_lost > 0)
    {
        snprintf(locstr, sizeof(locstr), "%d: %s", scientists_lost, scientist_lost_reason);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;
    }
    return 0;
}

ubyte show_mission_people_stats(struct ScreenBox *box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_show_mission_people_stats\n"
        : "=r" (ret) : "a" (box));
    return ret;
#endif
    char *text;
    char locstr[40];
    int fheight, lnheight, textw;
    int x, y;

    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;

    if ((box->Flags & 0x8000) == 0)
    {
        lbDisplay.DrawFlags = 0;

        if (screentype == SCRT_9)
        {
            // Row with names
            x = 10;
            y = lnheight;

            draw_text_purple_list2(x, y, gui_strings[618], 0);
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[619], 0);
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[620], 0);
            y += lnheight;
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[621], 0);
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[622], 0);
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[623], 0);
            y += lnheight;
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[624], 0);
            y += lnheight;

            draw_text_purple_list2(x, y, gui_strings[630], 0);
            y += lnheight;
        }
        else
        {
            int i, k;
            int used_num; // Player number counting existing player; starts at 1
            int x1, x2, x3;

            y = 2 * lnheight;
            used_num = 1;
            for (i = 0; i < 8; i++)
            {
                k = byte_1C5C28[i];
                word_1C4856[k] = 0;
            }

            x1 = 20;
            x3 = 25;
            for (i = 0; i < 8; i++)
            {
                ushort plyr;
                ushort *p_stat;

                if (unkn2_names[i][0] == '\0')
                    continue;

                plyr = (players[i].MyAgent[0]->U.UPerson.ComCur & 0x1C) >> 2;
                word_1C4846[i] = 0;
                // The CivsKilled property should start a list of ushort stats; there are 6 of these
                p_stat = &mission_status[plyr].CivsKilled;
                for (k = 0; k < 8; k++, p_stat++)
                {
                    if (unkn2_names[k][0] == '\0')
                        continue;

                    if (k == i)
                        word_1C4846[i] -= *p_stat;
                    else
                        word_1C4846[i] += *p_stat;
                }

                k = byte_1C5C28[i];
                if (k != 0)
                    word_1C4856[k] += word_1C4846[i];

                snprintf(locstr, sizeof(locstr), "%d", used_num);
                text = (char *)(back_buffer + text_buf_pos);
                strcpy(text, locstr);
                textw = my_string_width(text);
                draw_text_purple_list2(x1 - textw, y, text, 0);
                x2 = 140 + 40 * (used_num - 1);
                draw_text_purple_list2(x2 - textw, lnheight, text, 0);
                text_buf_pos += strlen(text) + 1;
                draw_text_purple_list2(x3, y, unkn2_names[i], 0);
                y += lnheight;

                used_num++;
            }
        }
        box->Flags |= 0x8000;
        copy_box_purple_list(box->X, box->Y, box->Width, box->Height);
    }

    if (screentype == SCRT_9)
    {
        // Row with values
        x = 300;
        y = lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].CivsPersuaded);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].SecurityPersuaded);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].EnemiesPersuaded);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].CivsKilled);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].SecurityKilled);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].EnemiesKilled);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].AgentsLost);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;

        snprintf(locstr, sizeof(locstr), "%d", mission_status[open_brief].AgentsGained);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        draw_text_purple_list2(x, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        y += lnheight;
    }
    else
    {
        int i, k;

        y = 2 * lnheight;
        lbDisplay.DrawColour = 87;

        for (i = 0; i < 8; i++)
        {
            ushort plyr;
            ushort *p_stat;

            if (unkn2_names[i][0] == '\0')
                continue;

            x = 140;

            plyr = (players[i].MyAgent[0]->U.UPerson.ComCur & 0x1C) >> 2;
            p_stat = &mission_status[plyr].CivsKilled;
            for (k = 0; k < 8; k++, p_stat++)
            {
                if (unkn2_names[k][0] == '\0')
                    continue;

                if (k == i)
                    lbDisplay.DrawFlags |= 0x0040;
                snprintf(locstr, sizeof(locstr), "%d", *p_stat);
                text = (char *)(back_buffer + text_buf_pos);
                strcpy(text, locstr);
                textw = my_string_width(text);
                draw_text_purple_list2(x - textw, y, text, 0);
                text_buf_pos += strlen(text) + 1;
                if (k == i)
                    lbDisplay.DrawFlags &= ~0x0040;
                x += 40;
            }
            lbDisplay.DrawFlags |= 0x0040;

            snprintf(locstr, sizeof(locstr), "%d", word_1C4846[i]);
            text = (char *)(back_buffer + text_buf_pos);
            strcpy(text, locstr);
            textw = my_string_width(text);
            draw_text_purple_list2(x - textw, y, text, 0);
            text_buf_pos += strlen(text) + 1;

            k = byte_1C5C28[i];
            if (k != 0)
            {
                x += 1;
                draw_text_purple_list2(x, y, "/", 0);
                x += LbTextCharWidth('/') + 1;

                snprintf(locstr, sizeof(locstr), "%d", word_1C4856[k]);
                text = (char *)(back_buffer + text_buf_pos);
                strcpy(text, locstr);
                draw_text_purple_list2(x, y, text, 0);
                text_buf_pos += strlen(text) + 1;
            }
            lbDisplay.DrawFlags &= ~0x0040;
            y += lnheight;
        }

        x = 140;
        lbDisplay.DrawFlags |= 0x0040;

        for (i = 0; i != 8; i++)
        {
            ushort plyr;
            ushort *p_stat;
            int n;

            if (unkn2_names[i][0] == '\0')
                continue;

            n = 0;
            plyr = (players[k].MyAgent[0]->U.UPerson.ComCur & 0x1C) >> 2;
            p_stat = &mission_status[plyr].CivsKilled;
            for (k = 0; k != 8; k++, p_stat++)
            {

                if (unkn2_names[k][0] == '\0')
                    continue;

                n += *p_stat;
            }
            snprintf(locstr, sizeof(locstr), "%d", n);
            text = (char *)(back_buffer + text_buf_pos);
            strcpy(text, locstr);
            textw = my_string_width(text);
            draw_text_purple_list2(x - textw, y, text, 0);
            text_buf_pos += strlen(text) + 1;
            x += 40;
        }
        lbDisplay.DrawFlags &= ~0x0040;
    }
    return 0;
}

void init_debrief_screen_boxes(void)
{
    init_screen_box(&debrief_mission_box, 7u, 72u, 518u, 172, 6);
    debrief_mission_box.SpecialDrawFn = ac_show_mission_stats;

    init_screen_box(&debrief_people_box, 7u, 253u, 518u, 173, 6);
    debrief_people_box.SpecialDrawFn = ac_show_mission_people_stats;
}

void set_flag01_debrief_screen_boxes(void)
{
    debrief_people_box.Flags = 0x0001;
    debrief_mission_box.Flags = 0x0001;
}

/******************************************************************************/
