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
#include "campaign.h"
#include "display.h"
#include "guiboxes.h"
#include "guitext.h"
#include "game_sprts.h"
#include "game.h"
#include "thing.h"
#include "misstat.h"
#include "player.h"
#include "purpldrw.h"
#include "research.h"
#include "swlog.h"
/******************************************************************************/
#define SCIENTIST_LOST_REASONS_COUNT 20
#define MISSION_STATS_SECOND_COLUMN_X 300
#define PEOPLE_STATS_SECOND_COLUMN_X 300

struct DebriefReport curr_report;

extern struct ScreenBox debrief_mission_box;
extern struct ScreenBox debrief_people_box;

const ushort mod_group_type_strid[] = {74, 71, 72, 70, 73, };

// Shared boxes
extern struct ScreenTextBox world_city_info_box;

extern ushort word_1C4846[8];
extern ushort word_1C4856[8];

ubyte ac_show_mission_stats(struct ScreenBox *box);
ubyte ac_show_mission_people_stats(struct ScreenBox *box);

void show_debrief_screen(void)
{
    asm volatile ("call ASM_show_debrief_screen\n"
        :  :  : "eax" );
}

void draw_mission_stats_names_column(struct ScreenBox *box,
  ubyte research_ln, ubyte scilost_ln)
{
    short fheight, lnheight, sepheight;
    short x, y;

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;
    sepheight = fheight + 4;

    if (research_ln + scilost_ln >= 4) // compact mode
        sepheight /= 2;

    // Row with names
    x = 20;
    y = sepheight;

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
    y += sepheight;

    // Income
    draw_text_purple_list2(x, y, gui_strings[633], 0);
    y += lnheight;

    // Expenditure
    draw_text_purple_list2(x, y, gui_strings[632], 0);
    y += lnheight;

    if (research_ln > 0)
    {
        draw_text_purple_list2(x, y, gui_strings[631], 0);
        y += lnheight * research_ln;
    }
    if (scilost_ln > 0)
    {
        // Scientists lost
        draw_text_purple_list2(x, y, gui_strings[537], 0);
        y += lnheight * scilost_ln;
    }
}

void draw_mission_stats_vals_static(struct ScreenBox *box,
  struct DebriefReport *p_rep, ubyte research_ln, ubyte scilost_ln)
{
    struct MissionStatus *p_mistat;
    char *text;
    char locstr[40];
    short fheight, lnheight, sepheight;
    short x, y;

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;
    sepheight = fheight + 4;

    if (research_ln + scilost_ln >= 4) // compact mode
        sepheight /= 2;

    // Row with values
    x = MISSION_STATS_SECOND_COLUMN_X;
    y = sepheight;
    p_mistat = &mission_status[p_rep->BriefNo];

    // Reference no
    snprintf(locstr, sizeof(locstr), "%hd", p_rep->RefNo);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%s", gui_strings[635 + p_rep->Status]);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprint_dh_time_duration(locstr, sizeof(locstr),
      p_mistat->CityDays, p_mistat->CityHours);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprint_dh_time_duration(locstr, sizeof(locstr),
      p_mistat->Days, p_mistat->Hours);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;
    y += sepheight;

    // Income
    snprintf(locstr, sizeof(locstr), "%ld C", p_rep->Income);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    // Expenditure
    snprintf(locstr, sizeof(locstr), "%ld C", p_rep->Expenditure);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;
}

void snprint_concat_comma_separated_weapons_list(char *out, ushort outlen, ulong weapons)
{
    ushort wtype;
    ushort strid;
    ushort pos;

    wtype = WEP_TYPES_COUNT;
    while (1)
    {
        struct Campaign *p_campgn;

        wtype = weapons_prev_weapon(weapons, wtype);
        if (wtype == 0)
            break;

        if (strlen(out) > outlen - 4u)
            break;

        p_campgn = &campaigns[background_type];
        strid = p_campgn->WeaponsTextIdShift + wtype - 1;

        pos = strlen(out);
        if (pos == 0)
            snprintf(out, outlen, "%s", gui_strings[strid]);
        else
            snprintf(out + pos, outlen - pos, ", %s", gui_strings[strid]);
    }
}

