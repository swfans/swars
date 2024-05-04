/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehtraffic.h
 *     Header file for vehtraffic.c.
 * @par Purpose:
 *     Pathfinding triangulation support required by the 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef VEHTRAFFIC_H
#define VEHTRAFFIC_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct STngUTraffic {
    short Link[4];
    ubyte Flags[4];
};

struct TrafficNode { // sizeof=36
    short X;
    short Y;
    short Z;
    struct STngUTraffic UTraffic;
    ushort Flags;	// ofs=0x12
    short GateLink;
    ubyte field_16[11];
    short Dummy1;
    ubyte Dummy2;
};

struct Thing;

#pragma pack()
/******************************************************************************/
extern struct TrafficNode *game_traffic_nodes;
extern ushort next_traffic_node;

short get_next_tnode(struct Thing *p_vehicle, struct TrafficNode *p_tnode);

void process_next_tnode(struct Thing *p_vehicle);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
