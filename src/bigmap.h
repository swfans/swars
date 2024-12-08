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

#define MAP_COORD_WIDTH TILE_TO_MAPCOORD(MAP_TILE_WIDTH, 0)
#define MAP_COORD_HEIGHT TILE_TO_MAPCOORD(MAP_TILE_HEIGHT, 0)

/** Convert precise map coordinate to normal map coordinate.
 */
#define PRCCOORD_TO_MAPCOORD(pcor) ((pcor) >> 8)

/** Convert precise map Y coordinate to normal engine coordinate.
 * For unknown reason, Y has often different shift than other coords..
 */
#define PRCCOORD_TO_YCOORD(pcor) ((pcor) >> 5)

/** Convert normal map coordinate to precise map coordinate.
 */
#define MAPCOORD_TO_PRCCOORD(mcor, ppos) (((mcor) << 8) + (ppos))

/** Convert normal map coordinate to tile no.
 */
#define MAPCOORD_TO_TILE(mcor) ((mcor) >> 8)

/** Convert tile(block) no to normal map coordinate.
 */
#define TILE_TO_MAPCOORD(tile, mpos) (((tile) << 8) + (mpos))

/** Length of the spiral, which speeds up searching for things in vicinity
 * of given map coords. It does not make sense to search this way if the
 * amount of tiles searched exceeds half of things limit; so this should
 * be set to no more than (THINGS_LIMIT+STHINGS_LIMIT)/2.
 */
#define SPIRAL_STEPS_COUNT   1700

#define MAX_WALKABLE_STEEPNESS (21 * LbFPMath_PI / 180)

/* Linear distance equal to tan(MAX_WALKABLE_STEEPNESS) * 256
 */
#define MAX_WALKABLE_STEEPNESS_PER_256 98

struct MyMapElement { // sizeof=18
  ushort Texture; /**< offs=0x00 Index of SingleFloorTexture for this tile, and texture flags. */
  ushort Shade;
  ubyte ShadeR;
  ubyte Flags;
  short Alt; /**< offs=0x06 Ground element altitude (at the beginning of the tile). */
  short Child; /**< offs=0x08 Index of a first Thing in a chain of things located on this element. */
  ushort ColHead; /**< offs=0x0A Index of ColVectList for this tile. */
  ushort Ambient; /**< offs=0x0C Background illumination. */
  ubyte Zip;
  ubyte Flags2;
  ushort ColumnHead; /**< offs=0x10 Index of ColColumn for this tile, storing collision properties. */
};

struct MyMapElementOldV7 { // sizeof=24
  ushort Texture; /**< offs=0x00 Index of SingleFloorTexture for this tile, and texture flags. */
  ushort Shade;
  ubyte ShadeR;
  ubyte Flags;
  ushort mefield_6;
  ushort mefield_8;
  short Alt; /**< offs=0x0A Ground element altitude. */
  ubyte Zip;
  ubyte Flags2;
  short Child; /**< offs=0x0E Index of a first Thing in a chain of things located on this element. */
  ushort ColHead; /**< offs=0x10 Index of ColVectList for this tile. */
  ushort Ambient;
  ushort mefield_14;
  ushort ColumnHead; /**< offs=0x16 Index of ColColumn for this tile, storing collision properties. */
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

void clear_mapwho_on_whole_map(void);
void refresh_old_my_big_map_format(struct MyMapElement *p_mapel,
 struct MyMapElementOldV7 *p_oldmapel, ulong fmtver);
short get_mapwho_thing_index(short tile_x, short tile_z);
void init_search_spiral(void);
int alt_at_point(short x, short z);

/** Returns maximal change of altitude between corners of given tile.
 */
int alt_change_at_tile(short tile_x, short tile_z, int *change_xz);

/** Sets some of the map elements flags based on other properties.
 */
void update_map_flags(void);

void quick_crater(int x, int z, int size);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
