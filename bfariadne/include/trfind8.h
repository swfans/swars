/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file trfind8.h
 *     Header file for trfind8.c.
 * @par Purpose:
 *     Routines for finding features in the triangulation data.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 09 Feb 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ARIADNE_TRFIND8_H
#define ARIADNE_TRFIND8_H

#include "bftypes.h"
#include "triangls.h"
#include "trpoints.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

int triangle_find8(TrFineCoord pt_x, TrFineCoord pt_y);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
