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
#define MISSIONS_MAX_COUNT 120

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

enum MissionExtraRewards {
    MEReward_None = 0,
    MEReward_Scientists,
    MEReward_ResearchLab,
    MEReward_CybModResearched,
    MEReward_WeaponResearched,
    MEReward_CybModSingle,
    MEReward_WeaponSingle,
};

struct Campaign {
    /** Default campaign title. */
    const char *TextName;
    /** Translated campaign title text ID. */
    u16 TextId;
    /** Index of first mission in the campaign. */
    u16 FirstTrigger;
    /** Translated text ID for netscan button. */
    u16 NetscanTextId;
    /** Shift for translated text IDs for all weapon names. */
    u16 WeaponsTextIdShift;
    ulong ResearchWeapons;
    ulong StandardWeapons;
    ulong ResearchMods;
    ulong StandardMods;
    /** Char marking text translations file names for the campaign. */
    const char *TextFnMk;
    /** Char marking projector file names for the campaign. */
    const char *ProjectorFnMk;
    /** Outro movie file name. */
    const char *OutroFMV;
    /** Outro background after the FMV ends. */
    const char *OutroBkFn;
    /** Flag switches for the campaign. */
    u16 Flags;
};

struct Mission { // sizeof=76
    /** Mission code name from developers. */
    char *TextName;
    /** Translated mission title text ID. */
    u16 TextId;
    u16 StartHead;
    u16 SpecialEffectID;
    /** Head of mission objective chain which indicates success in achieving goals. */
    u16 SuccessHead;
    /** Head of mission objective chain which indicates failure to achieve goals. */
    u16 FailHead;
    u16 SourceID;
    u16 SuccessID;
    u16 FailID;
    /** Special triggers; replaced by load game. */
    u8 SpecialTrigger[3];
    u8 SuccessTrigger[3];
    u8 FailTrigger[3];
    /** Objective priority number reached for chain within SuccessHead. */
    u8 BankTest;
    u16 SpecialEffectFailID;
    u16 SpecialEffectSuccessID;
    u16 StringIndex;
    /** Index of first of netscan objectives (mission brief objectives) assigned to this mission. */
    u16 NetscanObvIndex;
    /** Amount of netscan objectives (mission brief objectives) assigned to this mission. */
    u8 NetscanObvCount;
    /** Weapons made researchable by completing this mission. */
    ulong ResearchWeapons;
    /** Type of an extra, unique reward given to the player for mission complete. */
    u8 ExtraRewardType;
    /** Parameter for the extra unique reward, meaning depends on its type. */
    u16 ExtraRewardParam;
    u8 SuccessLevel[2];
    u8 FailMap[3];
    u8 FailLevel[3];
    /** Map number on which this mission takes place. */
    u8 MapNo;
    /** Level number identifying level file to play. */
    u8 LevelNo;
    /** Objective priority number reached for chain within FailHead. */
    u8 BankTestFail;
    /** Whether the mission was completed by the current player; replaced by load game. */
    i8 Complete;
    u8 MissionCond[5];
    /** Level number when replaying the mission, if it's different than the first LevelNo (ie. skips cutscene). */
    u8 ReLevelNo;
    /** Lump sum cash reward for mission complete, added to the rewards from briefcases and persuasion. */
    u16 CashReward;
    u8 PANStart;
    u8 PANEnd;
    /** When fade to black is expected at end of mission, sets how many game turns that will take. */
    u16 WaitToFade;
    /** Amount of game turns to blindly process before the level starts. */
	u16 PreProcess;
    /** Flag switches for the mission. */
    u16 Flags;
    u8 field_4A;
    u8 field_4B;
};

#pragma pack()
/******************************************************************************/
extern struct Campaign campaigns[CAMPAIGNS_MAX_COUNT];
extern struct Mission mission_list[MISSIONS_MAX_COUNT];
extern char mission_name[50];

extern char *netscan_text;
#define netscan_text_len 16384

void load_campaigns(void);
u16 selectable_campaigns_count(void);

u16 find_mission_state_slot(u16 missi);
u16 find_empty_mission_state_slot(void);
void remove_mission_state_slot(u16 mslot);
void init_mission_states(void);

u16 find_mission_with_map_and_level(u16 mapno, u16 level);
void fix_mission_used_objectives(i16 missi);

TbBool mission_remain_until_success(u16 missi);
TbBool mission_has_immediate_next_on_success(u16 missi);
TbBool mission_has_immediate_previous(u16 missi);
TbBool mission_is_final_at_game_end(u16 missi);

TbBool read_missions_conf_info(int num);
void load_missions(int num);
void save_missions_conf_file(int num);
void read_missions_conf_file(int num);
void read_missions_bin_file(int num);
void read_mission_netscan_objectives_bin(void);
void apply_missions_fixups(void);

TbResult load_netscan_text_data(u16 mapno, u16 level);
TbResult load_mission_name_text(u8 missi);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
