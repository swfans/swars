/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvobjctv.c
 *     Routines for level objectives handling.
 * @par Purpose:
 *     Implement functions for handling and checking level objectives.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "lvobjctv.h"

#include <string.h>
#include <stdlib.h>
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfini.h"
#include "bfkeybd.h"
#include "thing.h"
#include "pepgroup.h"
#include "player.h"
#include "scanner.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

enum ObjectiveDefFlags {
    ObDF_None = 0x0000,
    /* === What to place in Thing/UniqueId === */
    /** Group index in Thing field */
    ObDF_ReqGroup = 0x0001,
    /** Person reference in Thing and UniqueID fields */
    ObDF_ReqPerson = 0x0002,
    /** Vehicle reference in Thing and UniqueID fields */
    ObDF_ReqVehicle = 0x0004,
    /** Carried Item reference in Thing and UniqueID fields; this also uses
     * Arg2 field, filling it with type of weapon stored in the item */
    ObDF_ReqItem = 0x0008,
    /** Object (ie building) index in Thing field */
    ObDF_ReqObject = 0x0010,
    /** Any Thing reference in Thing and UniqueID fields */
    ObDF_ReqThing = 0x0020,
    /** Count/Amount in Thing field */
    ObDF_ReqCount = 0x0020,
    /* === What to place in Coord/Radius === */
    /** Coordinates in X/Y/Z fields (or only X/Z, if the Y is reused) */
    ObDF_ReqCoord = 0x0100,
    /** Radius/Range in Radius field */
    ObDF_ReqRadius = 0x0200,
    /** Amount in the Y field; if used, it leaves only X/Z for coords */
    ObDF_ReqAmountY = 0x0400,
    /** Thing index in the Y field; if used, it leaves only X/Z for coords */
    ObDF_ReqThingY = 0x0800,
    /* === What to place in Arg2 === */
    ObDF_ReqAmount = 0x1000,
    ObDF_ReqSecTng = 0x2000, // TODO this can cause bugs, use only Thing for thing index
    ObDF_ReqSecGrp = 0x4000,
};

struct ObjectiveDef {
    const char *CmdName;
    const char *DefText;
    ulong Flags;
};

