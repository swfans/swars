/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engintrns.h
 *     Header file for engintrns.c.
 * @par Purpose:
 *     Math transformations required by the 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ENGINTRNS_H
#define ENGINTRNS_H

#include "bftypes.h"
#include "bfmath.h"
#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

// TODO figure out why we have PolyPoint and EnginePoint
struct EnginePoint {
  struct PolyPoint pp;
  /*long X;
  long Y;
  long TMapX;
  long TMapY;
  long Shade;*/
  long X3d;
  long Y3d;
  long Z3d;
  long DistSqr;
  short EPUnkn24;
  ubyte Flags; // TODO check if it is ushort
};

/** Short and Shaded version of Engine Point.
 */
struct ShEnginePoint {
    short X;
    short Y;
    int field_4;
    ubyte Flags;
    short field_9;
    short Shade;
};

#pragma pack()
/******************************************************************************/
extern long dword_152EEC;

extern long dword_176D0C;
extern long dword_176D10;
extern long dword_176D14;
extern long dword_176D18;
extern long dword_176D1C;
extern long dword_176D3C;
extern long dword_176D40;
extern long dword_176D44;
extern long dword_176D4C;
extern long dword_176D54;
extern long dword_176D64;

void transform_point(struct EnginePoint *p_ep);
void transform_shpoint(struct ShEnginePoint *p_sp, int dxc, int dyc, int dzc);

void draw_trigpoly(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c);

void process_engine_unk1(void);
void process_engine_unk2(void);
void setup_engine_nullsub4(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
