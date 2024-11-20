/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file people.c
 *     Routines implementing people.
 * @par Purpose:
 *     Implement general functions for handling person things.
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
#include "people.h"

#include "pepgroup.h"
#include "bfmemory.h"
#include "bffile.h"
#include "bfini.h"
#include "bfutility.h"

#include "bigmap.h"
#include "command.h"
#include "display.h"
#include "drawtext.h"
#include "player.h"
#include "game.h"
#include "game_speed.h"
#include "scandraw.h"
#include "sound.h"
#include "thing.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/
struct PeepStat peep_type_stats[] = {
    {   0,    0,    0,    0,   0, 0,   0, 0, 0, 0},
    {1000, 1024, 2048, 1024,  20, 5, 512, 0, 0, 0},
    {1000, 1024, 2048, 1024,  20, 1, 512, 0, 0, 0},
    { 600,  524, 2048,  601,  10, 2, 312, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 172, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 160, 0, 0, 0},
    { 600,  600,  900,  601,  15, 3, 312, 0, 0, 0},
    {4000, 1000,  800, 2001,9999, 2, 362, 0, 0, 0},
    { 600,  400,  800,  601,   6, 2, 362, 0, 0, 0},
    { 600,  524, 2024,  601,  10, 2, 312, 0, 0, 0},
    { 200,  100,  300,  200,   0, 1, 212, 0, 0, 0},
    { 600,  524, 2024,  601,   0, 1, 312, 0, 0, 0},
    {2000, 1024, 2048, 1024,  20, 5, 512, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 150, 0, 0, 0},
    { 110,  100,  400,  300,   0, 1, 212, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 212, 0, 0, 0},
    {   0,    0,    0,    0,   0, 0,   0, 0, 0, 0},
};

ushort people_frames[SubTT_PERS_COUNT][22] = {
  {   1,   33,   33,    1,   40,   40,   33,   33,   33,   33,   73,   65,    9,   41,   25,   25,   57, 1010, 1018,   17, 1200,   49,},
  {   1,   33,   33,    1,   40,   40,   33,   33,   33,   33,   73,   65,    9,   41,   25,   25,   57, 1010, 1018,   17, 1200,   49,},
  { 113,  113,  113,   81,  113,  113,  113,  113,  113,  113,  153,  145,   89,  121,  105,  105,  137, 1010, 1018,   97, 1212,  129,},
  { 241,  273,  273,    1,   40,   40,  273,  273,  273,  273,  313,  305,  249,  281,  265,  265,  297, 1010, 1018,  241, 1236,  289,},
  { 321,  353,  353,  321,  353,  353,  353,  353,  353,  353,  393,  385,  329,  361,  345,  345,  377, 1010, 1018,  337, 1248,  369,},
  { 401,  433,  433,    1,  433,  433,  433,  433,  433,  433,  473,  465,  409,  441,  425,  425,  457, 1010, 1018,  401, 1260,  449,},
  { 561,  593,  593,  561,  593,  593,  593,  593,  593,  593,  633,  625,  569,  601,  593,  593,  617, 1010, 1018,  577, 1415,  609,},
  {1040, 1040, 1040,    1,   40,   40, 1040, 1040, 1040, 1040,  313,  313, 1018, 1040, 1040, 1040, 1040, 1010, 1018, 1040, 1040, 1040,},
  { 481,  513,  513,  513,  513,  513,  513,  513,  513,  513,  553,  545,  489,  521,  505,  505,  537, 1010, 1018,  497, 1272,  529,},
  { 161,  193,  193,  161,  193,  193,  193,  193,  193,  193,  233,  225,  169,  201,  185,  185,  217, 1010, 1018,  177, 1224,  209,},
  { 641,  673,  673,  673,  673,  673,  673,  673,  673,  673,  713,  705,  649,  665,  689,  689,  697, 1010, 1018,  657, 1295,  689,},
  { 721,  753,  753,  753,  753,  753,  753,  753,  753,  753,  713,  785,  729,  761,  753,  753,  777, 1010, 1018,  737, 1307,  769,},
  { 801,  833,  833,  833,  833,  833,  833,  833,  833,  833,  873,  865,  809,  841,  825,  825,  857, 1010, 1018,  817, 1319,  825,},
  {1331, 1363, 1363, 1363, 1363, 1363, 1363, 1363, 1363, 1363, 1395, 1387, 1339, 1371, 1355, 1355, 1379, 1010, 1018, 1331, 1403, 1347,},
  {1427, 1459, 1459, 1459, 1459, 1459, 1459, 1459, 1459, 1459, 1491, 1483, 1435, 1467, 1451, 1451, 1475, 1010, 1018, 1427, 1499, 1443,},
  {1331, 1363, 1363, 1363, 1363, 1363, 1363, 1363, 1363, 1363, 1395, 1387, 1339, 1371, 1355, 1355, 1379, 1010, 1018, 1331, 1403, 1331,},
};

/** Configuration options for each person state.
 */
struct PeepStateConfig person_states[] = {
  {"NONE_STATE",},
  {"GOTO_POINT",},
  {"BOMB_BUILDING",},
  {"WANDER",},
  {"EXPLODE_EFFECT",},
  {"WAIT",},
  {"AGENT_DEFENSIVE",},
  {"PICKUP_ITEM",},
  {"DROP_ITEM",},
  {"SHOOT_PERS_WT_BG",},
  {"SHOOT_BGUN_AT",},
  {"DIE",},
  {"DIEING",},
  {"DEAD",},
  {"INIT_SAVE_VICTIM",},
  {"SAVE_VICTIM",},
  {"GOTO_PERSON",},
  {"WAIT_P_DEAD",},
  {"WAIT_ALL_G_DEAD",},
  {"WAIT_MEM_G_DEAD",},
  {"WAIT_P_NEAR",},
  {"WAIT_MEM_G_NEAR",},
  {"WAIT_P_ARRIVES",},
  {"WAIT_MEM_G_ARRIV",},
  {"WAIT_TIME",},
  {"DEFINE_ROUTE",},
  {"STATE_UNUSED_1A",},
  {"KILL_PERSON",},
  {"BLOCK_PERSON",},
  {"SCARE_PERSON",},
  {"USE_ITEM",},
  {"STATIONARY_SHOT",},
  {"RECOIL_HIT",},
  {"VEHICLE_PARKED",},
  {"VEHICLE_DRIVEN",},
  {"GOTO_POINT_REL",},
  {"PERSON_BURNING",},
  {"IN_VEHICLE",},
  {"DRIVING_VEHICLE",},
  {"V_GOTO_POINT_FLY",},
  {"PERSUADE_PERSON",},
  {"FOLLOW_PERSON",},
  {"SUPPORT_PERSON",},
  {"PROTECT_PERSON",},
  {"GET_ITEM",},
  {"USE_VEHICLE",},
  {"USE_VEH_TO_POINT",},
  {"WAIT_VEHICLE",},
  {"GOTO_VEHICLE",},
  {"CATCH_TRAIN",},
  {"WAIT_TRAIN",},
  {"DESTROY_BUILDING",},
  {"WANDER_DRIVE",},
  {"GO_PLANT_MINE",},
  {"WAIT_TO_EXIT_VEH",},
  {"CATCH_FERRY",},
  {"EXIT_FERRY",},
  {"AVOID_GROUP",},
  {"STATE_UNUSED_3A",},
  {"BEING_PERSUADED",},
};

