/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginbckt.h
 *     Header file for enginbckt.c.
 * @par Purpose:
 *     Drawlist buckets handling for the 3D engine.
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
#ifndef ENGINBCKT_H
#define ENGINBCKT_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/** Amount of buckets for draw list elements.
 */
#define BUCKETS_COUNT 10000

/** Max amount of draw list elements within a bucket.
 */
#define BUCKET_ITEMS_MAX 2000

#pragma pack(1)

#pragma pack()
/******************************************************************************/

void draw_item_add(ubyte ditype, ushort offset, ushort bckt);

void draw_drawlist_1(void);
void draw_drawlist_2(void);
void reset_drawlist(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
