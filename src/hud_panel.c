/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file hud_panel.c
 *     Ingame Heads-Up Display panel draw.
 * @par Purpose:
 *     Implement functions drawing the HUD panel.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "hud_panel.h"

#include <assert.h>
#include "bfbox.h"
#include "bfgentab.h"
#include "bfline.h"
#include "bfmemut.h"
#include "bfplanar.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfutility.h"
#include "ssampply.h"

#include "bflib_render_drspr.h"
#include "bigmap.h"
#include "display.h"
#include "engintrns.h"
#include "game_data.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "guiboxes.h"
#include "keyboard.h"
#include "lvobjctv.h"
#include "mouse.h"
#include "mydraw.h"
#include "packet.h"
#include "player.h"
#include "scandraw.h"
#include "sound.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/
extern ubyte byte_1CAB64[];
extern ubyte byte_1DB088[];
extern long dword_1DC36C;

/** Over which agent weapon the cursor is currently placed.
 *
 * Used just for the graphical detail of highliting current weapon when mouse over.
 */
sbyte agent_with_mouse_over_weapon = -1;

short gui_scale = 0;

enum PanelType {
    PanT_NONE = 0,
    PanT_AgentBadge,
    PanT_AgentMood,
    PanT_UNKN03,
    PanT_UNKN04,
    PanT_AgentWeapon,
    PanT_AgentMedi,
    PanT_UNKN07,
    PanT_UNKN08,
    PanT_UNKN09,
    PanT_UNKN10,
};

enum PanelShift {
    PaSh_AGENT_PANEL_TO_NUMBER = 0,
    PaSh_AGENT_WEAPON_TO_LIST = 4,
    PaSh_GROUP_PANE_TO_THERMAL_BOX = 5,
    PaSh_GROUP_PANE_TO_THERMAL_SPR = 6,
    PaSh_GROUP_PANE_AGENTS = 7,
    PaSh_WEP_CURR_BTN_TO_SYMBOL = 10,
    PaSh_WEP_FRST_BTN_TO_SYMBOL = 11,
    PaSh_WEP_NEXT_BTN_TO_SYMBOL = 12,
    PaSh_WEP_CURR_BTN_TO_DECOR =  13,
    PaSh_WEP_FRST_BTN_TO_DECOR = 14,
    PaSh_WEP_NEXT_BTN_TO_DECOR = 15,
    PaSh_WEP_NEXT_DISTANCE = 16,
    PaSh_WEP_CURR_BUTTON_AREA = 17,
    PaSh_WEP_NEXT_BUTTON_AREA = 18,
};

struct GamePanel game_panel_hi[] = {
    // Per-agent main panel frame
    {  0,  0,  1,150, 32, 1, 1, 0, PanT_AgentBadge},
    {145,  0,  3,162, 28, 2, 1, 1, PanT_AgentBadge},
    {302,  0,  4,162, 28, 3, 1, 2, PanT_AgentBadge},
    {459,  0,  5,162, 28, 4, 1, 3, PanT_AgentBadge},
    // Per-agent mood bar
    { 45, 10,  0, 88, 10, 1, 1, 0, PanT_AgentMood},
    {202, 10,  0, 88, 10, 2, 1, 1, PanT_AgentMood},
    {359, 10,  0, 88, 10, 3, 1, 2, PanT_AgentMood},
    {516, 10,  0, 88, 10, 4, 1, 3, PanT_AgentMood},
    // Per-agent medikit button
    { 30, 25, 95,  0,  0, 1, 1, 0, PanT_AgentMedi},
    {187, 25, 95,  0,  0, 2, 1, 1, PanT_AgentMedi},
    {344, 25, 95,  0,  0, 3, 1, 2, PanT_AgentMedi},
    {501, 25, 95,  0,  0, 4, 1, 3, PanT_AgentMedi},
    // Per-agent current weapon frame
    { 57, 25, 12,  0,  0, 1, 1, 0, PanT_AgentWeapon},
    {214, 25, 12,  0,  0, 2, 1, 1, PanT_AgentWeapon},
    {371, 25, 12,  0,  0, 3, 1, 2, PanT_AgentWeapon},
    {528, 25, 12,  0,  0, 4, 1, 3, PanT_AgentWeapon},
    // Left-side indicators
    {  0, 37, 10,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0,191,105,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0,267, 11,  0,  0, 4, 1, 0, PanT_UNKN10},
    { -1, -1, -1,  0,  0, 4, 1, 0, PanT_NONE},
};

struct GamePanel game_panel_lo[] = {
    // Per-agent main panel frame
    {  0,  0,  1, 75, 16, 1, 1, 0, PanT_AgentBadge},
    { 73,  0,  3, 81, 14, 2, 1, 1, PanT_AgentBadge},
    {152,  0,  4, 81, 14, 3, 1, 2, PanT_AgentBadge},
    {231,  0,  5, 81, 14, 4, 1, 3, PanT_AgentBadge},
    // Per-agent mood bar
    { 22,  5,  0, 44,  5, 1, 1, 0, PanT_AgentMood},
    {101,  5,  0, 44,  5, 2, 1, 1, PanT_AgentMood},
    {180,  5,  0, 44,  5, 3, 1, 2, PanT_AgentMood},
    {259,  5,  0, 44,  5, 4, 1, 3, PanT_AgentMood},
    // Per-agent medikit button
    { 15, 12, 95,  0,  0, 1, 1, 0, PanT_AgentMedi},
    { 94, 12, 95,  0,  0, 2, 1, 1, PanT_AgentMedi},
    {173, 12, 95,  0,  0, 3, 1, 2, PanT_AgentMedi},
    {252, 12, 95,  0,  0, 4, 1, 3, PanT_AgentMedi},
    // Per-agent current weapon frame
    { 29, 12, 12,  0,  0, 1, 1, 0, PanT_AgentWeapon},
    {108, 12, 12,  0,  0, 2, 1, 1, PanT_AgentWeapon},
    {187, 12, 12,  0,  0, 3, 1, 2, PanT_AgentWeapon},
    {266, 12, 12,  0,  0, 4, 1, 3, PanT_AgentWeapon},
    // Left-side indicators
    {  0, 19, 10,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0, 89, 11,  0,  0, 4, 1, 0, PanT_UNKN10},
    { -1, -1, -1,  0,  0, 4, 1, 0, PanT_NONE},
};

struct GamePanel game_panel_prealp_hi[] = {
    // Per-agent main panel frame
    {  0,  0,  1,150, 32, 1, 1, 0, PanT_AgentBadge},
    {145,  0,  3,162, 28, 2, 1, 1, PanT_AgentBadge},
    {302,  0,  4,162, 28, 3, 1, 2, PanT_AgentBadge},
    {459,  0,  5,162, 28, 4, 1, 3, PanT_AgentBadge},
    // Per-agent mood bar
    { 44, 12,  0, 88, 10, 1, 1, 0, PanT_AgentMood},
    {202, 12,  0, 88, 10, 2, 1, 1, PanT_AgentMood},
    {360, 12,  0, 88, 10, 3, 1, 2, PanT_AgentMood},
    {518, 12,  0, 88, 10, 4, 1, 3, PanT_AgentMood},
    // Per-agent medikit button
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    // Per-agent current weapon frame
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    // Left-side indicators
    {  0, 37, 10,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0,191,105,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0,267, 11,  0,  0, 4, 1, 0, PanT_UNKN10},
    { 22, 36,  0,  0,  0, 4, 1, 0, PanT_AgentWeapon},
    { -1, -1, -1,  0,  0, 4, 1, 0, PanT_NONE},
};

struct GamePanel game_panel_prealp_lo[] = {
    // Per-agent main panel frame
    {  0,  0,  1, 75, 16, 1, 1, 0, PanT_AgentBadge},
    { 72,  0,  3, 81, 14, 2, 1, 1, PanT_AgentBadge},
    {151,  0,  4, 81, 14, 3, 1, 2, PanT_AgentBadge},
    {229,  0,  5, 81, 14, 4, 1, 3, PanT_AgentBadge},
    // Per-agent mood bar
    { 22,  6,  0, 44,  5, 1, 1, 0, PanT_AgentMood},
    {101,  6,  0, 44,  5, 2, 1, 1, PanT_AgentMood},
    {180,  6,  0, 44,  5, 3, 1, 2, PanT_AgentMood},
    {259,  6,  0, 44,  5, 4, 1, 3, PanT_AgentMood},
    // Per-agent medikit button
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    // Per-agent current weapon frame
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    {  0,  0,  0,  0,  0, 4, 1, 0, PanT_NONE},
    // Left-side indicators
    {  0, 18, 10,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0, 95,105,  0,  0, 4, 1, 0, PanT_UNKN08},
    {  0,133, 11,  0,  0, 4, 1, 0, PanT_UNKN10},
    { 11, 18,  0,  0,  0, 4, 1, 0, PanT_AgentWeapon},
    { -1, -1, -1,  0,  0, 4, 1, 0, PanT_NONE},
};

struct TbPoint game_panel_hi_shifts[] = {
    // PaSh_AGENT_PANEL_TO_NUMBER
    { 4, 2},
    { 8, 2},
    { 4, 2},
    {10, 2},
    // PaSh_AGENT_WEAPON_TO_LIST
    { 1,28},
    // PaSh_GROUP_PANE_TO_THERMAL_BOX
    { 0,48},
    // PaSh_GROUP_PANE_TO_THERMAL_SPR
    { 4,60},
    // PaSh_GROUP_PANE_AGENTS
    { 4, 36},
    { 4, 24},
    { 4, 12},
    // PaSh_WEP_CURR_BTN_TO_SYMBOL
    { 8, 4},
    // PaSh_WEP_FRST_BTN_TO_SYMBOL
    { 8, 4},
    // PaSh_WEP_NEXT_BTN_TO_SYMBOL
    { 8, 4},
    // PaSh_WEP_CURR_BTN_TO_DECOR
    { 0, 0},
    // PaSh_WEP_FRST_BTN_TO_DECOR
    {-16,-8},
    // PaSh_WEP_NEXT_BTN_TO_DECOR
    { 0, -4},
    // PaSh_WEP_NEXT_DISTANCE
    { 0, 28},
    // PaSh_WEP_CURR_BUTTON_AREA
    {76, 26},
    // PaSh_WEP_NEXT_BUTTON_AREA
    {74, 26},
};

struct TbPoint game_panel_lo_shifts[] = {
    // PaSh_AGENT_PANEL_TO_NUMBER
    { 2, 1},
    { 4, 1},
    { 2, 1},
    { 5, 1},
    // PaSh_AGENT_WEAPON_TO_LIST
    { 0,14},
    // PaSh_GROUP_PANE_TO_THERMAL_BOX
    { 0,22},
    // PaSh_GROUP_PANE_TO_THERMAL_SPR
    { 2,30},
    // PaSh_GROUP_PANE_AGENTS
    { 2, 18},
    { 2, 12},
    { 2,  6},
    // PaSh_WEP_CURR_BTN_TO_SYMBOL
    { 4, 2},
    // PaSh_WEP_FRST_BTN_TO_SYMBOL
    { 4, 2},
    // PaSh_WEP_NEXT_BTN_TO_SYMBOL
    { 4, 2},
    // PaSh_WEP_CURR_BTN_TO_DECOR
    { 0, 0},
    // PaSh_WEP_FRST_BTN_TO_DECOR
    { -8,-4},
    // PaSh_WEP_NEXT_BTN_TO_DECOR
    { 0,-2},
    // PaSh_WEP_NEXT_DISTANCE
    { 0, 14},
    // PaSh_WEP_CURR_BUTTON_AREA
    {38, 13},
    // PaSh_WEP_NEXT_BUTTON_AREA
    {37, 13},
};

