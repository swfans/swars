/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file weapon.c
 *     Weapons related functions.
 * @par Purpose:
 *     Implement functions for handling weapons.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "weapon.h"

#include "bfmemory.h"
#include "bfendian.h"
#include "bffile.h"
#include "bfini.h"
#include "bfutility.h"
#include "ssampply.h"

#include "bigmap.h"
#include "network.h"
#include "thing.h"
#include "player.h"
#include "game.h"
#include "game_speed.h"
#include "thing_search.h"
#include "wadfile.h"
#include "sound.h"
#include "vehicle.h"
#include "swlog.h"
/******************************************************************************/

struct WeaponDef weapon_defs[] = {
    { 0,    0,  0,  0,   0,  0, 0, 0, WEPDFLG_None,          0, 0,     0,     0,  0},
    { 5,   50,  4,  5,   8, 10, 1, 1, WEPDFLG_CanPurchease, 16, 1,    40,    40, 10},
    { 8,  100,  3,  8,  20, 10, 1, 1, WEPDFLG_CanPurchease, 17, 1,   120,   120, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, WEPDFLG_CanPurchease|0x01|0x02, 19, 7,   370,   370, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, WEPDFLG_CanPurchease|0x01|0x02, 20, 7,   420,   420, 10},
    {12,  800, 30,  8, 300, 10, 1, 1, WEPDFLG_CanPurchease,  24, 1,   750,   750, 10},
    { 6,  800, 40,  8, 600, 10, 1, 1, WEPDFLG_CanPurchease,  21, 3,  1000,  1000, 10},
    { 1,    0, 10,  8,   2, 10, 1, 1, WEPDFLG_CanPurchease,  15, 9,   150,   150, 10},
    { 4,   80,  1, 16,   6,  5, 0, 0, WEPDFLG_CanPurchease,  23, 2,   160,   160, 10},
    { 0, 9999,  3, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  29, 7,   200,   200, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, WEPDFLG_CanPurchease,  33, 2,   100,   100, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, WEPDFLG_CanPurchease,  30, 2,    80,    80, 10},
    { 4,  300,  5, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  25, 7,   620,   620, 10},
    { 4, 9999, 40, 16, 200,  5, 0, 0, WEPDFLG_CanPurchease,  27, 1,   480,   480, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, WEPDFLG_None,          25, 2,   400,   400, 10},
    {16, 1300, 80,  8, 200, 10, 1, 1, WEPDFLG_CanPurchease,  28, 1,   300,   300, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  35, 3,  1050,  1050, 10},
    {10,  550,  2, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  26,10,  2000,  2000, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, WEPDFLG_CanPurchease,  18, 1,   100,   100, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, WEPDFLG_None,          24, 6,   850,   850, 10},
    {11, 1500, 10, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  38, 6, 65535, 65535, 10},
    { 2,    0, 10,  8,   2, 10, 1, 1, WEPDFLG_CanPurchease,  41, 9,   450,   450, 10},
    { 8, 9999, 20, 16, 200,  5, 0, 0, WEPDFLG_CanPurchease,  39, 6,  1100,  1100, 10},
    { 0, 9999,  1, 16,   4,  5, 0, 0, WEPDFLG_None,          14,10,    80,    80, 10},
    { 4, 9999,  1, 16, 200,  5, 0, 0, WEPDFLG_CanPurchease,  37, 6,  1200,  1200, 10},
    { 8,   25, 80, 16, 600,  5, 0, 0, WEPDFLG_CanPurchease,  40, 6,  1300,  1300, 10},
    { 2,  500, 50, 16, 600,  5, 0, 0, WEPDFLG_CanPurchease,  36, 6,   650,   650, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, WEPDFLG_CanPurchease,  22, 5,    30,    30, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, WEPDFLG_CanPurchease,  31, 5,   130,   130, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, WEPDFLG_CanPurchease,  32, 1,   950,   950, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  34, 5,  1000,  1000, 10},
    { 0,    0,  0, 16,   4,  5, 0, 0, WEPDFLG_None,          72, 5,  3000,    10,  8},
    { 0,    0,  0, 16,   4,  5, 0, 0, WEPDFLG_None,          72, 5,  3000,    10,  8},
};

ubyte weapon_tech_level[33] = {
  0, 1, 1, 3, 3, 5, 6, 2, 4, 3, 3, 2, 4, 4, 255, 5, 7, 8, 1, 255, 9, 6, 6, 255, 8, 7, 5, 2, 6, 7, 5, 255, 255,
};

struct WeaponDefAdd weapon_defs_a[33] = {0};
struct TbNamedEnum weapon_names[33] = {0};

short persuaded_person_weapons_sell_cost_permil = 0;

short low_energy_alarm_level = 50;

enum WeaponsConfigCmd {
    CCWep_WeaponsCount = 1,
    CCWep_Name,
    CCWep_Category,
    CCWep_RangeBlocks,
    CCWep_HitDamage,
    CCWep_ReFireDelay,
    CCWep_Weight,
    CCWep_EnergyUsed,
    CCWep_Accuracy,
    CCWep_TargetType,
    CCWep_TargetMode,
    CCWep_Sprite,
    CCWep_Cost,
    CCWep_Funding,
    CCWep_PercentPerDay,
};

const struct TbNamedEnum weapons_conf_common_cmds[] = {
  {"WeaponsCount",	CCWep_WeaponsCount},
  {NULL,		0},
};

const struct TbNamedEnum weapons_conf_weapon_cmds[] = {
  {"Name",			CCWep_Name},
  {"Category",	    CCWep_Category},
  {"RangeBlocks",	CCWep_RangeBlocks},
  {"HitDamage",		CCWep_HitDamage},
  {"ReFireDelay",	CCWep_ReFireDelay},
  {"Weight",		CCWep_Weight},
  {"EnergyUsed",	CCWep_EnergyUsed},
  {"Accuracy",		CCWep_Accuracy},
  {"TargetType",	CCWep_TargetType},
  {"TargetMode",	CCWep_TargetMode},
  {"Sprite",		CCWep_Sprite},
  {"Cost",			CCWep_Cost},
  {"ResearchFunding",		CCWep_Funding},
  {"ResearchPercentPerDay",	CCWep_PercentPerDay},
  {NULL,		0},
};

void read_weapons_conf_file(void)
{
    TbFileHandle conf_fh;
    TbBool done;
    int i;
    long k;
    int cmd_num;
    char *conf_buf;
    struct TbIniParser parser;
    char *conf_fname = "conf" FS_SEP_STR "weapons.ini";
    int conf_len;
    int weapons_count, wtype;

    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE) {
        conf_len = LbFileLengthHandle(conf_fh);
        if (conf_len > 1024*1024)
            conf_len = 1024*1024;
        conf_buf = LbMemoryAlloc(conf_len+16);
        conf_len = LbFileRead(conf_fh, conf_buf, conf_len);
        LOGSYNC("Processing %s file, %d bytes", conf_fname, conf_len);
        LbFileClose(conf_fh);
    } else {
        LOGERR("Could not open weapons config file, going with defaults.");
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    weapons_count = 0;
    // Parse the [common] section of loaded file
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return;
    }
    done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(weapons_conf_common_cmds,cmd_num)
    while (!done)
    {
        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, weapons_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case CCWep_WeaponsCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            weapons_count = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), weapons_count);
            break;
        case 0: // comment
            break;
        case -1: // end of buffer
        case -3: // end of section
            done = true;
            break;
        default:
            CONFWRNLOG("Unrecognized command.");
            break;
        }
        LbIniSkipToNextLine(&parser);
    }
