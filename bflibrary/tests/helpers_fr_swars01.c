/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_fr_swars01.c
 *     Draws a single frame from the game Syndicate Wars.
 *     The frame is IML Link from city Adelaide.
 * @par Purpose:
 *     Testing implementation of bflibrary routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 May 2022 - 29 Jun 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "poly.h"
#include "bftstlog.h"

/******************************************************************************/

void test_frame_swars01(void)
{
    {vec_mode=5;
    struct PolyPoint pA = {639, -14, 0x0, 0x5f0000, 0x84c00};
    struct PolyPoint pB = {676, -32, 0x0, 0x400000, 0xe7400};
    struct PolyPoint pC = {684, 3, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {684, 3, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {676, -32, 0x0, 0x400000, 0xe7400};
    struct PolyPoint pC = {721, -14, 0x1f0000, 0x400000, 0x84c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {601, 3, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {639, -14, 0x0, 0x0, 0x84c00};
    struct PolyPoint pC = {646, 23, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {646, 23, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {639, -14, 0x0, 0x0, 0x84c00};
    struct PolyPoint pC = {684, 3, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {558, -14, 0x0, 0x1f0000, 0x50000};
    struct PolyPoint pB = {595, -32, 0x0, 0x0, 0x6f800};
    struct PolyPoint pC = {601, 3, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {601, 3, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {595, -32, 0x0, 0x0, 0x6f800};
    struct PolyPoint pC = {639, -14, 0x1f0000, 0x0, 0x84c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {607, 42, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {646, 23, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {653, 61, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {653, 61, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {646, 23, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {692, 42, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {563, 23, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {601, 3, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {607, 42, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {607, 42, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {601, 3, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {646, 23, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {519, 3, 0x0, 0x1f0000, 0x50000};
    struct PolyPoint pB = {558, -14, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {563, 23, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {563, 23, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {558, -14, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {601, 3, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {476, -14, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {515, -32, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {519, 3, 0x1f0000, 0x1f0000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {519, 3, 0x1f0000, 0x1f0000, 0x50000};
    struct PolyPoint pB = {515, -32, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {558, -14, 0x1f0000, 0x0, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {568, 61, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {607, 42, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {613, 80, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {613, 80, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {607, 42, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {653, 61, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {523, 42, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {563, 23, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {568, 61, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {568, 61, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {563, 23, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {607, 42, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {479, 23, 0x0, 0x1f0000, 0x50000};
    struct PolyPoint pB = {519, 3, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {523, 42, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {523, 42, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {519, 3, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {563, 23, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {436, 3, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {476, -14, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {479, 23, 0x1f0000, 0x1f0000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {479, 23, 0x1f0000, 0x1f0000, 0x50000};
    struct PolyPoint pB = {476, -14, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {519, 3, 0x1f0000, 0x0, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {483, 61, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {523, 42, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {527, 80, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {527, 80, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {523, 42, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {568, 61, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {439, 42, 0x0, 0x1f0000, 0x50000};
    struct PolyPoint pB = {479, 23, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {483, 61, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {483, 61, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {479, 23, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {523, 42, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {396, 23, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {436, 3, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {439, 42, 0x1f0000, 0x1f0000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {439, 42, 0x1f0000, 0x1f0000, 0x50000};
    struct PolyPoint pB = {436, 3, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {479, 23, 0x1f0000, 0x0, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {397, 61, 0x0, 0x5f0000, 0x50000};
    struct PolyPoint pB = {439, 42, 0x0, 0x400000, 0x50000};
    struct PolyPoint pC = {441, 80, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {441, 80, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {439, 42, 0x0, 0x400000, 0x50000};
    struct PolyPoint pC = {483, 61, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {354, 42, 0x0, 0x5f0000, 0x38000};
    struct PolyPoint pB = {396, 23, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {397, 61, 0x1f0000, 0x5f0000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {397, 61, 0x1f0000, 0x5f0000, 0x50000};
    struct PolyPoint pB = {396, 23, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {439, 42, 0x1f0000, 0x400000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 23, 0x0, 0x5f0000, 0x38000};
    struct PolyPoint pB = {354, 3, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {354, 42, 0x1f0000, 0x5f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {354, 42, 0x1f0000, 0x5f0000, 0x38000};
    struct PolyPoint pB = {354, 3, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {396, 23, 0x1f0000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 61, 0x0, 0x5f0000, 0x38000};
    struct PolyPoint pB = {354, 42, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {355, 80, 0x1f0000, 0x5f0000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {355, 80, 0x1f0000, 0x5f0000, 0x50000};
    struct PolyPoint pB = {354, 42, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {397, 61, 0x1f0000, 0x400000, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {270, 42, 0x0, 0x5f0000, 0x38000};
    struct PolyPoint pB = {312, 23, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {312, 61, 0x1f0000, 0x5f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 61, 0x1f0000, 0x5f0000, 0x38000};
    struct PolyPoint pB = {312, 23, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {354, 42, 0x1f0000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {143, 61, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {187, 42, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {184, 80, 0x1f0000, 0x1f0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {184, 80, 0x1f0000, 0x1f0000, 0x20000};
    struct PolyPoint pB = {187, 42, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {227, 61, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {613, 80, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {653, 61, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {660, 101, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {660, 101, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {653, 61, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {700, 80, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {527, 80, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {568, 61, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {573, 101, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {573, 101, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {568, 61, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {613, 80, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {441, 80, 0x0, 0x5f0000, 0x60000};
    struct PolyPoint pB = {483, 61, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {486, 101, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {486, 101, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {483, 61, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {527, 80, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {355, 80, 0x0, 0x5f0000, 0x50000};
    struct PolyPoint pB = {397, 61, 0x0, 0x400000, 0x50000};
    struct PolyPoint pC = {399, 101, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {399, 101, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {397, 61, 0x0, 0x400000, 0x50000};
    struct PolyPoint pC = {441, 80, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {269, 80, 0x0, 0x1f0000, 0x68000};
    struct PolyPoint pB = {312, 61, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {312, 101, 0x1f0000, 0x1f0000, 0x98000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 101, 0x1f0000, 0x1f0000, 0x98000};
    struct PolyPoint pB = {312, 61, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {355, 80, 0x1f0000, 0x0, 0x50000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {184, 80, 0x0, 0x1f0000, 0x20000};
    struct PolyPoint pB = {227, 61, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {225, 101, 0x1f0000, 0x1f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {225, 101, 0x1f0000, 0x1f0000, 0x38000};
    struct PolyPoint pB = {227, 61, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {269, 80, 0x1f0000, 0x0, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {573, 101, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {613, 80, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {619, 121, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {619, 121, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {613, 80, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {660, 101, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {486, 101, 0x0, 0x5f0000, 0x60000};
    struct PolyPoint pB = {527, 80, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {532, 121, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {532, 121, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {527, 80, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {573, 101, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {399, 101, 0x0, 0x5f0000, 0x60000};
    struct PolyPoint pB = {441, 80, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {444, 121, 0x1f0000, 0x5f0000, 0x82800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {444, 121, 0x1f0000, 0x5f0000, 0x82800};
    struct PolyPoint pB = {441, 80, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {486, 101, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 101, 0x0, 0x1f0000, 0x98000};
    struct PolyPoint pB = {355, 80, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {356, 121, 0x1f0000, 0x1f0000, 0xe0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {356, 121, 0x1f0000, 0x1f0000, 0xe0400};
    struct PolyPoint pB = {355, 80, 0x0, 0x0, 0x50000};
    struct PolyPoint pC = {399, 101, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {225, 101, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {269, 80, 0x0, 0x0, 0x68000};
    struct PolyPoint pC = {268, 121, 0x1f0000, 0x1f0000, 0x6cc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {268, 121, 0x1f0000, 0x1f0000, 0x6cc00};
    struct PolyPoint pB = {269, 80, 0x0, 0x0, 0x68000};
    struct PolyPoint pC = {312, 101, 0x1f0000, 0x0, 0x98000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {619, 121, 0x0, 0x1f0000, 0x60000};
    struct PolyPoint pB = {660, 101, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {667, 142, 0x1f0000, 0x1f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {667, 142, 0x1f0000, 0x1f0000, 0x60000};
    struct PolyPoint pB = {660, 101, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {707, 121, 0x1f0000, 0x0, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {532, 121, 0x0, 0x5f0000, 0x60000};
    struct PolyPoint pB = {573, 101, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {578, 142, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {578, 142, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {573, 101, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {619, 121, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {444, 121, 0x0, 0x5f0000, 0x82800};
    struct PolyPoint pB = {486, 101, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {489, 142, 0x1f0000, 0x5f0000, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {489, 142, 0x1f0000, 0x5f0000, 0x88400};
    struct PolyPoint pB = {486, 101, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {532, 121, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {356, 121, 0x0, 0x1f0000, 0xe0400};
    struct PolyPoint pB = {399, 101, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {401, 142, 0x1f0000, 0x1f0000, 0xebc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {401, 142, 0x1f0000, 0x1f0000, 0xebc00};
    struct PolyPoint pB = {399, 101, 0x0, 0x0, 0x60000};
    struct PolyPoint pC = {444, 121, 0x1f0000, 0x0, 0x82800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {268, 121, 0x0, 0x1f0000, 0x6cc00};
    struct PolyPoint pB = {312, 101, 0x0, 0x0, 0x98000};
    struct PolyPoint pC = {312, 142, 0x1f0000, 0x1f0000, 0x93c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 142, 0x1f0000, 0x1f0000, 0x93c00};
    struct PolyPoint pB = {312, 101, 0x0, 0x0, 0x98000};
    struct PolyPoint pC = {356, 121, 0x1f0000, 0x0, 0xe0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {489, 142, 0x0, 0x5f0000, 0x88400};
    struct PolyPoint pB = {532, 121, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {536, 165, 0x1f0000, 0x5f0000, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {536, 165, 0x1f0000, 0x5f0000, 0x88400};
    struct PolyPoint pB = {532, 121, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {578, 142, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {401, 142, 0x0, 0x1f0000, 0xebc00};
    struct PolyPoint pB = {444, 121, 0x0, 0x0, 0x82800};
    struct PolyPoint pC = {446, 165, 0x1f0000, 0x1f0000, 0xf1800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {446, 165, 0x1f0000, 0x1f0000, 0xf1800};
    struct PolyPoint pB = {444, 121, 0x0, 0x0, 0x82800};
    struct PolyPoint pC = {489, 142, 0x1f0000, 0x0, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 142, 0x0, 0x1f0000, 0x93c00};
    struct PolyPoint pB = {356, 121, 0x0, 0x0, 0xe0400};
    struct PolyPoint pC = {357, 165, 0x1f0000, 0x1f0000, 0xa5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {357, 165, 0x1f0000, 0x1f0000, 0xa5000};
    struct PolyPoint pB = {356, 121, 0x0, 0x0, 0xe0400};
    struct PolyPoint pC = {401, 142, 0x1f0000, 0x0, 0xebc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {625, 165, 0x0, 0x5f0000, 0x60000};
    struct PolyPoint pB = {667, 142, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {674, 186, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {674, 186, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {667, 142, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {715, 165, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {536, 165, 0x0, 0x5f0000, 0x88400};
    struct PolyPoint pB = {578, 142, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {583, 186, 0x1f0000, 0x5f0000, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {583, 186, 0x1f0000, 0x5f0000, 0x88400};
    struct PolyPoint pB = {578, 142, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {625, 165, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {357, 165, 0x0, 0x1f0000, 0xa5000};
    struct PolyPoint pB = {401, 142, 0x0, 0x0, 0xebc00};
    struct PolyPoint pC = {402, 186, 0x1f0000, 0x1f0000, 0xbc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {402, 186, 0x1f0000, 0x1f0000, 0xbc000};
    struct PolyPoint pB = {401, 142, 0x0, 0x0, 0xebc00};
    struct PolyPoint pC = {446, 165, 0x1f0000, 0x0, 0xf1800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {583, 186, 0x0, 0x5f0000, 0x88400};
    struct PolyPoint pB = {625, 165, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {631, 209, 0x1f0000, 0x5f0000, 0xe0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {631, 209, 0x1f0000, 0x5f0000, 0xe0400};
    struct PolyPoint pB = {625, 165, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {674, 186, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {402, 186, 0x0, 0x1f0000, 0xbc000};
    struct PolyPoint pB = {446, 165, 0x0, 0x0, 0xf1800};
    struct PolyPoint pC = {449, 209, 0x1f0000, 0x1f0000, 0xc7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {449, 209, 0x1f0000, 0x1f0000, 0xc7800};
    struct PolyPoint pB = {446, 165, 0x0, 0x0, 0xf1800};
    struct PolyPoint pC = {493, 186, 0x1f0000, 0x0, 0xf7400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-49, 186, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {-1, 165, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {-7, 209, 0x1f0000, 0x1f0000, 0x79400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-7, 209, 0x1f0000, 0x1f0000, 0x79400};
    struct PolyPoint pB = {-1, 165, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {41, 186, 0x1f0000, 0x0, 0x79400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {631, 209, 0x0, 0x5f0000, 0xe0400};
    struct PolyPoint pB = {674, 186, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {681, 231, 0x1f0000, 0x5f0000, 0xfd000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {681, 231, 0x1f0000, 0x5f0000, 0xfd000};
    struct PolyPoint pB = {674, 186, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {723, 209, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {449, 209, 0x0, 0x1f0000, 0xc7800};
    struct PolyPoint pB = {493, 186, 0x0, 0x0, 0xf7400};
    struct PolyPoint pC = {496, 231, 0x1f0000, 0x1f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {496, 231, 0x1f0000, 0x1f0000, 0xea000};
    struct PolyPoint pB = {493, 186, 0x0, 0x0, 0xf7400};
    struct PolyPoint pC = {540, 209, 0x1f0000, 0x0, 0xf7400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {84, 209, 0x0, 0x1f0000, 0x7e000};
    struct PolyPoint pB = {132, 186, 0x0, 0x0, 0x7e000};
    struct PolyPoint pC = {128, 231, 0x1f0000, 0x1f0000, 0xb6400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {128, 231, 0x1f0000, 0x1f0000, 0xb6400};
    struct PolyPoint pB = {132, 186, 0x0, 0x0, 0x7e000};
    struct PolyPoint pC = {175, 209, 0x1f0000, 0x0, 0xbc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-7, 209, 0x0, 0x1f0000, 0x79400};
    struct PolyPoint pB = {41, 186, 0x0, 0x0, 0x79400};
    struct PolyPoint pC = {36, 231, 0x1f0000, 0x1f0000, 0xc0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {36, 231, 0x1f0000, 0x1f0000, 0xc0400};
    struct PolyPoint pB = {41, 186, 0x0, 0x0, 0x79400};
    struct PolyPoint pC = {84, 209, 0x1f0000, 0x0, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {496, 231, 0x0, 0x1f0000, 0xea000};
    struct PolyPoint pB = {540, 209, 0x0, 0x0, 0xf7400};
    struct PolyPoint pC = {544, 255, 0x1f0000, 0x1f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {544, 255, 0x1f0000, 0x1f0000, 0xea000};
    struct PolyPoint pB = {540, 209, 0x0, 0x0, 0xf7400};
    struct PolyPoint pC = {588, 231, 0x1f0000, 0x0, 0x119c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {128, 231, 0x0, 0x1f0000, 0xb6400};
    struct PolyPoint pB = {175, 209, 0x0, 0x0, 0xbc000};
    struct PolyPoint pC = {173, 255, 0x1f0000, 0x1f0000, 0xefc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {173, 255, 0x1f0000, 0x1f0000, 0xefc00};
    struct PolyPoint pB = {175, 209, 0x0, 0x0, 0xbc000};
    struct PolyPoint pC = {219, 231, 0x1f0000, 0x0, 0xfb400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {36, 231, 0x0, 0x1f0000, 0xc0400};
    struct PolyPoint pB = {84, 209, 0x0, 0x0, 0x7e000};
    struct PolyPoint pC = {80, 255, 0x1f0000, 0x1f0000, 0xaac00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {80, 255, 0x1f0000, 0x1f0000, 0xaac00};
    struct PolyPoint pB = {84, 209, 0x0, 0x0, 0x7e000};
    struct PolyPoint pC = {128, 231, 0x1f0000, 0x0, 0xb6400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-57, 231, 0x0, 0x1f0000, 0x68000};
    struct PolyPoint pB = {-7, 209, 0x0, 0x0, 0x79400};
    struct PolyPoint pC = {-14, 255, 0x1f0000, 0x1f0000, 0xb4c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-14, 255, 0x1f0000, 0x1f0000, 0xb4c00};
    struct PolyPoint pB = {-7, 209, 0x0, 0x0, 0x79400};
    struct PolyPoint pC = {36, 231, 0x1f0000, 0x0, 0xc0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {544, 255, 0x0, 0x1f0000, 0xea000};
    struct PolyPoint pB = {588, 231, 0x0, 0x0, 0x119c00};
    struct PolyPoint pC = {593, 278, 0x1f0000, 0x1f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {593, 278, 0x1f0000, 0x1f0000, 0xea000};
    struct PolyPoint pB = {588, 231, 0x0, 0x0, 0x119c00};
    struct PolyPoint pC = {637, 255, 0x1f0000, 0x0, 0x119c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {451, 255, 0x0, 0x1f0000, 0x134c00};
    struct PolyPoint pB = {496, 231, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {499, 278, 0x1f0000, 0x1f0000, 0x129400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {499, 278, 0x1f0000, 0x1f0000, 0x129400};
    struct PolyPoint pB = {496, 231, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {544, 255, 0x1f0000, 0x0, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {173, 255, 0x0, 0x1f0000, 0xefc00};
    struct PolyPoint pB = {219, 231, 0x0, 0x0, 0xfb400};
    struct PolyPoint pC = {217, 278, 0x1f0000, 0x1f0000, 0x151800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {217, 278, 0x1f0000, 0x1f0000, 0x151800};
    struct PolyPoint pB = {219, 231, 0x0, 0x0, 0xfb400};
    struct PolyPoint pC = {265, 255, 0x1f0000, 0x0, 0x16e400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {80, 255, 0x0, 0x1f0000, 0xaac00};
    struct PolyPoint pB = {128, 231, 0x0, 0x0, 0xb6400};
    struct PolyPoint pC = {124, 278, 0x1f0000, 0x1f0000, 0xd3000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {124, 278, 0x1f0000, 0x1f0000, 0xd3000};
    struct PolyPoint pB = {128, 231, 0x0, 0x0, 0xb6400};
    struct PolyPoint pC = {173, 255, 0x1f0000, 0x0, 0xefc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-14, 255, 0x0, 0x1f0000, 0xb4c00};
    struct PolyPoint pB = {36, 231, 0x0, 0x0, 0xc0400};
    struct PolyPoint pC = {30, 278, 0x1f0000, 0x1f0000, 0x99800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {30, 278, 0x1f0000, 0x1f0000, 0x99800};
    struct PolyPoint pB = {36, 231, 0x0, 0x0, 0xc0400};
    struct PolyPoint pC = {80, 255, 0x1f0000, 0x0, 0xaac00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {593, 278, 0x0, 0x1f0000, 0xea000};
    struct PolyPoint pB = {637, 255, 0x0, 0x0, 0x119c00};
    struct PolyPoint pC = {643, 302, 0x1f0000, 0x1f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {643, 302, 0x1f0000, 0x1f0000, 0xea000};
    struct PolyPoint pB = {637, 255, 0x0, 0x0, 0x119c00};
    struct PolyPoint pC = {687, 278, 0x1f0000, 0x0, 0x119c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {499, 278, 0x0, 0x1f0000, 0x129400};
    struct PolyPoint pB = {544, 255, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {548, 302, 0x1f0000, 0x1f0000, 0x11dc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {548, 302, 0x1f0000, 0x1f0000, 0x11dc00};
    struct PolyPoint pB = {544, 255, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {593, 278, 0x1f0000, 0x0, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {405, 278, 0x0, 0x1f0000, 0x1b9000};
    struct PolyPoint pB = {451, 255, 0x0, 0x0, 0x134c00};
    struct PolyPoint pC = {454, 302, 0x1f0000, 0x1f0000, 0x179c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {454, 302, 0x1f0000, 0x1f0000, 0x179c00};
    struct PolyPoint pB = {451, 255, 0x0, 0x0, 0x134c00};
    struct PolyPoint pC = {499, 278, 0x1f0000, 0x0, 0x129400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {217, 278, 0x0, 0x1f0000, 0x151800};
    struct PolyPoint pB = {265, 255, 0x0, 0x0, 0x16e400};
    struct PolyPoint pC = {264, 302, 0x1f0000, 0x1f0000, 0x20f400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {264, 302, 0x1f0000, 0x1f0000, 0x20f400};
    struct PolyPoint pB = {265, 255, 0x0, 0x0, 0x16e400};
    struct PolyPoint pC = {311, 278, 0x1f0000, 0x0, 0x22c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {124, 278, 0x0, 0x1f0000, 0xd3000};
    struct PolyPoint pB = {173, 255, 0x0, 0x0, 0xefc00};
    struct PolyPoint pC = {170, 302, 0x1f0000, 0x1f0000, 0x12f000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {170, 302, 0x1f0000, 0x1f0000, 0x12f000};
    struct PolyPoint pB = {173, 255, 0x0, 0x0, 0xefc00};
    struct PolyPoint pC = {217, 278, 0x1f0000, 0x0, 0x151800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {30, 278, 0x0, 0x1f0000, 0x99800};
    struct PolyPoint pB = {80, 255, 0x0, 0x0, 0xaac00};
    struct PolyPoint pC = {75, 302, 0x1f0000, 0x1f0000, 0xd8c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 302, 0x1f0000, 0x1f0000, 0xd8c00};
    struct PolyPoint pB = {80, 255, 0x0, 0x0, 0xaac00};
    struct PolyPoint pC = {124, 278, 0x1f0000, 0x0, 0xd3000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-64, 278, 0x0, 0x1f0000, 0xaf000};
    struct PolyPoint pB = {-14, 255, 0x0, 0x0, 0xb4c00};
    struct PolyPoint pC = {-20, 302, 0x1f0000, 0x1f0000, 0xb0800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-20, 302, 0x1f0000, 0x1f0000, 0xb0800};
    struct PolyPoint pB = {-14, 255, 0x0, 0x0, 0xb4c00};
    struct PolyPoint pC = {30, 278, 0x1f0000, 0x0, 0x99800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {548, 302, 0x0, 0x1f0000, 0x11dc00};
    struct PolyPoint pB = {593, 278, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {598, 328, 0x1f0000, 0x1f0000, 0x181400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {598, 328, 0x1f0000, 0x1f0000, 0x181400};
    struct PolyPoint pB = {593, 278, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {643, 302, 0x1f0000, 0x0, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {454, 302, 0x0, 0x1f0000, 0x179c00};
    struct PolyPoint pB = {499, 278, 0x0, 0x0, 0x129400};
    struct PolyPoint pC = {503, 328, 0x1f0000, 0x1f0000, 0x162c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {503, 328, 0x1f0000, 0x1f0000, 0x162c00};
    struct PolyPoint pB = {499, 278, 0x0, 0x0, 0x129400};
    struct PolyPoint pC = {548, 302, 0x1f0000, 0x0, 0x11dc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {359, 302, 0x0, 0x1f0000, 0x22c000};
    struct PolyPoint pB = {405, 278, 0x0, 0x0, 0x1b9000};
    struct PolyPoint pC = {407, 328, 0x1f0000, 0x1f0000, 0x1b9000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {407, 328, 0x1f0000, 0x1f0000, 0x1b9000};
    struct PolyPoint pB = {405, 278, 0x0, 0x0, 0x1b9000};
    struct PolyPoint pC = {454, 302, 0x1f0000, 0x0, 0x179c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {264, 302, 0x0, 0x1f0000, 0x20f400};
    struct PolyPoint pB = {311, 278, 0x0, 0x0, 0x22c000};
    struct PolyPoint pC = {311, 328, 0x1f0000, 0x1f0000, 0x26d000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 328, 0x1f0000, 0x1f0000, 0x26d000};
    struct PolyPoint pB = {311, 278, 0x0, 0x0, 0x22c000};
    struct PolyPoint pC = {359, 302, 0x1f0000, 0x0, 0x22c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {170, 302, 0x0, 0x1f0000, 0x12f000};
    struct PolyPoint pB = {217, 278, 0x0, 0x0, 0x151800};
    struct PolyPoint pC = {215, 328, 0x1f0000, 0x1f0000, 0x185400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {215, 328, 0x1f0000, 0x1f0000, 0x185400};
    struct PolyPoint pB = {217, 278, 0x0, 0x0, 0x151800};
    struct PolyPoint pC = {264, 302, 0x1f0000, 0x0, 0x20f400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 302, 0x0, 0x1f0000, 0xd8c00};
    struct PolyPoint pB = {124, 278, 0x0, 0x0, 0xd3000};
    struct PolyPoint pC = {121, 328, 0x1f0000, 0x1f0000, 0x101000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {121, 328, 0x1f0000, 0x1f0000, 0x101000};
    struct PolyPoint pB = {124, 278, 0x0, 0x0, 0xd3000};
    struct PolyPoint pC = {170, 302, 0x1f0000, 0x0, 0x12f000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-20, 302, 0x0, 0x1f0000, 0xb0800};
    struct PolyPoint pB = {30, 278, 0x0, 0x0, 0x99800};
    struct PolyPoint pC = {25, 328, 0x1f0000, 0x1f0000, 0xc7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {25, 328, 0x1f0000, 0x1f0000, 0xc7800};
    struct PolyPoint pB = {30, 278, 0x0, 0x0, 0x99800};
    struct PolyPoint pC = {75, 302, 0x1f0000, 0x0, 0xd8c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {503, 328, 0x0, 0x1f0000, 0x162c00};
    struct PolyPoint pB = {548, 302, 0x0, 0x0, 0x11dc00};
    struct PolyPoint pC = {553, 353, 0x1f0000, 0x1f0000, 0x1e3000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {553, 353, 0x1f0000, 0x1f0000, 0x1e3000};
    struct PolyPoint pB = {548, 302, 0x0, 0x0, 0x11dc00};
    struct PolyPoint pC = {598, 328, 0x1f0000, 0x0, 0x181400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {407, 328, 0x0, 0x1f0000, 0x1b9000};
    struct PolyPoint pB = {454, 302, 0x0, 0x0, 0x179c00};
    struct PolyPoint pC = {456, 353, 0x1f0000, 0x1f0000, 0x190c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {456, 353, 0x1f0000, 0x1f0000, 0x190c00};
    struct PolyPoint pB = {454, 302, 0x0, 0x0, 0x179c00};
    struct PolyPoint pC = {503, 328, 0x1f0000, 0x0, 0x162c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 328, 0x0, 0x1f0000, 0x26d000};
    struct PolyPoint pB = {359, 302, 0x0, 0x0, 0x22c000};
    struct PolyPoint pC = {359, 353, 0x1f0000, 0x1f0000, 0x1c4800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {359, 353, 0x1f0000, 0x1f0000, 0x1c4800};
    struct PolyPoint pB = {359, 302, 0x0, 0x0, 0x22c000};
    struct PolyPoint pC = {407, 328, 0x1f0000, 0x0, 0x1b9000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {121, 328, 0x0, 0x1f0000, 0x101000};
    struct PolyPoint pB = {170, 302, 0x0, 0x0, 0x12f000};
    struct PolyPoint pC = {167, 353, 0x1f0000, 0x1f0000, 0x146000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 353, 0x1f0000, 0x1f0000, 0x146000};
    struct PolyPoint pB = {170, 302, 0x0, 0x0, 0x12f000};
    struct PolyPoint pC = {215, 328, 0x1f0000, 0x0, 0x185400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {25, 328, 0x0, 0x1f0000, 0xc7800};
    struct PolyPoint pB = {75, 302, 0x0, 0x0, 0xd8c00};
    struct PolyPoint pC = {71, 353, 0x1f0000, 0x1f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {71, 353, 0x1f0000, 0x1f0000, 0xea000};
    struct PolyPoint pB = {75, 302, 0x0, 0x0, 0xd8c00};
    struct PolyPoint pC = {121, 328, 0x1f0000, 0x0, 0x101000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-71, 328, 0x0, 0x1f0000, 0xa5000};
    struct PolyPoint pB = {-20, 302, 0x0, 0x0, 0xb0800};
    struct PolyPoint pC = {-26, 353, 0x1f0000, 0x1f0000, 0xbc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-26, 353, 0x1f0000, 0x1f0000, 0xbc000};
    struct PolyPoint pB = {-20, 302, 0x0, 0x0, 0xb0800};
    struct PolyPoint pC = {25, 328, 0x1f0000, 0x0, 0xc7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {456, 353, 0x0, 0x1f0000, 0x190c00};
    struct PolyPoint pB = {503, 328, 0x0, 0x0, 0x162c00};
    struct PolyPoint pC = {506, 378, 0x1f0000, 0x1f0000, 0x228000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {506, 378, 0x1f0000, 0x1f0000, 0x228000};
    struct PolyPoint pB = {503, 328, 0x0, 0x0, 0x162c00};
    struct PolyPoint pC = {553, 353, 0x1f0000, 0x0, 0x1e3000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {359, 353, 0x0, 0x1f0000, 0x1c4800};
    struct PolyPoint pB = {407, 328, 0x0, 0x0, 0x1b9000};
    struct PolyPoint pC = {409, 378, 0x1f0000, 0x1f0000, 0x1a7c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {409, 378, 0x1f0000, 0x1f0000, 0x1a7c00};
    struct PolyPoint pB = {407, 328, 0x0, 0x0, 0x1b9000};
    struct PolyPoint pC = {456, 353, 0x1f0000, 0x0, 0x190c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {71, 353, 0x0, 0x1f0000, 0xea000};
    struct PolyPoint pB = {121, 328, 0x0, 0x0, 0x101000};
    struct PolyPoint pC = {117, 378, 0x1f0000, 0x1f0000, 0x10c800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {117, 378, 0x1f0000, 0x1f0000, 0x10c800};
    struct PolyPoint pB = {121, 328, 0x0, 0x0, 0x101000};
    struct PolyPoint pC = {167, 353, 0x1f0000, 0x0, 0x146000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-26, 353, 0x0, 0x1f0000, 0xbc000};
    struct PolyPoint pB = {25, 328, 0x0, 0x0, 0xc7800};
    struct PolyPoint pC = {19, 378, 0x1f0000, 0x1f0000, 0xde800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {19, 378, 0x1f0000, 0x1f0000, 0xde800};
    struct PolyPoint pB = {25, 328, 0x0, 0x0, 0xc7800};
    struct PolyPoint pC = {71, 353, 0x1f0000, 0x0, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {409, 378, 0x0, 0x1f0000, 0x1a7c00};
    struct PolyPoint pB = {456, 353, 0x0, 0x0, 0x190c00};
    struct PolyPoint pC = {459, 404, 0x1f0000, 0x1f0000, 0x233800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {459, 404, 0x1f0000, 0x1f0000, 0x233800};
    struct PolyPoint pB = {456, 353, 0x0, 0x0, 0x190c00};
    struct PolyPoint pC = {506, 378, 0x1f0000, 0x0, 0x228000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {213, 378, 0x0, 0x5f0000, 0x15d000};
    struct PolyPoint pB = {263, 353, 0x0, 0x400000, 0x1f4400};
    struct PolyPoint pC = {262, 404, 0x1f0000, 0x5f0000, 0x16e400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {262, 404, 0x1f0000, 0x5f0000, 0x16e400};
    struct PolyPoint pB = {263, 353, 0x0, 0x400000, 0x1f4400};
    struct PolyPoint pC = {311, 378, 0x1f0000, 0x400000, 0x1e3000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {19, 378, 0x0, 0x1f0000, 0xde800};
    struct PolyPoint pB = {71, 353, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {66, 404, 0x1f0000, 0x1f0000, 0x112400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {66, 404, 0x1f0000, 0x1f0000, 0x112400};
    struct PolyPoint pB = {71, 353, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {117, 378, 0x1f0000, 0x0, 0x10c800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-78, 378, 0x0, 0x1f0000, 0x99800};
    struct PolyPoint pB = {-26, 353, 0x0, 0x0, 0xbc000};
    struct PolyPoint pC = {-32, 404, 0x1f0000, 0x1f0000, 0xd8c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-32, 404, 0x1f0000, 0x1f0000, 0xd8c00};
    struct PolyPoint pB = {-26, 353, 0x0, 0x0, 0xbc000};
    struct PolyPoint pC = {19, 378, 0x1f0000, 0x0, 0xde800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {459, 404, 0x0, 0x1f0000, 0x233800};
    struct PolyPoint pB = {506, 378, 0x0, 0x0, 0x228000};
    struct PolyPoint pC = {509, 430, 0x1f0000, 0x1f0000, 0x27e400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {509, 430, 0x1f0000, 0x1f0000, 0x27e400};
    struct PolyPoint pB = {506, 378, 0x0, 0x0, 0x228000};
    struct PolyPoint pC = {557, 404, 0x1f0000, 0x0, 0x295400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {262, 404, 0x0, 0x5f0000, 0x16e400};
    struct PolyPoint pB = {311, 378, 0x0, 0x400000, 0x1e3000};
    struct PolyPoint pC = {311, 430, 0x1f0000, 0x5f0000, 0x168800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 430, 0x1f0000, 0x5f0000, 0x168800};
    struct PolyPoint pB = {311, 378, 0x0, 0x400000, 0x1e3000};
    struct PolyPoint pC = {360, 404, 0x1f0000, 0x400000, 0x1d7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {164, 404, 0x0, 0x1f0000, 0x157400};
    struct PolyPoint pB = {213, 378, 0x0, 0x0, 0x15d000};
    struct PolyPoint pC = {212, 430, 0x1f0000, 0x1f0000, 0x17f800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {212, 430, 0x1f0000, 0x1f0000, 0x17f800};
    struct PolyPoint pB = {213, 378, 0x0, 0x0, 0x15d000};
    struct PolyPoint pC = {262, 404, 0x1f0000, 0x0, 0x16e400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-32, 404, 0x0, 0x1f0000, 0xd8c00};
    struct PolyPoint pB = {19, 378, 0x0, 0x0, 0xde800};
    struct PolyPoint pC = {14, 430, 0x1f0000, 0x1f0000, 0x118000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {14, 430, 0x1f0000, 0x1f0000, 0x118000};
    struct PolyPoint pB = {19, 378, 0x0, 0x0, 0xde800};
    struct PolyPoint pC = {66, 404, 0x1f0000, 0x0, 0x112400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {609, 430, 0x0, 0x1f0000, 0x203c00};
    struct PolyPoint pB = {655, 404, 0x0, 0x0, 0x190c00};
    struct PolyPoint pC = {661, 457, 0x1f0000, 0x1f0000, 0x1bfc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {661, 457, 0x1f0000, 0x1f0000, 0x1bfc00};
    struct PolyPoint pB = {655, 404, 0x0, 0x0, 0x190c00};
    struct PolyPoint pC = {708, 430, 0x1f0000, 0x0, 0x191c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {509, 430, 0x0, 0x1f0000, 0x27e400};
    struct PolyPoint pB = {557, 404, 0x0, 0x0, 0x295400};
    struct PolyPoint pC = {561, 457, 0x1f0000, 0x1f0000, 0x1d5c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {561, 457, 0x1f0000, 0x1f0000, 0x1d5c00};
    struct PolyPoint pB = {557, 404, 0x0, 0x0, 0x295400};
    struct PolyPoint pC = {609, 430, 0x1f0000, 0x0, 0x203c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {212, 430, 0x0, 0x1f0000, 0x17f800};
    struct PolyPoint pB = {262, 404, 0x0, 0x0, 0x16e400};
    struct PolyPoint pC = {261, 457, 0x1f0000, 0x1f0000, 0x179c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {261, 457, 0x1f0000, 0x1f0000, 0x179c00};
    struct PolyPoint pB = {262, 404, 0x0, 0x0, 0x16e400};
    struct PolyPoint pC = {311, 430, 0x1f0000, 0x0, 0x168800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {113, 430, 0x0, 0x1f0000, 0x162c00};
    struct PolyPoint pB = {164, 404, 0x0, 0x0, 0x157400};
    struct PolyPoint pC = {162, 457, 0x1f0000, 0x1f0000, 0x1d0000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {162, 457, 0x1f0000, 0x1f0000, 0x1d0000};
    struct PolyPoint pB = {164, 404, 0x0, 0x0, 0x157400};
    struct PolyPoint pC = {212, 430, 0x1f0000, 0x0, 0x17f800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {561, 457, 0x0, 0x1f0000, 0x1d5c00};
    struct PolyPoint pB = {609, 430, 0x0, 0x0, 0x203c00};
    struct PolyPoint pC = {614, 484, 0x1f0000, 0x1f0000, 0x1ae800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {614, 484, 0x1f0000, 0x1f0000, 0x1ae800};
    struct PolyPoint pB = {609, 430, 0x0, 0x0, 0x203c00};
    struct PolyPoint pC = {661, 457, 0x1f0000, 0x0, 0x1bfc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {461, 457, 0x0, 0x1f0000, 0x1f4400};
    struct PolyPoint pB = {509, 430, 0x0, 0x0, 0x27e400};
    struct PolyPoint pC = {513, 484, 0x1f0000, 0x1f0000, 0x17f800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {513, 484, 0x1f0000, 0x1f0000, 0x17f800};
    struct PolyPoint pB = {509, 430, 0x0, 0x0, 0x27e400};
    struct PolyPoint pC = {561, 457, 0x1f0000, 0x0, 0x1d5c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {162, 457, 0x0, 0x1f0000, 0x1d0000};
    struct PolyPoint pB = {212, 430, 0x0, 0x0, 0x17f800};
    struct PolyPoint pC = {210, 484, 0x1f0000, 0x1f0000, 0x1d0000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {210, 484, 0x1f0000, 0x1f0000, 0x1d0000};
    struct PolyPoint pB = {212, 430, 0x0, 0x0, 0x17f800};
    struct PolyPoint pC = {261, 457, 0x1f0000, 0x0, 0x179c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {61, 457, 0x0, 0x1f0000, 0x185400};
    struct PolyPoint pB = {113, 430, 0x0, 0x0, 0x162c00};
    struct PolyPoint pC = {110, 484, 0x1f0000, 0x1f0000, 0x1f8400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {110, 484, 0x1f0000, 0x1f0000, 0x1f8400};
    struct PolyPoint pB = {113, 430, 0x0, 0x0, 0x162c00};
    struct PolyPoint pC = {162, 457, 0x1f0000, 0x0, 0x1d0000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {513, 484, 0x0, 0x1f0000, 0x17f800};
    struct PolyPoint pB = {561, 457, 0x0, 0x0, 0x1d5c00};
    struct PolyPoint pC = {565, 513, 0x1f0000, 0x1f0000, 0x16f400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {565, 513, 0x1f0000, 0x1f0000, 0x16f400};
    struct PolyPoint pB = {561, 457, 0x0, 0x0, 0x1d5c00};
    struct PolyPoint pC = {614, 484, 0x1f0000, 0x0, 0x1ae800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {412, 484, 0x0, 0x1f0000, 0x1a3c00};
    struct PolyPoint pB = {461, 457, 0x0, 0x0, 0x1f4400};
    struct PolyPoint pC = {464, 513, 0x1f0000, 0x1f0000, 0x112400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {464, 513, 0x1f0000, 0x1f0000, 0x112400};
    struct PolyPoint pB = {461, 457, 0x0, 0x0, 0x1f4400};
    struct PolyPoint pC = {513, 484, 0x1f0000, 0x0, 0x17f800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {110, 484, 0x0, 0x1f0000, 0x1f8400};
    struct PolyPoint pB = {162, 457, 0x0, 0x0, 0x1d0000};
    struct PolyPoint pC = {159, 513, 0x1f0000, 0x1f0000, 0x1f8400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {159, 513, 0x1f0000, 0x1f0000, 0x1f8400};
    struct PolyPoint pB = {162, 457, 0x0, 0x0, 0x1d0000};
    struct PolyPoint pC = {210, 484, 0x1f0000, 0x0, 0x1d0000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {9, 484, 0x0, 0x1f0000, 0x1a3c00};
    struct PolyPoint pB = {61, 457, 0x0, 0x0, 0x185400};
    struct PolyPoint pC = {57, 513, 0x1f0000, 0x1f0000, 0x216c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {57, 513, 0x1f0000, 0x1f0000, 0x216c00};
    struct PolyPoint pB = {61, 457, 0x0, 0x0, 0x185400};
    struct PolyPoint pC = {110, 484, 0x1f0000, 0x0, 0x1f8400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-92, 484, 0x0, 0x1f0000, 0x181400};
    struct PolyPoint pB = {-39, 457, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {-45, 513, 0x1f0000, 0x1f0000, 0x1b5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-45, 513, 0x1f0000, 0x1f0000, 0x1b5000};
    struct PolyPoint pB = {-39, 457, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {9, 484, 0x1f0000, 0x0, 0x1a3c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {394, -14, 0x0, 0x5f0000, 0x38000};
    struct PolyPoint pB = {434, -32, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {436, 3, 0x1f0000, 0x5f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {436, 3, 0x1f0000, 0x5f0000, 0x38000};
    struct PolyPoint pB = {434, -32, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {476, -14, 0x1f0000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, -14, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {353, -32, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {354, 3, 0x1f0000, 0x1f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {354, 3, 0x1f0000, 0x1f0000, 0x38000};
    struct PolyPoint pB = {353, -32, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {394, -14, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {354, 3, 0x0, 0x5f0000, 0x38000};
    struct PolyPoint pB = {394, -14, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {396, 23, 0x1f0000, 0x5f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {396, 23, 0x1f0000, 0x5f0000, 0x38000};
    struct PolyPoint pB = {394, -14, 0x0, 0x400000, 0x38000};
    struct PolyPoint pC = {436, 3, 0x1f0000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {271, 3, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {312, -14, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {312, 23, 0x1f0000, 0x1f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 23, 0x1f0000, 0x1f0000, 0x38000};
    struct PolyPoint pB = {312, -14, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {354, 3, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {229, 23, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {271, 3, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {270, 42, 0x1f0000, 0x1f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {270, 42, 0x1f0000, 0x1f0000, 0x38000};
    struct PolyPoint pB = {271, 3, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {312, 23, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {484, -86, 0x600000, 0x600000, 0x88000};
    struct PolyPoint pB = {687, -3, 0xa70000, 0x600000, 0xc4000};
    struct PolyPoint pC = {482, -14, 0x600000, 0x7f0000, 0x88000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {683, 70, 0xa70000, 0x7f0000, 0xc4000};
    struct PolyPoint pB = {482, -14, 0x600000, 0x7f0000, 0x88000};
    struct PolyPoint pC = {687, -3, 0xa70000, 0x600000, 0xc4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {187, 42, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {229, 23, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {227, 61, 0x1f0000, 0x1f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {227, 61, 0x1f0000, 0x1f0000, 0x38000};
    struct PolyPoint pB = {229, 23, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {270, 42, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {227, 61, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {270, 42, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {269, 80, 0x1f0000, 0x1f0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {269, 80, 0x1f0000, 0x1f0000, 0x68000};
    struct PolyPoint pB = {270, 42, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {312, 61, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {613, 32, 0xdf0000, 0x600000, 0xc4000};
    struct PolyPoint pB = {610, 106, 0xdf0000, 0x7f0000, 0xc4000};
    struct PolyPoint pC = {411, -53, 0x890000, 0x600000, 0x88000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {410, 18, 0x890000, 0x7e0000, 0x88000};
    struct PolyPoint pB = {411, -53, 0x890000, 0x600000, 0x88000};
    struct PolyPoint pC = {610, 106, 0xdf0000, 0x7f0000, 0xc4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {98, 80, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {143, 61, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {139, 101, 0x1f0000, 0x1f0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {139, 101, 0x1f0000, 0x1f0000, 0x20000};
    struct PolyPoint pB = {143, 61, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {184, 80, 0x1f0000, 0x0, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {616, -44, 0xdf0000, 0x600000, 0xf4000};
    struct PolyPoint pB = {613, 32, 0xdf0000, 0x7f0000, 0xc4000};
    struct PolyPoint pC = {412, -127, 0x890000, 0x600000, 0x88000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {411, -53, 0x890000, 0x7e0000, 0x88000};
    struct PolyPoint pB = {412, -127, 0x890000, 0x600000, 0x88000};
    struct PolyPoint pC = {613, 32, 0xdf0000, 0x7f0000, 0xc4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {139, 101, 0x0, 0x1f0000, 0x20000};
    struct PolyPoint pB = {184, 80, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {181, 121, 0x1f0000, 0x1f0000, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {181, 121, 0x1f0000, 0x1f0000, 0x49400};
    struct PolyPoint pB = {184, 80, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {225, 101, 0x1f0000, 0x0, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {52, 101, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {98, 80, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {93, 121, 0x1f0000, 0x1f0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {93, 121, 0x1f0000, 0x1f0000, 0x20000};
    struct PolyPoint pB = {98, 80, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {139, 101, 0x1f0000, 0x0, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {181, 121, 0x0, 0x1f0000, 0x49400};
    struct PolyPoint pB = {225, 101, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {223, 142, 0x1f0000, 0x1f0000, 0x72800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {223, 142, 0x1f0000, 0x1f0000, 0x72800};
    struct PolyPoint pB = {225, 101, 0x0, 0x0, 0x38000};
    struct PolyPoint pC = {268, 121, 0x1f0000, 0x0, 0x6cc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {93, 121, 0x0, 0x1f0000, 0x20000};
    struct PolyPoint pB = {139, 101, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {135, 142, 0x1f0000, 0x1f0000, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {135, 142, 0x1f0000, 0x1f0000, 0x49400};
    struct PolyPoint pB = {139, 101, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {181, 121, 0x1f0000, 0x0, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {5, 121, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {52, 101, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {47, 142, 0x1f0000, 0x1f0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {47, 142, 0x1f0000, 0x1f0000, 0x20000};
    struct PolyPoint pB = {52, 101, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {93, 121, 0x1f0000, 0x0, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {578, 142, 0x0, 0x5f0000, 0x60000};
    struct PolyPoint pB = {619, 121, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {625, 165, 0x1f0000, 0x5f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {625, 165, 0x1f0000, 0x5f0000, 0x60000};
    struct PolyPoint pB = {619, 121, 0x0, 0x400000, 0x60000};
    struct PolyPoint pC = {667, 142, 0x1f0000, 0x400000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {223, 142, 0x0, 0x1f0000, 0x72800};
    struct PolyPoint pB = {268, 121, 0x0, 0x0, 0x6cc00};
    struct PolyPoint pC = {267, 165, 0x1f0000, 0x1f0000, 0xa5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {267, 165, 0x1f0000, 0x1f0000, 0xa5000};
    struct PolyPoint pB = {268, 121, 0x0, 0x0, 0x6cc00};
    struct PolyPoint pC = {312, 142, 0x1f0000, 0x0, 0x93c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {135, 142, 0x0, 0x1f0000, 0x49400};
    struct PolyPoint pB = {181, 121, 0x0, 0x0, 0x49400};
    struct PolyPoint pC = {178, 165, 0x1f0000, 0x1f0000, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {178, 165, 0x1f0000, 0x1f0000, 0x7e000};
    struct PolyPoint pB = {181, 121, 0x0, 0x0, 0x49400};
    struct PolyPoint pC = {223, 142, 0x1f0000, 0x0, 0x72800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {47, 142, 0x0, 0x1f0000, 0x20000};
    struct PolyPoint pB = {93, 121, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {89, 165, 0x1f0000, 0x1f0000, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {89, 165, 0x1f0000, 0x1f0000, 0x49400};
    struct PolyPoint pB = {93, 121, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {135, 142, 0x1f0000, 0x0, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-42, 142, 0x0, 0x1f0000, 0x8000};
    struct PolyPoint pB = {5, 121, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {-1, 165, 0x1f0000, 0x1f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-1, 165, 0x1f0000, 0x1f0000, 0x38000};
    struct PolyPoint pB = {5, 121, 0x0, 0x0, 0x8000};
    struct PolyPoint pC = {47, 142, 0x1f0000, 0x0, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {610, 106, 0xe00000, 0x7f0000, 0xc4000};
    struct PolyPoint pB = {613, 32, 0xe00000, 0x600000, 0xc4000};
    struct PolyPoint pC = {829, 204, 0x860000, 0x7f0000, 0xc4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {834, 127, 0x860000, 0x600000, 0xc4000};
    struct PolyPoint pB = {829, 204, 0x860000, 0x7f0000, 0xc4000};
    struct PolyPoint pC = {613, 32, 0xe00000, 0x600000, 0xc4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {65, -4, 0x0, 0xc00000, 0x8000};
    struct PolyPoint pB = {104, 14, 0x0, 0xdf0000, 0x40000};
    struct PolyPoint pC = {19, 15, 0x1f0000, 0xc00000, 0x8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {57, 34, 0x1f0000, 0xdf0000, 0x40000};
    struct PolyPoint pB = {19, 15, 0x1f0000, 0xc00000, 0x8000};
    struct PolyPoint pC = {104, 14, 0x0, 0xdf0000, 0x40000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {446, 165, 0x0, 0x1f0000, 0xf1800};
    struct PolyPoint pB = {489, 142, 0x0, 0x0, 0x88400};
    struct PolyPoint pC = {493, 186, 0x1f0000, 0x1f0000, 0xf7400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {493, 186, 0x1f0000, 0x1f0000, 0xf7400};
    struct PolyPoint pB = {489, 142, 0x0, 0x0, 0x88400};
    struct PolyPoint pC = {536, 165, 0x1f0000, 0x0, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {267, 165, 0x0, 0x1f0000, 0xa5000};
    struct PolyPoint pB = {312, 142, 0x0, 0x0, 0x93c00};
    struct PolyPoint pC = {312, 186, 0x1f0000, 0x1f0000, 0xc1c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 186, 0x1f0000, 0x1f0000, 0xc1c00};
    struct PolyPoint pB = {312, 142, 0x0, 0x0, 0x93c00};
    struct PolyPoint pC = {357, 165, 0x1f0000, 0x0, 0xa5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {178, 165, 0x0, 0x1f0000, 0x7e000};
    struct PolyPoint pB = {223, 142, 0x0, 0x0, 0x72800};
    struct PolyPoint pC = {221, 186, 0x1f0000, 0x1f0000, 0xb6400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {221, 186, 0x1f0000, 0x1f0000, 0xb6400};
    struct PolyPoint pB = {223, 142, 0x0, 0x0, 0x72800};
    struct PolyPoint pC = {267, 165, 0x1f0000, 0x0, 0xa5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {89, 165, 0x0, 0x1f0000, 0x49400};
    struct PolyPoint pB = {135, 142, 0x0, 0x0, 0x49400};
    struct PolyPoint pC = {132, 186, 0x1f0000, 0x1f0000, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {132, 186, 0x1f0000, 0x1f0000, 0x7e000};
    struct PolyPoint pB = {135, 142, 0x0, 0x0, 0x49400};
    struct PolyPoint pC = {178, 165, 0x1f0000, 0x0, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-1, 165, 0x0, 0x1f0000, 0x38000};
    struct PolyPoint pB = {47, 142, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {41, 186, 0x1f0000, 0x1f0000, 0x79400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {41, 186, 0x1f0000, 0x1f0000, 0x79400};
    struct PolyPoint pB = {47, 142, 0x0, 0x0, 0x20000};
    struct PolyPoint pC = {89, 165, 0x1f0000, 0x0, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {104, 14, 0x0, 0xc00000, 0x40000};
    struct PolyPoint pB = {145, 34, 0x0, 0xdf0000, 0x68000};
    struct PolyPoint pC = {57, 34, 0x1f0000, 0xc00000, 0x40000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {100, 55, 0x1f0000, 0xdf0000, 0x68000};
    struct PolyPoint pB = {57, 34, 0x1f0000, 0xc00000, 0x40000};
    struct PolyPoint pC = {145, 34, 0x0, 0xdf0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {19, 15, 0x200000, 0xc00000, 0x8000};
    struct PolyPoint pB = {57, 34, 0x200000, 0xdf0000, 0x28000};
    struct PolyPoint pC = {-4, 53, 0x3f0000, 0xc00000, 0x8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {35, 73, 0x3f0000, 0xdf0000, 0x28000};
    struct PolyPoint pB = {-4, 53, 0x3f0000, 0xc00000, 0x8000};
    struct PolyPoint pC = {57, 34, 0x200000, 0xdf0000, 0x28000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {613, 32, 0x890000, 0x7e0000, 0xc4000};
    struct PolyPoint pB = {616, -44, 0x890000, 0x600000, 0xf4000};
    struct PolyPoint pC = {834, 127, 0xdf0000, 0x7f0000, 0xc4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {840, 49, 0xdf0000, 0x600000, 0xf4000};
    struct PolyPoint pB = {834, 127, 0xdf0000, 0x7f0000, 0xc4000};
    struct PolyPoint pC = {616, -44, 0x890000, 0x600000, 0xf4000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {493, 186, 0x0, 0x1f0000, 0xf7400};
    struct PolyPoint pB = {536, 165, 0x0, 0x0, 0x88400};
    struct PolyPoint pC = {540, 209, 0x1f0000, 0x1f0000, 0xf7400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {540, 209, 0x1f0000, 0x1f0000, 0xf7400};
    struct PolyPoint pB = {536, 165, 0x0, 0x0, 0x88400};
    struct PolyPoint pC = {583, 186, 0x1f0000, 0x0, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {312, 186, 0x0, 0x1f0000, 0xc1c00};
    struct PolyPoint pB = {357, 165, 0x0, 0x0, 0xa5000};
    struct PolyPoint pC = {357, 209, 0x1f0000, 0x1f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {357, 209, 0x1f0000, 0x1f0000, 0xea000};
    struct PolyPoint pB = {357, 165, 0x0, 0x0, 0xa5000};
    struct PolyPoint pC = {402, 186, 0x1f0000, 0x0, 0xbc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {221, 186, 0x0, 0x1f0000, 0xb6400};
    struct PolyPoint pB = {267, 165, 0x0, 0x0, 0xa5000};
    struct PolyPoint pC = {266, 209, 0x1f0000, 0x1f0000, 0x13c400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {266, 209, 0x1f0000, 0x1f0000, 0x13c400};
    struct PolyPoint pB = {267, 165, 0x0, 0x0, 0xa5000};
    struct PolyPoint pC = {312, 186, 0x1f0000, 0x0, 0xc1c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {132, 186, 0x0, 0x1f0000, 0x7e000};
    struct PolyPoint pB = {178, 165, 0x0, 0x0, 0x7e000};
    struct PolyPoint pC = {175, 209, 0x1f0000, 0x1f0000, 0xbc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {175, 209, 0x1f0000, 0x1f0000, 0xbc000};
    struct PolyPoint pB = {178, 165, 0x0, 0x0, 0x7e000};
    struct PolyPoint pC = {221, 186, 0x1f0000, 0x0, 0xb6400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {41, 186, 0x0, 0x1f0000, 0x79400};
    struct PolyPoint pB = {89, 165, 0x0, 0x0, 0x49400};
    struct PolyPoint pC = {84, 209, 0x1f0000, 0x1f0000, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {84, 209, 0x1f0000, 0x1f0000, 0x7e000};
    struct PolyPoint pB = {89, 165, 0x0, 0x0, 0x49400};
    struct PolyPoint pC = {132, 186, 0x1f0000, 0x0, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {145, 34, 0x0, 0xc00000, 0x68000};
    struct PolyPoint pB = {190, 55, 0x0, 0xdf0000, 0x68000};
    struct PolyPoint pC = {100, 55, 0x1f0000, 0xc00000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {143, 76, 0x1f0000, 0xdf0000, 0x68000};
    struct PolyPoint pB = {100, 55, 0x1f0000, 0xc00000, 0x68000};
    struct PolyPoint pC = {190, 55, 0x0, 0xdf0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {57, 34, 0x200000, 0xc00000, 0x28000};
    struct PolyPoint pB = {100, 55, 0x200000, 0xdf0000, 0x3c000};
    struct PolyPoint pC = {35, 73, 0x3f0000, 0xc00000, 0x28000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {77, 93, 0x3f0000, 0xdf0000, 0x3c000};
    struct PolyPoint pB = {35, 73, 0x3f0000, 0xc00000, 0x28000};
    struct PolyPoint pC = {100, 55, 0x200000, 0xdf0000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {616, -44, 0x0, 0x500000, 0x122800};
    struct PolyPoint pB = {636, -77, 0x0, 0x490000, 0xfc000};
    struct PolyPoint pC = {840, 49, 0x500000, 0x500000, 0xfc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {860, 14, 0x500000, 0x490000, 0xfc000};
    struct PolyPoint pB = {840, 49, 0x500000, 0x500000, 0xfc000};
    struct PolyPoint pC = {636, -77, 0x0, 0x490000, 0xfc000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {540, 209, 0x0, 0x1f0000, 0xf7400};
    struct PolyPoint pB = {583, 186, 0x0, 0x0, 0x88400};
    struct PolyPoint pC = {588, 231, 0x1f0000, 0x1f0000, 0x119c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {588, 231, 0x1f0000, 0x1f0000, 0x119c00};
    struct PolyPoint pB = {583, 186, 0x0, 0x0, 0x88400};
    struct PolyPoint pC = {631, 209, 0x1f0000, 0x0, 0xe0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {357, 209, 0x0, 0x1f0000, 0xea000};
    struct PolyPoint pB = {402, 186, 0x0, 0x0, 0xbc000};
    struct PolyPoint pC = {404, 231, 0x1f0000, 0x1f0000, 0x12f000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {404, 231, 0x1f0000, 0x1f0000, 0x12f000};
    struct PolyPoint pB = {402, 186, 0x0, 0x0, 0xbc000};
    struct PolyPoint pC = {449, 209, 0x1f0000, 0x0, 0xc7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {266, 209, 0x0, 0x1f0000, 0x13c400};
    struct PolyPoint pB = {312, 186, 0x0, 0x0, 0xc1c00};
    struct PolyPoint pC = {311, 231, 0x1f0000, 0x1f0000, 0x134c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 231, 0x1f0000, 0x1f0000, 0x134c00};
    struct PolyPoint pB = {312, 186, 0x0, 0x0, 0xc1c00};
    struct PolyPoint pC = {357, 209, 0x1f0000, 0x0, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {175, 209, 0x0, 0x1f0000, 0xbc000};
    struct PolyPoint pB = {221, 186, 0x0, 0x0, 0xb6400};
    struct PolyPoint pC = {219, 231, 0x1f0000, 0x1f0000, 0xfb400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {219, 231, 0x1f0000, 0x1f0000, 0xfb400};
    struct PolyPoint pB = {221, 186, 0x0, 0x0, 0xb6400};
    struct PolyPoint pC = {266, 209, 0x1f0000, 0x0, 0x13c400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {100, 55, 0x200000, 0xc00000, 0x3c000};
    struct PolyPoint pB = {143, 76, 0x200000, 0xdf0000, 0x3c000};
    struct PolyPoint pC = {77, 93, 0x3f0000, 0xc00000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {120, 115, 0x3f0000, 0xdf0000, 0x3c000};
    struct PolyPoint pB = {77, 93, 0x3f0000, 0xc00000, 0x3c000};
    struct PolyPoint pC = {143, 76, 0x200000, 0xdf0000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {190, 55, 0x0, 0xc00000, 0x68000};
    struct PolyPoint pB = {237, 77, 0x0, 0xdf0000, 0x68000};
    struct PolyPoint pC = {143, 76, 0x1f0000, 0xc00000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {190, 99, 0x1f0000, 0xdf0000, 0x68000};
    struct PolyPoint pB = {143, 76, 0x1f0000, 0xc00000, 0x68000};
    struct PolyPoint pC = {237, 77, 0x0, 0xdf0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {588, 231, 0x0, 0x1f0000, 0x119c00};
    struct PolyPoint pB = {631, 209, 0x0, 0x0, 0xe0400};
    struct PolyPoint pC = {637, 255, 0x1f0000, 0x1f0000, 0x119c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {637, 255, 0x1f0000, 0x1f0000, 0x119c00};
    struct PolyPoint pB = {631, 209, 0x0, 0x0, 0xe0400};
    struct PolyPoint pC = {681, 231, 0x1f0000, 0x0, 0xfd000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {404, 231, 0x0, 0x1f0000, 0x12f000};
    struct PolyPoint pB = {449, 209, 0x0, 0x0, 0xc7800};
    struct PolyPoint pC = {451, 255, 0x1f0000, 0x1f0000, 0x134c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {451, 255, 0x1f0000, 0x1f0000, 0x134c00};
    struct PolyPoint pB = {449, 209, 0x0, 0x0, 0xc7800};
    struct PolyPoint pC = {496, 231, 0x1f0000, 0x0, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 231, 0x0, 0x1f0000, 0x134c00};
    struct PolyPoint pB = {357, 209, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {358, 255, 0x1f0000, 0x1f0000, 0x1b3400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {358, 255, 0x1f0000, 0x1f0000, 0x1b3400};
    struct PolyPoint pB = {357, 209, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {404, 231, 0x1f0000, 0x0, 0x12f000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {219, 231, 0x0, 0x1f0000, 0xfb400};
    struct PolyPoint pB = {266, 209, 0x0, 0x0, 0x13c400};
    struct PolyPoint pC = {265, 255, 0x1f0000, 0x1f0000, 0x16e400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {265, 255, 0x1f0000, 0x1f0000, 0x16e400};
    struct PolyPoint pB = {266, 209, 0x0, 0x0, 0x13c400};
    struct PolyPoint pC = {311, 231, 0x1f0000, 0x0, 0x134c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {143, 76, 0x200000, 0xc00000, 0x3c000};
    struct PolyPoint pB = {190, 99, 0x200000, 0xdf0000, 0x3c000};
    struct PolyPoint pC = {120, 115, 0x3f0000, 0xc00000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 138, 0x3f0000, 0xdf0000, 0x3c000};
    struct PolyPoint pB = {120, 115, 0x3f0000, 0xc00000, 0x3c000};
    struct PolyPoint pC = {190, 99, 0x200000, 0xdf0000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {237, 77, 0x0, 0xc00000, 0x68000};
    struct PolyPoint pB = {285, 100, 0x0, 0xdf0000, 0x7e000};
    struct PolyPoint pC = {190, 99, 0x1f0000, 0xc00000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {238, 123, 0x1f0000, 0xdf0000, 0x68000};
    struct PolyPoint pB = {190, 99, 0x1f0000, 0xc00000, 0x68000};
    struct PolyPoint pC = {285, 100, 0x0, 0xdf0000, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {637, 255, 0x0, 0x1f0000, 0x119c00};
    struct PolyPoint pB = {681, 231, 0x0, 0x0, 0xfd000};
    struct PolyPoint pC = {687, 278, 0x1f0000, 0x1f0000, 0x119c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {687, 278, 0x1f0000, 0x1f0000, 0x119c00};
    struct PolyPoint pB = {681, 231, 0x0, 0x0, 0xfd000};
    struct PolyPoint pC = {731, 255, 0x1f0000, 0x0, 0xe0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {358, 255, 0x0, 0x1f0000, 0x1b3400};
    struct PolyPoint pB = {404, 231, 0x0, 0x0, 0x12f000};
    struct PolyPoint pC = {405, 278, 0x1f0000, 0x1f0000, 0x1b9000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {405, 278, 0x1f0000, 0x1f0000, 0x1b9000};
    struct PolyPoint pB = {404, 231, 0x0, 0x0, 0x12f000};
    struct PolyPoint pC = {451, 255, 0x1f0000, 0x0, 0x134c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {265, 255, 0x0, 0x1f0000, 0x16e400};
    struct PolyPoint pB = {311, 231, 0x0, 0x0, 0x134c00};
    struct PolyPoint pC = {311, 278, 0x1f0000, 0x1f0000, 0x22c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 278, 0x1f0000, 0x1f0000, 0x22c000};
    struct PolyPoint pB = {311, 231, 0x0, 0x0, 0x134c00};
    struct PolyPoint pC = {358, 255, 0x1f0000, 0x0, 0x1b3400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {190, 99, 0x200000, 0xc00000, 0x3c000};
    struct PolyPoint pB = {238, 123, 0x200000, 0xdf0000, 0x3c000};
    struct PolyPoint pC = {167, 138, 0x3f0000, 0xc00000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {215, 162, 0x3f0000, 0xdf0000, 0x3c000};
    struct PolyPoint pB = {167, 138, 0x3f0000, 0xc00000, 0x3c000};
    struct PolyPoint pC = {238, 123, 0x200000, 0xdf0000, 0x3c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {285, 100, 0x0, 0xc00000, 0x83800};
    struct PolyPoint pB = {336, 125, 0x0, 0xdf0000, 0x112c00};
    struct PolyPoint pC = {238, 123, 0x1f0000, 0xc00000, 0x7b400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {289, 147, 0x1f0000, 0xdf0000, 0xe6c00};
    struct PolyPoint pB = {238, 123, 0x1f0000, 0xc00000, 0x7b400};
    struct PolyPoint pC = {336, 125, 0x0, 0xdf0000, 0x112c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {87, 220, 0xc00000, 0x7b0000, 0x20000};
    struct PolyPoint pB = {85, 156, 0xc00000, 0x5c0000, 0x20000};
    struct PolyPoint pC = {105, 229, 0xdf0000, 0x7b0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {103, 165, 0xdf0000, 0x5c0000, 0x20000};
    struct PolyPoint pB = {105, 229, 0xdf0000, 0x7b0000, 0x20000};
    struct PolyPoint pC = {85, 156, 0xc00000, 0x5c0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {105, 229, 0xc00000, 0x7b0000, 0x37000};
    struct PolyPoint pB = {103, 165, 0xc00000, 0x5c0000, 0x37000};
    struct PolyPoint pC = {126, 220, 0xdf0000, 0x7b0000, 0x37000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {124, 156, 0xdf0000, 0x5c0000, 0x37000};
    struct PolyPoint pB = {126, 220, 0xdf0000, 0x7b0000, 0x37000};
    struct PolyPoint pC = {103, 165, 0xc00000, 0x5c0000, 0x37000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 278, 0x0, 0x1f0000, 0x22c000};
    struct PolyPoint pB = {358, 255, 0x0, 0x0, 0x1b3400};
    struct PolyPoint pC = {359, 302, 0x1f0000, 0x1f0000, 0x22c000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {359, 302, 0x1f0000, 0x1f0000, 0x22c000};
    struct PolyPoint pB = {358, 255, 0x0, 0x0, 0x1b3400};
    struct PolyPoint pC = {405, 278, 0x1f0000, 0x0, 0x1b9000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {74, 76, 0xcc0000, 0x400000, 0x38000};
    struct PolyPoint pB = {76, 143, 0xcc0000, 0x580000, 0x20000};
    struct PolyPoint pC = {103, 63, 0xc10000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {74, 76, 0xcc0000, 0x400000, 0x38000};
    struct PolyPoint pB = {103, 63, 0xc10000, 0x400000, 0x38000};
    struct PolyPoint pC = {76, 143, 0xcc0000, 0x580000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {104, 131, 0xc10000, 0x580000, 0x20000};
    struct PolyPoint pB = {103, 63, 0xc10000, 0x400000, 0x38000};
    struct PolyPoint pC = {76, 143, 0xcc0000, 0x580000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {104, 131, 0xc10000, 0x580000, 0x20000};
    struct PolyPoint pB = {76, 143, 0xcc0000, 0x580000, 0x20000};
    struct PolyPoint pC = {103, 63, 0xc10000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=27;
    struct PolyPoint pA = {176, 166, 0x200000, 0xff0000, 0x200000};
    struct PolyPoint pB = {147, 180, 0x200000, 0xe00000, 0x200000};
    struct PolyPoint pC = {104, 131, 0x3f0000, 0xff0000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=27;
    struct PolyPoint pA = {76, 143, 0x3f0000, 0xe00000, 0x200000};
    struct PolyPoint pB = {104, 131, 0x3f0000, 0xff0000, 0x200000};
    struct PolyPoint pC = {147, 180, 0x200000, 0xe00000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {238, 123, 0x200000, 0xc00000, 0x3c000};
    struct PolyPoint pB = {289, 147, 0x200000, 0xdf0000, 0xb6c00};
    struct PolyPoint pC = {215, 162, 0x3f0000, 0xc00000, 0x4d400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {266, 188, 0x3f0000, 0xdf0000, 0xd3800};
    struct PolyPoint pB = {215, 162, 0x3f0000, 0xc00000, 0x4d400};
    struct PolyPoint pC = {289, 147, 0x200000, 0xdf0000, 0xb6c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {336, 125, 0x0, 0xc00000, 0x11dc00};
    struct PolyPoint pB = {389, 150, 0x0, 0xdf0000, 0x11dc00};
    struct PolyPoint pC = {289, 147, 0x1f0000, 0xc00000, 0xec400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {342, 173, 0x1f0000, 0xdf0000, 0xec400};
    struct PolyPoint pB = {289, 147, 0x1f0000, 0xc00000, 0xec400};
    struct PolyPoint pC = {389, 150, 0x0, 0xdf0000, 0x11dc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {199, 87, 0x200000, 0x9f0000, 0xa1400};
    struct PolyPoint pB = {174, 83, 0x200000, 0x800000, 0x88400};
    struct PolyPoint pC = {127, 53, 0x5f0000, 0x9f0000, 0x71400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {102, 47, 0x5f0000, 0x800000, 0x71400};
    struct PolyPoint pB = {127, 53, 0x5f0000, 0x9f0000, 0x71400};
    struct PolyPoint pC = {174, 83, 0x200000, 0x800000, 0x88400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {147, 180, 0x3f0000, 0x800000, 0x49400};
    struct PolyPoint pB = {94, 204, 0x3f0000, 0x9f0000, 0x38000};
    struct PolyPoint pC = {76, 143, 0x200000, 0x800000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {23, 168, 0x200000, 0x9f0000, 0x38000};
    struct PolyPoint pB = {76, 143, 0x200000, 0x800000, 0x38000};
    struct PolyPoint pC = {94, 204, 0x3f0000, 0x9f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {76, 143, 0xe90000, 0x580000, 0x20000};
    struct PolyPoint pB = {74, 76, 0xe90000, 0x400000, 0x38000};
    struct PolyPoint pC = {23, 168, 0xff0000, 0x580000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {76, 143, 0xe90000, 0x580000, 0x20000};
    struct PolyPoint pB = {23, 168, 0xff0000, 0x580000, 0x20000};
    struct PolyPoint pC = {74, 76, 0xe90000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {21, 100, 0xff0000, 0x400000, 0x38000};
    struct PolyPoint pB = {23, 168, 0xff0000, 0x580000, 0x20000};
    struct PolyPoint pC = {74, 76, 0xe90000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {21, 100, 0xff0000, 0x400000, 0x38000};
    struct PolyPoint pB = {74, 76, 0xe90000, 0x400000, 0x38000};
    struct PolyPoint pC = {23, 168, 0xff0000, 0x580000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {174, 83, 0x400000, 0x9f0000, 0x68000};
    struct PolyPoint pB = {91, 120, 0x400000, 0x800000, 0x68000};
    struct PolyPoint pC = {102, 47, 0x5f0000, 0x9f0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {20, 85, 0x5f0000, 0x800000, 0x68000};
    struct PolyPoint pB = {102, 47, 0x5f0000, 0x9f0000, 0x68000};
    struct PolyPoint pC = {91, 120, 0x400000, 0x800000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {394, 197, 0xb70000, 0xe00000, 0x200000};
    struct PolyPoint pB = {394, 264, 0xb70000, 0xc00000, 0x200000};
    struct PolyPoint pC = {390, 264, 0xa90000, 0xc00000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {390, 197, 0xa90000, 0xe00000, 0x200000};
    struct PolyPoint pB = {394, 197, 0xb70000, 0xe00000, 0x200000};
    struct PolyPoint pC = {390, 264, 0xa90000, 0xc00000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {289, 147, 0x200000, 0xc00000, 0xd9400};
    struct PolyPoint pB = {342, 173, 0x200000, 0xdf0000, 0x191400};
    struct PolyPoint pC = {266, 188, 0x3f0000, 0xc00000, 0x101800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {318, 214, 0x3f0000, 0xdf0000, 0x3f8000};
    struct PolyPoint pB = {266, 188, 0x3f0000, 0xc00000, 0x101800};
    struct PolyPoint pC = {342, 173, 0x200000, 0xdf0000, 0x191400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {389, 150, 0x0, 0xc00000, 0x112c00};
    struct PolyPoint pB = {443, 176, 0x0, 0xdf0000, 0x97000};
    struct PolyPoint pC = {342, 173, 0x1f0000, 0xc00000, 0xe6c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {396, 199, 0x1f0000, 0xdf0000, 0x83c00};
    struct PolyPoint pB = {342, 173, 0x1f0000, 0xc00000, 0xe6c00};
    struct PolyPoint pC = {443, 176, 0x0, 0xdf0000, 0x97000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {598, 328, 0x0, 0x1f0000, 0x181400};
    struct PolyPoint pB = {643, 302, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {649, 353, 0x1f0000, 0x1f0000, 0x18cc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {649, 353, 0x1f0000, 0x1f0000, 0x18cc00};
    struct PolyPoint pB = {643, 302, 0x0, 0x0, 0xea000};
    struct PolyPoint pC = {694, 328, 0x1f0000, 0x0, 0xe4400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {215, 328, 0x0, 0x5f0000, 0x185400};
    struct PolyPoint pB = {264, 302, 0x0, 0x400000, 0x20f400};
    struct PolyPoint pC = {263, 353, 0x1f0000, 0x5f0000, 0x1f4400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {263, 353, 0x1f0000, 0x5f0000, 0x1f4400};
    struct PolyPoint pB = {264, 302, 0x0, 0x400000, 0x20f400};
    struct PolyPoint pC = {311, 328, 0x1f0000, 0x400000, 0x26d000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=27;
    struct PolyPoint pA = {147, 180, 0x200000, 0xff0000, 0x200000};
    struct PolyPoint pB = {176, 166, 0x200000, 0xe00000, 0x200000};
    struct PolyPoint pC = {220, 217, 0x3f0000, 0xff0000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=27;
    struct PolyPoint pA = {248, 203, 0x3f0000, 0xe00000, 0x200000};
    struct PolyPoint pB = {220, 217, 0x3f0000, 0xff0000, 0x200000};
    struct PolyPoint pC = {176, 166, 0x200000, 0xe00000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-73, 142, 0xe90000, 0x400000, 0x52400};
    struct PolyPoint pB = {-70, 211, 0xe90000, 0x580000, 0x3f800};
    struct PolyPoint pC = {21, 100, 0xc00000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-73, 142, 0xe90000, 0x400000, 0x52400};
    struct PolyPoint pB = {21, 100, 0xc00000, 0x400000, 0x38000};
    struct PolyPoint pC = {-70, 211, 0xe90000, 0x580000, 0x3f800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {23, 168, 0xc00000, 0x580000, 0x20000};
    struct PolyPoint pB = {21, 100, 0xc00000, 0x400000, 0x38000};
    struct PolyPoint pC = {-70, 211, 0xe90000, 0x580000, 0x3f800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {23, 168, 0xc00000, 0x580000, 0x20000};
    struct PolyPoint pB = {-70, 211, 0xe90000, 0x580000, 0x3f800};
    struct PolyPoint pC = {21, 100, 0xc00000, 0x400000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {94, 204, 0x3f0000, 0x800000, 0x38000};
    struct PolyPoint pB = {1, 249, 0x3f0000, 0x9f0000, 0x38000};
    struct PolyPoint pC = {23, 168, 0x200000, 0x800000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-70, 211, 0x200000, 0x9f0000, 0x38000};
    struct PolyPoint pB = {23, 168, 0x200000, 0x800000, 0x38000};
    struct PolyPoint pC = {1, 249, 0x3f0000, 0x9f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {229, 294, 0xa00000, 0x7b0000, 0x170400};
    struct PolyPoint pB = {228, 229, 0xa00000, 0x5c0000, 0x200000};
    struct PolyPoint pC = {249, 284, 0xbf0000, 0x7b0000, 0x1c6800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {248, 219, 0xbf0000, 0x5c0000, 0x2a6c00};
    struct PolyPoint pB = {249, 284, 0xbf0000, 0x7b0000, 0x1c6800};
    struct PolyPoint pC = {228, 229, 0xa00000, 0x5c0000, 0x200000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {210, 284, 0x800000, 0x7b0000, 0x34000};
    struct PolyPoint pB = {209, 219, 0x800000, 0x5c0000, 0x34000};
    struct PolyPoint pC = {229, 294, 0x9f0000, 0x7b0000, 0x34000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {228, 229, 0x9f0000, 0x5c0000, 0x34000};
    struct PolyPoint pB = {229, 294, 0x9f0000, 0x7b0000, 0x34000};
    struct PolyPoint pC = {209, 219, 0x800000, 0x5c0000, 0x34000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {102, 231, 0xa00000, 0x720000, 0x5e000};
    struct PolyPoint pB = {122, 240, 0xa00000, 0x7f0000, 0x63c00};
    struct PolyPoint pC = {127, 245, 0x860000, 0x720000, 0xccc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {165, 263, 0x810000, 0x7f0000, 0x106400};
    struct PolyPoint pB = {127, 245, 0x860000, 0x720000, 0xccc00};
    struct PolyPoint pC = {122, 240, 0xa00000, 0x7f0000, 0x63c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {174, 83, 0x5f0000, 0x800000, 0x88400};
    struct PolyPoint pB = {199, 87, 0x5f0000, 0x9f0000, 0xa1400};
    struct PolyPoint pC = {247, 118, 0x200000, 0x800000, 0x17a800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {273, 123, 0x200000, 0x9f0000, 0x1c7800};
    struct PolyPoint pB = {247, 118, 0x200000, 0x800000, 0x17a800};
    struct PolyPoint pC = {199, 87, 0x5f0000, 0x9f0000, 0xa1400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {94, 204, 0x400000, 0x9f0000, 0x38000};
    struct PolyPoint pB = {147, 180, 0x400000, 0x800000, 0x49400};
    struct PolyPoint pC = {167, 243, 0x5f0000, 0x9f0000, 0x93c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {220, 217, 0x5f0000, 0x800000, 0xde800};
    struct PolyPoint pB = {167, 243, 0x5f0000, 0x9f0000, 0x93c00};
    struct PolyPoint pC = {147, 180, 0x400000, 0x800000, 0x49400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {342, 173, 0x200000, 0xc00000, 0x1cac00};
    struct PolyPoint pB = {396, 199, 0x200000, 0xdf0000, 0x151c00};
    struct PolyPoint pC = {318, 214, 0x3f0000, 0xc00000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {373, 240, 0x3f0000, 0xdf0000, 0x30c800};
    struct PolyPoint pB = {318, 214, 0x3f0000, 0xc00000, 0x3f8000};
    struct PolyPoint pC = {396, 199, 0x200000, 0xdf0000, 0x151c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {47, 354, 0x800000, 0x800000, 0x95c00};
    struct PolyPoint pB = {46, 322, 0x800000, 0x5c0000, 0xa1400};
    struct PolyPoint pC = {165, 296, 0xe50000, 0x800000, 0xcf400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {165, 263, 0xe50000, 0x5c0000, 0xe0800};
    struct PolyPoint pB = {165, 296, 0xe50000, 0x800000, 0xcf400};
    struct PolyPoint pC = {46, 322, 0x800000, 0x5c0000, 0xa1400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {553, 353, 0x0, 0x1f0000, 0x1e3000};
    struct PolyPoint pB = {598, 328, 0x0, 0x0, 0x181400};
    struct PolyPoint pC = {604, 378, 0x1f0000, 0x1f0000, 0x211000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {604, 378, 0x1f0000, 0x1f0000, 0x211000};
    struct PolyPoint pB = {598, 328, 0x0, 0x0, 0x181400};
    struct PolyPoint pC = {649, 353, 0x1f0000, 0x0, 0x18cc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {263, 353, 0x0, 0x5f0000, 0x1f4400};
    struct PolyPoint pB = {311, 328, 0x0, 0x400000, 0x26d000};
    struct PolyPoint pC = {311, 378, 0x1f0000, 0x5f0000, 0x1e3000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 378, 0x1f0000, 0x5f0000, 0x1e3000};
    struct PolyPoint pB = {311, 328, 0x0, 0x400000, 0x26d000};
    struct PolyPoint pC = {359, 353, 0x1f0000, 0x400000, 0x1c4800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 353, 0x0, 0x5f0000, 0x146000};
    struct PolyPoint pB = {215, 328, 0x0, 0x400000, 0x185400};
    struct PolyPoint pC = {213, 378, 0x1f0000, 0x5f0000, 0x15d000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {213, 378, 0x1f0000, 0x5f0000, 0x15d000};
    struct PolyPoint pB = {215, 328, 0x0, 0x400000, 0x185400};
    struct PolyPoint pC = {263, 353, 0x1f0000, 0x400000, 0x1f4400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {91, 120, 0x3f0000, 0x800000, 0x68000};
    struct PolyPoint pB = {174, 83, 0x3f0000, 0x9f0000, 0x68000};
    struct PolyPoint pC = {166, 158, 0x200000, 0x800000, 0xb8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {247, 118, 0x200000, 0x9f0000, 0xf7400};
    struct PolyPoint pB = {166, 158, 0x200000, 0x800000, 0xb8000};
    struct PolyPoint pC = {174, 83, 0x3f0000, 0x9f0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {91, 120, 0x400000, 0x9f0000, 0x68000};
    struct PolyPoint pB = {-2, 164, 0x400000, 0x800000, 0x68000};
    struct PolyPoint pC = {20, 85, 0x5f0000, 0x9f0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-74, 127, 0x5f0000, 0x800000, 0x68000};
    struct PolyPoint pB = {20, 85, 0x5f0000, 0x9f0000, 0x68000};
    struct PolyPoint pC = {-2, 164, 0x400000, 0x800000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {442, 292, 0x200000, 0x9e0000, 0xf1c00};
    struct PolyPoint pB = {387, 129, 0x200000, 0x800000, 0xa3400};
    struct PolyPoint pC = {568, 226, 0x600000, 0x9e0000, 0x84800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {515, 66, 0x600000, 0x800000, 0x8c400};
    struct PolyPoint pB = {568, 226, 0x600000, 0x9e0000, 0x84800};
    struct PolyPoint pC = {387, 129, 0x200000, 0x800000, 0xa3400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {220, 233, 0x290000, 0x930000, 0x1c0c00};
    struct PolyPoint pB = {220, 217, 0x290000, 0x840000, 0x1a9c00};
    struct PolyPoint pC = {248, 219, 0x480000, 0x930000, 0x2a6c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {248, 203, 0x480000, 0x840000, 0x256400};
    struct PolyPoint pB = {248, 219, 0x480000, 0x930000, 0x2a6c00};
    struct PolyPoint pC = {220, 217, 0x290000, 0x840000, 0x1a9c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {127, 245, 0x1f0000, 0xa40000, 0x96400};
    struct PolyPoint pB = {165, 263, 0x1f0000, 0xb20000, 0xc4400};
    struct PolyPoint pC = {47, 283, 0x0, 0xa40000, 0x90800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {46, 322, 0x0, 0xb20000, 0x96400};
    struct PolyPoint pB = {47, 283, 0x0, 0xa40000, 0x90800};
    struct PolyPoint pC = {165, 263, 0x1f0000, 0xb20000, 0xc4400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-37, 308, 0x800000, 0x7f0000, 0x2fc00};
    struct PolyPoint pB = {-38, 275, 0x800000, 0x5c0000, 0x35000};
    struct PolyPoint pC = {5, 331, 0xbc0000, 0x7f0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {3, 298, 0xbc0000, 0x5c0000, 0x20000};
    struct PolyPoint pB = {5, 331, 0xbc0000, 0x7f0000, 0x20000};
    struct PolyPoint pC = {-38, 275, 0x800000, 0x5c0000, 0x35000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {220, 217, 0xc00000, 0x720000, 0x1a9c00};
    struct PolyPoint pB = {219, 148, 0xc00000, 0x590000, 0xf9800};
    struct PolyPoint pC = {248, 203, 0xd00000, 0x720000, 0x256400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {220, 217, 0xc00000, 0x720000, 0x1a9c00};
    struct PolyPoint pB = {248, 203, 0xd00000, 0x720000, 0x256400};
    struct PolyPoint pC = {219, 148, 0xc00000, 0x590000, 0xf9800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {247, 134, 0xd00000, 0x590000, 0x144400};
    struct PolyPoint pB = {248, 203, 0xd00000, 0x720000, 0x256400};
    struct PolyPoint pC = {219, 148, 0xc00000, 0x590000, 0xf9800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {247, 134, 0xd00000, 0x590000, 0x144400};
    struct PolyPoint pB = {219, 148, 0xc00000, 0x590000, 0xf9800};
    struct PolyPoint pC = {248, 203, 0xd00000, 0x720000, 0x256400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {515, 66, 0x200000, 0x9e0000, 0xfcc00};
    struct PolyPoint pB = {387, 129, 0x200000, 0x800000, 0xb8000};
    struct PolyPoint pC = {493, 56, 0x600000, 0x9e0000, 0x120800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {365, 118, 0x600000, 0x800000, 0xb8000};
    struct PolyPoint pB = {493, 56, 0x600000, 0x9e0000, 0x120800};
    struct PolyPoint pC = {387, 129, 0x200000, 0x800000, 0xb8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {1, 249, 0x400000, 0x9f0000, 0x38000};
    struct PolyPoint pB = {94, 204, 0x400000, 0x800000, 0x38000};
    struct PolyPoint pC = {75, 289, 0x5f0000, 0x9f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 243, 0x5f0000, 0x800000, 0x93c00};
    struct PolyPoint pB = {75, 289, 0x5f0000, 0x9f0000, 0x60000};
    struct PolyPoint pC = {94, 204, 0x400000, 0x800000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {23, 269, 0xa00000, 0x720000, 0x24000};
    struct PolyPoint pB = {3, 298, 0xa00000, 0x7f0000, 0x24000};
    struct PolyPoint pC = {-2, 256, 0x860000, 0x720000, 0x24000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-38, 275, 0x810000, 0x7f0000, 0x24000};
    struct PolyPoint pB = {-2, 256, 0x860000, 0x720000, 0x24000};
    struct PolyPoint pC = {3, 298, 0xa00000, 0x7f0000, 0x24000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {1, 249, 0x3f0000, 0x800000, 0x38000};
    struct PolyPoint pB = {-83, 289, 0x3f0000, 0x9f0000, 0x38000};
    struct PolyPoint pC = {-70, 211, 0x200000, 0x800000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-154, 250, 0x200000, 0x9f0000, 0x38000};
    struct PolyPoint pB = {-70, 211, 0x200000, 0x800000, 0x38000};
    struct PolyPoint pC = {-83, 289, 0x3f0000, 0x9f0000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {604, 378, 0x0, 0x1f0000, 0x211000};
    struct PolyPoint pB = {649, 353, 0x0, 0x0, 0x18cc00};
    struct PolyPoint pC = {655, 404, 0x1f0000, 0x1f0000, 0x190c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {655, 404, 0x1f0000, 0x1f0000, 0x190c00};
    struct PolyPoint pB = {649, 353, 0x0, 0x0, 0x18cc00};
    struct PolyPoint pC = {701, 378, 0x1f0000, 0x0, 0x101000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {506, 378, 0x0, 0x1f0000, 0x228000};
    struct PolyPoint pB = {553, 353, 0x0, 0x0, 0x1e3000};
    struct PolyPoint pC = {557, 404, 0x1f0000, 0x1f0000, 0x295400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {557, 404, 0x1f0000, 0x1f0000, 0x295400};
    struct PolyPoint pB = {553, 353, 0x0, 0x0, 0x1e3000};
    struct PolyPoint pC = {604, 378, 0x1f0000, 0x0, 0x211000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 378, 0x0, 0x5f0000, 0x1e3000};
    struct PolyPoint pB = {359, 353, 0x0, 0x400000, 0x1c4800};
    struct PolyPoint pC = {360, 404, 0x1f0000, 0x5f0000, 0x1d7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {360, 404, 0x1f0000, 0x5f0000, 0x1d7800};
    struct PolyPoint pB = {359, 353, 0x0, 0x400000, 0x1c4800};
    struct PolyPoint pC = {409, 378, 0x1f0000, 0x400000, 0x1a7c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {117, 378, 0x0, 0x5f0000, 0x10c800};
    struct PolyPoint pB = {167, 353, 0x0, 0x400000, 0x146000};
    struct PolyPoint pC = {164, 404, 0x1f0000, 0x5f0000, 0x157400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {164, 404, 0x1f0000, 0x5f0000, 0x157400};
    struct PolyPoint pB = {167, 353, 0x0, 0x400000, 0x146000};
    struct PolyPoint pC = {213, 378, 0x1f0000, 0x400000, 0x15d000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {220, 217, 0x470000, 0x850000, 0x1a9c00};
    struct PolyPoint pB = {220, 233, 0x470000, 0x900000, 0x1c0c00};
    struct PolyPoint pC = {167, 243, 0x280000, 0x850000, 0xdac00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 258, 0x280000, 0x900000, 0xe0800};
    struct PolyPoint pB = {167, 243, 0x280000, 0x850000, 0xdac00};
    struct PolyPoint pC = {220, 233, 0x470000, 0x900000, 0x1c0c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {247, 134, 0x200000, 0x9f0000, 0x144400};
    struct PolyPoint pB = {247, 118, 0x200000, 0x800000, 0xff400};
    struct PolyPoint pC = {273, 123, 0x3f0000, 0x9f0000, 0x155800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {219, 148, 0xff0000, 0x590000, 0xf9800};
    struct PolyPoint pB = {220, 217, 0xff0000, 0x720000, 0x1a9c00};
    struct PolyPoint pC = {166, 174, 0xe70000, 0x590000, 0xe8400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {219, 148, 0xff0000, 0x590000, 0xf9800};
    struct PolyPoint pB = {166, 174, 0xe70000, 0x590000, 0xe8400};
    struct PolyPoint pC = {220, 217, 0xff0000, 0x720000, 0x1a9c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 243, 0xe70000, 0x720000, 0xdac00};
    struct PolyPoint pB = {166, 174, 0xe70000, 0x590000, 0xe8400};
    struct PolyPoint pC = {220, 217, 0xff0000, 0x720000, 0x1a9c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 243, 0xe70000, 0x720000, 0xdac00};
    struct PolyPoint pB = {220, 217, 0xff0000, 0x720000, 0x1a9c00};
    struct PolyPoint pC = {166, 174, 0xe70000, 0x590000, 0xe8400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {5, 331, 0xaa0000, 0x7f0000, 0x20000};
    struct PolyPoint pB = {3, 298, 0xaa0000, 0x5c0000, 0x20000};
    struct PolyPoint pC = {47, 354, 0xdf0000, 0x7f0000, 0x5c400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {46, 322, 0xdf0000, 0x5c0000, 0x5c400};
    struct PolyPoint pB = {47, 354, 0xdf0000, 0x7f0000, 0x5c400};
    struct PolyPoint pC = {3, 298, 0xaa0000, 0x5c0000, 0x20000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {365, 118, 0x420000, 0x850000, 0x110800};
    struct PolyPoint pB = {359, 250, 0x430000, 0x970000, 0x3f8000};
    struct PolyPoint pC = {360, 122, 0x300000, 0x8f0000, 0x121c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {359, 250, 0x200000, 0x9e0000, 0x3f8000};
    struct PolyPoint pB = {365, 118, 0x200000, 0x800000, 0x110800};
    struct PolyPoint pC = {442, 292, 0x600000, 0x9e0000, 0x15f000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {387, 129, 0x600000, 0x800000, 0xf9800};
    struct PolyPoint pB = {442, 292, 0x600000, 0x9e0000, 0x15f000};
    struct PolyPoint pC = {365, 118, 0x200000, 0x800000, 0x110800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {247, 118, 0x5f0000, 0x800000, 0xff400};
    struct PolyPoint pB = {247, 134, 0x5f0000, 0x850000, 0x144400};
    struct PolyPoint pC = {166, 158, 0x400000, 0x800000, 0xdcc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {166, 174, 0x400000, 0x850000, 0xe8400};
    struct PolyPoint pB = {166, 158, 0x400000, 0x800000, 0xdcc00};
    struct PolyPoint pC = {247, 134, 0x5f0000, 0x850000, 0x144400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {47, 283, 0x9b0000, 0x720000, 0x40000};
    struct PolyPoint pB = {46, 322, 0x9f0000, 0x7f0000, 0x62800};
    struct PolyPoint pC = {23, 269, 0x800000, 0x720000, 0x24000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {3, 298, 0x800000, 0x7f0000, 0x24000};
    struct PolyPoint pB = {23, 269, 0x800000, 0x720000, 0x24000};
    struct PolyPoint pC = {46, 322, 0x9f0000, 0x7f0000, 0x62800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-2, 164, 0x3f0000, 0x800000, 0x68000};
    struct PolyPoint pB = {91, 120, 0x3f0000, 0x9f0000, 0x68000};
    struct PolyPoint pC = {72, 202, 0x200000, 0x800000, 0xb8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {166, 158, 0x200000, 0x9f0000, 0xb8000};
    struct PolyPoint pB = {72, 202, 0x200000, 0x800000, 0xb8000};
    struct PolyPoint pC = {91, 120, 0x3f0000, 0x9f0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {557, 404, 0x0, 0x1f0000, 0x295400};
    struct PolyPoint pB = {604, 378, 0x0, 0x0, 0x211000};
    struct PolyPoint pC = {609, 430, 0x1f0000, 0x1f0000, 0x203c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {609, 430, 0x1f0000, 0x1f0000, 0x203c00};
    struct PolyPoint pB = {604, 378, 0x0, 0x0, 0x211000};
    struct PolyPoint pC = {655, 404, 0x1f0000, 0x0, 0x190c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {360, 404, 0x0, 0x5f0000, 0x1d7800};
    struct PolyPoint pB = {409, 378, 0x0, 0x400000, 0x1a7c00};
    struct PolyPoint pC = {410, 430, 0x1f0000, 0x5f0000, 0x1e8c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {410, 430, 0x1f0000, 0x5f0000, 0x1e8c00};
    struct PolyPoint pB = {409, 378, 0x0, 0x400000, 0x1a7c00};
    struct PolyPoint pC = {459, 404, 0x1f0000, 0x400000, 0x233800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {66, 404, 0x0, 0x5f0000, 0x112400};
    struct PolyPoint pB = {117, 378, 0x0, 0x400000, 0x10c800};
    struct PolyPoint pC = {113, 430, 0x1f0000, 0x5f0000, 0x162c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {113, 430, 0x1f0000, 0x5f0000, 0x162c00};
    struct PolyPoint pB = {117, 378, 0x0, 0x400000, 0x10c800};
    struct PolyPoint pC = {164, 404, 0x1f0000, 0x400000, 0x157400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {167, 243, 0x470000, 0x850000, 0xdac00};
    struct PolyPoint pB = {167, 258, 0x470000, 0x900000, 0xe0800};
    struct PolyPoint pC = {75, 289, 0x280000, 0x850000, 0xb2800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 304, 0x280000, 0x900000, 0xb8400};
    struct PolyPoint pB = {75, 289, 0x280000, 0x850000, 0xb2800};
    struct PolyPoint pC = {167, 258, 0x470000, 0x900000, 0xe0800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {166, 174, 0xeb0000, 0x400000, 0xe8400};
    struct PolyPoint pB = {167, 243, 0xeb0000, 0x580000, 0xdac00};
    struct PolyPoint pC = {72, 218, 0xc00000, 0x400000, 0xd1400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {166, 174, 0xeb0000, 0x400000, 0xe8400};
    struct PolyPoint pB = {72, 218, 0xc00000, 0x400000, 0xd1400};
    struct PolyPoint pC = {167, 243, 0xeb0000, 0x580000, 0xdac00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 289, 0xc00000, 0x580000, 0xb2800};
    struct PolyPoint pB = {72, 218, 0xc00000, 0x400000, 0xd1400};
    struct PolyPoint pC = {167, 243, 0xeb0000, 0x580000, 0xdac00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 289, 0xc00000, 0x580000, 0xb2800};
    struct PolyPoint pB = {167, 243, 0xeb0000, 0x580000, 0xdac00};
    struct PolyPoint pC = {72, 218, 0xc00000, 0x400000, 0xd1400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 289, 0x5f0000, 0x800000, 0x60000};
    struct PolyPoint pB = {-10, 329, 0x5f0000, 0x9f0000, 0x60000};
    struct PolyPoint pC = {1, 249, 0x400000, 0x800000, 0x38000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-83, 289, 0x400000, 0x9f0000, 0x38000};
    struct PolyPoint pB = {1, 249, 0x400000, 0x800000, 0x38000};
    struct PolyPoint pC = {-10, 329, 0x5f0000, 0x9f0000, 0x60000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {410, 430, 0x0, 0x5f0000, 0x1e8c00};
    struct PolyPoint pB = {459, 404, 0x0, 0x400000, 0x233800};
    struct PolyPoint pC = {461, 457, 0x1f0000, 0x5f0000, 0x1f4400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {461, 457, 0x1f0000, 0x5f0000, 0x1f4400};
    struct PolyPoint pB = {459, 404, 0x0, 0x400000, 0x233800};
    struct PolyPoint pC = {509, 430, 0x1f0000, 0x400000, 0x27e400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 430, 0x0, 0x5f0000, 0x168800};
    struct PolyPoint pB = {360, 404, 0x0, 0x400000, 0x1d7800};
    struct PolyPoint pC = {361, 457, 0x1f0000, 0x5f0000, 0x1b5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {361, 457, 0x1f0000, 0x5f0000, 0x1b5000};
    struct PolyPoint pB = {360, 404, 0x0, 0x400000, 0x1d7800};
    struct PolyPoint pC = {410, 430, 0x1f0000, 0x400000, 0x1e8c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {14, 430, 0x0, 0x5f0000, 0x118000};
    struct PolyPoint pB = {66, 404, 0x0, 0x400000, 0x112400};
    struct PolyPoint pC = {61, 457, 0x1f0000, 0x5f0000, 0x185400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {61, 457, 0x1f0000, 0x5f0000, 0x185400};
    struct PolyPoint pB = {66, 404, 0x0, 0x400000, 0x112400};
    struct PolyPoint pC = {113, 430, 0x1f0000, 0x400000, 0x162c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-85, 430, 0x0, 0x5f0000, 0xb6400};
    struct PolyPoint pB = {-32, 404, 0x0, 0x400000, 0xd8c00};
    struct PolyPoint pC = {-39, 457, 0x1f0000, 0x5f0000, 0xea000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-39, 457, 0x1f0000, 0x5f0000, 0xea000};
    struct PolyPoint pB = {-32, 404, 0x0, 0x400000, 0xd8c00};
    struct PolyPoint pC = {14, 430, 0x1f0000, 0x400000, 0x118000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {166, 158, 0x3f0000, 0x800000, 0xdcc00};
    struct PolyPoint pB = {166, 174, 0x3f0000, 0x850000, 0xe8400};
    struct PolyPoint pC = {72, 202, 0x200000, 0x800000, 0xcb800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {72, 218, 0x200000, 0x850000, 0xd1400};
    struct PolyPoint pB = {72, 202, 0x200000, 0x800000, 0xcb800};
    struct PolyPoint pC = {166, 174, 0x3f0000, 0x850000, 0xe8400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {165, 379, 0x400000, 0x800000, 0x8a000};
    struct PolyPoint pB = {237, 383, 0x5f0000, 0x800000, 0xb8000};
    struct PolyPoint pC = {233, 416, 0x400000, 0x9f0000, 0x7e800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {304, 419, 0x5f0000, 0x9f0000, 0x95800};
    struct PolyPoint pB = {233, 416, 0x400000, 0x9f0000, 0x7e800};
    struct PolyPoint pC = {237, 383, 0x5f0000, 0x800000, 0xb8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {236, 353, 0x550000, 0x800000, 0x155800};
    struct PolyPoint pB = {237, 383, 0x5d0000, 0x880000, 0x114400};
    struct PolyPoint pC = {164, 350, 0x200000, 0x800000, 0x19a800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {236, 353, 0x550000, 0x800000, 0x155800};
    struct PolyPoint pB = {164, 350, 0x200000, 0x800000, 0x19a800};
    struct PolyPoint pC = {237, 383, 0x5d0000, 0x880000, 0x114400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {165, 379, 0x280000, 0x880000, 0x16a800};
    struct PolyPoint pB = {164, 350, 0x200000, 0x800000, 0x19a800};
    struct PolyPoint pC = {237, 383, 0x5d0000, 0x880000, 0x114400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {165, 379, 0x280000, 0x880000, 0x16a800};
    struct PolyPoint pB = {237, 383, 0x5d0000, 0x880000, 0x114400};
    struct PolyPoint pC = {164, 350, 0x200000, 0x800000, 0x19a800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-88, 203, 0x3f0000, 0x800000, 0x68000};
    struct PolyPoint pB = {-2, 164, 0x3f0000, 0x9f0000, 0x68000};
    struct PolyPoint pC = {-13, 243, 0x200000, 0x800000, 0xb8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {72, 202, 0x200000, 0x9f0000, 0xb8000};
    struct PolyPoint pB = {-13, 243, 0x200000, 0x800000, 0xb8000};
    struct PolyPoint pC = {-2, 164, 0x3f0000, 0x9f0000, 0x68000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {361, 457, 0x0, 0x5f0000, 0x1b5000};
    struct PolyPoint pB = {410, 430, 0x0, 0x400000, 0x1e8c00};
    struct PolyPoint pC = {412, 484, 0x1f0000, 0x5f0000, 0x1a3c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {412, 484, 0x1f0000, 0x5f0000, 0x1a3c00};
    struct PolyPoint pB = {410, 430, 0x0, 0x400000, 0x1e8c00};
    struct PolyPoint pC = {461, 457, 0x1f0000, 0x400000, 0x1f4400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {261, 457, 0x0, 0x1f0000, 0x179c00};
    struct PolyPoint pB = {311, 430, 0x0, 0x0, 0x168800};
    struct PolyPoint pC = {311, 484, 0x1f0000, 0x1f0000, 0x1af400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 484, 0x1f0000, 0x1f0000, 0x1af400};
    struct PolyPoint pB = {311, 430, 0x0, 0x0, 0x168800};
    struct PolyPoint pC = {361, 457, 0x1f0000, 0x0, 0x1b5000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-39, 457, 0x0, 0x5f0000, 0xea000};
    struct PolyPoint pB = {14, 430, 0x0, 0x400000, 0x118000};
    struct PolyPoint pC = {9, 484, 0x1f0000, 0x5f0000, 0x1a3c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {9, 484, 0x1f0000, 0x5f0000, 0x1a3c00};
    struct PolyPoint pB = {14, 430, 0x0, 0x400000, 0x118000};
    struct PolyPoint pC = {61, 457, 0x1f0000, 0x400000, 0x185400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-10, 329, 0x3f0000, 0x800000, 0x95000};
    struct PolyPoint pB = {75, 289, 0x3f0000, 0x9f0000, 0x7e000};
    struct PolyPoint pC = {81, 422, 0x200000, 0x800000, 0x205000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {165, 379, 0x200000, 0x9f0000, 0xce800};
    struct PolyPoint pB = {81, 422, 0x200000, 0x800000, 0x205000};
    struct PolyPoint pC = {75, 289, 0x3f0000, 0x9f0000, 0x7e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {164, 350, 0x550000, 0x800000, 0xf3c00};
    struct PolyPoint pB = {165, 379, 0x5d0000, 0x880000, 0xc3c00};
    struct PolyPoint pC = {74, 259, 0x200000, 0x800000, 0x67c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {164, 350, 0x550000, 0x800000, 0xf3c00};
    struct PolyPoint pB = {74, 259, 0x200000, 0x800000, 0x67c00};
    struct PolyPoint pC = {165, 379, 0x5d0000, 0x880000, 0xc3c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 289, 0x280000, 0x880000, 0x6d800};
    struct PolyPoint pB = {74, 259, 0x200000, 0x800000, 0x67c00};
    struct PolyPoint pC = {165, 379, 0x5d0000, 0x880000, 0xc3c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {75, 289, 0x280000, 0x880000, 0x6d800};
    struct PolyPoint pB = {165, 379, 0x5d0000, 0x880000, 0xc3c00};
    struct PolyPoint pC = {74, 259, 0x200000, 0x800000, 0x67c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-7, 413, 0x800000, 0x7b0000, 0xa7000};
    struct PolyPoint pB = {-9, 345, 0x800000, 0x5c0000, 0xb2800};
    struct PolyPoint pC = {15, 402, 0x9f0000, 0x7b0000, 0xa1400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {12, 335, 0x9f0000, 0x5c0000, 0xacc00};
    struct PolyPoint pB = {15, 402, 0x9f0000, 0x7b0000, 0xa1400};
    struct PolyPoint pC = {-9, 345, 0x800000, 0x5c0000, 0xb2800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {280, 432, 0x4f0000, 0x8d0000, 0x131000};
    struct PolyPoint pB = {233, 432, 0x260000, 0x8d0000, 0x1b5400};
    struct PolyPoint pC = {304, 419, 0x560000, 0x860000, 0x12b400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 416, 0x2d0000, 0x860000, 0x1c6800};
    struct PolyPoint pB = {304, 419, 0x560000, 0x860000, 0x12b400};
    struct PolyPoint pC = {233, 432, 0x260000, 0x8d0000, 0x1b5400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {304, 390, 0x550000, 0x800000, 0x161000};
    struct PolyPoint pB = {304, 419, 0x5d0000, 0x880000, 0x12b400};
    struct PolyPoint pC = {233, 386, 0x200000, 0x800000, 0x1f6800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {304, 390, 0x550000, 0x800000, 0x161000};
    struct PolyPoint pB = {233, 386, 0x200000, 0x800000, 0x1f6800};
    struct PolyPoint pC = {304, 419, 0x5d0000, 0x880000, 0x12b400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 416, 0x280000, 0x880000, 0x1c6800};
    struct PolyPoint pB = {233, 386, 0x200000, 0x800000, 0x1f6800};
    struct PolyPoint pC = {304, 419, 0x5d0000, 0x880000, 0x12b400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 416, 0x280000, 0x880000, 0x1c6800};
    struct PolyPoint pB = {304, 419, 0x5d0000, 0x880000, 0x12b400};
    struct PolyPoint pC = {233, 386, 0x200000, 0x800000, 0x1f6800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {614, 484, 0x400000, 0x1f0000, 0x1ae800};
    struct PolyPoint pB = {661, 457, 0x400000, 0x0, 0x1bfc00};
    struct PolyPoint pC = {666, 536, 0x5f0000, 0x1f0000, 0x181c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {666, 536, 0x5f0000, 0x1f0000, 0x181c00};
    struct PolyPoint pB = {661, 457, 0x400000, 0x0, 0x1bfc00};
    struct PolyPoint pC = {714, 507, 0x5f0000, 0x0, 0x187800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {311, 484, 0x0, 0x5f0000, 0x1af400};
    struct PolyPoint pB = {361, 457, 0x0, 0x400000, 0x1b5000};
    struct PolyPoint pC = {362, 513, 0x1f0000, 0x5f0000, 0x18cc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {362, 513, 0x1f0000, 0x5f0000, 0x18cc00};
    struct PolyPoint pB = {361, 457, 0x0, 0x400000, 0x1b5000};
    struct PolyPoint pC = {412, 484, 0x1f0000, 0x400000, 0x1a3c00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {210, 484, 0x0, 0x1f0000, 0x1d0000};
    struct PolyPoint pB = {261, 457, 0x0, 0x0, 0x179c00};
    struct PolyPoint pC = {260, 513, 0x1f0000, 0x1f0000, 0x1d7800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {260, 513, 0x1f0000, 0x1f0000, 0x1d7800};
    struct PolyPoint pB = {261, 457, 0x0, 0x0, 0x179c00};
    struct PolyPoint pC = {311, 484, 0x1f0000, 0x0, 0x1af400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {100, 233, 0x5f0000, 0x9f0000, 0xd1800};
    struct PolyPoint pB = {15, 273, 0x200000, 0x9f0000, 0xe8800};
    struct PolyPoint pC = {72, 202, 0x5f0000, 0x800000, 0xc0400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-13, 243, 0x200000, 0x800000, 0xcbc00};
    struct PolyPoint pB = {72, 202, 0x5f0000, 0x800000, 0xc0400};
    struct PolyPoint pC = {15, 273, 0x200000, 0x9f0000, 0xe8800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {81, 422, 0x3f0000, 0x800000, 0x106c00};
    struct PolyPoint pB = {165, 379, 0x3f0000, 0x9f0000, 0x8e000};
    struct PolyPoint pC = {149, 460, 0x200000, 0x800000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 416, 0x200000, 0x9f0000, 0x9f400};
    struct PolyPoint pB = {149, 460, 0x200000, 0x800000, 0x3f8000};
    struct PolyPoint pC = {165, 379, 0x3f0000, 0x9f0000, 0x8e000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {81, 422, 0x470000, 0x850000, 0x228400};
    struct PolyPoint pB = {81, 438, 0x470000, 0x900000, 0x1eec00};
    struct PolyPoint pC = {-10, 329, 0x280000, 0x850000, 0x7ec00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-9, 345, 0x280000, 0x900000, 0x84800};
    struct PolyPoint pB = {-10, 329, 0x280000, 0x850000, 0x7ec00};
    struct PolyPoint pC = {81, 438, 0x470000, 0x900000, 0x1eec00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {80, 392, 0x550000, 0x800000, 0x24cc00};
    struct PolyPoint pB = {81, 422, 0x5d0000, 0x880000, 0x228400};
    struct PolyPoint pC = {-11, 300, 0x200000, 0x800000, 0x79000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {80, 392, 0x550000, 0x800000, 0x24cc00};
    struct PolyPoint pB = {-11, 300, 0x200000, 0x800000, 0x79000};
    struct PolyPoint pC = {81, 422, 0x5d0000, 0x880000, 0x228400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-10, 329, 0x280000, 0x880000, 0x7ec00};
    struct PolyPoint pB = {-11, 300, 0x200000, 0x800000, 0x79000};
    struct PolyPoint pC = {81, 422, 0x5d0000, 0x880000, 0x228400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {-10, 329, 0x280000, 0x880000, 0x7ec00};
    struct PolyPoint pB = {81, 422, 0x5d0000, 0x880000, 0x228400};
    struct PolyPoint pC = {-11, 300, 0x200000, 0x800000, 0x79000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 416, 0x470000, 0x850000, 0x1c6800};
    struct PolyPoint pB = {233, 432, 0x470000, 0x900000, 0x1b5400};
    struct PolyPoint pC = {149, 460, 0x280000, 0x850000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {149, 476, 0x280000, 0x900000, 0x3f8000};
    struct PolyPoint pB = {149, 460, 0x280000, 0x850000, 0x3f8000};
    struct PolyPoint pC = {233, 432, 0x470000, 0x900000, 0x1b5400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {15, 273, 0x3f0000, 0x9f0000, 0xba400};
    struct PolyPoint pB = {-13, 259, 0x200000, 0x9f0000, 0xa3400};
    struct PolyPoint pC = {-13, 243, 0x200000, 0x800000, 0x9d800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 386, 0x3f0000, 0x800000, 0x1f6800};
    struct PolyPoint pB = {233, 416, 0x3f0000, 0x8d0000, 0x1c6800};
    struct PolyPoint pC = {148, 430, 0x200000, 0x800000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {233, 386, 0x3f0000, 0x800000, 0x1f6800};
    struct PolyPoint pB = {148, 430, 0x200000, 0x800000, 0x3f8000};
    struct PolyPoint pC = {233, 416, 0x3f0000, 0x8d0000, 0x1c6800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {149, 460, 0x200000, 0x8d0000, 0x3f8000};
    struct PolyPoint pB = {148, 430, 0x200000, 0x800000, 0x3f8000};
    struct PolyPoint pC = {233, 416, 0x3f0000, 0x8d0000, 0x1c6800};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {149, 460, 0x200000, 0x8d0000, 0x3f8000};
    struct PolyPoint pB = {233, 416, 0x3f0000, 0x8d0000, 0x1c6800};
    struct PolyPoint pC = {148, 430, 0x200000, 0x800000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {149, 460, 0x470000, 0x850000, 0x3f8000};
    struct PolyPoint pB = {149, 476, 0x470000, 0x900000, 0x3f8000};
    struct PolyPoint pC = {81, 422, 0x280000, 0x850000, 0x228400};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {81, 438, 0x280000, 0x900000, 0x1eec00};
    struct PolyPoint pB = {81, 422, 0x280000, 0x850000, 0x228400};
    struct PolyPoint pC = {149, 476, 0x470000, 0x900000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {148, 430, 0x3f0000, 0x800000, 0x3f8000};
    struct PolyPoint pB = {149, 460, 0x3f0000, 0x8d0000, 0x3f8000};
    struct PolyPoint pC = {80, 392, 0x200000, 0x800000, 0x24cc00};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {148, 430, 0x3f0000, 0x800000, 0x3f8000};
    struct PolyPoint pB = {80, 392, 0x200000, 0x800000, 0x24cc00};
    struct PolyPoint pC = {149, 460, 0x3f0000, 0x8d0000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {81, 422, 0x200000, 0x8d0000, 0x228400};
    struct PolyPoint pB = {80, 392, 0x200000, 0x800000, 0x24cc00};
    struct PolyPoint pC = {149, 460, 0x3f0000, 0x8d0000, 0x3f8000};
    draw_gpoly(&pA, &pB, &pC); }
    {vec_mode=5;
    struct PolyPoint pA = {81, 422, 0x200000, 0x8d0000, 0x228400};
    struct PolyPoint pB = {149, 460, 0x3f0000, 0x8d0000, 0x3f8000};
    struct PolyPoint pC = {80, 392, 0x200000, 0x800000, 0x24cc00};
    draw_gpoly(&pA, &pB, &pC); }
}

/******************************************************************************/
