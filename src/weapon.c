/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file weapon.c
 *     Weapons related functions.
 * @par Purpose:
 *     Implement functions for handling weapons.
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
#include "weapon.h"
#include "thing.h"
/******************************************************************************/

void do_weapon_quantities1(struct Thing *p_person)
{
    asm volatile ("call ASM_do_weapon_quantities1\n"
        : : "a" (p_person));
}

void do_weapon_quantities_proper1(struct Thing *p_person)
{
    asm volatile ("call ASM_do_weapon_quantities_proper1\n"
        : : "a" (p_person));
}

/******************************************************************************/
