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

ubyte person_mod_level_A(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods << 7) >> 13;
}

ubyte person_mod_level_B(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods & 7);
}

ubyte person_mod_level_C(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods << 10) >> 13;
}

ubyte person_mod_level_D(struct Thing *p_person)
{
    return (p_person->U.UPerson.UMod.Mods << 4) >> 13;
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

    speed = pstat->Speed + 2 * mood + 100 * person_mod_level_B(p_person);

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
    asm volatile ("call ASM_init_person_thing\n"
        : : "a" (p_person));
}
/******************************************************************************/
