/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehtraffic.c
 *     Pathfinding triangulation support required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling pathfinding triangulation.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "vehtraffic.h"

#include <stdlib.h>
#include "bfutility.h"
#include "bigmap.h"
#include "thing.h"
#include "vehicle.h"
#include "swlog.h"
/******************************************************************************/

short tnode_find_used_link(struct TrafficNode *p_tnode)
{
    short i;
    for (i = 0; i < 4; i++)
    {
        if (p_tnode->UTraffic.Link[i] != 0)
            return i;
    }
    return -1;
}

short get_next_tnode(struct Thing *p_vehicle, struct TrafficNode *p_tnode)
{
    short ret;
    asm volatile (
      "call ASM_get_next_tnode\n"
        : "=r" (ret) : "a" (p_vehicle), "d" (p_tnode));
    return ret;
}

void process_next_vnav_turn(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_next_vnav_turn\n"
        : : "a" (p_vehicle));
    return;
}

void remove_locks(struct Thing *p_thing)
{
    asm volatile ("call ASM_remove_locks\n"
        : : "a" (p_thing));
    return;
}

void process_next_tnode(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_next_tnode\n"
        : : "a" (p_vehicle));
    return;
}


/******************************************************************************/