struct TbPoint game_panel_prealp_hi_shifts[] = {
    // PaSh_AGENT_PANEL_TO_NUMBER
    { 8, 4},
    { 8, 4},
    { 4, 4},
    {12, 4},
    // PaSh_AGENT_WEAPON_TO_LIST
    { 1,28},
    // PaSh_GROUP_PANE_TO_THERMAL_BOX
    { 0,48},
    // PaSh_GROUP_PANE_TO_THERMAL_SPR
    { 4,60},
    // PaSh_GROUP_PANE_AGENTS
    { 4, 36},
    { 4, 24},
    { 4, 12},
    // PaSh_WEP_CURR_BTN_TO_SYMBOL
    { 8, 4},
    // PaSh_WEP_FRST_BTN_TO_SYMBOL
    { 8, 4},
    // PaSh_WEP_NEXT_BTN_TO_SYMBOL
    { 8, 4},
    // PaSh_WEP_CURR_BTN_TO_DECOR
    { 0, 0},
    // PaSh_WEP_FRST_BTN_TO_DECOR
    {-16,-8},
    // PaSh_WEP_NEXT_BTN_TO_DECOR
    { 0,-4},
    // PaSh_WEP_NEXT_DISTANCE
    { 0, 28},
    // PaSh_WEP_CURR_BUTTON_AREA
    {74, 26},
    // PaSh_WEP_NEXT_BUTTON_AREA
    {74, 26},
};

struct TbPoint game_panel_prealp_lo_shifts[] = {
    // PaSh_AGENT_PANEL_TO_NUMBER
    { 4, 1},
    { 4, 1},
    { 3, 1},
    { 6, 1},
    // PaSh_AGENT_WEAPON_TO_LIST
    { 0,14},
    // PaSh_GROUP_PANE_TO_THERMAL_BOX
    { 0,22},
    // PaSh_GROUP_PANE_TO_THERMAL_SPR
    { 2,30},
    // PaSh_GROUP_PANE_AGENTS
    { 2, 18},
    { 2, 12},
    { 2,  6},
    // PaSh_WEP_CURR_BTN_TO_SYMBOL
    { 4, 2},
    // PaSh_WEP_FRST_BTN_TO_SYMBOL
    { 4, 2},
    // PaSh_WEP_NEXT_BTN_TO_SYMBOL
    { 4, 2},
    // PaSh_WEP_CURR_BTN_TO_DECOR
    { 0, 0},
    // PaSh_WEP_FRST_BTN_TO_DECOR
    {-8,-4},
    // PaSh_WEP_NEXT_BTN_TO_DECOR
    { 0,-2},
    // PaSh_WEP_NEXT_DISTANCE
    { 0, 14},
    // PaSh_WEP_CURR_BUTTON_AREA
    {37, 13},
    // PaSh_WEP_NEXT_BUTTON_AREA
    {37, 13},
};

struct TbPoint *game_panel_shifts;

TbResult prep_pop_sprites(short detail)
{
    PathInfo *pinfo;
    short colorno;
    TbResult ret;

    pinfo = &game_dirs[DirPlace_Data];
    if (ingame.PanelPermutation >= 0)
    {
        colorno = ingame.PanelPermutation;
        ret = load_prealp_pop_sprites(pinfo->directory, colorno, detail);
    }
    else
    {
        colorno = -ingame.PanelPermutation - 1;
        ret = load_pop_sprites(pinfo->directory, colorno, detail);
    }
    gui_scale = detail;
    setup_pop_sprites();
    if (ret == Lb_FAIL) {
        LOGERR("Some files were not loaded successfully");
    }
    return ret;
}

void size_panels_for_detail(short detail)
{

    if (detail == 0)
    {
        if (ingame.PanelPermutation >= 0)
        {
            game_panel = game_panel_prealp_lo;
            game_panel_shifts = game_panel_prealp_lo_shifts;
        }
        else
        {
            game_panel = game_panel_lo;
            game_panel_shifts = game_panel_lo_shifts;
        }
    }
    else
    {
        if (ingame.PanelPermutation >= 0)
        {
            game_panel = game_panel_prealp_hi;
            game_panel_shifts = game_panel_prealp_hi_shifts;
        }
        else
        {
            game_panel = game_panel_hi;
            game_panel_shifts = game_panel_hi_shifts;
        }
    }
}

void load_pop_sprites_up_to(short max_detail)
{
    short detail;
    for (detail = max_detail; detail >= 0; detail--)
    {
        TbResult ret;

        ret = prep_pop_sprites(detail);
        if (ret != Lb_FAIL)
            break;
    }
    if (detail < 0)
        detail = 0;
    size_panels_for_detail(detail);
}

void load_pop_sprites_for_current_mode(void)
{
    short i, max_detail;

    max_detail = 0;
    for (i = 0; i <= MAX_SUPPORTED_SCREEN_HEIGHT/180; i++) {
        if ((320 * i > lbDisplay.GraphicsScreenWidth) || (180 * i > lbDisplay.GraphicsScreenHeight))
            break;
        max_detail = i;
    }
    load_pop_sprites_up_to(max_detail);
}

//TODO not the best location for agent state update
void update_dropped_item_under_agent_exists(short agent)
{
    struct SimpleThing *p_pickup;
    struct Thing *p_agent;
    ThingIdx thing;

    p_agent = &things[agent];

    if ((p_agent->Flag & TngF_Unkn08000000) == 0)
        return;

    thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
    if (thing < 0)
        p_pickup = &sthings[thing];
    else
        p_pickup = NULL;
    if ((p_pickup == NULL) || (p_pickup->Type != SmTT_DROPPED_ITEM)) {
        p_agent->Flag &= ~TngF_Unkn08000000;
    }
}

int SCANNER_objective_info_height(void)
{
    int h;

    if (lbDisplay.GraphicsScreenHeight < 400)
        return 9;
    h = 18 * lbDisplay.GraphicsScreenHeight / 400;

    h -= (h % 9);

    return h;
}

/* draws a sprite scaled to double size; remove pending */
void SCANNER_unkn_func_200(struct TbSprite *spr, int x, int y, ubyte col)
{
    int xwind_beg;
    int xwind_end;
    int xwind_start;
    sbyte *inp;
    ubyte *oline;
    int opitch;
    int h;
    TbBool needs_window_bounding;

    xwind_beg = lbDisplay.GraphicsWindowX;
    xwind_end = lbDisplay.GraphicsWindowX + lbDisplay.GraphicsWindowWidth;
    xwind_start = lbDisplay.GraphicsWindowX + x;
    inp = (sbyte *)spr->Data;
    opitch = lbDisplay.GraphicsScreenWidth;
    oline = &lbDisplay.WScreen[opitch * (lbDisplay.GraphicsWindowY + y) + lbDisplay.GraphicsWindowX + x];
    if (xwind_start < lbDisplay.GraphicsWindowX) {
        if (xwind_start + 2 * spr->SWidth <= lbDisplay.GraphicsWindowX)
            return;
        needs_window_bounding = true;
    } else {
        if (xwind_start >= xwind_end)
            return;
        needs_window_bounding = (xwind_start + 2 * spr->SWidth > xwind_end);
    }

    if (!needs_window_bounding)
    {
        // Simplified and faster drawing when we do not have to check bounds
        for (h = 0; h < spr->SHeight; h++)
        {
            ubyte *o;

            o = oline;
            while (*inp)
            {
                int ival;
                int i;

                ival = *inp;
                if (ival < 0)
                {
                    inp++;
                    o -= 2 * ival;
                    continue;
                }
                inp += ival + 1;
                for (i = 0; i < ival; i++)
                {
                    o[0] = col;
                    o[opitch + 0] = col;
                    o[1] = col;
                    o[opitch + 1] = col;
                    o += 2;
                }
            }
            inp++;
            oline += 2 * opitch;
        }
    }
    else
    {
        for (h = 0; h < spr->SHeight; h++)
        {
            ubyte *o;
            int xwind_curr;

            o = oline;
            xwind_curr = xwind_start;
            while (*inp)
            {
                int ival;
                int i;

                ival = *inp;
                if (ival < 0)
                {
                    inp++;
                    o -= 2 * ival;
                    xwind_curr -= 2 * ival;
                    continue;
                }
                inp += ival + 1;
                for (i = 0; i < ival; i++)
                {
                    if (xwind_curr >= xwind_beg && xwind_curr < xwind_end) {
                        o[0] = col;
                        o[opitch] = col;
                    }
                    xwind_curr++;
                    o++;
                    if (xwind_curr >= xwind_beg && xwind_curr < xwind_end) {
                        o[0] = col;
                        o[opitch] = col;
                    }
                    xwind_curr++;
                    o++;
                }
            }
            inp++;
            oline += 2 * opitch;
        }
    }
}


void SCANNER_unkn_func_201(struct TbSprite *spr, int x, int y, ubyte *fade)
{
    ubyte *oline;
    ubyte *dt;
    int ich;
    ubyte *o;

    oline = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * y + x];
    dt = spr->Data;
    for (ich = spr->SHeight; ich > 0; ich--)
    {
        o = oline;
        while (1)
        {
            ushort ftidx;
            sbyte len;

            len = *dt;
            if (!len)
                break;
            if (len > 0)
            {
                ++dt;
                while (len)
                {
                    ftidx = *dt;
                    *o = fade[ftidx];
                    ++dt;
                    ++o;
                    len--;
                }
            }
            else
            {
                len = -len;
                o += len;
                ++dt;
            }
        }
        ++dt;
        oline += lbDisplay.GraphicsScreenWidth;
    }
}

void SCANNER_unkn_func_202(struct TbSprite *spr, int x, int y, int ctr, int bri)
{
    ubyte *oline;
    ubyte *dt;
    int ich;
    ubyte *o;

    if ((x < 0) || (x > lbDisplay.PhysicalScreenWidth))
        return;
    if ((y < 0) || (y > lbDisplay.PhysicalScreenHeight))
        return;
    if ((x + spr->SWidth < 0) || (x + spr->SWidth > lbDisplay.PhysicalScreenWidth))
        return;
    if ((y + spr->SHeight < 0) || (y + spr->SHeight > lbDisplay.PhysicalScreenHeight))
        return;

    oline = &lbDisplay.WScreen[y * lbDisplay.GraphicsScreenWidth + x];
    dword_1DC36C = bri;
    dt = spr->Data;
    for (ich = spr->SHeight; ich > 0; ich--)
    {
        o = oline;
        while (1)
        {
            ushort ftsub, ftidx;
            sbyte len;

            len = *dt;
            if (!len)
                break;
            if (len > 0)
            {
                ++dt;
                while (len)
                {
                    ftidx = *dt++;
                    ftsub = dword_1DC36C + ((byte_1CAB64[ftidx] >> 1) + (byte_1CAB64[*o] >> 1));
                    ftidx |= byte_1DB088[ftsub] << 8;
                    *o++ = pixmap.fade_table[ftidx];
                    len--;
                }
            }
            else
            {
                len = -len;
                o += len;
                ++dt;
            }
        }
        ++dt;
        oline += lbDisplay.GraphicsScreenWidth;
    }
}

void SCANNER_unkn_func_203(int a1, int a2, int a3, int a4, ubyte a5, int a6, int a7)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_SCANNER_unkn_func_203\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6), "g" (a7));
}