#undef COMMAND_TEXT
    for (wtype = 0; wtype < weapons_count; wtype++)
    {
        char sect_name[16];
        struct WeaponDef *wdef;
        struct WeaponDefAdd *wdefa;

        // Parse the [weaponN] sections of loaded file
        sprintf(sect_name, "weapon%d", wtype);
        wdef = &weapon_defs[wtype];
        wdefa = &weapon_defs_a[wtype];
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
        done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(weapons_conf_weapon_cmds,cmd_num)
        while (!done)
        {
            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, weapons_conf_weapon_cmds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case CCWep_Name:
                i = LbIniValueGetStrWord(&parser, wdefa->Name, sizeof(wdefa->Name));
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)wdefa->Name);
                break;
            case CCWep_Category:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if ((k < 0) || (k > WEP_CATEGORIES_COUNT)) {
                    CONFWRNLOG("Outranged value of \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    k = WEP_CATEGORIES_COUNT;
                }
                wdef->Category = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(wdef->Category));
                break;
            case CCWep_RangeBlocks:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if ((k < 0) || (k > WEAPON_RANGE_BLOCKS_LIMIT)) {
                    CONFWRNLOG("Outranged value of \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    k = WEAPON_RANGE_BLOCKS_LIMIT;
                }
                wdef->RangeBlocks = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->RangeBlocks);
                break;
            case CCWep_HitDamage:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->HitDamage = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->HitDamage);
                break;
            case CCWep_ReFireDelay:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->ReFireDelay = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->ReFireDelay);
                break;
            case CCWep_Weight:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Weight = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Weight);
                break;
            case CCWep_EnergyUsed:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->EnergyUsed = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->EnergyUsed);
                break;
            case CCWep_Accuracy:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Accuracy = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Accuracy);
                break;
            case CCWep_TargetType:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->TargetType = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->TargetType);
                break;
            case CCWep_TargetMode:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->TargetMode = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->TargetMode);
                break;
            case CCWep_Sprite:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Sprite = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(wdef->Sprite));
                break;
            case CCWep_Cost:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Cost = k / 100;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Cost);
                break;
            case CCWep_Funding:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Funding = k / 100;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Funding);
                break;
            case CCWep_PercentPerDay:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->PercentPerDay = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->PercentPerDay);
                break;
            case 0: // comment
                break;
            case -1: // end of buffer
            case -3: // end of section
                done = true;
                break;
            default:
                CONFWRNLOG("Unrecognized command.");
                break;
            }
            LbIniSkipToNextLine(&parser);
        }
#undef COMMAND_TEXT
    }
#undef CONFDBGLOG
#undef CONFWRNLOG
    LbIniParseEnd(&parser);
    LbMemoryFree(conf_buf);

    i = 0;
    for (wtype = 1; wtype < weapons_count; wtype++)
    {
        struct WeaponDefAdd *wdefa;

        wdefa = &weapon_defs_a[wtype];
        if (strlen(wdefa->Name) > 0) {
            weapon_names[i].name = wdefa->Name;
            weapon_names[i].num = wtype;
            i++;
        }
    }
    weapon_names[i].name = NULL;
    weapon_names[i].num = 0;
}

const char *weapon_codename(ushort wtype)
{
    struct WeaponDefAdd *wdefa;

    if (wtype >= WEP_TYPES_COUNT)
        return "";

    wdefa = &weapon_defs_a[wtype];
    return wdefa->Name;
}

void init_weapon_text(void)
{
    char locstr[512];
    int weptxt_pos;
    int totlen;
    char *s;
    int i, n;

    totlen = load_file_alltext("textdata/wms.txt", weapon_text);
    if (totlen == Lb_FAIL)
        return;
    if (totlen >= weapon_text_len) {
        LOGERR("Insufficient memory for weapon_text - %d instead of %d", weapon_text_len, totlen);
        totlen = weapon_text_len - 1;
    }

    // TODO change the format to use our INI parser
    s = weapon_text;
    weapon_text[totlen] = '\0';

    // section_start = s;
    weptxt_pos = 0;

    s = strchr(s, '[');
    s++;
    s = strchr(s, ']'); // position at start of WEAPONS section
    s++;

    s += 2;
    while (1)
    {
        if (*s == '[')
            break;

        // Read weapon name
        n = 0;
        while ((*s != '\r') && (*s != '\n'))
        {
            locstr[n] = *s++;
            n++;
        }
        locstr[n] = '\0';
        s += 2;

        // Recognize the weapon name
        for (i = 1; i < WEP_TYPES_COUNT; i++)
        {
            const char *codename;
            codename = weapon_codename(i);
            if (strcmp(codename, locstr) == 0) {
                i--;
                break;
            }
        }
        if (i < WEP_TYPES_COUNT)
        {
            weapon_text_index[i] = weptxt_pos;

            while ((*s != '\r') && (*s != '\n')) {
                weapon_text[weptxt_pos] = *s++;
                weptxt_pos++;
            }
            weapon_text[weptxt_pos] = '\0';
            weptxt_pos++;
            s += 2;

            n = weapon_text_index[i];
            my_preprocess_text(&weapon_text[n]);
        } else {
            LOGERR("Weapon name not recognized: \"%s\"", locstr);
            if (s) s = strpbrk(s, "\r\n");
            if (s) s += 2;
        }
    }

    s = strchr(s, '[');
    s++;
    s = strchr(s, ']'); // position at start of MODS section
    s++;

    s += 2;
    while (1)
    {
        if ((*s == '[') || (*s == '\0'))
            break;

        // Read mod name
        n = 0;
        while ((*s != '\r') && (*s != '\n') && (*s != '\0'))
        {
            locstr[n] = *s++;
            n++;
        }
        locstr[n] = '\0';
        s += 2;

        for (i = 1; i < MOD_TYPES_COUNT; i++)
        {
            const char *codename;
            codename = cybmod_codename(i);
            if (strcmp(codename, locstr) == 0) {
                i--;
                break;
            }
        }
        if (i < MOD_TYPES_COUNT)
        {
            cybmod_text_index[i] = weptxt_pos;

            while ((*s != '\r') && (*s != '\n') && (*s != '\0')) {
                weapon_text[weptxt_pos] = *s++;
                weptxt_pos++;
            }
            weapon_text[weptxt_pos] = '\0';
            weptxt_pos++;
            s += 2;

            n = cybmod_text_index[i];
            my_preprocess_text(&weapon_text[n]);
        } else {
            LOGERR("Cyb Mod name not recognized: \"%s\"", locstr);
            if (s) s = strpbrk(s, "\r\n");
            if (s) s += 2;
        }
    }
}

ushort weapon_sprite_index(ushort wtype, TbBool enabled)
{
    ushort sprid;
    struct WeaponDef *wdef;

    if (wtype >= WEP_TYPES_COUNT)
        return 0;

    wdef = &weapon_defs[wtype];
    // Block of normal weapon sprites is 15..41, lighter are 42..68.
    // Sprites added after that have normal image and ligher image adjacent.
    if (enabled) {
        if (wdef->Sprite < 42)
            sprid = 27 + wdef->Sprite;
        else
            sprid = wdef->Sprite + 1;
    } else {
        sprid = 0 + wdef->Sprite;
    }
    return sprid;
}

