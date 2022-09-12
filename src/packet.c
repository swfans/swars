/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file packet.c
 *     Packet structure support.
 * @par Purpose:
 *     Implement general functions for handling packets.
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

#include "swlog.h"
/******************************************************************************/
void (*my_build_packet)(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);

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

/******************************************************************************/
