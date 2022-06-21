/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render.h
 *     Header file for bflib_render.c and bflib_render_*.c.
 * @par Purpose:
 *     Rendering the 3D view functions.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 30 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_REND_H
#define BFLIB_REND_H

#include "bflib_basics.h"
#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define POLY_SCANS_COUNT 576

enum VecModes {
    VM_Unknown0 = 0,
    VM_Unknown1,
    VM_Unknown2,
    VM_Unknown3,
    VM_Unknown4,
    VM_Unknown5,
    VM_Unknown6,
    VM_Unknown7,
    VM_Unknown8,
    VM_Unknown9,
    VM_Unknown10,
    VM_Unknown11,
    VM_Unknown12,
    VM_Unknown13,
    VM_Unknown14,
    VM_Unknown15,
    VM_Unknown16,
    VM_Unknown17,
    VM_Unknown18,
    VM_Unknown19,
    VM_Unknown20,
    VM_Unknown21,
    VM_Unknown22,
    VM_Unknown23,
    VM_Unknown24,
    VM_Unknown25,
    VM_Unknown26,
    VM_Unknown27,
};


struct GtBlock { // sizeof = 48
  unsigned char *field_0;
  unsigned long field_4;
  unsigned long field_8;
  unsigned long field_C;
  unsigned long field_10;
  unsigned long field_14;
  unsigned long field_18;
  unsigned long field_1C;
  unsigned long field_20;
  unsigned long field_24;
  unsigned long field_28;
  unsigned long field_2C;
};

#pragma pack()
/******************************************************************************/
extern ubyte *render_fade_tables;
extern TbPixel *render_ghost;
// Rename pending for these entries
extern ubyte *LOC_poly_screen;

extern long LOC_vec_window_width;
extern long LOC_vec_window_height;

extern long LOC_vec_screen_width;
extern ubyte *LOC_vec_screen;

extern ubyte *LOC_vec_map;
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
