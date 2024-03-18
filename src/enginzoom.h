/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginzoom.h
 *     Header file for enginzoom.c.
 * @par Purpose:
 *     Zoom level handling for the 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Sep 2023 - 17 Mar 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ENGINZOOM_H
#define ENGINZOOM_H

#include "bftypes.h"
#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#pragma pack()
/******************************************************************************/
/** Minimum user zoom (when most area is visible). */
extern short user_zoom_min;
/** Maxumum user zoom (largest magnification). */
extern short user_zoom_max;

ushort get_scaled_zoom(ushort zoom);
int get_weapon_zoom_min(ushort wtype);

short get_overall_scale_min(void);
short get_overall_scale_max(void);

short get_render_area_for_zoom(short zoom);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
