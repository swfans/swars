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

extern ushort signal_count;
extern ulong turn_last; // = 999;
extern ulong SCANNER_keep_arcs;
extern ulong dword_1DB1A0;

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

void SCANNER_init_arcpoint(int x1, int z1, int x2, int z2, int c)
{
    asm volatile (
      "push %4\n"
      "call ASM_SCANNER_init_arcpoint\n"
        : : "a" (x1), "d" (z1), "b" (x2), "c" (z2), "g" (c));
}

void SCANNER_update_arcpoint(ushort arc_no, short fromX, short fromZ, short toX, short toZ)
{
    if (arc_no >= SCANNER_ARC_COUNT)
        return;
    ingame.Scanner.Arc[arc_no].X1 = fromX;
    ingame.Scanner.Arc[arc_no].Z1 = fromZ;
    ingame.Scanner.Arc[arc_no].X2 = toX;
    ingame.Scanner.Arc[arc_no].Z2 = toZ;
}

void SCANNER_init_blippoint(ushort blip_no, int x, int z, int colour)
{
    if (blip_no >= SCANNER_BIG_BLIP_COUNT)
        return;
    ingame.Scanner.BigBlip[blip_no].X = x;
    ingame.Scanner.BigBlip[blip_no].Z = z;
    ingame.Scanner.BigBlip[blip_no].Speed = 4;
    ingame.Scanner.BigBlip[blip_no].Colour = colour;
    ingame.Scanner.BigBlip[blip_no].Period = 32;
}

ushort do_group_scanner(struct Objective *p_objectv, ushort next_signal)
{
#if 0
    ushort ret;
    asm volatile ("call ASM_do_group_scanner\n"
        : "=r" (ret) : "a" (p_objectv), "d" (next_signal));
    return ret;
#else
    ushort n;
    ubyte colr;
    short dcthing;
    short nearthing;
    int blipX, blipZ;

    n = next_signal;
    dcthing = players[local_player_no].DirectControl[0];
    if (objective_target_is_ally(p_objectv)) {
        nearthing = find_nearest_from_group(&things[dcthing], p_objectv->Thing, 0);
        colr = colour_lookup[1];
    } else if (objective_target_is_to_be_acquired(p_objectv)) {
        nearthing = find_nearest_from_group(&things[dcthing], p_objectv->Thing, 1);
        colr = colour_lookup[3];
    } else {
        nearthing = find_nearest_from_group(&things[dcthing], p_objectv->Thing, 0);
        colr = colour_lookup[2];
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
        SCANNER_init_blippoint(n, blipX, blipZ, colr);
        n++;
        if (ingame.Scanner.GroupCount < SCANNER_GROUP_COUNT)
        {
            int group;

            group = ingame.Scanner.GroupCount;
            ++ingame.Scanner.GroupCount;
            ingame.Scanner.Group[group] = p_objectv->Thing;
            ingame.Scanner.GroupCol[group] = colr;
        }
    }
    return n;
#endif
}

ushort do_target_thing_scanner(struct Objective *p_objectv, ushort next_signal)
{
    long X, Z;
    ushort n;
    ubyte colr;

    if (objective_target_is_ally(p_objectv))
        colr = colour_lookup[1];
    else if (objective_target_is_to_be_acquired(p_objectv))
        colr = colour_lookup[3];
    else
        colr = colour_lookup[2];

    n = next_signal;
    if (p_objectv->Thing <= 0) {
        struct SimpleThing *p_sthing;
        p_sthing = &sthings[p_objectv->Thing];
        X = p_sthing->X;
        Z = p_sthing->Z;
    } else {
        struct Thing *p_thing;
        p_thing = &things[p_objectv->Thing];
        X = p_thing->X;
        Z = p_thing->Z;
    }
    SCANNER_init_blippoint(n, X, Z, colr);
    n++;
    return n;
}

ushort do_target_item_scanner(struct Objective *p_objectv, ushort next_signal)
{
    long X, Z;
    ushort n;
    ushort weapon;
    ubyte colr;

    if (objective_target_is_ally(p_objectv))
        colr = colour_lookup[1];
    else if (objective_target_is_to_be_acquired(p_objectv))
        colr = colour_lookup[3];
    else
        colr = colour_lookup[2];

    n = next_signal;
    weapon = p_objectv->Arg2;
    if (p_objectv->Thing <= 0)
    {
        struct SimpleThing *p_sthing;
        p_sthing = &sthings[p_objectv->Thing];
        if ((p_sthing->Type == SmTT_DROPPED_ITEM) &&
          (p_sthing->U.UWeapon.WeaponType == weapon)) {
            X = p_sthing->X;
            Z = p_sthing->Z;
        } else if (p_sthing->Type == SmTT_CARRIED_ITEM) {
            struct Thing *p_person;
            p_person = &things[p_sthing->U.UWeapon.Owner];
            X = p_person->X;
            Z = p_person->Z;
        } else {
            struct Thing *p_person;
            short person;
            person = find_person_carrying_weapon(weapon);
            p_person = &things[person];
            X = p_person->X;
            Z = p_person->Z;
        }
    }
    else
    {
        return n;
    }
    SCANNER_init_blippoint(n, X, Z, colr);
    n++;
    return n;
}

