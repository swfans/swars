/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file command.h
 *     Header file for command.c.
 * @par Purpose:
 *     Handling Command structs.
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
#ifndef COMMAND_H
#define COMMAND_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum PersonCommandType
{
  PCmd_NONE = 0x0,
  PCmd_STAY = 0x1,
  PCmd_GO_TO_POINT = 0x2,
  PCmd_GO_TO_PERSON = 0x3,
  PCmd_KILL_PERSON = 0x4,
  PCmd_KILL_MEM_GROUP = 0x5,
  PCmd_KILL_ALL_GROUP = 0x6,
  PCmd_PERSUADE_PERSON = 0x7,
  PCmd_PERSUADE_MEM_GROUP = 0x8,
  PCmd_PERSUADE_ALL_GROUP = 0x9,
  PCmd_BLOCK_PERSON = 0xA,
  PCmd_SCARE_PERSON = 0xB,
  PCmd_FOLLOW_PERSON = 0xC,
  PCmd_SUPPORT_PERSON = 0xD,
  PCmd_PROTECT_PERSON = 0xE,
  PCmd_HIDE = 0xF,
  PCmd_GET_ITEM = 0x10,
  /** Use specified weapon pointing at given coords.
   */
  PCmd_USE_WEAPON = 0x11,
  PCmd_DROP_SPEC_ITEM = 0x12,
  PCmd_AVOID_PERSON = 0x13,
  PCmd_WAND_AVOID_GROUP = 0x14,
  /** Plant a bomb under a building. Given coords not only
   * identify the building, but are also bomb location.
   */
  PCmd_DESTROY_BUILDING = 0x15,
  PCmd_UNKN16 = 0x16,
  PCmd_USE_VEHICLE = 0x17,
  PCmd_EXIT_VEHICLE = 0x18,
  PCmd_CATCH_TRAIN = 0x19,
  PCmd_OPEN_DOME = 0x1A,
  PCmd_CLOSE_DOME = 0x1B,
  PCmd_DROP_WEAPON = 0x1C,
  PCmd_CATCH_FERRY = 0x1D,
  PCmd_EXIT_FERRY = 0x1E,
  PCmd_PING_EXIST = 0x1F,
  PCmd_GOTOPOINT_FACE = 0x20,
  PCmd_SELF_DESTRUCT = 0x21,
  PCmd_PROTECT_MEM_G = 0x22,
  PCmd_RUN_TO_POINT = 0x23,
  PCmd_KILL_EVERYONE = 0x24,
  PCmd_GUARD_OFF = 0x25,
  PCmd_EXECUTE_COMS = 0x26,
  PCmd_UNKN27 = 0x27,
  PCmd_UNKN28,
  PCmd_UNKN29,
  PCmd_UNKN2A,
  PCmd_UNKN2B,
  PCmd_UNKN2C,
  PCmd_UNKN2D,
  PCmd_UNKN2E,
  PCmd_UNKN2F,
  PCmd_UNKN30, // = 0x30
  PCmd_UNKN31,
  PCmd_UNKN32,
  PCmd_WAIT_P_V_DEAD = 0x33,
  PCmd_WAIT_MEM_G_DEAD = 0x34,
  PCmd_WAIT_ALL_G_DEAD = 0x35,
  PCmd_WAIT_P_V_I_NEAR = 0x36,
  PCmd_WAIT_MEM_G_NEAR = 0x37,
  PCmd_WAIT_ALL_G_NEAR = 0x38,
  PCmd_WAIT_P_V_I_ARRIVE = 0x39,
  PCmd_WAIT_MEM_G_ARRIVE = 0x3A,
  PCmd_WAIT_ALL_G_ARRIVE = 0x3B,
  PCmd_WAIT_P_PERSUADE = 0x3C,
  PCmd_WAIT_MEM_G_PERSUADE = 0x3D,
  PCmd_WAIT_ALL_G_PERSUADE = 0x3E,
  PCmd_WAIT_MISSION_SUCC = 0x3F,
  PCmd_WAIT_MISSION_FAIL = 0x40,
  PCmd_WAIT_MISSION_START = 0x41,
  PCmd_WAIT_OBJT_DESTROY = 0x42,
  PCmd_WAIT_TIME = 0x43,
  PCmd_UNKN44,
  PCmd_UNKN45,
  PCmd_UNKN46,
  PCmd_WAND_P_V_DEAD = 0x47,
  PCmd_WAND_MEM_G_DEAD = 0x48,
  PCmd_WAND_ALL_G_DEAD = 0x49,
  PCmd_WAND_P_V_I_NEAR = 0x4A,
  PCmd_WAND_MEM_G_NEAR = 0x4B,
  PCmd_WAND_ALL_G_NEAR = 0x4C,
  PCmd_WAND_P_V_I_ARRIVE = 0x4D,
  PCmd_WAND_MEM_G_ARRIVE = 0x4E,
  PCmd_WAND_ALL_G_ARRIVE = 0x4F,
  PCmd_WAND_P_PERSUADE = 0x50,
  PCmd_WAND_MEM_G_PERSUADE = 0x51,
  PCmd_WAND_ALL_G_PERSUADE = 0x52,
  PCmd_WAND_MISSION_SUCC = 0x53,
  PCmd_WAND_MISSION_FAIL = 0x54,
  PCmd_WAND_MISSION_START = 0x55,
  PCmd_WAND_OBJT_DESTROY = 0x56,
  PCmd_WAND_TIME = 0x57,
  PCmd_UNKN58,
  PCmd_UNKN59,
  PCmd_UNKN5A,
  PCmd_UNKN5B,
  PCmd_UNKN5C,
  PCmd_UNKN5D,
  PCmd_UNKN5E,
  PCmd_UNKN5F,
  PCmd_UNKN60,
  PCmd_UNKN61,
  PCmd_UNKN62,
  PCmd_UNKN63,
  PCmd_UNKN64,
  PCmd_UNKN65,
  PCmd_UNKN66,
  PCmd_UNKN67,
  PCmd_ADD_STATIC,
  PCmd_WAIT_TIME2,
  PCmd_UNKN6A,
  PCmd_UNKN6B,
  PCmd_UNKN6C,
  PCmd_UNKN6D,
  PCmd_LOOP_COM = 0x6E,
  PCmd_UNTIL_P_V_DEAD = 0x6F,
  PCmd_UNTIL_MEM_G_DEAD = 0x70,
  PCmd_UNTIL_ALL_G_DEAD = 0x71,
  PCmd_UNTIL_P_V_I_NEAR = 0x72,
  PCmd_UNTIL_MEM_G_NEAR = 0x73,
  PCmd_UNTIL_ALL_G_NEAR = 0x74,
  PCmd_UNTIL_P_V_I_ARRIVE = 0x75,
  PCmd_UNTIL_MEM_G_ARRIVE = 0x76,
  PCmd_UNTIL_ALL_G_ARRIVE = 0x77,
  PCmd_UNTIL_P_PERSUADE = 0x78,
  PCmd_UNTIL_MEM_G_PERSUADE = 0x79,
  PCmd_UNTIL_ALL_G_PERSUADE = 0x7A,
  PCmd_UNTIL_MISSION_SUCC = 0x7B,
  PCmd_UNTIL_MISSION_FAIL = 0x7C,
  PCmd_UNTIL_MISSION_START = 0x7D,
  PCmd_UNTIL_OBJT_DESTROY = 0x7E,
  PCmd_UNTIL_TIME = 0x7F,
  PCmd_WAIT_OBJV = 0x80,
  PCmd_WAND_OBJV = 0x81,
  PCmd_UNTIL_OBJV = 0x82,
  PCmd_WITHIN_AREA = 0x83,
  PCmd_WITHIN_OFF = 0x84,
  PCmd_LOCK_BUILDN = 0x85,
  PCmd_UNLOCK_BUILDN = 0x86,
  PCmd_SELECT_WEAPON = 0x87,
  PCmd_HARD_AS_AGENT = 0x88,
  PCmd_UNTIL_G_NOT_SEEN = 0x89,
  PCmd_START_DANGER_MUSIC = 0x8A,
  PCmd_PING_P_V = 0x8B,
  PCmd_CAMERA_TRACK = 0x8C,
  PCmd_UNTRUCE_GROUP = 0x8D,
  PCmd_PLAY_SAMPLE = 0x8E,
  PCmd_IGNORE_ENEMIES = 0x8F,
  PCmd_FULL_STAMINA = 0x90, // Name uncertain
  PCmd_UNKN91 = 0x91,
  PCmd_TYPES_COUNT,
};