struct ObjectiveDef objectv_defs[] = {
    /* Unreachable. */
    {"GAME_OBJ_NONE",		"DO NOTHING",		ObDF_None },
    /* Require the target person to be dead or destroyed. */
    {"GAME_OBJ_P_DEAD",		"ASSASSINATE",		ObDF_ReqPerson },
    /* Require whole group to be neutralized. */
    {"GAME_OBJ_ALL_G_DEAD",	"ELIMINATE GROUP",	ObDF_ReqGroup },
    /* Require at least specified amount of group members to be dead or destroyed. */
    {"GAME_OBJ_MEM_G_DEAD",	"KILL GROUP MEM",	ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require the Person and the Thing to be within Radius around each other. */
    {"GAME_OBJ_P_NEAR",		"RENDEZVOUS",		ObDF_ReqPerson|ObDF_ReqThingY|ObDF_ReqRadius },
    /* Require specified amount of group members to be within radius around given thing. */
    {"GAME_OBJ_MEM_G_NEAR",	"RENDEZVOUS MEM",	ObDF_ReqThing|ObDF_ReqSecGrp|ObDF_ReqAmountY|ObDF_ReqRadius },
    /* Require the target person to be within given radius around given coordinates. */
    {"GAME_OBJ_P_ARRIVES",	"GOTO LOCATION",	ObDF_ReqPerson|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require at least specified amount of group members to be within radius around given coords. */
    {"GAME_OBJ_MEM_G_ARRIVES", "GOTO LOCATION", ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius|ObDF_ReqAmount },
    /* Require all of group members to be within radius around given coords. */
    {"GAME_OBJ_ALL_G_ARRIVES", "ALL GOTO LOCATION", ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require target person to be within the group belonging to local player. */
    {"GAME_OBJ_PERSUADE_P",	"PERSUADE",			ObDF_ReqPerson },
    /* Require at least specified amount of group members to be within the local player group. */
    {"GAME_OBJ_PERSUADE_MEM_G", "PERSUADE GANG MEM", ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require all of group members to be persuaded. */
    {"GAME_OBJ_PERSUADE_ALL_G", "PERSUADE ALL GANG", ObDF_ReqGroup },
    /* Require specified amount of game turns to pass. */
    {"GAME_OBJ_TIME",		"TIMER",			ObDF_ReqCount },
    /* Require specified carried item to change owner to a person belonging to local player. */
    {"GAME_OBJ_GET_ITEM",	"COLLECT ITEM",		ObDF_ReqItem },
    /* Unreachable. Require specified item to be used? */
    {"GAME_OBJ_USE_ITEM",	"USE ITEM",			ObDF_ReqItem },
    /* Unreachable. Require acquiring specified amount of funds? */
    {"GAME_OBJ_FUNDS",		"GET BULLION",		ObDF_None },
    /* Require given thing to have DESTROYED flag set.
     * Coords need to be provided, rather than UniqueId, to find the object thing in case it changed. */
    {"GAME_OBJ_DESTROY_OBJECT", "DESTROY BUILDING", ObDF_ReqObject|ObDF_ReqCoord },
    /** Require the target person to either be DESTROYED or change owner to local player group. */
    {"GAME_OBJ_PKILL_P",	"NEUTRALISE",		ObDF_ReqPerson },
    /* Require all of group members to be killed or persuaded by local player group. */
    {"GAME_OBJ_PKILL_MEM_G", "NEUTRALISE MEM",	ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require all of group members to either be DESTROYED or change owner to local player group. */
    {"GAME_OBJ_PKILL_ALL_G", "NEUTRALISE G",	ObDF_ReqGroup },
    /* Unreachable. Require using P.A. Network? */
    {"GAME_OBJ_USE_PANET",	"USE P.A.NET",		ObDF_None },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_21",	"UNEXPECT 21",		ObDF_None },
    /* Fail if any of group members are dead/destroyed. The only negative objective. */
    {"GAME_OBJ_PROTECT_G",	"PROTECT GROUP",	ObDF_ReqGroup },
    /* Require all of group members to change owner to specified person. */
    {"GAME_OBJ_P_PERS_G",	"PEEP PERSUADE ALL", ObDF_ReqGroup|ObDF_ReqSecTng },
    /* Require all of group members to either be dead/destroyed or within specified vehicle. */
    {"GAME_OBJ_ALL_G_USE_V", "ALL USE VEHICLE",	ObDF_ReqVehicle|ObDF_ReqSecGrp },
    /* Require at least specified amount of group members to be within specified vehicle. */
    {"GAME_OBJ_MEM_G_USE_V", "MEM USE VEHICLE",	ObDF_ReqVehicle|ObDF_ReqSecGrp|ObDF_ReqAmountY },
    /* Require the target vehicle to be within given radius around given coordinates. */
    {"GAME_OBJ_V_ARRIVES",	"DRIVE TO LOCATION",ObDF_ReqVehicle|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require given thing to have DESTROYED flag set. */
    {"GAME_OBJ_DESTROY_V", "DESTROY VEHICLE",   ObDF_ReqVehicle },
    /* Require the target person to be within given radius around given coordinates. */
    {"GAME_OBJ_ITEM_ARRIVES", "BRING TO LOCATION",	ObDF_ReqItem|ObDF_ReqCoord|ObDF_ReqRadius },
    {NULL,					NULL,				ObDF_None },
};

/* deprecated */
const char *gameobjctv_names[] = {
    "GAME_OBJ_NONE",
    "GAME_OBJ_P_DEAD",
    "GAME_OBJ_ALL_G_DEAD",
    "GAME_OBJ_MEM_G_DEAD",
    "GAME_OBJ_P_NEAR",
    "GAME_OBJ_MEM_G_NEAR",
    "GAME_OBJ_P_ARRIVES",
    "GAME_OBJ_MEM_G_ARRIVES",
    "GAME_OBJ_ALL_G_ARRIVES",
    "GAME_OBJ_PERSUADE_P",
    "GAME_OBJ_PERSUADE_MEM_G",
    "GAME_OBJ_PERSUADE_ALL_G",
    "GAME_OBJ_TIME",
    "GAME_OBJ_GET_ITEM",
    "GAME_OBJ_USE_ITEM",
    "GAME_OBJ_FUNDS",
    "GAME_OBJ_DESTROY_OBJECT",
    "GAME_OBJ_PKILL_P",
    "GAME_OBJ_PKILL_MEM_G",
    "GAME_OBJ_PKILL_ALL_G",
    "GAME_OBJ_USE_PANET",
    "GAME_OBJ_UNUSED_21",
    "GAME_OBJ_PROTECT_G",
    "GAME_OBJ_P_PERS_G",
    "GAME_OBJ_ALL_G_USE_V",
    "GAME_OBJ_MEM_G_USE_V",
    "GAME_OBJ_V_ARRIVES",
    "GAME_OBJ_DESTROY_V",
    "GAME_OBJ_ITEM_ARRIVES",
};

enum ObjectiveConfigParam {
    ObvP_Thing = 1,
    ObvP_Group,
    ObvP_Count,
    ObvP_UniqueID,
    ObvP_Coord,
    ObvP_Radius,
    ObvP_Amount,
    ObvP_SecGroup,
    ObvP_SecThing,
    ObvP_Arg2,
    ObvP_StringIndex,
    ObvP_Flags,
    ObvP_TextId,
};

const struct TbNamedEnum missions_conf_objective_params[] = {
  {"Thing",			ObvP_Thing},
  {"Group",			ObvP_Group},
  {"Count",			ObvP_Count},
  {"UniqueID",		ObvP_UniqueID},
  {"Coord",			ObvP_Coord},
  {"Radius",		ObvP_Radius},
  {"Amount",		ObvP_Amount},
  {"SecGroup",		ObvP_SecGroup},
  {"SecThing",		ObvP_SecThing},
  {"Arg2",			ObvP_Arg2},
  {"StringIndex",	ObvP_StringIndex},
  {"Flags",			ObvP_Flags},
  {"TextId",		ObvP_TextId},
  {NULL,			0},
};


int add_used_objective(long mapno, long levelno)
{
    struct Objective *p_objectv;
    int objectv;

    objectv = next_used_objective;
    next_used_objective++;

    p_objectv = &game_used_objectives[objectv];
    LbMemorySet(p_objectv, 0, sizeof(struct Objective));
    p_objectv->Type = 0;
    p_objectv->Pri = 0;
    p_objectv->Map = mapno;
    p_objectv->Level = (levelno - 1) % 15 + 1;
    p_objectv->Status = 0;

    return objectv;
}

void draw_objective(ushort objectv, ubyte flag)
{
    struct Objective *p_objectv;
    ushort bkpType;
    // workaround due to scanner not understanding new objectives
    p_objectv = &game_used_objectives[objectv];
    bkpType = p_objectv->Type;
    switch (bkpType)
    {
    case GAME_OBJ_MEM_G_USE_V:
        p_objectv->Type = GAME_OBJ_ALL_G_USE_V;
        break;
    case GAME_OBJ_V_ARRIVES:
    case GAME_OBJ_ITEM_ARRIVES:
        p_objectv->Type = GAME_OBJ_P_ARRIVES;
        break;
    case GAME_OBJ_DESTROY_V:
        p_objectv->Type = GAME_OBJ_P_DEAD;
        break;
    }
    asm volatile ("call ASM_draw_objective\n"
        : : "a" (objectv), "d" (flag));
    p_objectv->Type = bkpType;
}

TbBool thing_arrived_at_obj(short thing, struct Objective *p_objectv)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_thing_arrived_at_obj\n"
        : "=r" (ret) : "a" (thing), "d" (p_objectv));
    return ret;
#endif
    return thing_is_within_circle(thing, p_objectv->X, p_objectv->Z, p_objectv->Radius << 6);
}

TbBool thing_is_destroyed(short thing)
{
    struct Thing *p_thing;
    p_thing = &things[thing];
    return ((p_thing->Flag & 0x0002) != 0);
}

TbBool person_is_dead(short thing)
{
    struct Thing *p_thing;

    if (thing <= 0)
        return false;

    p_thing = &things[thing];

    if (p_thing->Type != TT_PERSON)
        return false;

    return (p_thing->State == PerSt_DEAD);
}

TbBool thing_arrived_at_objectv(short thing, struct Objective *p_objectv)
{
    if (thing == 0)
        return false;
    if (thing_is_destroyed(thing))
        return false;
    return thing_is_within_circle(thing, p_objectv->X, p_objectv->Z, p_objectv->Radius << 6);
}

TbBool item_arrived_at_objectv(short thing, ushort weapon, struct Objective *p_objectv)
{
    struct SimpleThing *p_sthing;

    if (thing >= 0)
        return false;

    p_sthing = &sthings[thing];

    if ((p_sthing->Type == SmTT_DROPPED_ITEM) || (p_sthing->Type == SmTT_CARRIED_ITEM))
    {
        if (!thing_is_destroyed(thing))
            return thing_is_within_circle(thing, p_objectv->X, p_objectv->Z, p_objectv->Radius << 6);
    }
    // If the target is no longer a correct thing, then it is now either carried weapon or
    // a different dropped weapon (dropping it created another thing)
    thing = find_dropped_weapon_within_circle(p_objectv->X, p_objectv->Z, p_objectv->Radius << 6, weapon);
    if (thing != 0)
        return true;
    thing = find_person_carrying_weapon_within_circle(p_objectv->X, p_objectv->Z, p_objectv->Radius << 6, weapon);
    return (thing != 0);
}

/** Returns if given item, or the weapon which it represented, is carried
 * by given player.
 *
 * Many pickups are CARRIED_ITEM things, converted from DROPPED_ITEM.
 * But for weapons, the DROPPED_ITEM is deleted rather than converted.
 * So to make the function quasi-work for weapons, we check if the player
 * acquired said weapon. We can't check Unique ID to make sure this is the
 * exact same weapon instance, but that should not matter in most practical
 * uses of the objective.
 */
TbBool item_is_carried_by_player(short thing, ushort plyr, ushort weapon)
{
    struct SimpleThing *p_sthing;
    short plyagent, plygroup;
    struct Thing *p_person;
    PlayerInfo *p_player;
    int i;

    if (thing >= 0)
        return false;

    p_sthing = &sthings[thing];
    p_player = &players[plyr];

    if ((p_sthing->Type == SmTT_DROPPED_ITEM) &&
      (p_sthing->U.UWeapon.WeaponType == weapon))
        return false;

    plyagent = p_player->DirectControl[0];
    plygroup = things[plyagent].U.UPerson.Group;

    if (p_sthing->Type == SmTT_CARRIED_ITEM)
    {
        p_person = &things[p_sthing->U.UWeapon.Owner];
        return (p_person->U.UPerson.Group == plygroup);
    }

    for (i = 0; i < 4; i++)
    {
        struct Thing *p_agent;
        p_agent = p_player->MyAgent[i];
        if (person_carries_weapon(p_agent, weapon))
            return true;
    }
    return false;
}

TbBool person_is_persuaded(short thing)
{
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_person = &things[thing];
    return ((p_person->Flag & 0x80000) != 0);
}

TbBool person_is_persuaded_by_person(short thing, short owntng)
{
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_person = &things[thing];
    if ((p_person->Flag & 0x80000) == 0)
        return false;

    return (p_person->Owner == owntng);
}

TbBool person_is_persuaded_by_player(short thing, ushort plyr)
{
    struct Thing *p_thing;
    short plyagent, plygroup;
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_thing = &things[thing];
    if ((p_thing->Flag & 0x80000) == 0)
        return false;

    plyagent = players[plyr].DirectControl[0];
    plygroup = things[plyagent].U.UPerson.Group;

    p_person = &things[p_thing->Owner];
    return (p_person->U.UPerson.Group == plygroup);
}

ubyte all_group_arrived(ushort group, short x, short y, short z, int radius)
{
    ubyte ret;
    asm volatile (
      "push %5\n"
      "call ASM_all_group_arrived\n"
        : "=r" (ret) : "a" (group), "d" (x), "b" (y), "c" (z), "g" (radius));
    return ret;
}

TbBool all_group_persuaded(ushort group)
{
    short thing;
    struct Thing *p_thing;

    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
            p_thing = &things[thing];
            if (!person_is_persuaded(thing) || ((things[p_thing->Owner].Flag & 0x2000) == 0))
            {
                if (!person_is_dead(thing) && !thing_is_destroyed(thing))
                    return false;
            }
    }
    return true;
}

TbBool group_all_killed_or_persuaded_by_player(ushort group, ushort plyr)
{
    short thing;
    struct Thing *p_thing;

    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (!person_is_persuaded_by_player(thing, plyr))
        {
            if (!person_is_dead(thing) && !thing_is_destroyed(thing))
                return false;
        }
    }
    return true;
}

TbBool group_all_survivors_in_vehicle(ushort group, short vehicle)
{
    short thing;
    struct Thing *p_thing;

    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (p_thing->U.UPerson.Vehicle != vehicle)
        {
            if (!person_is_dead(thing) && !thing_is_destroyed(thing))
                return false;
        }
    }
    return false;
}

TbBool group_members_in_vehicle(ushort group, short vehicle, ushort amount)
{
    short thing;
    struct Thing *p_thing;
    ushort n;

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (!person_is_dead(thing) && !thing_is_destroyed(thing))
        {
            if (p_thing->U.UPerson.Vehicle == vehicle)
                n++;
        }
        if (n >= amount)
            return true;
    }
    return false;
}

