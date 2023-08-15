/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file scanner.c
 *     Ingame scanner (minimap/radar) support.
 * @par Purpose:
 *     Implement functions for handling the scanner map and its state.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "scanner.h"

#include "campaign.h"
#include "thing.h"
#include "player.h"
#include "game.h"
#include "lvobjctv.h"
#include "display.h"
#include "swlog.h"
/******************************************************************************/

void SCANNER_init(void)
{
    asm volatile ("call ASM_SCANNER_init\n"
        :  :  : "eax" );
}

void SCANNER_set_colour(ubyte col)
{
    asm volatile ("call ASM_SCANNER_set_colour\n"
        :  : "a" ((long)col));
}

void SCANNER_fill_in(void)
{
    asm volatile ("call ASM_SCANNER_fill_in\n"
        :  :  : "eax" );
}

ushort do_group_scanner(struct Objective *p_objectv, ushort signal_count)
{
#if 0
    ushort ret;
    asm volatile ("call ASM_do_group_scanner\n"
        : "=r" (ret) : "a" (p_objectv), "d" (signal_count));
    return ret;
#else
    ushort n;
    ubyte col;
    short dcthing;
    short nearthing;
    int blipX, blipZ;

    n = signal_count;
    if (n >= SCANNER_BIG_BLIP_COUNT) {
        LOGWARN("Scaner blips limit reached, blip discarded.");
        return n;
    }
    dcthing = players[local_player_no].DirectControl[0];
    switch (p_objectv->Type)
    {
    case GAME_OBJ_PERSUADE_MEM_G:
    case GAME_OBJ_PERSUADE_ALL_G:
        nearthing = find_nearest_from_group(&things[dcthing], p_objectv->Thing, 1);
        col = colour_lookup[3];
        break;
    case GAME_OBJ_PROTECT_G:
        nearthing = find_nearest_from_group(&things[dcthing], p_objectv->Thing, 0);
        col = colour_lookup[1];
        break;
    default:
        nearthing = find_nearest_from_group(&things[dcthing], p_objectv->Thing, 0);
        col = colour_lookup[2];
        break;
    }
    ingame.Scanner.NearThing1 = nearthing;

    if (ingame.Scanner.NearThing1 != 0)
    {
        if (ingame.Scanner.NearThing1 <= 0) {
            struct SimpleThing *p_sthing;
            p_sthing = &sthings[ingame.Scanner.NearThing1];
            blipX = p_sthing->X;
            blipZ = p_sthing->Z;
        } else {
            struct Thing *p_thing;
            p_thing = &things[ingame.Scanner.NearThing1];
            blipX = p_thing->X;
            blipZ = p_thing->Z;
        }
        ingame.Scanner.BigBlip[n].Period = 32;
        ingame.Scanner.BigBlip[n].X = blipX;
        ingame.Scanner.BigBlip[n].Z = blipZ;
        ingame.Scanner.BigBlip[n].Speed = 4;
        ingame.Scanner.BigBlip[n].Colour = col;
        ++n;
        if (ingame.Scanner.GroupCount < SCANNER_GROUP_COUNT)
        {
            int group;

            group = ingame.Scanner.GroupCount;
            ++ingame.Scanner.GroupCount;
            ingame.Scanner.Group[group] = p_objectv->Thing;
            ingame.Scanner.GroupCol[group] = col;
        }
    }
    return n;
#endif
}

void add_signal_to_scanner(struct Objective *p_objectv, ubyte flag)
{
    asm volatile ("call ASM_add_signal_to_scanner\n"
        :  : "a" (p_objectv), "d" (flag));
}

/******************************************************************************/
