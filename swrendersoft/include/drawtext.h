/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file drawtext.h
 *     Header file for drawtext.c.
 * @par Purpose:
 *     Drawing simple text within the 3D world.
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
#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

void draw_text(short x, short y, const char *text, ubyte colour);

void draw_text_transformed_col(int cor_x, int cor_y_m8, int cor_z,
  const char *text, TbPixel col);
void draw_text_transformed(int cor_x, int cor_y_m8, int cor_z, const char *text);
void draw_number_transformed(int cor_x, int cor_y_m8, int cor_z, int num);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