TbBool weapon_is_deployed_at_wielder_pos(ushort wtype)
{
    return (wtype == WEP_ELEMINE) || (wtype == WEP_EXPLMINE) ||
      (wtype == WEP_AIRSTRIKE) || (wtype == WEP_CEREBUSIFF) ||
      (wtype == WEP_RAZORWIRE) || (wtype == WEP_EXPLWIRE);
}

TbBool weapon_is_for_throwing(ushort weptype)
{
    return (weptype == WEP_NUCLGREN) || (weptype == WEP_CRAZYGAS) || (weptype == WEP_KOGAS);
}

TbBool weapon_is_breaking_will(ushort weptype)
{
    return (weptype == WEP_PERSUADRTRN) || (weptype == WEP_PERSUADER2);
}

ushort weapon_fourpack_index(ushort wtype)
{
    switch (wtype)
    {
    case WEP_NUCLGREN:
        return WFRPK_NUCLGREN;
    case WEP_ELEMINE:
        return WFRPK_ELEMINE;
    case WEP_EXPLMINE:
        return WFRPK_EXPLMINE;
    case WEP_KOGAS:
        return WFRPK_KOGAS;
    case WEP_CRAZYGAS:
        return WFRPK_CRAZYGAS;
    }
    return WFRPK_COUNT;
}

TbBool weapon_has_targetting(ushort wtype)
{
    return (wtype == WEP_RAP);
}

TbBool weapons_has_weapon(ulong weapons, ushort wtype)
{
    ulong wepflg = 1 << (wtype-1);
    return (weapons & wepflg) != 0;
}

/** Returns weapon set in given flags with index below last.
 */
ushort weapons_prev_weapon(ulong weapons, ushort last_wtype)
{
    ushort wtype;

    if (last_wtype < 2)
        return 0;

    for (wtype = last_wtype - 1; wtype > 0; wtype--)
    {
        ulong wepflg = 1 << (wtype-1);
        if ((weapons & wepflg) != 0)
            return wtype;
    }
    return 0;
}

void weapons_remove_weapon(ulong *p_weapons, struct WeaponsFourPack *p_fourpacks, ushort wtype)
{
    ushort fp;

    *p_weapons &= ~(1 << (wtype-1));

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT)
        p_fourpacks->Amount[fp] = 0;
}

TbBool weapons_remove_one_from_npc(ulong *p_weapons, ushort wtype)
{
    ushort fp;
    TbBool was_last;

    if ((*p_weapons & (1 << (wtype-1))) == 0)
        return false;

    was_last = true;
    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        was_last = false;
    }
    if (was_last)
        *p_weapons &= ~(1 << (wtype-1));
    return true;
}

TbBool weapons_remove_one(ulong *p_weapons, struct WeaponsFourPack *p_fourpacks, ushort wtype)
{
    ushort fp;
    TbBool was_last;

    if ((*p_weapons & (1 << (wtype-1))) == 0)
        return false;

    was_last = true;
    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        was_last = (p_fourpacks->Amount[fp] <= 1);
        p_fourpacks->Amount[fp]--;
    }
    if (was_last)
        *p_weapons &= ~(1 << (wtype-1));
    return true;
}

/** Remove one weapon from player-controlled person in-game.
 * Player struct contains dumb own array rather than uniform WeaponsFourPack, so it requires
 * this special function. To be removed when possible.
 */
TbBool weapons_remove_one_for_player(ulong *p_weapons,
  ubyte p_plfourpacks[][4], ushort plagent, ushort wtype)
{
    ushort fp;
    TbBool was_last;

    if ((*p_weapons & (1 << (wtype-1))) == 0)
        return false;

    was_last = true;
    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        was_last = (p_plfourpacks[fp][plagent] <= 1);
        p_plfourpacks[fp][plagent]--;
    }
    if (was_last)
        *p_weapons &= ~(1 << (wtype-1));
    return true;
}

TbBool weapons_add_one(ulong *p_weapons, struct WeaponsFourPack *p_fourpacks, ushort wtype)
{
    ushort fp;
    TbBool is_first;

    if (number_of_set_bits(*p_weapons) >= WEAPONS_CARRIED_MAX_COUNT)
        return false;

    is_first = ((*p_weapons & (1 << (wtype-1))) == 0);

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        if ((!is_first) && (p_fourpacks->Amount[fp] > 3))
            return false;

        if (is_first)
            p_fourpacks->Amount[fp] = 1;
        else
            p_fourpacks->Amount[fp]++;
    } else {
        if (!is_first)
            return false;
    }

    if (is_first)
        *p_weapons |= (1 << (wtype-1));

    return true;
}

void sanitize_weapon_quantities(ulong *p_weapons, struct WeaponsFourPack *p_fourpacks)
{
    ushort wtype;
    ushort n_weapons;

    n_weapons = 0;
    for (wtype = WEP_TYPES_COUNT-1; wtype > 0; wtype--)
    {
        ushort fp, n;
        TbBool has_weapon;

        has_weapon = weapons_has_weapon(*p_weapons, wtype);

        if (has_weapon && (n_weapons > WEAPONS_CARRIED_MAX_COUNT)) {
            weapons_remove_weapon(p_weapons, p_fourpacks, wtype);
            has_weapon = false;
        }

        fp = weapon_fourpack_index(wtype);
        if (fp >= WFRPK_COUNT)
            continue;

        n = p_fourpacks->Amount[fp];

        if (!has_weapon)
            n = 0;
        else if (n < 1)
            n = 1;
        else if (n > 4)
            n = 4;
        p_fourpacks->Amount[fp] = n;
    }
}

short get_hand_weapon_range(struct Thing *p_person, ushort weptype)
{
    struct WeaponDef *wdef;
    short range;

    if (weptype >= WEP_TYPES_COUNT)
        return 0;

    wdef = &weapon_defs[weptype];

    range = TILE_TO_MAPCOORD(wdef->RangeBlocks, 0);

    if (weapon_is_for_throwing(weptype))
        range = (85 * range * (3 + cybmod_arms_level(&p_person->U.UPerson.UMod)) + range) >> 8;

    if (weapon_is_breaking_will(weptype))
        range = (85 * range * (3 + cybmod_brain_level(&p_person->U.UPerson.UMod)) + range) >> 8;

    return range;
}

