/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file pepgroup.c
 *     People groups support.
 * @par Purpose:
 *     Implement handling parameters of groups to which people are assigned.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2023 - 11 Jul 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "pepgroup.h"

#include "bfutility.h"
#include "bfmemut.h"
#include "thing.h"
#include "game.h"
/******************************************************************************/

short find_unused_group_id(TbBool largest)
{
    short thing, group;
    ulong used_groups;
    struct Thing *p_thing;

    used_groups = 0;
    for (thing = things_used_head; thing != 0; thing = p_thing->LinkChild)
    {
        p_thing = &things[thing];
        used_groups |= (1 << p_thing->U.UPerson.Group);
    }
    if (largest)
    {
        for (group = PEOPLE_GROUPS_COUNT-1; group > 0; group--) {
            if ((used_groups & (1 << group)) == 0)
                return group;
        }
    }
    else
    {
        for (group = 1; group < PEOPLE_GROUPS_COUNT; group++) {
            if ((used_groups & (1 << group)) == 0)
                return group;
        }
    }
    return -1;
}

ushort count_people_in_group(ushort group, short subtype)
{
    short thing;
    struct Thing *p_thing;
    ushort count;

    count = 0;
    for (thing = things_used_head; thing != 0; thing = p_thing->LinkChild)
    {
        p_thing = &things[thing];

        if (p_thing->U.UPerson.Group != group)
            continue;

        if (p_thing->Type != TT_PERSON)
            continue;

        if ((subtype != -1) && (p_thing->SubType != subtype))
            continue;

        count++;
    }
    return count;
}

void thing_group_copy(short pv_group, short nx_group, ubyte allow_kill)
{
    int i;

    for (i = 0; i < PEOPLE_GROUPS_COUNT; i++)
    {
        if (i == pv_group)
        {
            war_flags[i].KillOnSight &= ~(1 << pv_group);
            war_flags[i].KillIfWeaponOut &= ~(1 << pv_group);
            war_flags[i].KillIfArmed &= ~(1 << pv_group);
            war_flags[i].KillOnSight &= ~(1 << nx_group);
            war_flags[i].KillIfWeaponOut &= ~(1 << nx_group);
            war_flags[i].KillIfArmed &= ~(1 << nx_group);
            continue;
        }
        if ((war_flags[i].KillOnSight & (1 << pv_group)) != 0)
            war_flags[i].KillOnSight |= (1 << nx_group);
        else
            war_flags[i].KillOnSight &= ~(1 << nx_group);

        if ((war_flags[i].KillIfWeaponOut & (1 << pv_group)) != 0)
            war_flags[i].KillIfWeaponOut |= (1 << nx_group);
        else
            war_flags[i].KillIfWeaponOut &= ~(1 << nx_group);

        if ((war_flags[i].KillIfArmed & (1 << pv_group)) != 0)
            war_flags[i].KillIfArmed |= (1 << nx_group);
        else
            war_flags[i].KillIfArmed &= ~(1 << nx_group);

        if ((war_flags[i].Truce & (1 << pv_group)) != 0)
            war_flags[i].Truce |= (1 << nx_group);
        else
            war_flags[i].Truce &= ~(1 << nx_group);
    }

    LbMemoryCopy(&war_flags[nx_group], &war_flags[pv_group], sizeof(struct WarFlag));

    if (allow_kill & 0x01) {
        war_flags[pv_group].KillOnSight |= (1 << nx_group);
        war_flags[nx_group].KillOnSight |= (1 << pv_group);
    }
    if (allow_kill & 0x02) {
        war_flags[pv_group].KillIfWeaponOut |= (1 << nx_group);
        war_flags[nx_group].KillIfWeaponOut |= (1 << pv_group);
    }
    if (allow_kill & 0x04) {
        war_flags[pv_group].KillIfArmed |= (1 << nx_group);
        war_flags[nx_group].KillIfArmed |= (1 << pv_group);
    }

    for (i = 0; i < 8; i++)
    {
        if (level_def.PlayableGroups[i] == pv_group) {
            level_def.PlayableGroups[i] = nx_group;
            if (pv_group == 0) // If replacing group 0, don't modify all the filler zeros
                break;
        }
    }
}

/******************************************************************************/
