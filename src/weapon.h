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

#define WEAPONS_CARRIED_MAX_COUNT 6
#define WEP_CATEGORIES_COUNT 11

/** Range of the weapon chan never exceed this limit.
 */
#define WEAPON_RANGE_BLOCKS_LIMIT 27

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

enum WeaponFourPackIndex
{
    WFRPK_ELEMINE = 0,
    WFRPK_EXPLMINE,
    WFRPK_NUCLGREN,
    WFRPK_CRAZYGAS,
    WFRPK_KOGAS,
    WFRPK_COUNT,
};

struct Thing;

enum WeaponDefFlags
{
    WEPDFLG_None = 0, /**< No flags set. */
    WEPDFLG_UNKN0001 = 0x0001, /**< set for LASER/ELLASER. */
    WEPDFLG_UNKN0002 = 0x0002, /**< set for LASER/ELLASER. */
    WEPDFLG_UNKN0004 = 0x0004,
    WEPDFLG_UNKN0008 = 0x0008,
    WEPDFLG_CanPurchease = 0x0010, /**< Whether the weapon can be bought by player (when researched). */
};

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
  ubyte Sprite; // TODO would be better to store sprite index in 16-bit var
  ubyte Category;
  ushort Cost;
  ushort Funding;
  ubyte PercentPerDay;
};

/* Additional WeaponDef fields, to be later merged with that struct.
 */
struct WeaponDefAdd {
  char Name[12];
};

struct WeaponsFourPack {
  ubyte Amount[WFRPK_COUNT];
};

#pragma pack()
/******************************************************************************/
extern struct WeaponDef weapon_defs[33];
extern struct TbNamedEnum weapon_names[33];
extern ubyte weapon_tech_level[33];
extern short persuaded_person_weapons_sell_cost_permil;

void read_weapons_conf_file(void);
void init_weapon_text(void);

TbBool weapons_has_weapon(ulong weapons, ushort wtype);
ushort weapons_prev_weapon(ulong weapons, ushort last_wtype);
const char *weapon_codename(ushort wtype);
ushort weapon_sprite_index(ushort wtype, TbBool enabled);

ushort weapon_fourpack_index(ushort weapon);
void weapons_remove_weapon(ulong *p_weapons,
  struct WeaponsFourPack *p_fourpacks, ushort wtype);
TbBool weapons_remove_one(ulong *p_weapons,
  struct WeaponsFourPack *p_fourpacks, ushort wtype);
TbBool weapons_add_one(ulong *p_weapons,
  struct WeaponsFourPack *p_fourpacks, ushort wtype);
void sanitize_weapon_quantities(ulong *p_weapons,
  struct WeaponsFourPack *p_fourpacks);

sbyte find_nth_weapon_held(ushort index, ubyte n);

/** Returns range (in normal map coord points) of the person current weapon.
 *
 * Simply gets the value from weapon config, without adjustments.
 */
short current_weapon_range(struct Thing *p_person);

/** Returns range (in normal map coord points) of the person current weapon.
 *
 * Implements modifications for vehicle weapons and throwing weapons.
 */
int get_weapon_range(struct Thing *p_person);

/** Returns whether the person current weapon requires some time to achieve target lock.
 */
TbBool current_weapon_has_targetting(struct Thing *p_person);

ulong person_carried_weapons_pesuaded_sell_value(struct Thing *p_person);

void do_weapon_quantities_net_to_player(struct Thing *p_person);
void do_weapon_quantities1(struct Thing *p_person);
void do_weapon_quantities_proper1(struct Thing *p_person);

void process_weapon(struct Thing *p_person);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
