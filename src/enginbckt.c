/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginbckt.c
 *     Drawlist buckets handling for the 3D engine.
 * @par Purpose:
 *     Implements functions for filling and using the drawlist buckets.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 12 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginbckt.h"

#include "engindrwlst.h"
#include "swlog.h"
/******************************************************************************/
extern ushort buckets[BUCKETS_COUNT];

void draw_item_add(ubyte ditype, ushort offset, ushort bckt)
{
    if (bckt < 1)
        bckt = 1;
    if (bckt >= 10000)
        bckt = 9997;
    p_current_draw_item->Type = ditype; 
    p_current_draw_item->Offset = offset;
    p_current_draw_item->Child = buckets[bckt];
    p_current_draw_item++;
    buckets[bckt] = next_draw_item;
    next_draw_item++;
}

void draw_drawlist_1(void)
{
    ushort *p_bucket;
    ushort dihead;
    short n;

    p_bucket = &buckets[BUCKETS_COUNT-1];
    for (n = BUCKETS_COUNT-1; n >= 0; n--)
    {
        dihead = *p_bucket;
        *p_bucket = 0;
        draw_drawitem_1(dihead);
        p_bucket--;
    }
}

void draw_drawlist_2(void)
{
    ushort *p_bucket;
    ushort dihead;
    short n;

    p_bucket = &buckets[BUCKETS_COUNT-1];
    for (n = BUCKETS_COUNT-1; n >= 0; n--)
    {
        dihead = *p_bucket;
        *p_bucket = 0;
        scene_post_effect_for_bucket(n);
        draw_drawitem_2(dihead);
        p_bucket--;
    }
}

/******************************************************************************/