ushort do_thing_arrive_area_scanner(struct Objective *p_objectv, ushort next_signal)
{
    long X, Z;
    ushort n;
    ubyte colr;

    if (objective_target_is_ally(p_objectv))
        colr = colour_lookup[1];
    else if (objective_target_is_to_be_acquired(p_objectv))
        colr = colour_lookup[3];
    else
        colr = colour_lookup[2];

    n = next_signal;
    SCANNER_init_blippoint(n, p_objectv->X << 8, p_objectv->Z << 8, colr);
    n++;
    if (p_objectv->Thing <= 0) {
        struct SimpleThing *p_sthing;
        p_sthing = &sthings[p_objectv->Thing];
        X = p_sthing->X;
        Z = p_sthing->Z;
    } else {
        struct Thing *p_thing;
        p_thing = &things[p_objectv->Thing];
        X = p_thing->X;
        Z = p_thing->Z;
    }
    if (dword_1DB1A0)
    {
        SCANNER_update_arcpoint(0, Z, X, p_objectv->Z << 8, p_objectv->X << 8);
    }
    else
    {
        struct Thing *p_thing;
        p_thing = &things[ingame.TrackThing];
        if (((ingame.TrackThing == 0) || p_thing->Flag & 0x2000) && (ingame.Flags & 0x2000))
            SCANNER_init_arcpoint(Z, X, p_objectv->Z << 8, p_objectv->X << 8, 1);
    }
    SCANNER_keep_arcs = 1;
    return n;
}

ushort do_group_arrive_area_scanner(struct Objective *p_objectv, ushort next_signal)
{
    ushort n;
    ubyte colr;

    if (objective_target_is_ally(p_objectv))
        colr = colour_lookup[1];
    else if (objective_target_is_to_be_acquired(p_objectv))
        colr = colour_lookup[3];
    else
        colr = colour_lookup[2];

    n = next_signal;
    SCANNER_init_blippoint(n, p_objectv->X << 8, p_objectv->Z << 8, colr);
    n++;
    return n;
}

void add_signal_to_scanner(struct Objective *p_objectv, ubyte flag)
{
#if 0
    asm volatile ("call ASM_add_signal_to_scanner\n"
        :  : "a" (p_objectv), "d" (flag));
#endif
    if (flag)
    {
        int i;

        signal_count = 0;
        for (i = 0; i < SCANNER_BIG_BLIP_COUNT; i++)
            ingame.Scanner.BigBlip[i].Period = 0;
        for (i = 0; i < SCANNER_ARC_COUNT; i++)
            ingame.Scanner.Arc[i].Period = 0;
    }
    if (gameturn != turn_last)
    {
        turn_last = gameturn;
        ingame.Scanner.GroupCount = 0;
    }
    if ((p_objectv == NULL) || ((p_objectv->Flags & 0x01) != 0))
        return;

    if (signal_count >= SCANNER_BIG_BLIP_COUNT) {
        LOGWARN("Scaner blips limit reached, blip discarded.");
        return;
    }

    if (objective_target_is_group_to_area(p_objectv))
    {
        signal_count = do_group_arrive_area_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_group(p_objectv))
    {
        signal_count = do_group_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_person_to_area(p_objectv))
    {
        signal_count = do_thing_arrive_area_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_person(p_objectv))
    {
        signal_count = do_target_thing_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_vehicle_to_area(p_objectv))
    {
        signal_count = do_thing_arrive_area_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_vehicle(p_objectv))
    {
        signal_count = do_target_thing_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_item_to_area(p_objectv))
    {
        signal_count = do_thing_arrive_area_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_item(p_objectv))
    {
        signal_count = do_target_item_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_object(p_objectv))
    {
        signal_count = do_target_thing_scanner(p_objectv, signal_count);
    }
    else if (objective_target_is_any_thing(p_objectv))
    {
        signal_count = do_target_thing_scanner(p_objectv, signal_count);
    }
}

/******************************************************************************/
