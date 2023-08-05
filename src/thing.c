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
#include "game.h"
/******************************************************************************/
void init_things(void)
{
    asm volatile ("call ASM_init_things\n"
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

short find_nearest_from_group(struct Thing *p_person, ushort group, ubyte no_persuaded)
{
    short ret;
    asm volatile ("call ASM_find_nearest_from_group\n"
        : "=r" (ret) : "a" (p_person), "d" (group), "b" (no_persuaded));
    return ret;
}

/** Maps fields from old Thing struct to the current one.
 */
void refresh_old_thing_format(struct Thing *p_thing, struct ThingOldV9 *p_oldthing, ulong fmtver)
{
    ushort len;

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
    // Fields from VX to VZ
    len = offsetof(struct Thing, VZ) + sizeof(p_thing->VZ) - offsetof(struct Thing, VX);
    memset(&p_thing->VX, 0, len); // Leftover data in these three causes weird bugs for flying cars when they move
    p_thing->Speed = p_oldthing->Speed;
    p_thing->Health = p_oldthing->Health;
    p_thing->Owner = p_oldthing->Owner;
    // Type-dependent fields which are the same for most types
    p_thing->U.UPerson.UniqueID = p_oldthing->PersonUniqueID;
    p_thing->U.UPerson.Group = p_oldthing->PersonGroup;
    p_thing->U.UPerson.EffectiveGroup = p_oldthing->PersonGroup;
    // Really type-dependent fields
    if (p_thing->Type == TT_PERSON)
    {
        p_thing->U.UPerson.ComHead = p_oldthing->PersonComHead;
        //p_thing->U.UPerson.ComCur = p_oldthing->PersonComCur; -- not yet re-added
        p_thing->U.UPerson.WeaponsCarried = p_oldthing->PersonWeaponsCarried;
        p_thing->U.UPerson.BumpMode = 0;
        p_thing->U.UPerson.LastDist = 0;
        p_thing->U.UPerson.AnimMode = 0;
        // TODO verify - should we clear UMod? We're sanitizing it later, so maybe not...
        // Field FrameId
        len = 2;
        memset(&p_thing->U.UPerson.FrameId.Version[0], 0, len); // Blank part of it, to avoid missing body parts
        if (p_thing->SubType == SubTT_PERS_PUNK_F) {
            // Randomize hair color - 50% normal red(0), 25% blonde(1), 25% blue(2)
            len = (LbRandomAnyShort() & 0xFF);
            if (len < 64)
                p_thing->U.UPerson.FrameId.Version[0] = 1;
            else if (len < 128)
                p_thing->U.UPerson.FrameId.Version[0] = 2;
        }
        p_thing->U.UPerson.MaxHealth = p_oldthing->PersonMaxHealth;
        p_thing->U.UPerson.ShieldEnergy = p_oldthing->PersonShieldEnergy;
        p_thing->U.UPerson.Stamina = p_oldthing->PersonStamina;
        p_thing->U.UPerson.MaxStamina = p_oldthing->PersonMaxStamina;
    }
    else if (p_thing->Type == TT_VEHICLE)
    {
        p_thing->U.UVehicle.MaxHealth = 0; // In old format this is stored in additional vehicle block, not in the thing
    }
}

/******************************************************************************/