short current_hand_weapon_range(struct Thing *p_person)
{
#if 0
    short ret;
    asm volatile ("call ASM_current_hand_weapon_range\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
#endif
    return get_hand_weapon_range(p_person, p_person->U.UPerson.CurrentWeapon);
}

int get_weapon_range(struct Thing *p_person)
{
    int ret;
    asm volatile ("call ASM_get_weapon_range\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

TbBool current_weapon_has_targetting(struct Thing *p_person)
{
    ushort weptype;

    weptype = p_person->U.UPerson.CurrentWeapon;
    if (weptype >= WEP_TYPES_COUNT)
        return false;

    return weapon_has_targetting(weptype);
}

ubyte find_nth_weapon_held(ushort index, ubyte n)
{
    char ret;
    asm volatile ("call ASM_find_nth_weapon_held\n"
        : "=r" (ret) : "a" (index), "d" (n));
    return ret;
}

ulong person_carried_weapons_pesuaded_sell_value(struct Thing *p_person)
{
    ulong credits;
    ushort weptype;

    credits = 0;
    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        struct WeaponDef *wdef;

        if (!person_carries_weapon(p_person, weptype))
            continue;

        wdef = &weapon_defs[weptype];
        credits += wdef->Cost * persuaded_person_weapons_sell_cost_permil / 1000;
    }
    return credits;
}

void do_weapon_quantities_net_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = net_agents__FourPacks[plyr][plagent].Amount[fp];
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities_player_to_net(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = players[plyr].FourPacks[fp][plagent];
        else
            n = 0;
        net_agents__FourPacks[plyr][plagent].Amount[fp] = n;
    }
}

void do_weapon_quantities_cryo_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = cryo_agents.FourPacks[plagent].Amount[fp];
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities_max_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = 4;
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities1(struct Thing *p_person)
{
    if (in_network_game)
    {
        // No action
    }
    else
    {
        do_weapon_quantities_max_to_player(p_person);
    }
}

void do_weapon_quantities_proper1(struct Thing *p_person)
{
    if (in_network_game)
    {
        do_weapon_quantities_player_to_net(p_person);
    }
    else
    {
        do_weapon_quantities_cryo_to_player(p_person);
    }
}

void init_laser(struct Thing *p_person, ushort timer)
{
    asm volatile ("call ASM_init_laser\n"
        : : "a" (p_person), "d" (timer));
}

void init_laser_6shot(struct Thing *p_person, ushort timer)
{
    struct Thing *p_target;
    struct Thing *p_thing;
    ThingIdx thing;
    ushort group, n_targets;

    p_target = p_person->PTarget;
    if (p_target == NULL)
        return;

    group = p_target->U.UPerson.EffectiveGroup & 0x1F;
    init_laser(p_person, timer);

    n_targets = 0;
    thing = same_type_head[256 + group];
    for (; thing != 0; thing = p_thing->LinkSameGroup)
    {
        if (n_targets >= 5)
            break;
        p_thing = &things[thing];
        if ((p_thing->Flag & TngF_Destroyed) == 0)
        {
            int dist_x, dist_z;
            dist_x = abs(PRCCOORD_TO_MAPCOORD(p_thing->X - p_person->X));
            dist_z = abs(PRCCOORD_TO_MAPCOORD(p_thing->Z - p_person->Z));
            // Simplification to avoid multiplication and square root to get proper distance
            if (dist_x <= dist_z)
                dist_x >>= 1;
            else
                dist_z >>= 1;
            if (dist_x + dist_z + 128 < TILE_TO_MAPCOORD(weapon_defs[WEP_LASER].RangeBlocks,0))
            {
                n_targets++;
                p_person->PTarget = p_thing;
                init_laser(p_person, timer);
            }
        }
    }
    p_person->PTarget = p_target;
}

short init_taser(struct Thing *p_person)
{
    short ret;
    asm volatile ("call ASM_init_taser\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

void init_rocket(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_rocket\n"
        : : "a" (p_owner));
}

void init_razor_wire(struct Thing *p_person, ubyte flag)
{
    asm volatile ("call ASM_init_razor_wire\n"
        : : "a" (p_person), "d" (flag));
}

void finalise_razor_wire(struct Thing *p_person)
{
    asm volatile ("call ASM_finalise_razor_wire\n"
        : : "a" (p_person));
}

void update_razor_wire(struct Thing *p_person)
{
    asm volatile ("call ASM_update_razor_wire\n"
        : : "a" (p_person));
}

void init_laser_beam(struct Thing *p_owner, ushort size, ubyte type)
{
    asm volatile ("call ASM_init_laser_beam\n"
        : : "a" (p_owner), "d" (size), "b" (type));
}

void init_laser_guided(struct Thing *p_owner, ushort size)
{
    asm volatile ("call ASM_init_laser_guided\n"
        : : "a" (p_owner), "d" (size));
}

void init_laser_elec(struct Thing *p_owner, ushort size)
{
    asm volatile ("call ASM_init_laser_elec\n"
        : : "a" (p_owner), "d" (size));
}

void init_laser_q_sep(struct Thing *p_owner, ushort size)
{
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_beam(p_owner, size, 0x14u);
}

void init_fire_weapon(struct Thing *p_person)
{
    asm volatile ("call ASM_init_fire_weapon\n"
        : : "a" (p_person));
}

void init_clone_disguise(struct Thing *p_person)
{
    if ((p_person->Flag2 & TgF2_Unkn00400000) != 0)
        return;

    p_person->U.UPerson.AnimMode = 0;
    p_person->U.UPerson.OldSubType = p_person->SubType;
    switch (LbRandomAnyShort() & 3)
    {
    case 0:
        p_person->SubType = SubTT_PERS_BRIEFCASE_M;
        break;
    case 1:
        p_person->SubType = SubTT_PERS_WHITE_BRUN_F;
        break;
    case 2:
        p_person->SubType = SubTT_PERS_WHIT_BLOND_F;
        break;
    case 3:
        p_person->SubType = SubTT_PERS_LETH_JACKT_M;
        break;
    default:
        break;
    }
    reset_person_frame(p_person);
    p_person->Flag2 |= TgF2_Unkn00400000;
}

int gun_out_anim(struct Thing *p_person, ubyte shoot_flag)
{
    int ret;
    asm volatile ("call ASM_gun_out_anim\n"
        : "=r" (ret) : "a" (p_person), "d" (shoot_flag));
    return ret;
}

ushort player_weapon_time(struct Thing *p_person)
{
    ushort ret;
    asm volatile ("call ASM_player_weapon_time\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

void process_vehicle_weapon(struct Thing *p_vehicle, struct Thing *p_person)
{
    asm volatile ("call ASM_process_vehicle_weapon\n"
        : : "a" (p_vehicle), "d" (p_person));
}

void process_mech_weapon(struct Thing *p_vehicle, struct Thing *p_person)
{
    asm volatile ("call ASM_process_mech_weapon\n"
        : : "a" (p_vehicle), "d" (p_person));
}

ushort persuade_power_required(ThingIdx victim)
{
    struct Thing *p_victim;
    ushort ptype;
    ushort persd_pwr_rq;
    short brain_lv;

    if (victim <= 0)
        return 9999;

    p_victim = &things[victim];
    if ((p_victim->Flag2 & TgF2_Unkn00400000) != 0)
        ptype = p_victim->U.UPerson.OldSubType;
    else
        ptype = p_victim->SubType;

    persd_pwr_rq = peep_type_stats[ptype].PersuadeReqd;
    brain_lv = cybmod_brain_level(&p_victim->U.UPerson.UMod);
    if (brain_lv == 5)
        return 9999;
    if (brain_lv == 4)
        persd_pwr_rq = peep_type_stats[SubTT_PERS_AGENT].PersuadeReqd;

    return persd_pwr_rq;
}

TbBool person_can_be_persuaded_now(ThingIdx attacker, ThingIdx target,
  short weapon_range, ubyte target_select, ushort *energy_reqd)
{
    struct Thing *p_target;
    struct Thing *p_attacker;
    short target_persd_pwr_rq;

    if ((target <= 0) || (attacker <= 0))
        return false;

    p_attacker = &things[attacker];

    p_target = &things[target];
    if (p_target->Type != TT_PERSON)
        return false;

    {
        int dist_y;
        short cntr_cor_y;

        cntr_cor_y = PRCCOORD_TO_YCOORD(p_attacker->Y);
        dist_y = abs(PRCCOORD_TO_YCOORD(p_target->Y) - cntr_cor_y);
        if (dist_y >= 1240)
            return false;
    }

    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      ((p_target->Flag & (TngF_Unkn40000000|TngF_Persuaded|TngF_Destroyed)) != 0))
        return false;

    // If already harvested the soul
    if ((target_select == PTargSelect_SoulCollect) && ((p_target->Flag2 & TgF2_SoulDepleted) != 0))
        return false;

    // Cannot persuade people from own group
    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      (p_target->U.UPerson.EffectiveGroup == p_attacker->U.UPerson.EffectiveGroup))
        return false;

    // Holding a taser prevents both persuasion and soul harvest
    if (p_target->U.UPerson.CurrentWeapon == WEP_H2HTASER)
        return false;

    // Self-affecting not allowed for both persuasion and soul harvest
    if (target == attacker)
        return false;

    // Some people can only be affected by advanced persuader
    if ((target_select == PTargSelect_Persuader) &&
      person_only_affected_by_adv_persuader(target))
        return false;

    target_persd_pwr_rq = persuade_power_required(target);

    // Check if we have enough persuade power to overwhelm the target
    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      (target_persd_pwr_rq > p_attacker->U.UPerson.PersuadePower))
        return false;

    // Only player agents require energy to persuade
    *energy_reqd = 0;
    if ((p_attacker->Flag & TngF_PlayerAgent) != 0)
        *energy_reqd = 30 * (target_persd_pwr_rq + 1);
    if (*energy_reqd > 600)
        *energy_reqd = 600;

    // Check if we have enough weapon energy
    if (*energy_reqd > p_attacker->U.UPerson.Energy)
        return false;

    // If under commands to persuade a specific person, accept only that person ignoring anyone else
    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      (p_attacker->State == PerSt_PERSUADE_PERSON) && (target != p_attacker->GotoThingIndex))
        return false;

    return true;
}

