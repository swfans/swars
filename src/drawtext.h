/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file drawtext.h
 *     Header file for drawtext.c.
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
#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include "bftypes.h"
#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

void draw_text(short x, short y, const char *text, ubyte colour);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
