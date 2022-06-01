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

void do_weapon_quantities1(struct Thing *p_person)
{
    asm volatile ("call ASM_do_weapon_quantities1\n"
        : : "a" (p_person));
}

/******************************************************************************/