void snprint_concat_comma_separated_cybmods_list(char *out, ushort outlen, ulong cybmods)
{
    ushort mtype, mgrouptype;
    ushort gt_strid;
    ushort mv_strid;
    ushort pos;

    mtype = MOD_TYPES_COUNT;
    while (1)
    {
        mtype = cybmodflags_prev_mod(cybmods, mtype);
        if (mtype == 0)
            break;

        if (strlen(out) > outlen - 4u)
            break;

        mgrouptype = cybmod_group_type(mtype);
        gt_strid = mod_group_type_strid[mgrouptype];
        if (mgrouptype == 4)
            mv_strid = 75;
        else
            mv_strid = 76;

        pos = strlen(out);
        if (pos == 0)
            snprintf(out, outlen, "%s %s %d",
              gui_strings[gt_strid], gui_strings[mv_strid], cybmod_version(mtype));
        else
            snprintf(out + pos, outlen - pos, ", %s %s %d",
              gui_strings[gt_strid], gui_strings[mv_strid], cybmod_version(mtype));
    }
}

short new_weapomn_mods_text_width(struct DebriefReport *p_rep)
{
    char locstr[80];

    lbFontPtr = med_font;

    locstr[0] = '\0';
    snprint_concat_comma_separated_weapons_list(locstr, sizeof(locstr), p_rep->WeaponsResearched);
    snprint_concat_comma_separated_cybmods_list(locstr, sizeof(locstr), p_rep->ModsResearched);

    return LbTextStringWidth(locstr);
}

/** Returns width of the scientists died line in debriefing.
 * Requires scientist_lost_reason to be filled before the call.
 */
short scientists_died_text_width(struct DebriefReport *p_rep)
{
    char locstr[80];

    lbFontPtr = med_font;

    snprintf(locstr, sizeof(locstr), "%d: %s", p_rep->ScientistsLost, scientist_lost_reason);

    return LbTextStringWidth(locstr);
}

void draw_mission_stats_vals_dynamic(struct ScreenBox *box,
  struct DebriefReport *p_rep, ubyte research_ln, ubyte scilost_ln)
{
    short fheight, lnheight, sepheight;
    short x, y;
    char *text;
    char locstr[80];

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;
    sepheight = fheight + 4;

    if (research_ln + scilost_ln >= 4) // compact mode
        sepheight /= 2;

    x = MISSION_STATS_SECOND_COLUMN_X;
    y = sepheight + 4 * lnheight + sepheight + 2 * lnheight;

    if (research_ln > 0)
    {
        locstr[0] = '\0';
        snprint_concat_comma_separated_weapons_list(locstr, sizeof(locstr), p_rep->WeaponsResearched);
        snprint_concat_comma_separated_cybmods_list(locstr, sizeof(locstr), p_rep->ModsResearched);

        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        text_buf_pos += strlen(text) + 1;

        // If cannot fit in expected num of lines, use smaller font
        if (LbTextStringWidth(text) > research_ln * (box->Width - 8 - MISSION_STATS_SECOND_COLUMN_X))
            lbFontPtr = small_med_font;
        draw_text_purple_list2(x, y, text, 0);
        lbFontPtr = med_font;
        y += lnheight * research_ln;
    }

    if (scilost_ln > 0)
    {
        snprintf(locstr, sizeof(locstr), "%d: %s", p_rep->ScientistsLost, scientist_lost_reason);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        text_buf_pos += strlen(text) + 1;

        if (LbTextStringWidth(text) > scilost_ln * (box->Width - 8 - MISSION_STATS_SECOND_COLUMN_X)) {
            // Cannot fit in expected num of lines - divide the text into
            // amount and reason, and print the latter small
            char *text2;
            short dx, dy;

            text2 = strchr(text, ' ');
            text2[0] = '\0';
            text2++;
            draw_text_purple_list2(x, y, text, 0);
            dx = LbTextStringWidth(text) + 4;
            lbFontPtr = small_med_font;
            dy = fheight - font_height('A'); // print at same level as previous test
            draw_text_purple_list2(x + dx, y + dy, text2, 0);
            lbFontPtr = med_font;
        } else {
            draw_text_purple_list2(x, y, text, 0);
        }
        y += lnheight * scilost_ln;
    }
}

void debrief_report_fill(struct DebriefReport *p_rep)
{
    p_rep->RefNo = byte_1C4AA3;
    p_rep->BriefNo = open_brief;
    p_rep->Status = ingame.MissionStatus;
    p_rep->Income = (ingame.Credits + ingame.Expenditure) - ingame.CashAtStart;
    p_rep->Expenditure = ingame.Expenditure;
    LOGSYNC("Research completed wep=0x%lx mod=0x%lx", new_weapons_researched, new_mods_researched);
    p_rep->WeaponsResearched = new_weapons_researched;
    p_rep->ModsResearched = new_mods_researched;
    p_rep->ScientistsLost = scientists_lost;
    p_rep->SciLostReason = LbRandomAnyShort() % SCIENTIST_LOST_REASONS_COUNT;
}

