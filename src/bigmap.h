/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bigmap.h
 *     Header file for bigmap.c.
 * @par Purpose:
 *     Routines implementing map elements reference lists.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BIGMAP_H
#define BIGMAP_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define MAP_TILE_WIDTH 128
#define MAP_TILE_HEIGHT 128

/** Length of the spiral, which speeds up searching for things in vicinity
 * of given map coords. It does not make sense to search this way if the
 * amount of tiles searched exceeds half of things limit; so this should
 * be set to no more than (THINGS_LIMIT+STHINGS_LIMIT)/2.
 */
#define SPIRAL_STEPS_COUNT   1700

struct MyMapElement {
  ushort Texture;
  ushort Alt;
  ushort field_4;
  ushort field_6;
  short Child;
  ushort ColHead;
  ubyte field_C;
  ubyte Flags2;
  ubyte Zip;
  ubyte field_F;
  ushort field_10;
};

struct MapOffset {
  sbyte v;
  sbyte h;
  short both;
};

#pragma pack()
/******************************************************************************/
extern struct MyMapElement *game_my_big_map;

extern struct MapOffset spiral_step[SPIRAL_STEPS_COUNT];
extern ushort dist_tiles_to_spiral_step[MAP_TILE_WIDTH];
extern ushort spiral_dist_tiles_limit;

short get_mapwho_thing_index(short tile_x, short tile_y);
void init_search_spiral(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
