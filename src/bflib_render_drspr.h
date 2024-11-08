/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bflib_render_drspr.h
 *     Header file for bflib_render_drspr.c.
 * @par Purpose:
 *     Special variants os sprite drawing functs.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 12 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_RENDER_DRSPR_H
#define BFLIB_RENDER_DRSPR_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

struct TbSprite;

#pragma pack()
/******************************************************************************/

void LbSpriteDraw_1(int x, int y, struct TbSprite *spr);
void LbSpriteDraw_2(int x, int y, struct TbSprite *spr);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
