/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file hud_panecfg.c
 *     Ingame panel config loading from file.
 * @par Purpose:
 *     Implement functions for reading file with in-game panel parameters.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     21 Mar 2024 - 11 Jan 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "hud_panecfg.h"

#include "bffile.h"
#include "bfini.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfplanar.h"
#include "bfsprite.h"

#include "game.h"
#include "game_sprts.h"
#include "hud_panel.h"
#include "swlog.h"
/******************************************************************************/

enum PanelsCommonConfigCmd {
    PnComnCmd_PanelsCount = 1,
};

const struct TbNamedEnum panels_conf_common_cmnds[] = {
  {"PanelsCount",	PnComnCmd_PanelsCount},
  {NULL,			0},
};

enum PanelsPanelConfigCmd {
    PnPanelCmd_ScreenPos = 1,
    PnPanelCmd_ScreenSize,
    PnPanelCmd_DynaPos,
    PnPanelCmd_DynaSize,
    PnPanelCmd_Sprite,
    PnPanelCmd_ExtraSpr,
    PnPanelCmd_Use,
    PnPanelCmd_Flags,
    PnPanelCmd_Ident,
    PnPanelCmd_Type,
};

const struct TbNamedEnum panels_conf_panel_cmnds[] = {
  {"ScreenPos",		PnPanelCmd_ScreenPos},
  {"ScreenSize",	PnPanelCmd_ScreenSize},
  {"DynaPos",		PnPanelCmd_DynaPos},
  {"DynaSize",		PnPanelCmd_DynaSize},
  {"Sprite",		PnPanelCmd_Sprite},
  {"ExtraSpr",		PnPanelCmd_ExtraSpr},
  {"Use",			PnPanelCmd_Use},
  {"Flags",			PnPanelCmd_Flags},
  {"Ident",			PnPanelCmd_Ident},
  {"Type",			PnPanelCmd_Type},
  {NULL,			0},
};

enum PanelsShiftsConfigCmd {
    PnShiftCmd_AgentWeaponToList = 1,
    PnShiftCmd_GroupPaneToThermalBox,
    PnShiftCmd_GroupPaneToThermalSpr,
    PnShiftCmd_GroupPaneAgents2,
    PnShiftCmd_GroupPaneAgents3,
    PnShiftCmd_GroupPaneAgents4,
    PnShiftCmd_WepCurrBtnToSymbol,
    PnShiftCmd_WepFrstBtnToSymbol,
    PnShiftCmd_WepNextBtnToSymbol,
    PnShiftCmd_WepCurrBtnToDecor,
    PnShiftCmd_WepFrstBtnToDecor,
    PnShiftCmd_WepNextBtnToDecor,
    PnShiftCmd_WepNextDistance,
    PnShiftCmd_WepCurrButtonArea,
    PnShiftCmd_WepNextButtonArea,
};

const struct TbNamedEnum panels_conf_shifts_cmnds[] = {
  {"AgentWeaponToList",		PaSh_AGENT_WEAPON_TO_LIST + 1},
  {"GroupPaneToThermalBox",	PaSh_GROUP_PANE_TO_THERMAL_BOX + 1},
  {"GroupPaneToThermalSpr",	PaSh_GROUP_PANE_TO_THERMAL_SPR + 1},
  {"GroupPaneAgents2",		PaSh_GROUP_PANE_AGENTS + 1},
  {"GroupPaneAgents3",		PaSh_GROUP_PANE_AGENTS + 2},
  {"GroupPaneAgents4",		PaSh_GROUP_PANE_AGENTS + 3},
  {"WepCurrBtnToSymbol",	PaSh_WEP_CURR_BTN_TO_SYMBOL + 1},
  {"WepFrstBtnToSymbol",	PaSh_WEP_FRST_BTN_TO_SYMBOL + 1},
  {"WepNextBtnToSymbol",	PaSh_WEP_NEXT_BTN_TO_SYMBOL + 1},
  {"WepCurrBtnToDecor",		PaSh_WEP_CURR_BTN_TO_DECOR + 1},
  {"WepFrstBtnToDecor",		PaSh_WEP_FRST_BTN_TO_DECOR + 1},
  {"WepNextBtnToDecor",		PaSh_WEP_NEXT_BTN_TO_DECOR + 1},
  {"WepNextDistance",		PaSh_WEP_NEXT_DISTANCE + 1},
  {"WepCurrButtonArea",		PaSh_WEP_CURR_BUTTON_AREA + 1},
  {"WepNextButtonArea",		PaSh_WEP_NEXT_BUTTON_AREA + 1},
};

const struct TbNamedEnum panels_conf_panel_type[] = {
  {"Default",		PanT_NONE + 1},
  {"AgentBadge",	PanT_AgentBadge + 1},
  {"AgentMood",		PanT_AgentMood + 1},
  {"AgentHealth",	PanT_AgentHealth + 1},
  {"AgentEnergy",	PanT_AgentEnergy + 1},
  {"AgentWeapon",	PanT_AgentWeapon + 1},
  {"AgentMedi",		PanT_AgentMedi + 1},
  {"UNKN07",		PanT_UNKN07 + 1},
  {"WeaponEnergy",	PanT_WeaponEnergy + 1},
  {"UNKN09",		PanT_UNKN09 + 1},
  {"Grouping",		PanT_Grouping + 1},
  {NULL,			0},
};