struct PeepStatAdd peep_type_stats_a[SubTT_PERS_COUNT] = {0};
struct TbNamedEnum peep_names[SubTT_PERS_COUNT] = {0};

enum PeepTypesConfigCmd {
    PpTyp_PersonTypesCount = 1,
    PpTyp_Name,
    PpTyp_MaxHealth,
    PpTyp_MaxShield,
    PpTyp_MaximumStamina,
    PpTyp_MaxEnergy,
    PpTyp_PersuadeReqd,
    PpTyp_PersuadeWorth,
    PpTyp_Speed,

};

const struct TbNamedEnum people_conf_common_cmds[] = {
  {"PersonTypesCount",	PpTyp_PersonTypesCount},
  {NULL,		0},
};

const struct TbNamedEnum people_conf_person_cmds[] = {
  {"Name",			PpTyp_Name},
  {"MaxHealth",		PpTyp_MaxHealth},
  {"MaxShield",		PpTyp_MaxShield},
  {"MaximumStamina",PpTyp_MaximumStamina},
  {"MaxEnergy",		PpTyp_MaxEnergy},
  {"PersuadeRequired",PpTyp_PersuadeReqd},
  {"PersuadeWorth",	PpTyp_PersuadeWorth},
  {"Speed",			PpTyp_Speed},
  {NULL,			0},
};

const ubyte follow_dist[4][4] = {
  { 0, 3, 6, 9 },
  { 3, 0, 6, 9 },
  { 3, 6, 0, 9 },
  { 3, 6, 9, 0 },
};

void read_people_conf_file(void)
{
    TbFileHandle conf_fh;
    TbBool done;
    int i;
    long k;
    int cmd_num;
    char *conf_buf;
    struct TbIniParser parser;
    char *conf_fname = "conf" FS_SEP_STR "people.ini";
    int conf_len;
    int peep_count, ptype;

    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE) {
        conf_len = LbFileLengthHandle(conf_fh);
        if (conf_len > 1024*1024)
            conf_len = 1024*1024;
        conf_buf = LbMemoryAlloc(conf_len+16);
        conf_len = LbFileRead(conf_fh, conf_buf, conf_len);
        LOGSYNC("Processing '%s' file, %d bytes", conf_fname, conf_len);
        LbFileClose(conf_fh);
    } else {
        LOGERR("Could not open people types config file, going with defaults.");
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    peep_count = 0;
    // Parse the [common] section of loaded file
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return;
    }
    done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(people_conf_common_cmds,cmd_num)
    while (!done)
    {
        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, people_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case PpTyp_PersonTypesCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            peep_count = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), peep_count);
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
    for (ptype = 0; ptype < peep_count; ptype++)
    {
        char sect_name[16];
        struct PeepStat *p_pestat;
        struct PeepStatAdd *p_pestata;

        // Parse the [personN] sections of loaded file
        sprintf(sect_name, "person%d", ptype);
        p_pestat = &peep_type_stats[ptype];
        p_pestata = &peep_type_stats_a[ptype];
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
        done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(people_conf_person_cmds,cmd_num)
        while (!done)
        {
            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, people_conf_person_cmds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case PpTyp_Name:
                i = LbIniValueGetStrWord(&parser, p_pestata->Name, sizeof(p_pestata->Name));
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_pestata->Name);
                break;
            case PpTyp_MaxHealth:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->MaxHealth = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->MaxHealth);
                break;
            case PpTyp_MaxShield:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->MaxShield = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->MaxShield);
                break;
            case PpTyp_MaximumStamina:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->MaximumStamina = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->MaximumStamina);
                break;
            case PpTyp_MaxEnergy:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->MaxEnergy = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->MaxEnergy);
                break;
            case PpTyp_PersuadeReqd:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->PersuadeReqd = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->PersuadeReqd);
                break;
            case PpTyp_PersuadeWorth:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->PersuadeWorth = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->PersuadeWorth);
                break;
            case PpTyp_Speed:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_pestat->Speed = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_pestat->Speed);
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
    for (ptype = 1; ptype < peep_count; ptype++)
    {
        struct PeepStatAdd *p_pestata;

        p_pestata = &peep_type_stats_a[ptype];
        if (strlen(p_pestata->Name) > 0) {
            peep_names[i].name = p_pestata->Name;
            peep_names[i].num = ptype;
            i++;
        }
    }
    peep_names[i].name = NULL;
    peep_names[i].num = 0;
}

void load_peep_type_stats_bin(void)
{
    char locstr[52];
    TbFileHandle fp;

    sprintf(locstr, "%s" FS_SEP_STR "peepdefs.dat", "data");
    fp = LbFileOpen(locstr, 2u);
    if (fp != INVALID_FILE)
    {
        LbFileRead(fp, peep_type_stats, 12 * sizeof(struct PeepStat));
        LbFileClose(fp);
    }
}

void load_peep_type_stats(void)
{
#if 0
    load_peep_type_stats_bin();
#else
    read_people_conf_file();
#endif
}

const char *person_type_name(ushort ptype)
{
    struct PeepStatAdd *p_pestata;

    p_pestata = &peep_type_stats_a[ptype];
    if (strlen(p_pestata->Name) == 0)
        return "OUTRNG_PERSON";
    return p_pestata->Name;
}

void snprint_person_state(char *buf, ulong buflen, struct Thing *p_thing)
{
    char *s;
    ubyte nparams;
    struct PeepStateConfig *pstatcfg;

    pstatcfg = &person_states[p_thing->State];
    s = buf;

    sprintf(s, "%s( ", pstatcfg->Name);
    s += strlen(s);
    nparams = 0;

    if (p_thing->State == PerSt_PROTECT_PERSON)
    {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Person(%d)", (int)p_thing->GotoThingIndex);
        s += strlen(s);
        nparams++;
    }
    if (p_thing->State == PerSt_GOTO_POINT)
    {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Coord(%d,%d,%d)", (int)p_thing->U.UPerson.GotoX, 0, (int)p_thing->U.UPerson.GotoX);
        s += strlen(s);
        nparams++;
    }
    // TODO support more properties

    snprintf(s, buflen - (s-buf), " )");
}


TbBool person_is_dead(ThingIdx thing)
{
    struct Thing *p_thing;

    if (thing <= 0)
        return false;

    p_thing = &things[thing];

    if (p_thing->Type != TT_PERSON)
        return false;

    return (p_thing->State == PerSt_DEAD);
}

TbBool person_is_dead_or_dying(ThingIdx thing)
{
    struct Thing *p_thing;

    if (thing <= 0)
        return false;

    p_thing = &things[thing];

    if (p_thing->Type != TT_PERSON)
        return false;

    return (p_thing->State == PerSt_DEAD)
      || (p_thing->State == PerSt_PERSON_BURNING)
      || (p_thing->State == PerSt_DIEING);
}

