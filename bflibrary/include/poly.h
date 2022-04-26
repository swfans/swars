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
extern int poly_screen;
int mgt_jt();
int add_to_edi();
extern int vec_window_height;
extern int vec_screen_width;
int setup_vecs();
extern int vec_pal;
extern int dither_map;
extern int dither_end;
extern int vec_window_width;
extern int vec_screen;
extern int vec_map;
int t12_jt();
int render_table();

#ifdef __cplusplus
};
#endif

#endif // POLY_H_
/******************************************************************************/
