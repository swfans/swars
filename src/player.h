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
    ulong Weapons[32];
    union Mod Mods[32];
    long Sex;
    char RandomName[32];
    struct WeaponsFourPack FourPacks[32];
    ubyte NumAgents;
};

struct SpecialUserInput
{
  long Bits;
  char DtX;
  char DtZ;
  ushort Turn; /* from do_user_interface(), this seem to be game turn of last action */
  ubyte LastDir; /* probably wrong place */
  ubyte CurDir; /* probably wrong place */
  ushort MapX[3];
  ushort MapY[3];
  ushort MapZ[3];
  ushort OnFace;
  ushort ControlMode;
  ushort MapIndex;
};

typedef struct {
  struct SpecialUserInput UserInput[3];
  char field_66[2];
  int field_68[4];
  int field_78;
  int field_7C[2];
  int field_84;
    ulong DirectControl[4]; /* offs=0x88 */
    ulong ControlPad;
    struct Thing *MyAgent[4];
    ubyte PrevWeapon[4];  /* offs=0xAC */
    ubyte PanelState[4];
    ubyte PanelItem[4];
    ushort Dummy98; /* offs=0xB8 */
    ubyte Dummy97;
    ubyte MissionAgents;
    ushort LastPacketType;
    ubyte State[4];
    ubyte SubState[4]; /* offs=0xC2 */
    ubyte PrevState[4];
    ubyte SubStateTimer[4];
    ubyte DoubleMode;
    ubyte PlayerNo;
    ulong Weapons[4];
    union Mod Mods[4];
  short field_E8;
  int field_EA;
  int field_EE;
  int field_F2;
  int field_F6;
    ushort PanelTimer;
    short PanelX;
    short PanelY;
    short Target;
  short field_102;
    short TargetType;
    ubyte FourPacks[5][4];
    ubyte WepDelays[4][32];
  ushort field_19A[4];
  ushort field_1A2[4];
} PlayerInfo;


#pragma pack()
/******************************************************************************/
extern PlayerInfo players[8];
extern ubyte local_player_no;
extern struct AgentInfo cryo_agents;

void place_single_player(void);

void player_update_agents_from_cryo(PlayerInfo *p_player);
void cryo_update_agents_from_player(PlayerInfo *p_player);
void players_sync_from_cryo(void);

TbBool player_agent_has_weapon(ushort plagent, ubyte weapon);
TbBool free_slot(ushort plagent, ubyte weapon);
TbBool player_cryo_add_weapon_one(ushort cryo_no, ubyte weapon);
TbBool player_cryo_remove_weapon_one(ushort cryo_no, ubyte weapon);
TbBool player_cryo_transfer_weapon_between_agents(ushort from_cryo_no,
  ushort to_cryo_no, ubyte weapon);
const char *get_cryo_agent_name(ushort cryo_no);
void remove_agent(ubyte cryo_no);
void add_agent(ulong weapons, ushort mods);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