short process_persuadertron(struct Thing *p_person, ubyte target_select, ushort *energy_reqd)
{
#if 0
    short ret;
    asm volatile ("call ASM_process_persuadertron\n"
        : "=r" (ret) : "a" (p_person), "d" (target_select), "b" (energy_reqd));
    return ret;
#endif
    short cor_x, cor_z;
    short weapon_range;

    cor_x = PRCCOORD_TO_MAPCOORD(p_person->X);
    cor_z = PRCCOORD_TO_MAPCOORD(p_person->Z);

    switch (target_select)
    {
    case PTargSelect_Persuader:
    default:
        weapon_range = get_hand_weapon_range(p_person, WEP_PERSUADRTRN);
        break;
    case PTargSelect_PersuadeAdv:
        weapon_range = get_hand_weapon_range(p_person, WEP_PERSUADER2);
        break;
    case PTargSelect_SoulCollect:
        weapon_range = get_hand_weapon_range(p_person, WEP_SOULGUN);
        break;
    }

    return find_person_which_can_be_persuaded_now(cor_x, cor_z, weapon_range,
      p_person->ThingOffset, target_select, energy_reqd);
}

void get_soul(struct Thing *p_dead, struct Thing *p_person)
{
    asm volatile ("call ASM_get_soul\n"
        : : "a" (p_dead), "d" (p_person));
}

void choose_best_weapon_for_range(struct Thing *p_person, int dist)
{
    asm volatile ("call ASM_choose_best_weapon_for_range\n"
        : : "a" (p_person), "d" (dist));
}

void process_weapon_recoil(struct Thing *p_person)
{
    if (((p_person->Flag2 & TgF2_Unkn0800) == 0) &&
      ((p_person->Flag & (TngF_Unkn20000000|TngF_PlayerAgent)) == (TngF_Unkn20000000|TngF_PlayerAgent)))
    {
        PlayerInfo *p_player;
        ushort plyr, plagent;

        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
        p_person->U.UPerson.ComTimer = -1;
        p_person->VX = p_player->field_19A[plagent];
        p_person->VY = p_player->field_E8[plagent];
        p_person->VZ = p_player->field_1A2[plagent];
    }
    else if ((p_person->Flag & (TngF_Unkn20000000|TngF_Persuaded)) == (TngF_Unkn20000000|TngF_Persuaded))
    {
        PlayerInfo *p_player;
        struct Thing *p_owner;
        ushort plyr;
        int vel_x, vel_z, vel_y;
        int dt_x, dt_z, dt_y, range;
        int ln_x, ln_z, dist;

        p_owner = &things[p_person->Owner];
        plyr = (p_owner->U.UPerson.ComCur & 0x1C) >> 2;
        p_player = &players[plyr];
        range = get_weapon_range(p_person);
        vel_x = p_player->SpecialItems[0];
        vel_y = p_player->SpecialItems[1];
        vel_z = p_player->SpecialItems[2];

        dt_x = vel_x - (p_person->X >> 8);
        dt_y = vel_y - (p_person->Y >> 8);
        dt_z = vel_z - (p_person->Z >> 8);
        ln_z = abs(dt_z);
        ln_x = abs(dt_x);
        if (ln_x >= ln_z)
            dist = ln_x + (ln_z >> 2) + (ln_z >> 3) + (ln_z >> 6) + (ln_z >> 7) - (ln_x >> 5) - (ln_x >> 7);
        else
            dist = ln_z + (ln_x >> 2) + (ln_x >> 3) + (ln_x >> 6) + (ln_x >> 7) - (ln_z >> 5) - (ln_z >> 7);
        if (dist > range)
        {
            if (dist == 0)
                dist = 1;
            vel_x = (p_person->X >> 8) + range * dt_x / dist;
            vel_y = (p_person->Y >> 8) + range * dt_y / dist;
            vel_z = (p_person->Z >> 8) + range * dt_z / dist;
        }
        p_person->U.UPerson.ComTimer = -1;
        p_person->VX = vel_x;
        p_person->VY = vel_y;
        p_person->VZ = vel_z;
    }
}

static void process_energy_recovery(struct Thing *p_person)
{
    if (p_person->U.UPerson.Energy < p_person->U.UPerson.MaxEnergy)
    {
        if ((p_person->Flag2 & TgF2_ExistsOffMap) != 0)
        {
            p_person->U.UPerson.Energy = p_person->U.UPerson.MaxEnergy;
        }
        else if (p_person->U.UPerson.WeaponTurn == 0)
        {
            p_person->U.UPerson.Energy += 2;
        }
    }
}

static void process_health_recovery(struct Thing *p_person)
{
    ulong mask;

    if ((p_person->Flag & TngF_PlayerAgent) != 0)
        mask = 1;
    else
        mask = 7;
    // Chest level 4 means invincible
    if (cybmod_chest_level(&p_person->U.UPerson.UMod) == 4)
    {
        person_set_helath_to_max_limit(p_person);
    }
    if (((mask & gameturn) == 0) && p_person->Health < p_person->U.UPerson.MaxHealth)
    {
        int i;

        i = 4 * (cybmod_chest_level(&p_person->U.UPerson.UMod) + 1);
        if (abs(p_person->U.UPerson.Mood) > 32)
            i >>= 2;
        else if (abs(p_person->U.UPerson.Mood) > 16)
            i >>= 1;
        if (abs(p_person->U.UPerson.Mood) > 64 && (gameturn & 2) != 0)
            i = 0;
        p_person->Health += i;
    }
}

