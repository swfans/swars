/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvdraw3d.c
 *     Routines for level and map drawing using 3D rendering.
 * @par Purpose:
 *     Implement functions for creating drawlist for rendering the 3D view.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     24 Dec 2023 - 10 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "lvdraw3d.h"

#include <assert.h>
#include "bfmath.h"
#include "bfplanar.h"

#include "display.h"
#include "enginzoom.h"
#include "swlog.h"
/******************************************************************************/
extern long dword_152E50;
extern long dword_152E54;
extern long dword_152E58;

extern long dword_176CC0;


ubyte lvdraw_fill_limit_points(struct TbPoint *limit_pts)
{
    int fctr_x1, fctr_z1, fctr_x2, fctr_z2;
    int mag_x, mag_z;
    ubyte slt, slt_zmin;
    ushort angl;
    int sin_angl, cos_angl;

    angl = (engn_anglexz >> 5) & 0x7FF;
    sin_angl = lbSinTable[angl];
    cos_angl = lbSinTable[angl + 512];

    fctr_x1 = (dword_152E58 * render_area_a * cos_angl) >> 16;
    fctr_z2 = (dword_152E58 * render_area_a * -sin_angl) >> 16;
    fctr_z1 = (render_area_b * cos_angl) >> 8;
    fctr_x2 = (render_area_b * sin_angl) >> 8;
    mag_z = engn_zc & 0xFF00;
    mag_x = engn_xc & 0xFF00;

    limit_pts[0].y = (mag_z + ((dword_152E50 * (fctr_z1 + fctr_z2)) >> 8)) >> 8;
    limit_pts[1].y = (mag_z + ((dword_152E54 * (fctr_z2 - fctr_z1)) >> 8)) >> 8;
    limit_pts[1].x = (mag_x + ((dword_152E54 * (fctr_x1 - fctr_x2)) >> 8)) >> 8;
    limit_pts[2].x = (mag_x + ((dword_152E54 * (-fctr_x1 - fctr_x2)) >> 8)) >> 8;
    limit_pts[0].x = (mag_x + ((dword_152E50 * (fctr_x2 + fctr_x1)) >> 8)) >> 8;
    limit_pts[2].y = (mag_z + ((dword_152E54 * (-fctr_z2 - fctr_z1)) >> 8)) >> 8;
    limit_pts[3].y = (mag_z + ((dword_152E50 * (fctr_z1 - fctr_z2)) >> 8)) >> 8;
    limit_pts[3].x = (mag_x + ((dword_152E50 * (fctr_x2 - fctr_x1)) >> 8)) >> 8;

    slt_zmin = 0;
    for (slt = 1; slt < 4; slt++) {
        if (limit_pts[slt].y < limit_pts[slt_zmin].y)
            slt_zmin = slt;
    }

    return slt_zmin;
}

void func_2e440(void)
{
#if 1
    asm volatile ("call ASM_func_2e440\n"
        :  :  : "eax" );
    return;
#endif
}

/******************************************************************************/