int SCANNER_text_draw(const char *text, int start_x, int height)
{
    const ubyte *str;
    struct TbSprite *p_spr;
    int x;
    ubyte sel_c1;
    ubyte ch;
    TbPixel col;

    str = (const ubyte *)text;
    sel_c1 = SCANNER_colour[0];
    x = start_x;
    if (lbDisplay.GraphicsScreenHeight >= 400)
    {
      int chr_width, chr_height;

      while (*str != '\0')
      {
        if (*str == '\1') {
          str++;
          sel_c1 = *str;
        } else {
          ch = my_char_to_upper(*str);
          col = pixmap.fade_table[56 * PALETTE_8b_COLORS + sel_c1];
          p_spr = &small_font[ch - 31];
          chr_width = p_spr->SWidth * height / 9;
          chr_height = p_spr->SHeight * height / 9;
          LbSpriteDrawScaledOneColour(x, 2, p_spr, chr_width, chr_height, col);
          x += chr_width;
        }
        str++;
      }
    }
    else
    {
      while (*str != '\0')
      {
        if (*str == '\1') {
          str++;
          sel_c1 = *str;
        } else {
          ch = my_char_to_upper(*str);
          col = pixmap.fade_table[56 * PALETTE_8b_COLORS + sel_c1];
          p_spr = &small_font[ch - 31];
          LbSpriteDrawOneColour(x, 1, p_spr, col);
          x += p_spr->SWidth;
        }
        str++;
      }
    }
    return x;
}

short panel_state_to_player_agent(ushort panstate)
{
    if ((panstate >= PANEL_STATE_WEP_SEL_ONE) && (panstate <= PANEL_STATE_MOOD_SET_GRP + 3))
        return (panstate - 1) & 3;
    return -1;
}

void SCANNER_move_objective_info(int width, int height, int end_pos)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    if (in_network_game && p_locplayer->PanelState[mouser] == PANEL_STATE_SEND_MESSAGE)
    {
      if ( end_pos < lbDisplay.PhysicalScreenWidth - (lbDisplay.PhysicalScreenWidth >> 2) )
          scanner_unkn370 = -20;
      if (end_pos > lbDisplay.PhysicalScreenWidth - 16)
          scanner_unkn370 = 10;
      if (scanner_unkn370 > 0)
      {
          scanner_unkn370--;
          scanner_unkn3CC -= 1 * height / 9;
      }
      if (scanner_unkn370 < 0)
      {
          scanner_unkn370++;
          scanner_unkn3CC += 1 * height / 9;
          if (scanner_unkn3CC > 0)
              scanner_unkn3CC = 0;
      }
    }
    else
    {
        if (end_pos < 0)
            scanner_unkn3CC = width;
        scanner_unkn3CC -= 2 * height / 9;
    }
}

void draw_text_linewrap1(int base_x, int *p_pos_y, int plyr, const char *text)
{
    int pos_x, pos_y;
    const char *str;
    int base_shift;
    TbPixel col2;

    str = text;
    pos_x = base_x;
    pos_y = *p_pos_y;
    col2 = byte_1C5C30[plyr];
    base_shift = -180;
    while (*str != 0)
    {
        if (*str == 32)
        {
            if (pos_x + 2 * font_word_length(str + 1) < lbDisplay.PhysicalScreenWidth - 16) {
                pos_x += 8;
            } else {
                pos_x = base_x;
                pos_y += 12;
            }
        }
        else
        {
            struct TbSprite *p_spr;
            int fd;
            ubyte ch;
            TbPixel col1;

            ch = my_char_to_upper(*str);
            p_spr = &small_font[ch - 31];
            fd = base_shift + 4 * player_unkn0C9[plyr];
            if (fd > 63)
                fd = 63 - (fd - 63);
            if (fd > 63)
                fd = 63;
            if (fd < 0)
                fd = 0;
            col1 = pixmap.fade_table[256 * fd + colour_lookup[8]];
            SCANNER_unkn_func_200(p_spr, pos_x + 1, pos_y + 1, col1);
            SCANNER_unkn_func_200(p_spr, pos_x, pos_y, col2);
            pos_x += 2 * p_spr->SWidth;
        }
        str++;
        base_shift++;
    }
    pos_y += 12;
    *p_pos_y = pos_y;
}

void draw_text_linewrap2(int base_x, int *p_pos_y, int plyr, const char *text)
{
    int pos_x, pos_y;
    const char *str;
    int base_shift;
    TbPixel col2;

    str = text;
    pos_x = base_x;
    pos_y = *p_pos_y;
    col2 = byte_1C5C30[plyr];
    base_shift = -180;
    while (*str != 0)
    {
        if (*str == 32)
        {
            if (font_word_length(str + 1) + pos_x < lbDisplay.PhysicalScreenWidth - 8) {
                pos_x += 4;
            } else {
                pos_x = base_x;
                pos_y += 6;
            }
        }
        else
        {
            struct TbSprite *p_spr;
            int fd;
            ubyte ch;
            TbPixel col1;

            ch = my_char_to_upper(*str);
            p_spr = &small_font[ch - 31];
            fd = base_shift + 4 * (ubyte)player_unkn0C9[plyr];
            if (fd > 63)
                fd = 63 - (fd - 63);
            if (fd > 63)
                fd = 63;
            if (fd < 0)
                fd = 0;
            col1 = pixmap.fade_table[256 * fd + colour_lookup[8]];
            LbSpriteDrawOneColour(pos_x + 1, pos_y + 1, p_spr, col1);
            LbSpriteDrawOneColour(pos_x, pos_y, p_spr, col2);
            pos_x += p_spr->SWidth;
        }
        str++;
        base_shift++;
    }
    pos_y += 6;
    *p_pos_y = pos_y;
}

void draw_players_chat_talk(int x, int y)
{
    char locstr[164];
    int plyr;
    int base_x, pos_y;

    base_x = x;
    pos_y = y;

    for (plyr = 0; plyr < PLAYERS_LIMIT; plyr++)
    {
        char *plname;

        if (player_unkn0C9[plyr] == 0)
            continue;

        plname = unkn2_names[plyr];
        if (player_unknCC9[plyr][0] != '\0')
        {
            if (plname[0] != '\0')
                sprintf(locstr, "%s: %s", plname, player_unknCC9[plyr]);
            else
                sprintf(locstr, "%s", player_unknCC9[plyr]);
        }
        else
        {
            sprintf(locstr, "%s said nothing.", plname);
        }

        if (lbDisplay.GraphicsScreenHeight >= 400)
        {
            draw_text_linewrap1(base_x, &pos_y, plyr, locstr);
        }
        else
        {
            draw_text_linewrap2(base_x, &pos_y, plyr, locstr);
        }

        if ( !--player_unkn0C9[plyr] ) {
            player_unknCC9[plyr][0] = '\0';
        }
    }
}

void SCANNER_draw_objective_info(int x, int y, int width)
{
#if 0
    asm volatile (
      "call ASM_SCANNER_draw_objective_info\n"
        : : "a" (x), "d" (y), "b" (width));
#endif
    int v48;
    int end_pos;
    struct TbAnyWindow bkpwnd;
    int height;
    int i;

    height = SCANNER_objective_info_height();
    v48 = y;
    for (i = 0; i < height; i++)
    {
        SCANNER_unkn_func_203(x, v48, x + width - 1, v48, SCANNER_colour[0],
          ingame.Scanner.Brightness, ingame.Scanner.Contrast);
        ++v48;
    }

    LbScreenStoreGraphicsWindow(&bkpwnd);
    LbScreenSetGraphicsWindow(x + 1, y, width - 2, height);

    end_pos = SCANNER_text_draw(scroll_text, scanner_unkn3CC, height);

    SCANNER_move_objective_info(width, height, end_pos);

    LbScreenLoadGraphicsWindow(&bkpwnd);

    // TODO it would make sense to move this to higher level function
    if (in_network_game)
    {
        short x, y;

        if (lbDisplay.GraphicsScreenHeight >= 400) {
            x = 22;
            y = 51;
        } else {
            x = 11;
            y = 26;
        }
        draw_players_chat_talk(x, y);
    }
}

void SCANNER_unkn_func_205(void)
{
    asm volatile ("call ASM_SCANNER_unkn_func_205\n"
        :  :  : "eax" );
}

TbBool check_scanner_input(void)
{
    int map_x, map_y, map_z;

    SCANNER_find_position(lbDisplay.MouseX, lbDisplay.MouseY, &map_z, &map_x);
    if (MAPCOORD_TO_TILE(map_x) >= 0 && MAPCOORD_TO_TILE(map_x) < 128
      && MAPCOORD_TO_TILE(map_z) >= 0 && MAPCOORD_TO_TILE(map_z) < 128)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];

        if (lbDisplay.LeftButton)
        {
            struct SpecialUserInput *p_usrinp;
            struct Packet *p_pckt;
            TbBool can_control;
            short dcthing;

            dcthing = p_locplayer->DirectControl[mouser];
            can_control = person_can_accept_control(dcthing);
            p_usrinp = &p_locplayer->UserInput[mouser];
            p_pckt = &packets[local_player_no];

            lbDisplay.LeftButton = 0;
            p_usrinp->ControlMode |= 0x8000;
            if ((p_locplayer->DoubleMode) || ((p_usrinp->ControlMode & 0x1FFF) == 1))
            {
                map_y = (alt_at_point(map_x, map_z) >> 8) + 20;
                if ((gameturn & 0x7FFF) - p_usrinp->Turn >= 7)
                {
                    p_usrinp->Turn = gameturn & 0x7FFF;
                    if (can_control)
                        my_build_packet(p_pckt, PAct_AGENT_GOTO_GND_PT_ABS, dcthing, map_x, map_y, map_z);
                }
                else
                {
                    p_usrinp->Turn = 0;
                    if (can_control)
                        my_build_packet(p_pckt, PAct_AGENT_GOTO_GND_PT_ABS_FF, dcthing, map_x, map_y, map_z);
                }
            }
            else
            {
                do_change_mouse(8);
                build_packet(p_pckt, PAct_CONTROL_MODE, 1, 0, 0, 0);
            }
            return true;
        }

        if (lbDisplay.RightButton)
        {
            struct SpecialUserInput *p_usrinp;
            struct Packet *p_pckt;
            TbBool can_control;
            short dcthing;
            short cwep;

            dcthing = p_locplayer->DirectControl[mouser];
            can_control = person_can_accept_control(dcthing);
            p_usrinp = &p_locplayer->UserInput[mouser];

            lbDisplay.RightButton = 0;
            p_usrinp->ControlMode |= 0x4000;
            if (!p_locplayer->DoubleMode)
            {
                map_y = (alt_at_point(map_x, map_z) >> 8) + 20;
                dcthing = p_locplayer->DirectControl[mouser];
                cwep = things[dcthing].U.UPerson.CurrentWeapon;
                p_pckt = &packets[local_player_no];

                if ((cwep == WEP_ELEMINE) || (cwep == WEP_EXPLMINE))
                {
                    if ((gameturn & 0x7FFF) - p_usrinp->Turn >= 7)
                    {
                        p_usrinp->Turn = gameturn & 0x7FFF;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_FACE_PT, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_GND_PT, dcthing, map_x, map_y, map_z);
                        }
                    }
                    else
                    {
                        p_usrinp->Turn = 0;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_FACE_PT_FF, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_GND_PT_FF, dcthing, map_x, map_y, map_z);
                        }
                    }
                }
                else if ((cwep == WEP_RAZORWIRE) || (cwep == WEP_EXPLWIRE))
                {
                    if ((gameturn & 0x7FFF) - p_usrinp->Turn >= 7)
                    {
                        p_usrinp->Turn = gameturn & 0x7FFF;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_SHOOT_AT_FACE_POINT, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_SHOOT_AT_GND_POINT, dcthing, map_x, map_y, map_z);
                        }
                    }
                    else
                    {
                        p_usrinp->Turn = 0;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_SHOOT_AT_FACE_POINT_FF, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_SHOOT_AT_GND_POINT_FF, dcthing, map_x, map_y, map_z);
                        }
                    }
                }
                else
                {
                    if (can_control)
                    {
                        if (p_locplayer->TargetType == 3)
                            my_build_packet(p_pckt, PAct_SHOOT_AT_FACE_POINT, dcthing, map_x, p_locplayer->Target, map_z);
                        else
                            my_build_packet(p_pckt, PAct_SHOOT_AT_GND_POINT, dcthing, map_x, map_y, map_z);
                    }
                }
            }
            return true;
        }
    }
    return false;
}

