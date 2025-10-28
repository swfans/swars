/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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
#include "game_bstype.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum TNodeFlags {
    TNdF_Unkn0001     = 0x0001,
    TNdF_Unkn0002     = 0x0002,
    TNdF_Unkn0004     = 0x0004,
    TNdF_Unkn0008     = 0x0008,
    TNdF_Unkn0010     = 0x0010,
    TNdF_Unkn0020     = 0x0020,
    TNdF_Unkn0040     = 0x0040,
    TNdF_Unkn0080     = 0x0080,
    TNdF_Unkn0100     = 0x0100,
    TNdF_Unkn0200     = 0x0200,
    TNdF_Unkn0400     = 0x0400,
    TNdF_Unkn0800     = 0x0800,
    TNdF_Unkn1000     = 0x1000,
    TNdF_Unkn2000     = 0x2000,
    TNdF_Unkn4000     = 0x4000,
    TNdF_Unkn8000     = 0x8000,
};

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
void tnode_all_unlink_thing(ThingIdx thing);

void process_next_tnode(struct Thing *p_vehicle);

void VNAV_preprocess_bezier_turns(ulong nturns);
void VNAV_init_new_traffic_system(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
