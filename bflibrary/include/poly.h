/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly.h
 *     Header file for poly.asm.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef POLY_H_
#define POLY_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define POLY_SCANS_COUNT 576

#pragma pack(1)

struct PolyPoint { // sizeof=20
    long X; // offset=0
    long Y; // offset=4
    long U; // offset=8
    long V; // offset=12
    long S; // offset=16
};

#pragma pack()

extern struct PolyPoint polyscans[POLY_SCANS_COUNT];

extern TbPixel vec_colour;
extern ubyte vec_mode;

extern ubyte *poly_screen;

extern long vec_window_width;
extern long vec_window_height;

extern long vec_screen_width;
extern ubyte *vec_screen;

extern ubyte *vec_map;
extern ubyte *vec_pal;

extern ubyte *dither_map;
extern ubyte *dither_end;

void setup_vecs(ubyte *screenbuf, ubyte *nvec_map,
  ulong line_len, ulong width, ulong height);

void trig(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c);

#if 0

int render_table();
int poly_line();
#endif

#ifdef __cplusplus
};
#endif

#endif // POLY_H_
/******************************************************************************/