/**
 * Draw the button with standard palette, for selectable items.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_std(int px, int py, ulong spr_id)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(p_spr, px, py, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
    else
        LbSpriteDraw_1(px, py, p_spr);
}

/**
 * Draw the button with darkened palette, like the item is unavailable.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_dark(int px, int py, ulong spr_id)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(p_spr, px, py, ingame.Scanner.Contrast, 8);
    else
        SCANNER_unkn_func_201(p_spr, px, py, &pixmap.fade_table[4096]);
}

/**
 * Draw the button with standard palette, version for old single weapon list.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_prealp(int px, int py, ulong spr_id)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(p_spr, px, py, ingame.Scanner.Contrast,
          ingame.Scanner.Brightness);
}

/**
 * Draws squares for fourpacks, given specific screen coords and amount to fill.
 *
 * @param x
 * @param y
 * @param amount
 */
void draw_fourpack_amount(short x, ushort y, ushort amount)
{
    // We're expecting to draw 4 items; 8 are supported mostly to signal an issue
    static short dtx[] = {0+2, 0+2, 60-4-2, 60-4-2, 0+8, 0+8, 60-4-2-4-2, 60-4-2-4-2,};
    static short dty[] = {0+0, 18-4-0, 0+0, 18-4-0, 0+0, 18-4-0, 0+0, 18-4-0,};
    int i;
    TbPixel col;

    if (ingame.PanelPermutation == -3)
        col = 26;
    else
        col = 247;

    for (i = 0; i < min(amount,8); i++)
    {
      if (lbDisplay.GraphicsScreenHeight < 400)
          LbDrawBox(x + (dtx[i] >> 1), y + (dty[i] >> 1), 2, 2, col);
      else
          LbDrawBox(x + dtx[i], y + dty[i], 4, 4, col);
    }
}

TbBool panel_agents_weapon_highlighted(PlayerInfo *p_locplayer, ushort plagent, short weptype)
{
    return ((weptype == p_locplayer->PanelItem[mouser]) && (agent_with_mouse_over_weapon == plagent));
}

/**
 * For weapons which contain up to four itemized parts, draw the items.
 *
 * @param x
 * @param y
 * @param plagent
 * @param weptype
 */
void draw_fourpack_items(short x, short y, short plagent, short weptype)
{
    ushort fp;

    fp = weapon_fourpack_index(weptype);
    if (fp < WFRPK_COUNT) {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
        draw_fourpack_amount(x, y, p_locplayer->FourPacks[fp][plagent]);
    }
}

TbBool draw_panel_pickable_thing_below_agent(struct Thing *p_agent)
{
    struct SimpleThing *p_pickup;
    ThingIdx thing;
    TbBool drawn;

    if ((p_agent->Flag & TngF_Unkn08000000) == 0)
        return false;

    thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
    if (thing < 0)
        p_pickup = &sthings[thing];
    else
        p_pickup = NULL;

    drawn = false;
    if ((p_pickup != NULL) && (p_pickup->Type == SmTT_DROPPED_ITEM))
    {
        ushort weptype;
        short x, y;
        ushort spr;

        if (lbDisplay.GraphicsScreenHeight >= 400) {
            x = lbDisplay.GraphicsScreenWidth - 100;
            y = lbDisplay.GraphicsScreenHeight - 40;
        } else {
            x = lbDisplay.GraphicsScreenWidth - 50;
            y = lbDisplay.GraphicsScreenHeight - 20;
        }
        lbDisplay.DrawFlags = 0;
        weptype = p_pickup->U.UWeapon.WeaponType;

        draw_new_panel_sprite_std(x, y, 12);
        if (weptype)
            spr = weapon_sprite_index(weptype, false);
        else
            spr = 70;
        x += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
        y += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
        draw_new_panel_sprite_std(x, y, spr);
        drawn = true;
    }
    return drawn;
}

TbBool draw_panel_pickable_thing_player_targeted(PlayerInfo *p_locplayer)
{
    struct SimpleThing *p_pickup;
    ThingIdx thing;
    TbBool drawn;

    if (p_locplayer->TargetType != 5)
        return false;

    thing = p_locplayer->field_102;
    if (thing < 0)
        p_pickup = &sthings[thing];
    else
        p_pickup = NULL;

    drawn = false;
    if (p_pickup != NULL)
    {
        ushort weptype;
        short x, y;
        ushort spr;

        if (lbDisplay.GraphicsScreenHeight >= 400) {
            x = lbDisplay.GraphicsScreenWidth - 100;
            y = lbDisplay.GraphicsScreenHeight - 40;
        } else {
            x = lbDisplay.GraphicsScreenWidth - 50;
            y = lbDisplay.GraphicsScreenHeight - 20;
        }
        lbDisplay.DrawFlags = 0;
        weptype = p_pickup->U.UWeapon.WeaponType;

        draw_new_panel_sprite_std(x, y, 12);
        if (weptype)
            spr = weapon_sprite_index(weptype, false);
        else
            spr = 70;
        x += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
        y += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
        draw_new_panel_sprite_std(x, y, spr);
        drawn = true;
    }
    return drawn;
}

/**
 * Counts weapons in given flags, returning total, amount below given weapon and above it.
 * To be used for displaying scrollable list of weapons where current weapon is always visible.
 *
 * @param p_ncarr_below
 * @param p_ncarr_above
 * @param weapons_carried
 * @param current_weapon
 * @return
 */
int count_weapons_in_flags(int *p_ncarr_below, int *p_ncarr_above, ulong weapons_carried, short current_weapon)
{
    int ncarried, ncarr_below, ncarr_above;
    ulong wepflags;
    ushort nweptype;

    ncarried = 0;
    ncarr_above = 0;
    ncarr_below = 0;
    wepflags = weapons_carried;

    for (nweptype = 1; nweptype < WEP_TYPES_COUNT; nweptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if (wepflags & 1)
        {
            ncarried++;
            if (nweptype > current_weapon)
                ncarr_above++;
            if (nweptype < current_weapon)
                ncarr_below++;
        }
    }

    *p_ncarr_below = ncarr_below;
    *p_ncarr_above = ncarr_above;
    return ncarried;
}

void draw_agent_carried_weapon(PlayerInfo *p_locplayer, ushort plagent, short slot, TbBool ready, short weptype, short cx, short cy)
{
    TbBool wep_highlight;
    TbBool recharging;
    short x, y;
    ushort spr;

    recharging = p_locplayer->WepDelays[plagent][weptype] != 0;
    wep_highlight = panel_agents_weapon_highlighted(p_locplayer, plagent, weptype);

    lbDisplay.DrawFlags = 0;
    if (!recharging || (gameturn & 1))
    {
        if (slot == 1) { // First weapon on list entry starts its sprite a bit earlier
            x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].y;
            spr = 13;
        } else {
            x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].y;
            spr = 94;
        }
        draw_new_panel_sprite_std(x, y, spr);

        if (slot == 1) {
            x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].x;
            y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].y;
        } else {
            x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].x;
            y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].y;
        }
        draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, ready));
    }

    if (wep_highlight)
    {
        lbDisplay.DrawFlags = 0;
        if (slot == 1) {// The first on list longer sprite has its own highlight
            x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].y;
            spr = 92;
        } else {
            x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].y;
            spr = 93;
        }
        draw_new_panel_sprite_std(x, y, spr);
    }

    if (slot == 1) {
        x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].x;
        y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].y;
    } else {
        x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].x;
        y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].y;
    }

    draw_fourpack_items(x, y, plagent, weptype);
}

void draw_agent_current_weapon(PlayerInfo *p_locplayer, ushort plagent, short slot, TbBool darkened, TbBool ready, short weptype, short cx, short cy)
{
    TbBool wep_highlight;
    TbBool recharging;
    short x, y;

    recharging = p_locplayer->WepDelays[plagent][weptype] != 0;
    wep_highlight = panel_agents_weapon_highlighted(p_locplayer, plagent, weptype);

    if (!recharging || (gameturn & 1))
    {
        x = cx + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
        y = cy + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
        if (ready)
            draw_new_panel_sprite_prealp(x, y, 14);

        if (!darkened) {
            draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, ready));
        } else {
            draw_new_panel_sprite_dark(x, y, weapon_sprite_index(weptype, ready));
        }
    }

    // Additional border highlight around button on mouse over
    if (wep_highlight)
    {
        draw_new_panel_sprite_std(cx, cy, 90);
    }

    x = cx + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
    y = cy + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
    draw_fourpack_items(x, y, plagent, weptype);
}

void draw_agent_carried_weapon_prealp_list(PlayerInfo *p_locplayer, ushort plagent, short slot, TbBool ready, short weptype, short cx, short cy)
{
    TbBool wep_highlight;
    TbBool recharging;
    short x, y;

    recharging = p_locplayer->WepDelays[plagent][weptype] != 0;
    wep_highlight = panel_agents_weapon_highlighted(p_locplayer, plagent, weptype);

    x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].x;
    y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].y;

    lbDisplay.DrawFlags = 0;
    if (!recharging || (gameturn & 1))
    {
        if (slot == 6)
            draw_new_panel_sprite_std(cx, cy, 13);
        else
            draw_new_panel_sprite_std(cx, cy, 12);

        draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, false));
    }
    if (ready) {
        draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, true));
    }

    if (wep_highlight)
    {
        draw_new_panel_sprite_std(cx, cy, 90);
    }

    draw_fourpack_items(x, y, plagent, weptype);
}

TbBool panel_mouse_over_weapon(short box_x, short box_y, short box_w, short box_h, int panstate, short box_no)
{
    short msx, msy;

    if ((panstate >= PANEL_STATE_WEP_SEL_GRP) && (panstate < PANEL_STATE_WEP_SEL_GRP + 4))
    {
        if (!lbDisplay.MRightButton) {
            msx = lbDisplay.RMouseX;
            msy = lbDisplay.RMouseY;
            if (over_box(msx, msy, box_x, box_y, box_w, box_h))
                return true;
        }
    }
    {
        msx = lbDisplay.MMouseX;
        msy = lbDisplay.MMouseY;
        if (over_box(msx, msy, box_x, box_y, box_w, box_h))
            return true;
    }
    return false;
}

/** Updates a list of weapons in pre-alpha style.
 *
 * This function is intended to loop through weapons in the same way
 * as draw_weapons_list_prealp(), but update state instead of drawing.
 */
