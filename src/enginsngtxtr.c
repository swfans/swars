/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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

ushort next_anim_tmap = 10;

ushort next_face_texture = 1;

ushort next_floor_texture = 1;

ushort next_local_mat = 1;

/******************************************************************************/

/** Convert given texture index into page+U+V coordinates in texture atlas.
 */
void texture_index_to_page_u_v(ubyte *p_page, ubyte *p_tmap_u, ubyte *p_tmap_v, int index)
{
    *p_page = index / (8 * 8);
    *p_tmap_u = (index % 8) * 32;
    *p_tmap_v = ((index / 8) % 8) * 32;
}

/** Convert page+U+V coordinates from texture atlas into single texture index.
 */
int texture_page_u_v_to_index(ubyte page, ubyte tmap_u, ubyte tmap_v)
{
    int index;

    index = page * (8 * 8);
    index += (tmap_v / 32) * 8;
    index += (tmap_u / 32);
    return index;
}

/** Checks whether texture with specified index is the only in use within given SingleFloorTexture.
 */
TbBool floor_texture_is_only_using_index(struct SingleFloorTexture *p_fltextr, int index)
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

/** Checks whether specified index is the lowest texture in use within given SingleFloorTexture.
 *
 * Note that this function does not care if the SingleFloorTexture spans multiple
 * textures or only a single one.
 */
TbBool floor_texture_starts_within_index(struct SingleFloorTexture *p_fltextr, int index)
{
    short beg_x, end_x;
    short beg_y, end_y;

    beg_x = (index % 8) * 32;
    end_x = beg_x + 31;
    beg_y = (index / 8) * 32;
    end_y = beg_y + 31;

    // Fail if any lower texture is in use

    if (p_fltextr->TMapY1 < beg_y || p_fltextr->TMapY2 < beg_y
      || p_fltextr->TMapY3 < beg_y || p_fltextr->TMapY4 < beg_y)
        return false;

    if (p_fltextr->TMapX1 < beg_x || p_fltextr->TMapX2 < beg_x
      || p_fltextr->TMapX3 < beg_x || p_fltextr->TMapX4 < beg_x)
        return false;

    // Fail if only higher textures are in use

    if (p_fltextr->TMapX1 > end_x && p_fltextr->TMapX2 > end_x
      && p_fltextr->TMapX3 > end_x && p_fltextr->TMapX4 > end_x)
        return false;

    if (p_fltextr->TMapY1 > end_y && p_fltextr->TMapY2 > end_y
      && p_fltextr->TMapY3 > end_y && p_fltextr->TMapY4 > end_y)
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
    short prev_beg_x, prev_beg_y;

    beg_x = (index % 8) * 32;
    beg_y = (index / 8) * 32;

    prev_beg_x = p_fltextr->TMapX1;
    if (p_fltextr->TMapX2 < prev_beg_x)
        prev_beg_x = p_fltextr->TMapX2;
    if (p_fltextr->TMapX3 < prev_beg_x)
        prev_beg_x = p_fltextr->TMapX3;
    if (p_fltextr->TMapX4 < prev_beg_x)
        prev_beg_x = p_fltextr->TMapX4;
    prev_beg_x -= (prev_beg_x % 32);

    prev_beg_y = p_fltextr->TMapY1;
    if (p_fltextr->TMapY2 < prev_beg_y)
        prev_beg_y = p_fltextr->TMapY2;
    if (p_fltextr->TMapY3 < prev_beg_y)
        prev_beg_y = p_fltextr->TMapY3;
    if (p_fltextr->TMapY4 < prev_beg_y)
        prev_beg_y = p_fltextr->TMapY4;
    prev_beg_y -= (prev_beg_y % 32);

    p_fltextr->TMapX1 = beg_x + (p_fltextr->TMapX1 - prev_beg_x);
    p_fltextr->TMapX2 = beg_x + (p_fltextr->TMapX2 - prev_beg_x);
    p_fltextr->TMapX3 = beg_x + (p_fltextr->TMapX3 - prev_beg_x);
    p_fltextr->TMapX4 = beg_x + (p_fltextr->TMapX4 - prev_beg_x);

    p_fltextr->TMapY1 = beg_y + (p_fltextr->TMapY1 - prev_beg_y);
    p_fltextr->TMapY2 = beg_y + (p_fltextr->TMapY2 - prev_beg_y);
    p_fltextr->TMapY3 = beg_y + (p_fltextr->TMapY3 - prev_beg_y);
    p_fltextr->TMapY4 = beg_y + (p_fltextr->TMapY4 - prev_beg_y);
}

