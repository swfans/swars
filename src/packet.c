/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file packet.c
 *     Handling of packets storing player actions.
 * @par Purpose:
 *     Packets are an abstraction layer between input devices and in-game
 *     commands and actions. They also easy replay storage and exchange
 *     over network.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 11 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "packet.h"

#include "campaign.h"
#include "game_data.h"
#include "game.h"
#include "player.h"
#include "swlog.h"
/******************************************************************************/
#pragma pack(1)

struct PacketFileHead {
    ulong magic;
    ushort campgn;
    ushort missi;
    ushort mapno;
    ushort levelno;
};

#pragma pack()

const char *packet_action_result_text[] = {
    "action done",
    "action succeeded",
    "unsupported action code",
    "invalid argument",
    "invalid agent slot",
    "already done/in progress",
    "thing in bad state",
};

const char *packet_action_name[] = {
    "NONE",
    "UNKNACTN_01",
    "MISSN_ABORT",
    "UNKNACTN_03",
    "UNKNACTN_04",
    "UNKNACTN_05",
    "UNKNACTN_06",
    "UNKNACTN_07",
    "UNKNACTN_08",
    "UNKNACTN_09",
    "UNKNACTN_0A",
    "AGENT_GOTO_GND_PT_ABS",
    "AGENT_GOTO_GND_PT_REL",
    "SELECT_NEXT_WEAPON",
    "DROP_SELC_WEAPON_SECR",
    "UNKNACTN_0F",
    "PICKUP",
    "UNKNACTN_11",
    "UNKNACTN_12",
    "UNKNACTN_13",
    "ENTER_VEHICLE",
    "LEAVE_VEHICLE",
    "UNKNACTN_16",
    "SELECT_AGENT",
    "UNKNACTN_18",
    "AGENT_GOTO_GND_PT_REL_FF",
    "SHOOT_AT_GND_POINT",
    "SELECT_PREV_WEAPON",
    "PROTECT_INC",
    "PROTECT_TOGGLE",
    "SHOOT_AT_THING",
    "GET_ITEM",
    "PLANT_MINE_AT_GND_PT",
    "SELECT_SPECIFIC_WEAPON",
    "DROP_HELD_WEAPON_SECR",
    "AGENT_SET_MOOD",
    "GO_ENTER_VEHICLE",
    "FOLLOW_PERSON",
    "CONTROL_MODE",
    "AGENT_GOTO_FACE_PT_ABS",
    "AGENT_GOTO_GND_PT_ABS_FF",
    "UNKNACTN_29",
    "AGENT_GOTO_FACE_PT_ABS_FF",
    "GO_ENTER_VEHICLE_FF",
    "GET_ITEM_FAST",
    "SHIELD_TOGGLE",
    "PLANT_MINE_AT_GND_PT_FF",
    "SHOOT_AT_GND_POINT_FF",
    "PEEPS_SCATTER",
    "SELECT_GRP_SPEC_WEAPON",
    "AGENT_USE_MEDIKIT",
    "UNKNACTN_33",
    "GROUP_SET_MOOD",
    "AGENT_UNKGROUP_PROT",
    "AGENT_UNKGROUP_ADD",
    "CHAT_MESSAGE_KEY",
    "SHOOT_AT_FACE_POINT",
    "SHOOT_AT_FACE_POINT_FF",
    "PLANT_MINE_AT_FACE_PT",
    "PLANT_MINE_AT_FACE_PT_FF",
    "UNKNACTN_3C",
    "UNKNACTN_3D",
    "UNKNACTN_3E",
    "UNKNACTN_3F",
    "UNKNACTN_40",
    "AGENT_SELF_DESTRUCT",
};

void (*my_build_packet)(struct Packet *packet, ushort action, ulong param1, long x, long y, long z);

/******************************************************************************/

extern TbFileHandle packet_rec_fh;
ushort packet_rec_no = 0;

const char * get_packet_action_name(ushort atype)
{
    if (atype > sizeof(packet_action_name)/sizeof(packet_action_name[0]))
        return "outranged";
    return packet_action_name[atype];
}

const char * get_packet_action_result_text(short result)
{
    if ((result < 0) || (result > (int)(sizeof(packet_action_result_text)/sizeof(packet_action_result_text[0]))))
        return "outranged";
    return packet_action_result_text[result];
}

void packet_action_immediate_local_change(ubyte cmouser, ushort action)
{
    players[local_player_no].PrevState[cmouser] = action;

    switch (action & 0x7FFF)
    {
    case PAct_SELECT_PREV_WEAPON:
    case PAct_SELECT_SPECIFIC_WEAPON:
    case PAct_SELECT_NEXT_WEAPON:
    case PAct_SELECT_GRP_SPEC_WEAPON:
    case PAct_33:
        ingame.UserZoom = 120;
        break;
    default:
        break;
    }
}