ubyte show_mission_stats(struct ScreenBox *box)
{
    struct DebriefReport *p_rep;
    ubyte research_ln, scilost_ln;

    p_rep = &curr_report;
    research_ln = 0;
    scilost_ln = 0;

    // TODO maybe fill the report at higher level?
    if ((box->Flags & GBxFlg_BkgndDrawn) == 0) {
        debrief_report_fill(p_rep);
        ingame.CashAtStart = ingame.Credits;
        ingame.Expenditure = 0;

        if (p_rep->ScientistsLost > 0) {
            load_scientist_lost_reason(p_rep->SciLostReason);
        }
    }

    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);

    if ((p_rep->WeaponsResearched != 0) || (p_rep->ModsResearched != 0)) {
        short textw;
        research_ln += 1;
        textw = new_weapomn_mods_text_width(p_rep);
        if (textw > box->Width - 8 - MISSION_STATS_SECOND_COLUMN_X)
            research_ln += 1;
        if (textw > 2 * (box->Width - 8 - MISSION_STATS_SECOND_COLUMN_X) + 8)
            research_ln += 1;
    }
    if (p_rep->ScientistsLost > 0) {
        short textw;
        scilost_ln += 1;
        // use two lines for death reason only if we have unused space
        if (research_ln <= 2) {
            textw = scientists_died_text_width(p_rep);
            if (textw > box->Width - 8 - MISSION_STATS_SECOND_COLUMN_X)
                scilost_ln += 1;
        }
    }

    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        draw_mission_stats_names_column(box, research_ln, scilost_ln);

        draw_mission_stats_vals_static(box, p_rep, research_ln, scilost_ln);

        box->Flags |= GBxFlg_BkgndDrawn;
        copy_box_purple_list(box->X, box->Y, box->Width, box->Height);
    }

    draw_mission_stats_vals_dynamic(box, p_rep, research_ln, scilost_ln);
    return 0;
}

void draw_mission_people_stats_names_column(struct ScreenBox *box,
  struct DebriefReport *p_rep)
{
    int fheight, lnheight;
    short x, y;

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;

    // Row with names
    x = 20;
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

void draw_mission_people_stats_vals_column(struct ScreenBox *box,
  struct DebriefReport *p_rep)
{
    struct MissionStatus *p_mistat;
    char *text;
    char locstr[40];
    int fheight, lnheight;
    short x, y;

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;

    // Row with values
    x = PEOPLE_STATS_SECOND_COLUMN_X;
    y = lnheight;
    p_mistat = &mission_status[p_rep->BriefNo];

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->SP.CivsPersuaded);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->SP.SecurityPersuaded);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->SP.EnemiesPersuaded);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->SP.CivsKilled);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->SP.SecurityKilled);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->SP.EnemiesKilled);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->AgentsLost);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;

    snprintf(locstr, sizeof(locstr), "%d", p_mistat->AgentsGained);
    text = (char *)(back_buffer + text_buf_pos);
    strcpy(text, locstr);
    draw_text_purple_list2(x, y, text, 0);
    text_buf_pos += strlen(text) + 1;
    y += lnheight;
}

void draw_mission_mp_players_names_column(struct ScreenBox *box,
  struct DebriefReport *p_rep)
{
    char *text;
    char locstr[40];
    int fheight, lnheight;
    PlayerIdx plyr;
    int k;
    int used_num; // Player number counting existing player; starts at 1
    short x1, x2, x3;
    short y;

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;

    y = 2 * lnheight;
    used_num = 1;
    for (plyr = 0; plyr < PLAYERS_LIMIT; plyr++)
    {
        k = byte_1C5C28[plyr];
        word_1C4856[k] = 0;
    }

    x1 = 20;
    x3 = 25;
    for (plyr = 0; plyr < PLAYERS_LIMIT; plyr++)
    {
        int textw;

        if (unkn2_names[plyr][0] == '\0')
            continue;

        word_1C4846[plyr] = stats_mp_count_net_players_agents_kills(plyr);

        k = byte_1C5C28[plyr];
        if (k != 0)
            word_1C4856[k] += word_1C4846[plyr];

        snprintf(locstr, sizeof(locstr), "%d", used_num);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        textw = my_string_width(text);
        draw_text_purple_list2(x1 - textw, y, text, 0);
        x2 = 140 + 40 * (used_num - 1);
        draw_text_purple_list2(x2 - textw, lnheight, text, 0);
        text_buf_pos += strlen(text) + 1;
        draw_text_purple_list2(x3, y, unkn2_names[plyr], 0);
        y += lnheight;

        used_num++;
    }
}

