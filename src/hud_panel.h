/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file hud_panel.h
 *     Header file for hud_panel.c.
 * @par Purpose:
 *     Ingame Heads-Up Display panel draw.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef HUD_PANEL_H
#define HUD_PANEL_H

#include "bftypes.h"
#include "game_bstype.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define PANEL_STATE_NORMAL        0
#define PANEL_STATE_WEP_SEL_ONE   1
#define PANEL_STATE_WEP_SEL_GRP   5
#define PANEL_STATE_MOOD_SET_ONE  9
#define PANEL_STATE_MOOD_SET_GRP 13
#define PANEL_STATE_SEND_MESSAGE 17

enum PanelType {
    PanT_NONE = 0,
    PanT_AgentBadge,
    PanT_AgentMood,
    PanT_AgentHealth,
    PanT_AgentEnergy,
    PanT_AgentWeapon,
    PanT_AgentMedi,
    PanT_UNKN07,
    PanT_WeaponEnergy,
    PanT_UNKN09,
    PanT_Grouping,
};

enum PanelShift {
    PaSh_AGENT_PANEL_TO_NUMBER = 0,
    PaSh_AGENT_WEAPON_TO_LIST = 4,
    PaSh_AGENT_HEALTH_PAD_BEF = 5,
    PaSh_AGENT_HEALTH_PAD_AFT = 6,
    PaSh_AGENT_MOOD_PAD_BEF = 7,
    PaSh_AGENT_MOOD_PAD_AFT = 8,
    PaSh_AGENT_ENERGY_PAD_BEF = 9,
    PaSh_AGENT_ENERGY_PAD_AFT = 10,
    PaSh_GROUP_PANE_TO_THERMAL_BOX = 11,
    PaSh_GROUP_PANE_TO_THERMAL_SPR = 12,
    PaSh_GROUP_PANE_AGENTS = 13,
    PaSh_WEP_CURR_BTN_TO_SYMBOL = 16,
    PaSh_WEP_FRST_BTN_TO_SYMBOL = 17,
    PaSh_WEP_NEXT_BTN_TO_SYMBOL = 18,
    PaSh_WEP_CURR_BTN_TO_DECOR =  19,
    PaSh_WEP_FRST_BTN_TO_DECOR = 20,
    PaSh_WEP_NEXT_BTN_TO_DECOR = 21,
    PaSh_WEP_NEXT_DISTANCE = 22,
    PaSh_WEP_CURR_BUTTON_AREA = 23,
    PaSh_WEP_NEXT_BUTTON_AREA = 24,
    PaSh_WEP_FOURPACK_BASE_SH = 25,
    PaSh_WEP_FOURPACK_SIZE = 26,
    PaSh_WEP_FOURPACK_SLOTS = 27,
};

struct GamePanel
{
  short X;
  short Y;
  short Spr;
  ushort Width;
  ushort Height;
  ushort Use;
  ushort Flags;
  ubyte ID;
  ubyte Type;
};

struct Thing;
struct TbPoint;

#pragma pack()
/******************************************************************************/
extern struct GamePanel *game_panel;
extern struct TbPoint *game_panel_shifts;
extern ubyte byte_153198;
extern long scanner_unkn370;

extern char player_unknCC9[8][128];
extern ubyte player_unkn0C9[8];
extern long scanner_unkn3CC;

/** Scale factor for figuring out GUI size.
 */
extern short gui_scale;

int SCANNER_objective_info_height(void);

void draw_new_panel(void);

TbResult load_pop_sprites_for_current_mode(void);

/** Returns if a game panel is active, considering the target which it controls.
 */
TbBool panel_active_based_on_target(short panel);

/** Returns if current mouse move position is over the given game panel.
 */
TbBool mouse_move_over_panel(short panel);

TbBool mouse_over_infrared_slant_box(short panel);

TbBool process_panel_state(void);
TbBool check_panel_button(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
