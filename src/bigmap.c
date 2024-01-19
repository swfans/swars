/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bigmap.c
 *     Routines implementing map elements reference lists.
 * @par Purpose:
 *     Implement functions for handling bigmap handling.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bigmap.h"

#include <stdlib.h>
#include "bfmemut.h"
#include "swlog.h"
/******************************************************************************/
struct MapOffset spiral_step[SPIRAL_STEPS_COUNT];
ushort dist_tiles_to_spiral_step[MAP_TILE_WIDTH];
ushort spiral_dist_tiles_limit = 0;

void clear_mapwho_on_whole_map(void)
{
    short tile_x, tile_z;
    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            ulong cellno;
            cellno = tile_z * MAP_TILE_WIDTH + tile_x;
            game_my_big_map[cellno].Child = 0;
        }
    }
}

short get_mapwho_thing_index(short tile_x, short tile_z)
{
    struct MyMapElement *mapel;

    if ((tile_x < 0) || (tile_x >= MAP_TILE_WIDTH))
        return 0;
    if ((tile_z < 0) || (tile_z >= MAP_TILE_HEIGHT))
        return 0;

    mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];

    return mapel->Child;
}

/** Maps fields from old MyMapElement struct to the current one.
 */
void refresh_old_my_big_map_format(struct MyMapElement *p_mapel, struct MyMapElementOldV7 *p_oldmapel, ulong fmtver)
{
    //TODO make sane matching for old fields
    LbMemoryCopy(p_mapel, p_oldmapel, 18);

    p_mapel->Alt = p_oldmapel->Alt;
    p_mapel->Child = p_oldmapel->Child;
    p_mapel->ColHead = p_oldmapel->ColHead;
    p_mapel->ColumnHead = p_oldmapel->ColumnHead;

    if (fmtver <= 9)
    {
        if (p_mapel->Texture & 0x8000) {
            short k;
            k = p_mapel->Texture;
            k = -k;
            k = (k & 0xFF) | ((k - (0x80 << 8)) & 0xFF00);
            p_mapel->Texture = k;
        }
    } else if (fmtver <= 14)
    {
        //TODO some textures need fixing here, ie. road-walkway transition
    }
}

void init_search_spiral_steps(void)
{
    struct MapOffset *sstep;
    long x,y;
    long i;

    y = 0;
    x = 0;
    sstep = &spiral_step[0];
    sstep->h = y;
    sstep->v = x;
    sstep->both = (short)y + ((short)x << 8);
    y = -1;
    x = -1;
    for (i=1; i < SPIRAL_STEPS_COUNT; i++)
    {
      sstep = &spiral_step[i];
      sstep->h = y;
      sstep->v = x;
      sstep->both = (short)y + ((short)x * MAP_TILE_WIDTH);
      if ((y < 0) && (x-y == 1))
      {
          y--;
          x -= 2;
      } else
      if (x == y)
      {
          if (y < 0)
            y++;
          else
            y--;
      } else
      if (y+x == 0)
      {
          if (x >= 0)
            x--;
          else
            x++;
      } else
      if (abs(x) >= abs(y))
      {
          if (x < 0)
            y++;
          else
            y--;
      } else
      {
          if (y >= 0)
            x++;
          else
            x--;
      }
    }
}

void init_dist_to_spiral_steps(void)
{
    struct MapOffset *sstep;
    long i, ssi;
    ulong dist, distss;

    ssi = SPIRAL_STEPS_COUNT-1;
    for (i = MAP_TILE_WIDTH; i > 0; i--)
    {
        dist = (i * i) * 2;
        for (; ssi > 0; ssi--) {
            sstep = &spiral_step[ssi];
            distss = (sstep->h * sstep->h) + (sstep->v * sstep->v);
            if (distss < dist) {
                break;
            }
        }
        dist_tiles_to_spiral_step[i-1] = ssi + 1;
    }
    for (i = 0; i < MAP_TILE_WIDTH; i++) {
        if (dist_tiles_to_spiral_step[i] >= SPIRAL_STEPS_COUNT)
            break;
    }
    spiral_dist_tiles_limit = i;
}

void init_search_spiral(void)
{
    init_search_spiral_steps();
    init_dist_to_spiral_steps();
    LOGSYNC("Created for distance up to %hu tiles", spiral_dist_tiles_limit);
}

int alt_at_point(short x, short z)
{
    int ret;
    asm volatile ("call ASM_alt_at_point\n"
        : "=r" (ret) : "a" (x), "d" (z));
    return ret;
}

/******************************************************************************/