const struct TbNamedEnum panels_conf_panel_flags[] = {
  {"Enabled",				PanF_ENABLED},
  {"SpritesInLineHoriz",	PanF_SPRITES_IN_LINE_HORIZ},
  {"SpritesInLineVertc",	PanF_SPRITES_IN_LINE_VERTC},
  {NULL,					0},
};

const struct TbNamedEnum panels_conf_any_bool[] = {
  {"True",			1},
  {"False",			2},
  {NULL,			0},
};

struct GamePanel game_panel_custom[GAME_PANELS_LIMIT];
struct TbPoint game_panel_custom_shifts[48];

void update_panel_derivative_shifts(short detail)
{
    struct TbSprite *p_spr;
    struct TbPoint *p_base_shift;
    struct TbPoint *p_slot_size;
    int i;

    p_base_shift = &game_panel_custom_shifts[PaSh_WEP_FOURPACK_BASE_SH];
    p_slot_size = &game_panel_custom_shifts[PaSh_WEP_FOURPACK_SIZE];
    p_spr = &pop1_sprites[14];
    // We're expecting to use 4 ammo slots; 8 are supported mostly to signal an issue
    for (i = 0; i < 8; i++)
    {
        struct TbPoint *p_shift;
        int dx, dy;

        if ((i & 3) < 2)
        {
            dx = p_base_shift->x;
            dx += (p_base_shift->x + p_slot_size->x) * (i / 4);
        }
        else
        {
            dx = p_spr->SWidth - (p_base_shift->x + p_slot_size->x);
            dx -= (p_base_shift->x + p_slot_size->x) * (i / 4);
        }

        if ((i & 3) == 0 || (i & 3) == 3)
        {
            dy = p_base_shift->y;
        }
        else
        {
            dy = p_spr->SHeight - (p_base_shift->y + p_slot_size->y);
        }

        p_shift = &game_panel_custom_shifts[PaSh_WEP_FOURPACK_SLOTS + i];
        p_shift->x = dx;
        p_shift->y = dy;
    }
}

void size_panels_for_detail(short detail)
{
    const char *name;
    short styleno;

    if (ingame.PanelPermutation >= 0) {
        name = "panel";
    } else {
        name = "pop";
    }
    // Currently we use the same config for all styles
    styleno = 0;
    read_panel_config(name, styleno, detail);
    update_panel_derivative_shifts(detail);
    game_panel = game_panel_custom;
    game_panel_shifts = game_panel_custom_shifts;
}

