/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file scandraw.h
 *     Header file for scandraw.c.
 * @par Purpose:
 *     Ingame scanner (minimap/radar) drawing.
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
#ifndef SCANDRAW_H
#define SCANDRAW_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/
void unkn_draw_transformed_point(short x, short y, long ptX, long ptY, long ptZ, ubyte colour);
void draw_objective_point(long x, long y, short thing, short a4, ubyte colour);
void func_711F4(short a1, short a2, short a3, short a4, ubyte colour);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
