/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file people.c
 *     Routines implementing people.
 * @par Purpose:
 *     Implement general functions for handling person things.
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
#include "people.h"
#include "game.h"
#include "thing.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/
struct PeepStat peep_type_stats[] = {
    {   0,    0,    0,    0,   0, 0,   0, 0, 0, 0},
    {1000, 1024, 2048, 1024,  20, 5, 512, 0, 0, 0},
    {1000, 1024, 2048, 1024,  20, 1, 512, 0, 0, 0},
    { 600,  524, 2048,  601,  10, 2, 312, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 172, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 160, 0, 0, 0},
    { 600,  600,  900,  601,  15, 3, 312, 0, 0, 0},
    {4000, 1000,  800, 2001,9999, 2, 362, 0, 0, 0},
    { 600,  400,  800,  601,   6, 2, 362, 0, 0, 0},
    { 600,  524, 2024,  601,  10, 2, 312, 0, 0, 0},
    { 200,  100,  300,  200,   0, 1, 212, 0, 0, 0},
    { 600,  524, 2024,  601,   0, 1, 312, 0, 0, 0},
    {2000, 1024, 2048, 1024,  20, 5, 512, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 150, 0, 0, 0},
    { 110,  100,  400,  300,   0, 1, 212, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 212, 0, 0, 0},
    {   0,    0,    0,    0,   0, 0,   0, 0, 0, 0},
};


void set_person_stats_type(struct Thing *p_person, ushort type)
{
    asm volatile ("call ASM_set_person_stats_type\n"
        : : "a" (p_person), "d" (type));
}

ubyte person_mod_chest_level(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods >> 6) & 7;
}

void set_person_mod_chest_level(struct Thing *p_person, ubyte nmod)
{
    p_person->U.UPerson.UMod.Mods &= ~(7 << 6);
    p_person->U.UPerson.UMod.Mods |= (nmod << 6);
}

ubyte person_mod_legs_level(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods) & 7;
}

void set_person_mod_legs_level(struct Thing *p_person, ubyte nmod)
{
    p_person->U.UPerson.UMod.Mods &= ~(7);
    p_person->U.UPerson.UMod.Mods |= (nmod);
}

ubyte person_mod_arms_level(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods >> 3) & 7;
}

void set_person_mod_arms_level(struct Thing *p_person, ubyte nmod)
{
    p_person->U.UPerson.UMod.Mods &= ~(7 << 3);
    p_person->U.UPerson.UMod.Mods |= (nmod << 3);
}

ubyte person_mod_brain_level(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods >> 9) & 7;
}

void set_person_mod_brain_level(struct Thing *p_person, ubyte nmod)
{
    p_person->U.UPerson.UMod.Mods &= ~(7 << 9);
    p_person->U.UPerson.UMod.Mods |= (nmod << 9);
}

void person_give_best_mods(struct Thing *p_person)
{
    set_person_mod_legs_level(p_person, 3);
    set_person_mod_arms_level(p_person, 3);
    set_person_mod_brain_level(p_person, 3);
    set_person_mod_chest_level(p_person, 3);
}

short calc_person_speed(struct Thing *p_person)
{
    struct PeepStat *pstat;
    short mood;
    short speed;

    pstat = &peep_type_stats[p_person->SubType];

    mood = p_person->U.UPerson.Mood;
    if (mood < 0)
        mood = -mood;

    speed = pstat->Speed + 2 * mood + 100 * person_mod_legs_level(p_person);

    // Flamer slows you down? Why? This makes it even more useless..
    if (p_person->U.UPerson.CurrentWeapon == WEP_FLAMER)
        speed >>= 1;

    if (p_person->Flag & 0x80000)
        speed += 250;

    if (p_person->Flag2 & 0x80000)
        speed += 512;

    if (speed > PERSON_MAX_SPEED)
        speed = PERSON_MAX_SPEED;

    return speed;
}

void init_person_thing(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_init_person_thing\n"
        : : "a" (p_person));
#endif
    struct PeepStat *pstat;
    ushort paniframe;

    pstat = &peep_type_stats[p_person->SubType];

    p_person->U.UPerson.Energy = pstat->MaxEnergy +
        (person_mod_chest_level(p_person) * pstat->MaxEnergy * 50 / 100);

    p_person->U.UPerson.ShieldEnergy = pstat->MaxShield +
        (person_mod_brain_level(p_person) * pstat->MaxShield * 50 / 100);

    p_person->Health = pstat->MaxHealth +
       ((person_mod_chest_level(p_person) * pstat->MaxHealth * 50 / 100) +
        (person_mod_brain_level(p_person) * pstat->MaxHealth * 50 / 100) +
        (person_mod_legs_level(p_person) * pstat->MaxHealth * 25 / 100) +
        (person_mod_arms_level(p_person) * pstat->MaxHealth * 25 / 100)) / 4;

    p_person->U.UPerson.Stamina = pstat->MaximumStamina;
    p_person->U.UPerson.PersuadePower = 0;
    p_person->U.UPerson.MaxHealth = p_person->Health;
    p_person->U.UPerson.MaxEnergy = p_person->U.UPerson.Energy;
    p_person->U.UPerson.MaxShieldEnergy = p_person->U.UPerson.ShieldEnergy;
    p_person->U.UPerson.MaxStamina = p_person->U.UPerson.Stamina;

    if (person_mod_chest_level(p_person) == 4)
    {
        p_person->Health = 2 * PERSON_MAX_HEALTH_LIMIT;
        p_person->U.UPerson.MaxHealth = PERSON_MAX_HEALTH_LIMIT;
    }
    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_POLICE:
    case SubTT_PERS_PUNK_M:
        p_person->Radius = 80;
        break;
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_HIGH_PRIEST:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
        p_person->Radius = 100;
        break;
    case SubTT_PERS_MECH_SPIDER:
        p_person->Radius = 384;
        break;
    default:
        break;
    }
    if (current_level != 0)
        p_person->U.UPerson.CurrentWeapon = 0;
    if ((p_person->Flag & 0x0002) != 0)
    {
        p_person->U.UPerson.AnimMode = 20;
        p_person->State = PerSt_DEAD;
        p_person->U.UPerson.FrameId.Version[3] = 1;
        p_person->U.UPerson.FrameId.Version[4] = 0;
    }
    else
    {
        paniframe = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
        p_person->Frame -= nstart_ani[paniframe + p_person->U.UPerson.Angle];
        p_person->U.UPerson.AnimMode = 0;
        p_person->Frame += nstart_ani[paniframe + p_person->U.UPerson.Angle];
        p_person->StartFrame = paniframe - 1;
        p_person->Speed = calc_person_speed(p_person);
    }
}
/******************************************************************************/
