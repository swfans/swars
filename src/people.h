/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file people.h
 *     Header file for people.c.
 * @par Purpose:
 *     Routines implementing people.
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
#ifndef PEOPLE_H
#define PEOPLE_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum SubTypePerson {
  SubTT_PERS_NONE = 0x0,
  SubTT_PERS_AGENT = 0x1,
  SubTT_PERS_ZEALOT = 0x2,
  SubTT_PERS_PUNK_F = 0x3,
  SubTT_PERS_BRIEFCASE_M = 0x4,
  SubTT_PERS_WHITE_BRUN_F = 0x5,
  SubTT_PERS_MERCENARY = 0x6,
  SubTT_PERS_MECH_SPIDER = 0x7,
  SubTT_PERS_POLICE = 0x8,
  SubTT_PERS_PUNK_M = 0x9,
  SubTT_PERS_SCIENTIST = 0xA,
  SubTT_PERS_SHADY_M = 0xB,
  SubTT_PERS_HIGH_PRIEST = 0xC,
  SubTT_PERS_WHIT_BLOND_F = 0xD,
  SubTT_PERS_LETH_JACKT_M = 0xE,
  SubTT_PERS_FAST_BLOND_F = 0xF,
  SubTT_PERS_COUNT,
};

enum PersonState {
  PerSt_NONE = 0x0,
  PerSt_GOTO_POINT = 0x1,
  PerSt_BOMB_BUILDING = 0x2,
  PerSt_WANDER = 0x3,
  PerSt_EXPLODE_EFFECT = 0x4,
  PerSt_WAIT = 0x5,
  PerSt_AGENT_DEFENSIVE = 0x6,
  PerSt_PICKUP_ITEM = 0x7,
  PerSt_DROP_ITEM = 0x8,
  PerSt_SHOOT_PERSON_WITH_BGUN = 0x9,
  PerSt_SHOOT_BGUN_AT = 0xA,
  PerSt_DIE = 0xB,
  PerSt_DIEING = 0xC,
  PerSt_DEAD = 0xD,
  PerSt_INIT_SAVE_VICTIM = 0xE,
  PerSt_SAVE_VICTIM = 0xF,
  PerSt_GOTO_PERSON = 0x10,
  PerSt_WAIT_P_DEAD = 0x11,
  PerSt_WAIT_ALL_G_DEAD = 0x12,
  PerSt_WAIT_MEM_G_DEAD = 0x13,
  PerSt_WAIT_P_NEAR = 0x14,
  PerSt_WAIT_MEM_G_NEAR = 0x15,
  PerSt_WAIT_P_ARRIVES = 0x16,
  PerSt_WAIT_MEM_G_ARRIVES = 0x17,
  PerSt_WAIT_TIME = 0x18,
  PerSt_DEFINE_ROUTE = 0x19,
  PerSt_UNUSED_1A = 0x1A,
  PerSt_KILL_PERSON = 0x1B,
  PerSt_BLOCK_PERSON = 0x1C,
  PerSt_SCARE_PERSON = 0x1D,
  PerSt_USE_ITEM = 0x1E,
  PerSt_STATIONARY_SHOT = 0x1F,
  PerSt_RECOIL_HIT = 0x20,
  PerSt_VEH_PARKED = 0x21,
  PerSt_VEH_DRIVEN = 0x22,
  PerSt_GOTO_POINT_REL = 0x23,
  PerSt_PERSON_BURNING = 0x24,
  PerSt_IN_VEHICLE = 0x25,
  PerSt_DRIVING_VEHICLE = 0x26,
  PerSt_V_GOTO_POINT_FLY = 0x27,
  PerSt_PERSUADE_PERSON = 0x28,
  PerSt_FOLLOW_PERSON = 0x29,
  PerSt_SUPPORT_PERSON = 0x2A,
  PerSt_PROTECT_PERSON = 0x2B,
  PerSt_GET_ITEM = 0x2C,
  PerSt_USE_VEHICLE = 0x2D,
  PerSt_USE_VEHICLE_TO_POINT = 0x2E,
  PerSt_WAIT_VEHICLE = 0x2F,
  PerSt_GOTO_VEHICLE = 0x30,
  PerSt_CATCH_TRAIN = 0x31,
  PerSt_WAIT_TRAIN = 0x32,
  PerSt_DESTROY_BUILDING = 0x33,
  PerSt_WANDER_DRIVE = 0x34,
  PerSt_GO_PLANT_MINE = 0x35,
  PerSt_WAIT_TO_EXIT_VEHICLE = 0x36,
  PerSt_CATCH_FERRY = 0x37,
  PerSt_EXIT_FERRY = 0x38,
  PerSt_AVOID_GROUP = 0x39,
  PerSt_UNUSED_3A = 0x3A,
};

/** Max health of a person cannot safely go beyond that.
 */
#define PERSON_MAX_HEALTH_LIMIT 16383

#define PERSON_MAX_SPEED 2048

struct Thing;

struct PeepStat
{
    short MaxHealth;
    short MaxShield;
    short MaximumStamina;
    short MaxEnergy;
    short PersuadeReqd;
    short PersuadeWorth;
    short Speed;
    short dum2;
    short dum3;
    short dum4;
};

#pragma pack()
/******************************************************************************/
extern struct PeepStat peep_type_stats[];
extern ushort people_frames[SubTT_PERS_COUNT][22];

TbBool person_carries_weapon(struct Thing *p_person, ubyte weapon);

void set_person_stats_type(struct Thing *p_person, ushort type);
void init_person_thing(struct Thing *p_person);
void person_give_best_mods(struct Thing *p_person);

TbBool person_is_persuaded(short thing);
TbBool person_is_persuaded_by_person(short thing, short owntng);
TbBool person_is_persuaded_by_player(short thing, ushort plyr);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