void refresh_old_floor_texture_format(struct SingleFloorTexture *p_fltextr,
  struct SingleFloorTexture *p_oldfltextr, ulong fmtver)
{
    LbMemoryCopy(p_fltextr, p_oldfltextr, sizeof(struct SingleFloorTexture));

    if (p_fltextr->Page == 0)
    {
        if (floor_texture_is_only_using_index(p_fltextr, 3)) {
            floor_texture_switch_to_index(p_fltextr, 2);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 6)) {
            floor_texture_switch_to_index(p_fltextr, 4);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 7)) {
            floor_texture_switch_to_index(p_fltextr, 5);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 19)) {
            floor_texture_switch_to_index(p_fltextr, 18);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 22)) {
            floor_texture_switch_to_index(p_fltextr, 20);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 23)) {
            floor_texture_switch_to_index(p_fltextr, 21);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 44)) {
            floor_texture_switch_to_index(p_fltextr, 42);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 45)) {
            floor_texture_switch_to_index(p_fltextr, 43);
            floor_texture_rotate(p_fltextr, 2);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 46)) {
            floor_texture_rotate(p_fltextr, 2);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 57)) {
            // No such texture - selecting closest match
            floor_texture_switch_to_index(p_fltextr, 63);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 60)) {
            floor_texture_switch_to_index(p_fltextr, 58);
            floor_texture_rotate(p_fltextr, 3);
        } else
        if (floor_texture_is_only_using_index(p_fltextr, 62)) {
            floor_texture_switch_to_index(p_fltextr, 61);
        }
    } else
    if (p_fltextr->Page == 5)
    {
        if (floor_texture_starts_within_index(p_fltextr, 0)) {
            p_fltextr->Page = 4;
            floor_texture_switch_to_index(p_fltextr, 42);
        } else
        if (floor_texture_starts_within_index(p_fltextr, 1)) {
            p_fltextr->Page = 4;
            floor_texture_switch_to_index(p_fltextr, 43);
        } else
        if (floor_texture_starts_within_index(p_fltextr, 2)) {
            p_fltextr->Page = 4;
            floor_texture_switch_to_index(p_fltextr, 44);
        } else
        if (floor_texture_starts_within_index(p_fltextr, 8)) {
            p_fltextr->Page = 4;
            floor_texture_switch_to_index(p_fltextr, 50);
        } else
        if (floor_texture_starts_within_index(p_fltextr, 9)) {
            p_fltextr->Page = 4;
            floor_texture_switch_to_index(p_fltextr, 51);
        } else
        if (floor_texture_starts_within_index(p_fltextr, 10)) {
            p_fltextr->Page = 4;
            floor_texture_switch_to_index(p_fltextr, 52);
        }
    }
}

/** Checks whether texture with specified index is the only in use within given SingleTexture.
 */
TbBool face_texture_is_only_using_index(struct SingleTexture *p_fctextr, int index)
{
    short beg_x, end_x;
    short beg_y, end_y;

    beg_x = (index % 8) * 32;
    end_x = beg_x + 31;
    beg_y = (index / 8) * 32;
    end_y = beg_y + 31;

    // Comparing 2 points is enough to conclude that only the exact texture is in use

    if (p_fctextr->TMapX1 < beg_x || p_fctextr->TMapX1 > end_x)
        return false;

    if (p_fctextr->TMapY1 < beg_y || p_fctextr->TMapY1 > end_y)
        return false;

    if (p_fctextr->TMapX3 < beg_x || p_fctextr->TMapX3 > end_x)
        return false;

    if (p_fctextr->TMapY3 < beg_y || p_fctextr->TMapY3 > end_y)
        return false;

    return true;
}

/** Checks whether specified index is the lowest texture in use within given SingleTexture.
 *
 * Note that this function does not care if the SingleTexture spans multiple
 * textures or only a single one.
 */
