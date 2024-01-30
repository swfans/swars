/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginsngtxtr.c
 *     Engine single objects required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling single objects in 3D world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginsngtxtr.h"

#include "bfmemut.h"
#include "swlog.h"
/******************************************************************************/

/** Checks whether rexture with specified index is in use within given SingleFloorTexture.
 */
TbBool floor_texture_is_from_index(struct SingleFloorTexture *p_fltextr, int index)
{
    short beg_x, end_x;
    short beg_y, end_y;

    beg_x = (index % 8) * 32;
    end_x = beg_x + 31;
    beg_y = (index / 8) * 32;
    end_y = beg_y + 31;

    // Comparing 2 points is enough to conclude that only the exact texture is in use

    if (p_fltextr->TMapX1 < beg_x || p_fltextr->TMapX1 > end_x)
        return false;

    if (p_fltextr->TMapY1 < beg_y || p_fltextr->TMapY1 > end_y)
        return false;

    if (p_fltextr->TMapX3 < beg_x || p_fltextr->TMapX3 > end_x)
        return false;

    if (p_fltextr->TMapY3 < beg_y || p_fltextr->TMapY3 > end_y)
        return false;

    return true;
}

/** Rotates texture within SingleFloorTexture, by 90, 180 or 270 degrees.
 */
void floor_texture_rotate(struct SingleFloorTexture *p_fltextr, int rot)
{
    ushort r;

    for (r = rot; r > 0; r--) {
        ushort tmpval;

        tmpval = p_fltextr->TMapX1;
        p_fltextr->TMapX1 = p_fltextr->TMapX2;
        p_fltextr->TMapX2 = p_fltextr->TMapX3;
        p_fltextr->TMapX3 = p_fltextr->TMapX4;
        p_fltextr->TMapX4 = tmpval;

        tmpval = p_fltextr->TMapY1;
        p_fltextr->TMapY1 = p_fltextr->TMapY2;
        p_fltextr->TMapY2 = p_fltextr->TMapY3;
        p_fltextr->TMapY3 = p_fltextr->TMapY4;
        p_fltextr->TMapY4 = tmpval;
    }
}

/** Switches floor texture to given index, without losing rotation.
 */
void floor_texture_switch_to_index(struct SingleFloorTexture *p_fltextr, int index)
{
    short beg_x, beg_y;

    beg_x = (index % 8) * 32;
    beg_y = (index / 8) * 32;

    p_fltextr->TMapX1 = beg_x + (p_fltextr->TMapX1 % 32);
    p_fltextr->TMapX2 = beg_x + (p_fltextr->TMapX2 % 32);
    p_fltextr->TMapX3 = beg_x + (p_fltextr->TMapX3 % 32);
    p_fltextr->TMapX4 = beg_x + (p_fltextr->TMapX4 % 32);

    p_fltextr->TMapX1 = beg_y + (p_fltextr->TMapX1 % 32);
    p_fltextr->TMapX2 = beg_y + (p_fltextr->TMapX2 % 32);
    p_fltextr->TMapX3 = beg_y + (p_fltextr->TMapX3 % 32);
    p_fltextr->TMapX4 = beg_y + (p_fltextr->TMapX4 % 32);
}

void refresh_old_floor_texture_format(struct SingleFloorTexture *p_fltextr,
  struct SingleFloorTexture *p_oldfltextr, ulong fmtver)
{
    LbMemoryCopy(p_fltextr, p_oldfltextr, sizeof(struct SingleFloorTexture));

    if (p_fltextr->Page == 0)
    {
        if (floor_texture_is_from_index(p_fltextr, 3)) {
            floor_texture_switch_to_index(p_fltextr, 2);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 6)) {
            floor_texture_switch_to_index(p_fltextr, 4);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 7)) {
            floor_texture_switch_to_index(p_fltextr, 5);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 19)) {
            floor_texture_switch_to_index(p_fltextr, 18);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 22)) {
            floor_texture_switch_to_index(p_fltextr, 20);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 23)) {
            floor_texture_switch_to_index(p_fltextr, 21);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 44)) {
            floor_texture_switch_to_index(p_fltextr, 42);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 45)) {
            floor_texture_switch_to_index(p_fltextr, 43);
            floor_texture_rotate(p_fltextr, 2);
        } else
        if (floor_texture_is_from_index(p_fltextr, 46)) {
            floor_texture_rotate(p_fltextr, 2);
        } else
        if (floor_texture_is_from_index(p_fltextr, 57)) {
            // No such texture - selecting closest match
            floor_texture_switch_to_index(p_fltextr, 63);
        } else
        if (floor_texture_is_from_index(p_fltextr, 60)) {
            floor_texture_switch_to_index(p_fltextr, 58);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_from_index(p_fltextr, 62)) {
            floor_texture_switch_to_index(p_fltextr, 61);
        }
    }
}

/******************************************************************************/