ubyte packet_action_params_count(ushort action)
{
    switch (action & 0x7FFF)
    {
    case PAct_NONE:
    case PAct_AGENT_UNKGROUP_PROT:
    case PAct_AGENT_UNKGROUP_ADD:
    case PAct_CHAT_MESSAGE_KEY:
    case PAct_AGENT_USE_MEDIKIT:
    case PAct_LEAVE_VEHICLE:
    case PAct_AGENT_SELF_DESTRUCT:
    case PAct_PROTECT_INC:
    case PAct_PROTECT_TOGGLE:
    case PAct_DROP_SELC_WEAPON_SECR:
    case PAct_SHIELD_TOGGLE:
    case PAct_CONTROL_MODE:
        return 1;

    case PAct_GET_ITEM:
    case PAct_DROP_HELD_WEAPON_SECR:
    case PAct_AGENT_SET_MOOD:
    case PAct_F:
    case PAct_PICKUP:
    case PAct_ENTER_VEHICLE:
    case PAct_GO_ENTER_VEHICLE:
    case PAct_GROUP_SET_MOOD:
    case PAct_GO_ENTER_VEHICLE_FF:
    case PAct_GET_ITEM_FAST:
    case PAct_FOLLOW_PERSON:
    case PAct_SELECT_AGENT:
    case PAct_SHOOT_AT_THING:
        return 2;

    case PAct_SELECT_PREV_WEAPON:
    case PAct_SELECT_SPECIFIC_WEAPON:
    case PAct_SELECT_NEXT_WEAPON:
    case PAct_SELECT_GRP_SPEC_WEAPON:
    case PAct_33:
        return 3;

    case PAct_18:
    case PAct_AGENT_GOTO_GND_PT_REL_FF:
    case PAct_SHOOT_AT_GND_POINT:
    case PAct_PLANT_MINE_AT_GND_PT:
    case PAct_AGENT_GOTO_GND_PT_ABS:
    case PAct_AGENT_GOTO_GND_PT_REL:
    case PAct_AGENT_GOTO_FACE_PT_ABS:
    case PAct_AGENT_GOTO_GND_PT_ABS_FF:
    case PAct_SHOOT_AT_GND_POINT_FF:
    case PAct_PEEPS_SCATTER:
    case PAct_SHOOT_AT_FACE_POINT:
    case PAct_SHOOT_AT_FACE_POINT_FF:
    case PAct_PLANT_MINE_AT_FACE_PT:
    case PAct_PLANT_MINE_AT_FACE_PT_FF:
    case PAct_AGENT_GOTO_FACE_PT_ABS_FF:
    case PAct_PLANT_MINE_AT_GND_PT_FF:
    case PAct_29:
        return 4;

    case PAct_1:
    case PAct_MISSN_ABORT:
    case PAct_3:
    case PAct_4:
    case PAct_5:
    case PAct_6:
    case PAct_7:
    case PAct_8:
    case PAct_9:
    case PAct_A:
    case PAct_11:
    case PAct_12:
    case PAct_13:
    case PAct_16:
    case PAct_3C:
    case PAct_3D:
    case PAct_3E:
    case PAct_3F:
    case PAct_40:
    default:
        return 0;
    }
}

void build_packet(struct Packet *packet, ushort action, ulong param1, long x, long y, long z)
{
#if 0
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet\n"
        : : "a" (packet), "d" (action), "b" (param1), "c" (x), "g" (y), "g" (z));
#endif
    ubyte num_params;

    num_params = packet_action_params_count(action);
    packet_action_immediate_local_change(0, action);

    switch (num_params)
    {
    case 1:
        packet->Data = param1;
        packet->Action = action;
        break;
    case 2:
        packet->Data = param1;
        packet->X = x;
        packet->Action = action;
        break;
    case 3:
        packet->Data = param1;
        packet->X = x;
        packet->Y = y;
        packet->Action = action;
        break;
    case 4:
        packet->Data = param1;
        packet->X = x;
        packet->Y = y;
        packet->Z = z;
        packet->Action = action;
        break;
    default:
        break;
    }
}

void build_packet2(struct Packet *packet, ushort action, ulong param1, long x, long y, long z)
{
#if 0
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet2\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
#endif
    ubyte num_params;

    num_params = packet_action_params_count(action);
    packet_action_immediate_local_change(1, action);

    switch (num_params)
    {
    case 1:
        packet->Data2 = param1;
        packet->Action2 = action;
        break;
    case 2:
        packet->Data2 = param1;
        packet->X2 = x;
        packet->Action2 = action;
        break;
    case 3:
        packet->Data2 = param1;
        packet->X2 = x;
        packet->Y2 = y;
        packet->Action2 = action;
        break;
    case 4:
        packet->Data2 = param1;
        packet->X2 = x;
        packet->Y2 = y;
        packet->Z2 = z;
        packet->Action2 = action;
        break;
    default:
        break;
    }
}

