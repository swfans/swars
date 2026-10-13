/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file drawshape.h
 *     Header file for drawshape.c.
 * @par Purpose:
 *     Drawing simple 2D shapes within the 3D world.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 20 Jan 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DRAWSHAPE_H
#define DRAWSHAPE_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

/** Draws a line between two map points.
 */
void draw_mapcoord_line_col(int cor1_x, int cor1_y_m8, int cor1_z,
  int cor2_x, int cor2_y_m8, int cor2_z, TbPixel colour);

void draw_mapcoord_line_annotated(int cor1_x, int cor1_y_m8, int cor1_z,
  int cor2_x, int cor2_y_m8, int cor2_z, const char *annot_text,
  TbPixel lncol, TbPixel txcol);

void draw_mapcoord_line_annotated_num(int cor1_x, int cor1_y_m8, int cor1_z,
  int cor2_x, int cor2_y_m8, int cor2_z, int annot_num,
  TbPixel lncol, TbPixel txcol);

/** Draws a line between a map point and screen point.
 */
void draw_mapcoord_to_scrcoord_line(short scr_x, short scr_y,
  int cor_x, int cor_y_m8, int cor_z, TbPixel colour);

/** Draws a rectangle on a beveled box.
 */
void draw_bevel_rect(int scr_x, int scr_y,
  int width, int height, TbPixel colour, TbPixel bvcolour);

/** Draws a rectangle, but corners only.
 */
void draw_corner_rect(int scr_x, int scr_y,
  int width, int height, int lnlen, int lnthick, TbPixel colour);

void draw_mapcoord_flat_circle_outline(short cor_x, short cor_y_m8, short cor_z,
  short radius, TbPixel colour);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
