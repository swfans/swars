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
#include "bffile.h"
#include "bfini.h"
#include "thing.h"
#include "player.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
struct WeaponDef weapon_defs[] = {
    { 0,    0,  0,  0,   0,  0, 0, 0, 0,    0,     0,     0,  0},
    { 5,   50,  4,  5,   8, 10, 1, 1, 0,  272,    40,    40, 10},
    { 8,  100,  3,  8,  20, 10, 1, 1, 0,  273,   120,   120, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, 3, 1811,   370,   370, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, 3, 1812,   420,   420, 10},
    {12,  800, 30,  8, 300, 10, 1, 1, 0,  280,   750,   750, 10},
    { 6,  800, 40,  8, 600, 10, 1, 1, 0,  789,  1000,  1000, 10},
    { 1,    0, 10,  8,   2, 10, 1, 1, 0, 2319,   150,   150, 10},
    { 4,   80,  1, 16,   6,  5, 0, 0, 0,  535,   160,   160, 10},
    { 0, 9999,  3, 16,   4,  5, 0, 0, 0, 1821,   200,   200, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, 0,  545,   100,   100, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, 0,  542,    80,    80, 10},
    { 4,  300,  5, 16,   4,  5, 0, 0, 0, 1817,   620,   620, 10},
    { 4, 9999, 40, 16, 200,  5, 0, 0, 0,  283,   480,   480, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, 0,  537,   400,   400, 10},
    {16, 1300, 80,  8, 200, 10, 1, 1, 0,  284,   300,   300, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, 0,  803,  1050,  1050, 10},
    {10,  550,  2, 16,   4,  5, 0, 0, 0, 2586,  2000,  2000, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, 0,  274,   100,   100, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, 0, 1560,   850,   850, 10},
    {11, 1500, 10, 16,   4,  5, 0, 0, 0, 1574, 65535, 65535, 10},
    { 2,    0, 10,  8,   2, 10, 1, 1, 0, 2345,   450,   450, 10},
    { 8, 9999, 20, 16, 200,  5, 0, 0, 0, 1575,  1100,  1100, 10},
    { 0, 9999,  1, 16,   4,  5, 0, 0, 0, 2574,    80,    80, 10},
    { 4, 9999,  1, 16, 200,  5, 0, 0, 0, 1573,  1200,  1200, 10},
    { 8,   25, 80, 16, 600,  5, 0, 0, 0, 1576,  1300,  1300, 10},
    { 2,  500, 50, 16, 600,  5, 0, 0, 0, 1572,   650,   650, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, 0, 1302,    30,    30, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, 0, 1311,   130,   130, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, 0,  288,   950,   950, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, 0, 1314,  1000,  1000, 10},
    { 0,    0,  0, 16,   4,  5, 0, 0, 0, 1352,  3000,    10,  8},
    { 0,    0,  0, 16,   4,  5, 0, 0, 0, 1352,  3000,    10,  8},
};

enum WeaponsConfigCmd {
    CCWep_WeaponsCount = 1,
    CCWep_Name,
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
    unsigned int i;
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

        // Parse the [weaponN] sections of loaded file
        sprintf(sect_name, "weapon%d", wtype);
        wdef = &weapon_defs[wtype];
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
#if 0 // TODO add name to weapon params
                i = LbIniValueGetStrWord(&parser, wdef->Name, sizeof(wdef->Name));
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)wdef->Name);
#endif
                break;
            case CCWep_RangeBlocks:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
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
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Sprite);
                break;
            case CCWep_Cost:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Cost = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Cost);
                break;
            case CCWep_Funding:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Funding = k;
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
}

void do_weapon_quantities_net_to_player(struct Thing *p_person)
{
    ushort plyr, cc2, n;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    cc2 = (p_person->U.UPerson.ComCur & 3);

    if (person_carries_weapon(p_person, WEP_NUCLGREN))
        n = net_agents__FourPacks[plyr][cc2][WFRPK_NUCLGREN];
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_NUCLGREN][cc2] = n;

    if (person_carries_weapon(p_person, WEP_ELEMINE))
        n = net_agents__FourPacks[plyr][cc2][WFRPK_ELEMINE];
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_ELEMINE][cc2] = n;

    if (person_carries_weapon(p_person, WEP_EXPLMINE))
        n = net_agents__FourPacks[plyr][cc2][WFRPK_EXPLMINE];
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_EXPLMINE][cc2] = n;

    if (person_carries_weapon(p_person, WEP_KOGAS))
        n = net_agents__FourPacks[plyr][cc2][WFRPK_KOGAS];
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_KOGAS][cc2] = n;

    if (person_carries_weapon(p_person, WEP_CRAZYGAS))
        n = net_agents__FourPacks[plyr][cc2][WFRPK_CRAZYGAS];
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_CRAZYGAS][cc2] = n;
}

