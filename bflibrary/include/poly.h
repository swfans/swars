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

extern int polyscans;
int ft_jt();
int fb_jt();
int gt_jt();
int ll_jt();
int rl_jt();
int poly_line();
extern int vec_mode;
extern int vec_colour;
int trig();
int mgt_jt();
int add_to_edi();

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

int t12_jt();
int render_table();

#ifdef __cplusplus
};
#endif

#endif // POLY_H_
/******************************************************************************/