void draw_mission_mp_players_vals_column(struct ScreenBox *box,
  struct DebriefReport *p_rep)
{
    char *text;
    char locstr[40];
    int fheight, lnheight;
    short x, y;
    int i, k;

    lbDisplay.DrawFlags = 0;
    lbFontPtr = med_font;
    fheight = font_height('A');
    lnheight = fheight + 4;

    y = 2 * lnheight;
    lbDisplay.DrawColour = 87;

    for (i = 0; i < PLAYERS_LIMIT; i++)
    {
        struct MissionStatus *p_mistat;
        int textw;
        ushort plyr;

        if (unkn2_names[i][0] == '\0')
            continue;

        x = 140;

        plyr = (players[i].MyAgent[0]->U.UPerson.ComCur & 0x1C) >> 2;
        p_mistat = &mission_status[plyr];
        for (k = 0; k < PLAYERS_LIMIT; k++)
        {
            if (unkn2_names[k][0] == '\0')
                continue;

            if (k == i)
                lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
            snprintf(locstr, sizeof(locstr), "%d", (int)p_mistat->MP.AgentsKilled[k]);
            text = (char *)(back_buffer + text_buf_pos);
            strcpy(text, locstr);
            textw = my_string_width(text);
            draw_text_purple_list2(x - textw, y, text, 0);
            text_buf_pos += strlen(text) + 1;
            if (k == i)
                lbDisplay.DrawFlags &= ~Lb_TEXT_ONE_COLOR;
            x += 40;
        }
        lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;

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
        lbDisplay.DrawFlags &= ~Lb_TEXT_ONE_COLOR;
        y += lnheight;
    }

    x = 140;
    lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;

    for (i = 0; i != PLAYERS_LIMIT; i++)
    {
        int textw;
        ushort plyr;
        int n;

        if (unkn2_names[i][0] == '\0')
            continue;

        plyr = (players[k].MyAgent[0]->U.UPerson.ComCur & 0x1C) >> 2;
        n = stats_mp_count_players_agents_killed(plyr);
        snprintf(locstr, sizeof(locstr), "%d", n);
        text = (char *)(back_buffer + text_buf_pos);
        strcpy(text, locstr);
        textw = my_string_width(text);
        draw_text_purple_list2(x - textw, y, text, 0);
        text_buf_pos += strlen(text) + 1;
        x += 40;
    }
    lbDisplay.DrawFlags &= ~Lb_TEXT_ONE_COLOR;
}

ubyte show_mission_people_stats(struct ScreenBox *box)
{
    struct DebriefReport *p_rep;

    p_rep = &curr_report;

    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);

    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        if (screentype == SCRT_DEBRIEF)
        {
            draw_mission_people_stats_names_column(box, p_rep);
        }
        else
        {
            draw_mission_mp_players_names_column(box, p_rep);
        }
        box->Flags |= GBxFlg_BkgndDrawn;
        copy_box_purple_list(box->X, box->Y, box->Width, box->Height);
    }

    if (screentype == SCRT_DEBRIEF)
    {
        draw_mission_people_stats_vals_column(box, p_rep);
    }
    else
    {
        draw_mission_mp_players_vals_column(box, p_rep);
    }
    return 0;
}

void init_debrief_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_box(&debrief_mission_box, 7u, 72u, 518u, 172, 6);
    debrief_mission_box.SpecialDrawFn = ac_show_mission_stats;

    init_screen_box(&debrief_people_box, 7u, 253u, 518u, 173, 6);
    debrief_people_box.SpecialDrawFn = ac_show_mission_people_stats;

    start_x = (scr_w - debrief_mission_box.Width - world_city_info_box.Width - 23) / 2;

    debrief_mission_box.X = start_x + 7;
    debrief_people_box.X = start_x + 7;
}

void reset_debrief_screen_boxes_flags(void)
{
    debrief_people_box.Flags = GBxFlg_Unkn0001;
    debrief_mission_box.Flags = GBxFlg_Unkn0001;
}

/******************************************************************************/
