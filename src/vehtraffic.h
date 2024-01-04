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

struct TrafficNode { // sizeof=36
    ubyte field_0[18];
    ubyte field_12;
    ubyte field_13;
    ubyte field_14[15];
    ubyte field_23;
};

#pragma pack()
/******************************************************************************/
extern struct TrafficNode *game_traffic_nodes;
extern ushort next_traffic_node;


/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
