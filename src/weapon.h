/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file weapon.h
 *     Header file for weapon.c.
 * @par Purpose:
 *     Weapons related functions.
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
#ifndef WEAPON_H
#define WEAPON_H

#include "bftypes.h"
#include "bfini.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum WeaponType
{
  WEP_NULL = 0x0,
  WEP_UZI = 0x1,
  WEP_MINIGUN = 0x2,
  WEP_LASER = 0x3,
  WEP_ELLASER = 0x4,
  WEP_RAP = 0x5,
  WEP_NUCLGREN = 0x6,
  WEP_PERSUADRTRN = 0x7,
  WEP_FLAMER = 0x8,
  WEP_H2HTASER = 0x9,
  WEP_CRAZYGAS = 0xA,
  WEP_KOGAS = 0xB,
  WEP_ELEMINE = 0xC,
  WEP_EXPLMINE = 0xD,
  WEP_NAPALMMINE = 0xE,
  WEP_LONGRANGE = 0xF,
  WEP_AIRSTRIKE = 0x10,
  WEP_BEAM = 0x11,
  WEP_RAZORWIRE = 0x12,
  WEP_SONICBLAST = 0x13,
  WEP_QDEVASTATOR = 0x14,
  WEP_PERSUADER2 = 0x15,
  WEP_STASISFLD = 0x16,
  WEP_ENERGYSHLD = 0x17,
  WEP_SOULGUN = 0x18,
  WEP_TIMEGUN = 0x19,
  WEP_CEREBUSIFF = 0x1A,
  WEP_MEDI1 = 0x1B,
  WEP_MEDI2 = 0x1C,
  WEP_EXPLWIRE = 0x1D,
  WEP_CLONESHLD = 0x1E,
  WEP_UNUSED1F = 0x1F,
  WEP_TYPES_COUNT,
};

enum WeaponFourPack
{
    WFRPK_ELEMINE = 0,
    WFRPK_EXPLMINE,
    WFRPK_NUCLGREN,
    WFRPK_CRAZYGAS,
    WFRPK_KOGAS,
    WFRPK_COUNT,
};


struct Thing;

struct WeaponDef {
  ushort RangeBlocks;
  ushort HitDamage;
  ushort ReFireDelay;
  ushort Weight;
  short EnergyUsed;
  ushort Accuracy;
  ushort TargetType;
  ushort TargetMode;
  ushort Flags;
  ushort Sprite;
  ushort Cost;
  ushort Funding;
  ubyte PercentPerDay;
};

/* Additional WeaponDef fields, to be later merged with that struct.
 */
struct WeaponDefAdd {
  char Name[12];
};

#pragma pack()
/******************************************************************************/
extern struct WeaponDef weapon_defs[33];
extern struct TbNamedEnum weapon_names[33];
extern ubyte weapon_tech_level[33];

void read_weapons_conf_file(void);

void do_weapon_quantities_net_to_player(struct Thing *p_person);
void do_weapon_quantities1(struct Thing *p_person);
void do_weapon_quantities_proper1(struct Thing *p_person);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
