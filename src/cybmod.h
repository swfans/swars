/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file cybmod.h
 *     Header file for cybmod.c.
 * @par Purpose:
 *     Agent mods related functions.
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
#ifndef CYBMOD_H
#define CYBMOD_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;

struct ModDef {
	ushort PowerOutput;
	ushort Resilience;
	ushort Funding;
	ushort Flags;
    char field_8[10];
    short Sprite;
	ushort Cost;
	uchar PercentPerDay;
	uchar Pad1;
};

#pragma pack()
/******************************************************************************/
extern struct ModDef mod_defs[];

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
