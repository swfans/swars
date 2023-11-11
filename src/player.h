/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file player.h
 *     Header file for player.c.
 * @par Purpose:
 *     Player state and information handling.
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
#ifndef PLAYER_H
#define PLAYER_H

#include "bftypes.h"
#include "cybmod.h"
#include "weapon.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;

struct AgentInfo {
    u32 Weapons[32];
    union Mod Mods[32];
    i32 Sex;
    char RandomName[32];
    struct WeaponsFourPack FourPacks[32];
    u8 NumAgents;
};

struct SpecialUserInput
{
  i32 Bits;
  char DtX;
  char DtZ;
  u16 Turn; /* from do_user_i32erface(), this seem to be game turn of last action */
  u8 LastDir; /* probably wrong place */
  u8 CurDir; /* probably wrong place */
  u16 MapX[3];
  u16 MapY[3];
  u16 MapZ[3];
  u16 OnFace;
  u16 ControlMode;
  u16 MapIndex;
};

typedef struct {
  struct SpecialUserInput UserInput[3];
  char field_66[2];
  i32 field_68[4];
  i32 field_78;
  i32 field_7C[2];
  i32 field_84;
    u32 DirectControl[4]; /* offs=0x88 */
    u32 ControlPad;
    struct Thing *MyAgent[4];
    u8 PrevWeapon[4];  /* offs=0xAC */
    u8 PanelState[4];
    u8 PanelItem[4];
    u16 Dummy98; /* offs=0xB8 */
    u8 Dummy97;
    u8 MissionAgents;
    u16 LastPacketType;
    u8 State[4];
    u8 SubState[4]; /* offs=0xC2 */
    u8 PrevState[4];
    u8 SubStateTimer[4];
    u8 DoubleMode;
    u8 PlayerNo;
    u32 Weapons[4];
    union Mod Mods[4];
  i16 field_E8;
  i32 field_EA;
  i32 field_EE;
  i32 field_F2;
  i32 field_F6;
  char field_FA[4];
  i32 field_FE;
  i16 field_102;
  i16 TargetType;
  u8 FourPacks[5][4];
  u8 WepDelays[4][32];
  u16 field_19A[4];
  u16 field_1A2[4];
} PlayerInfo;


#pragma pack()
/******************************************************************************/
extern PlayerInfo players[8];
extern u8 local_player_no;
extern struct AgentInfo cryo_agents;

void place_single_player(void);

void player_update_agents_from_cryo(PlayerInfo *p_player);
void cryo_update_agents_from_player(PlayerInfo *p_player);
void players_sync_from_cryo(void);
void remove_agent(u8 cryo_no);
void add_agent(u32 weapons, u16 mods);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