void set_person_weapon_turn(struct Thing *p_person, short n_turn)
{
    if ((p_person->Flag & TngF_PlayerAgent) == 0)
    {
        p_person->U.UPerson.WeaponTurn = n_turn;
    }
    else if ((p_person->Flag2 & TgF2_Unkn0800) == 0)
    {
        PlayerInfo *p_player;
        ushort plyr, plagent;
        ushort weptype;

        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
        weptype = p_person->U.UPerson.CurrentWeapon;
        p_player->WepDelays[plagent][weptype] = n_turn;
        p_person->U.UPerson.WeaponTurn = n_turn;
    }
}

void process_clone_disguise(struct Thing *p_person)
{
    struct WeaponDef *wdef;
    short en_used;

    if ((p_person->Flag2 & TgF2_Unkn00400000) == 0)
        return;

    wdef = &weapon_defs[WEP_CLONESHLD];
    if (in_network_game)
        en_used = wdef->EnergyUsed >> 1;
    else
        en_used = wdef->EnergyUsed;
    p_person->U.UPerson.Energy -= en_used;

    if (((p_person->Flag & TngF_PlayerAgent) != 0) && (p_person->U.UPerson.Energy < 0))
        p_person->U.UPerson.CurrentWeapon = WEP_NULL;

    if (p_person->U.UPerson.CurrentWeapon != WEP_CLONESHLD)
    {
        p_person->Flag2 &= ~TgF2_Unkn00400000;
        p_person->SubType = p_person->U.UPerson.OldSubType;
        reset_person_frame(p_person);
    }
}

TbBool person_weapons_remove_one(struct Thing *p_person, ushort weptype)
{
    PlayerInfo *p_player;
    ushort plagent;
    TbBool done;

    p_player = NULL;
    plagent = 0;
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
    {
        PlayerIdx plyr;
        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
    }

    if (p_person->U.UPerson.CurrentWeapon == weptype)
        p_person->U.UPerson.CurrentWeapon = WEP_NULL;

    if (p_player != NULL)
        //TODO replace  with weapons_remove_one() call, when FourPacks have unified format
        done = weapons_remove_one_for_player(&p_person->U.UPerson.WeaponsCarried, p_player->FourPacks, plagent, weptype);
    else
        done = weapons_remove_one_from_npc(&p_person->U.UPerson.WeaponsCarried, weptype);
    return done;
}

void process_automedkit(struct Thing *p_person)
{
    if (!weapons_has_weapon(p_person->U.UPerson.WeaponsCarried, WEP_MEDI2))
        return;
    if (p_person->Health >= p_person->U.UPerson.MaxHealth / 8)
        return;

    p_person->Health = p_person->U.UPerson.MaxHealth;
    person_weapons_remove_one(p_person, WEP_MEDI2);
    play_dist_sample(p_person, 2, 127, 64, 100, 0, 1);
}

void low_energy_alarm_stop(void)
{
    if (IsSamplePlaying(0, 93, 0))
        stop_sample_using_heap(0, 93);
}

void low_energy_alarm_play(void)
{
    if (!IsSamplePlaying(0, 93, 0))
        play_sample_using_heap(0, 93, 127, 64, 100, -1, 3);
}

void process_energy_alarm(struct Thing *p_person)
{
    ThingIdx dcthing;

    if ((p_person->Flag & TngF_Unkn1000) == 0)
        return;

    dcthing = players[local_player_no].DirectControl[0];
    if (p_person->ThingOffset == dcthing)
    {
        if (p_person->U.UPerson.Energy >= low_energy_alarm_level)
        {
            if ((gameturn & 7) == 0)
                low_energy_alarm_stop();
        }
        else
        {
            low_energy_alarm_play();
        }
    }
}

void process_move_while_firing(struct Thing *p_person)
{
    struct Thing *p_vehicle;
    ubyte currWeapon;

    if ((p_person->Flag & TngF_Unkn0800) == 0)
        return;

    currWeapon = p_person->U.UPerson.CurrentWeapon;
    if (currWeapon != 0 && currWeapon != WEP_RAZORWIRE
        && currWeapon != WEP_EXPLWIRE && (p_person->Flag2 & TgF2_Unkn00080000) != 0)
    {
        p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 0);
        reset_person_frame(p_person);
        p_person->Timer1 = 48;
        p_person->StartTimer1 = 48;
        p_person->Flag2 &= ~TgF2_Unkn00080000;
        p_person->Speed = calc_person_speed(p_person);
    }
    p_vehicle = &things[p_person->U.UPerson.Vehicle];
    if (((p_person->Flag & TngF_InVehicle) != 0) && (p_vehicle->State == 69))
    {
        p_person->Flag &= ~TngF_Unkn0800;
    }
    else
    {
        if ((p_person->Flag & TngF_Unkn20000000) == 0)
            check_persons_target(p_person);
        if (p_person->U.UPerson.Target2 != 0)
        {
            check_persons_target2(p_person);
        }
        else if ((p_person->Type != TT_MINE) && (p_person->Flag & TngF_PlayerAgent) == 0)
        {
            struct Thing *p_target;

            p_target = p_person->PTarget;
            if (p_target != NULL)
            {
                int dt_x, dt_z;
                ushort i;

                dt_x = (p_target->X - p_person->X) >> 8;
                dt_z = (p_target->Z - p_person->Z) >> 8;
                i = (arctan(dt_x, -dt_z) + 128) & 0x7FF;
                change_player_angle(p_person, i >> 8);
            }
        }
    }
}

void process_weapon_wind_down(struct Thing *p_person)
{
    ushort plagent;

    if ((p_person->Flag2 & TgF2_Unkn0200) == 0)
        return;

    switch (p_person->U.UPerson.CurrentWeapon)
    {
    case WEP_UZI:
        if ((p_person->Flag & TngF_PlayerAgent) != 0)
            plagent = p_person->U.UPerson.ComCur & 3;
        else
            plagent = 0;
        ReleaseLoopedSample(p_person->ThingOffset, 80 + plagent);
        p_person->Flag2 &= ~TgF2_Unkn0200;
        break;
    case WEP_MINIGUN:
        if ((p_person->Flag & TngF_PlayerAgent) != 0)
            plagent = p_person->U.UPerson.ComCur & 3;
        else
            plagent = 0;
        ReleaseLoopedSample(p_person->ThingOffset, 84 + plagent);
        plagent = p_person->U.UPerson.ComCur & 3;
        play_dist_sample(p_person, 88 + plagent, 0x7F, 0x40, 100, 0, 1);
        p_person->Flag2 &= ~TgF2_Unkn0200;
        break;
    }
}

