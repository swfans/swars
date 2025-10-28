/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file lvwalk.h
 *     Header file for lvwalk.c.
 * @par Purpose:
 *     Routines for handling walk items on a map.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 24 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef LVWALK_H
#define LVWALK_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct WalkHeader { // sizeof=4
    ushort Count;
    ushort StartItem;
};

#pragma pack()
/******************************************************************************/

/** Header linking a face to a list of walk items.
 *
 * Some SingleObjectFace* instances have a WalkHeader assigned.
 * If they do, that defines a range of walk items containing
 * neighbor faces, to which it is possible to walk.
 *
 */
extern struct WalkHeader *game_walk_headers;
extern ushort next_walk_header;

/** List of faces which can be walked to from a specific face.
 *
 * Each entry is a face index which has at least corner very close
 * to a corner of current face, allowing walking between the faces.
 */
extern short *game_walk_items;
extern ushort next_walk_item;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
