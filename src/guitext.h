/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guitext.h
 *     Header file for guitext.c.
 * @par Purpose:
 *     GUI text for menu and in-game display.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GUITEXT_H
#define GUITEXT_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#define STRINGS_MAX 2000

#pragma pack(1)

enum GuiStrings {
    GSTR_MTR_SCIENTISTS_DEAD = 537,
    // Per-campaign, name for players cyborgs
    GSTR_CPG_CYBORG_NOUN_FULL = 608,
    // Mission metrics
    GSTR_MTR_MISSION_REF = 611,
    GSTR_MTR_MISSION_STATUS = 612,
    GSTR_MTR_OBJCTV_N_COMPL = 613,
    GSTR_MTR_CITY_NAME = 614,
    GSTR_MTR_MISS_TIME_ELP = 615,
    GSTR_MTR_CREDS_NET_GAIN = 616,
    GSTR_MTR_EFFICIENCY = 617,
    GSTR_MTR_PERSD_CIVIL = 618,
    GSTR_MTR_PERSD_MILIT = 619,
    GSTR_MTR_PERSD_ADVERS = 620,
    GSTR_MTR_KILLD_CIVIL = 621,
    GSTR_MTR_KILLD_MILIT = 622,
    GSTR_MTR_KILLD_ADVERS = 623,
    GSTR_MTR_CYBORGS_LOST = 624,
    GSTR_MTR_LAST_CITY_NAME = 629,
    GSTR_MTR_CYBORGS_GAIN = 630,
    GSTR_MTR_RESEARCH_DONE = 631,
    GSTR_MTR_CREDS_EXPENDIT = 632,
    GSTR_MTR_CREDS_INCOME = 633,
    // Names for mods, per type
    GSTR_MOD_EPIDERMIS = 645,
    // Group: Network messages - 1950..1999
    // Network messages during mission
    GSTR_NET_LEFT_GAME = 1960,
    GSTR_NET_HOSQ_QUIT = 1961,
};

#pragma pack()
/******************************************************************************/
extern char *gui_strings[STRINGS_MAX];
extern char *gui_strings_data;
extern char *gui_strings_data_end;
extern ulong text_buf_pos;

void snprint_dh_time_duration(char *out, ulong outlen, long ndays, short nhours);

/** Converts any kind of text pointer to a global text pointer.
 *
 * Global text pointers are pointers within scratch buffers, valid for
 * whole drawing the current frame. Such pointers can be used in drawlists.
 */
const char *loctext_to_gtext(const char *ltext);

void read_strings_file(void);
TbBool create_strings_list(char **strings, char *strings_data, char *strings_data_end);

TbBool flashy_draw_text(int x, int y, const char *text, ubyte speed,
  int top_line, short *textpos, int cyan_flag);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