TbBool group_members_persuaded_by_player(ushort group, ushort plyr, ushort amount)
{
    short thing;
    struct Thing *p_thing;
    ushort n;

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (person_is_persuaded_by_player(thing, plyr))
            n++;
        if (n >= amount)
            return true;
    }
    return false;
}

TbBool group_members_killed_or_persuaded_by_player(ushort group, ushort plyr, ushort amount)
{
    short thing;
    struct Thing *p_thing;
    ushort n;

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (person_is_persuaded_by_player(thing, plyr) ||
          person_is_dead(thing) || thing_is_destroyed(thing))
            n++;
        if (n >= amount)
            return true;
    }
    return false;
}

TbBool group_members_dead(ushort group, ushort amount)
{
    short thing;
    struct Thing *p_thing;
    ushort n;

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (person_is_dead(thing) || thing_is_destroyed(thing))
            n++;
        if (n >= amount)
            return true;
    }
    return false;
}

TbBool person_is_near_thing(short neartng, short thing, ushort radius)
{
    short nearX, nearZ;

    if ((neartng == 0) || person_is_dead(neartng) || thing_is_destroyed(neartng))
        return false;
    if (neartng <= 0) {
        struct SimpleThing *p_neartng;
        p_neartng = &sthings[neartng];
        nearX = p_neartng->X;
        nearZ = p_neartng->Z;
    } else {
        struct Thing *p_neartng;
        p_neartng = &things[neartng];
        nearX = p_neartng->X;
        nearZ = p_neartng->Z;
    }

    if ((thing == 0) || person_is_dead(thing) || thing_is_destroyed(thing))
        return false;

    return (thing_is_within_circle(thing, nearX, nearZ, radius << 6));
}


