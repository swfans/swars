/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehicle.c
 *     Routines implementing vehicle.
 * @par Purpose:
 *     Implement functions for handling vehicle things.
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
#include "vehicle.h"

#include "thing.h"
#include "swlog.h"
/******************************************************************************/

void init_mech(void)
{
    asm volatile ("call ASM_init_mech\n"
        :  :  : "eax" );
}

void mech_unkn_func_02(void)
{
    asm volatile ("call ASM_mech_unkn_func_02\n"
        :  :  : "eax" );
}

void mech_unkn_func_09(short thing)
{
    asm volatile ("call ASM_mech_unkn_func_09\n"
        : : "a" (thing));
}

void veh_add(struct Thing *p_thing, short frame)
{
    asm volatile ("call ASM_veh_add\n"
        : : "a" (p_thing), "d" (frame));
}

void VNAV_unkn_func_207(struct Thing *p_thing)
{
    asm volatile ("call ASM_VNAV_unkn_func_207\n"
        : : "a" (p_thing));
}

/******************************************************************************/