void build_packet3(struct Packet *packet, ushort action, ulong param1, long x, long y, long z)
{
#if 0
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet3\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
#endif
    ubyte num_params;

    num_params = packet_action_params_count(action);
    packet_action_immediate_local_change(2, action);

    switch (num_params)
    {
    case 1:
        packet->Data3 = param1;
        packet->Action3 = action;
        break;
    case 2:
        packet->Data3 = param1;
        packet->X3 = x;
        packet->Action3 = action;
        break;
    case 3:
        packet->Data3 = param1;
        packet->X3 = x;
        packet->Y3 = y;
        packet->Action3 = action;
        break;
    case 4:
        packet->Data3 = param1;
        packet->X3 = x;
        packet->Y3 = y;
        packet->Z3 = z;
        packet->Action3 = action;
        break;
    default:
        break;
    }
}

void build_packet4(struct Packet *packet, ushort action, ulong param1, long x, long y, long z)
{
#if 0
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet4\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
#endif
    ubyte num_params;

    num_params = packet_action_params_count(action);
    packet_action_immediate_local_change(3, action);

    switch (num_params)
    {
    case 1:
        packet->Data4 = param1;
        packet->Action4 = action;
        break;
    case 2:
        packet->Data4 = param1;
        packet->X4 = x;
        packet->Action4 = action;
        break;
    case 3:
        packet->Data4 = param1;
        packet->X4 = x;
        packet->Y4 = y;
        packet->Action4 = action;
        break;
    case 4:
        packet->Data4 = param1;
        packet->X4 = x;
        packet->Y4 = y;
        packet->Z4 = z;
        packet->Action4 = action;
        break;
    default:
        break;
    }
}

void PacketRecord_Close(void)
{
    if (in_network_game)
        return;
    LbFileClose(packet_rec_fh);
}

void PacketRecord_OpenWrite(void)
{
    char fname[DISKPATH_SIZE];
    struct Mission *p_missi;
    struct PacketFileHead head;
    int file_no;

    head.magic = 0x544B4350; // 'PCKT'
    head.campgn = background_type;
    head.missi = ingame.CurrentMission;
    p_missi = &mission_list[head.missi];
    head.mapno = p_missi->MapNo;
    head.levelno = p_missi->LevelNo;

    file_no = get_highest_used_packet_record_no(head.campgn, head.missi);
    packet_rec_no = file_no + 1;
    get_packet_record_fname(fname, head.campgn, head.missi, packet_rec_no);
    LOGDBG("%s: Opening for packet save", fname);
    packet_rec_fh = LbFileOpen(fname, Lb_FILE_MODE_NEW);
    LbFileWrite(packet_rec_fh, &head, sizeof(head));
}

void PacketRecord_OpenRead(void)
{
    char fname[DISKPATH_SIZE];
    struct Mission *p_missi;
    struct PacketFileHead head;

    head.campgn = background_type;
    head.missi = ingame.CurrentMission;
    get_packet_record_fname(fname, head.campgn, head.missi, packet_rec_no);
    LOGDBG("%s: Opening for packet input", fname);
    packet_rec_fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if (packet_rec_fh == INVALID_FILE) {
        LOGERR("Packet file open failed");
        pktrec_mode = PktR_NONE;
        exit_game = true;
        return;
    }
    LbFileRead(packet_rec_fh, &head, sizeof(head));
    if (head.missi < next_mission)
        p_missi = &mission_list[head.missi];
    else
        p_missi = &mission_list[0];
    if (head.magic != 0x544B4350)
        LOGWARN("%s: Packet file has bad magic value", fname);
    if ((head.campgn != background_type) && (head.campgn != 0xFFFF))
        LOGWARN("Packet file expects campaign %hu, not %d",
          fname, head.campgn, (int)background_type);
    if ((head.missi != ingame.CurrentMission) && (head.missi != 0xFFFF))
        LOGWARN("Packet file expects mission %hu, not %d",
          fname, head.missi, (int)ingame.CurrentMission);
    if ((head.mapno != p_missi->MapNo) && (head.mapno != 0xFFFF))
        LOGWARN("Packet file expects map %hu, not %d",
          fname, head.mapno, (int)p_missi->MapNo);
    if ((head.levelno != p_missi->LevelNo) && (head.levelno != 0xFFFF))
        LOGWARN("Packet file expects level %hu, not %d",
          fname, head.levelno, (int)p_missi->LevelNo);
}

void PacketRecord_Read(struct Packet *p_pckt)
{
    asm volatile (
      "call ASM_PacketRecord_Read\n"
        : : "a" (p_pckt));
}

void PacketRecord_Write(struct Packet *p_pckt)
{
    asm volatile (
      "call ASM_PacketRecord_Write\n"
        : : "a" (p_pckt));
}

/******************************************************************************/
