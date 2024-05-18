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
#include "bigmap.h"
#include "network.h"
#include "thing.h"
#include "player.h"
#include "game.h"
#include "wadfile.h"
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

void do_weapon_quantities_net_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort wtype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = (p_person->U.UPerson.ComCur & 3);

    for (wtype = WEP_TYPES_COUNT-1; wtype > 0; wtype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(wtype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, wtype))
            n = net_agents__FourPacks[plyr][plagent].Amount[fp];
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities_player_to_net(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort wtype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (wtype = WEP_TYPES_COUNT-1; wtype > 0; wtype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(wtype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, wtype))
            n = players[plyr].FourPacks[fp][plagent];
        else
            n = 0;
        net_agents__FourPacks[plyr][plagent].Amount[fp] = n;
    }
}

void do_weapon_quantities_cryo_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort wtype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (wtype = WEP_TYPES_COUNT-1; wtype > 0; wtype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(wtype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, wtype))
            n = cryo_agents.FourPacks[plagent].Amount[fp];
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities_max_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort wtype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (wtype = WEP_TYPES_COUNT-1; wtype > 0; wtype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(wtype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, wtype))
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
    for (thing = same_type_head[256 + group]; thing != 0; thing = p_thing->LinkSameGroup)
    {
        if (n_targets >= 5)
            break;
        p_thing = &things[thing];
        if ((p_thing->Flag & TngF_Unkn0002) == 0)
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

/******************************************************************************/