TbBool update_weapons_list_prealp(PlayerInfo *p_locplayer, ushort plagent, ulong weapons_carried, short current_weapon)
{
    struct GamePanel *p_panel;
    ushort nshown;
    int weptype;
    int nchecked;
    ulong wepflags;
    short cx, cy;
    int ncarried;
    int ncarr_below;
    int ncarr_above;
    TbBool ret;

    ret = false;
    ncarried = count_weapons_in_flags(&ncarr_below, &ncarr_above, weapons_carried, current_weapon);
    // Show at least 4 weapons below the currently selected
    if (ncarried <= 6)
        ncarr_below = 0;
    else if (ncarr_below <= 4)
        ncarr_below = 0;
    else
        ncarr_below -= 4;
    p_panel = &game_panel[19];
    cx = p_panel->X;
    cy = p_panel->Y;

    nshown = 0;
    wepflags = weapons_carried;
    nchecked = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        if (nshown >= ncarr_below)
        {
            TbBool wep_highlight;
            short w, h;

            w = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].x + 2, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x - 1);
            h = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].y, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y - 1);
            wep_highlight = panel_mouse_over_weapon(cx - 1, cy, w, h, PANEL_STATE_WEP_SEL_GRP + plagent, 0);

            if (wep_highlight)
            {
                p_locplayer->PanelItem[mouser] = weptype;
                agent_with_mouse_over_weapon = plagent;
                ret = true;
                break;
            }

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            ++nchecked;
        }
        nshown++;
        if (nchecked == 6)
            break;
    }
    return ret;
}

void draw_weapons_list_prealp(PlayerInfo *p_locplayer, ushort plagent, ulong weapons_carried, short current_weapon)
{
    struct GamePanel *p_panel;
    ushort nshown;
    ushort weptype;
    int nchecked;
    ulong wepflags;
    short cx, cy;
    int ncarried;
    int ncarr_below;
    int ncarr_above;

    ncarried = count_weapons_in_flags(&ncarr_below, &ncarr_above, weapons_carried, current_weapon);
    // Show at least 4 weapons below the currently selected
    if (ncarried <= 6)
        ncarr_below = 0;
    else if (ncarr_below <= 4)
        ncarr_below = 0;
    else
        ncarr_below -= 4;
    p_panel = &game_panel[19];
    cx = p_panel->X;
    cy = p_panel->Y;

    nshown = 0;
    wepflags = weapons_carried;
    nchecked = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        if (nshown >= ncarr_below)
        {
            draw_agent_carried_weapon_prealp_list(p_locplayer, plagent, nshown, (weptype == current_weapon), weptype, cx, cy);

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            ++nchecked;
        }
        nshown++;
        if (nchecked == 6)
            break;
    }
}

TbBool panel_update_weapon_current(PlayerIdx plyr, short nagent, ubyte flags)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ushort panstate;
    ushort curwep, prevwep;
    short cx, cy;
    TbBool wep_highlight;
    short w, h;

    // If 0x01 not set, do not draw and do not access the agent as it may be invalid
    if ((flags & 0x01) == 0)
        return false;
    // If the panel is drawn but disabled, disallow interaction
    if ((flags & 0x02) != 0)
        return false;

    p_player = &players[plyr];
    p_panel = &game_panel[12 + nagent];

    cx = p_panel->X;
    cy = p_panel->Y;
    p_agent = p_player->MyAgent[nagent];

    panstate = p_player->PanelState[mouser];
    // If we have panel state opened for another agent, disallow interaction
    if ((panstate != PANEL_STATE_NORMAL) && (nagent != panel_state_to_player_agent(panstate)))
        return false;

    curwep = p_agent->U.UPerson.CurrentWeapon;
    prevwep = p_player->PrevWeapon[nagent];
    if (!curwep && !prevwep) {
        prevwep = find_nth_weapon_held(p_agent->ThingOffset, 1);
        p_player->PrevWeapon[nagent] = prevwep;
    }

    if ((curwep == 0) && (prevwep == 0)) {
        return false;
    }

    w = game_panel_shifts[PaSh_WEP_CURR_BUTTON_AREA].x;
    h = game_panel_shifts[PaSh_WEP_CURR_BUTTON_AREA].y;
    wep_highlight = panel_mouse_over_weapon(cx, cy, w, h, panstate, 0);

    if (wep_highlight)
    {
        if (curwep)
            p_player->PanelItem[mouser] = curwep;
        else
            p_player->PanelItem[mouser] = prevwep;

        agent_with_mouse_over_weapon = nagent;
    }

    return wep_highlight;
}

short draw_current_weapon_button(PlayerInfo *p_locplayer, short nagent, ubyte flags)
{
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ushort curwep, prevwep;
    short cx, cy;
    TbBool darkened;

    // If 0x01 not set, do not draw and do not access the agent as it may be invalid
    if ((flags & 0x01) == 0)
        return 0;

    p_agent = p_locplayer->MyAgent[nagent];
    p_panel = &game_panel[12 + nagent];

    cx = p_panel->X;
    cy = p_panel->Y;
    curwep = p_agent->U.UPerson.CurrentWeapon;
    prevwep = p_locplayer->PrevWeapon[nagent];

    darkened = (p_agent->State == PerSt_PROTECT_PERSON) || ((flags & 0x02) != 0);
    if (curwep != 0) // Is ready/drawn weapon - draw lighted weapon shape
    {
        draw_agent_current_weapon(p_locplayer, nagent, 0, darkened, true, curwep, cx, cy);
    }
    else if (prevwep != 0) // Weapon is carried but hidden - draw with dark weapon shape
    {
        curwep = prevwep;
        draw_agent_current_weapon(p_locplayer, nagent, 0, darkened, false, curwep, cx, cy);
    }
    return curwep;
}

/** Updates a list of weapons below current weapon panel.
 *
 * This function is intended to loop through weapons in the same way
 * as draw_agent_weapons_selection(), but update state instead of drawing.
 */
TbBool update_agent_weapons_selection(PlayerIdx plyr, short nagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ulong wepflags;
    ushort panstate;
    int cx, cy;
    short cur_ready_wep, weptype;
    ushort plagent;
    ushort nshown, nchecked;
    TbBool ret;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[nagent];
    // Protect from damaged / unfinished levels
    if ((p_agent->Type != TT_PERSON) || (p_agent->Flag & TngF_Destroyed) != 0) {
        p_player->PanelState[mouser] = PANEL_STATE_NORMAL;
        return false;
    }

    ret = false;
    panstate = p_player->PanelState[mouser];
    plagent = p_agent->U.UPerson.ComCur & 3;
    wepflags = p_agent->U.UPerson.WeaponsCarried;
    p_panel = &game_panel[12 + plagent];

    cx = p_panel->X;
    cy = p_panel->Y;
    cx += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].x;
    cy += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].y;
    { // Shift the input a bit up to avoid having a do-nothin area between current weapon and the weapon list
        struct TbSprite *p_spr;

        p_spr = &pop1_sprites[12];
        cy -= max(game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].y - p_spr->SHeight, 0);
    }

    cur_ready_wep = player_agent_current_or_prev_weapon(plyr, plagent);

    // Some weapons are not selectable
    wepflags &= ~(1 << (WEP_ENERGYSHLD-1));
    nchecked = 0;
    nshown = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        TbBool wep_visible;

        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        wep_visible = (cur_ready_wep != weptype);

        if (wep_visible)
        {
            TbBool wep_highlight;
            short w, h;

            w = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].x + 2, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x - 1);
            h = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].y, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y - 1);
            wep_highlight = panel_mouse_over_weapon(cx - 1, cy, w, h, panstate, nshown + 1);

            if (wep_highlight) {
                p_player->PanelItem[mouser] = weptype;
                agent_with_mouse_over_weapon = plagent;
                ret = true;
                break;
            }

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            nshown++;
        }

        nchecked++;
        if (nshown == 12)
            break;
    }
    return ret;
}

/** Draws a list of weapons below current weapon panel.
 */
void draw_agent_weapons_selection(PlayerIdx plyr, short nagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ulong wepflags;
    int cx, cy;
    short cur_ready_wep, weptype;
    ushort plagent;
    ushort nshown, nchecked;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[nagent];
    // Assured by update_agent_weapons_selection()
    assert(p_agent->Type == TT_PERSON);

    plagent = p_agent->U.UPerson.ComCur & 3;
    wepflags = p_agent->U.UPerson.WeaponsCarried;
    p_panel = &game_panel[12 + plagent];

    cx = p_panel->X;
    cy = p_panel->Y;
    cx += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].x;
    cy += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].y;

    cur_ready_wep = player_agent_current_or_prev_weapon(plyr, plagent);

    // Some weapons are not selectable
    wepflags &= ~(1 << (WEP_ENERGYSHLD-1));
    nchecked = 0;
    nshown = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        TbBool wep_visible;

        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        wep_visible = (cur_ready_wep != weptype);

        if (wep_visible)
        {
            draw_agent_carried_weapon(p_player, plagent, nshown + 1, false, weptype, cx, cy);

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            nshown++;
        }

        nchecked++;
        if (nshown == 12)
            break;
    }
}

void draw_panel_pickable_item(void)
{
    TbBool ret;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    short dcthing;

    p_locplayer = &players[local_player_no];
    dcthing = direct_control_thing_for_player(local_player_no);
    // FIXME a strange place for fixing state of an agent; should be moved to game world update
    update_dropped_item_under_agent_exists(dcthing);
    p_agent = &things[dcthing];

    ret = draw_panel_pickable_thing_below_agent(p_agent);
    if (!ret)
        draw_panel_pickable_thing_player_targeted(p_locplayer);
}

TbBool func_1caf8(ubyte *panel_wep)
{
    TbBool ret;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    short dcthing;

    p_locplayer = &players[local_player_no];
    dcthing = direct_control_thing_for_player(local_player_no);
    p_agent = &things[dcthing];

    p_locplayer->PanelItem[mouser] = 0;
    if (ingame.PanelPermutation >= 0)
    {
        ushort plagent;
        plagent = p_agent->U.UPerson.ComCur & 3;
        ret = update_weapons_list_prealp(p_locplayer, plagent,
            p_agent->U.UPerson.WeaponsCarried, p_agent->U.UPerson.CurrentWeapon);
    }
    else
    {
        short nagent;
        ushort panstate;

        agent_with_mouse_over_weapon = -1;
        ret = false;
        for (nagent = 0; nagent < playable_agents; nagent++)
        {
            ret |= panel_update_weapon_current(local_player_no, nagent, panel_wep[nagent]);
        }
        panstate = p_locplayer->PanelState[mouser];
        nagent = panel_state_to_player_agent(panstate);
        if ((panstate >= PANEL_STATE_WEP_SEL_ONE) && (panstate <= PANEL_STATE_WEP_SEL_GRP + 3) && (nagent >= 0))
        {
            ret |= update_agent_weapons_selection(local_player_no, nagent);
        }
    }

    if (ingame.PanelPermutation >= 0)
    {
        ushort plagent;
        plagent = p_agent->U.UPerson.ComCur & 3;
        draw_weapons_list_prealp(p_locplayer, plagent,
            p_agent->U.UPerson.WeaponsCarried, p_agent->U.UPerson.CurrentWeapon);
    }
    else
    {
        short nagent;
        ushort panstate;

        for (nagent = 0; nagent < playable_agents; nagent++)
        {
            draw_current_weapon_button(p_locplayer, nagent, panel_wep[nagent]);
        }

        panstate = p_locplayer->PanelState[mouser];
        nagent = panel_state_to_player_agent(panstate);
        if ((panstate >= PANEL_STATE_WEP_SEL_ONE) && (panstate <= PANEL_STATE_WEP_SEL_GRP + 3) && (nagent >= 0))
        {
            draw_agent_weapons_selection(local_player_no, nagent);
        }
    }

    lbDisplay.DrawFlags = 0;
    return ret;
}