void process_weapon_continuous_fire(struct Thing *p_person)
{
    struct WeaponDef *wdef;
    short prevWepTurn;

    wdef = &weapon_defs[p_person->U.UPerson.CurrentWeapon];
    prevWepTurn = p_person->U.UPerson.WeaponTurn;
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
        p_person->U.UPerson.WeaponTurn = player_weapon_time(p_person);
    if (prevWepTurn)
    {
        if ((p_person->Flag & TngF_PlayerAgent) == 0)
            p_person->U.UPerson.WeaponTurn--;
        if (p_person->U.UPerson.WeaponTurn == 0)
        {
            int animMode;

            animMode = p_person->U.UPerson.AnimMode;
            if ((animMode == 14 || animMode == 15) && p_person->Type != TT_MINE)
            {
                p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 0);
                reset_person_frame(p_person);
            }
            switch (p_person->U.UPerson.CurrentWeapon)
            {
            case WEP_UZI:
                if ((p_person->U.UPerson.Energy <= wdef->EnergyUsed) ||
                  ((p_person->Flag & TngF_Unkn0800) == 0))
                {
                    ushort plagent;

                    if ((p_person->Flag & TngF_PlayerAgent) != 0)
                        plagent = p_person->U.UPerson.ComCur & 3;
                    else
                        plagent = 0;
                    ReleaseLoopedSample(p_person->ThingOffset, 80 + plagent);
                    p_person->Flag2 &= ~TgF2_Unkn0200;
                }
                break;
            case WEP_MINIGUN:
                if ((p_person->U.UPerson.Energy <= wdef->EnergyUsed) ||
                  ((p_person->Flag & TngF_Unkn0800) == 0))
                {
                    ushort plagent;

                    if ((p_person->Flag & TngF_PlayerAgent) != 0)
                        plagent = p_person->U.UPerson.ComCur & 3;
                    else
                        plagent = 0;
                    ReleaseLoopedSample(p_person->ThingOffset, 84 + plagent);
                    plagent = p_person->U.UPerson.ComCur & 3;
                    play_dist_sample(p_person, 88 + plagent, 0x7F, 0x40, 100, 0, 1);
                    p_person->Flag2 &= ~TgF2_Unkn0200;
                }
                break;
            case WEP_FLAMER:
                if ((p_person->U.UPerson.Energy <= wdef->EnergyUsed) ||
                  ((p_person->Flag & TngF_Unkn0800) == 0))
                {
                    stop_sample_using_heap(p_person->ThingOffset, 14);
                    play_dist_sample(p_person, 15, 0x7F, 0x40, 100, 0, 2);
                    p_person->Flag2 &= ~TgF2_Unkn0200;
                }
                break;
            }
        }
    }
}

void process_wielded_weapon(struct Thing *p_person)
{
    ThingIdx targtng;
    ushort energy_rq;
    struct WeaponDef *wdef;
    short wepTurn;

    switch (p_person->U.UPerson.CurrentWeapon)
    {
    case WEP_SOULGUN:
        if ((p_person->Health < 2 * p_person->U.UPerson.MaxHealth) &&
          (((gameturn + p_person->ThingOffset) & 7) == 0))
        {
            targtng = process_persuadertron(p_person, PTargSelect_SoulCollect, &energy_rq);
            if (targtng > 0)
                get_soul(&things[targtng], p_person);
        }
        return;
    case WEP_PERSUADRTRN:
        {
            targtng = process_persuadertron(p_person, PTargSelect_Persuader, &energy_rq);
            if (targtng > 0)
                set_person_persuaded(&things[targtng], p_person, energy_rq);
        }
        return;
    case WEP_PERSUADER2:
        {
            targtng = process_persuadertron(p_person, PTargSelect_PersuadeAdv, &energy_rq);
            if (targtng > 0)
                set_person_persuaded(&things[targtng], p_person, energy_rq);
        }
        return;
    case WEP_H2HTASER:
        init_taser(p_person);
        break;
    case WEP_CLONESHLD:
        init_clone_disguise(p_person);
        break;
    default:
        break;
    }

    process_clone_disguise(p_person);

    wdef = &weapon_defs[p_person->U.UPerson.CurrentWeapon];
    wepTurn = p_person->U.UPerson.WeaponTurn;
    if ((wepTurn == 0) || (wepTurn < wdef->ReFireDelay - 6))
        p_person->U.UPerson.FrameId.Version[4] = 0;
    switch (p_person->U.UPerson.CurrentWeapon)
    {
    case WEP_EXPLWIRE:
        if ((p_person->Flag2 & TgF2_Unkn0001) == 0)
        {
            if ((p_person->Flag & TngF_Unkn0800) != 0)
                init_razor_wire(p_person, 1u);
        }
        else if (((p_person->Flag & TngF_Unkn0800) != 0)
            || ((p_person->Flag2 & TgF2_Unkn0004) != 0))
        {
            update_razor_wire(p_person);
        }
        else
        {
            finalise_razor_wire(p_person);
        }
        break;
    case WEP_RAZORWIRE:
        if ((p_person->Flag2 & TgF2_Unkn0001) == 0)
        {
            if ((p_person->Flag & TngF_Unkn0800) != 0)
                init_razor_wire(p_person, 0);
        }
        else if (((p_person->Flag & TngF_Unkn0800) != 0)
            || ((p_person->Flag2 & TgF2_Unkn0004) != 0))
        {
            update_razor_wire(p_person);
        }
        else
        {
            finalise_razor_wire(p_person);
        }
        break;
    case WEP_FLAMER:
        if (p_person->U.UPerson.WeaponTurn == 1)
        {
            if ((p_person->Flag & TngF_PlayerAgent) == 0)
                p_person->Flag &= ~TngF_Unkn0800;
        }
        if (p_person->U.UPerson.WeaponTurn <= 1)
        {
            if ((p_person->Flag & TngF_Unkn0800) != 0)
            {
                if ((p_person->U.UPerson.WeaponTurn == 0)
                    && ((p_person->Flag2 & TgF2_Unkn0400) == 0))
                {
                    play_dist_sample(p_person, 14, 0x7F, 0x40, 100, -1, 2);
                    play_dist_sample(p_person, 13, 0x7F, 0x40, 100, 0, 2);
                    p_person->Flag2 |= TgF2_Unkn0200;
                }
                init_fire_weapon(p_person);
                p_person->U.UPerson.WeaponTurn = 5;
            }
            else
            {
                p_person->U.UPerson.WeaponDir = 32
                    * p_person->U.UPerson.Angle;
            }
        }
        else
        {
            init_fire_weapon(p_person);
        }
        break;
    default:
        if (((p_person->Flag & TngF_Unkn0800) != 0) && (p_person->U.UPerson.CurrentWeapon & 0xFF)
            && (p_person->U.UPerson.WeaponTurn == 0) && ((p_person->Flag & TngF_Unkn0400) == 0))
        {
            process_weapon_recoil(p_person);
            init_fire_weapon(p_person);
        }
        break;
    }

    if ((p_person->Flag & TngF_Unkn0400) != 0)
    {
        if (p_person->U.UPerson.CurrentWeapon != 5)
        {
            if (p_person->U.UPerson.WeaponTimer > 15)
                p_person->U.UPerson.WeaponTimer = 15;
            else
                p_person->U.UPerson.Energy -= 8;
        }

        switch (p_person->U.UPerson.CurrentWeapon)
        {
        case WEP_LASER:
            p_person->U.UPerson.FrameId.Version[4] = 2;
            break;
        case WEP_RAP:
            p_person->U.UPerson.FrameId.Version[4] = 0;
            break;
        default:
            p_person->U.UPerson.FrameId.Version[4] = 4;
            break;
        }

        if ((p_person->Flag & TngF_Unkn0800) != 0)
        {
            if (p_person->U.UPerson.WeaponTimer == 14)
            {
                stop_sample_using_heap(p_person->ThingOffset, 7);
                play_dist_sample(p_person, 52, 0x7F, 0x40, 100, -1, 3);
            }
            else
            {
                SetSamplePitch(p_person->ThingOffset, 7,
                    2 * p_person->U.UPerson.WeaponTimer + 100);
            }
            p_person->U.UPerson.WeaponTimer++;
            if (p_person->U.UPerson.CurrentWeapon == WEP_RAP)
                p_person->U.UPerson.WeaponTimer++;

            if ((p_person->Flag & (TngF_Unkn20000000|TngF_PlayerAgent)) != (TngF_Unkn20000000|TngF_PlayerAgent))
            {
                int resp_time;

                if ((p_person->Health < (p_person->U.UPerson.MaxHealth >> 1)
                    || ((p_person->Flag & TngF_InVehicle) != 0)
                    || (p_person->ThingOffset & 3) != 0)
                    && ((p_person->Flag & TngF_PlayerAgent) == 0))
                {
                    resp_time = 5;
                }
                else
                {
                    resp_time = 4 * (cybmod_brain_level(&p_person->U.UPerson.UMod) + 1);
                    if (resp_time > 15)
                        resp_time = 15;
                }
                if (p_person->U.UPerson.WeaponTimer >= resp_time)
                {
                    if ((p_person->Flag & TngF_Unkn1000) == 0)
                        p_person->Flag &= ~TngF_Unkn0800;
                }
            }
            if ((p_person->U.UPerson.CurrentWeapon == WEP_RAP) && (p_person->PTarget == NULL))
                p_person->U.UPerson.WeaponTimer = 0;
        }

        if ((p_person->Flag & TngF_Unkn0800) == 0)
        {
            int i;
            short reFireShift;

            reFireShift = 0;
            if (p_person->U.UPerson.WeaponTimer >= 5)
            {
                i = p_person->U.UPerson.WeaponTimer - 3;
                set_person_weapon_turn(p_person, i);
            }
            else
            {
                i = p_person->U.UPerson.WeaponTimer;
                set_person_weapon_turn(p_person, i);
                p_person->U.UPerson.WeaponTimer = 5;
                reFireShift = 5 - i;
            }
            process_weapon_recoil(p_person);

            wdef = &weapon_defs[p_person->U.UPerson.CurrentWeapon];
            switch (p_person->U.UPerson.CurrentWeapon)
            {
            case WEP_LASER:
                if (p_person->SubType == SubTT_PERS_MECH_SPIDER)
                {
                    init_laser_6shot(p_person, p_person->U.UPerson.WeaponTimer);
                    p_person->U.UPerson.WeaponTurn = 1;
                }
                else
                {
                    init_laser(p_person, p_person->U.UPerson.WeaponTimer);
                    p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
                }
                stop_sample_using_heap(p_person->ThingOffset, 7);
                stop_sample_using_heap(p_person->ThingOffset, 52);
                play_dist_sample(p_person, 18, 0x7F, 0x40, 100, 0, 3);
                break;
            case WEP_ELLASER:
                init_laser_elec(p_person, p_person->U.UPerson.WeaponTimer);
                stop_sample_using_heap(p_person->ThingOffset, 7);
                stop_sample_using_heap(p_person->ThingOffset, 52);
                if ((p_person->Flag2 & TgF2_ExistsOffMap) == 0)
                    play_dist_sample(p_person, 6, 0x7F, 0x40, 100, 0, 3);
                p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
                break;
            case WEP_RAP:
                p_person->U.UPerson.Energy -= wdef->EnergyUsed;
                init_rocket(p_person);
                stop_sample_using_heap(p_person->ThingOffset, 7);
                stop_sample_using_heap(p_person->ThingOffset, 52);
                p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
                break;
            case WEP_BEAM:
                 init_laser_beam(p_person, p_person->U.UPerson.WeaponTimer, 17);
                 stop_sample_using_heap(p_person->ThingOffset, 7);
                 stop_sample_using_heap(p_person->ThingOffset, 52);
                 play_dist_sample(p_person, 5, 0x7F, 0x40, 100, 0, 3);
                 p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
                break;
            case WEP_QDEVASTATOR:
                init_laser_q_sep(p_person, p_person->U.UPerson.WeaponTimer);
                stop_sample_using_heap(p_person->ThingOffset, 7);
                stop_sample_using_heap(p_person->ThingOffset, 52);
                play_dist_sample(p_person, 28, 0x7F, 0x40, 100, 0, 3);
                p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
                break;
            default:
                break;
            }
            // We probably updated WeaponTurn, now do the update properly
            set_person_weapon_turn(p_person, p_person->U.UPerson.WeaponTurn);

            p_person->Flag &= ~TngF_Unkn0400;
            if ((p_person->U.UPerson.WeaponTimer > 5)
              && (p_person->U.UPerson.CurrentWeapon != WEP_RAP))
                p_person->Flag |= TngF_StationrSht;
        }
    }
}

