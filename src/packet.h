/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file packet.h
 *     Header file for packet.c.
 * @par Purpose:
 *     Handling of packets storing player actions.
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
#ifndef PACKET_H
#define PACKET_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;

enum PacketActions
{
    PAct_NONE = 0x0,
    PAct_1 = 0x1,
    PAct_MISSN_ABORT = 0x2,
    PAct_3 = 0x3,
    PAct_4 = 0x4,
    PAct_5 = 0x5,
    PAct_6 = 0x6,
    PAct_7 = 0x7,
    PAct_8 = 0x8,
    PAct_9 = 0x9,
    PAct_A = 0xA,
    PAct_AGENT_GOTO_GND_PT_ABS = 0xB,
    PAct_AGENT_GOTO_GND_PT_REL = 0xC,
    PAct_SELECT_NEXT_WEAPON = 0xD,
    PAct_DROP_SELC_WEAPON_SECR = 0xE,
    PAct_F = 0xF,
    PAct_PICKUP = 0x10,
    PAct_11 = 0x11,
    PAct_12 = 0x12,
    PAct_13 = 0x13,
    PAct_ENTER_VEHICLE = 0x14,
    PAct_LEAVE_VEHICLE = 0x15,
    PAct_16 = 0x16,
    PAct_SELECT_AGENT = 0x17,
    PAct_18 = 0x18,
    PAct_AGENT_GOTO_GND_PT_REL_FF = 0x19,
    PAct_SHOOT_AT_GND_POINT = 0x1A,
    PAct_SELECT_PREV_WEAPON = 0x1B,
    PAct_PROTECT_INC = 0x1C,
    PAct_PROTECT_TOGGLE = 0x1D,
    PAct_SHOOT_AT_THING = 0x1E,
    PAct_GET_ITEM = 0x1F,
    PAct_PLANT_MINE_AT_GND_PT = 0x20,
    PAct_SELECT_SPECIFIC_WEAPON = 0x21,
    PAct_DROP_HELD_WEAPON_SECR = 0x22,
    PAct_AGENT_SET_MOOD = 0x23,
    PAct_GO_ENTER_VEHICLE = 0x24,
    PAct_FOLLOW_PERSON = 0x25,
    PAct_CONTROL_MODE = 0x26,
    PAct_AGENT_GOTO_FACE_PT_ABS = 0x27,
    PAct_AGENT_GOTO_GND_PT_ABS_FF = 0x28,
    PAct_29 = 0x29,
    PAct_AGENT_GOTO_FACE_PT_ABS_FF = 0x2A,
    PAct_GO_ENTER_VEHICLE_FF = 0x2B,
    PAct_GET_ITEM_FAST = 0x2C,
    PAct_SHIELD_TOGGLE = 0x2D,
    PAct_PLANT_MINE_AT_GND_PT_FF = 0x2E,
    PAct_SHOOT_AT_GND_POINT_FF = 0x2F,
    PAct_PEEPS_SCATTER = 0x30,
    PAct_SELECT_GRP_SPEC_WEAPON = 0x31,
    PAct_AGENT_USE_MEDIKIT = 0x32,
    PAct_33 = 0x33,
    PAct_GROUP_SET_MOOD = 0x34,
    PAct_AGENT_UNKGROUP_PROT = 0x35,
    PAct_AGENT_UNKGROUP_ADD = 0x36,
    PAct_CHAT_MESSAGE_KEY = 0x37,
    PAct_SHOOT_AT_FACE_POINT = 0x38,
    PAct_SHOOT_AT_FACE_POINT_FF = 0x39,
    PAct_PLANT_MINE_AT_FACE_PT = 0x3A,
    PAct_PLANT_MINE_AT_FACE_PT_FF = 0x3B,
    PAct_3C = 0x3C,
    PAct_3D = 0x3D,
    PAct_3E = 0x3E,
    PAct_3F = 0x3F,
    PAct_40 = 0x40,
    PAct_AGENT_SELF_DESTRUCT = 0xFF,
};

struct Packet
{
    ushort Action;
    ushort Data;
    short X;
    short Y;
    short Z;
    ushort Action2;
    ushort Data2;
    short X2;
    short Y2;
    short Z2;
    ushort Action3;
    ushort Data3;
    short X3;
    short Y3;
    short Z3;
    ushort Action4;
    ushort Data4;
    short X4;
    short Y4;
    short Z4;
    long D1Seed;
    long D2Check;
};


#pragma pack()
/******************************************************************************/
extern struct Packet packets[8];
extern void (*my_build_packet)(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);

void build_packet(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);
void build_packet2(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);
void build_packet3(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);
void build_packet4(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);

void PacketRecord_Close(void);
void PacketRecord_OpenWrite(void);
void PacketRecord_OpenRead(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