TbBool group_members_near_thing(short neartng, ushort group, ushort amount, ushort radius)
{
    short thing;
    struct Thing *p_thing;
    ushort n;
    short nearX, nearZ;

    if ((neartng == 0) || person_is_dead(neartng) || thing_is_destroyed(neartng))
        return false;
    if (neartng <= 0) {
        struct SimpleThing *p_neartng;
        p_neartng = &sthings[neartng];
        nearX = p_neartng->X;
        nearZ = p_neartng->Z;
    } else {
        struct Thing *p_neartng;
        p_neartng = &things[neartng];
        nearX = p_neartng->X;
        nearZ = p_neartng->Z;
    }

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (!person_is_dead(thing) && !thing_is_destroyed(thing))
        {
            if (thing_is_within_circle(thing, nearX, nearZ, radius << 6))
                n++;
        }
        if (n >= amount)
            return true;
    }
    return false;
}

TbBool group_members_persuaded_by_person(ushort group, short owntng, ushort amount)
{
    short thing;
    struct Thing *p_thing;
    ushort n;

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (person_is_persuaded_by_person(thing, owntng))
            n++;
        if (n >= amount)
            return true;
    }
    return false;
}

TbBool group_members_arrived_at_objectv(ushort group, struct Objective *p_objectv, ushort amount)
{
    short thing;
    struct Thing *p_thing;
    ushort n;

    n = 0;
    thing = same_type_head[256 + group];
    for (; thing > 0; thing = p_thing->LinkSameGroup)
    {
        p_thing = &things[thing];
        if (thing_arrived_at_obj(thing, p_objectv))
            n++;
        if (n >= amount)
            return true;
    }
    return false;
}

ubyte fix_single_objective(struct Objective *p_objectv, ushort objectv, const char *srctext)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_fix_single_objective\n"
        : "=r" (ret) : "a" (p_objectv));
    return ret;
