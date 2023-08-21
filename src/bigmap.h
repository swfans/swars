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

#pragma pack()
/******************************************************************************/
extern struct MyMapElement *game_my_big_map;

short get_mapwho_thing_index(short tile_x, short tile_y);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
