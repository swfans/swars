/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file thing.c
 *     Thing structure support.
 * @par Purpose:
 *     Implement general functions for handling Thing instances.
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
#include "thing.h"

#include "bfutility.h"
#include "bfmemut.h"
#include "building.h"
#include "vehicle.h"
#include "bigmap.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

/** Radiuses of Things of type STATIC.
 */
short static_radii[] = {
  40, 50, 80, 15,  0, 40,  0,  0,  0, 40,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0, 70, 70, 70, 70,
 130, 40, 80, 40, 40, 40, 50,100, 40, 40,
   0,  0,  0,  0,  0,  0,  0,  0, 40, 70,
  40, 40, 80,100,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

void init_things(void)
{
    asm volatile ("call ASM_init_things\n"
        :  :  : "eax" );
}

void process_things(void)
{
    asm volatile ("call ASM_process_things\n"
        :  :  : "eax" );
}

TbResult delete_node(struct Thing *p_thing)
{
    TbResult ret;
    asm volatile ("call ASM_delete_node\n"
        : "=r" (ret) : "a" (p_thing));
    return ret;
}

void add_node_thing(ushort new_thing)
{
    asm volatile ("call ASM_add_node_thing\n"
        : : "a" (new_thing));
}

short get_new_thing(void)
{
    short ret;
    asm volatile ("call ASM_get_new_thing\n"
        : "=r" (ret) : );
    return ret;
}

void remove_thing(short tngno)
{
    asm volatile ("call ASM_remove_thing\n"
        : : "a" (tngno));
}

void add_node_sthing(ushort new_thing)
{
    asm volatile ("call ASM_add_node_sthing\n"
        : : "a" (new_thing));
}

short get_new_sthing(void)
{
    short ret;
    asm volatile ("call ASM_get_new_sthing\n"
        : "=r" (ret) : );
    return ret;
}

void remove_sthing(short tngno)
{
    asm volatile ("call ASM_remove_sthing\n"
        : : "a" (tngno));
}

short add_static(int x, int y, int z, ushort frame, int timer)
{
    short ret;
    asm volatile (
      "push %5\n"
      "call ASM_add_static\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (timer));
    return ret;
}

TbBool thing_is_within_circle(short thing, short X, short Z, ushort R)
{
    long dtX, dtZ, r2;

    if (thing <= 0) {
        struct SimpleThing *p_sthing;
        p_sthing = &sthings[thing];
        dtX = (p_sthing->X >> 8) - X;
        dtZ = (p_sthing->Z >> 8) - Z;
    } else {
        struct Thing *p_thing;
        p_thing = &things[thing];
        dtX = (p_thing->X >> 8) - X;
        dtZ = (p_thing->Z >> 8) - Z;
    }
    r2 = R * R;
    return ((dtZ * dtZ + dtX * dtX) < r2);
}

/** Searches for a thing of given type on the specific mapwho tile.
 * Besides being given a title, the thing must also meet circular range condition.
 */
short find_thing_on_mapwho_tile_within_circle_with_filter(short tile_x, short tile_z, short X, short Z, ushort R,
  short ttype, short subtype, ThingBoolFilter filter, ThingFilterParams *params)
{
    short thing;
    ulong k;

    k = 0;
    thing = get_mapwho_thing_index(tile_x, tile_z);
    while (thing != 0)
    {
        if (thing <= 0)
        {
            struct SimpleThing *p_sthing;
            p_sthing = &sthings[thing];
            // Per thing code start
            if (p_sthing->Type == ttype) {
                if ((p_sthing->SubType == subtype) || (subtype == -1)) {
                    // Our search radius could have exceeded expected one a bit
                    if (thing_is_within_circle(thing, X, Z, R)) {
                        if (filter(thing, params))
                            return thing;
                    }
                }
            }
            // Per thing code end
            thing = p_sthing->Next;
        }
        else
        {
            struct Thing *p_thing;
            p_thing = &things[thing];
            // Per thing code start
            if (p_thing->Type == ttype) {
                if ((p_thing->SubType == subtype) || (subtype == -1)) {
                    // Our search radius could have exceeded expected one a bit
                    if (thing_is_within_circle(thing, X, Z, R)) {
                        if (filter(thing, params))
                            return thing;
                    }
                }
            }
            // Per thing code end
            thing = p_thing->Next;
        }
        k++;
        if (k >= STHINGS_LIMIT+THINGS_LIMIT) {
            LOGERR("Infinite loop in mapwho things list");
            break;
        }
    }
    return 0;
}

/**
 * Searches for thing of given type and subtype around tile under given coords.
 * Uses `spiral` checking of surrounding `mapwho` tiles, up to given number of tiles.
 *
 * @return Gives thing index, or 0 if not found.
 */
static short find_thing_type_on_spiral_near_tile(short X, short Z, ushort R, long spiral_len,
  short ttype, short subtype, ThingBoolFilter filter, ThingFilterParams *params)
{
    short tile_x, tile_z;
    int around;

    tile_x = X >> 8;
    tile_z = Z >> 8;
    for (around = 0; around < spiral_len; around++)
    {
        struct MapOffset *sstep;
        short thing;
        long sX, sZ;

        sstep = &spiral_step[around];
        sX = tile_x + sstep->h;
        sZ = tile_z + sstep->v;
        thing = find_thing_on_mapwho_tile_within_circle_with_filter(sX, sZ, X, Z, R,
          ttype, subtype, filter, params);
        if (thing != 0)
            return thing;
    }
    return 0;
}

short get_thing_same_type_head(short ttype, short subtype)
{
    short thing;

    switch (ttype)
    {
    case TT_PERSON:
    case TT_UNKN4:
        thing = same_type_head[1];
        break;
    case TT_VEHICLE:
        if (subtype == SubTT_VEH_SHIP)
            thing = same_type_head[5];
        else
            thing = same_type_head[2];
        break;
    case TT_BUILDING:
        if (subtype == SubTT_BLD_MGUN)
            thing = same_type_head[7];
        else
            thing = same_type_head[3];
        break;
    case TT_MINE:
        if (subtype == 48)
            thing = same_type_head[6];
        break;
    default:
        thing = 0;
        break;
    }
    return thing;
}

TbBool thing_type_is_simple(short ttype)
{
    return (ttype == SmTT_STATIC) ||
     (ttype == SmTT_SPARK) ||
     (ttype == SmTT_INTELLIG_DOOR) ||
     (ttype == SmTT_SCALE_EFFECT) ||
     (ttype == SmTT_NUCLEAR_BOMB) ||
     (ttype == SmTT_SMOKE_GENERATOR) ||
     (ttype == SmTT_DROPPED_ITEM) ||
     (ttype == SmTT_CARRIED_ITEM) ||
     (ttype == SmTT_ELECTRIC_STRAND) ||
     (ttype == SmTT_TIME_POD) ||
     (ttype == SmTT_CANISTER) ||
     (ttype == SmTT_STASIS_POD) ||
     (ttype == SmTT_SOUL) ||
     (ttype == SmTT_BANG) ||
     (ttype == SmTT_FIRE) ||
     (ttype == SmTT_SFX) ||
     (ttype == SmTT_TEMP_LIGHT);
}

static short find_thing_type_on_same_type_list_within_circle(short X, short Z, ushort R,
  short ttype, short subtype, ThingBoolFilter filter, ThingFilterParams *params)
{
    short thing;
    ulong k;

    k = 0;
    thing = get_thing_same_type_head(ttype, subtype);
    while (thing != 0)
    {
        if (thing <= 0)
        {
            struct SimpleThing *p_sthing;
            p_sthing = &sthings[thing];
            // Per thing code start
            if (p_sthing->Type == ttype) {
                if ((p_sthing->SubType == subtype) || (subtype == -1)) {
                    if (thing_is_within_circle(thing, X, Z, R)) {
                        if (filter(thing, params))
                            return thing;
                    }
                }
            }
            // Per thing code end
            thing = p_sthing->LinkSame;
        }
        else
        {
            struct Thing *p_thing;
            p_thing = &things[thing];
            // Per thing code start
            if (p_thing->Type == ttype) {
                if ((p_thing->SubType == subtype) || (subtype == -1)) {
                    if (thing_is_within_circle(thing, X, Z, R)) {
                        if (filter(thing, params))
                            return thing;
                    }
                }
            }
            // Per thing code end
            thing = p_thing->LinkSame;
        }
        // If searching for all subtypes, make sure we really catch them all; switch to
        // second linked list if forst one did not gave results
        if ((thing == 0) && (subtype == -1) && (ttype == TT_VEHICLE)) {
            subtype = SubTT_VEH_SHIP;
            thing = get_thing_same_type_head(ttype, subtype);
        }
        if ((thing == 0) && (subtype == -1) && (ttype == TT_BUILDING)) {
            subtype = SubTT_BLD_MGUN;
            thing = get_thing_same_type_head(ttype, subtype);
        }
        k++;
        if (k >= STHINGS_LIMIT+THINGS_LIMIT) {
            LOGERR("Infinite loop in mapwho things list");
            break;
        }
    }
    return 0;
}

static short find_thing_type_on_same_type_list(short ttype, short subtype,
  ThingBoolFilter filter, ThingFilterParams *params)
{
    short thing;
    ulong k;

    k = 0;
    thing = get_thing_same_type_head(ttype, subtype);
    while (thing != 0)
    {
        if (thing <= 0)
        {
            struct SimpleThing *p_sthing;
            p_sthing = &sthings[thing];
            // Per thing code start
            if (p_sthing->Type == ttype) {
                if ((p_sthing->SubType == subtype) || (subtype == -1)) {
                    if (filter(thing, params))
                        return thing;
                }
            }
            // Per thing code end
            thing = p_sthing->LinkSame;
        }
        else
        {
            struct Thing *p_thing;
            p_thing = &things[thing];
            // Per thing code start
            if (p_thing->Type == ttype) {
                if ((p_thing->SubType == subtype) || (subtype == -1)) {
                    if (filter(thing, params))
                        return thing;
                }
            }
            // Per thing code end
            thing = p_thing->LinkSame;
        }
        // If searching for all subtypes, make sure we really catch them all; switch to
        // second linked list if forst one did not gave results
        if ((thing == 0) && (subtype == -1) && (ttype == TT_VEHICLE)) {
            subtype = SubTT_VEH_SHIP;
            thing = get_thing_same_type_head(ttype, subtype);
        }
        if ((thing == 0) && (subtype == -1) && (ttype == TT_BUILDING)) {
            subtype = SubTT_BLD_MGUN;
            thing = get_thing_same_type_head(ttype, subtype);
        }
        k++;
        if (k >= STHINGS_LIMIT+THINGS_LIMIT) {
            LOGERR("Infinite loop in mapwho things list");
            break;
        }
    }
    return 0;
}

static short find_thing_type_on_used_list_within_circle(short X, short Z, ushort R,
  short ttype, short subtype, ThingBoolFilter filter, ThingFilterParams *params)
{
    short thing;

    if (thing_type_is_simple(ttype))
    {
        struct SimpleThing *p_sthing;
        for (thing = sthings_used_head; thing < 0; thing = p_sthing->LinkChild)
        {
            p_sthing = &sthings[thing];
            // Per thing code start
            if (p_sthing->Type == ttype) {
                if ((p_sthing->SubType == subtype) || (subtype == -1)) {
                    if (thing_is_within_circle(thing, X, Z, R)) {
                        if (filter(thing, params))
                            return thing;
                    }
                }
            }
            // Per thing code end
        }
    }
    else
    {
        struct Thing *p_thing;
        for (thing = things_used_head; thing > 0; thing = p_thing->LinkChild)
        {
            p_thing = &things[thing];
            // Per thing code start
            if (p_thing->Type == ttype) {
                if ((p_thing->SubType == subtype) || (subtype == -1)) {
                    if (thing_is_within_circle(thing, X, Z, R)) {
                        if (filter(thing, params))
                            return thing;
                    }
                }
            }
            // Per thing code end
        }
    }
    return 0;
}

short find_thing_type_within_circle_with_filter(short X, short Z, ushort R,
  short ttype, short subtype, ThingBoolFilter filter, ThingFilterParams *params)
{
    ushort tile_dist;
    short thing;

    tile_dist = (R + 256) >> 8;
    if (tile_dist <= spiral_dist_tiles_limit)
    {
        thing = find_thing_type_on_spiral_near_tile(X, Z, R,
          dist_tiles_to_spiral_step[tile_dist], ttype, subtype, filter, params);
    }
    else if ((ttype == TT_PERSON) || (ttype == TT_UNKN4) || (ttype == TT_VEHICLE) || (ttype == TT_BUILDING))
    {
        thing = find_thing_type_on_same_type_list_within_circle(
          X, Z, R, ttype, subtype, filter, params);
    }
    else
    {
        thing = find_thing_type_on_used_list_within_circle(
          X, Z, R, ttype, subtype, filter, params);
    }
    return thing;
}

TbBool bfilter_item_is_weapon(short thing, ThingFilterParams *params)
{
    struct SimpleThing *p_sthing;
    short weapon;

    if (thing >= 0)
        return false;
    weapon = params->Arg1;

    p_sthing = &sthings[thing];
    if ((p_sthing->Type != SmTT_DROPPED_ITEM) && (p_sthing->Type != SmTT_CARRIED_ITEM))
        return false;

    return (weapon == -1) || (p_sthing->U.UWeapon.WeaponType == weapon);
}

TbBool bfilter_person_carries_weapon(short thing, ThingFilterParams *params)
{
    struct Thing *p_thing;
    short weapon;

    if (thing <= 0)
        return false;
    weapon = params->Arg1;

    p_thing = &things[thing];
    if (p_thing->Type != TT_PERSON)
        return false;

    return (weapon == -1) || person_carries_weapon(p_thing, weapon);
}

short find_dropped_weapon_within_circle(short X, short Z, ushort R, short weapon)
{
    short thing;
    ThingFilterParams params;

    params.Arg1 = weapon;
    thing = find_thing_type_within_circle_with_filter(X, Z, R, SmTT_DROPPED_ITEM, 0, bfilter_item_is_weapon, &params);

    return thing;
}

short find_person_carrying_weapon_within_circle(short X, short Z, ushort R, short weapon)
{
    short thing;
    ThingFilterParams params;

    params.Arg1 = weapon;
    thing = find_thing_type_within_circle_with_filter(X, Z, R, TT_PERSON, -1, bfilter_person_carries_weapon, &params);

    return thing;
}

short find_nearest_from_group(struct Thing *p_person, ushort group, ubyte no_persuaded)
{
    short ret;
    asm volatile ("call ASM_find_nearest_from_group\n"
        : "=r" (ret) : "a" (p_person), "d" (group), "b" (no_persuaded));
    return ret;
}

short find_person_carrying_weapon(short weapon)
{
    short thing;
    ThingFilterParams params;

    params.Arg1 = weapon;
    thing = find_thing_type_on_same_type_list(TT_PERSON, -1, bfilter_person_carries_weapon, &params);

    return thing;
}

short search_things_for_index(short index)
{
#if 0
    short ret;
    asm volatile ("call ASM_search_things_for_index\n"
        : "=r" (ret) : "a" (index));
    return ret;
#endif
    short thing;
    if (index <= 0)
    {
        struct SimpleThing *p_sthing;
        for (thing = sthings_used_head; thing < 0; thing = p_sthing->LinkChild)
        {
            p_sthing = &sthings[thing];
            if (index == p_sthing->ThingOffset) {
                return thing;
            }
        }
    }
    else
    {
        struct Thing *p_thing;
        for (thing = things_used_head; thing > 0; thing = p_thing->LinkChild)
        {
            p_thing = &things[thing];
            if (index == p_thing->ThingOffset) {
                if (p_thing->Type != TT_UNKN33)
                    return thing;
            }
        }
    }
    return 0;
}

short search_things_for_uniqueid(short index, ubyte flag)
{
    short ret;
    asm volatile ("call ASM_search_things_for_uniqueid\n"
        : "=r" (ret) : "a" (index), "d" (flag));
    return ret;
}

short find_nearest_object2(short mx, short mz, ushort sub_type)
{
    short ret;
    asm volatile ("call ASM_find_nearest_object2\n"
        : "=r" (ret) : "a" (mx), "d" (mz), "b" (sub_type));
    return ret;
}

short search_object_for_qface(ushort object, ubyte gflag, ubyte flag, ushort after)
{
    short ret;
    asm volatile ("call ASM_search_object_for_qface\n"
        : "=r" (ret) : "a" (object), "d" (gflag), "b" (flag), "c" (after));
    return ret;
}

short search_for_station(short x, short z)
{
    ushort ret;
    asm volatile ("call ASM_search_for_station\n"
        : "=r" (ret) : "a" (x), "d" (z));
    return ret;
}

/** Maps fields from old Thing struct to the current one.
 */
void refresh_old_thing_format(struct Thing *p_thing, struct ThingOldV9 *p_oldthing, ulong fmtver)
{
    ushort len;

    LbMemorySet(p_thing, 0, sizeof(struct Thing));

    p_thing->Parent = p_oldthing->Parent;
    p_thing->Next = p_oldthing->Next;
    p_thing->LinkParent = p_oldthing->LinkParent;
    p_thing->LinkChild = p_oldthing->LinkChild;
    p_thing->SubType = p_oldthing->SubType;
    p_thing->Type = p_oldthing->Type;
    p_thing->State = p_oldthing->State;
    p_thing->Flag = p_oldthing->Flag;
    p_thing->LinkSame = p_oldthing->LinkSame;
    p_thing->Radius = p_oldthing->Radius;
    p_thing->X = p_oldthing->X;
    p_thing->Y = p_oldthing->Y;
    p_thing->Z = p_oldthing->Z;
    p_thing->Frame = p_oldthing->Frame;
    p_thing->StartFrame = p_oldthing->StartFrame;
    p_thing->Timer1 = p_oldthing->Timer1;
    p_thing->StartTimer1 = p_oldthing->StartTimer1;
    p_thing->LinkSameGroup = p_oldthing->LinkSameGroup;
    p_thing->ThingOffset = p_oldthing->ThingOffset;
    p_thing->VX = p_oldthing->VX;
    p_thing->VY = p_oldthing->VY;
    p_thing->VZ = p_oldthing->VZ;
    p_thing->Speed = p_oldthing->Speed;
    p_thing->Health = p_oldthing->Health;
    p_thing->Owner = p_oldthing->Owner;
    p_thing->PathOffset = p_oldthing->PathOffset;
    // The target pointer will be cleared anyway
    p_thing->PTarget = NULL; // p_oldthing->PTarget;
    p_thing->GotoThingIndex = p_oldthing->GotoThingIndex;
    p_thing->SubState = p_oldthing->SubState;

    // Type-dependent fields
    if (p_thing->Type == TT_PERSON)
    {
        p_thing->U.UPerson.AnimMode = p_oldthing->PersonAnimMode;
        p_thing->U.UPerson.OldAnimMode = p_oldthing->PersonOldAnimMode;
        p_thing->U.UPerson.Timer2 = p_oldthing->Timer2;
        p_thing->U.UPerson.StartTimer2 = p_oldthing->StartTimer2;
        p_thing->U.UPerson.Angle = p_oldthing->PersonAngle;
        p_thing->U.UPerson.GotoX = p_oldthing->PersonGotoX;
        p_thing->U.UPerson.GotoZ = p_oldthing->PersonGotoZ;
        p_thing->U.UPerson.Group = p_oldthing->PersonGroup;
        p_thing->U.UPerson.EffectiveGroup = p_oldthing->PersonEffectiveGroup;
        p_thing->U.UPerson.WeaponsCarried = p_oldthing->PersonWeaponsCarried;
        p_thing->U.UPerson.CurrentWeapon = p_oldthing->PersonCurrentWeapon;
        p_thing->U.UPerson.ComHead = p_oldthing->PersonComHead;
        p_thing->U.UPerson.ComCur = p_oldthing->PersonComCur;
        p_thing->U.UPerson.ComTimer = p_oldthing->PersonComTimer;
        p_thing->U.UPerson.Brightness = p_oldthing->PersonBrightness;
        p_thing->U.UPerson.MaxShieldEnergy = p_oldthing->PersonMaxShieldEnergy;
        p_thing->U.UPerson.UniqueID = p_oldthing->PersonUniqueID;
        p_thing->U.UPerson.PathIndex = p_oldthing->PersonPathIndex;
        p_thing->U.UPerson.WeaponTimer = p_oldthing->PersonWeaponTimer;
        p_thing->U.UPerson.WeaponTurn = p_oldthing->PersonWeaponTurn;
        p_thing->U.UPerson.Energy = p_oldthing->PersonEnergy;
        p_thing->U.UPerson.MaxEnergy = p_oldthing->PersonMaxEnergy;
        // Uncertain fields
        p_thing->U.UPerson.ShieldEnergy = p_oldthing->PersonShieldEnergy;
        p_thing->U.UPerson.SpecialTimer = p_oldthing->PersonSpecialTimer;
        p_thing->U.UPerson.ComRange = p_oldthing->PersonComRange;
        p_thing->U.UPerson.BumpMode = p_oldthing->PersonBumpMode;
        p_thing->U.UPerson.BumpCount = p_oldthing->PersonBumpCount;
        p_thing->U.UPerson.Vehicle = p_oldthing->PersonVehicle;
        p_thing->U.UPerson.LinkPassenger = p_oldthing->PersonLinkPassenger;
        p_thing->U.UPerson.Within = p_oldthing->PersonWithin;
        p_thing->U.UPerson.LastDist = p_oldthing->PersonLastDist;
        p_thing->U.UPerson.OnFace = p_oldthing->PersonOnFace;
        // End of uncertain fields
        p_thing->U.UPerson.ComRange = p_oldthing->PersonComRange;
        p_thing->U.UPerson.Shadows[0] = p_oldthing->PersonShadows[0];
        p_thing->U.UPerson.Shadows[1] = p_oldthing->PersonShadows[1];
        p_thing->U.UPerson.Shadows[2] = p_oldthing->PersonShadows[2];
        p_thing->U.UPerson.Shadows[3] = p_oldthing->PersonShadows[3];
        p_thing->U.UPerson.RecoilTimer = p_oldthing->PersonRecoilTimer;
        p_thing->U.UPerson.MaxHealth = p_oldthing->PersonMaxHealth;
        p_thing->U.UPerson.RecoilDir = p_oldthing->PersonRecoilDir;
        // Having cybernetic mods is kind of important; but we do not have location of these
        //p_thing->U.UPerson.UMod.Mods = p_oldthing->?;
        if (fmtver >= 12) {
            p_thing->U.UPerson.Stamina = p_oldthing->PersonStamina;
            p_thing->U.UPerson.MaxStamina = p_oldthing->PersonMaxStamina;
        }
        // Field FrameId
        if (p_thing->SubType == SubTT_PERS_PUNK_F) {
            // Randomize hair color - 50% normal red(0), 25% blonde(1), 25% blue(2)
            // The ThingOffset should be random enough
            len = (p_thing->ThingOffset ^ p_thing->U.UPerson.UniqueID) & 0xF;
            if (len < 4)
                p_thing->U.UPerson.FrameId.Version[0] = 1;
            else if (len < 8)
                p_thing->U.UPerson.FrameId.Version[0] = 2;
        }
    }
    else if (p_thing->Type == TT_VEHICLE)
    {
        p_thing->U.UVehicle.Object = p_oldthing->VehicleObject;
        p_thing->U.UVehicle.NumbObjects = p_oldthing->VehicleNumbObjects;
        p_thing->U.UVehicle.Timer2 = p_oldthing->Timer2;
        p_thing->U.UVehicle.AngleX = p_oldthing->VehicleAngleX;
        p_thing->U.UVehicle.AngleY = p_oldthing->VehicleAngleY;
        p_thing->U.UVehicle.AngleZ = p_oldthing->VehicleAngleZ;
        p_thing->U.UVehicle.GotoX = p_oldthing->VehicleGotoX;
        p_thing->U.UVehicle.GotoY = p_oldthing->VehicleGotoY;
        p_thing->U.UVehicle.GotoZ = p_oldthing->VehicleGotoZ;
        p_thing->U.UVehicle.MatrixIndex = p_oldthing->VehicleMatrixIndex;
        p_thing->U.UVehicle.UniqueID = p_oldthing->VehicleUniqueID;
        p_thing->U.UVehicle.MaxSpeed = p_oldthing->VehicleReqdSpeed;
        p_thing->U.UVehicle.ReqdSpeed = p_oldthing->VehicleReqdSpeed;
        p_thing->U.UVehicle.PassengerHead = p_oldthing->VehiclePassengerHead;
        p_thing->U.UVehicle.TNode = p_oldthing->VehicleTNode;
        p_thing->U.UVehicle.AngleDY = p_oldthing->VehicleAngleDY;
        p_thing->U.UVehicle.RecoilTimer = p_oldthing->VehicleRecoilTimer;
        // Only one group seem to exist in the old vehicles
        p_thing->U.UVehicle.Group = p_oldthing->VehicleEffectiveGroup;
        p_thing->U.UVehicle.EffectiveGroup = p_oldthing->VehicleEffectiveGroup;
        // In old format, MaxHealth is stored in additional vehicle block, not in the thing
        //p_thing->U.UVehicle.MaxHealth = ?;
    }
    else if (p_thing->Type == TT_BUILDING)
    {
        p_thing->U.UObject.Object = p_oldthing->ObjectObject;
        p_thing->U.UObject.NumbObjects = p_oldthing->ObjectNumbObjects;
        p_thing->U.UObject.Timer[0] = p_oldthing->ObjectTimer0;
        p_thing->U.UObject.Timer[1] = p_oldthing->ObjectTimer1;
        p_thing->U.UObject.Angle = p_oldthing->ObjectAngle;
        p_thing->U.UObject.TargetDX = p_oldthing->ObjectTargetDX;
        p_thing->U.UObject.TargetDY = p_oldthing->ObjectTargetDY;
        p_thing->U.UObject.TargetDZ = p_oldthing->ObjectTargetDZ;
        // Only one group seem to exist in the old Objects
        p_thing->U.UObject.Group = p_oldthing->ObjectEffectiveGroup;
        p_thing->U.UObject.EffectiveGroup = p_oldthing->ObjectEffectiveGroup;
        if (p_thing->SubType == SubTT_BLD_GATE) {
            p_thing->U.UObject.RaiseDY[0] = p_oldthing->ObjectRaiseDY[0];
            p_thing->U.UObject.RaiseDY[1] = p_oldthing->ObjectRaiseDY[1];
            p_thing->VX = 0;
            p_thing->VY = 0;
            p_thing->VZ = 0;
            p_thing->U.UObject.MinY[0] = p_oldthing->ObjectMinY0;
            p_thing->U.UObject.RaiseY[1] = p_oldthing->ObjectRaiseY1;
            p_thing->Frame = 0;
            p_thing->StartFrame = 0;
        }
        else if (p_thing->SubType == SubTT_BLD_MGUN) {
            p_thing->U.UMGun.Object = p_oldthing->MGunObject;
            p_thing->U.UMGun.NumbObjects = p_oldthing->MGunNumbObjects;
            p_thing->U.UMGun.AngleX = p_oldthing->MGunAngleX;
            p_thing->U.UMGun.AngleY = p_oldthing->MGunAngleY;
            p_thing->U.UMGun.AngleZ = p_oldthing->MGunAngleZ;
            p_thing->U.UMGun.GotoX = p_oldthing->MGunGotoX;
            p_thing->U.UMGun.GotoY = p_oldthing->MGunGotoY;
            p_thing->U.UMGun.GotoZ = p_oldthing->MGunGotoZ;
            p_thing->U.UMGun.UniqueID = p_oldthing->MGunUniqueID;
            // Only one group seem to exist in the old MGun
            p_thing->U.UMGun.Group = p_oldthing->MGunEffectiveGroup;
            p_thing->U.UMGun.EffectiveGroup = p_oldthing->MGunEffectiveGroup;
        }
    }
}

/******************************************************************************/