#else
    struct ObjectiveDef *p_odef;
    short thing;
    ubyte ret;

    // Skip objectives for other levels
    if (p_objectv->Level != (current_level - 1) % 15 + 1)
        return 1;
    if (p_objectv->Map != current_map)
        return 1;

    p_odef = &objectv_defs[p_objectv->Type];

    ret = 1;
    if ((p_odef->Flags & ObDF_ReqGroup) != 0)
    {
        if ((p_objectv->Thing < 0) || (p_objectv->Thing >= PEOPLE_GROUPS_COUNT)) {
            LOGERR("Objv%s%d = %s Group(%hd) out of range",
              srctext, objectv, p_odef->CmdName, p_objectv->Thing);
            p_objectv->Thing = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqPerson) != 0)
    {
        {
            thing = search_things_for_index(p_objectv->Thing);
            if (thing <= 0) {
                thing = 0;
            } else {
                struct Thing *p_thing = &things[thing];
                if (p_thing->Type != TT_PERSON)
                    thing = 0;
                else if (p_thing->U.UPerson.UniqueID != p_objectv->UniqueID)
                    thing = 0;
            }
        }
        if (thing == 0) {
            thing = search_things_for_uniqueid(p_objectv->UniqueID, 1);
            if (thing <= 0) {
                thing = 0;
            } else {
                struct Thing *p_thing = &things[thing];
                if (p_thing->Type != TT_PERSON)
                    thing = 0;
            }
        }
#if 0 // If UniqueID does not match, it is probably better to give up
        if (thing == 0) {
            thing = p_objectv->Thing;
            if ((thing <= 0) || (thing >= THINGS_LIMIT)) {
                thing = 0;
            } else {
                struct Thing *p_thing = &things[thing];
                if (p_thing->Type != TT_PERSON)
                    thing = 0;
            }
        }
#endif
        if (thing != 0) {
            p_objectv->Thing = thing;
            if (ret) ret = 2;
        } else {
            LOGERR("Objv%s%d = %s Thing(%hd,%hu) not found",
              srctext, objectv, p_odef->CmdName,
              p_objectv->Thing, p_objectv->UniqueID);
            p_objectv->Thing = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqVehicle) != 0)
    {
        {
            thing = search_things_for_index(p_objectv->Thing);
            if (thing <= 0) {
                thing = 0;
            } else {
                struct Thing *p_thing = &things[thing];
                if (p_thing->Type != TT_VEHICLE)
                    thing = 0;
                else if (p_thing->U.UVehicle.UniqueID != p_objectv->UniqueID)
                    thing = 0;
            }
        }
        if (thing == 0) {
            thing = search_things_for_uniqueid(p_objectv->UniqueID, 1);
            if (thing <= 0) {
                thing = 0;
            } else {
                struct Thing *p_thing = &things[thing];
                if (p_thing->Type != TT_VEHICLE)
                    thing = 0;
            }
        }
        if (thing != 0) {
            p_objectv->Thing = thing;
            if (ret) ret = 2;
        } else {
            LOGERR("Objv%s%d = %s Thing(%hd,%hu) not found",
              srctext, objectv, p_odef->CmdName,
              p_objectv->Thing, p_objectv->UniqueID);
            p_objectv->Thing = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqItem) != 0)
    {
        {
            thing = search_things_for_index(p_objectv->Thing);
            // One mistake which often happens on the map, is that we have static
            // lights assigned as items. This should be really fixed in the level,
            // but there is also no reson not to try fix it here.
            if (thing < 0) {
                struct SimpleThing *p_sthing = &sthings[thing];
                if (p_sthing->Type == SmTT_STATIC) {
                    LOGWARN("Objv%s%d = %s target Thing(%hd,%hu) index %hd is a static; jumping to parent",
                      srctext, objectv, p_odef->CmdName,
                      p_objectv->Thing, p_objectv->UniqueID, thing);
                    thing = p_sthing->Parent;
                }
            }
            if (thing >= 0) {
                thing = 0;
            } else {
                struct SimpleThing *p_sthing = &sthings[thing];
                if ((p_sthing->Type != SmTT_CARRIED_ITEM) && (p_sthing->Type != SmTT_DROPPED_ITEM))
                    thing = 0;
                else if (p_sthing->UniqueID != p_objectv->UniqueID)
                    thing = 0;
            }
        }
        if (thing == 0) {
            thing = search_things_for_uniqueid(p_objectv->UniqueID, 0);
            if (thing >= 0) {
                thing = 0;
            } else {
                struct SimpleThing *p_sthing = &sthings[thing];
                if ((p_sthing->Type != SmTT_CARRIED_ITEM) && (p_sthing->Type != SmTT_DROPPED_ITEM))
                    thing = 0;
                else if (p_sthing->UniqueID != p_objectv->UniqueID)
                    thing = 0;
            }
        }
        if (thing != 0) {
            struct SimpleThing *p_sthing = &sthings[thing];
            p_objectv->Thing = thing;
            p_objectv->Arg2 = p_sthing->U.UWeapon.WeaponType;
            if (ret) ret = 2;
        } else {
            LOGERR("Objv%s%d = %s Thing(%hd,%hu) not found",
              srctext, objectv, p_odef->CmdName,
              p_objectv->Thing, p_objectv->UniqueID);
            p_objectv->Thing = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqObject) != 0)
    {
        {
            thing = find_nearest_object2(p_objectv->X, p_objectv->Z, 0);
        }
        if (thing == 0) {
            thing = search_things_for_index(p_objectv->Thing);
            if (thing <= 0) {
                thing = 0;
            } else {
                struct Thing *p_thing = &things[thing];
                if (p_thing->Type != TT_BUILDING)
                    thing = 0;
            }
        }
        if (thing != 0) {
            p_objectv->Thing = thing;
            if (ret) ret = 2;
        } else {
            LOGERR("Objv%s%d = %s Thing(%hd,%hu) not found",
              srctext, objectv, p_odef->CmdName,
              p_objectv->Thing, p_objectv->UniqueID);
            p_objectv->Thing = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqThing) != 0)
    {
        {
            thing = search_things_for_index(p_objectv->Thing);
        }
        if (thing == 0) { // Not all things have UniqueID, but worth to try
            thing = search_things_for_uniqueid(p_objectv->UniqueID, 1);
        }
        if (thing == 0) {
            thing = search_things_for_uniqueid(p_objectv->UniqueID, 0);
        }
        if (thing != 0) {
            p_objectv->Thing = thing;
            if (ret) ret = 2;
        } else {
            LOGERR("Objv%s%d = %s Thing(%hd,%hu) not found",
              srctext, objectv, p_odef->CmdName,
              p_objectv->Thing, p_objectv->UniqueID);
            p_objectv->Thing = 0;
            ret = 0;
        }
    }

    if ((p_odef->Flags & ObDF_ReqSecGrp) != 0)
    {
        if ((p_objectv->Arg2 <= 0) || (p_objectv->Arg2 >= PEOPLE_GROUPS_COUNT)) {
            LOGERR("Objv%s%d = %s SecGroup(%d) out of range",
              srctext, objectv, p_odef->CmdName, (int)p_objectv->Arg2);
            p_objectv->Arg2 = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqSecTng) != 0)
    {
        {
            thing = search_things_for_index(p_objectv->Arg2);
        }
        if (thing > 255) { // TODO when possible , stop storing thing index in Arg2
            LOGERR("Objv%s%d = %s SecThing(%d) exceeds 8-bit Arg2 range",
              srctext, objectv, p_odef->CmdName, (int)p_objectv->Arg2);
            ret = 0;
        } else if (thing != 0) {
            p_objectv->Arg2 = thing;
            if (ret) ret = 2;
        } else {
            LOGERR("Objv%s%d = %s SecThing(%d) not found",
              srctext, objectv, p_odef->CmdName, (int)p_objectv->Arg2);
            p_objectv->Arg2 = 0;
            ret = 0;
        }
    }
    else if ((p_odef->Flags & ObDF_ReqAmount) != 0)
    {
        if (p_objectv->Arg2 < 1) {
            LOGERR("Objv%s%d = %s Amount(%d) out of range",
              srctext, objectv, p_odef->CmdName, (int)p_objectv->Arg2);
            p_objectv->Arg2 = 1;
            ret = 0;
        }
    }

    return ret;