void draw_agent_grouping_bars(void)
{
    struct Thing *p_thing;
    short dcthing;
    short i, n;

    n = 0;
    for (i = 0; i < playable_agents; i++)
    {
        p_thing = players[local_player_no].MyAgent[i];
        if (p_thing->Type != TT_PERSON) continue;
        dcthing = players[local_player_no].DirectControl[byte_153198-1];
        if ((p_thing->State != PerSt_PROTECT_PERSON) || (p_thing->GotoThingIndex != dcthing)) {
            if (((p_thing->Flag2 & TgF2_Unkn10000000) == 0) || (p_thing->Owner != dcthing))
                continue;
        }
        n++;
    }

    for (n--; n >= 0; n--)
    {
        struct GamePanel *p_panel;
        short x, y;

        if (lbDisplay.GraphicsScreenHeight < 400) {
            p_panel = &game_panel[17];
        } else {
            p_panel = &game_panel[18];
        }
        x = p_panel->X;
        y = p_panel->Y;
        x += game_panel_shifts[PaSh_GROUP_PANE_AGENTS + n].x;
        y += game_panel_shifts[PaSh_GROUP_PANE_AGENTS + n].y;
        if (ingame.PanelPermutation == -1)
            SCANNER_unkn_func_202(&pop1_sprites[69], x, y,
              ingame.Scanner.Contrast, ingame.Scanner.Brightness);
        else
            LbSpriteDraw_1(x, y, &pop1_sprites[69]);
    }
}

void func_702c0(int a1, int a2, int a3, int a4, int a5, ubyte a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_func_702c0\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6));
}

void draw_transparent_slant_bar(short x, short y, ushort w, ushort h)
{
    long waftx, wafty;
    ushort tmx, tmy;
    struct EnginePoint point4;
    struct EnginePoint point2;
    struct EnginePoint point1;
    struct EnginePoint point3;
    short sh_x;

    sh_x = 3;
    if (lbDisplay.GraphicsScreenHeight < 400)
        sh_x /= 2;
    point1.pp.X = x;
    point1.pp.Y = y;
    point4.pp.X = (x + w);
    point4.pp.Y = y;
    point2.pp.Y = (y + h);
    point2.pp.X = (x + w - sh_x);
    point3.pp.Y = (y + h);
    point3.pp.X = (x - sh_x);

    // The shield bar is animated, even if it's not possible to see
    waftx = waft_table[(gameturn >> 3) & 31];
    wafty = waft_table[(gameturn + 16) & 31];
    tmx = ((waftx + 30) >> 1);
    tmy = ((wafty + 30) >> 3) + 64;
    point1.pp.U = tmx << 16;
    point4.pp.U = (tmx + 64) << 16;
    point2.pp.U = (tmx + 64) << 16;
    point1.pp.V = tmy << 16;
    point4.pp.V = tmy << 16;
    point2.pp.V = (tmy + 8) << 16;
    point3.pp.U = tmx << 16;
    point3.pp.V = (tmy + 8) << 16;

    point1.pp.S = 0;
    point2.pp.S = 0;
    point3.pp.S = 0;
    point4.pp.S = 0;

    vec_mode = 18;
    vec_map = dword_1AA280;
    draw_trigpoly(&point1.pp, &point4.pp, &point3.pp);
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point4.pp, &point2.pp, &point3.pp);
}

void draw_health_level(short x, short y, ushort w, ushort h, short lv, ushort lvmax, ubyte col, ubyte transp)
{
    short cw;

    if ((lv <= 0) || (lvmax == 0))
        return;

    cw = w * lv / lvmax;
    if (transp)
    {
        draw_transparent_slant_bar(x, y, cw, h);
    }
    else
    {
        ApDrawSlantBox(x, y, cw, h, col);
    }
}

void func_1eae4(int x, short y, int w, ushort h, short lv, ushort lvmax, ubyte col, int a8)
{
    short cw, ch;

    if ((lv <= 0) || (lvmax == 0))
        return;

    ch = h * lv / lvmax;
    if (lbDisplay.GraphicsScreenHeight < 400)
    {
        short cx, cy;
        cx = x;
        cy = y;
        for (cw = w; cw > 0; cw--)
        {
            short cy1, cy2;
            cy1 = cy + h;
            cy2 = cy + h - ch;
            if (lbDisplay.GraphicsScreenHeight < 400)
                SCANNER_unkn_func_203(2 * cx >> 1, 2 * cy1 >> 1, 2 * cx >> 1, 2 * cy2 >> 1, col,
                    ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            else
                SCANNER_unkn_func_203(2 * cx, 2 * cy1, 2 * cx, 2 * cy2, col,
                    ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            ++cx;
            ++cy;
        }
    }
    else
    {
        short cx, cy;
        cx = 2 * x;
        cy = 2 * y;
        for (cw = 2 * w; cw > 0; cw--)
        {
            short cy1, cy2;
            cy1 = 2*h + cy;
            cy2 = 2*h + cy - 2*ch;
            if (lbDisplay.GraphicsScreenHeight < 400)
                LbDrawLine(cx >> 1, cy1 >> 1, cx >> 1, cy2 >> 1, col);
            else
                LbDrawLine(cx, cy1, cx, cy2, col);
            ++cx;
            ++cy;
        }
    }
}

void draw_mood_level(short x, short y, ushort w, int h, short value)
{
    short cent_x;
    short x1, y1, x2;
    TbPixel col;
    short fade;
    short i;

    fade = value >> 2;
    if (value >= 0)
        col = pixmap.fade_table[PALETTE_8b_COLORS * (63 - fade) + colour_lookup[ColLU_RED]];
    else
        col = pixmap.fade_table[PALETTE_8b_COLORS * (63 + fade) + colour_lookup[ColLU_BLUE]];

    cent_x = x + (w >> 1);
    x1 = cent_x;
    x2 = x1 + (w >> 1) * value / 88;
    y1 = y;

    for (i = h; i > 0; i--)
    {
        SCANNER_unkn_func_203(x1, y1, x2, y1,
            col, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
        x1--;
        x2--;
        y1++;
    }
}

void draw_mood_limits(short x, short y, short w, short h, short value, short maxval)
{
    short scaled_val;
    short curr_x, sh_x;
    TbPixel col;

    if (value <= 0)
        return;

    sh_x = 4;
    if (lbDisplay.GraphicsScreenHeight < 400)
        sh_x /= 2;

    col = colour_lookup[ColLU_WHITE];
    scaled_val = (w * value / maxval) >> 1;

    curr_x = x + (w >> 1) - scaled_val;
    LbDrawLine(curr_x, y, curr_x - sh_x, (y + h), col);

    curr_x = x + (w >> 1) + scaled_val;
    LbDrawLine(curr_x, y, curr_x - sh_x, (y + h), col);
}

void draw_energy_bar(int x1, int y1, short w, short h, int value, int maxval)
{
    short scaled_val;
    short x2, y2;
    short i;
    TbPixel col;

    if (maxval == 0)
        return;

    col = colour_lookup[ColLU_WHITE];
    scaled_val = h * value / maxval;

    x2 = x1 + scaled_val;
    y2 = y1 - scaled_val;
    for (i = w; i > 0; i--)
    {
        LbDrawLine(x1, y1, x2, y2, col);
        x1++;
        x2++;
    }
}


TbBool panel_active_based_on_target(short panel)
{
    struct GamePanel *p_panel;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;

    p_panel = &game_panel[panel];

    if ((p_panel->Type == PanT_UNKN08) || (p_panel->Type == PanT_UNKN10))
        return true;

    if (p_panel->ID >= playable_agents)
        return true;

    p_locplayer = &players[local_player_no];
    p_agent = p_locplayer->MyAgent[p_panel->ID];

    if (p_agent->Type != TT_PERSON)
        return false;

    return ((p_agent->Flag & TngF_Destroyed) == 0);
}

TbBool mouse_move_over_panel(short panel)
{
    struct GamePanel *p_panel;
    short x, y, w, h;

    p_panel = &game_panel[panel];

    x = p_panel->X;
    y = p_panel->Y;
    if ((p_panel->Width == 0) && (p_panel->Height == 0))
    {
        struct TbSprite *p_spr;
        p_spr = &pop1_sprites[p_panel->Spr];
        w = p_spr->SWidth;
        h = p_spr->SHeight;
    } else {
        w = p_panel->Width;
        h = p_panel->Height;
    }

    if (!panel_active_based_on_target(panel))
        return false;

    // We are only interested in panels for control of agents
    if (p_panel->ID >= playable_agents)
        return false;

    return mouse_move_over_box_coords(x, y, x + w, y + h);
}

void update_game_panel(void)
{
    int i;
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    // If an agent has a medkit, use the sprite with lighted cross
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        p_agent = p_locplayer->MyAgent[i];
        if ((p_agent->Type == TT_PERSON) && person_carries_any_medikit(p_agent))
            game_panel[8+i].Spr = 96;
        else
            game_panel[8+i].Spr = 95;
    }

    { // If supershield is enabled for the current agent, draw energy bar in red
        struct Thing *p_agent;
        p_agent = &things[p_locplayer->DirectControl[0]];
        if ((p_agent->Type == TT_PERSON) && (p_agent->Flag & TngF_Unkn0100) != 0)
        {
            game_panel[16].Spr = 99;
            if (lbDisplay.GraphicsScreenHeight >= 400)
                game_panel[17].Spr = 106;
        }
        else
        {
            game_panel[16].Spr = 10;
            if (lbDisplay.GraphicsScreenHeight >= 400)
                game_panel[17].Spr = 105;
        }
    }
}

TbBool mouse_over_infrared_slant_box(short panel)
{
    struct GamePanel *p_panel;
    short x, y;
    short ms_x, ms_y;
    short delta_x, delta_y;

    p_panel = &game_panel[panel];

    x = p_panel->X;
    y = p_panel->Y;
    x += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_BOX].x;
    y += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_BOX].y;

    ms_x = lbDisplay.MouseX;
    ms_y = lbDisplay.MouseY;
    delta_x = (ms_x - x);
    delta_y = (ms_y - y);

    return (ms_y > y) && (delta_x + 22 - delta_y < 22);
}

short panel_mouse_move_mood_value(short panel)
{
    struct GamePanel *p_panel;
    short i;
    short x;

    p_panel = &game_panel[panel];
    x = p_panel->X;
    i = mouse_move_position_horizonal_over_bar_coords(x, p_panel->Width);
    i = 2 * (i * 88 / p_panel->Width) - 88;
    if (i < -88) i = -88;
    if (i > 88) i = 88;

    return i;
}

/** Thermal vision button light.
 */
void draw_panel_thermal_button(void)
{
    if ((ingame.Flags & GamF_ThermalView) != 0)
    {
        struct GamePanel *p_panel;
        short x, y;

        if (lbDisplay.GraphicsScreenHeight < 400) {
            p_panel = &game_panel[17];
        } else {
            p_panel = &game_panel[18];
        }
        x = p_panel->X;
        y = p_panel->Y;
        x += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_SPR].x;
        y += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_SPR].y;
        draw_new_panel_sprite_std(x, y, 91);
    }
}

/** Objective text, or net players list.
 */
void draw_panel_objective_info(void)
{
    int x, y, w;
    x = ingame.Scanner.X1 - 1;
    if (x < 0)
        x = 0;
    y = lbDisplay.GraphicsScreenHeight - SCANNER_objective_info_height();
    if (in_network_game) {
        SCANNER_unkn_func_205();
        w = lbDisplay.PhysicalScreenWidth;
    } else {
        // original width 67 low res, 132 high res
        w = ingame.Scanner.X2 - ingame.Scanner.X1 + 3;
    }
    SCANNER_draw_objective_info(x, y, w);
}