void process_weapon(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_process_weapon\n"
        : : "a" (p_person));
    return;
#endif
    process_energy_alarm(p_person);

    p_person->U.UPerson.Flag3 &= ~PrsF3_Unkn40;

    process_move_while_firing(p_person);

    if (((p_person->Flag & TngF_Persuaded) != 0) || (p_person->State == PerSt_PROTECT_PERSON))
    {
        struct Thing *p_owner;
        struct Thing *p_target;

        p_owner = &things[p_person->Owner];
        p_target = p_owner->PTarget;
        if (p_target != NULL)
        {
            if ((p_target->State == PerSt_DEAD) && (p_owner->Flag & (TngF_Unkn0400|TngF_Unkn0800)) != 0)
            {
                p_person->Flag |= TngF_Unkn0800;
                p_person->PTarget = p_target;
                p_person->Flag &= ~TngF_Unkn20000000;
            }
        }
    }
    process_automedkit(p_person);

    if (p_person->U.UPerson.WeaponTurn == 0)
        process_weapon_wind_down(p_person);

    if (p_person->U.UPerson.MaxStamina != 0)
        process_stamina(p_person);
    process_energy_recovery(p_person);
    process_health_recovery(p_person);

    process_weapon_continuous_fire(p_person);

    if ((p_person->Flag & TngF_Unkn40000000) == 0)
    {
        if ((p_person->Flag & TngF_InVehicle) != 0)
        {
            struct Thing *p_vehicle;

            p_vehicle = &things[p_person->U.UPerson.Vehicle];
            if ((p_vehicle->Flag2 & TgF2_Unkn0002) != 0)
                return;
            switch (p_vehicle->SubType)
            {
            case SubTT_VEH_TANK:
                process_vehicle_weapon(p_vehicle, p_person);
                return;
            case SubTT_VEH_MECH:
                process_mech_weapon(p_vehicle, p_person);
                return;
            default:
                break;
            }
        }
        process_wielded_weapon(p_person);
    }
}

s32 laser_hit_at(s32 x1, s32 y1, s32 z1, s32 *x2, s32 *y2, s32 *z2, struct Thing *p_shot)
{
    s32 ret;
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "call ASM_laser_hit_at\n"
        : "=r" (ret) : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (p_shot));
    return ret;
}

/******************************************************************************/
