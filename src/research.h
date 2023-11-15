/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file research.h
 *     Header file for research.c.
 * @par Purpose:
 *     Research state and progress support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef RESEARCH_H
#define RESEARCH_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define RESEARCH_COMPLETE_POINTS 25600
#define RESEARCH_DAYS_STORED 10

struct ResearchInfo
{
    ushort WeaponProgress[32][RESEARCH_DAYS_STORED];
    ushort ModProgress[32][RESEARCH_DAYS_STORED];
    u8 WeaponDaysDone[32];
    u8 ModDaysDone[32];
    i32 WeaponFunding;
    i32 ModFunding;
    i8 Scientists;
    char NumBases;
    /** Currently researched weapon index, weapon type - 1 */
    i8 CurrentWeapon;
    i8 CurrentMod;
    u32 WeaponsAllowed;
    u32 ModsAllowed;
    u32 WeaponsCompleted;
    u32 ModsCompleted;
};

#pragma pack()
/******************************************************************************/
extern struct ResearchInfo research;
extern char scientist_lost_reason[60];

void load_scientist_lost_reason(ushort reason_no);

int research_daily_progress_for_type(u8 rstype);
int research_unkn_func_004(ushort percent_per_day, int expect_funding, int real_funding);

TbBool is_research_weapon_completed(ushort wtype);
TbBool is_research_weapon_allowed(ushort wtype);
void research_weapon_allow(ushort wtype);
void research_weapon_complete(ushort wtype);
void research_weapon_flags_allow(u32 wpflags);

TbBool is_research_cymod_completed(ushort mtype);
TbBool is_research_cymod_allowed(ushort mtype);
void research_cymod_complete(ushort mtype);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