void draw_new_panel_badge_overlay(ushort panel, ushort plagent, TbBool darkened)
{
    struct GamePanel *p_panel;
    struct GamePanel *p_mopanel;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    int lv, lvmax;
    short x, y, w, h;

    p_panel = &game_panel[0 + plagent];
    p_mopanel = &game_panel[4 + plagent];
    p_locplayer = &players[local_player_no];
    p_agent = p_locplayer->MyAgent[plagent];
    if ((p_agent->Flag & TngF_PlayerAgent) == 0) {
        LOGERR("Agent %d unexpected flags", plagent);
        return;
    }

    // The X of health bar in anchored to mood panel
    x = p_mopanel->X;
    y = p_panel->Y;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        x += 4;
        y += 2;
    } else {
        x += 8;
        y += 4;
    }
    w = p_mopanel->Width;
    if (ingame.PanelPermutation >= 0)
        h = 6;
    else
        h = 4;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        h /= 2;
    }
    // Draw health level
    lv = p_agent->Health;
    lvmax = p_agent->U.UPerson.MaxHealth;
    if (lv <= lvmax) { // Normal health amount
        draw_health_level(x, y, w, h, lv, lvmax, colour_lookup[ColLU_WHITE], 0);
    } else { // Health reinforced beyond max is drawn in red
        draw_health_level(x, y, w, h, lvmax, lvmax, colour_lookup[ColLU_WHITE], 0);
        draw_health_level(x, y, w, h, lv - lvmax, lvmax, colour_lookup[ColLU_RED], 0);
    }
    // Draw shield level over health
    lv = p_agent->U.UPerson.ShieldEnergy;
    draw_health_level(x, y, w, h, lv, 0x400, colour_lookup[ColLU_WHITE], 1);

    // Draw drug level aka mood (or just a red line if no drugs)
    x = p_mopanel->X;
    y = p_mopanel->Y;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        y += 1;
    } else {
        y += 2;
    }
    w = p_mopanel->Width;
    h = 6;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        h /= 2;
    }
    draw_mood_level(x, y, w, h, p_agent->U.UPerson.Mood);
    // Draw stamina level which caps the mood level
    h = 4;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        h /= 2;
    }
    lv = p_agent->U.UPerson.Stamina;
    lvmax = p_agent->U.UPerson.MaxStamina;
    draw_mood_limits(x, y, w, h, lv, lvmax);

    if (lbDisplay.GraphicsScreenHeight < 400)
        x = p_panel->X + p_panel->Width - 8;
    else
        x = p_panel->X + p_panel->Width - 17;
    y = p_panel->Y;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        y += 9;
    } else {
        y += 18;
    }
    w = 4;
    h = 14;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        w /= 2;
        h /= 2;
    }
    lv = p_agent->U.UPerson.Energy;
    lvmax = p_agent->U.UPerson.MaxEnergy;
    draw_energy_bar(x, y, w, h, lv, lvmax);
}

void draw_new_panel_weapon_overlay(ushort panel, ushort plagent, TbBool darkened)
{
}

void draw_new_panel(void)
{
    PlayerInfo *p_locplayer;
    int i;
    ubyte panel_wep[AGENTS_SQUAD_MAX_COUNT];

    update_game_panel();

    p_locplayer = &players[local_player_no];
    LbMemorySet(panel_wep, '\0', sizeof(panel_wep));

    for (i = 0; true; i++)
    {
        struct GamePanel *p_panel;

        p_panel = &game_panel[i];
        if (p_panel->Spr < 0)
          break;
        if (p_panel->Spr == 0)
          continue;
        lbDisplay.DrawFlags = 0;

        if (p_panel->Type != PanT_AgentBadge && p_panel->Type != PanT_AgentMedi && p_panel->Type != PanT_AgentWeapon)
        {
            short x, y;
            x = p_panel->X;
            y = p_panel->Y;
            draw_new_panel_sprite_std(x, y, p_panel->Spr);
        }
        else
        {
            TbBool is_visible;
            TbBool is_darkened;
            short x, y;

            is_visible = true;
            if (p_panel->ID >= playable_agents)
            {
                is_visible = false;
            }
            if (!player_agent_is_alive(local_player_no, p_panel->ID))
            {
                is_visible = false;
            }
            else if (p_panel->Type == PanT_AgentWeapon)
            {
                if (!player_agent_is_executing_commands(local_player_no, p_panel->ID))
                {
                    ubyte weapon;
                    short wep_delay;

                    weapon = player_agent_current_or_prev_weapon(local_player_no, p_panel->ID);
                    wep_delay = player_agent_weapon_delay(local_player_no, p_panel->ID, weapon);

                    is_visible = (weapon != 0) && ((wep_delay == 0) || (gameturn & 1));
                }
                else
                {
                    // If executing predefined commands rather than being controlled by player
                    is_visible = false;
                }
            }

            if (!is_visible)
                continue;
            {
                struct Thing *p_agent;
                p_agent = p_locplayer->MyAgent[p_panel->ID];

                is_darkened = ((p_agent->State == PerSt_PROTECT_PERSON) ||
                  (p_agent->Flag2 & TgF2_Unkn10000000) ||
                  person_is_executing_commands(p_agent->ThingOffset));
            }

            x = p_panel->X;
            y = p_panel->Y;
            if (is_darkened)
                draw_new_panel_sprite_dark(x, y, p_panel->Spr);
            else
                draw_new_panel_sprite_std(x, y, p_panel->Spr);

            // Fill additional info on top of the sprites
            switch (p_panel->Type)
            {
            case PanT_AgentBadge:
                draw_new_panel_badge_overlay(i, p_panel->ID, is_darkened);
                break;
            case PanT_AgentMedi:
                // Medi sprite gets switched when we have medikit, so no need for update
                break;
            case PanT_AgentWeapon:
                panel_wep[p_panel->ID] |= 0x01;
                if (is_darkened)
                    panel_wep[p_panel->ID] |= 0x02;
                break;
            }
        }
    }

    // Blink the number of active agent
    if (gameturn & 4)
    {
        short x, y;
        short dcthing;
        struct Thing *p_agent;

        dcthing = direct_control_thing_for_player(local_player_no);
        p_agent = &things[dcthing];
        if ((p_agent->Flag & TngF_Destroyed) == 0 && !person_is_executing_commands(p_agent->ThingOffset))
        {
            ushort plagent;

            plagent = p_agent->U.UPerson.ComCur & 3;
            x = game_panel[0 + plagent].X;
            y = game_panel[0 + plagent].Y;
            x += game_panel_shifts[PaSh_AGENT_PANEL_TO_NUMBER + plagent].x;
            y += game_panel_shifts[PaSh_AGENT_PANEL_TO_NUMBER + plagent].y;
            draw_new_panel_sprite_std(x, y, 6 + plagent);
        }
    }
    lbDisplay.DrawFlags = 0;

    draw_panel_pickable_item();

    if (!func_1caf8(panel_wep))
    {
        if (ingame.Flags & GamF_Unkn0200) {
            ulong md, y;
            md = p_locplayer->UserInput[0].ControlMode & 0x1FFF;
            if (md == 1 && pktrec_mode != PktR_PLAYBACK) {
                y = alt_at_point(mouse_map_x, mouse_map_z);
                func_702c0(mouse_map_x, PRCCOORD_TO_YCOORD(y), mouse_map_z, 64, 64, colour_lookup[ColLU_RED]);
            }
        }
    }
    draw_agent_grouping_bars();

    // Fill the left energy bar
    {
        short dcthing;
        struct Thing *p_agent;
        int lv, lvmax, col, w;

        dcthing = direct_control_thing_for_player(local_player_no);
        p_agent = &things[dcthing];
        if ((p_agent->U.UPerson.Energy < 50) && (gameturn & 1))
            col = 2;
        else
            col = 1;
        lvmax = p_agent->U.UPerson.MaxEnergy;
        lv = p_agent->U.UPerson.Energy;
        if (lbDisplay.GraphicsScreenHeight < 400)
            w = 0;
        else
            w = 45;
        func_1eae4(3, 30, 4, 54+w, lv, lvmax, colour_lookup[col], 0);
    }

    ingame.Scanner.MX = engn_xc >> 7;
    ingame.Scanner.MZ = engn_zc >> 7;
    ingame.Scanner.Angle = 2047 - ((engn_anglexz >> 5) & 0x7FF);
    SCANNER_draw_new_transparent();

    draw_panel_objective_info();

    draw_panel_thermal_button();
}

TbBool process_panel_state_one_agent_weapon(ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;
    short pnitm;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];
    pnitm = p_locplayer->PanelItem[mouser];

    if (lbDisplay.RightButton)
    {
        // Right click while holding left weapon drop
        struct Thing *p_agent;

        lbDisplay.RightButton = 0;
        p_agent = p_locplayer->MyAgent[agent];

        if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
        {
            p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
            my_build_packet(p_pckt, PAct_DROP_HELD_WEAPON_SECR, p_agent->ThingOffset, pnitm, 0, 0);
            p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
            return true;
        }
    }
    if (!lbDisplay.MLeftButton)
    {
        struct Thing *p_agent;

        p_agent = p_locplayer->MyAgent[agent];

        if (lbDisplay.MRightButton)
        {
            // Hold left, hold right, release left weapon drop
            lbDisplay.RightButton = 0;
            if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
                my_build_packet(p_pckt, PAct_DROP_HELD_WEAPON_SECR, p_agent->ThingOffset, pnitm, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
                return true;
            }
        }

        // release button while in weapon selection mode
        {
            if ((p_agent != NULL) && (pnitm != 0))
            {
                my_build_packet(p_pckt, PAct_SELECT_SPECIFIC_WEAPON, p_agent->ThingOffset, pnitm, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
                lbDisplay.RightButton = 0;
                lbDisplay.LeftButton = 0;
                p_locplayer->UserInput[mouser].ControlMode &= ~(0x4000|0x8000);
                return true;
            }
        }
        p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
    }
    return false;
}

TbBool process_panel_state_grp_agents_weapon(ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;
    short pnitm;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];
    pnitm = p_locplayer->PanelItem[mouser];

    if (lbDisplay.LeftButton)
    {
        // Left click while holding right weapon drop
        struct Thing *p_agent;

        lbDisplay.LeftButton = 0;
        p_agent = p_locplayer->MyAgent[agent];
        if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
        {
            p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
            my_build_packet(p_pckt, PAct_DROP_HELD_WEAPON_SECR, p_agent->ThingOffset, pnitm, 0, 0);
            p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
            return true;
        }
    }
    if (!lbDisplay.MRightButton)
    {
        struct Thing *p_agent;

        p_agent = p_locplayer->MyAgent[agent];
        // release button while in weapon selection mode
        if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
        {
            my_build_packet(p_pckt, PAct_SELECT_GRP_SPEC_WEAPON, p_agent->ThingOffset, pnitm, 0, 0);
            p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
            lbDisplay.RightButton = 0;
            lbDisplay.LeftButton = 0;
            p_locplayer->UserInput[mouser].ControlMode &= ~(0x4000|0x8000);
            return true;
        }
        p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
    }
    return false;
}

/** Process panel in regard to a previously enter mood alteration state.
 *
 * @param main_panel Index of the main panel linked to the current state.
 * @param agent Agent index whose panel is currenly under mouse.
 * @param can_control Whether the agent currently under mouse can receive control commands.
 */