ubyte person_mod_chest_level(struct Thing *p_person)
{
    return cybmod_chest_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_chest_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_chest_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_legs_level(struct Thing *p_person)
{
    return cybmod_legs_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_legs_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_legs_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_arms_level(struct Thing *p_person)
{
    return cybmod_arms_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_arms_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_arms_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_brain_level(struct Thing *p_person)
{
    return cybmod_brain_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_brain_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_brain_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_skin_level(struct Thing *p_person)
{
    return cybmod_skin_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_skin_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_skin_level(&p_person->U.UPerson.UMod, nmod);
}

TbBool person_carries_weapon(struct Thing *p_person, ubyte weapon)
{
    return weapons_has_weapon(p_person->U.UPerson.WeaponsCarried, weapon);
}

TbBool person_carries_any_medikit(struct Thing *p_person)
{
    return person_carries_weapon(p_person, WEP_MEDI2) || person_carries_weapon(p_person, WEP_MEDI1);
}

TbBool person_can_accept_control(ThingIdx person)
{
    return !person_is_dead_or_dying(person) && !thing_is_destroyed(person);
}

void person_give_best_mods(struct Thing *p_person)
{
    set_person_mod_brain_level(p_person, 3);
    set_person_mod_legs_level(p_person, 3);
    set_person_mod_arms_level(p_person, 3);
    set_person_mod_chest_level(p_person, 3);
}

TbBool person_is_persuaded(ThingIdx thing)
{
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_person = &things[thing];
    return ((p_person->Flag & TngF_Persuaded) != 0);
}

TbBool person_is_persuaded_by_person(ThingIdx thing, ThingIdx owntng)
{
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_person = &things[thing];
    if ((p_person->Flag & TngF_Persuaded) == 0)
        return false;

    return (p_person->Owner == owntng);
}

TbBool person_is_persuaded_by_player(ThingIdx thing, ushort plyr)
{
    struct Thing *p_thing;
    short plyagent, plygroup;
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_thing = &things[thing];
    if ((p_thing->Flag & TngF_Persuaded) == 0)
        return false;

    plyagent = players[plyr].DirectControl[0];
    plygroup = things[plyagent].U.UPerson.Group;

    p_person = &things[p_thing->Owner];
    return (p_person->U.UPerson.Group == plygroup);
}

short calc_person_speed(struct Thing *p_person)
{
    struct PeepStat *pstat;
    short mood;
    short speed;

    pstat = &peep_type_stats[p_person->SubType];

    mood = p_person->U.UPerson.Mood;
    if (mood < 0)
        mood = -mood;

    speed = pstat->Speed + 2 * mood + 100 * person_mod_legs_level(p_person);

    // Flamer slows you down? Why? This makes it even more useless..
    if (p_person->U.UPerson.CurrentWeapon == WEP_FLAMER)
        speed >>= 1;

    if ((p_person->Flag & TngF_Persuaded) != 0)
        speed += 250;

    if ((p_person->Flag2 & TgF2_Unkn00080000) != 0)
        speed += 512;

    if (speed > PERSON_MAX_SPEED)
        speed = PERSON_MAX_SPEED;

    return speed;
}

void set_person_health_energy_shield_stamina_type(struct Thing *p_person, ushort stype)
{
    struct PeepStat *pstat;

    pstat = &peep_type_stats[stype];

    p_person->U.UPerson.Energy = pstat->MaxEnergy +
        (person_mod_chest_level(p_person) * pstat->MaxEnergy * 50 / 100);

    p_person->U.UPerson.ShieldEnergy = pstat->MaxShield +
        (person_mod_brain_level(p_person) * pstat->MaxShield * 50 / 100);

    p_person->Health = pstat->MaxHealth +
       ((person_mod_chest_level(p_person) * pstat->MaxHealth * 50 / 100) +
        (person_mod_brain_level(p_person) * pstat->MaxHealth * 50 / 100) +
        (person_mod_legs_level(p_person) * pstat->MaxHealth * 25 / 100) +
        (person_mod_arms_level(p_person) * pstat->MaxHealth * 25 / 100)) / 4;

    p_person->U.UPerson.Stamina = pstat->MaximumStamina;

    p_person->U.UPerson.PersuadePower = 0;

    p_person->U.UPerson.MaxHealth = p_person->Health;
    p_person->U.UPerson.MaxEnergy = p_person->U.UPerson.Energy;
    p_person->U.UPerson.MaxShieldEnergy = p_person->U.UPerson.ShieldEnergy;
    p_person->U.UPerson.MaxStamina = p_person->U.UPerson.Stamina;

    if (person_mod_chest_level(p_person) == 4)
    {
        p_person->Health = 2 * PERSON_MAX_HEALTH_LIMIT;
        p_person->U.UPerson.MaxHealth = PERSON_MAX_HEALTH_LIMIT;
    }
}

void person_resurrect(struct Thing *p_person)
{
    p_person->Flag &= ~TngF_Destroyed;
    p_person->Flag &= ~TngF_Unkn02000000;
    p_person->State = PerSt_WAIT;
    p_person->Health = p_person->U.UPerson.MaxHealth * 3 / 4;
    set_person_anim_mode(p_person, 1);
}

void person_set_helath_to_max_limit(struct Thing *p_person)
{
    p_person->U.UPerson.MaxHealth = PERSON_MAX_HEALTH_LIMIT;
    p_person->Health = 2 * p_person->U.UPerson.MaxHealth; // double health - fill red bar
}

void person_set_energy_to_max_limit(struct Thing *p_person)
{
    p_person->U.UPerson.MaxEnergy = 32000;
    p_person->U.UPerson.Energy = p_person->U.UPerson.MaxEnergy;
}

void person_set_persuade_power__to_allow_all(struct Thing *p_person)
{
    short max_required_pers_power;

    max_required_pers_power = 20;
    p_person->U.UPerson.PersuadePower = max(p_person->U.UPerson.PersuadePower,
      max_required_pers_power);
}

ushort calc_person_radius_type(struct Thing *p_person, ushort stype)
{
    ushort r;
    switch (stype)
    {
    case SubTT_PERS_AGENT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_POLICE:
    case SubTT_PERS_PUNK_M:
    default:
        r = 80;
        break;
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_HIGH_PRIEST:
        r = 100;
        break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
        r = 100;
        break;
    case SubTT_PERS_MECH_SPIDER:
        r = 384;
        break;
    }
    return r;
}

void set_person_stats_type(struct Thing *p_person, ushort stype)
{
    set_person_health_energy_shield_stamina_type(p_person, stype);
    p_person->Speed = calc_person_speed(p_person);
}

void reset_person_frame(struct Thing *p_person)
{
    ushort person_anim;

    person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
    p_person->StartFrame = person_anim - 1;
    p_person->Frame = nstart_ani[p_person->StartFrame + 1 + p_person->U.UPerson.Angle];
}

void switch_person_anim_mode(struct Thing *p_person, ubyte animode)
{
    ushort person_anim;

    person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
    p_person->Frame -= nstart_ani[person_anim + p_person->U.UPerson.Angle];
    p_person->U.UPerson.AnimMode = animode;
    person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
    p_person->StartFrame = person_anim - 1;
    p_person->Frame += nstart_ani[p_person->StartFrame + 1 + p_person->U.UPerson.Angle];
}

void set_person_anim_mode(struct Thing *p_person, ubyte animode)
{
    p_person->U.UPerson.AnimMode = animode;
    reset_person_frame(p_person);
}

void change_player_angle(struct Thing *p_person, ushort angle)
{
    short person_anim, sframe;

    if (angle == p_person->U.UPerson.Angle)
        return;

    p_person->U.UPerson.Angle = angle;
    person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode] - 1;
    // TODO why are we not updating p_person->StartFrame here?
    sframe = person_anim - 1;
    p_person->Frame = nstart_ani[sframe + 1 + p_person->U.UPerson.Angle];
}

void init_person_thing(struct Thing *p_person)
{
    set_person_health_energy_shield_stamina_type(p_person, p_person->SubType);

    p_person->Radius = calc_person_radius_type(p_person, p_person->SubType);

    if (current_level != 0)
        p_person->U.UPerson.CurrentWeapon = 0;

    if ((p_person->Flag & TngF_Destroyed) != 0)
    {
        p_person->U.UPerson.AnimMode = 20;
        p_person->State = PerSt_DEAD;
        p_person->U.UPerson.FrameId.Version[3] = 1;
        p_person->U.UPerson.FrameId.Version[4] = 0;
    }
    else
    {
        switch_person_anim_mode(p_person, 0);
    }
}

/** Frees a linked list of paths, and adds the paths to free list.
 */
void path_free_linked_list(ushort first_path)
{
    ushort free_head, path;
    ushort count;

    free_head = head_my_path;
    count = path_count;

    path = first_path;
    while (path != 0)
    {
        ushort tmpath;

        tmpath = my_paths[path].Next;
        my_paths[path].Flag = 0;
        my_paths[path].Next = free_head;
        free_head = path;
        path = tmpath;
        count--;
    }
    path_count = count;
    head_my_path = free_head;
}

void remove_path(struct Thing *p_thing)
{
    ushort path;

    p_thing->Flag2 &= ~TgF2_Unkn0040;
    if (p_thing->U.UPerson.PathIndex != 0)
    {
        if (p_thing->Type == TT_PERSON)
            p_thing->U.UPerson.LastDist = 32000;
        path = p_thing->U.UPerson.PathIndex;
        p_thing->U.UPerson.PathIndex = 0;
        path_free_linked_list(path);
        p_thing->PathOffset = 0;
        p_thing->Flag &= ~TngF_Unkn00020000;
    }
}

void set_person_animmode_walk(struct Thing *p_person)
{
    asm volatile ("call ASM_set_person_animmode_walk\n"
        : : "a" (p_person));
}

int can_i_see_thing(struct Thing *p_me, struct Thing *p_him, int max_dist, ushort flags)
{
    int ret;
    asm volatile ("call ASM_can_i_see_thing\n"
        : "=r" (ret) : "a" (p_me), "d" (p_him), "b" (max_dist), "c" (flags));
    return ret;
}

TbBool can_i_enter_vehicle(struct Thing *p_me, struct Thing *p_vehicle)
{
    TbBool ret;
    asm volatile ("call ASM_can_i_enter_vehicle\n"
        : "=r" (ret) : "a" (p_me), "d" (p_vehicle));
    return ret;
}

void persuaded_person_add_to_stats(struct Thing *p_person, ushort brief)
{
    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
          ++mission_status[brief].AgentsGained;
          // fall through
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_HIGH_PRIEST:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_PUNK_M:
          ++mission_status[brief].EnemiesPersuaded;
          break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
    case SubTT_PERS_FAST_BLOND_F:
          ++mission_status[brief].CivsPersuaded;
          break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_MECH_SPIDER:
    case SubTT_PERS_POLICE:
          ++mission_status[brief].SecurityPersuaded;
          break;
    default:
          break;
    }
}

void persuaded_person_remove_from_stats(struct Thing *p_person, ushort brief)
{
    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
        --mission_status[brief].AgentsGained;
        // fall through
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_PUNK_M:
    case SubTT_PERS_HIGH_PRIEST:
        --mission_status[brief].EnemiesPersuaded;
        break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
    case SubTT_PERS_FAST_BLOND_F:
        --mission_status[brief].CivsPersuaded;
        break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_MECH_SPIDER:
    case SubTT_PERS_POLICE:
        --mission_status[brief].SecurityPersuaded;
        break;
    default:
        break;
    }
}

void set_person_persuaded(struct Thing *p_person, struct Thing *p_attacker, ushort energy)
{
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
        return;

    if (p_person->State == PerSt_DEAD)
        return;

    play_dist_sample(p_person, 20, 127, 64, 100, 0, 3);
    remove_path(p_person);
    set_person_animmode_walk(p_person);
    p_attacker->U.UPerson.Energy -= energy;
    if (p_attacker->U.UPerson.Energy < 0)
        p_attacker->U.UPerson.Energy = 0;
    p_person->PTarget = NULL;
    p_person->U.UPerson.Target2 = 0;
    p_person->State = 0;

    p_person->Flag |= TngF_Unkn40000000 | TngF_Unkn0004;
    p_person->Owner = p_attacker->ThingOffset;
    p_person->Flag &= ~(TngF_Unkn00800000|TngF_Unkn00040000|TngF_Unkn00020000|TngF_Unkn0800|TngF_Unkn0080);
    set_person_animmode_walk(p_person);
    p_person->U.UPerson.ComTimer = -1;
    p_person->U.UPerson.ComRange = 3;
    p_person->U.UPerson.Timer2 = 5;
    p_person->U.UPerson.StartTimer2 = 5;
    p_person->SubState = 0;

    p_person->U.UPerson.EffectiveGroup = p_attacker->U.UPerson.EffectiveGroup;
    p_person->U.UPerson.Within = 0;
    {
        struct PeepStat *pstat;

        pstat = &peep_type_stats[p_person->SubType];
        p_attacker->U.UPerson.PersuadePower += pstat->PersuadeWorth;
    }
    if ((p_person->Flag2 & TgF2_KnockedOut) == 0)
    {
          p_person->StartFrame = 1059;
          p_person->Frame = nstart_ani[p_person->StartFrame + 1];
    }
    p_person->Timer1 = 48;
    p_person->StartTimer1 = 48;
    {
        word_17FA58[word_1531DA] = p_person->ThingOffset;
        word_1531DA++;
    }
    p_person->State = PerSt_BEING_PERSUADED;
    {
        ushort group;
        group = p_person->U.UPerson.Group & 0x1F;
        group_actions[group].Persuaded++;
    }
    if (!in_network_game && (p_attacker->Flag & TngF_PlayerAgent) &&
      (p_attacker->U.UPerson.EffectiveGroup == ingame.MyGroup))
    {
        short plagent;

        persuaded_person_add_to_stats(p_person, open_brief);

        for (plagent = 0; plagent < playable_agents; plagent++)
        {
            struct Thing *p_agent;

            p_agent = players[local_player_no].MyAgent[plagent];
            if (p_person == p_agent->PTarget)
                p_agent->PTarget = NULL;
            if (p_agent->U.UPerson.Target2 == p_person->ThingOffset)
                p_agent->U.UPerson.Target2 = 0;
        }
    }
}

void unpersuade_my_peeps(struct Thing *p_owntng)
{
    ThingIdx person;
    struct Thing *p_person;
    ushort count;
    int i, k;

    count = word_1531DA;
    for (i = 1; i < count; i++)
    {
        person = word_17FA58[i];
        p_person = &things[person];
        if (p_person->Owner != p_owntng->ThingOffset)
            continue;
        p_person->Flag &= ~TngF_Persuaded;
        p_person->U.UPerson.EffectiveGroup = p_owntng->U.UPerson.Group;
        p_person->State = 0;
        p_person->Flag |= TngF_Unkn0040 | TngF_Unkn0004;
        --group_actions[p_person->U.UPerson.Group].Persuaded;
        for (k = i; k < count - 1; k++) {
            word_17FA58[k] = word_17FA58[k+1];
        }
        count--;
        if (!in_network_game && (p_owntng->U.UPerson.EffectiveGroup == ingame.MyGroup))
        {
            persuaded_person_remove_from_stats(p_person, open_brief);
        }
    }
    p_owntng->U.UPerson.PersuadePower = 0;
    word_1531DA = count;
}

struct Thing *new_sim_person(int x, int y, int z, ubyte subtype)
{
#if 0
    struct Thing *p_person;
    asm volatile ("call ASM_new_sim_person\n"
        : "=r" (p_person) : "a" (x), "d" (y), "b" (z), "c" (subtype));
    return p_person;
#endif
    ubyte ptype;
    struct PeepStat *p_pestat;
    ThingIdx person;
    struct Thing *p_person;
    ushort rnd;
    int ry;

    ptype = subtype;
    if (subtype <= 100) {
        person = get_new_thing();
    } else {
        ptype = SubTT_PERS_AGENT;
        person = unkn01_thing_idx;
    }
    if (person <= 0)
        return &things[0];
    p_pestat = &peep_type_stats[ptype];
    rnd = LbRandomAnyShort();

    p_person = &things[person];
    p_person->X = MAPCOORD_TO_PRCCOORD(x,0);
    p_person->Z = MAPCOORD_TO_PRCCOORD(z,0);
    p_person->U.UPerson.Target2 = 0;
    ry = alt_at_point(x, z);
    p_person->Speed = 512;
    p_person->U.UPerson.Angle = (rnd >> 1) % 8;
    p_person->U.UPerson.AnimMode = 0;
    p_person->StartTimer1 = 48;
    p_person->Timer1 = 48;
    p_person->Y = ry;
    p_person->U.UPerson.OnFace = 0;
    p_person->Type = TT_PERSON;
    p_person->Radius = calc_person_radius_type(p_person, ptype);
    switch (ptype)
    {
    case SubTT_PERS_AGENT:
        p_person->U.UPerson.Angle = 0;
        p_person->U.UPerson.CurrentWeapon = WEP_MINIGUN;
        break;
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_HIGH_PRIEST:
        p_person->U.UPerson.CurrentWeapon = WEP_ELLASER;
        break;
    case SubTT_PERS_PUNK_F:
        p_person->U.UPerson.CurrentWeapon = WEP_UZI;
        p_person->U.UPerson.FrameId.Version[0] = rnd % 3;
        break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
        p_person->U.UPerson.CurrentWeapon = WEP_NULL;
        break;
    case SubTT_PERS_MECH_SPIDER:
        p_person->U.UPerson.CurrentWeapon = WEP_LASER;
        break;
    case SubTT_PERS_POLICE:
        p_person->U.UPerson.CurrentWeapon = WEP_LASER;
        break;
    case SubTT_PERS_PUNK_M:
        p_person->U.UPerson.CurrentWeapon = WEP_UZI;
        break;
    case SubTT_PERS_SHADY_M:
        p_person->U.UPerson.CurrentWeapon = WEP_UZI;
        break;
    default:
        break;
    }
    p_person->SubType = ptype;
    p_person->U.UPerson.Group = ptype + 4;
    p_person->U.UPerson.EffectiveGroup = p_person->U.UPerson.Group;
    p_person->U.UPerson.AnimMode = (p_person->U.UPerson.CurrentWeapon != WEP_NULL) ? 1 : 0;
    reset_person_frame(p_person);
    init_person_thing(p_person);
    p_person->U.UPerson.WeaponsCarried = 0;
    if (p_person->U.UPerson.CurrentWeapon)
        p_person->U.UPerson.WeaponsCarried |= (1 << (p_person->U.UPerson.CurrentWeapon - 1));
    p_person->U.UPerson.WeaponsCarried |= (1 << (WEP_ENERGYSHLD-1));
    p_person->U.UPerson.EffectiveGroup = p_person->U.UPerson.Group;
    p_person->Speed = p_pestat->Speed;
    add_node_thing(person);
    p_person->U.UPerson.ComHead = 0;
    p_person->U.UPerson.ComCur = 0;
    return p_person;
}

short check_for_other_people(int x, int y, int z, struct Thing *p_person)
{
    short ret;
    asm volatile (
      "call ASM_check_for_other_people\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (p_person));
    return ret;
}

void check_persons_target(struct Thing *p_person)
{
    asm volatile ("call ASM_check_persons_target\n"
        : : "a" (p_person));
}

void check_persons_target2(struct Thing *p_person)
{
    asm volatile ("call ASM_check_persons_target2\n"
        : : "a" (p_person));
}

void process_stamina(struct Thing *p_person)
{
    asm volatile ("call ASM_process_stamina\n"
        : : "a" (p_person));
}

void person_init_command(struct Thing *p_person, ushort from)
{
    asm volatile ("call ASM_person_init_command\n"
        : : "a" (p_person), "d" (from));
}

ubyte conditional_command_state_true(ushort com, struct Thing *p_me, ubyte from)
{
    ubyte ret;
    asm volatile (
      "call ASM_conditional_command_state_true\n"
        : "=r" (ret) : "a" (com), "d" (p_me), "b" (from));
    return ret;
}

ubyte is_command_completed(struct Thing *p_person)
{
    ubyte ret;
    asm volatile (
      "call ASM_is_command_completed\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

int person_goto_person_nav(struct Thing *p_person)
{
    int ret;
    asm volatile (
      "call ASM_person_goto_person_nav\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

void person_goto_point(struct Thing *p_person)
{
    asm volatile ("call ASM_person_goto_point\n"
        : : "a" (p_person));
}

void person_goto_point_rel(struct Thing *p_person)
{
    asm volatile ("call ASM_person_goto_point_rel\n"
        : : "a" (p_person));
}

void person_init_drop(struct Thing *p_person, ThingIdx item)
{
    asm volatile ("call ASM_person_init_drop\n"
        : : "a" (p_person), "d" (item));
}

void person_init_drop_special(struct Thing *p_person, ThingIdx item)
{
    asm volatile ("call ASM_person_init_drop_special\n"
        : : "a" (p_person), "d" (item));
}

void stop_looped_weapon_sample(struct Thing *p_person, short weapon)
{
    asm volatile ("call ASM_stop_looped_weapon_sample\n"
        : : "a" (p_person), "d" (weapon));
}

ubyte person_attempt_to_leave_vehicle(struct Thing *p_thing)
{
    ubyte ret;
    asm volatile (
      "call ASM_person_attempt_to_leave_vehicle\n"
        : "=r" (ret) : "a" (p_thing));
    return ret;
}

void person_attempt_to_leave_ferry(struct Thing *p_thing)
{
    asm volatile ("call ASM_person_attempt_to_leave_ferry\n"
        : : "a" (p_thing));
}
void person_scare_person(struct Thing *p_person)
{
    struct Thing *p_target;

    person_goto_person_nav(p_person);
    if (p_person->State == PerSt_NONE)
    {
        int range;

        p_person->State = PerSt_SCARE_PERSON;
        range = get_weapon_range(p_person);
        if (can_i_see_thing(p_person, p_person->PTarget, range, 0) > 0)
        {
            p_person->Flag |= TngF_Unkn0800;
            if ((p_person->Flag & TngF_InVehicle) != 0)
            {
                struct Thing *p_vehicle;

                p_vehicle = &things[p_person->U.UPerson.Vehicle];
                p_vehicle->Flag |= TngF_Unkn01000000;
            }
        }
    }
    p_target = p_person->PTarget;
    if (p_target->State == PerSt_DEAD)
    {
        p_person->State = PerSt_NONE;
    }
}

void person_block_person(struct Thing *p_person)
{
    struct Thing *p_target;

    person_goto_person_nav(p_person);
    if (p_person->State == PerSt_NONE)
    {
        int range;

        p_person->State = PerSt_BLOCK_PERSON;
        range = get_weapon_range(p_person);
        if (can_i_see_thing(p_person, p_person->PTarget, range * range, 0) > 0)
        {
            p_person->Flag |= TngF_Unkn0800;
            if ((p_person->Flag & TngF_InVehicle) != 0)
            {
                struct Thing *p_vehicle;

                p_vehicle = &things[p_person->U.UPerson.Vehicle];
                p_vehicle->Flag |= TngF_Unkn01000000;
            }
        }
    }
    p_target = p_person->PTarget;
    if (p_target->State == PerSt_DEAD)
    {
        p_person->State = PerSt_NONE;
    }
}

void process_stationary_shot(struct Thing *p_person)
{
    if (p_person->U.UPerson.WeaponTurn == 0)
    {
        if ((p_person->Flag & TngF_Unkn0800) == 0)
            p_person->Flag &= ~TngF_StationrSht;
    }
}

void process_wait_train(struct Thing *p_person)
{
    if ((p_person->Flag & TngF_Unkn4000) != 0)
    {
        p_person->SubState = 25;
    }
    else if (p_person->SubState == 25)
    {
        p_person->State = PerSt_NONE;
    }
}

void person_follow_person(struct Thing *p_person)
{
    struct Thing *p_target;

    person_goto_person_nav(p_person);
    if (p_person->State == PerSt_NONE)
    {
        p_person->State = PerSt_FOLLOW_PERSON;
    }
    p_target = &things[p_person->GotoThingIndex];
    if ((p_target->Flag & TngF_Destroyed) != 0)
    {
        p_person->State = PerSt_NONE;
    }
}

void person_go_plant_mine(struct Thing *p_person)
{
    person_goto_point(p_person);
    if (p_person->State == PerSt_NONE)
    {
        person_init_drop(p_person, p_person->U.UPerson.CurrentWeapon);
    }
}

void process_knocked_out(struct Thing *p_person)
{
    asm volatile ("call ASM_process_knocked_out\n"
        : : "a" (p_person));
}

void process_tasered_person(struct Thing *p_person)
{
    asm volatile ("call ASM_process_tasered_person\n"
        : : "a" (p_person));
}

void person_intel(struct Thing *p_person)
{
    asm volatile ("call ASM_person_intel\n"
        : : "a" (p_person));
}

void process_im_shoved(struct Thing *p_person)
{
    asm volatile ("call ASM_process_im_shoved\n"
        : : "a" (p_person));
}

void process_protect_person(struct Thing *p_person)
{
    asm volatile ("call ASM_process_protect_person\n"
        : : "a" (p_person));
}

void process_wander(struct Thing *p_person)
{
    asm volatile ("call ASM_process_wander\n"
        : : "a" (p_person));
}

void person_recoil(struct Thing *p_person)
{
    asm volatile ("call ASM_person_recoil\n"
        : : "a" (p_person));
}

void calc_lighting(struct Thing *p_person)
{
    asm volatile ("call ASM_calc_lighting\n"
        : : "a" (p_person));
}

void process_persuaded(struct Thing *p_person)
{
    asm volatile ("call ASM_process_persuaded\n"
        : : "a" (p_person));
}

void process_danger(struct Thing *p_person)
{
    asm volatile ("call ASM_process_danger\n"
        : : "a" (p_person));
}

void person_kill_target2(struct Thing *p_person)
{
    asm volatile ("call ASM_person_kill_target2\n"
        : : "a" (p_person));
}

void person_wait(struct Thing *p_person)
{
    asm volatile ("call ASM_person_wait\n"
        : : "a" (p_person));
}

void person_pickup(struct Thing *p_person)
{
    asm volatile ("call ASM_person_pickup\n"
        : : "a" (p_person));
}

void person_drop_item(struct Thing *p_person)
{
    asm volatile ("call ASM_person_drop_item\n"
        : : "a" (p_person));
}

void person_dieing(struct Thing *p_person)
{
    asm volatile ("call ASM_person_dieing\n"
        : : "a" (p_person));
}

void person_dead(struct Thing *p_person)
{
    asm volatile ("call ASM_person_dead\n"
        : : "a" (p_person));
}

void person_kill_person(struct Thing *p_person)
{
    asm volatile ("call ASM_person_kill_person\n"
        : : "a" (p_person));
}

void person_save_victim(struct Thing *p_person)
{
    asm volatile ("call ASM_person_save_victim\n"
        : : "a" (p_person));
}

void person_get_item(struct Thing *p_person)
{
    asm volatile ("call ASM_person_get_item\n"
        : : "a" (p_person));
}

void person_run_away(struct Thing *p_person)
{
    asm volatile ("call ASM_person_run_away\n"
        : : "a" (p_person));
}

void person_burning(struct Thing *p_person)
{
    asm volatile ("call ASM_person_burning\n"
        : : "a" (p_person));
}

void person_persuade_person(struct Thing *p_person)
{
    asm volatile ("call ASM_person_persuade_person\n"
        : : "a" (p_person));
}

void process_support_person(struct Thing *p_person)
{
    asm volatile ("call ASM_process_support_person\n"
        : : "a" (p_person));
}

void person_use_vehicle(struct Thing *p_person)
{
    asm volatile ("call ASM_person_use_vehicle\n"
        : : "a" (p_person));
}

void person_wait_vehicle(struct Thing *p_person)
{
    asm volatile ("call ASM_person_wait_vehicle\n"
        : : "a" (p_person));
}

void person_destroy_building(struct Thing *p_person)
{
    asm volatile ("call ASM_person_destroy_building\n"
        : : "a" (p_person));
}

void person_catch_train(struct Thing *p_person)
{
    asm volatile ("call ASM_person_catch_train\n"
        : : "a" (p_person));
}

void process_avoid_group(struct Thing *p_person)
{
    asm volatile ("call ASM_process_avoid_group\n"
        : : "a" (p_person));
}

void person_being_persuaded(struct Thing *p_person)
{
    asm volatile ("call ASM_person_being_persuaded\n"
        : : "a" (p_person));
}

void process_wander_and_fly(struct Thing *p_person, struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_wander_and_fly\n"
        : : "a" (p_person), "d" (p_vehicle));
}

void set_peep_comcur(struct Thing *p_person)
{
    asm volatile ("call ASM_set_peep_comcur\n"
        : : "a" (p_person));
}

void process_lighting_unkn1(struct Thing *p_person)
{
    asm volatile ("call ASM_process_lighting_unkn1\n"
        : : "a" (p_person));
}

void person_find_next_state(struct Thing *p_person)
{
    short cmd;

    if (((p_person->Flag & TngF_PlayerAgent) != 0) && ((p_person->Flag2 & TgF2_Unkn0800) == 0))
    {
        p_person->State = PerSt_WAIT;
        p_person->U.UPerson.ComTimer = 50;
        return;
    }

    if (p_person->U.UPerson.ComCur == 0)
    {
        if (((p_person->Flag & TngF_PlayerAgent) != 0) && ((p_person->Flag2 & TgF2_Unkn0800) != 0))
        {
            p_person->Flag2 &= ~TgF2_Unkn0800;
            ingame.Flags &= ~GamF_Unkn0100;
            set_peep_comcur(p_person);
        }
        p_person->State = PerSt_WAIT;
        p_person->U.UPerson.ComTimer = 50;
        return;
    }

    if ((p_person->Flag & TngF_Unkn0040) != 0)
    {
        p_person->Flag &= ~TngF_Unkn0040;
        person_init_command(p_person, PCmd_GET_ITEM);
        return;
    }

    if (!is_command_completed(p_person))
        return;

    if (p_person->U.UPerson.ComHead == 0)
    {
        p_person->State = PerSt_WAIT;
        p_person->U.UPerson.ComTimer = 50;
        return;
    }

    cmd = p_person->U.UPerson.ComCur;
    cmd = game_commands[cmd].Next;
    p_person->U.UPerson.ComCur = cmd;

    // Repeat with new ComCur
    if (p_person->U.UPerson.ComCur == 0)
    {
        if (((p_person->Flag & TngF_PlayerAgent) != 0) && ((p_person->Flag2 & TgF2_Unkn0800) != 0))
        {
            p_person->Flag2 &= ~TgF2_Unkn0800;
            ingame.Flags &= ~GamF_Unkn0100;
            set_peep_comcur(p_person);
        }
        p_person->State = PerSt_WAIT;
        p_person->U.UPerson.ComTimer = 100;
        return;
    }

    person_init_command(p_person, PCmd_USE_WEAPON);
}

void make_peep_protect_peep(struct Thing *p_protector, struct Thing *p_leader)
{
    asm volatile ("call ASM_make_peep_protect_peep\n"
        : : "a" (p_protector), "d" (p_leader));
}

void people_intel(ubyte flag)
{
    asm volatile ("call ASM_people_intel\n"
        : : "a" (flag));
}

void process_person(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_process_person\n"
        : : "a" (p_person));
    return;
#endif
    struct MyMapElement *p_mapel;
    short state;

    if ( (p_person->Flag & TngF_PlayerAgent) != 0 && (p_person->Flag2 & TgF2_Unkn10000000) != 0 )
    {
        state = p_person->State;
        if ((state != PerSt_GET_ITEM) && (state != PerSt_DROP_ITEM) && (state != PerSt_PICKUP_ITEM)
          && ((p_person->Flag & (TngF_Unkn40000000|TngF_Destroyed)) == 0)
          && ((p_person->Flag2 & (TgF2_KnockedOut|TgF2_Unkn0008)) == 0))
        {
            struct Thing *p_target;

            p_person->GotoThingIndex = p_person->Owner;
            p_target = &things[p_person->GotoThingIndex];
            p_person->State = PerSt_PROTECT_PERSON;
            p_person->U.UPerson.ComRange = follow_dist[p_target->U.UPerson.ComCur & 3][p_person->U.UPerson.ComCur & 3];
            p_person->Flag2 &= ~TgF2_Unkn10000000;
        }
    }
    p_person->Flag2 &= ~TgF2_Unkn8000;
    p_mapel = &game_my_big_map[MAP_TILE_WIDTH * (p_person->Z >> 16) + (p_person->X >> 16)];
    if (((p_person->Flag2 & TgF2_InsideBuilding) != 0) && (p_mapel->ColumnHead == 0))
        p_person->Flag2 &= ~TgF2_InsideBuilding;

    if ( ((gameturn + p_person->ThingOffset) & 0x7F) == 0)
    {
        p_person->U.UPerson.Flag3 &= ~0x0020;
        if ((p_person->Flag2 & TgF2_Unkn00020000) != 0)
        {
            p_person->U.UPerson.ComTimer = -1;
            p_person->Flag2 &= ~TgF2_Unkn00020000;
            remove_path(p_person);
        }
    }
    if (((p_person->Flag2 & TgF2_Unkn08000000) != 0) && ((p_person->Flag & TngF_PlayerAgent) != 0))
    {
        state = p_person->State;
        if ((state != PerSt_GET_ITEM) && (state != PerSt_PICKUP_ITEM))
        {
          make_peep_protect_peep(p_person, &things[p_person->Owner]);
          func_711F4(p_person->X >> 8, p_person->Y >> 8, p_person->Z >> 8, 200, colour_lookup[1]);
        }
    }
    if ((p_person->Flag2 & TgF2_Unkn0020) != 0)
    {
        return;
    }
    if ((p_person->Flag2 & TgF2_Unkn00100000) != 0)
    {
        process_tasered_person(p_person);
        return;
    }
    if ((p_person->Flag2 & TgF2_KnockedOut) != 0)
    {
        process_knocked_out(p_person);
        stop_looped_weapon_sample(p_person, p_person->U.UPerson.CurrentWeapon);
        return;
    }
    if ((p_person->Flag2 & TgF2_Unkn0008) != 0)
    {
        p_person->U.UPerson.BumpCount--;
        if (p_person->U.UPerson.BumpCount != 0)
        {
            if (((gameturn + p_person->ThingOffset) & 7) == 0)
                person_intel(p_person);
        }
        else
        {
            ushort group;

            group = p_person->U.UPerson.EffectiveGroup;
            p_person->Flag2 &= ~TgF2_Unkn0008;
            if (group >= 128)
              p_person->U.UPerson.EffectiveGroup = group + 128;
            p_person->U.UPerson.Target2 = 0;
            p_person->PTarget = NULL;
            p_person->State = 0;
            p_person->Flag |= TngF_Unkn0040;
            calc_person_speed(p_person);
        }
    }
    else
    {
        ushort limit;
        limit = 8;
        if ((gameturn & 0x7FF) == 0)
            p_person->U.UPerson.BumpCount = 0;
        if ((p_person->Flag & TngF_PlayerAgent) != 0)
            limit = 2;
        if (p_person->U.UPerson.BumpCount > limit)
        {
            p_person->U.UPerson.BumpCount = 0;
            p_person->Flag &= ~TngF_Unkn00020000;
            if ((p_person->Flag & TngF_Persuaded) == 0)
                p_person->Flag |= TngF_Unkn0004;
            remove_path(p_person);
            p_person->U.UPerson.ComTimer = -1;
        }
    }
    if ((gameturn & 3) == 0)
    {
        state = p_person->State;
        if ((state != PerSt_WAIT) && (state != 0))
        {
            p_person->Flag &= ~TngF_Unkn08000000;
            p_person->Flag &= ~TngF_Unkn01000000;
        }
    }
    if ((p_person->U.UPerson.BumpMode != 0) && ((p_person->Flag & TngF_Destroyed) == 0))
    {
        if ((p_person->Flag2 & TgF2_Unkn0100) != 0)
            p_person->U.UPerson.BumpMode = 0;
        else
            process_im_shoved(p_person);
    }
    if (((p_person->Flag & TngF_Unkn40000000) == 0) || (((p_person->Flag & TngF_Destroyed) == 0)
        && ((p_person->Flag & (TngF_Persuaded|TngF_Unkn00040000|TngF_WepRecoil|TngF_Unkn4000|TngF_StationrSht)) != 0)))
    {

        if ((p_person->Flag & TngF_Unkn4000) != 0)
          return;
        if ((p_person->Flag & TngF_WepRecoil) != 0)
        {
          stop_looped_weapon_sample(p_person, p_person->U.UPerson.CurrentWeapon);
          person_recoil(p_person);
          calc_lighting(p_person);
          process_weapon(p_person);
          return;
        }
        if (((p_person->Flag & TngF_Persuaded) != 0) && ((p_person->Flag & TngF_Destroyed) == 0))
        {
          process_persuaded(p_person);
          calc_lighting(p_person);
          process_weapon(p_person);
          return;
        }
        if (((p_person->Flag & TngF_Unkn00040000) != 0) && ((p_person->Flag & (TngF_Unkn1000|TngF_Destroyed)) == 0))
        {
          person_run_away(p_person);
          calc_lighting(p_person);
          return;
        }

        if ((p_person->Flag & TngF_StationrSht) != 0)
        {
          if ((p_person->U.UPerson.WeaponTurn == 0) && ((p_person->Flag & TngF_Unkn0800) == 0))
          {
              p_person->Flag &= ~TngF_StationrSht;
          }
          process_weapon(p_person);
          return;
        }
    }

    if (((p_person->Flag & (TngF_InVehicle|TngF_Unkn40000000|TngF_Persuaded|TngF_Unkn4000)) == 0)
      && ((p_person->Flag2 & TgF2_Unkn0008) == 0)
      && ((gameturn + p_person->ThingOffset) & 3) == 0
      && (p_person->Flag & TngF_Destroyed) == 0)
    {
        ushort subType;

        subType = p_person->SubType;
        if (subType == SubTT_PERS_BRIEFCASE_M
          || subType == SubTT_PERS_WHITE_BRUN_F
          || subType == SubTT_PERS_WHIT_BLOND_F
          || subType == SubTT_PERS_LETH_JACKT_M
          || subType == SubTT_PERS_SCIENTIST)
        {
            process_danger(p_person);
        }
    }

    if (((p_person->Flag & (TngF_Unkn40000000|TngF_PlayerAgent)) == 0 || (p_person->Flag2 & TgF2_Unkn0800) != 0)
      && ((gameturn + p_person->ThingOffset) & 7) == 0)
    {
        if (p_person->U.UPerson.ComHead != 0)
        {
          struct Command *p_cmd;
          TbBool cond_met;
          short cmd;

          cmd = p_person->U.UPerson.ComCur;
          p_cmd = &game_commands[cmd];
          if (((p_cmd->Flags & 0x0002) != 0) && ((p_person->Flag & TngF_Destroyed) == 0))
          {
              cmd = p_cmd->Next;
              cond_met = 0;
              for (p_cmd = &game_commands[cmd]; (p_cmd->Flags & 0x0004) != 0;
                p_cmd = &game_commands[p_cmd->Next])
              {
                  if (cmd == 0)
                      break;
                  if (conditional_command_state_true(cmd, p_person, 3))
                      cond_met = 1;
                  cmd = p_cmd->Next;
              }
              if (!cond_met)
                  cmd = -1;
              if (cmd >= 0)
              {
                  p_person->U.UPerson.ComCur = cmd;
                  p_person->State = 0;
                  person_init_command(p_person, PCmd_PROTECT_PERSON);
              }
          }
        }
    }

    if ((p_person->Flag & (TngF_Unkn40000000|TngF_Destroyed)) != 0)
        p_person->Flag &= ~TngF_Unkn0800;
    else
        process_weapon(p_person);

    if ((debug_hud_collision == 1) && (p_person->Y != 0))
    {
        char locstr[150];

        sprintf(locstr, "%ld", (long)p_person->Y);
        draw_text_transformed(p_person->X >> 8, p_person->Y, p_person->Z >> 8, locstr);
    }

    if ((p_person->U.UPerson.Target2 != 0) && ((p_person->Flag & (TngF_Unkn40000000|TngF_Destroyed)) == 0))
    {
        person_kill_target2(p_person);
        if (p_person->State == PerSt_PROTECT_PERSON)
          process_protect_person(p_person);
    }
    else if ((p_person->Flag2 & TgF2_Unkn0008) != 0)
    {
        process_wander(p_person);
        if (p_person->U.UPerson.Timer2 == 1)
            p_person->Speed = 500 + (LbRandomAnyShort() % 700);
    }
    else
    {
        switch (p_person->State & 0x7FFF)
        {
        case PerSt_NONE:
              person_find_next_state(p_person);
              break;
        case PerSt_GOTO_POINT:
              person_goto_point(p_person);
              if ((p_person->State == 0) && ((p_person->U.UPerson.Flag3 & 0x04) != 0) && ((p_person->Flag & TngF_Unkn1000) == 0))
              {
                  struct Thing *p_target;

                  p_target = &things[p_person->GotoThingIndex];
                  p_person->State = PerSt_PROTECT_PERSON;
                  p_person->U.UPerson.ComRange = follow_dist[p_target->U.UPerson.ComCur & 3][p_person->U.UPerson.ComCur & 3];
              }
              break;
        case PerSt_WANDER:
              process_wander(p_person);
              break;
        case PerSt_WAIT:
              person_wait(p_person);
              break;
        case PerSt_PICKUP_ITEM:
              person_pickup(p_person);
              break;
        case PerSt_DROP_ITEM:
              person_drop_item(p_person);
              break;
        case PerSt_DIEING:
              person_dieing(p_person);
              break;
        case PerSt_DEAD:
              person_dead(p_person);
              break;
        case PerSt_INIT_SAVE_VICTIM:
        case PerSt_SAVE_VICTIM:
              person_save_victim(p_person);
              break;
        case PerSt_GOTO_PERSON:
              person_goto_person_nav(p_person);
              break;
        case PerSt_KILL_PERSON:
              person_kill_person(p_person);
              break;
        case PerSt_BLOCK_PERSON:
              person_block_person(p_person);
              break;
        case PerSt_SCARE_PERSON:
              person_scare_person(p_person);
              break;
        case PerSt_STATIONARY_SHOT:
              process_stationary_shot(p_person);
              break;
        case PerSt_RECOIL_HIT:
              person_recoil(p_person);
              break;
        case PerSt_GOTO_POINT_REL:
              person_goto_point_rel(p_person);
              break;
        case PerSt_PERSON_BURNING:
              person_burning(p_person);
              break;
        case PerSt_PERSUADE_PERSON:
              person_persuade_person(p_person);
              break;
        case PerSt_FOLLOW_PERSON:
              person_follow_person(p_person);
              break;
        case PerSt_SUPPORT_PERSON:
              process_support_person(p_person);
              break;
        case PerSt_PROTECT_PERSON:
              process_protect_person(p_person);
              break;
        case PerSt_GET_ITEM:
              person_get_item(p_person);
              break;
        case PerSt_USE_VEHICLE:
              person_use_vehicle(p_person);
              break;
        case PerSt_WAIT_VEHICLE:
              person_wait_vehicle(p_person);
              break;
        case PerSt_CATCH_TRAIN:
              person_catch_train(p_person);
              break;
        case PerSt_WAIT_TRAIN:
              process_wait_train(p_person);
              break;
        case PerSt_DESTROY_BUILDING:
              person_destroy_building(p_person);
              break;
        case PerSt_GO_PLANT_MINE:
              person_go_plant_mine(p_person);
              break;
        case PerSt_WAIT_TO_EXIT_VEHICLE:
              person_attempt_to_leave_vehicle(p_person);
              break;
        case PerSt_EXIT_FERRY:
              person_attempt_to_leave_ferry(p_person);
              break;
        case PerSt_AVOID_GROUP:
              process_avoid_group(p_person);
              break;
        case PerSt_UNUSED_3A:
              person_init_command(p_person, PCmd_DROP_SPEC_ITEM);
              break;
        case PerSt_BEING_PERSUADED:
              person_being_persuaded(p_person);
              break;
        default:
              break;
        }
        calc_lighting(p_person);
        process_lighting_unkn1(p_person);
    }
}

/******************************************************************************/
