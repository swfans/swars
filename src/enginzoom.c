/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginzoom.c
 *     Zoom level handling for the 3D engine.
 * @par Purpose:
 *     Implements functions for modifying zoom level and its limits.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Sep 2023 - 17 Mar 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginzoom.h"

#include "bfutility.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/

/** Smallest size of render area. Must be even to avoid shifted terrain.
 */
#define RENDER_AREA_MIN 2

/** Largest size of render area. Must be even to avoid shifted terrain.
 */
#define RENDER_AREA_MAX 88

short user_zoom_min = 127;
short user_zoom_max = 260;

ushort zoom_levels[WEAPON_RANGE_BLOCKS_LIMIT+1] = {
    256, 256, 256, 256, 256, 256, 240, 230,
    220, 210, 200, 190, 180, 170, 170, 170,
    170, 170, 170, 170, 170, 165, 160, 155,
    150, 145, 140, 135,
};

void zoom_update(short zoom_min, short zoom_max)
{
    int i, dt;
    int zoom_arr_min, zoom_arr_max;

    dt = (zoom_max - zoom_min);
    zoom_arr_min = zoom_min + ((dt * 16) >> 8);
    zoom_arr_max = zoom_max - ((dt * 8) >> 8);
    for (i = 0; i < WEAPON_RANGE_BLOCKS_LIMIT+1; i++)
    {
        int n, val;
        // The curve has 3 steepness levels: from right, first 9, then 2, then 21
        val = zoom_arr_min;
        if (i > 0) {
            if (i < 7)
                n = 9 * (i - 0);
            else
                n = 9 * (7 - 0);
            val += ((dt * n) >> 8);
        }

        if (i > 7) {
            if (i < 15)
                n = 2 * (i - 7);
            else
                n = 2 * (15 - 7);
            val += ((dt * n) >> 8);
        }

        if (i > 15) {
            n = 21 * (i - 15);
            val += ((dt * n) >> 8);
        }

        if (val > zoom_arr_max)
            val = zoom_arr_max;
        zoom_levels[WEAPON_RANGE_BLOCKS_LIMIT-i] = val;
    }
    user_zoom_min = zoom_min;
    user_zoom_max = zoom_max;
}

ushort get_scaled_zoom(ushort zoom)
{
    short h;

    h = min(lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
    if (h < 400)
        return zoom;
    else
        return  h * zoom / 240;
}

int get_weapon_zoom_min(ushort wtype)
{
    struct WeaponDef *wdef;
    int zoom;

    wdef = &weapon_defs[wtype];
    zoom = zoom_levels[wdef->RangeBlocks];
    if (zoom > user_zoom_max)
        zoom = user_zoom_max;
    if (zoom < user_zoom_min)
        zoom = user_zoom_min;
    return zoom;
}

short get_overall_scale_min(void)
{
    return get_scaled_zoom(user_zoom_min);
}

short get_overall_scale_max(void)
{
    return get_scaled_zoom(user_zoom_max);
}

short bound_render_area(short rarea)
{
    if (rarea < RENDER_AREA_MIN)
        rarea = RENDER_AREA_MIN;
    if (rarea > RENDER_AREA_MAX)
        rarea = RENDER_AREA_MAX;
    // Odd size of the area would produce shifted terrain
    if (rarea & 1) rarea++;
    return rarea;
}

short get_render_area_for_zoom(short zoom)
{
    short rarea;
    int w, h, dt;
    // Original value was 30 for zoom=127 and screen proportion 4:3.
    // The algorithm below was found experimentally, by lowering
    // camera angle on London map and finding value at which view
    // corner is black when ground bevel edge is near that corner.
    dt = 14;
    // Increase delta by 1 for 16:9 (round to over 16:10)
    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    if (h > w) {
        h = lbDisplay.GraphicsScreenWidth;
        w = lbDisplay.GraphicsScreenHeight;
    }
    dt += (w - h * 4 / 3) * 10 / (h * (16/4));
    // Compute the area
    rarea = ((106*14/4) - zoom) * 4 / dt;
    // In low resolution, use even lower render area
    if (h < 400)
        rarea  = rarea * 4 / 5;
    LOGSYNC("Render area %hd dt %d zoom %hd", rarea, dt, zoom);
    return bound_render_area(rarea);
}

/******************************************************************************/