TbBool process_panel_state_one_agent_mood(ushort main_panel, ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];

    if (lbDisplay.MLeftButton)
    {
        // Left button hold mood control
        struct Thing *p_agent;
        short dcthing;
        short mood;
        TbBool can_control;

        p_agent = p_locplayer->MyAgent[agent];
        mood = panel_mouse_move_mood_value(main_panel);
        dcthing = p_locplayer->DirectControl[mouser];
        can_control = person_can_accept_control(dcthing);

        if ((p_agent->Type == TT_PERSON) && (can_control))
            build_packet(p_pckt, PAct_AGENT_SET_MOOD, p_agent->ThingOffset, mood, 0, 0);
        return true;
    }
    p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
    return false;
}

TbBool process_panel_state_grp_agents_mood(ushort main_panel, ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];

    if (lbDisplay.MRightButton)
    {
        // Right button hold mood control
        struct Thing *p_agent;
        short dcthing;
        short mood;
        TbBool can_control;

        p_agent = p_locplayer->MyAgent[agent];
        mood = panel_mouse_move_mood_value(main_panel);
        dcthing = p_locplayer->DirectControl[mouser];
        can_control = person_can_accept_control(dcthing);

        if ((p_agent->Type == TT_PERSON) && (can_control))
            build_packet(p_pckt, PAct_GROUP_SET_MOOD, p_agent->ThingOffset, mood, 0, 0);
        return true;
    }
    p_locplayer->UserInput[mouser].ControlMode &= ~0xC000;
    p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;

    return false;
}

TbBool process_panel_state(void)
{
    PlayerInfo *p_locplayer;
    ubyte pnsta;

    p_locplayer = &players[local_player_no];
    pnsta = p_locplayer->PanelState[mouser];

    if ((ingame.Flags & GamF_Unkn00100000) != 0)
    {
        if ((pnsta < PANEL_STATE_WEP_SEL_GRP + 4) || (pnsta >= PANEL_STATE_SEND_MESSAGE))
        {
            while (IsSamplePlaying(0, 21, 0))
                stop_sample_using_heap(0, 21);
            ingame.Flags &= ~GamF_Unkn00100000;
        }
    }

    if ((pnsta >= PANEL_STATE_WEP_SEL_ONE) && (pnsta < PANEL_STATE_WEP_SEL_ONE + 4))
    {
        if (process_panel_state_one_agent_weapon((pnsta - PANEL_STATE_WEP_SEL_ONE) % 4))
            return 1;
    }
    else if ((pnsta >= PANEL_STATE_WEP_SEL_GRP) && (pnsta < PANEL_STATE_WEP_SEL_GRP + 4))
    {
        if (process_panel_state_grp_agents_weapon((pnsta - PANEL_STATE_WEP_SEL_GRP) % 4))
            return 1;
    }
    else if ((pnsta >= PANEL_STATE_MOOD_SET_ONE) && (pnsta < PANEL_STATE_MOOD_SET_ONE + 4))
    {
        if (process_panel_state_one_agent_mood(pnsta - 5, (pnsta - PANEL_STATE_MOOD_SET_ONE) % 4))
            return 1;
    }
    else if ((pnsta >= PANEL_STATE_MOOD_SET_GRP) && (pnsta < PANEL_STATE_MOOD_SET_GRP + 4))
    {
        if (process_panel_state_grp_agents_mood(pnsta - 9, (pnsta - PANEL_STATE_MOOD_SET_GRP) % 4))
            return 1;
    }
    else if (pnsta == PANEL_STATE_SEND_MESSAGE)
    {
        struct Packet *p_pckt;
        ushort i;

        p_pckt = &packets[local_player_no];
        i = next_buffered_key();
        if (i != 0)
        {
            if (lbShift & 1)
                i |= 0x0100;
            my_build_packet(p_pckt, PAct_CHAT_MESSAGE_KEY, i, 0, 0, 0);
            return 1;
        }
    }
    return 0;
}

TbBool check_panel_input(short panel)
{
    PlayerInfo *p_locplayer;
    int i;

    p_locplayer = &players[local_player_no];

    if (lbDisplay.LeftButton)
    {
        struct Packet *p_pckt;
        struct Thing *p_agent;
        struct GamePanel *p_panel;
        short dcthing;

        lbDisplay.LeftButton = 0;
        p_panel = &game_panel[panel];
        p_pckt = &packets[local_player_no];

        switch (p_panel->Type)
        {
        case PanT_AgentBadge:
            // Select controlled agent
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type != TT_PERSON) || ((p_agent->Flag & TngF_Destroyed) != 0) || ((p_agent->Flag2 & TgF2_KnockedOut) != 0))
                return 0;
            if (p_locplayer->DoubleMode) {
                byte_153198 = p_panel->ID + 1;
            } else {
                dcthing = p_locplayer->DirectControl[0];
                build_packet(p_pckt, PAct_SELECT_AGENT, dcthing, p_agent->ThingOffset, 0, 0);
                p_locplayer->UserInput[0].ControlMode |= 0x8000;
            }
            return 1;
        case PanT_AgentMood:
            // Change mood / drugs level
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && (p_agent->State != PerSt_DEAD))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
                i = panel_mouse_move_mood_value(panel);
                if (panel_active_based_on_target(panel))
                    my_build_packet(p_pckt, PAct_AGENT_SET_MOOD, p_agent->ThingOffset, i, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_MOOD_SET_ONE + p_panel->ID;
                if (!IsSamplePlaying(0, 21, 0))
                    play_sample_using_heap(0, 21, 127, 64, 100, -1, 1u);
                ingame.Flags |= GamF_Unkn00100000;
                return 1;
            }
            break;
        case PanT_AgentWeapon:
            // Weapon selection for single agent
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && person_can_accept_control(p_agent->ThingOffset))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
                p_locplayer->PanelState[mouser] = PANEL_STATE_WEP_SEL_ONE + p_panel->ID;
                return 1;
            }
            break;
        case PanT_AgentMedi:
            // Use medikit
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && person_carries_any_medikit(p_agent))
            {
                my_build_packet(p_pckt, PAct_AGENT_USE_MEDIKIT, p_agent->ThingOffset, 0, 0, 0);
                return 1;
            }
            break;
        case PanT_UNKN08:
            // Enable supershield
            if (p_locplayer->DoubleMode && byte_153198 - 1 != mouser)
                break;
            if (p_locplayer->DoubleMode)
                break;
            dcthing = p_locplayer->DirectControl[mouser];
            if ((things[dcthing].Flag & TngF_Destroyed) != 0)
                break;
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if (p_agent->Type != TT_PERSON)
                break;
            build_packet(p_pckt, PAct_SHIELD_TOGGLE, dcthing, p_agent->ThingOffset, 0, 0);
            p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
            return 1;
        case PanT_UNKN10:
            if (mouse_over_infrared_slant_box(panel))
            {
                // Toggle thermal view
                if ((ingame.Flags & GamF_ThermalView) == 0)
                {
                    dcthing = p_locplayer->DirectControl[mouser];
                    if (things[dcthing].U.UPerson.Energy > 100)
                    {
                        ingame.Flags |= GamF_ThermalView;
                        play_sample_using_heap(0, 35, 127, 64, 100, 0, 1);
                        ingame_palette_reload();
                    }
                }
                else
                {
                    ingame.Flags &= ~GamF_ThermalView;
                    change_brightness(0);
                }
            }
            else
            {
                // Increase agent grouping
                dcthing = p_locplayer->DirectControl[mouser];
                p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
                if (panel_active_based_on_target(panel))
                    my_build_packet(p_pckt, PAct_PROTECT_INC, dcthing, 0, 0, 0);
            }
            return 1;
        default:
            break;
        }
    }

    if (lbDisplay.RightButton)
    {
        struct Packet *p_pckt;
        struct Thing *p_agent;
        struct GamePanel *p_panel;

        lbDisplay.RightButton = 0;
        p_panel = &game_panel[panel];
        p_pckt = &packets[local_player_no];

        switch (p_panel->Type)
        {
        case PanT_AgentMood:
            // Change mood / drugs level
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && (p_agent->State != PerSt_DEAD))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
                i = panel_mouse_move_mood_value(panel);
                if (panel_active_based_on_target(panel))
                    my_build_packet(p_pckt, PAct_GROUP_SET_MOOD, p_agent->ThingOffset, i, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_MOOD_SET_GRP + p_panel->ID;
                if (!IsSamplePlaying(0, 21, 0))
                    play_sample_using_heap(0, 21, 127, 64, 100, -1, 1u);
                ingame.Flags |= GamF_Unkn00100000;
                return 1;
            }
            break;
        case PanT_AgentWeapon:
            // Weapon selection for all grouped agent
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && person_can_accept_control(p_agent->ThingOffset))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
                p_locplayer->PanelState[mouser] = PANEL_STATE_WEP_SEL_GRP + p_panel->ID;
                return 1;
            }
            break;
        case PanT_UNKN10:
            // Switch grouping fully on or fully off
            p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
            if (panel_active_based_on_target(panel))
            {
                short dcthing;
                dcthing = p_locplayer->DirectControl[mouser];
                my_build_packet(p_pckt, PAct_PROTECT_TOGGLE, dcthing, 0, 0, 0);
            }
            return 1;
        default:
            break;
        }
    }

    if (lbDisplay.MRightButton)
    {
        struct Packet *p_pckt;
        struct Thing *p_agent;
        struct GamePanel *p_panel;

        p_panel = &game_panel[panel];
        p_pckt = &packets[local_player_no];

        switch (p_panel->Type)
        {
        case PanT_AgentBadge:
            // Center view on the selected agent
            if (!p_locplayer->DoubleMode)
            {
                p_agent = p_locplayer->MyAgent[p_panel->ID];
                if ((p_agent->Type == TT_PERSON) && ((p_agent->Flag & TngF_Destroyed) == 0))
                {
                    ushort dcthing;

                    dcthing = p_locplayer->DirectControl[mouser];
                    if ((things[dcthing].Flag & TngF_Unkn0400) == 0)
                    {
                        game_set_cam_track_thing_xz(p_agent->ThingOffset);
                        engn_yc = PRCCOORD_TO_MAPCOORD(p_agent->Y);
                        build_packet(p_pckt, PAct_SELECT_AGENT, dcthing, p_agent->ThingOffset, 0, 0);
                        if (p_agent->ThingOffset == dcthing)
                        {
                            engn_xc = PRCCOORD_TO_MAPCOORD(p_agent->X);
                            engn_zc = PRCCOORD_TO_MAPCOORD(p_agent->Z);
                        }
                        return 1;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return 0;
}

TbBool check_panel_button(void)
{
    short panel, tot_panels;

    if (lbDisplay.LeftButton && lbDisplay.RightButton)
    {
        struct Packet *p_pckt;
        PlayerInfo *p_locplayer;
        short dcthing;

        lbDisplay.LeftButton = 0;
        lbDisplay.RightButton = 0;
        p_locplayer = &players[local_player_no];
        p_pckt = &packets[local_player_no];
        dcthing = p_locplayer->DirectControl[mouser];
        my_build_packet(p_pckt, PAct_PEEPS_SCATTER, dcthing,
            mouse_map_x, 0, mouse_map_z);
        return 1;
    }

    if (mouse_move_over_scanner())
    {
        if (check_scanner_input())
            return 1;
    }

    tot_panels = lbDisplay.GraphicsScreenHeight < 400 ? 17 : 18;
    for (panel = tot_panels; panel >= 0; panel--)
    {
        if (mouse_move_over_panel(panel))
        {
            if (check_panel_input(panel))
                return 1;
        }
    }
    return 0;
}

/******************************************************************************/
