/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file packet.h
 *     Header file for packet.c.
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
#ifndef PACKET_H
#define PACKET_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;
struct Packet;



#pragma pack()
/******************************************************************************/
extern void (*my_build_packet)(struct Packet *packet, ushort type, ulong param1, long x, long y, long z);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
