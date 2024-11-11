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

#define CRYO_PODS_MAX_COUNT 32
#define AGENTS_SQUAD_MAX_COUNT 4

struct Thing;

//TODO would make more sense to have a struct for each agent, and then a merging struct
struct AgentInfo {
    ulong Weapons[CRYO_PODS_MAX_COUNT];
    union Mod Mods[CRYO_PODS_MAX_COUNT];
    ulong Sex;
    char RandomName[CRYO_PODS_MAX_COUNT];
    struct WeaponsFourPack FourPacks[CRYO_PODS_MAX_COUNT];
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
    ulong DirectControl[AGENTS_SQUAD_MAX_COUNT]; /* offs=0x88 */
    ulong ControlPad;
    /* Pointers to agents in the squad of this player.
     * These are guaranteed treference a valid thing (non-NULL),
     * but if an agent is not in use, the reference may be
     * to a dummy thing. Obviously the pointers are valid only
     * during the tactical mission, not in-menu.
     */
    struct Thing *MyAgent[AGENTS_SQUAD_MAX_COUNT];
    ubyte PrevWeapon[AGENTS_SQUAD_MAX_COUNT];  /* offs=0xAC */
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
    ulong Weapons[AGENTS_SQUAD_MAX_COUNT];
    union Mod Mods[AGENTS_SQUAD_MAX_COUNT];
  short field_E8[3];
  short field_EE;
    short SpecialItems[4];
    short GotoFace;
    ushort PanelTimer;
    short PanelX;
    short PanelY;
    short Target;
  short field_102;
    short TargetType;
    ubyte FourPacks[5][AGENTS_SQUAD_MAX_COUNT];
    ubyte WepDelays[AGENTS_SQUAD_MAX_COUNT][32];
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

short direct_control_thing_for_player(short plyr);
void player_target_clear(short plyr);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
