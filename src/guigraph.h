/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file guigraph.h
 *     Header file for guigraph.c.
 * @par Purpose:
 *     GUI graphs drawing for menu system.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GUIGRAPH_H
#define GUIGRAPH_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

void draw_chartxy_axis_x_values(int x, int y, int width,
  int ax_min, int ax_max, int tot_values);
void draw_chartxy_axis_y_values(int x, int y, int height,
  int ax_min, int ax_max, int tot_values);

void draw_chartxy_axis_x_main(int x, int y, int width);
void draw_chartxy_axis_y_main(int x, int y, int height);

void draw_chartxy_axis_y_grid(int x, int y, int width, int height,
  int tot_values);
void draw_chartxy_axis_x_grid(int x, int y, int width, int height,
  int tot_values);

void draw_chartxy_curve(int x, int y, int width, int height,
  ushort *y_vals, int n_y_vals, int y_scale,
  ushort y_trend_delta, int tot_values);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
