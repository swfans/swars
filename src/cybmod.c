/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file cybmod.c
 *     Agent mods related functions.
 * @par Purpose:
 *     Implement functions for handling cybernetic modifications.
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
#include "cybmod.h"

#include "bfmemory.h"
#include "bffile.h"
#include "bfini.h"
#include "swlog.h"
/******************************************************************************/
struct ModDef mod_defs[] = {
    {0, 0,   0, 0, {0},  0,    0,  0, 0},
    {3, 2,  30, 0, {0}, 16,  300, 10, 0},
    {5, 5, 120, 0, {0}, 17, 1200, 10, 0},
    {8, 8, 300, 0, {0}, 19, 3000, 10, 0},
    {2, 2,  25, 0, {0}, 20,  250, 10, 0},
    {4, 3, 100, 0, {0}, 24, 1000, 10, 0},
    {7, 6, 250, 0, {0}, 21, 2500, 10, 0},
    {3, 3,  50, 0, {0}, 15,  500, 10, 0},
    {5, 6, 200, 0, {0}, 23, 2000, 10, 0},
    {8, 8, 500, 0, {0}, 29, 5000, 10, 0},
    {1, 2,  45, 0, {0}, 33,  450, 10, 0},
    {2, 3, 180, 0, {0}, 30, 1800, 10, 0},
    {4, 6, 450, 0, {0}, 27, 4500, 10, 0},
    {1, 5, 200, 0, {0}, 25, 2000, 10, 0},
    {1, 4, 350, 0, {0}, 25, 3500, 10, 0},
    {1, 7, 600, 0, {0}, 28, 6000, 10, 0},
    {1, 2, 950, 0, {0}, 35, 9500, 10, 0},
};

struct TbNamedEnum mod_names[33] = {0};

enum CybModsConfigCmd {
    CCMod_ModsCount = 1,
    CCMod_Name,
    CCMod_PowerOutput,
    CCMod_Resilience,
    CCMod_Sprite,
    CCMod_Cost,
    CCMod_Funding,
    CCMod_PercentPerDay,
};

const struct TbNamedEnum cybmods_conf_common_cmds[] = {
  {"ModsCount",	CCMod_ModsCount},
  {NULL,		0},
};

const struct TbNamedEnum cybmods_conf_mod_cmds[] = {
  {"Name",			CCMod_Name},
  {"PowerOutput",	CCMod_PowerOutput},
  {"Resilience",	CCMod_Resilience},
  {"Sprite",		CCMod_Sprite},
  {"Cost",			CCMod_Cost},
  {"ResearchFunding",		CCMod_Funding},
  {"ResearchPercentPerDay",	CCMod_PercentPerDay},
  {NULL,		0},
};

void read_cybmods_conf_file(void)
{
    TbFileHandle conf_fh;
    TbBool done;
    unsigned int i;
    long k;
    int cmd_num;
    char *conf_buf;
    struct TbIniParser parser;
    char *conf_fname = "conf" FS_SEP_STR "cybmods.ini";
    int conf_len;
    int mods_count, mtype;

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
        LOGERR("Could not open cyb mods config file, going with defaults.");
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    mods_count = 0;
    // Parse the [common] section of loaded file
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return;
    }
    done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(cybmods_conf_common_cmds,cmd_num)
    while (!done)
    {
        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, cybmods_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case CCMod_ModsCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            mods_count = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), mods_count);
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
    for (mtype = 0; mtype < mods_count; mtype++)
    {
        char sect_name[16];
        struct ModDef *mdef;

        // Parse the [modN] sections of loaded file
        sprintf(sect_name, "mod%d", mtype);
        mdef = &mod_defs[mtype];
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
        done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(cybmods_conf_mod_cmds,cmd_num)
        while (!done)
        {
            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, cybmods_conf_mod_cmds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case CCMod_Name:
#if 0 // TODO add name to mod params
                i = LbIniValueGetStrWord(&parser, mdef->Name, sizeof(mdef->Name));
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)mdef->Name);
#endif
                break;
            case CCMod_PowerOutput:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                mdef->PowerOutput = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)mdef->PowerOutput);
                break;
            case CCMod_Resilience:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                mdef->Resilience = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)mdef->Resilience);
                break;
            case CCMod_Sprite:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                mdef->Sprite = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)mdef->Sprite);
                break;
            case CCMod_Cost:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                mdef->Cost = k / 10;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)mdef->Cost);
                break;
            case CCMod_Funding:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                mdef->Funding = k / 100;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)mdef->Funding);
                break;
            case CCMod_PercentPerDay:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                mdef->PercentPerDay = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)mdef->PercentPerDay);
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

/******************************************************************************/