#endif
}

short test_objective(ushort objectv, ushort show_obj)
{
#if 0
    short ret;
    asm volatile ("call ASM_test_objective\n"
        : "=r" (ret) : "a" (objectv), "d" (show_obj));
    return ret;
#else
    struct Objective *p_objectv;
    short thing, thing2, group, amount;

    if (show_obj == 2)
    {
        p_objectv = &game_used_lvl_objectives[objectv];
        if (((ingame.Cheats & 0x04) != 0) &&
          (p_objectv->Status != 2) && lbKeyOn[KC_BACKSLASH])
        {
            lbKeyOn[KC_BACKSLASH] = 0;
            p_objectv->Status = 2;
        }
        if (p_objectv->Status == 2)
            return 1;
        if (p_objectv->Status == 1)
            return 0;
    }
    else
    {
        p_objectv = &game_used_objectives[objectv];
        if (((ingame.Cheats & 0x04) != 0) &&
          (p_objectv->Status != 2) && lbKeyOn[KC_BACKSLASH])
        {
            lbKeyOn[KC_BACKSLASH] = 0;
            p_objectv->Status = 2;
        }
        if (p_objectv->Status == 2)
            return 1;
        if (((p_objectv->Flags & 0x01) == 0) && word_1C8446 && (show_obj != 0))
            draw_objective(objectv, 0);
        if (show_obj != 0)
            add_signal_to_scanner(p_objectv, 0);
        if ((p_objectv->Flags & 0x02) != 0)
            return 1;
        if (p_objectv->Status == 1)
            return -1;
    }

    switch (p_objectv->Type)
    {
    case GAME_OBJ_P_DEAD:
        thing = p_objectv->Thing;
        if (person_is_dead(thing) || thing_is_destroyed(thing)) {
            p_objectv->Status = 2;
            return 1;
        }
        p_objectv->Status = 0;
        break;
    case GAME_OBJ_ALL_G_DEAD:
        group = p_objectv->Thing;
        if (group_actions[group].Alive <= 0) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_MEM_G_DEAD:
        group = p_objectv->Thing;
        amount = p_objectv->Arg2;
        if (group_members_dead(group, amount)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_P_NEAR:
        thing = p_objectv->Thing;
        thing2 = p_objectv->Y;
        if (person_is_near_thing(thing, thing2, p_objectv->Radius)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_MEM_G_NEAR:
        thing = p_objectv->Thing;
        group = p_objectv->Arg2;
        amount = p_objectv->Y;
        if (group_members_near_thing(thing, group, amount, p_objectv->Radius)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_P_ARRIVES:
        thing = p_objectv->Thing;
        if (thing_arrived_at_objectv(thing, p_objectv)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_MEM_G_ARRIVES:
        group = p_objectv->Thing;
        amount = p_objectv->Arg2;
        if (group_members_arrived_at_objectv(group, p_objectv, amount)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_ALL_G_ARRIVES:
        group = p_objectv->Thing;
        if (all_group_arrived(group, p_objectv->X, p_objectv->Y, p_objectv->Z, p_objectv->Radius)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_PERSUADE_P:
        thing = p_objectv->Thing;
        if (person_is_persuaded_by_player(thing, local_player_no)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_PERSUADE_MEM_G:
        group = p_objectv->Thing;
        amount = p_objectv->Arg2;
        if (group_members_persuaded_by_player(group, local_player_no, amount)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_PERSUADE_ALL_G:
        group = p_objectv->Thing;
        if (all_group_persuaded(group)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_TIME:
        p_objectv->Radius++;
        if (p_objectv->Radius >= p_objectv->Thing) {
            p_objectv->Radius = 0;
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_GET_ITEM:
        thing = p_objectv->Thing;
        if (item_is_carried_by_player(thing, local_player_no, p_objectv->Arg2)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_USE_ITEM:
    case GAME_OBJ_FUNDS:
        //TODO implement
        break;
    case GAME_OBJ_DESTROY_OBJECT:
        thing = p_objectv->Thing;
        if (thing_is_destroyed(thing)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_PKILL_P:
        thing = p_objectv->Thing;
        if (person_is_persuaded_by_player(thing, local_player_no) ||
          person_is_dead(thing) || thing_is_destroyed(thing)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_PKILL_MEM_G:
        group = p_objectv->Thing;
        amount = p_objectv->Arg2;
        if (group_members_killed_or_persuaded_by_player(group, local_player_no, amount)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_PKILL_ALL_G:
        group = p_objectv->Thing;
        if (group_all_killed_or_persuaded_by_player(group, local_player_no)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_USE_PANET:
    case GAME_OBJ_UNUSED_21:
        //TODO implement
        break;
    case GAME_OBJ_PROTECT_G:
        group = p_objectv->Thing;
        if (group_members_dead(group, 1)) {
            p_objectv->Status = 1;
            return 0;
        }
        break;
    case GAME_OBJ_P_PERS_G:
        group = p_objectv->Thing;
        thing = p_objectv->Arg2;
        if (group_members_persuaded_by_person(group, thing, 1)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_ALL_G_USE_V:
        group = p_objectv->Arg2;
        thing = p_objectv->Thing;
        if (group_all_survivors_in_vehicle(group, thing)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_MEM_G_USE_V:
        group = p_objectv->Arg2;
        thing = p_objectv->Thing;
        amount = p_objectv->Y;
        if (group_members_in_vehicle(group, thing, amount)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_V_ARRIVES:
        thing = p_objectv->Thing;
        if (thing_arrived_at_objectv(thing, p_objectv)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    case GAME_OBJ_DESTROY_V:
        thing = p_objectv->Thing;
        if (thing_is_destroyed(thing)) {
            p_objectv->Status = 2;
            return 1;
        }
        p_objectv->Status = 0;
        break;
    case GAME_OBJ_ITEM_ARRIVES:
        thing = p_objectv->Thing;
        if (item_arrived_at_objectv(thing, p_objectv->Arg2, p_objectv)) {
            p_objectv->Status = 2;
            return 1;
        }
        break;
    }
    return 0;
#endif
}

void snprint_objective(char *buf, ulong buflen, ushort objectv)
{
    struct Objective *p_objectv;
    struct ObjectiveDef *p_odef;
    char *s;
    ubyte nparams;

    p_objectv = &game_used_objectives[objectv];
    p_odef = &objectv_defs[p_objectv->Type];

    s = buf;

    sprintf(s, "P%02d = %s( ", (int)p_objectv->Pri, p_odef->CmdName);
    s += strlen(s);
    nparams = 0;

    if ((p_odef->Flags & ObDF_ReqGroup) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Group(%hd)", p_objectv->Thing);
        s += strlen(s);
        nparams++;
    } else if ((p_odef->Flags & ObDF_ReqCount) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Count(%hd)", p_objectv->Thing);
        s += strlen(s);
        nparams++;
    } else if (((p_odef->Flags & ObDF_ReqThing) != 0) ||
      ((p_odef->Flags & ObDF_ReqPerson) != 0) ||
      ((p_odef->Flags & ObDF_ReqVehicle) != 0) ||
      ((p_odef->Flags & ObDF_ReqItem) != 0) ||
      ((p_odef->Flags & ObDF_ReqObject) != 0) ||
      (p_objectv->Thing != 0) || (p_objectv->UniqueID != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Thing(%hd,%hu)", p_objectv->Thing, p_objectv->UniqueID);
        s += strlen(s);
        nparams++;
    }

    // Unexpected to have unique id without thing
    if (((p_odef->Flags & (ObDF_ReqThing|ObDF_ReqPerson|ObDF_ReqVehicle|
      ObDF_ReqItem|ObDF_ReqObject)) == 0) && (p_objectv->UniqueID != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "UniqueID(%hu)", p_objectv->UniqueID);
        s += strlen(s);
    }

    if (((p_odef->Flags & ObDF_ReqCoord) != 0) ||
      (p_objectv->X|p_objectv->Y|p_objectv->Z) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Coord(%hd,%hd,%hd)", p_objectv->X, p_objectv->Y, p_objectv->Z);
        s += strlen(s);
        nparams++;
    }
    if (((p_odef->Flags & ObDF_ReqRadius) != 0) ||
      (p_objectv->Radius != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Radius(%hd)", p_objectv->Radius);
        s += strlen(s);
        nparams++;
    }
    if ((p_odef->Flags & ObDF_ReqAmount) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Amount(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    } else if ((p_odef->Flags & ObDF_ReqSecGrp) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "SecGroup(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    } else if ((p_odef->Flags & ObDF_ReqSecTng) != 0) {
        // TODO storing a ThingOffset in 8-bit value is risky; when possible, switch Arg2 with Thing
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "SecThing(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    } else if (p_objectv->Arg2 != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Arg2(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    }
    if (p_objectv->StringIndex != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "StringIndex(%hu)", p_objectv->StringIndex);
        s += strlen(s);
    }
    if (p_objectv->Flags != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Flags(%hu)", p_objectv->Flags);
        s += strlen(s);
        nparams++;
    }
    if (p_objectv->ObjText != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "TextId(%d)", (int)p_objectv->ObjText);
        s += strlen(s);
    }
    if ((p_objectv->field_1B[0]|p_objectv->field_1B[1]|p_objectv->field_1B[2]|p_objectv->field_1B[3]) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "field_1B(%d,%d,%d,%d)", (int)p_objectv->field_1B[0],
          (int)p_objectv->field_1B[1], (int)p_objectv->field_1B[2], (int)p_objectv->field_1B[3]);
        s += strlen(s);
    }
    if (p_objectv->field_1F != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "field_1F(%d)", (int)p_objectv->field_1F);
        s += strlen(s);
    }

    snprintf(s, buflen - (s-buf), " )");
}

void save_objective_chain_conf(TbFileHandle fh, ushort objectv_head, char *buf, ulong buflen)
{
    ushort objectv;

    objectv = 0;
    while (objectv != objectv_head)
    {
        { // Go backwards through single-directional chain
            ushort nxobjectv;
            nxobjectv = objectv_head;
            while (game_used_objectives[nxobjectv].Next != objectv)
                nxobjectv = game_used_objectives[nxobjectv].Next;
            objectv = nxobjectv;
        }

        snprint_objective(buf, buflen, objectv);
        strncat(buf, "\n", buflen);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (objectv_head == 0) {
        sprintf(buf, "; no objectives defined\n");
        LbFileWrite(fh, buf, strlen(buf));
    }
}

int tokenize_script_func(char *olist[], char *obuf, const char *ibuf, long ibuflen)
{
    TbBool in_quotes, token_end, parse_end;
    int li;
    int opos, pos, in_parath;

    li = 0;
    pos = 0;
    in_quotes = false;
    token_end = true;
    parse_end = false;
    in_parath = 0;
    if (pos >= ibuflen) return 0;
    // Skipping spaces after previous parameter
    while ((ibuf[pos] == ' ') || (ibuf[pos] == '\t'))
    {
        pos++;
        if (pos >= ibuflen) return 0;
    }
    if (ibuf[pos] == '\"') {
        in_quotes = true;
        pos++;
        if (pos >= ibuflen) return 0;
    }
    opos = 0;
    while (!parse_end)
    {
        if (token_end) {
            olist[li] = &obuf[opos];
            li++;
            token_end = false;
        }
        for (; pos < ibuflen; pos++)
        {
            if (in_quotes) {
                if ((ibuf[pos] == '\"') ||
                     (ibuf[pos] == '\r') ||
                     (ibuf[pos] == '\n')) {
                    in_quotes = false;
                    if (in_parath == 0) {
                        token_end = true;
                        continue; // do not copy quote to output
                    }
                }
            } else if (ibuf[pos] == '\"') {
                in_quotes = true;
                if (in_parath == 0)
                    continue; // do not copy quote to output
            } else if (ibuf[pos] == '(') {
                in_parath++;
                if (in_parath == 1) {
                    token_end = true;
                    // do not copy first parenthesis to output
                    continue;
                }
            } else if (ibuf[pos] == ')') {
                in_parath--;
                if (in_parath == 0) {
                    // do not copy last parenthesis to output, and end here
                    parse_end = true;
                    break;
                }
            } else if (in_parath < 2) {
                if (ibuf[pos] == ',') {
                    token_end = true;
                    continue;
                } else if ((ibuf[pos] == ' ') ||
                     (ibuf[pos] == '\t') ||
                     ((uchar)ibuf[pos] < 7)) {
                    continue;
                } else if ((ibuf[pos] == '\r') ||
                     (ibuf[pos] == '\n')) {
                    parse_end = true;
                    break;
                } else if (token_end) {
                    // we found a char to include in a token, but that's a new one
                    break;
                }
            }
            obuf[opos] = ibuf[pos];
            opos++;
            if (pos >= ibuflen) {
                opos++;
                parse_end = true;
                break;
            }
        }
        obuf[opos] = '\0';
        opos++;
    }
    olist[li] = NULL;
    return li;
}

int parse_objective_param(struct Objective *p_objectv, const char *buf, long buflen)
{
    char *toklist[16];
    char tokbuf[128];
    int i;

    LbMemorySet(toklist, 0, sizeof(toklist));
    i = tokenize_script_func(toklist, tokbuf, buf, buflen);
    if (i < 2) {
        LOGWARN("Objective parameter consists of less than 2 tokens.");
        return -1;
    }

    // Finding parameter number
    i = 0;
    while (1)
    {
        const struct TbNamedEnum *param;

        if (missions_conf_objective_params[i].name == NULL) {
            i = -1;
            break;
        }
        param = &missions_conf_objective_params[i];
        if (strcasecmp(toklist[0], param->name) == 0) {
            i = param->num;
            break;
        }
        i++;
    }
    switch (i)
    {
    case ObvP_Group:
    case ObvP_Count:
        p_objectv->Thing = atoi(toklist[1]);
        break;
    case ObvP_Thing:
        if (toklist[2] == NULL)  {
            LOGWARN("Objective parameter \"%s\" requires 2 numbers.", toklist[0]);
            return -1;
        }
        p_objectv->Thing = atoi(toklist[1]);
        p_objectv->UniqueID = atoi(toklist[2]);
        break;
    case ObvP_UniqueID:
        p_objectv->UniqueID = atoi(toklist[1]);
        break;
    case ObvP_Coord:
        if (toklist[3] == NULL)  {
            LOGWARN("Objective parameter \"%s\" requires 3 numbers.", toklist[0]);
            return -1;
        }
        p_objectv->X = atoi(toklist[1]);
        p_objectv->Y = atoi(toklist[2]);
        p_objectv->Z = atoi(toklist[3]);
        break;
    case ObvP_Radius:
        p_objectv->Radius = atoi(toklist[1]);
        break;
    case ObvP_Amount:
    case ObvP_SecGroup:
    case ObvP_SecThing:
    case ObvP_Arg2:
        p_objectv->Arg2 = atoi(toklist[1]);
        break;
    case ObvP_StringIndex:
        p_objectv->StringIndex = atoi(toklist[1]);
        break;
    case ObvP_Flags:
        p_objectv->Flags = atoi(toklist[1]);
        break;
    case ObvP_TextId:
        p_objectv->ObjText = atoi(toklist[1]);
        break;
    default:
        LOGWARN("Objective parameter name \"%s\" not recognized.", toklist[0]);
        return -1;
    }
    return 1;
}

int parse_next_used_objective(const char *buf, long buflen, long pri, long mapno, long levelno)
{
    struct ObjectiveDef *p_odef;
    struct Objective *p_objectv;
    char *toklist[32];
    char tokbuf[256];
    int i, objectv;

    tokenize_script_func(toklist, tokbuf, buf, buflen);

    // Finding command number
    i = 0;
    while (objectv_defs[i].CmdName != NULL)
    {
        p_odef = &objectv_defs[i];
        if (strcasecmp(toklist[0], p_odef->CmdName) == 0)
            break;
        i++;
    }
    p_odef = &objectv_defs[i];
    if (p_odef->CmdName == NULL) {
        LOGWARN("Objective name not recognized.");
        return -1;
    }
    objectv = add_used_objective(mapno, levelno);
    p_objectv = &game_used_objectives[objectv];
    p_objectv->Type = i;
    p_objectv->Pri = pri;

    for (i = 1; toklist[i] != NULL; i++)
    {
        parse_objective_param(p_objectv, toklist[i], sizeof(tokbuf) - (toklist[i] - tokbuf) );
    }

    return objectv;
}

/******************************************************************************/
