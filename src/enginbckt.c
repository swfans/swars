/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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

#include "engindrwlstx.h"
#include "enginpeff.h"
#include "game_data.h"
#include "swlog.h"
/******************************************************************************/
#define DEBUG_DRAWLIST_BUCKETS_LIMITS 0

ushort buckets[BUCKETS_COUNT];

TbBool draw_item_add(ubyte ditype, ushort offset, int bckt)
{
    struct DrawItem *p_dritm;
#if DEBUG_DRAWLIST_BUCKETS_LIMITS
    static int bckt_min = INT32_MAX;
    static int bckt_max = 0;
    if ((bckt < bckt_min) || (bckt > bckt_max)) {
        if (bckt < bckt_min)
            bckt_min = bckt;
        if (bckt > bckt_max)
            bckt_max = bckt;
        LOGSYNC("Drawlist buckets range %d..%d", bckt_min, bckt_max);
    }
#endif
    if (bckt < 1)
        bckt = 1;
    if (bckt >= BUCKETS_COUNT)
        bckt = BUCKETS_COUNT-1;

    if (next_draw_item >= mem_game[31].N)
        return false;

    p_dritm = p_current_draw_item;
    p_current_draw_item++;
    p_dritm->Type = ditype;
    p_dritm->Offset = offset;
    p_dritm->Child = buckets[bckt];
    buckets[bckt] = next_draw_item;
    next_draw_item++;
    return true;
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