enum PersonCommandFlags
{
  PCmdF_NONE = 0x0000,
  PCmdF_Unkn0001 = 0x0001,
  PCmdF_Unkn0002 = 0x0002,
  PCmdF_Unkn0004 = 0x0004,
  PCmdF_Unkn0008 = 0x0008,
  PCmdF_Unkn0010 = 0x0010,
  PCmdF_Unkn0020 = 0x0020,
  PCmdF_Unkn0040 = 0x0040,
  PCmdF_Unkn0080 = 0x0080,
  PCmdF_Unkn0100 = 0x0100,
  PCmdF_Unkn0200 = 0x0200,
  PCmdF_Unkn0400 = 0x0400,
  PCmdF_Unkn0800 = 0x0800,
  PCmdF_Unkn1000 = 0x1000,
  PCmdF_Unkn2000 = 0x2000,
  PCmdF_Unkn4000 = 0x4000,
  PCmdF_Unkn8000 = 0x8000,
  PCmdF_Unkn010000 = 0x00010000,
  PCmdF_Unkn020000 = 0x00020000,
  PCmdF_Unkn040000 = 0x00040000,
  PCmdF_Unkn080000 = 0x00080000,
  PCmdF_Unkn100000 = 0x00100000,
  PCmdF_Unkn200000 = 0x00200000,
  PCmdF_Unkn400000 = 0x00400000,
  PCmdF_Unkn800000 = 0x00800000,
};

struct Command
{
  short Next;
  short OtherThing;
  short X;
  short Y;
  short Z;
  ubyte Type;
  ubyte SubType;
  short Arg1;
  short Arg2;
  short Time;
  short MyThing;
  short Parent;
  ulong Flags;
  ulong field_1A;
  short field_1E;
};


#pragma pack()
/******************************************************************************/
extern struct Command *game_commands;
extern ushort next_command;

void snprint_command(char *buf, ulong buflen, ushort cmd);
TbBool is_command_any_until(struct Command *p_cmd);

/** For Commands which rely on searching Things on map, find the Things
 * and set indexes.
 */
void fix_thing_commands_indexes(void);

/** Verify and update commands array.
 */
void check_and_fix_commands(void);

/** Verify and update commands in the context of their assignment to things.
 * Requires same_type lists to be already populated.
 */
void check_and_fix_thing_commands(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