void do_weapon_quantities1(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_do_weapon_quantities1\n"
        : : "a" (p_person));
#endif
    ushort plyr, cc2, n;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    cc2 = p_person->U.UPerson.ComCur & 3;
    if (in_network_game)
        return;

    if (person_carries_weapon(p_person, WEP_NUCLGREN))
        n= 4;
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_NUCLGREN][cc2] = n;

    if (person_carries_weapon(p_person, WEP_ELEMINE))
        n = 4;
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_ELEMINE][cc2] = n;

    if (person_carries_weapon(p_person, WEP_EXPLMINE))
        n = 4;
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_EXPLMINE][cc2] = n;

    if (person_carries_weapon(p_person, WEP_KOGAS))
        n = 4;
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_KOGAS][cc2] = n;

    if (person_carries_weapon(p_person, WEP_CRAZYGAS))
        n = 4;
    else
        n = 0;
    players[plyr].FourPacks[WFRPK_CRAZYGAS][cc2] = n;
}

void do_weapon_quantities_proper1(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_do_weapon_quantities_proper1\n"
        : : "a" (p_person));
#endif
    ushort plyr, cc2, n;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    cc2 = p_person->U.UPerson.ComCur & 3;

    if (in_network_game)
    {
        if (person_carries_weapon(p_person, WEP_NUCLGREN))
            n = players[plyr].FourPacks[WFRPK_NUCLGREN][cc2];
        else
            n = 0;
        net_agents__FourPacks[plyr][cc2][2] = n;

        if (person_carries_weapon(p_person, WEP_ELEMINE))
            n = players[plyr].FourPacks[WFRPK_ELEMINE][cc2];
        else
            n = 0;
        net_agents__FourPacks[plyr][cc2][WFRPK_ELEMINE] = n;

        if (person_carries_weapon(p_person, WEP_EXPLMINE))
            n = players[plyr].FourPacks[WFRPK_EXPLMINE][cc2];
        else
            n = 0;
        net_agents__FourPacks[plyr][cc2][WFRPK_EXPLMINE] = n;

        if (person_carries_weapon(p_person, WEP_KOGAS))
            n = players[plyr].FourPacks[WFRPK_KOGAS][cc2];
        else
            n = 0;
        net_agents__FourPacks[plyr][cc2][WFRPK_KOGAS] = n;

        if (person_carries_weapon(p_person, WEP_CRAZYGAS))
            n = players[plyr].FourPacks[WFRPK_CRAZYGAS][cc2];
        else
            n = 0;
        net_agents__FourPacks[plyr][cc2][WFRPK_CRAZYGAS] = n;
    }
    else
    {
        if (person_carries_weapon(p_person, WEP_NUCLGREN))
            n = cryo_agents.FourPacks[cc2][WFRPK_NUCLGREN];
        else
            n = 0;
        players[plyr].FourPacks[WFRPK_NUCLGREN][cc2] = n;

        if (person_carries_weapon(p_person, WEP_ELEMINE))
            n = cryo_agents.FourPacks[cc2][WFRPK_ELEMINE];
        else
            n = 0;
        players[plyr].FourPacks[WFRPK_ELEMINE][cc2] = n;

        if (person_carries_weapon(p_person, WEP_EXPLMINE))
            n = cryo_agents.FourPacks[cc2][WFRPK_EXPLMINE];
        else
            n = 0;
        players[plyr].FourPacks[WFRPK_EXPLMINE][cc2] = n;

        if (person_carries_weapon(p_person, WEP_KOGAS))
            n = cryo_agents.FourPacks[cc2][WFRPK_KOGAS];
        else
            n = 0;
        players[plyr].FourPacks[WFRPK_KOGAS][cc2] = n;

        if (person_carries_weapon(p_person, WEP_CRAZYGAS))
            n = cryo_agents.FourPacks[cc2][WFRPK_CRAZYGAS];
        else
            n = 0;
        players[plyr].FourPacks[WFRPK_CRAZYGAS][cc2] = n;
    }
}

/******************************************************************************/
