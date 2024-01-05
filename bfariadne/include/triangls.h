/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file triangls.h
 *     Header file for triangls.c.
 * @par Purpose:
 *     Pathfinding Triangle declaration, life cycle and usage.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ARIADNE_TRIANGLS_H
#define ARIADNE_TRIANGLS_H

#include "bftypes.h"
#include "trpoints.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)

/** Type for storing triangle index.
 */
typedef short TrTriangId;

/** Triangulation triangle struct.
 */
struct TrTriangle {
    TrPointId point[3];
    TrTriangId tri[3];
    short jump;
    ubyte solid;
    ubyte enter;
};

#pragma pack()
/******************************************************************************/

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