TbBool face_texture_starts_within_index(struct SingleTexture *p_fctextr, int index)
{
    short beg_x, end_x;
    short beg_y, end_y;

    beg_x = (index % 8) * 32;
    end_x = beg_x + 31;
    beg_y = (index / 8) * 32;
    end_y = beg_y + 31;

    // Fail if any lower texture is in use

    if (p_fctextr->TMapY1 < beg_y || p_fctextr->TMapY2 < beg_y || p_fctextr->TMapY3 < beg_y)
        return false;

    if (p_fctextr->TMapX1 < beg_x || p_fctextr->TMapX2 < beg_x || p_fctextr->TMapX3 < beg_x)
        return false;

    // Fail if only higher textures are in use

    if (p_fctextr->TMapX1 > end_x && p_fctextr->TMapX2 > end_x && p_fctextr->TMapX3 > end_x)
        return false;

    if (p_fctextr->TMapY1 > end_y && p_fctextr->TMapY2 > end_y && p_fctextr->TMapY3 > end_y)
        return false;

    return true;
}

/** Switches face3 texture to given index, without losing rotation.
 */
void face_texture_switch_to_index(struct SingleTexture *p_fctextr, int index)
{
    short beg_x, beg_y;
    short prev_beg_x, prev_beg_y;

    beg_x = (index % 8) * 32;
    beg_y = (index / 8) * 32;

    prev_beg_x = p_fctextr->TMapX1;
    if (p_fctextr->TMapX2 < prev_beg_x)
        prev_beg_x = p_fctextr->TMapX2;
    if (p_fctextr->TMapX3 < prev_beg_x)
        prev_beg_x = p_fctextr->TMapX3;
    prev_beg_x -= (prev_beg_x % 32);

    prev_beg_y = p_fctextr->TMapY1;
    if (p_fctextr->TMapY2 < prev_beg_y)
        prev_beg_y = p_fctextr->TMapY2;
    if (p_fctextr->TMapY3 < prev_beg_y)
        prev_beg_y = p_fctextr->TMapY3;
    prev_beg_y -= (prev_beg_y % 32);

    p_fctextr->TMapX1 = beg_x + (p_fctextr->TMapX1 - prev_beg_x);
    p_fctextr->TMapX2 = beg_x + (p_fctextr->TMapX2 - prev_beg_x);
    p_fctextr->TMapX3 = beg_x + (p_fctextr->TMapX3 - prev_beg_x);

    p_fctextr->TMapY1 = beg_y + (p_fctextr->TMapY1 - prev_beg_y);
    p_fctextr->TMapY2 = beg_y + (p_fctextr->TMapY2 - prev_beg_y);
    p_fctextr->TMapY3 = beg_y + (p_fctextr->TMapY3 - prev_beg_y);
}

void animate_textures(void)
{
    asm volatile ("call ASM_animate_textures\n"
        :  :  : "eax" );
    return;
}

void refresh_old_face_texture_format(struct SingleTexture *p_fctextr,
  struct SingleTexture *p_oldfctextr, ulong fmtver)
{
    LbMemoryCopy(p_fctextr, p_oldfctextr, sizeof(struct SingleTexture));

    if (p_fctextr->Page == 5)
    {
        if (face_texture_starts_within_index(p_fctextr, 0)) {
            p_fctextr->Page = 4;
            face_texture_switch_to_index(p_fctextr, 42);
        } else
        if (face_texture_starts_within_index(p_fctextr, 1)) {
            p_fctextr->Page = 4;
            face_texture_switch_to_index(p_fctextr, 43);
        } else
        if (face_texture_starts_within_index(p_fctextr, 2)) {
            p_fctextr->Page = 4;
            face_texture_switch_to_index(p_fctextr, 44);
        } else
        if (face_texture_starts_within_index(p_fctextr, 8)) {
            p_fctextr->Page = 4;
            face_texture_switch_to_index(p_fctextr, 50);
        } else
        if (face_texture_starts_within_index(p_fctextr, 9)) {
            p_fctextr->Page = 4;
            face_texture_switch_to_index(p_fctextr, 51);
        } else
        if (face_texture_starts_within_index(p_fctextr, 10)) {
            p_fctextr->Page = 4;
            face_texture_switch_to_index(p_fctextr, 52);
        }
    }
}

/******************************************************************************/
