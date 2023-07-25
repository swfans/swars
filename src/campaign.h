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

struct Mission { // sizeof=76
    char *TextName;
    /** Translated mission title text ID. */
    ushort TextId;
    ushort StartHead;
    ushort SpecialEffectID;
    ushort SuccessHead;
    ushort FailHead;
    ushort SourceID;
    ushort SuccessID;
    ushort FailID;
    /** Special triggers; replaced by load game. */
    ubyte SpecialTrigger[3];
    ubyte SuccessTrigger[3];
    ubyte FailTrigger[3];
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
    ubyte field_48[3];
    ubyte field_4B;
};

#pragma pack()
/******************************************************************************/
extern struct Objective *game_used_objectives;
extern ushort next_used_objective; // = 1;
extern struct Objective *game_objectives;
extern ushort next_objective;
extern struct Mission mission_list[120];

void load_missions(int num);
void save_missions_conf(int num);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
