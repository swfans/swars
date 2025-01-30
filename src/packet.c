/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
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

void (*my_build_packet)(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);

/******************************************************************************/

extern TbFileHandle packet_rec_fh;
ushort packet_rec_no = 0;

void build_packet(struct Packet *packet, ushort type, ulong param1, long x, long y, long z)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
}

void build_packet2(struct Packet *packet, ushort type, ulong param1, long x, long y, long z)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet2\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
}

void build_packet3(struct Packet *packet, ushort type, ulong param1, long x, long y, long z)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet3\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
}

void build_packet4(struct Packet *packet, ushort type, ulong param1, long x, long y, long z)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_build_packet4\n"
        : : "a" (packet), "d" (type), "b" (param1), "c" (x), "g" (y), "g" (z));
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
