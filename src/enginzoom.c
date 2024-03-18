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
#define RENDER_AREA_MIN 2
#define RENDER_AREA_MAX 63

short user_zoom_min = 127;
short user_zoom_max = 260;

ushort zoom_levels[] = {
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
    for (i = 0; i < 28; i++)
    {
        int n, val;
        // The curve is steeper in 2nd half than in first half
        if (i > 14)
            n = 15;
        else
            n = 8;
        val = zoom_arr_min + ((dt * n * i) >> 8);
        if (val > zoom_arr_max)
            val = zoom_arr_max;
        zoom_levels[27-i] = val;
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
    return rarea;
}

short get_render_area_for_zoom(short zoom)
{
    short rarea;
    int w, h;
    // Original value was 30 for zoom=127 and screen proportion 4:3.
    // Conclusions from tests by Moburma:
    // 4:3 - For every decrease of ten units of zoom, render area must be
    // increased by four units to make up for the now visible render edge.
    // 16:9 - had to increase 2 more render area units per 10 zoom.
    rarea = 81 - zoom * 2 / 5;
    // The 2 more for 16:9 (let's use 2 for over 16:10)
    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    if (h > w) {
        h = lbDisplay.GraphicsScreenWidth;
        w = lbDisplay.GraphicsScreenHeight;
    }
    rarea += (w - h * 4 / 3) * 10 / (h * (16/8));
    // In low resolution, use even lower render area
    if (h < 400)
        rarea  = rarea * 4 / 5;
    return bound_render_area(rarea);
}

/******************************************************************************/
