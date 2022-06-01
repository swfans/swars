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

struct ResearchInfo
{
    ushort WeaponProgress[10][32];
    ushort ModProgress[10][32];
    ubyte WeaponDaysDone[32];
    ubyte ModDaysDone[32];
    long WeaponFunding;
    long ModFunding;
    char Scientists;
    char NumBases;
    char CurrentWeapon;
    char CurrentMod;
    long WeaponsAllowed;
    long ModsAllowed;
    long WeaponsCompleted;
    long ModsCompleted;
};

#pragma pack()
/******************************************************************************/
extern struct ResearchInfo research;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
