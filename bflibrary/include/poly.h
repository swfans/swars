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
#include "bfscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

#define POLY_SCANS_COUNT (MAX_SUPPORTED_SCREEN_HEIGHT * 6 / 5)

#pragma pack(1)

enum RenderingVectorMode {
    RendVec_mode00 = 0,
    RendVec_mode01,
    RendVec_mode02,
    RendVec_mode03,
    RendVec_mode04,
    RendVec_mode05,
    RendVec_mode06,
    RendVec_mode07,
    RendVec_mode08,
    RendVec_mode09,
    RendVec_mode10,
    RendVec_mode11,
    RendVec_mode12,
    RendVec_mode13,
    RendVec_mode14,
    RendVec_mode15,
    RendVec_mode16,
    RendVec_mode17,
    RendVec_mode18,
    RendVec_mode19,
    RendVec_mode20,
    RendVec_mode21,
    RendVec_mode22,
    RendVec_mode23,
    RendVec_mode24,
    RendVec_mode25,
    RendVec_mode26,
};

struct PolyPoint { // sizeof=20
    long X; //< offset=0, Horizonal coordinate within screen buffer
    long Y; //< offset=4, Vertical coordinate within screen buffer
    long U; //< offset=8, Texture UV mapping, U coordinate
    long V; //< offset=12, Texture UV mapping, V coordinate
    long S; //< offset=16, Shiness / brighness of the point
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

/** Triangle rendering function.
 *
 * Draw textured triangle on graphics buffer previously set by setup_vecs(),
 * at given 3 points.
 *
 * For the triangle to be visible, points need to be provided in clockwise
 * order.
 *
 * @param point_a Coordinates and texture mapping of first point.
 * @param point_b Coordinates and texture mapping of second point.
 * @param point_c Coordinates and texture mapping of third point.
 */
void trig(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c);

void draw_gpoly(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c);

void poly_line(struct PolyPoint *point_a, struct PolyPoint *point_b);

#if 0

int render_table();
#endif

#ifdef __cplusplus
};
#endif

#endif // POLY_H_
/******************************************************************************/
