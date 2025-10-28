/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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

#include "game_bstype.h"

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
    ubyte WeaponDaysDone[32];
    ubyte ModDaysDone[32];
    long WeaponFunding;
    long ModFunding;
    sbyte Scientists;
    char NumBases;
    /** Currently researched weapon index, weapon type - 1 */
    sbyte CurrentWeapon;
    sbyte CurrentMod;
    ulong WeaponsAllowed;
    ulong ModsAllowed;
    ulong WeaponsCompleted;
    ulong ModsCompleted;
};

#pragma pack()
/******************************************************************************/
extern struct ResearchInfo research;
extern char scientist_lost_reason[60];
extern short daily_scientist_death_chance_permil;
extern short scientists_per_group;
extern short weapon_donate_research_incr_permil;
extern short research_progress_rtc_minutes;

void load_scientist_lost_reason(ushort reason_no, ubyte *scratch_buf);

int research_daily_progress_for_type(ubyte rstype);
int research_unkn_func_004(ushort percent_per_day, int expect_funding, int real_funding);
void research_unkn_func_003(void);

TbBool is_research_weapon_completed(WeaponType wtype);
TbBool is_research_weapon_allowed(WeaponType wtype);
void research_weapon_allow(WeaponType wtype);
void research_weapon_complete(WeaponType wtype);
void research_weapon_flags_allow(ulong wpflags);

TbBool is_research_cymod_completed(ushort mtype);
TbBool is_research_cymod_allowed(ushort mtype);
void research_cymod_complete(ushort mtype);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
