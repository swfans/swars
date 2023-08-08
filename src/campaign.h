/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file campaign.h
 *     Header file for campaign.c.
 * @par Purpose:
 *     Campaign progress and missions definitions.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define CAMPAIGNS_MAX_COUNT 6

/* deprecated - when callbacks are defined within objectv_defs[],
 * there will be no need to reference specific objective.
 */
enum GameObjectiveTypes {
    GAME_OBJ_NONE = 0,
    GAME_OBJ_P_DEAD,
    GAME_OBJ_ALL_G_DEAD,
    GAME_OBJ_MEM_G_DEAD,
    GAME_OBJ_P_NEAR,
    GAME_OBJ_MEM_G_NEAR,
    GAME_OBJ_P_ARRIVES,
    GAME_OBJ_MEM_G_ARRIVES,
    GAME_OBJ_ALL_G_ARRIVES,
    GAME_OBJ_PERSUADE_P,
    GAME_OBJ_PERSUADE_MEM_G,
    GAME_OBJ_PERSUADE_ALL_G,
    GAME_OBJ_TIME,
    GAME_OBJ_GET_ITEM,
    GAME_OBJ_USE_ITEM,
    GAME_OBJ_FUNDS,
    GAME_OBJ_DESTROY_OBJECT,
    GAME_OBJ_PKILL_P,
    GAME_OBJ_PKILL_G,
    GAME_OBJ_PKILL_ALL_G,
    GAME_OBJ_USE_PANET,
    GAME_OBJ_UNUSED_21,
    GAME_OBJ_PROTECT_G,
    GAME_OBJ_P_PERS_G,
    GAME_OBJ_ALL_G_USE_V,
    GAME_OBJ_UNUSED_25,
    GAME_OBJ_UNUSED_26,
};

enum CampaignFlags {
    CmpgF_IsSinglePlayer = 0x0001,
    CmpgF_IsSelectable   = 0x0002,
};

enum MissionFlags {
    /** Set if after this mission ends with success, a next one should start
     * immediatelly without going through menu. It is not possible to save
     * game or reequip your agents before the next mission starts.
     */
    MisF_ImmediateNextOnSuccess = 0x0001,
    /** Set if the current mission in set as SuccessTrigger in a mission
     * which has ImmediateNextOnSuccess flag set. This flag has no config file
     * mnemonic - it is set automatically for all missions pointed at by
     * other missions with expected flags. If this flag is set, the mission
     * will not have its own briefing slot, as it should replace values
     * within slot of the previous mission.
     */
    MisF_ImmediatePrevious = 0x0002,
    /** Set if failing this mission does not cause game over nor the mission
     * disappearing from selection. The mission can be played any amount
     * of times, and disappears only if it is finally completed.
     */
    MisF_RemainUntilSuccess = 0x0004,
    /** Set if the current mission it the final one in the campaign. After
     * the final mission is completed, outro sequence will play.
     */
    MisF_IsFinalMission = 0x0008,
};

struct Campaign {
    /** Default campaign title. */
    const char *TextName;
    /** Translated campaign title text ID. */
    ushort TextId;
    /** Index of first mission in the campaign. */
    ushort FirstTrigger;
    /** Translated text ID for netscan button. */
    ushort NetscanTextId;
    /** Shift for translated text IDs for all weapon names. */
    ushort WeaponsTextIdShift;
    ulong ResearchWeapons;
    ulong StandardWeapons;
    ulong ResearchMods;
    ulong StandardMods;
    /** Char marking projector file names for the campaign. */
    const char *ProjectorFnMk;
    /** Outro movie file name. */
    const char *OutroFMV;
    /** Outro background after the FMV ends. */
    const char *OutroBkFn;
    /** Flag switches for the campaign. */
    ushort Flags;
};

struct Mission { // sizeof=76
    /** Mission code name from developers. */
    char *TextName;
    /** Translated mission title text ID. */
    ushort TextId;
    ushort StartHead;
    ushort SpecialEffectID;
    /** Head of mission objective chain which indicates success in achieving goals. */
    ushort SuccessHead;
    /** Head of mission objective chain which indicates failure to achieve goals. */
    ushort FailHead;
    ushort SourceID;
    ushort SuccessID;
    ushort FailID;
    /** Special triggers; replaced by load game. */
    ubyte SpecialTrigger[3];
    ubyte SuccessTrigger[3];
    ubyte FailTrigger[3];
    /** Objective priority number reached for chain within SuccessHead. */
    ubyte BankTest;
    ushort SpecialEffectFailID;
    ushort SpecialEffectSuccessID;
    ushort StringIndex;
    ubyte StartMap[3];
    ubyte StartLevel[3];
    ubyte SuccessMap[3];
    ubyte SuccessLevel[3];
    ubyte FailMap[3];
    ubyte FailLevel[3];
    /** Map number on which this mission takes place. */
    ubyte MapNo;
    /** Level number identifying level file to play. */
    ubyte LevelNo;
    /** Objective priority number reached for chain within FailHead. */
    ubyte BankTestFail;
    /** Whether the mission was completed by the current player; replaced by load game. */
    sbyte Complete;
    ubyte MissionCond[5];
    /** Level number when replaying the mission, if it's different than the first LevelNo (ie. skips cutscene). */
    ubyte ReLevelNo;
    /** Lump sum cash reward for mission complete, added to the rewards from briefcases and persuasion. */
    ushort CashReward;
    ubyte PANStart;
    ubyte PANEnd;
    /** When fade to black is expected at end of mission, sets how many game turns that will take. */
    ushort WaitToFade;
    /** Amount of game turns to blindly process before the level starts. */
	ushort PreProcess;
    /** Flag switches for the mission. */
    ushort Flags;
    ubyte field_4A;
    ubyte field_4B;
};

struct Objective { // sizeof=32
    ushort Next;
    ubyte Map;
    ubyte Level;
    ushort Status;
    ushort Type;
    ushort Flags;
    short Thing;
    short X;
    short Y;
    short Z;
    short Radius;
    ubyte Pri;
    ubyte Arg2;
    ushort StringIndex;
    ushort UniqueID;
    ubyte ObjText;
    ubyte field_1B[4];
    ubyte field_1F;
};

#pragma pack()
/******************************************************************************/
extern struct Campaign campaigns[CAMPAIGNS_MAX_COUNT];
extern struct Objective *game_used_objectives;
extern ushort next_used_objective; // = 1;
extern struct Objective *game_objectives;
extern ushort next_objective;
extern struct Mission mission_list[120];

void load_campaigns(void);
ushort selectable_campaigns_count(void);

ushort find_mission_state_slot(ushort missi);
ushort find_empty_mission_state_slot(void);
void remove_mission_state_slot(ushort mslot);
void init_mission_states(void);

TbBool read_missions_conf_info(int num);
void load_missions(int num);
void save_missions_conf_file(int num);
void read_missions_conf_file(int num);
void read_missions_bin_file(int num);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
