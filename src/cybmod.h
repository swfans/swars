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
#include "bfini.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;

enum CyModType
{
  MOD_NULL = 0x0,
  MOD_TYP01 = 0x1,
  MOD_TYP02 = 0x2,
  MOD_TYP03 = 0x3,
  MOD_TYP04 = 0x4,
  MOD_TYP05 = 0x5,
  MOD_TYP06 = 0x6,
  MOD_TYP07 = 0x7,
  MOD_TYP08 = 0x8,
  MOD_TYP09 = 0x9,
  MOD_TYP10 = 0xA,
  MOD_TYP11 = 0xB,
  MOD_TYP12 = 0xC,
  MOD_EPIDERM1 = 0xD,
  MOD_EPIDERM2 = 0xE,
  MOD_EPIDERM3 = 0xF,
  MOD_EPIDERM4 = 0x10,
  MOD_TYPES_COUNT,
};


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

/* Additional ModDef fields, to be later merged with that struct.
 */
struct ModDefAdd {
  char Name[12];
};

/** Person Modifications.
 */
union Mod
{
  ushort Mods;
};

#pragma pack()
/******************************************************************************/
extern struct ModDef mod_defs[];
extern struct TbNamedEnum mod_names[33];
extern ubyte mod_tech_level[17];

void read_cybmods_conf_file(void);

ubyte cybmod_chest_level(union Mod *p_umod);
void set_cybmod_chest_level(union Mod *p_umod, ubyte nmod);
ubyte cybmod_legs_level(union Mod *p_umod);
void set_cybmod_legs_level(union Mod *p_umod, ubyte nmod);
ubyte cybmod_arms_level(union Mod *p_umod);
void set_cybmod_arms_level(union Mod *p_umod, ubyte nmod);
ubyte cybmod_brain_level(union Mod *p_umod);
void set_cybmod_brain_level(union Mod *p_umod, ubyte nmod);
ubyte cybmod_skin_level(union Mod *p_umod);
void set_cybmod_skin_level(union Mod *p_umod, ubyte nmod);

ushort cybmodflags_prev_mod(ulong modflags, ushort last_mtype);
ushort cybmod_group_type(ushort mtype);
ushort cybmod_version(ushort mtype);

void sanitize_cybmods_fmtver11_flags(union Mod *p_umod);
TbBool cybmod_fix_all(union Mod *p_umod);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
