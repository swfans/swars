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
    i32 X; //< offset=0, Horizonal coordinate within screen buffer
    i32 Y; //< offset=4, Vertical coordinate within screen buffer
    i32 U; //< offset=8, Texture UV mapping, U coordinate
    i32 V; //< offset=12, Texture UV mapping, V coordinate
    i32 S; //< offset=16, Shiness / brighness of the point
};

#pragma pack()

extern struct PolyPoint polyscans[POLY_SCANS_COUNT];

extern TbPixel vec_colour;
extern u8 vec_mode;

extern u8 *poly_screen;

extern i32 vec_window_width;
extern i32 vec_window_height;

extern i32 vec_screen_width;
extern u8 *vec_screen;

extern u8 *vec_map;
extern u8 *vec_pal;

extern u8 *dither_map;
extern u8 *dither_end;

void setup_vecs(u8 *screenbuf, u8 *nvec_map,
  u32 line_len, u32 width, u32 height);

void trig(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c);

void draw_gpoly(struct PolyPoint *point_a, struct PolyPoint *point_b,
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