TbBool read_panel_config(const char *name, ushort styleno, ushort detail)
{
    TbFileHandle conf_fh;
    TbBool done;
    int i, n;
    long k, m;
    char *conf_buf;
    struct TbIniParser parser;
    char conf_fname[80];
    int conf_len;
    short pop_panel_count, panel;

    sprintf(conf_fname, "%s" FS_SEP_STR "%s%hu-%hu.ini", "conf", name, styleno, detail);
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
        LOGERR("Could not open '%s' file, previous panel config still active.", conf_fname);
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    pop_panel_count = 0;

    // Parse the [common] section of loaded file
    done = false;
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section.", "common");
        done = true;
    }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(panels_conf_common_cmnds,cmd_num)
    while (!done)
    {
        int cmd_num;

        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, panels_conf_common_cmnds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case PnComnCmd_PanelsCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            pop_panel_count = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)pop_panel_count);
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

    // Parse the [shifts] section of loaded file
    if (conf_len > 0) {
        LbMemorySet(game_panel_custom_shifts, 0, sizeof(game_panel_custom_shifts));
    }
    done = false;
    if (LbIniFindSection(&parser, "shifts") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section.", "shifts");
        done = true;
    }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(panels_conf_shifts_cmnds,cmd_num)
    while (!done)
    {
        struct TbPoint *p_shift;
        int cmd_num;

        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, panels_conf_shifts_cmnds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case PaSh_AGENT_WEAPON_TO_LIST + 1:
        case PaSh_GROUP_PANE_TO_THERMAL_BOX + 1:
        case PaSh_GROUP_PANE_TO_THERMAL_SPR + 1:
        case PaSh_GROUP_PANE_AGENTS + 1:
        case PaSh_GROUP_PANE_AGENTS + 2:
        case PaSh_GROUP_PANE_AGENTS + 3:
        case PaSh_WEP_CURR_BTN_TO_SYMBOL + 1:
        case PaSh_WEP_FRST_BTN_TO_SYMBOL + 1:
        case PaSh_WEP_NEXT_BTN_TO_SYMBOL + 1:
        case PaSh_WEP_CURR_BTN_TO_DECOR + 1:
        case PaSh_WEP_FRST_BTN_TO_DECOR + 1:
        case PaSh_WEP_NEXT_BTN_TO_DECOR + 1:
        case PaSh_WEP_NEXT_DISTANCE + 1:
        case PaSh_WEP_CURR_BUTTON_AREA + 1:
        case PaSh_WEP_NEXT_BUTTON_AREA + 1:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command 1st parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            i = LbIniValueGetLongInt(&parser, &m);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command 2nd parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_shift = &game_panel_custom_shifts[cmd_num - 1];
            p_shift->x = k;
            p_shift->y = m;
            CONFDBGLOG("%s (%d,%d)", COMMAND_TEXT(cmd_num), (int)p_shift->x, (int)p_shift->y);
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
    { // TODO out to config file
    struct TbPoint *p_slot;

    p_slot = &game_panel_custom_shifts[PaSh_WEP_FOURPACK_BASE_SH];
    p_slot->x = 1 * (detail + 1);
    p_slot->y = 0;
    p_slot = &game_panel_custom_shifts[PaSh_WEP_FOURPACK_SIZE];
    p_slot->x = 2 * (detail + 1);
    p_slot->y = 2 * (detail + 1);
    }

    for (panel = 0; panel < pop_panel_count; panel++)
    {
        char sect_name[16];
        struct GamePanel *p_panel;

        // Parse the [panelN] sections of loaded file
        sprintf(sect_name, "panel%hd", panel);
        p_panel = &game_panel_custom[panel];
        LbMemorySet(p_panel, 0, sizeof(struct GamePanel));
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
        done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(panels_conf_panel_cmnds,cmd_num)
        while (!done)
        {
            int cmd_num;

            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, panels_conf_panel_cmnds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case PnPanelCmd_ScreenPos:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 1st parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                i = LbIniValueGetLongInt(&parser, &m);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 2nd parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->pos.X = k;
                p_panel->pos.Y = m;
                CONFDBGLOG("%s (%d,%d)", COMMAND_TEXT(cmd_num), (int)p_panel->pos.X, (int)p_panel->pos.Y);
                break;
            case PnPanelCmd_ScreenSize:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 1st parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                i = LbIniValueGetLongInt(&parser, &m);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 2nd parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->pos.Width = k;
                p_panel->pos.Height = m;
                CONFDBGLOG("%s (%d,%d)", COMMAND_TEXT(cmd_num), (int)p_panel->pos.Width, (int)p_panel->pos.Height);
                break;
            case PnPanelCmd_DynaPos:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 1st parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                i = LbIniValueGetLongInt(&parser, &m);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 2nd parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->dyn.X = k;
                p_panel->dyn.Y = m;
                CONFDBGLOG("%s (%d,%d)", COMMAND_TEXT(cmd_num), (int)p_panel->dyn.X, (int)p_panel->dyn.Y);
                break;
            case PnPanelCmd_DynaSize:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 1st parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                i = LbIniValueGetLongInt(&parser, &m);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command 2nd parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->dyn.Width = k;
                p_panel->dyn.Height = m;
                CONFDBGLOG("%s (%d,%d)", COMMAND_TEXT(cmd_num), (int)p_panel->dyn.Width, (int)p_panel->dyn.Height);
                break;
            case PnPanelCmd_Sprite:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->Spr = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_panel->Spr);
                break;
            case PnPanelCmd_ExtraSpr:
                for (n = 0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                        break;
                    }
                    p_panel->ExtraSpr[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_panel->ExtraSpr[0],
                  (int)p_panel->ExtraSpr[1], (int)p_panel->ExtraSpr[2]);
                break;
            case PnPanelCmd_Use:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->Use = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_panel->Use);
                break;
            case PnPanelCmd_Flags:
                k = 0;
                while (1)
                {
                    i = LbIniValueGetNamedEnum(&parser, panels_conf_panel_flags);
                    if (i <= 0) {
                        if (i == 0)
                            break;
                        CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                        break;
                    }
                    k |= i;
                }
                p_panel->Flags = k;
                CONFDBGLOG("%s 0x%x", COMMAND_TEXT(cmd_num), (int)p_panel->Flags);
                break;
            case PnPanelCmd_Ident:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->ID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_panel->ID);
                break;
            case PnPanelCmd_Type:
                i = LbIniValueGetNamedEnum(&parser, panels_conf_panel_type);
                if (i <= 0) {
                    CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_panel->Type = i - 1;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_panel->Type);
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
    // Fill unused panels
    if (conf_len > 0) {
        pop_panel_count = sizeof(game_panel_custom)/sizeof(game_panel_custom[0]);
    }
    for (; panel < pop_panel_count; panel++)
    {
        struct GamePanel *p_panel;

        p_panel = &game_panel_custom[panel];
        LbMemorySet(p_panel, 0, sizeof(struct GamePanel));
        p_panel->pos.X = -1;
        p_panel->pos.Y = -1;
        p_panel->Spr = -1;
    }

#undef CONFDBGLOG
#undef CONFWRNLOG
    LbIniParseEnd(&parser);
    LbMemoryFree(conf_buf);
    return true;
}
/******************************************************************************/
